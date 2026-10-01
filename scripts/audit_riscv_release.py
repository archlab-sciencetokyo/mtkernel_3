#!/usr/bin/env python3
"""Audit the RISC-V contribution for repository and release hygiene."""
from pathlib import Path
import subprocess
import sys
ROOT = Path(__file__).resolve().parents[1]
SUFFIXES = {".c", ".h", ".S", ".s", ".mk", ".ld"}
ROOTS = [ROOT / p for p in ("kernel/sysdepend/cpu/core/riscv", "kernel/sysdepend/iote_riscv", "include/sys/sysdepend/cpu/core/riscv", "include/tk/sysdepend/cpu/core/riscv", "lib/libtk/sysdepend/cpu/core/riscv", "build_make")]

def main():
    errors, warnings = [], []
    paths = [ROOT / x for x in subprocess.check_output(["git", "-C", str(ROOT), "ls-files"], text=True).splitlines()]
    for path in paths:
        try: text = path.read_text(encoding="utf-8")
        except (UnicodeDecodeError, OSError): continue
        conflict_lines = (line.startswith("<<<<<<< ") or line.startswith("=======") or line.startswith(">>>>>>> ")
                          for line in text.splitlines())
        if any(conflict_lines):
            errors.append(f"conflict marker: {path.relative_to(ROOT)}")
    for directory in ROOTS:
        if not directory.exists(): continue
        for path in directory.rglob("*"):
            if not path.is_file() or path.suffix not in SUFFIXES: continue
            text = path.read_text(encoding="utf-8")
            if path.name in {"riscv_board.c", "riscv_board.h"} and "T-License 2.2" not in text:
                errors.append(f"missing T-License 2.2 header: {path.relative_to(ROOT)}")
            if path.suffix in {".c", ".h", ".S", ".s"} and "Copyright" not in text:
                warnings.append(f"missing copyright attribution: {path.relative_to(ROOT)}")
    for path in paths:
        if path.suffix in {".o", ".d", ".elf", ".map", ".bin", ".hex"}:
            errors.append(f"generated artifact tracked: {path.relative_to(ROOT)}")
    for item in errors: print(f"ERROR: {item}")
    for item in warnings: print(f"WARNING: {item}")
    if errors: return 1
    print(f"RISC-V release audit passed ({len(paths)} tracked files inspected)")
    return 0
if __name__ == "__main__": sys.exit(main())
