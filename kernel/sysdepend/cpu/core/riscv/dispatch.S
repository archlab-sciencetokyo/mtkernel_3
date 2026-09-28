#include <sys/machine.h>
#ifdef CPU_CORE_RISCV

#define _in_asm_source_

#include <sys/machine.h>
#include <config.h>
#include <tk/errno.h>
#include <sys/sysdef.h>
#include <sys/knldef.h>

#include "offset.h"

	.extern Csym(knl_tmp_stack)
	.extern Csym(knl_dispatch_disabled)
	.extern Csym(knl_ctxtsk)
	.extern Csym(knl_schedtsk)
	.extern Csym(knl_lowpow_discnt)
	.extern Csym(low_pow)
	.extern Csym(knl_taskindp)
	.extern Csym(knl_int_nest)
	.extern Csym(knl_exc_stack)
	.extern Csym(knl_interrupt_handler)
	.extern Csym(knl_exception_handler)

	.text
	.align 2
	.globl Csym(knl_dispatch_to_schedtsk)
	.globl Csym(knl_dispatch_entry)
	.globl Csym(knl_trap_handler)

Csym(knl_dispatch_to_schedtsk):
	/* Set temporal stack */
	la	sp, Csym(knl_tmp_stack)
	li	t0, 256			/* TMP_STACK_SIZE */
	add	sp, sp, t0

	/* knl_dispatch_disabled = DDS_DISABLE_IMPLICIT */
	la	t0, Csym(knl_dispatch_disabled)
	li	t1, 1
	sw	t1, 0(t0)

	/* ctxtsk = NULL */
	la	t0, Csym(knl_ctxtsk)
	sw	zero, 0(t0)

	j	l_dispatch_loop

Csym(knl_dispatch_entry):
	/* Disable interrupts and get old mstatus in a0 */
	li	t0, 8			/* MIE bit is 8 */
	csrrc	a0, mstatus, t0

	/* Allocate stack frame (sizeof(SStackFrame) = 33 * 4 = 132 bytes) */
	addi	sp, sp, -132

	/* Save general registers x1 (ra), x3 - x31 */
	sw	x1, 8(sp)		/* r[0] */
	addi	a1, sp, 132
	sw	a1, 12(sp)		/* r[1] (saved sp value) */
	sw	x3, 16(sp)		/* r[2] */
	sw	x4, 20(sp)		/* r[3] */
	sw	x5, 24(sp)		/* r[4] */
	sw	x6, 28(sp)		/* r[5] */
	sw	x7, 32(sp)		/* r[6] */
	sw	x8, 36(sp)		/* r[7] */
	sw	x9, 40(sp)		/* r[8] */
	sw	x10, 44(sp)		/* r[9] */
	sw	x11, 48(sp)		/* r[10] */
	sw	x12, 52(sp)		/* r[11] */
	sw	x13, 56(sp)		/* r[12] */
	sw	x14, 60(sp)		/* r[13] */
	sw	x15, 64(sp)		/* r[14] */
	sw	x16, 68(sp)		/* r[15] */
	sw	x17, 72(sp)		/* r[16] */
	sw	x18, 76(sp)		/* r[17] */
	sw	x19, 80(sp)		/* r[18] */
	sw	x20, 84(sp)		/* r[19] */
	sw	x21, 88(sp)		/* r[20] */
	sw	x22, 92(sp)		/* r[21] */
	sw	x23, 96(sp)		/* r[22] */
	sw	x24, 100(sp)		/* r[23] */
	sw	x25, 104(sp)		/* r[24] */
	sw	x26, 108(sp)		/* r[25] */
	sw	x27, 112(sp)		/* r[26] */
	sw	x28, 116(sp)		/* r[27] */
	sw	x29, 120(sp)		/* r[28] */
	sw	x30, 124(sp)		/* r[29] */
	sw	x31, 128(sp)		/* r[30] */

	/* Save PC (ra is the return address from this call) and mstatus */
	sw	x1, 0(sp)		/* pc */
	sw	a0, 4(sp)		/* mstatus */

	/* Save SSP to ctxtsk TCB */
	la	t0, Csym(knl_ctxtsk)
	lw	t1, 0(t0)		/* t1 = ctxtsk */
	sw	sp, 24(t1)		/* ctxtsk->tskctxb.ssp = sp  (offset 24 is TCB_tskctxb) */

	/* ctxtsk = NULL */
	sw	zero, 0(t0)

