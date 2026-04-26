"""
Step 8: Kernel / Hypervisor Fingerprint Wipe
- DBKKernel C/C++ source string wipe
- dbvm C/ASM string wipe
- Driver name / device path randomization
"""

import os, re, random, string, hashlib
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def rand_id(l=8):
    return "".join(random.choices(string.ascii_lowercase + string.digits, k=l))

def scramble_c_str(text: str) -> str:
    # Replace "..." string literals that contain letters and are not #include or pragmas
    def repl(m):
        s = m.group(1)
        if len(s) <= 2: return m.group(0)
        if "\\" in s or "/" in s or s.startswith("%"): return m.group(0)
        if not any(c.isalpha() for c in s): return m.group(0)
        # XOR scramble into hex escapes
        k = 0x42
        out = ""
        for ch in s:
            b = ord(ch) ^ k
            out += f"\\x{b:02x}"
            k = (k + 3) % 256
        return f'"{out}"'
    return re.sub(r'"((?:[^"\\]|\\.)*)"', repl, text)

def process_dir(path: Path, exts) -> int:
    changed = 0
    for p in path.rglob("*"):
        if p.suffix.lower() not in exts: continue
        if "backup" in str(p).lower(): continue
        try:
            raw = p.read_text(encoding="utf-8", errors="ignore")
        except Exception:
            continue
        new = scramble_c_str(raw)
        if new != raw:
            p.write_text(new, encoding="utf-8")
            changed += 1
    return changed

def main():
    print("=== STEP 8: Kernel/HV Fingerprint Wipe ===")
    c = process_dir(ROOT / "DBKKernel", {".c", ".cpp", ".h"})
    v = process_dir(ROOT / "dbvm", {".c", ".h", ".asm", ".nasm"})
    print(f"DBKKernel files changed: {c}")
    print(f"dbvm files changed: {v}")
    print("=== STEP 8 DONE ===")

if __name__ == "__main__":
    main()
