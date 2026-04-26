"""
Master Build Pipeline for Obfuscated CE
=======================================
1. Pre-flight checks
2. Run step5_source_fingerprint_wipe.py
3. Build via lazbuild
4. Run step6_pe_mutator.py
5. Post-build verification
6. Artifact packaging

Usage:
    python master_build.py [--mode "Release 64-Bit"] [--salt RANDOMSTRING]
"""

import os
import sys
import json
import hashlib
import subprocess
import shutil
import argparse
from pathlib import Path
from datetime import datetime

ROOT = Path(r"C:\Users\KinngH\Desktop\cheat-engine-7.5")
CE_DIR = ROOT / "Cheat Engine"
BIN_DIR = CE_DIR / "bin"
DIST_DIR = ROOT / "dist"
BUILD_LOG = ROOT / "build.log"

LAZBUILD_CANDIDATES = [
    Path(r"C:\lazarus\lazbuild.exe"),
    Path(r"C:\Program Files\Lazarus\lazbuild.exe"),
    Path(r"C:\Program Files (x86)\Lazarus\lazbuild.exe"),
    Path(os.environ.get("LAZARUS_DIR", "")) / "lazbuild.exe",
]

SENSITIVE_STRINGS = [
    b"Cheat Engine",
    b"CheatEngine",
    b"cheatengine",
    b"cheat-engine",
    b"Dark Byte",
    b"dark_byte",
    b"cheatengine.org",
    b"forum.cheatengine.org",
    b"wiki.cheatengine.org",
    b"patreon.com/cheatengine",
    b"github.com/cheat-engine",
]


def find_lazbuild() -> Path:
    for cand in LAZBUILD_CANDIDATES:
        if cand.exists():
            return cand
    laz = shutil.which("lazbuild")
    if laz:
        return Path(laz)
    raise FileNotFoundError("lazbuild.exe not found. Install Lazarus or set LAZARUS_DIR env.")


def log(msg: str) -> None:
    ts = datetime.now().isoformat()
    line = f"[{ts}] {msg}"
    print(line)
    with open(BUILD_LOG, "a", encoding="utf-8") as f:
        f.write(line + "\n")


def run_step7() -> None:
    log("=== Running Step 7: String Encryption ===")
    result = subprocess.run(
        [sys.executable, str(ROOT / "step7_string_encrypt.py")],
        cwd=str(ROOT),
        capture_output=True,
        text=True,
    )
    log(result.stdout)
    if result.returncode != 0:
        log(result.stderr)
        raise RuntimeError("Step 7 failed")


def run_step5(salt: str) -> None:
    log("=== Running Step 5: Source Fingerprint Wipe ===")
    env = os.environ.copy()
    env["CE_OBF_SALT"] = salt
    result = subprocess.run(
        [sys.executable, str(ROOT / "step5_source_fingerprint_wipe.py")],
        cwd=str(ROOT),
        env=env,
        capture_output=True,
        text=True,
    )
    log(result.stdout)
    if result.returncode != 0:
        log(result.stderr)
        raise RuntimeError("Step 5 failed")


def build_ce(lazbuild: Path, mode: str) -> Path:
    log(f"=== Building CE [{mode}] ===")
    lpi = CE_DIR / "cheatengine.lpi"
    if not lpi.exists():
        raise FileNotFoundError(f"Project file not found: {lpi}")
    cmd = [
        str(lazbuild),
        f"--build-mode={mode}",
        "--build-all",
        str(lpi),
    ]
    log(f"CMD: {' '.join(cmd)}")
    result = subprocess.run(cmd, capture_output=True, text=True)
    log(result.stdout)
    if result.returncode != 0:
        log(result.stderr)
        raise RuntimeError(f"Lazarus build failed with code {result.returncode}")
    built_exe = BIN_DIR / "name_afa2b3b950-x86_64.exe"
    if not built_exe.exists():
        exes = sorted(BIN_DIR.glob("*.exe"), key=lambda p: p.stat().st_mtime, reverse=True)
        if not exes:
            raise FileNotFoundError("No .exe found in bin/ after build")
        built_exe = exes[0]
    log(f"Built: {built_exe}")
    return built_exe


def run_step6(input_exe: Path, output_exe: Path) -> None:
    log("=== Running Step 6: PE Mutator ===")
    result = subprocess.run(
        [
            sys.executable,
            str(ROOT / "step6_pe_mutator.py"),
            f"--input={input_exe}",
            f"--output={output_exe}",
        ],
        capture_output=True,
        text=True,
    )
    log(result.stdout)
    if result.returncode != 0:
        log(result.stderr)
        raise RuntimeError("Step 6 failed")


