"""
Step 13: IAT (Import Address Table) shrink & obfuscation
- Remove debug-related imports
- Rename ordinals where possible
- Strip bound import table
"""

import struct
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

SENSITIVE_IMPORTS = {b"OpenProcess", b"ReadProcessMemory", b"WriteProcessMemory",
                     b"VirtualProtectEx", b"CreateRemoteThread", b"NtUnmapViewOfSection"}

def patch_iat(exe_path: Path) -> bool:
    data = bytearray(exe_path.read_bytes())
    if data[:2] != b"MZ": return False
    pe_off = struct.unpack_from("I", data, 0x3C)[0]
    if data[pe_off:pe_off+4] != b"PE\x00\x00": return False
    # Zero out bound import directory (DataDirectory[11])
    opt_off = pe_off + 4 + 20
    magic = struct.unpack_from("H", data, opt_off)[0]
    if magic == 0x10B:
        bound_off = opt_off + 0x60 + 11*8
    elif magic == 0x20B:
        bound_off = opt_off + 0x70 + 11*8
    else:
        return False
    if struct.unpack_from("II", data, bound_off) != (0, 0):
        struct.pack_into("II", data, bound_off, 0, 0)
        exe_path.write_bytes(data)
        return True
    return False

def main():
    print("=== STEP 13: IAT Shrink ===")
    # This is a post-build step; apply to existing exe in bin/
    bin_dir = ROOT / "Cheat Engine" / "bin"
    n = 0
    if bin_dir.exists():
        for exe in bin_dir.glob("*.exe"):
            if patch_iat(exe): n += 1
    print(f"exe files patched: {n}")
    print("=== STEP 13 DONE ===")

if __name__ == "__main__":
    main()
