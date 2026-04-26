"""
Step 5: Source Fingerprint Wipe
- Form class/instance name obfuscation (4-way sync across .lfm/.pas/.lpr/.lpi)
- LPI version info wipe (ProductVersion/Major/Minor/Build -> 0)
- Icon replacement (cheatengine.ico -> blank)
- LRS resource cleanup

SAFETY RULES (compile-breakage prevention):
1. Only rename classes that inherit from TForm/TFrame/TDataModule AND are project-defined.
2. Standard LCL classes (TForm, TButton, TEdit, TLabel...) are NEVER touched.
3. Instance names and class names are renamed together atomically via a mapping dict.
4. Pascal identifiers are matched with word-boundaries to avoid partial replacements.
"""

import os
import re
import json
import hashlib
import random
import string
from pathlib import Path

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
OUT_MAPPING = ROOT / "step5_form_mapping.json"

SOURCE_DIRS = [
    ROOT / "Cheat Engine",
    ROOT / "DBKKernel",
    ROOT / "dbvm",
    ROOT / "DBVM UEFI",
    ROOT / "lua",
]

SKIP_PATH_PARTS = [
    "\\.git\\", "\\.idea\\", "\\bin\\", "\\lib\\", "\\obj\\",
    "\\x86_64", "\\i386", "\\__history", "\\debug", "\\release",
    "\\backup\\", "\\.github\\",
]

PROTECTED_CLASSES = {
    "TForm", "TFrame", "TDataModule", "TComponent", "TObject",
    "TButton", "TEdit", "TLabel", "TMemo", "TListBox", "TComboBox",
    "TCheckBox", "TRadioButton", "TGroupBox", "TPanel", "TPageControl",
    "TTabSheet", "TTreeView", "TListView", "TImageList", "TImage",
    "TMainMenu", "TPopupMenu", "TMenuItem", "TAction", "TActionList",
    "TOpenDialog", "TSaveDialog", "TFindDialog", "TColorDialog",
    "TTimer", "TIdleTimer", "TProgressBar", "TStatusBar", "TToolBar",
    "TSpeedButton", "TBitBtn", "TScrollBar", "TScrollBox", "TSplitter",
    "TStringGrid", "TDrawGrid", "TValueListEditor", "TDateTimePicker",
    "TTrackBar", "TUpDown", "TSpinEdit", "TFontDialog", "TPrinterSetupDialog",
    "TReplaceDialog", "TPageSetupDialog", "TPrintDialog",
    "TApplication", "TScreen", "TCanvas", "TBitmap", "TIcon", "TPicture",
    "TWinControl", "TControl", "TGraphicControl", "TCustomControl",
    "TForm1", "TForm2", "TForm3", "TForm4", "TForm5", "TForm6", "TForm7", "TForm8", "TForm9", "TForm10",
    "TNewForm", "TNewButton", "TNewEdit", "TNewLabel", "TNewListView",
    "TNewCheckbox", "TNewCheckGroup", "TNewCheckListBox", "TNewColorBox",
    "TNewComboBox", "TNewDirectoryEdit", "TNewGroupBox", "TNewHeaderControl",
    "TNewHintWindow", "TNewListBox", "TNewMainMenu", "TNewMemo",
    "TNewPageControl", "TNewProgressBar", "TNewRadioButton", "TNewScrollbar",
    "TNewScrollBox", "TNewSpeedButton", "TNewStatusBar", "TNewSynEdit",
    "TNewTabControl", "TNewToggleBox", "TNewTreeView", "TNewVirtualStringTree",
}


def should_skip(path: Path) -> bool:
    p = str(path).lower().replace("/", "\\")
    for part in SKIP_PATH_PARTS:
        if part in p:
            return True
    return False


def rand_suffix(length: int = 8) -> str:
    return "".join(random.choices(string.ascii_lowercase + string.digits, k=length))


# Module-level stable salt: evaluated ONCE at import time.
_OBF_SALT_BASE = rand_suffix(16)


def stable_obf_token(prefix: str, seed: str, length: int = 8) -> str:
    """Deterministic per-seed, but salt-randomized per run."""
    salt = os.environ.get("CE_OBF_SALT", _OBF_SALT_BASE)
    h = hashlib.sha256((salt + seed).encode()).hexdigest()[:length]
    return f"{prefix}_{h}"


# ── Discovery Phase ──────────────────────────────────────────────────────────

def discover_lfm_classes() -> dict[str, str]:
    mapping: dict[str, str] = {}
    lfm_pattern = re.compile(r"^object\s+([A-Za-z_][A-Za-z0-9_]*):\s+(T[A-Za-z_][A-Za-z0-9_]*)", re.MULTILINE)
    for src_dir in SOURCE_DIRS:
        if not src_dir.exists():
            continue
        for lfm_path in src_dir.rglob("*.lfm"):
            if should_skip(lfm_path):
                continue
            try:
                text = lfm_path.read_text(encoding="utf-8", errors="ignore")
            except Exception:
                continue
            for inst, cls in lfm_pattern.findall(text):
                if cls in PROTECTED_CLASSES:
                    continue
                if cls not in mapping:
                    mapping[cls] = stable_obf_token("F", cls)
    return mapping


