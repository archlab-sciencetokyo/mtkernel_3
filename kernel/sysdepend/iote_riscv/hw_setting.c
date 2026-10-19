#include <sys/machine.h>
#ifdef IOTE_RISCV

#include "kernel.h"
#include <tm/tmonitor.h>
#include "sysdepend.h"
#include "riscv_board.h"

EXPORT void knl_startup_hw(void)
{
	knl_riscv_timer_init();
	/* Set Trap Vector to knl_trap_handler */
	extern void knl_trap_handler(void);
	asm volatile("csrw mtvec, %0" :: "r"(knl_trap_handler));
}

#if USE_SHUTDOWN
EXPORT void knl_shutdown_hw( void )
{
	disint();
#if defined(RISCV_BOARD_SIMRV)
	*(volatile unsigned int*)0x00100000 = 0x5555;
#endif
	while(1) {
		asm volatile("wfi");
	}
}
#endif /* USE_SHUTDOWN */

EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
#if defined(RISCV_BOARD_RVCOMP)
		disint();
		*(volatile unsigned int*)RISCV_RESET_REG = 1U;
		while(1) { asm volatile("wfi"); }
#else
		return E_NOSPT;
#endif
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
#if defined(RISCV_BOARD_RVCOMP)
		disint();
		*(volatile unsigned int*)RISCV_RESET_REG = 1U;
		while(1) { asm volatile("wfi"); }
#else
		return E_NOSPT;
#endif
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
#if defined(RISCV_BOARD_RVCOMP)
		disint();
		*(volatile unsigned int*)RISCV_RESET_REG = 1U;
		while(1) { asm volatile("wfi"); }
#else
		return E_NOSPT;
#endif
	default:
		return E_PAR;
	}
}

#endif /* IOTE_RISCV */
