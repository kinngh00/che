"""
Fix remaining hex char constants that were missed.
#xx followed by ) or ] or other non-hex chars
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def hex_to_dec(match):
    val = int(match.group(1), 16)
    return f'#{val}'

def fix_file(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    # Match #xx where xx is 2 hex chars followed by non-hex char (but not already decimal)
    # Avoid #123 (3+ digits) and #$xx (already fixed)
    new = re.sub(r'#([0-9A-Fa-f]{2})(?=[^0-9A-Fa-f])', hex_to_dec, raw)
    if new != raw:
        path.write_text(new, encoding="utf-8")
        return True
    return False

def main():
    print("=== FIX LAST HEX CONSTANTS ===")
    n = 0
    for p in (ROOT / "Cheat Engine").rglob("*.pas"):
        if fix_file(p): n += 1
    for p in (ROOT / "Cheat Engine").rglob("*.lpr"):
        if fix_file(p): n += 1
    print(f"files changed: {n}")
    print("=== DONE ===")

if __name__ == "__main__":
    main()
