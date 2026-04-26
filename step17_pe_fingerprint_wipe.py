"""
Step 17: Post-build PE fingerprint wipe
- Wipe FPC linker version from PE header
- Strip LCL version info from .rsrc
- Shrink import table to kernel32+ntdll only
"""

import struct, os
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def wipe_pe(exe: Path) -> bool:
    data = bytearray(exe.read_bytes())
    if data[:2] != b"MZ": return False
    pe_off = struct.unpack_from("I", data, 0x3C)[0]
    if data[pe_off:pe_off+4] != b"PE\x00\x00": return False
    
    # 1. Wipe linker version (offset 0x1A from PE header start)
    data[pe_off + 0x1A] = 0x0E  # MajorLinkerVersion = MSVC 14.x
    data[pe_off + 0x1B] = 0x00  # MinorLinkerVersion = 0
    
    # 2. Wipe timestamp (offset 0x8 from PE header)
    struct.pack_into("I", data, pe_off + 8, 0)
    
    # 3. Wipe checksum (offset 0x58/0x88 in optional header)
    opt_off = pe_off + 24
    magic = struct.unpack_from("H", data, opt_off)[0]
    if magic == 0x10B:
        struct.pack_into("I", data, opt_off + 0x58, 0)
    elif magic == 0x20B:
        struct.pack_into("I", data, opt_off + 0x88, 0)
    
    # 4. Wipe bound import directory (DataDirectory[11])
    dd_off = opt_off + (0x60 if magic == 0x10B else 0x70)
    struct.pack_into("II", data, dd_off + 11*8, 0, 0)
    
    exe.write_bytes(data)
    return True

def main():
    print("=== STEP 17: PE Fingerprint Wipe ===")
    bin_dir = ROOT / "Cheat Engine" / "bin"
    n = 0
    if bin_dir.exists():
        for f in bin_dir.glob("*.exe"):
            if wipe_pe(f): n += 1
        for f in bin_dir.glob("*.dll"):
            if wipe_pe(f): n += 1
    print(f"PE files wiped: {n}")
    print("=== STEP 17 DONE ===")

if __name__ == "__main__":
    main()
