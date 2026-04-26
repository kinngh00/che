"""
Step 6: PE Mutator (Post-Build Binary Obfuscation)
- Section name randomization
- PE timestamp forgery
- CheckSum recalculation
- VERSIONINFO removal via Win32 UpdateResource API
- Debug directory stripping
- DOS stub replacement
- Output filename randomization
"""

import os
import re
import struct
import random
import string
import hashlib
import ctypes
from ctypes import wintypes
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

kernel32 = ctypes.windll.kernel32
imagehlp = ctypes.windll.imagehlp

BeginUpdateResourceW = kernel32.BeginUpdateResourceW
BeginUpdateResourceW.argtypes = [wintypes.LPCWSTR, wintypes.BOOL]
BeginUpdateResourceW.restype = wintypes.HANDLE

UpdateResourceW = kernel32.UpdateResourceW
UpdateResourceW.argtypes = [
    wintypes.HANDLE, wintypes.LPCWSTR, wintypes.LPCWSTR,
    wintypes.WORD, wintypes.LPVOID, wintypes.DWORD
]
UpdateResourceW.restype = wintypes.BOOL

EndUpdateResourceW = kernel32.EndUpdateResourceW
EndUpdateResourceW.argtypes = [wintypes.HANDLE, wintypes.BOOL]
EndUpdateResourceW.restype = wintypes.BOOL

MapFileAndCheckSumA = imagehlp.MapFileAndCheckSumA
MapFileAndCheckSumA.argtypes = [
    wintypes.LPCSTR, ctypes.POINTER(wintypes.DWORD), ctypes.POINTER(wintypes.DWORD)
]
MapFileAndCheckSumA.restype = wintypes.DWORD


def random_section_name(length: int = 8) -> bytes:
    name = "".join(random.choices(string.ascii_lowercase + string.digits, k=length))
    return name.encode("ascii").ljust(8, b"\x00")


def read_dos_header(data: bytes) -> int:
    if data[:2] != b"MZ":
        raise ValueError("Not a valid PE file (missing MZ)")
    return struct.unpack_from("<I", data, 0x3C)[0]


def parse_coff_header(data: bytes, pe_offset: int) -> tuple:
    coff = pe_offset + 4
    return struct.unpack_from("<HHIIIHH", data, coff)


def patch_pe_timestamp(data: bytearray, pe_offset: int, fake_ts: int) -> None:
    coff = pe_offset + 4
    struct.pack_into("<I", data, coff + 4, fake_ts)


def patch_section_names(data: bytearray, pe_offset: int, num_sections: int) -> None:
    coff = pe_offset + 4
    optional_header_size = struct.unpack_from("<H", data, coff + 16)[0]
    section_table = coff + 20 + optional_header_size
    FPC_SECTIONS = {b"CODE", b"DATA", b".rsrc", b".reloc", b".text", b".data"}
    for i in range(num_sections):
        sec_name_offset = section_table + i * 40
        sec_name = data[sec_name_offset:sec_name_offset + 8].rstrip(b"\x00")
        if sec_name in FPC_SECTIONS or sec_name.startswith(b"."):
            data[sec_name_offset:sec_name_offset + 8] = random_section_name()


def patch_dos_stub(data: bytearray, pe_offset: int) -> None:
    stub_end = pe_offset
    if stub_end > 0x40:
        msg = b"This program must be run under Win32\r\n$"
        start = 0x40
        data[start:start + len(msg)] = msg
        for i in range(start + len(msg), stub_end):
            data[i] = 0


def recalc_checksum(exe_path: str) -> int:
    header_sum = wintypes.DWORD()
    check_sum = wintypes.DWORD()
    ret = MapFileAndCheckSumA(exe_path.encode(), ctypes.byref(header_sum), ctypes.byref(check_sum))
    if ret != 0:
        print(f"[WARN] CheckSum calculation failed, error={ret}")
        return 0
    return check_sum.value


def write_checksum(data: bytearray, pe_offset: int, new_checksum: int) -> None:
    coff = pe_offset + 4
    optional_header_size = struct.unpack_from("<H", data, coff + 16)[0]
    if optional_header_size == 0:
        return
    optional_start = coff + 20
    magic = struct.unpack_from("<H", data, optional_start)[0]
    if magic not in (0x10B, 0x20B):
        return
    struct.pack_into("<I", data, optional_start + 0x40, new_checksum)


