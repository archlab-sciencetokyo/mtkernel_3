#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <kernel.h>
#include "../../../sysdepend.h"

#if (USE_EXCEPTION_DBG_MSG && USE_TMONITOR)
	#define EXCEPTION_DBG_MSG(a)	tm_putstring((UB*)a)
#else
	#define EXCEPTION_DBG_MSG(a)
#endif

/*
 * High-level exception handler called from trap entry
 */
EXPORT void knl_exception_handler(void *sp, UW mcause)
{
	EXCEPTION_DBG_MSG("*** Exception/Fault trap occurred ***\n");
#if (USE_EXCEPTION_DBG_MSG && USE_TMONITOR)
	tm_printf((UB*)"mcause: %x\n", mcause);
#endif
	while(1);
}