def verify_output(exe_path: Path) -> dict:
    log(f"=== Verifying {exe_path.name} ===")
    data = exe_path.read_bytes()
    sha256 = hashlib.sha256(data).hexdigest()
    findings = []
    text_blob = data
    for bad in SENSITIVE_STRINGS:
        if bad.lower() in text_blob.lower():
            findings.append(bad.decode("utf-8", errors="ignore"))
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0] if data[:2] == b"MZ" else 0
    has_pe = data[pe_offset:pe_offset + 4] == b"PE\x00\x00" if pe_offset else False
    result = {
        "path": str(exe_path),
        "sha256": sha256,
        "size": len(data),
        "sensitive_strings_found": findings,
        "pe_valid": has_pe,
        "timestamp": datetime.now().isoformat(),
    }
    log(f"SHA256: {sha256}")
    log(f"Size: {len(data)} bytes")
    if findings:
        log(f"WARN: Sensitive strings still present: {findings}")
    else:
        log("PASS: No sensitive strings detected")
    return result


def package_artifact(exe_path: Path, verify_data: dict, salt: str) -> Path:
    DIST_DIR.mkdir(exist_ok=True)
    final_name = f"ce_{hashlib.sha256(os.urandom(16)).hexdigest()[:10]}.exe"
    final_path = DIST_DIR / final_name
    shutil.copy2(exe_path, final_path)
    manifest = {
        "salt": salt,
        "build_mode": "Release 64-Bit",
        "verification": verify_data,
        "notes": [
            "This binary has been polymorphically obfuscated.",
            "Each build produces a unique hash and fingerprint.",
        ],
    }
    manifest_path = DIST_DIR / f"{final_name}.manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2, ensure_ascii=False), encoding="utf-8")
    log(f"Packaged: {final_path}")
    return final_path


def main() -> None:
    parser = argparse.ArgumentParser(description="CE Obfuscated Master Build")
    parser.add_argument("--mode", default="Release 64-Bit", help="Lazarus build mode")
    parser.add_argument("--salt", default=None, help="Obfuscation salt (random if omitted)")
    parser.add_argument("--skip-build", action="store_true", help="Skip Lazarus build")
    parser.add_argument("--input-exe", type=Path, default=None, help="Existing exe to mutate")
    args = parser.parse_args()

    salt = args.salt or hashlib.sha256(os.urandom(32)).hexdigest()[:16]
    log(f"Build started. Salt={salt}")

    lazbuild = find_lazbuild()
    log(f"Found lazbuild: {lazbuild}")

    DIST_DIR.mkdir(exist_ok=True)
    if BUILD_LOG.exists():
        BUILD_LOG.unlink()

    try:
        # Phase 1: Source fingerprint wipe
        if not args.skip_build:
            run_step5(salt)
        else:
            log("Skipping Step 5 as requested")

        # Phase 2: String encryption (UM evasion)
        if not args.skip_build:
            run_step7()
        else:
            log("Skipping Step 7 as requested")

        if args.input_exe:
            built_exe = args.input_exe.resolve()
            log(f"Using provided input: {built_exe}")
        elif args.skip_build:
            exes = sorted(BIN_DIR.glob("*.exe"), key=lambda p: p.stat().st_mtime, reverse=True)
            if not exes:
                raise FileNotFoundError("No existing .exe in bin/")
            built_exe = exes[0]
            log(f"Using existing build: {built_exe}")
        else:
            built_exe = build_ce(lazbuild, args.mode)

        mutated_exe = DIST_DIR / f"mutated_{hashlib.sha256(os.urandom(8)).hexdigest()[:8]}.exe"
        run_step6(built_exe, mutated_exe)
        verify_data = verify_output(mutated_exe)
        final = package_artifact(mutated_exe, verify_data, salt)

        log("\n=== BUILD SUCCESS ===")
        log(f"Final artifact: {final}")
        log(f"SHA256: {verify_data['sha256']}")
        if verify_data['sensitive_strings_found']:
            log("WARNING: Some sensitive strings remain. Review step1-4 coverage.")
        else:
            log("VERDICT: Clean build. No known signatures detected.")

    except Exception as e:
        log(f"\n=== BUILD FAILED ===")
        log(str(e))
        raise


if __name__ == "__main__":
    import struct
    main()