def strip_resources_winapi(exe_path: Path) -> None:
    path_str = str(exe_path.resolve())
    hUpdate = BeginUpdateResourceW(path_str, False)
    if not hUpdate:
        raise RuntimeError("BeginUpdateResource failed")
    try:
        if not UpdateResourceW(hUpdate, ctypes.c_wchar_p("#16"), ctypes.c_wchar_p("#1"), 1033, None, 0):
            print(f"[WARN] Failed to remove VERSIONINFO, err={ctypes.GetLastError()}")
        for rid in range(1, 11):
            UpdateResourceW(hUpdate, ctypes.c_wchar_p("#14"), ctypes.c_wchar_p(f"#{rid}"), 1033, None, 0)
            UpdateResourceW(hUpdate, ctypes.c_wchar_p("#3"), ctypes.c_wchar_p(f"#{rid}"), 1033, None, 0)
    finally:
        if not EndUpdateResourceW(hUpdate, False):
            print(f"[WARN] EndUpdateResource failed, err={ctypes.GetLastError()}")
        else:
            print(f"[RESOURCE] Stripped VERSIONINFO/icons from {exe_path.name}")


def strip_debug_directory(data: bytearray, pe_offset: int) -> bool:
    coff = pe_offset + 4
    optional_header_size = struct.unpack_from("<H", data, coff + 16)[0]
    if optional_header_size == 0:
        return False
    optional_start = coff + 20
    magic = struct.unpack_from("<H", data, optional_start)[0]
    if magic == 0x10B:
        num_rva = struct.unpack_from("<I", data, optional_start + 0x5C)[0]
        dd_start = optional_start + 0x60
    elif magic == 0x20B:
        num_rva = struct.unpack_from("<I", data, optional_start + 0x6C)[0]
        dd_start = optional_start + 0x70
    else:
        return False
    debug_offset = dd_start + 6 * 8
    if num_rva > 6:
        if struct.unpack_from("<II", data, debug_offset) != (0, 0):
            struct.pack_into("<II", data, debug_offset, 0, 0)
            return True
    return False


def mutate_pe(input_path: Path, output_path: Path) -> None:
    print(f"\n[MUTATE] {input_path.name} -> {output_path.name}")
    data = bytearray(input_path.read_bytes())
    pe_offset = read_dos_header(data)
    machine, num_sections, timestamp, sym_tab, num_sym, opt_hdr_size, characteristics = parse_coff_header(data, pe_offset)

    fake_ts = random.randint(1546300800, 1672531199)
    patch_pe_timestamp(data, pe_offset, fake_ts)
    print(f"  [TIMESTAMP] {fake_ts}")

    patch_section_names(data, pe_offset, num_sections)
    print(f"  [SECTIONS] Renamed {num_sections} sections")

    patch_dos_stub(data, pe_offset)
    print(f"  [DOSSTUB] Replaced")

    if strip_debug_directory(data, pe_offset):
        print(f"  [DEBUG] Stripped debug directory")

    output_path.write_bytes(data)
    strip_resources_winapi(output_path)

    new_checksum = recalc_checksum(str(output_path))
    if new_checksum:
        data2 = bytearray(output_path.read_bytes())
        pe_offset2 = read_dos_header(data2)
        write_checksum(data2, pe_offset2, new_checksum)
        output_path.write_bytes(data2)
        print(f"  [CHECKSUM] Recalculated = 0x{new_checksum:08X}")

    print(f"  [DONE] Output: {output_path}")


def main() -> None:
    import argparse
    parser = argparse.ArgumentParser(description="CE PE Mutator")
    parser.add_argument("--input", type=Path, required=True, help="Input PE file")
    parser.add_argument("--output", type=Path, default=None, help="Output PE file")
    args = parser.parse_args()

    if not args.input.exists():
        print(f"[ERROR] Input not found: {args.input}")
        return

    if args.output is None:
        rand_name = "app_" + hashlib.sha256(os.urandom(32)).hexdigest()[:12] + ".exe"
        args.output = args.input.parent / rand_name

    mutate_pe(args.input, args.output)


if __name__ == "__main__":
    main()
