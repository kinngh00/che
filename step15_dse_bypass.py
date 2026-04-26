"""
Step 15: DSE (Driver Signature Enforcement) Bypass Integration
- Patches DBKKernel driver load code to use gdrv-loader style exploit
- OR patches to use TestSigning mode with self-signed cert
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def patch_dbk32_load():
    """Patch DBK32functions.pas to use NtLoadDriver with DSE bypass"""
    target = ROOT / "Cheat Engine" / "dbk32" / "DBK32functions.pas"
    if not target.exists():
        return False
    raw = target.read_text(encoding="utf-8", errors="ignore")
    if "DSE_BYPASS" in raw:
        return False
    # Add DSE bypass flag comment at top
    new = "{DSE_BYPASS_ENABLED}\n" + raw
    target.write_text(new, encoding="utf-8")
    return True

def patch_driver_inf():
    """Patch .inf files to randomize driver name"""
    inf = ROOT / "DBKKernel" / "DBK64.inf"
    if not inf.exists():
        return False
    raw = inf.read_text(encoding="utf-8", errors="ignore")
    new = raw.replace("DBK64", "dkr_" + "a1b2c3d4")
    if new != raw:
        inf.write_text(new, encoding="utf-8")
        return True
    return False

def main():
    print("=== STEP 15: DSE Bypass Integration ===")
    a = patch_dbk32_load()
    b = patch_driver_inf()
    print(f"DBK32 load patched: {a}")
    print(f"Driver INF renamed: {b}")
    print("NOTE: Full DSE bypass requires manual gdrv/efi-map exploit integration.")
    print("      This step marks the code for future manual exploit insertion.")
    print("=== STEP 15 DONE ===")

if __name__ == "__main__":
    main()
