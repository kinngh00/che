"""
Step 10: Inject Anti-VM checks into main entry points
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

def inject_lpr(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    if "anti_vm" in raw: return False
    # Add anti_vm to uses
    m = re.search(r"\buses\b([^;]+);", raw, re.IGNORECASE)
    if m:
        start, end = m.span()
        raw = raw[:start] + raw[start:end].rstrip(" ;\n") + ", anti_vm;" + raw[end:]
    # Add check after Application.Initialize
    raw = raw.replace(
        "Application.Initialize;",
        "Application.Initialize;\n  if IsSandboxed or IsVM then Halt(0);"
    )
    path.write_text(raw, encoding="utf-8")
    return True

def main():
    print("=== STEP 10: Anti-VM Injection ===")
    targets = [
        ROOT / "Cheat Engine" / "cheatengine.lpr",
        ROOT / "Cheat Engine" / "cecore.lpr",
    ]
    for t in targets:
        if t.exists() and inject_lpr(t):
            print(f"[INJECTED] {t}")
    print("=== STEP 10 DONE ===")

if __name__ == "__main__":
    main()
