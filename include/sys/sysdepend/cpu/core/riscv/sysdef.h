#ifndef __SYS_SYSDEF_DEPEND_CORE_H__
#define __SYS_SYSDEF_DEPEND_CORE_H__

/* GCC defines this for every RISC-V compilation. */
#ifndef RISCV_XLEN
# ifdef __riscv_xlen
#  define RISCV_XLEN __riscv_xlen
# else
#  define RISCV_XLEN 32
# endif
#endif

#if (RISCV_XLEN != 32) && (RISCV_XLEN != 64)
# error "RISC-V XLEN must be 32 or 64"
#endif

#define RISCV_REG_BYTES        (RISCV_XLEN / 8)
#define RISCV_MSTATUS_MIE      (1UL << 3)
#define RISCV_MSTATUS_MPIE     (1UL << 7)
#define RISCV_MSTATUS_MPP      (3UL << 11)
#define RISCV_MIE_MTIE         (1UL << 7)
#define RISCV_MCAUSE_INT       (1UL << (RISCV_XLEN - 1))
#define RISCV_MCAUSE_CODE_MASK (RISCV_MCAUSE_INT - 1)
#define RISCV_MCAUSE_MTI       7
#define RISCV_MCAUSE_MEI       11

#define RISCV_CONTEXT_REGS     33
#define RISCV_CONTEXT_ALIGN    RISCV_REG_BYTES

/* Definition of minimum system stack size */
#define MIN_SYS_STACK_SIZE	256
#define DEFAULT_SYS_STKSZ	MIN_SYS_STACK_SIZE

/* Internal Memory (Main RAM) */
#define INTERNAL_RAM_START	0x80000000
#define INTERNAL_RAM_SIZE	0x00040000	/* 256KB */
#define INTERNAL_RAM_END	(INTERNAL_RAM_START + INTERNAL_RAM_SIZE)

/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

/* Coprocessor and physical timer capabilities */
#define CPU_HAS_PTMR		0
#define CPU_HAS_FPU		0
#define CPU_HAS_DSP		0
#define NUM_COPROCESSOR		0

#define INTPRI_BITWIDTH		3

/* Number of Interrupt vectors */
#define N_INTVEC		32
#define N_SYSVEC		0

#endif /* __SYS_SYSDEF_DEPEND_CORE_H__ */
