"""
Step 12: Wipe fingerprints from .lua scripts
"""

import os
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def wipe_lua(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    new = raw
    for old in ["cheatengine", "CheatEngine", "cheat engine", "cheat-engine"]:
        new = new.replace(old, "ce_" + os.urandom(3).hex())
    if new != raw:
        path.write_text(new, encoding="utf-8")
        return True
    return False

def main():
    print("=== STEP 12: Lua Wipe ===")
    n = 0
    for p in (ROOT / "Cheat Engine").rglob("*.lua"):
        if wipe_lua(p): n += 1
    for p in (ROOT / "lua").rglob("*.lua"):
        if wipe_lua(p): n += 1
    print(f"lua files changed: {n}")
    print("=== STEP 12 DONE ===")

if __name__ == "__main__":
    main()
