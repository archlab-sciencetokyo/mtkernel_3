#include <sys/machine.h>
#ifdef IOTE_RISCV

#include "kernel.h"
#include <tm/tmonitor.h>
#include "sysdepend.h"

EXPORT void knl_startup_hw(void)
{
	/* Set Trap Vector to knl_trap_handler */
	extern void knl_trap_handler(void);
	asm volatile("csrw mtvec, %0" :: "r"(knl_trap_handler));
}

#if USE_SHUTDOWN
EXPORT void knl_shutdown_hw( void )
{
	disint();
	*(volatile unsigned int*)0x00100000 = 0x5555;
	while(1);
}
#endif /* USE_SHUTDOWN */

EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}

#endif /* IOTE_RISCV */
