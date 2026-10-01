#include "kernel.h"
#include "../../../sysdepend.h"

#include "cpu_task.h"
#include "offset.h"
#include <stddef.h>

_Static_assert(offsetof(TCB, tskctxb) == TCB_tskctxb,
	"RISC-V assembly TCB_tskctxb offset is stale");
_Static_assert(sizeof(void *) == RISCV_REG_BYTES,
	"RISC-V ABI pointer width does not match XLEN");
_Static_assert(sizeof(T_REGS) == (31 * RISCV_REG_BYTES),
	"RISC-V T_REGS layout mismatch");
_Static_assert(sizeof(T_EIT) == (2 * RISCV_REG_BYTES),
	"RISC-V T_EIT layout mismatch");
_Static_assert(sizeof(T_CREGS) == RISCV_REG_BYTES,
	"RISC-V T_CREGS layout mismatch");
_Static_assert(sizeof(SStackFrame) == (RISCV_CONTEXT_REGS * RISCV_REG_BYTES),
	"RISC-V context frame layout mismatch");
_Static_assert(_Alignof(SStackFrame) >= RISCV_CONTEXT_ALIGN,
	"RISC-V context frame alignment mismatch");

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
		ssp->pc = (RISCV_CTX_REG)eit->pc;
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
		eit->pc       = (void*)(unsigned long)ssp->pc;
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
