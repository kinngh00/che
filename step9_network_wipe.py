"""
Step 9: Network Fingerprint Wipe
- ceserver port / protocol strings
- lua network keywords
- cloud/checksum URLs
"""

import os, re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def wipe_in(path: Path, exts: set, mapping: dict) -> int:
    changed = 0
    for p in path.rglob("*"):
        if p.suffix.lower() not in exts: continue
        if "backup" in str(p).lower(): continue
        try:
            raw = p.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            continue
        new = raw
        for old, new_s in sorted(mapping.items(), key=lambda x: -len(x[0])):
            new = new.replace(old, new_s)
        if new != raw:
            p.write_text(new, encoding="utf-8")
            changed += 1
    return changed

def main():
    print("=== STEP 9: Network Fingerprint Wipe ===")
    net_map = {
        "ceserver": "nsrv_" + os.urandom(4).hex(),
        "52736": str(10000 + (int.from_bytes(os.urandom(2), 'big') % 55535)),
        "tcp": "tp_" + os.urandom(2).hex(),
        "CheatEngine": "CE_" + os.urandom(3).hex(),
        "cheatengine.org": "cdn." + os.urandom(4).hex() + ".net",
        "patreon.com/cheatengine": "p." + os.urandom(4).hex() + ".io",
    }
    c = wipe_in(ROOT / "Cheat Engine" / "ceserver", {".c", ".h", ".cpp", ".pas"}, net_map)
    l = wipe_in(ROOT / "lua", {".c", ".h", ".pas", ".lua"}, net_map)
    ce = wipe_in(ROOT / "Cheat Engine", {".pas", ".lpr", ".c", ".h"}, net_map)
    print(f"ceserver files: {c}")
    print(f"lua files: {l}")
    print(f"CE files: {ce}")
    print("=== STEP 9 DONE ===")

if __name__ == "__main__":
    main()
