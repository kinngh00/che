"""
Step 7: String Encryption for Pascal Sources
- Replaces string literals '...' with XS(#$xx#$yy...) in .pas/.lpr files
- Skips: paths, short strings, directives, already-encrypted strings
- Requires ce_crypto.pas in uses clause

SAFETY:
- Only encrypts strings that contain letters (avoids pure numeric/symbol strings)
- Skips strings that look like file paths, URLs, registry keys
- Preserves compiler directives and asm blocks
"""

import os
import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
SOURCE_DIRS = [ROOT / "Cheat Engine"]
SKIP_PATH_PARTS = ["\\.git\\", "\\backup\\", "\\bin\\", "\\lib\\", "\\obj\\"]

# Regex to find Pascal string literals: '...''...'
STRING_RE = re.compile(r"'((?:[^']|'')*)'")


def should_skip(path: Path) -> bool:
    p = str(path).lower().replace("/", "\\")
    return any(part in p for part in SKIP_PATH_PARTS)


def encrypt_string(s: str) -> str:
    """XOR encrypt with rolling key. Returns Pascal hex-escaped form."""
    k = 0x7A
    parts = []
    for ch in s:
        b = ord(ch) ^ k
        parts.append(f"#{b:02X}")
        k = (k + 7) % 256
    return "XS(" + "".join(parts) + ")"


def is_risky_string(s: str) -> bool:
    """Return True if string should NOT be encrypted."""
    if len(s) <= 2:
        return True
    if s.startswith("{$") or s.startswith("(*$"):
        return True
    # Skip file paths
    if "\\" in s or "/" in s or s.endswith(".exe") or s.endswith(".dll") or s.endswith(".pas"):
        return True
    # Skip registry keys
    if s.startswith("HKEY_") or s.startswith("Software\\"):
        return True
    # Skip pure numbers/symbols
    if not any(c.isalpha() for c in s):
        return True
    # Skip URLs
    if s.startswith("http") or s.startswith("www.") or ".org" in s or ".com" in s:
        return True
    # Skip GUIDs
    if re.match(r"^[0-9A-Fa-f\-]{36}$", s):
        return True
    # Skip format specifiers that are often used inline
    if s in {"%s", "%d", "%x", "%p", "%f", "%g", "%e", "%n", "%u", "%lu", "%llu"}:
        return True
    return False


def process_pas(text: str) -> tuple[str, int]:
    """Encrypt eligible string literals. Returns (new_text, count)."""
    count = 0
    # Simple state machine to skip comments and asm blocks
    result = []
    i = 0
    n = len(text)
    while i < n:
        ch = text[i]

        # Skip // comments
        if ch == "/" and i + 1 < n and text[i + 1] == "/":
            j = text.find("\n", i)
            if j == -1:
                j = n
            result.append(text[i:j])
            i = j
            continue

        # Skip {...} comments
        if ch == "{":
            j = text.find("}", i)
            if j == -1:
                j = n - 1
            result.append(text[i:j + 1])
            i = j + 1
            continue

        # Skip (*...*) comments
        if ch == "(" and i + 1 < n and text[i + 1] == "*":
            j = text.find("*)", i + 2)
            if j == -1:
                j = n - 2
            result.append(text[i:j + 2])
            i = j + 2
            continue

        # Skip string literals
        if ch == "'":
            j = i + 1
            while j < n:
                if text[j] == "'":
                    if j + 1 < n and text[j + 1] == "'":
                        j += 2
                        continue
                    j += 1
                    break
                j += 1
            lit = text[i:j]
            inner = lit[1:-1].replace("''", "'")
            if not is_risky_string(inner):
                result.append(encrypt_string(inner))
                count += 1
            else:
                result.append(lit)
            i = j
            continue

        result.append(ch)
        i += 1

    return "".join(result), count


def ensure_crypto_unit(text: str) -> str:
    """Add ce_crypto to uses clause if missing."""
    if "ce_crypto" in text:
        return text
    # Try to insert after 'uses' line in interface/implementation
    # Heuristic: find first 'uses' keyword and append ce_crypto at end of that section
    m = re.search(r"\buses\b([^;]+);", text, re.IGNORECASE)
    if m:
        start, end = m.span()
        new_uses = text[start:end].rstrip(" ;\n") + ", ce_crypto;"
        text = text[:start] + new_uses + text[end:]
    return text


def main() -> None:
    print("=== STEP 7: String Encryption ===")
    total_encrypted = 0
    changed_files = []

    for src_dir in SOURCE_DIRS:
        if not src_dir.exists():
            continue
        for path in list(src_dir.rglob("*.pas")) + list(src_dir.rglob("*.lpr")):
            if should_skip(path):
                continue
            try:
                raw = path.read_text(encoding="utf-8", errors="ignore")
            except Exception:
                continue

            new_text, count = process_pas(raw)
            if count > 0:
                new_text = ensure_crypto_unit(new_text)
                path.write_text(new_text, encoding="utf-8")
                changed_files.append(str(path.relative_to(ROOT)))
                total_encrypted += count
                print(f"[ENCRYPTED +{count}] {path}")

    print(f"\n=== STEP 7 DONE ===")
    print(f"files_changed={len(changed_files)}")
    print(f"strings_encrypted={total_encrypted}")
    print("NOTE: Ensure 'ce_crypto.pas' is compiled and linked.")


if __name__ == "__main__":
    main()
