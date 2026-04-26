"""
Convert all 2-char hex char constants to decimal.
#xx (hex) → #decimal
#$xx (hex) → #decimal
This makes all constants FPC-safe.
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def hex_to_dec(match):
    val = int(match.group(1), 16)
    return f'#{val}'

def fix_file(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    # Match #$xx followed by # (next hex char)
    new = re.sub(r'#\$([0-9A-Fa-f]{2})(?=#)', hex_to_dec, raw)
    # Match #xx (2 hex chars without $) followed by #, but NOT already processed
    # Avoid matching #123 (3+ digit decimal)
    new = re.sub(r'#([0-9A-Fa-f]{2})(?=#)', hex_to_dec, new)
    if new != raw:
        path.write_text(new, encoding="utf-8")
        return True
    return False

def main():
    print("=== FIX HEX TO DECIMAL ===")
    n = 0
    for p in (ROOT / "Cheat Engine").rglob("*.pas"):
        if fix_file(p): n += 1
    for p in (ROOT / "Cheat Engine").rglob("*.lpr"):
        if fix_file(p): n += 1
    print(f"files changed: {n}")
    print("=== DONE ===")

if __name__ == "__main__":
    main()
