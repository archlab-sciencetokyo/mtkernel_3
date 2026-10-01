#ifndef _SYSDEPEND_CPU_CORE_CPUTASK_
#define _SYSDEPEND_CPU_CORE_CPUTASK_

#include <string.h>
#include <sys/sysdef.h>

#if RISCV_XLEN == 64
typedef UD RISCV_CTX_REG;
#else
typedef UW RISCV_CTX_REG;
#endif

/*
 * System stack configuration at task startup
 */
typedef struct {
	RISCV_CTX_REG	pc;		/* Program counter (mepc) */
	RISCV_CTX_REG	mstatus;	/* Machine status register (mstatus) */
	RISCV_CTX_REG	r[31];		/* General registers x1 - x31 */
} SStackFrame;

/*
 * Size of system stack area required to write by 'knl_setup_context().'
 */
#define DORMANT_STACK_SIZE	sizeof(SStackFrame)

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
Inline void knl_setup_context( TCB *tcb )
{
	SStackFrame	*ssp;

	ssp = (SStackFrame*)tcb->isstack;
	ssp--;

	/* Clear CPU context */
	memset(ssp, 0, sizeof(SStackFrame));

	/* CPU context initialization */
	ssp->pc		= (RISCV_CTX_REG)tcb->task;		/* Task startup address */
	ssp->mstatus	= RISCV_MSTATUS_MPP | RISCV_MSTATUS_MPIE;
	ssp->r[0]	= 0;					/* ra = 0 */
	ssp->r[1]	= (RISCV_CTX_REG)ssp;			/* sp = ssp */

	UW gp_val;
	__asm__ volatile ("mv %0, gp" : "=r"(gp_val));
	ssp->r[2]	= gp_val;				/* gp = x3 */

	tcb->tskctxb.ssp = ssp;		/* System stack pointer */
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
Inline void knl_setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp;
	
	ssp = (SStackFrame*)tcb->tskctxb.ssp;

	ssp->r[9] = (RISCV_CTX_REG)stacd;		/* a0 (x10) = stacd */
	ssp->r[10] = (RISCV_CTX_REG)tcb->exinf;	/* a1 (x11) = exinf */
}

/*
 * Delete task contexts
 */
Inline void knl_cleanup_context( TCB *tcb )
{
	/* No processing */
}

#endif /* _SYSDEPEND_CPU_CORE_CPUTASK_ */
