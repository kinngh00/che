"""
Apply class name obfuscation from step5_form_mapping.json to all .pas/.lfm/.lpr/.lpi files.
This fixes inconsistencies caused by restore_xs_files.py reverting some .pas files to HEAD.
"""
import json
import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
CE_DIR = ROOT / "Cheat Engine"

with open(ROOT / "step5_form_mapping.json", "r", encoding="utf-8") as f:
    mapping = json.load(f)

class_map = mapping.get("class_map", {})

# Build reverse map as well (obfuscated -> original) just in case
reverse_map = {v: k for k, v in class_map.items()}

# Process all relevant files
for ext in ("*.pas", "*.lfm", "*.lpr", "*.lpi"):
    for p in CE_DIR.rglob(ext):
        content = p.read_text(encoding="utf-8", errors="ignore")
        new_content = content
        changed = False
        
        # Replace original class names with obfuscated names (case-insensitive)
        for orig, obf in sorted(class_map.items(), key=lambda x: len(x[0]), reverse=True):
            pattern = re.compile(r'\b' + re.escape(orig) + r'\b', re.IGNORECASE)
            if pattern.search(new_content):
                new_content = pattern.sub(obf, new_content)
                changed = True
        
        if changed:
            p.write_text(new_content, encoding="utf-8")
            print(f"REPLACED {p.relative_to(ROOT)}")

print("=== DONE ===")
