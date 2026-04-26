"""
Step 11: Wipe visible text fingerprints from .lfm files
- Caption, Hint, Text, Title properties that contain letters
"""

import os, re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def wipe_lfm(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    new = raw
    # Match Caption = '...', Hint = '...', Text = '...', Title = '...'
    for prop in ("Caption", "Hint", "Text", "Title"):
        pat = re.compile(rf"\b{prop}\s*=\s*'([^']{{3,}})'", re.IGNORECASE)
        def repl(m):
            s = m.group(1)
            if not any(c.isalpha() for c in s): return m.group(0)
            if "\\" in s or "/" in s: return m.group(0)
            return f"{prop} = ''"
        new = pat.sub(repl, new)
    if new != raw:
        path.write_text(new, encoding="utf-8")
        return True
    return False

def main():
    print("=== STEP 11: LFM Text Wipe ===")
    n = 0
    for p in (ROOT / "Cheat Engine").rglob("*.lfm"):
        if wipe_lfm(p): n += 1
    print(f"lfm files changed: {n}")
    print("=== STEP 11 DONE ===")

if __name__ == "__main__":
    main()
