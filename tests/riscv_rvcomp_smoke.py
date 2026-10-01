#!/usr/bin/env python3
"""Run the RVComp Verilator μT-Kernel smoke workload."""

import argparse
import subprocess
import sys


BANNER = "microT-Kernel Version"
TASK1 = "[Task 1] Hello!"
TASK2 = "[Task 2] Hello!"
TASK1_DONE = "[Task 1] Finished."
TASK2_DONE = "[Task 2] Finished."
COMPLETION = "[usermain] Both tasks should be finished by now. Exiting..."
TIMER = "machine    timer interrupt detected"
FAULTS = ("access fault", "illegal instr", "page fault", "Exception/Fault")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sim", required=True, help="RVComp Verilator executable")
    parser.add_argument("--mem-file", required=True, help="RVComp 128-bit DRAM image")
    parser.add_argument("--max-cycles", type=int, default=100_000_000)
    parser.add_argument("--log", help="Write simulator output to this file")
    args = parser.parse_args()

    command = [
        args.sim,
        f"+max_cycles={args.max_cycles}",
        f"+mem_file={args.mem_file}",
        "+enable_debug_log=1",
    ]
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, check=False)
    output = result.stdout
    sys.stdout.write(output)
    if args.log:
        with open(args.log, "w", encoding="utf-8") as stream:
            stream.write(output)

    checks = {
        "boot": BANNER in output,
        "task": TASK1 in output and TASK2 in output,
        "timer": TIMER in output,
        "trap-return": TIMER in output and not any(fault in output for fault in FAULTS),
        "exit": COMPLETION in output and TASK1_DONE in output and TASK2_DONE in output,
    }
    missing = [name for name, passed in checks.items() if not passed]
    if result.returncode != 0:
        print(f"rvcomp: simulator exited with {result.returncode}", file=sys.stderr)
        return result.returncode
    if missing:
        print(f"rvcomp: missing smoke checks: {', '.join(missing)}", file=sys.stderr)
        return 1

    for name in checks:
        print(f"RISCV_SMOKE {name}")
    print("rvcomp: smoke workload passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
