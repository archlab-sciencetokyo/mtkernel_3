#ifndef _SYSDEPEND_CPU_CORE_SYSDEPEND_
#define _SYSDEPEND_CPU_CORE_SYSDEPEND_

#include <sys/sysdepend/cpu/core/riscv/sysdef.h>

/* Reset Handler */
IMPORT void Reset_Handler(void);

/* Exception Handler */
IMPORT void knl_exception_handler(void *sp, UW mcause);

/* Dispatcher */
IMPORT void knl_dispatch_entry(void);
IMPORT void knl_dispatch_to_schedtsk(void);

/* Task context block */
typedef struct {
	void	*ssp;		/* System stack pointer */
} CTXB;

#endif /* _SYSDEPEND_CPU_CORE_SYSDEPEND_ */
