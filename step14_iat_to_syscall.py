"""
Step 14: Auto-replace Win32 API calls with direct syscalls in .pas files
Targets: OpenProcess, ReadProcessMemory, WriteProcessMemory, VirtualProtectEx, CreateRemoteThread
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

# Map: Win32 API -> syscall wrapper function
API_MAP = {
    "OpenProcess": "scOpenProcess",
    "ReadProcessMemory": "scReadProcessMemory",
    "WriteProcessMemory": "scWriteProcessMemory",
    "VirtualProtectEx": "scVirtualProtectEx",
    "NtQueryVirtualMemory": "scNtQueryVirtualMemory",
    "NtProtectVirtualMemory": "scNtProtectVirtualMemory",
}

# Files to patch (core memory manipulation units)
TARGET_FILES = [
    "Cheat Engine/CEFuncProc.pas",
    "Cheat Engine/NewKernelHandler.pas",
    "Cheat Engine/memscan.pas",
    "Cheat Engine/MemoryRecordUnit.pas",
    "Cheat Engine/ProcessHandlerUnit.pas",
    "Cheat Engine/debughelper.pas",
    "Cheat Engine/autoassembler.pas",
    "Cheat Engine/addresslist.pas",
    "Cheat Engine/byteinterpreter.pas",
    "Cheat Engine/CEDebugger.pas",
    "Cheat Engine/WindowsDebugger.pas",
    "Cheat Engine/VEHDebugger.pas",
    "Cheat Engine/KernelDebuggerInterface.pas",
    "Cheat Engine/networkdebuggerinterface.pas",
    "Cheat Engine/dbk32/DBK32functions.pas",
    "Cheat Engine/dbvmdebuggerinterface.pas",
    "Cheat Engine/dbvmPhysicalMemoryHandler.pas",
    "Cheat Engine/ManualModuleLoader.pas",
    "Cheat Engine/PEInfoFunctions.pas",
    "Cheat Engine/symbolhandler.pas",
    "Cheat Engine/pointerscancontroller.pas",
    "Cheat Engine/pointerscannerfrm.pas",
    "Cheat Engine/speedhack2.pas",
    "Cheat Engine/sharedMemory.pas",
    "Cheat Engine/formsettingsunit.pas",
]

def patch_file(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    if "syscalls" in raw:
        return False  # Already patched
    new = raw
    changed = False
    for old_api, new_api in API_MAP.items():
        # Word-boundary replace, but not in comments or strings
        # Simple approach: replace standalone function calls
        pat = re.compile(r"\b" + old_api + r"\b")
        if pat.search(new):
            new = pat.sub(new_api, new)
            changed = True
    if changed:
        # Add syscalls to uses clause
        m = re.search(r"\buses\b([^;]+);", new, re.IGNORECASE)
        if m:
            start, end = m.span()
            new = new[:start] + new[start:end].rstrip(" ;\n") + ", syscalls;" + new[end:]
        path.write_text(new, encoding="utf-8")
        return True
    return False

def main():
    print("=== STEP 14: IAT -> Syscall Auto-Replace ===")
    n = 0
    for rel in TARGET_FILES:
        p = ROOT / rel
        if p.exists() and patch_file(p):
            print(f"[PATCHED] {rel}")
            n += 1
    print(f"\nfiles patched: {n}")
    print("NOTE: Manual review recommended for parameter mismatches.")
    print("=== STEP 14 DONE ===")

if __name__ == "__main__":
    main()
