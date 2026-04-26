"""
Restore any .pas/.lpr files that still contain XS(# back to Git HEAD.
"""
import subprocess
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
CE_DIR = ROOT / "Cheat Engine"

def main():
    files_to_restore = []
    for ext in ("*.pas", "*.lpr"):
        for p in CE_DIR.rglob(ext):
            content = p.read_text(encoding="utf-8", errors="ignore")
            if "XS(#" in content:
                rel = p.relative_to(ROOT).as_posix()
                files_to_restore.append(rel)
    
    print(f"=== FOUND {len(files_to_restore)} FILES WITH XS(# ===")
    for f in files_to_restore:
        print(f"  RESTORE {f}")
        subprocess.run(["git", "checkout", "HEAD", "--", f], cwd=ROOT, check=True)
    print("=== DONE ===")

if __name__ == "__main__":
    main()
