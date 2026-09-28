#include "kernel.h"
#include "../../../sysdepend.h"

#include "cpu_task.h"

/* Temporal stack used when 'dispatch_to_schedtsk' is called */
Noinit(EXPORT UB knl_tmp_stack[TMP_STACK_SIZE]);

/* Task independent status */
EXPORT	W	knl_taskindp = 0;

/* ------------------------------------------------------------------------ */
/*
 * Set task register contents (Used in tk_set_reg())
 */
EXPORT void knl_set_reg( TCB *tcb, CONST T_REGS *regs, CONST T_EIT *eit, CONST T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT	i;

	ssp = (SStackFrame*)(( cregs != NULL )? cregs->ssp: tcb->tskctxb.ssp);
	
	if ( regs != NULL ) {
		for ( i = 0; i < 31; ++i ) {
			ssp->r[i] = regs->r[i];
		}
	}

	if ( eit != NULL ) {
		ssp->pc = (UW)eit->pc;
		ssp->mstatus = eit->mstatus;
	}

	if ( cregs != NULL ) {
		tcb->tskctxb.ssp  = cregs->ssp;
	}
}

/* ------------------------------------------------------------------------ */
/*
 * Get task register contents (Used in tk_get_reg())
 */
EXPORT void knl_get_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT		i;

	ssp = (SStackFrame*)tcb->tskctxb.ssp;

	if ( regs != NULL ) {
		for ( i = 0; i < 31; ++i ) {
			regs->r[i] = ssp->r[i];
		}
	}

	if ( eit != NULL ) {
		eit->pc       = (void*)ssp->pc;
		eit->mstatus  = ssp->mstatus;
	}

	if ( cregs != NULL ) {
		cregs->ssp   = tcb->tskctxb.ssp;
	}
}

/* ----------------------------------------------------------------------- */
/*
 *	Task dispatcher startup
 */
EXPORT void knl_force_dispatch( void )
{
	extern void knl_dispatch_to_schedtsk(void);
	asm volatile("j knl_dispatch_to_schedtsk");
}

EXPORT void knl_dispatch( void )
{
	extern void knl_dispatch_entry(void);
	asm volatile("call knl_dispatch_entry");
}
