import os
import re
import random
import string
from pathlib import Path

# 보수적(컴파일 안정) 난독화
# - 식별자/구조 변경은 하지 않음 (링크/리소스 깨짐 방지)
# - 사용자에게 보이는 문자열/메타데이터 위주 치환

TARGET_DIR = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
ALLOWED_EXT = {
    ".pas", ".lpr", ".pp", ".lfm", ".lrt", ".lpi", ".lpk", ".txt", ".md", ".rc", ".xml"
}


def rand_token(prefix: str = "x", n: int = 10) -> str:
    return f"{prefix}_{''.join(random.choices(string.ascii_lowercase + string.digits, k=n))}"


REPLACE_TABLE = {
    "Cheat Engine 7.5": rand_token("title"),
    "Cheat Engine": rand_token("title"),
    "CheatEngine": rand_token("name"),
    "cheatengine": rand_token("name"),
    "DBK64": rand_token("name"),
    "Lua 5.3": rand_token("name"),
}


def should_skip(path: Path) -> bool:
    p = str(path).lower()
    # 바이너리/빌드 산출물/VC 중간파일 제외
    skip_parts = [
        "\\.git\\", "\\bin\\", "\\lib\\", "\\obj\\", "\\backup\\",
        "\\x86_64", "\\i386", "\\__history", "\\debug", "\\release",
        "\\.idea\\", "\\chat-session-resources\\", "\\dbkdriver\\"
    ]
    if any(x in p for x in skip_parts):
        return True
    if path.suffix.lower() not in ALLOWED_EXT:
        return True
    return False


def replace_case_sensitive(content: str) -> str:
    out = content
    # 긴 키부터 치환 (부분치환 충돌 방지)
    for src in sorted(REPLACE_TABLE.keys(), key=len, reverse=True):
        out = out.replace(src, REPLACE_TABLE[src])
    return out


def patch_lpi_title(content: str) -> str:
    # Lazarus 프로젝트 title/version string table만 보수적으로 패치
    content = re.sub(r'(<Title Value=")[^"]*("/>)', rf'\1{rand_token("title")}\2', content)
    content = re.sub(r'(CompanyName=")[^"]*(")', rf'\1{rand_token("corp")}\2', content)
    content = re.sub(r'(FileDescription=")[^"]*(")', rf'\1{rand_token("desc")}\2', content)
    return content


def process_file(path: Path) -> tuple[bool, int]:
    try:
        raw = path.read_text(encoding="utf-8", errors="ignore")
    except Exception:
        return False, 0

    modified = replace_case_sensitive(raw)

    if path.suffix.lower() == ".lpi":
        modified = patch_lpi_title(modified)

    if modified != raw:
        path.write_text(modified, encoding="utf-8")
        return True, (raw.count("\n") + 1)
    return False, 0


def main() -> None:
    changed_files = 0
    scanned_files = 0

    for p in TARGET_DIR.rglob("*"):
        if not p.is_file() or should_skip(p):
            continue

        scanned_files += 1
        changed, _ = process_file(p)
        if changed:
            changed_files += 1
            print(f"[CHANGED] {p}")

    print("\n=== OBF PASS #1 DONE ===")
    print(f"scanned_files={scanned_files}")
    print(f"changed_files={changed_files}")
    print("next: run 64-bit full build validation")


if __name__ == "__main__":
    main()