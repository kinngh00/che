"""
Step 16: dbvm Hypervisor Stealth
- Patch VMX-related strings and CPUID responses
- Add VMX MSR hiding comments/flags
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def patch_vmx_strings():
    """Replace VMX-related identifiers in dbvm source"""
    n = 0
    for p in (ROOT / "dbvm").rglob("*.c"):
        raw = p.read_text(encoding="utf-8", errors="ignore")
        new = raw
        # Replace VMX-specific debug strings with generic ones
        new = re.sub(r'"DBVM', '"HV_', new)
        new = re.sub(r'"dbvm', '"hv_', new, flags=re.IGNORECASE)
        if new != raw:
            p.write_text(new, encoding="utf-8")
            n += 1
    return n

def patch_cpuid_handler():
    """Add CPUID 0x40000000 (hypervisor leaf) spoofing comment"""
    target = ROOT / "dbvm" / "vmm" / "vmeventhandler.c"
    if not target.exists():
        return False
    raw = target.read_text(encoding="utf-8", errors="ignore")
    if "HIDE_HYPERVISOR" in raw:
        return False
    # Add comment marker for manual implementation
    new = raw.replace(
        "void handleVMEvent",
        "// HIDE_HYPERVISOR: Spoof CPUID 0x40000000 leaf to hide VM presence\n"
        "// TODO: Return eax=0, ebx/ecx/edx = random vendor string\n"
        "void handleVMEvent"
    )
    target.write_text(new, encoding="utf-8")
    return True

def main():
    print("=== STEP 16: dbvm Hypervisor Stealth ===")
    n = patch_vmx_strings()
    h = patch_cpuid_handler()
    print(f"dbvm .c files patched: {n}")
    print(f"CPUID handler marked: {h}")
    print("NOTE: Full HV stealth requires manual CPUID/MSR spoofing in vmeventhandler.")
    print("=== STEP 16 DONE ===")

if __name__ == "__main__":
    main()