l_dispatch_loop:
	/* Disable interrupts */
	li	t0, 8
	csrrc	a0, mstatus, t0

	la	t0, Csym(knl_schedtsk)
	lw	t2, 0(t0)		/* t2 = schedtsk */
	bne	t2, zero, l_dispatch_switch

	/* No task to run, go to low power mode */
	la	t3, Csym(knl_lowpow_discnt)
	lw	t3, 0(t3)
	bne	t3, zero, l_no_lowpow

	/* Enable interrupts before low_pow to allow wake-up */
	li	t0, 8
	csrrs	x0, mstatus, t0
	call	Csym(low_pow)
	j	l_dispatch_loop

l_no_lowpow:
	li	t0, 8
	csrrs	x0, mstatus, t0
	wfi
	j	l_dispatch_loop

l_dispatch_switch:
	/* Switch to schedtsk */
	la	t0, Csym(knl_ctxtsk)
	sw	t2, 0(t0)		/* ctxtsk = schedtsk */

	/* Restore stack pointer */
	lw	sp, 24(t2)		/* sp = schedtsk->tskctxb.ssp (offset 24 is TCB_tskctxb) */

	/* Restore mepc and mstatus */
	lw	t0, 0(sp)		/* pc */
	csrw	mepc, t0
	lw	t0, 4(sp)		/* mstatus */
	li	t1, 0x1800		/* Force MPP = 3 (Machine Mode) */
	or	t0, t0, t1
	csrw	mstatus, t0

	/* Restore registers */
	lw	x1, 8(sp)
	lw	x3, 16(sp)
	lw	x4, 20(sp)
	lw	x5, 24(sp)
	lw	x6, 28(sp)
	lw	x7, 32(sp)
	lw	x8, 36(sp)
	lw	x9, 40(sp)
	lw	x10, 44(sp)
	lw	x11, 48(sp)
	lw	x12, 52(sp)
	lw	x13, 56(sp)
	lw	x14, 60(sp)
	lw	x15, 64(sp)
	lw	x16, 68(sp)
	lw	x17, 72(sp)
	lw	x18, 76(sp)
	lw	x19, 80(sp)
	lw	x20, 84(sp)
	lw	x21, 88(sp)
	lw	x22, 92(sp)
	lw	x23, 96(sp)
	lw	x24, 100(sp)
	lw	x25, 104(sp)
	lw	x26, 108(sp)
	lw	x27, 112(sp)
	lw	x28, 116(sp)
	lw	x29, 120(sp)
	lw	x30, 124(sp)
	lw	x31, 128(sp)

	/* Enable dispatch */
	la	t0, Csym(knl_dispatch_disabled)
	sw	zero, 0(t0)

	/* Deallocate stack frame */
	addi	sp, sp, 132
	mret

	.p2align 2
Csym(knl_trap_handler):
	/* Save context to task stack */
	/* Allocate stack frame */
	addi	sp, sp, -132

	/* Save general registers x1 (ra), x3 - x31 */
	sw	x1, 8(sp)		/* r[0] */
	addi	a0, sp, 132
	sw	a0, 12(sp)		/* r[1] */
	sw	x3, 16(sp)		/* r[2] */
	sw	x4, 20(sp)		/* r[3] */
	sw	x5, 24(sp)		/* r[4] */
	sw	x6, 28(sp)		/* r[5] */
	sw	x7, 32(sp)		/* r[6] */
	sw	x8, 36(sp)		/* r[7] */
	sw	x9, 40(sp)		/* r[8] */
	sw	x10, 44(sp)		/* r[9] */
	sw	x11, 48(sp)		/* r[10] */
	sw	x12, 52(sp)		/* r[11] */
	sw	x13, 56(sp)		/* r[12] */
	sw	x14, 60(sp)		/* r[13] */
	sw	x15, 64(sp)		/* r[14] */
	sw	x16, 68(sp)		/* r[15] */
	sw	x17, 72(sp)		/* r[16] */
	sw	x18, 76(sp)		/* r[17] */
	sw	x19, 80(sp)		/* r[18] */
	sw	x20, 84(sp)		/* r[19] */
	sw	x21, 88(sp)		/* r[20] */
	sw	x22, 92(sp)		/* r[21] */
	sw	x23, 96(sp)		/* r[22] */
	sw	x24, 100(sp)		/* r[23] */
	sw	x25, 104(sp)		/* r[24] */
	sw	x26, 108(sp)		/* r[25] */
	sw	x27, 112(sp)		/* r[26] */
	sw	x28, 116(sp)		/* r[27] */
	sw	x29, 120(sp)		/* r[28] */
	sw	x30, 124(sp)		/* r[29] */
	sw	x31, 128(sp)		/* r[30] */

	/* Save mepc and mstatus */
	csrr	t0, mepc
	sw	t0, 0(sp)		/* pc */
	csrr	t1, mstatus
	sw	t1, 4(sp)		/* mstatus */

	/* Check trap cause */
	csrr	t2, mcause
	blt	t2, zero, l_trap_interrupt

	/* Exception Handling (e.g. faults) */
	mv	a0, sp			/* sp points to SStackFrame */
	mv	a1, t2			/* mcause */
	call	Csym(knl_exception_handler)
	j	l_trap_return

