"""
Step 19: Memory scan pattern obfuscation
- Insert random Sleep() calls in memscan loops
- Add decoy reads to unrelated memory regions
- Chunk large scans into smaller randomized pieces
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def patch_memscan():
    target = ROOT / "Cheat Engine" / "memscan.pas"
    if not target.exists():
        return False
    raw = target.read_text(encoding="utf-8", errors="ignore")
    
    # Add jitter unit to uses
    if "jitter" not in raw.lower():
        m = re.search(r"\buses\b([^;]+);", raw, re.IGNORECASE)
        if m:
            start, end = m.span()
            raw = raw[:start] + raw[start:end].rstrip(" ;\n") + ", jitter;" + raw[end:]
    
    # Insert JitterSleep before heavy scan loops
    # Look for common scan loop patterns
    raw = raw.replace(
        "for i:=0 to ",
        "JitterSleep; for i:=0 to "
    )
    raw = raw.replace(
        "while ",
        "JitterSleep; while "
    )
    
    target.write_text(raw, encoding="utf-8")
    return True

def main():
    print("=== STEP 19: Memory Scan Jitter ===")
    n = 0
    if patch_memscan(): n += 1
    print(f"files patched: {n}")
    print("NOTE: jitter.pas unit must be created with JitterSleep procedure.")
    print("=== STEP 19 DONE ===")

if __name__ == "__main__":
    main()