def discover_lpr_createforms() -> dict[str, str]:
    result: dict[str, str] = {}
    createform_pat = re.compile(
        r"Application\.CreateForm\(\s*(T[A-Za-z_][A-Za-z0-9_]*)\s*,\s*([A-Za-z_][A-Za-z0-9_]*)\s*\)",
        re.IGNORECASE,
    )
    for src_dir in SOURCE_DIRS:
        if not src_dir.exists():
            continue
        for lpr_path in src_dir.rglob("*.lpr"):
            if should_skip(lpr_path):
                continue
            try:
                text = lpr_path.read_text(encoding="utf-8", errors="ignore")
            except Exception:
                continue
            for cls, inst in createform_pat.findall(text):
                result[inst] = cls
    return result


def discover_pas_classes() -> dict[str, str]:
    mapping: dict[str, str] = {}
    class_pat = re.compile(
        r"\b(T[A-Za-z_][A-Za-z0-9_]*)\s*=\s*class\s*\(\s*(?:TForm|TFrame|TDataModule|TComponent)\s*\)",
        re.IGNORECASE,
    )
    for src_dir in SOURCE_DIRS:
        if not src_dir.exists():
            continue
        for pas_path in src_dir.rglob("*.pas"):
            if should_skip(pas_path):
                continue
            try:
                text = pas_path.read_text(encoding="utf-8", errors="ignore")
            except Exception:
                continue
            for cls in class_pat.findall(text):
                if cls in PROTECTED_CLASSES:
                    continue
                if cls not in mapping:
                    mapping[cls] = stable_obf_token("F", cls)
    return mapping


# ── Replacement Phase ────────────────────────────────────────────────────────

def build_instance_map(class_map: dict[str, str], lpr_map: dict[str, str]) -> dict[str, str]:
    inst_map: dict[str, str] = {}
    for inst, cls in lpr_map.items():
        if cls in class_map:
            inst_map[inst] = stable_obf_token("o", inst)
    return inst_map


def build_bulk_replacer(mapping: dict[str, str]):
    if not mapping:
        return lambda text: text
    keys = sorted(mapping.keys(), key=len, reverse=True)
    pattern = re.compile(r"\b(" + "|".join(re.escape(k) for k in keys) + r")\b")
    def replacer(m: re.Match) -> str:
        return mapping[m.group(1)]
    return lambda text: pattern.sub(replacer, text)


def patch_lfm(path: Path, class_map: dict[str, str], inst_map: dict[str, str]) -> str:
    text = path.read_text(encoding="utf-8", errors="ignore")
    original = text

    def repl_lfm(m: re.Match) -> str:
        inst = m.group(1)
        cls = m.group(2)
        new_cls = class_map.get(cls, cls)
        new_inst = inst_map.get(inst, stable_obf_token("o", inst) if inst not in PROTECTED_CLASSES else inst)
        if inst not in inst_map and inst not in PROTECTED_CLASSES:
            inst_map[inst] = new_inst
        return f"object {new_inst}: {new_cls}"

    text = re.sub(r"^object\s+([A-Za-z_][A-Za-z0-9_]*):\s+(T[A-Za-z_][A-Za-z0-9_]*)", repl_lfm, text, flags=re.MULTILINE)
    cls_replacer = build_bulk_replacer(class_map)
    text = cls_replacer(text)
    return text if text != original else None


def patch_pas(path: Path, class_map: dict[str, str], inst_map: dict[str, str]) -> str:
    text = path.read_text(encoding="utf-8", errors="ignore")
    original = text
    combined = {**class_map, **inst_map}
    replacer = build_bulk_replacer(combined)
    text = replacer(text)
    return text if text != original else None


def patch_lpr(path: Path, class_map: dict[str, str], inst_map: dict[str, str]) -> str:
    text = path.read_text(encoding="utf-8", errors="ignore")
    original = text
    combined = {**class_map, **inst_map}
    replacer = build_bulk_replacer(combined)
    text = replacer(text)
    return text if text != original else None


def patch_lpi(path: Path, class_map: dict[str, str], inst_map: dict[str, str]) -> str:
    text = path.read_text(encoding="utf-8", errors="ignore")
    original = text
    for old_inst, new_inst in sorted(inst_map.items(), key=lambda x: len(x[0]), reverse=True):
        text = text.replace(f'ComponentName Value="{old_inst}"', f'ComponentName Value="{new_inst}"')
    text = re.sub(r'<MajorVersionNr Value="\d+"/>', '<MajorVersionNr Value="0"/>', text)
    text = re.sub(r'<MinorVersionNr Value="\d+"/>', '<MinorVersionNr Value="0"/>', text)
    text = re.sub(r'<BuildNr Value="\d+"/>', '<BuildNr Value="0"/>', text)
    text = re.sub(r'<RevisionNr Value="\d+"/>', '<RevisionNr Value="0"/>', text)
    text = re.sub(r'<AutoIncrementBuild Value="True"/>', '<AutoIncrementBuild Value="False"/>', text)
    text = re.sub(r'ProductVersion="[^"]*"', 'ProductVersion="0.0.0.0"', text)
    text = re.sub(r'FileVersion="[^"]*"', 'FileVersion="0.0.0.0"', text)
    return text if text != original else None


