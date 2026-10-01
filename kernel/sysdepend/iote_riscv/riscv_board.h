/*
 * RISC-V board hook interface for micro T-Kernel 3.0.
 *
 * Copyright (C) 2026 by RISC-V port contributors.
 * This software is distributed under the T-License 2.2.
 *
 *-----------------------------------------------------------------------
 */

#ifndef _IOTE_RISCV_BOARD_H_
#define _IOTE_RISCV_BOARD_H_

#include <tk/typedef.h>
#include <sys/sysdef.h>

#ifdef RISCV_BOARD_RVCOMP
#define RISCV_CLINT_BASE	0x02000000UL
#define RISCV_PLIC_BASE	0x0c000000UL
#define RISCV_TIMER_NS_NUM	20U
#define RISCV_TIMER_NS_DEN	3U
#ifndef RISCV_TIMER_HZ
#define RISCV_TIMER_HZ	150000000U
#endif
#else
#define RISCV_CLINT_BASE	0x60000000UL
#define RISCV_PLIC_BASE	0x00000000UL
#define RISCV_TIMER_NS_NUM	1000U
#define RISCV_TIMER_NS_DEN	1U
#ifndef RISCV_TIMER_HZ
#define RISCV_TIMER_HZ	1000000U
#endif
#endif

IMPORT UD knl_riscv_timer_read(void);
IMPORT void knl_riscv_timer_set_compare(UD value);
IMPORT void knl_riscv_timer_init(void);
IMPORT void knl_riscv_external_interrupt(UW cause);

#endif /* _IOTE_RISCV_BOARD_H_ */
