"""
Step 21: Apply 2026 modern theme to Cheat Engine
- Patches .lfm files with modern color scheme
- Injects theme_engine into main forms
"""

import re
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")

# Catppuccin Mocha (Dark) colors in TColor format (0xBBGGRR)
DARK_BG      = "$2E1E1E"   # #1E1E2E
DARK_PANEL   = "$443131"   # #313244
DARK_INPUT   = "$585454"   # #45475A
DARK_TEXT    = "$F4D6CD"   # #CDD6F4
DARK_ACCENT  = "$FAB489"   # #89B4FA

def patch_lfm_colors(path: Path) -> bool:
    raw = path.read_text(encoding="utf-8", errors="ignore")
    new = raw
    
    # Replace common color values with dark theme
    # These are typical Lazarus default colors
    replacements = {
        "clDefault": DARK_BG,
        "clWindow": DARK_INPUT,
        "clBtnFace": DARK_PANEL,
        "clMenu": DARK_PANEL,
        "clHighlight": DARK_ACCENT,
        "clActiveCaption": DARK_PANEL,
        "clInactiveCaption": DARK_PANEL,
        "clBackground": DARK_BG,
        "cl3DLight": DARK_PANEL,
        "cl3DDkShadow": "$1E1E1E",
    }
    
    for old, new_c in replacements.items():
        new = new.replace(old, new_c)
    
    # Replace explicit RGB colors that look like old defaults
    # clWhite -> dark text
    new = new.replace("clWhite", DARK_TEXT)
    # clBlack -> dark text (inverted for dark mode)
    new = new.replace("clBlack", DARK_TEXT)
    
    if new != raw:
        path.write_text(new, encoding="utf-8")
        return True
    return False

def patch_mainunit_theme():
    """Add theme toggle to MainUnit"""
    target = ROOT / "Cheat Engine" / "MainUnit.pas"
    if not target.exists():
        return False
    raw = target.read_text(encoding="utf-8", errors="ignore")
    
    if "theme_engine" in raw:
        return False
    
    # Add theme_engine to uses
    m = re.search(r"\buses\b([^;]+);", raw, re.IGNORECASE)
    if m:
        start, end = m.span()
        raw = raw[:start] + raw[start:end].rstrip(" ;\n") + ", theme_engine;" + raw[end:]
    
    # Add theme init in FormCreate or similar
    # Find FormCreate and add SetTheme(atDark)
    raw = raw.replace(
        "procedure TMainForm.FormCreate(Sender: TObject);",
        "procedure TMainForm.FormCreate(Sender: TObject);\nbegin\n  SetTheme(atDark);\n  ApplyThemeToForm(Self);"
    )
    
    target.write_text(raw, encoding="utf-8")
    return True

def main():
    print("=== STEP 21: Modern Theme Application ===")
    n = 0
    for p in (ROOT / "Cheat Engine").rglob("*.lfm"):
        if patch_lfm_colors(p): n += 1
    print(f"lfm files themed: {n}")
    
    if patch_mainunit_theme():
        print("[PATCHED] MainUnit.pas theme integration")
    
    print("=== STEP 21 DONE ===")

if __name__ == "__main__":
    main()
