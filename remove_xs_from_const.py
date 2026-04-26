"""
Remove XS() calls from const/resourcestring sections by decrypting back to literals.
XS(#...) → 'string' (Pascal const/resourcestring compatible)
"""
import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
SOURCE_DIRS = [ROOT / "Cheat Engine"]

# Match XS(#...#...#...)
XS_RE = re.compile(r'XS\((#[0-9A-Fa-f$]+(?:#[0-9A-Fa-f$]+)*)\)')

def parse_pascal_chars(s: str) -> list:
    parts = s.split('#')
    result = []
    for p in parts:
        if not p:
            continue
        if p.startswith('$'):
            result.append(int(p[1:], 16))
        else:
            result.append(int(p))
    return result

def xs_decrypt(data: list) -> str:
    k = 0x7A
    result = []
    for b in data:
        result.append(chr(b ^ k))
        k = (k + 7) % 256
    return ''.join(result)

def is_printable(s: str) -> bool:
    return all(32 <= ord(c) <= 126 for c in s)

def replace_xs_in_file(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    changed = False
    
    def replacer(m):
        nonlocal changed
        chars_str = m.group(1)
        try:
            data = parse_pascal_chars(chars_str)
            decrypted = xs_decrypt(data)
            if is_printable(decrypted):
                escaped = decrypted.replace("'", "''")
                changed = True
                return f"'{escaped}'"
            else:
                return m.group(0)
        except Exception:
            return m.group(0)
    
    new = XS_RE.sub(replacer, raw)
    if changed:
        path.write_text(new, encoding="utf-8")
        print(f"  FIXED {path}")
        return True
    return False

def main():
    print("=== REMOVE XS() FROM CONST / RESOURCESTRING ===")
    n = 0
    for d in SOURCE_DIRS:
        for p in d.rglob("*.pas"):
            if replace_xs_in_file(p):
                n += 1
        for p in d.rglob("*.lpr"):
            if replace_xs_in_file(p):
                n += 1
    print(f"=== DONE ({n} files changed) ===")

if __name__ == "__main__":
    main()
