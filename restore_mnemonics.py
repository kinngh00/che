"""
Restore mnemonic string literals in opcode tables from Git HEAD.
Replaces mnemonic:XS(...) with mnemonic:'ORIGINAL' using the original file.
"""
import subprocess
import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
FILES = [
    "Cheat Engine/Assemblerunit.pas",
    "Cheat Engine/disassemblerarm32.pas",
    "Cheat Engine/disassemblerarm32thumb.pas",
    "Cheat Engine/disassemblerarm64.pas",
]

def get_original(file_path: str):
    cmd = ["git", "show", f"HEAD:{file_path}"]
    return subprocess.check_output(cmd, cwd=ROOT, text=True)

def restore_file(file_path: str):
    current_path = ROOT / file_path
    current = current_path.read_text(encoding="utf-8", errors="ignore")
    original = get_original(file_path)
    
    # Extract original mnemonic strings in order
    orig_mnemonics = re.findall(r"mnemonic:'((?:[^']|'')*)'", original)
    
    # Find XS(...) occurrences in current file's mnemonic fields
    pattern = re.compile(r"mnemonic:XS\([^)]+\)")
    matches = list(pattern.finditer(current))
    
    if len(matches) != len(orig_mnemonics):
        print(f"  MISMATCH in {file_path}: {len(matches)} XS calls vs {len(orig_mnemonics)} originals")
        return False
    
    # Replace from end to start to preserve positions
    result = current
    for m, orig in zip(reversed(matches), reversed(orig_mnemonics)):
        # Escape single quotes in original for Pascal
        escaped = orig.replace("'", "''")
        replacement = f"mnemonic:'{escaped}'"
        result = result[:m.start()] + replacement + result[m.end():]
    
    current_path.write_text(result, encoding="utf-8")
    print(f"  RESTORED {len(matches)} mnemonics in {file_path}")
    return True

def main():
    print("=== RESTORE MNEMONICS FROM GIT HEAD ===")
    for f in FILES:
        if (ROOT / f).exists():
            restore_file(f)
        else:
            print(f"  SKIP (not found): {f}")
    print("=== DONE ===")

if __name__ == "__main__":
    main()
