#!/usr/bin/env python3
"""Validate the common output contract of a RISC-V board smoke runner."""
import argparse
import shlex
import subprocess
import sys

MARKERS = ("boot", "task", "timer", "trap-return", "exit")

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--board", required=True, choices=("cfu-pg", "rvcomp", "simrv"))
    parser.add_argument("--elf", required=True)
    parser.add_argument("--runner", required=True, help="Command template containing {elf}")
    args = parser.parse_args()
    command = [part.replace("{elf}", args.elf) for part in shlex.split(args.runner)]
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, check=False)
    sys.stdout.write(result.stdout)
    missing = [f"RISCV_SMOKE {marker}" for marker in MARKERS if f"RISCV_SMOKE {marker}" not in result.stdout]
    if result.returncode:
        print(f"{args.board}: runner exited with {result.returncode}", file=sys.stderr)
        return result.returncode
    if missing:
        print(f"{args.board}: missing smoke markers: {', '.join(missing)}", file=sys.stderr)
        return 1
    print(f"{args.board}: smoke contract passed")
    return 0

if __name__ == "__main__":
    sys.exit(main())