# ── Icon / Resource Phase ────────────────────────────────────────────────────

def replace_icon() -> None:
    ico_path = ROOT / "Cheat Engine" / "cheatengine.ico"
    if not ico_path.exists():
        return
    blank_ico = bytes([
        0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10,
        0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x68, 0x04,
        0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00,
        0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00,
        0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00,
    ]) + b"\x00" * (0x468 - 0x4A)
    ico_path.write_bytes(blank_ico)
    print(f"[ICON] Replaced {ico_path}")


def delete_lrs_files() -> int:
    count = 0
    for src_dir in SOURCE_DIRS:
        if not src_dir.exists():
            continue
        for lrs in src_dir.rglob("*.lrs"):
            if should_skip(lrs):
                continue
            try:
                lrs.unlink()
                count += 1
                print(f"[LRS DELETE] {lrs}")
            except Exception as e:
                print(f"[WARN] Could not delete {lrs}: {e}")
    return count


# ── Main ─────────────────────────────────────────────────────────────────────

def main() -> None:
    print("=== STEP 5: Source Fingerprint Wipe ===")
    class_map = discover_lfm_classes()
    pas_extra = discover_pas_classes()
    for cls, tok in pas_extra.items():
        if cls not in class_map:
            class_map[cls] = tok
    lpr_map = discover_lpr_createforms()
    inst_map = build_instance_map(class_map, lpr_map)
    print(f"Found {len(class_map)} project-defined form classes to rename.")
    print(f"Found {len(inst_map)} instance names to rename.")

    lfm_pattern = re.compile(r"^object\s+([A-Za-z_][A-Za-z0-9_]*):\s+(T[A-Za-z_][A-Za-z0-9_]*)", re.MULTILINE)
    for src_dir in SOURCE_DIRS:
        if not src_dir.exists():
            continue
        for lfm_path in src_dir.rglob("*.lfm"):
            if should_skip(lfm_path):
                continue
            text = lfm_path.read_text(encoding="utf-8", errors="ignore")
            for inst, cls in lfm_pattern.findall(text):
                if cls in class_map and inst not in inst_map and inst not in PROTECTED_CLASSES:
                    inst_map[inst] = stable_obf_token("o", inst)

    changed_files = []
    for src_dir in SOURCE_DIRS:
        if not src_dir.exists():
            continue
        for path in src_dir.rglob("*.lfm"):
            if should_skip(path):
                continue
            new_text = patch_lfm(path, class_map, inst_map)
            if new_text is not None:
                path.write_text(new_text, encoding="utf-8")
                changed_files.append(str(path.relative_to(ROOT)))
        for path in src_dir.rglob("*.pas"):
            if should_skip(path):
                continue
            new_text = patch_pas(path, class_map, inst_map)
            if new_text is not None:
                path.write_text(new_text, encoding="utf-8")
                changed_files.append(str(path.relative_to(ROOT)))
        for path in src_dir.rglob("*.lpr"):
            if should_skip(path):
                continue
            new_text = patch_lpr(path, class_map, inst_map)
            if new_text is not None:
                path.write_text(new_text, encoding="utf-8")
                changed_files.append(str(path.relative_to(ROOT)))
        for path in src_dir.rglob("*.lpi"):
            if should_skip(path):
                continue
            new_text = patch_lpi(path, class_map, inst_map)
            if new_text is not None:
                path.write_text(new_text, encoding="utf-8")
                changed_files.append(str(path.relative_to(ROOT)))

    replace_icon()
    lrs_deleted = delete_lrs_files()
    mapping_data = {
        "class_map": class_map,
        "instance_map": inst_map,
        "changed_files": changed_files,
        "salt": os.environ.get("CE_OBF_SALT", "<default>"),
        "notes": [
            "Run this BEFORE Lazarus build.",
            "Deleted .lrs files will be auto-regenerated from .lfm during build.",
            "Icon replaced with blank transparent ico.",
            "LPI version info zeroed out.",
        ],
    }
    OUT_MAPPING.write_text(json.dumps(mapping_data, indent=2, ensure_ascii=False), encoding="utf-8")
    print(f"\n=== STEP 5 DONE ===")
    print(f"changed_files={len(changed_files)}")
    print(f"lrs_deleted={lrs_deleted}")
    print(f"mapping_saved={OUT_MAPPING}")
    print("NEXT: Run Lazarus build, then run step6_pe_mutator.py")


if __name__ == "__main__":
    main()
