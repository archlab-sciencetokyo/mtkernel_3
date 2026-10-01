# RISC-V port implementation contract

This port is maintained as a fork of the official TRON Forum μT-Kernel 3.0
source. Generic kernel behavior remains inherited from upstream; this document
records the RISC-V derivative-port choices and board boundary.

This records the implementation-defined parts of the μT-Kernel 3.0 RISC-V port.
The generic core remains reviewable while board support supplies hardware details.

## Supported profiles

| Profile | ISA | ABI | Register width | Context frame |
| --- | --- | ---: | ---: | ---: |
| RV32 | `rv32ima_zicsr_zifencei_zicntr` | `ilp32` | 32 bits | 132 bytes |
| RV64 | `rv64ima_zicsr_zifencei_zicntr` | `lp64` | 64 bits | 264 bytes |

Compressed instructions may be added to `RISCV_MARCH` when supported. Floating-point
and vector register context is not currently supported. `RISCV_XLEN`, `RISCV_MARCH`,
and `RISCV_MABI` are the single build configuration source.

## Execution model

The port runs in machine mode. Trap entry saves `mepc`, `mstatus`, and x1–x31 in
an XLEN-sized frame, dispatches machine timer or external interrupts, and returns
with `mret`. CSR access requires `zicsr`; synchronization requires `zifencei`; the
`time` counter used by microsecond waits requires `zicntr`.

The T-Kernel physical-timer capability remains disabled (`USE_PTMR=0`) because
that API is not implemented. The system tick uses the machine timer through board
hooks.

## Board hook contract

`kernel/sysdepend/iote_riscv/riscv_board.h` separates the generic port from a BSP.
A board implementation provides a monotonic timer read, timer compare programming,
timer initialization, and external interrupt-controller dispatch. The default
implementation is SimRV (`RISCV_BOARD=simrv`), with CLINT at `0x60000000`, 1 MHz timer
base, and standard 16550 UART. The `RISCV_BOARD=rvcomp` profile selects RVComp's
CLINT at `0x02000000`, PLIC at `0x0c000000`, and 150 MHz timer base. Its
external-interrupt hook claims and completes PLIC sources for machine context 0;
source priority/enable policy remains the responsibility of device initialization.
RVComp's UART is also board-specific: RX/TX data is at `0x10000000`, TX-full is at
`0x10000004`, and RX-empty is at `0x10000008`; the port selects this mapping through
`RISCV_BOARD_RVCOMP`. PLIC, vendor interrupt controllers, other UARTs, reset registers,
memory maps, and shutdown mechanisms belong in named board code.

## Conformance matrix

| Area | Host/static coverage | Board smoke coverage |
| --- | --- | --- |
| Context setup/switch/delete | frame offsets and RV32/RV64 assertions | first task and dispatch return |
| Register APIs | `T_REGS`, `T_EIT`, `T_CREGS` layout assertions | register set/get round trip |
| Interrupt state | cause decoding and CSR masks | timer/external entry and return |
| Trap path | save/restore and `mret` disassembly | trap reaches kernel handler |
| Timer | rollover and compare arithmetic | periodic tick and reprogramming |
| Microsecond waits | width and rollover checks | bounded `WaitUsec` smoke test |
| Unsupported API | `USE_PTMR=0` configuration | `E_NOSPT` behavior |

Host tests validate deterministic properties; they do not claim hardware conformance.

## Board smoke-test interface

`tests/riscv_board_smoke.py` provides a runner-neutral smoke-test protocol. A CFU
Proving Ground or RVComp runner supplies an ELF and emits these lines as it executes:

```text
RISCV_SMOKE boot
RISCV_SMOKE task
RISCV_SMOKE timer
RISCV_SMOKE trap-return
RISCV_SMOKE exit
```

The generic checker validates the markers and returns a non-zero status when a board
runner omits one. Board-specific build, Verilator, UART, and reset commands remain in
the hardware workspace and are passed through `--runner`.

For the RVComp Verilator workload, `tests/riscv_rvcomp_smoke.py` checks the kernel
banner, first-task output, machine-timer diagnostics, absence of trap faults, and the
sample application's completion message. The stock RVComp testbench still reports its
own cycle timeout after the workload because it has no μT-Kernel shutdown ABI; the
wrapper treats the application completion message as the test completion condition.
