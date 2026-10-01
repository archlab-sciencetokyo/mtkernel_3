/*
 * RISC-V board hook implementation for micro T-Kernel 3.0.
 *
 * Copyright (C) 2026 by RISC-V port contributors.
 * This software is distributed under the T-License 2.2.
 *
 *-----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef IOTE_RISCV

#include "kernel.h"
#include "riscv_board.h"

IMPORT void knl_interrupt_handler(UW intno);

#define CLINT_MTIME_LO     (*(volatile UW *)(RISCV_CLINT_BASE + 0xbff8UL))
#define CLINT_MTIME_HI     (*(volatile UW *)(RISCV_CLINT_BASE + 0xbffcUL))
#define CLINT_MTIMECMP_LO  (*(volatile UW *)(RISCV_CLINT_BASE + 0x4000UL))
#define CLINT_MTIMECMP_HI  (*(volatile UW *)(RISCV_CLINT_BASE + 0x4004UL))

#if RISCV_PLIC_BASE != 0
#define PLIC_CLAIM         (*(volatile UW *)(RISCV_PLIC_BASE + 0x200004UL))
#define PLIC_COMPLETE      (*(volatile UW *)(RISCV_PLIC_BASE + 0x200004UL))
#endif

EXPORT UD knl_riscv_timer_read(void)
{
#if RISCV_XLEN == 32
	UW hi1, lo, hi2;
	do {
		hi1 = CLINT_MTIME_HI;
		lo = CLINT_MTIME_LO;
		hi2 = CLINT_MTIME_HI;
	} while (hi1 != hi2);
	return ((UD)hi2 << 32) | lo;
#else
	return (*(volatile UD *)(RISCV_CLINT_BASE + 0xbff8UL));
#endif
}

EXPORT void knl_riscv_timer_set_compare(UD value)
{
#if RISCV_XLEN == 32
	/* Avoid a transient early compare while updating the 64-bit value. */
	CLINT_MTIMECMP_HI = 0xffffffffU;
	CLINT_MTIMECMP_LO = (UW)value;
	CLINT_MTIMECMP_HI = (UW)(value >> 32);
#else
	(*(volatile UD *)(RISCV_CLINT_BASE + 0x4000UL)) = value;
#endif
}

EXPORT void knl_riscv_external_interrupt(UW cause)
{
#if RISCV_PLIC_BASE != 0
	UW source;
	if (cause != RISCV_MCAUSE_MEI) {
		return;
	}
	source = PLIC_CLAIM;
	if (source != 0U && source < 32U) {
		knl_interrupt_handler(source);
		PLIC_COMPLETE = source;
	}
#else
	(void)cause;
#endif
}

EXPORT void knl_riscv_timer_init(void)
{
}

#endif /* IOTE_RISCV */
