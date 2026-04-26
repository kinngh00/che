"""
Fix Pascal char constants: #XX (hex without $) → #$XX
FPC requires #$ prefix for hex char constants.
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def fix_file(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    # Match # followed by 2 hex chars where at least one is A-F
    # But not already #$xx
    new = re.sub(r'#([A-Fa-f][0-9A-Fa-f])', r'#$\1', raw)
    if new != raw:
        path.write_text(new, encoding="utf-8")
        return True
    return False

def main():
    print("=== FIX HEX CHAR CONSTANTS ===")
    n = 0
    f = 0
    for p in (ROOT / "Cheat Engine").rglob("*.pas"):
        if fix_file(p): 
            n += 1
            f += 1
    for p in (ROOT / "Cheat Engine").rglob("*.lpr"):
        if fix_file(p): 
            n += 1
            f += 1
    print(f"files changed: {n}")
    print("=== DONE ===")

if __name__ == "__main__":
    main()