l_trap_interrupt:
	/* Enter task independent mode */
	la	t0, Csym(knl_taskindp)
	lw	t1, 0(t0)
	addi	t1, t1, 1
	sw	t1, 0(t0)

	/* Increment nested count */
	la	t3, Csym(knl_int_nest)
	lw	t4, 0(t3)
	mv	s1, t4			/* Save old nest count in s1 */
	addi	t5, t4, 1
	sw	t5, 0(t3)

	/* Check if we should switch stack pointer to interrupt stack */
	bne	s1, zero, l_skip_sp_switch

	/* Switch to interrupt stack (knl_exc_stack) */
	mv	s2, sp			/* Save task SP in s2 */
	la	sp, Csym(knl_exc_stack)
	li	t0, 2048		/* CNF_EXC_STACK_SIZE */
	add	sp, sp, t0

l_skip_sp_switch:
	/* Call interrupt handler passing the interrupt number */
	csrr	a0, mcause
	andi	a0, a0, 0x3ff		/* Extract interrupt number */
	call	Csym(knl_interrupt_handler)

	/* Restore stack if we switched */
	bne	s1, zero, l_skip_sp_restore
	mv	sp, s2

l_skip_sp_restore:
	/* Decrement nest count */
	la	t3, Csym(knl_int_nest)
	lw	t4, 0(t3)
	addi	t4, t4, -1
	sw	t4, 0(t3)

	/* Decrement task independent counter */
	la	t0, Csym(knl_taskindp)
	lw	t1, 0(t0)
	addi	t1, t1, -1
	sw	t1, 0(t0)

	/* Check if dispatch is required */
	bne	t4, zero, l_trap_return

	la	t0, Csym(knl_dispatch_disabled)
	lw	t1, 0(t0)
	bne	t1, zero, l_trap_return

	la	t0, Csym(knl_ctxtsk)
	lw	t1, 0(t0)
	la	t2, Csym(knl_schedtsk)
	lw	t2, 0(t2)
	beq	t1, t2, l_trap_return

	/* Yes, we need to dispatch */
	/* If ctxtsk (t1) is NULL, we don't need to save task context, just dispatch */
	beq	t1, zero, l_dispatch_loop

	/* Save the task sp to ctxtsk->tskctxb.ssp, clear ctxtsk, and jump to l_dispatch_loop */
	sw	sp, 24(t1)		/* ctxtsk->tskctxb.ssp = sp */
	la	t0, Csym(knl_ctxtsk)
	sw	zero, 0(t0)
	j	l_dispatch_loop

l_trap_return:
	/* Restore PC and mstatus */
	lw	t0, 0(sp)		/* pc */
	csrw	mepc, t0
	lw	t1, 4(sp)		/* mstatus */
	li	t0, 0x1800		/* Force MPP = 3 (Machine Mode) */
	or	t1, t1, t0
	csrw	mstatus, t1

	/* Restore registers */
	lw	x1, 8(sp)
	lw	x3, 16(sp)
	lw	x4, 20(sp)
	lw	x5, 24(sp)
	lw	x6, 28(sp)
	lw	x7, 32(sp)
	lw	x8, 36(sp)
	lw	x9, 40(sp)
	lw	x10, 44(sp)
	lw	x11, 48(sp)
	lw	x12, 52(sp)
	lw	x13, 56(sp)
	lw	x14, 60(sp)
	lw	x15, 64(sp)
	lw	x16, 68(sp)
	lw	x17, 72(sp)
	lw	x18, 76(sp)
	lw	x19, 80(sp)
	lw	x20, 84(sp)
	lw	x21, 88(sp)
	lw	x22, 92(sp)
	lw	x23, 96(sp)
	lw	x24, 100(sp)
	lw	x25, 104(sp)
	lw	x26, 108(sp)
	lw	x27, 112(sp)
	lw	x28, 116(sp)
	lw	x29, 120(sp)
	lw	x30, 124(sp)
	lw	x31, 128(sp)

	addi	sp, sp, 132
	mret

#endif /* CPU_CORE_RISCV */
