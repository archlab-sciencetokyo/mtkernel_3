#include <sys/machine.h>
#ifdef CPU_CORE_RISCV

#define _in_asm_source_
#include <config.h>
#include <sys/sysdef.h>
#include <sys/knldef.h>
#include "offset.h"

#if RISCV_XLEN == 64
# define RV_LOAD ld
# define RV_STORE sd
#else
# define RV_LOAD lw
# define RV_STORE sw
#endif

.equ RV_REG_BYTES, (RISCV_XLEN / 8)
.equ RV_FRAME_SIZE, (33 * RV_REG_BYTES)
.equ RV_PC_OFF, 0
.equ RV_STATUS_OFF, RV_REG_BYTES
.equ RV_R0_OFF, (2 * RV_REG_BYTES)
.equ RV_R1_OFF, (3 * RV_REG_BYTES)
.equ RV_R2_OFF, (4 * RV_REG_BYTES)
.equ RV_R3_OFF, (5 * RV_REG_BYTES)
.equ RV_R4_OFF, (6 * RV_REG_BYTES)
.equ RV_R5_OFF, (7 * RV_REG_BYTES)
.equ RV_R6_OFF, (8 * RV_REG_BYTES)
.equ RV_R7_OFF, (9 * RV_REG_BYTES)
.equ RV_R8_OFF, (10 * RV_REG_BYTES)
.equ RV_R9_OFF, (11 * RV_REG_BYTES)
.equ RV_R10_OFF, (12 * RV_REG_BYTES)
.equ RV_R11_OFF, (13 * RV_REG_BYTES)
.equ RV_R12_OFF, (14 * RV_REG_BYTES)
.equ RV_R13_OFF, (15 * RV_REG_BYTES)
.equ RV_R14_OFF, (16 * RV_REG_BYTES)
.equ RV_R15_OFF, (17 * RV_REG_BYTES)
.equ RV_R16_OFF, (18 * RV_REG_BYTES)
.equ RV_R17_OFF, (19 * RV_REG_BYTES)
.equ RV_R18_OFF, (20 * RV_REG_BYTES)
.equ RV_R19_OFF, (21 * RV_REG_BYTES)
.equ RV_R20_OFF, (22 * RV_REG_BYTES)
.equ RV_R21_OFF, (23 * RV_REG_BYTES)
.equ RV_R22_OFF, (24 * RV_REG_BYTES)
.equ RV_R23_OFF, (25 * RV_REG_BYTES)
.equ RV_R24_OFF, (26 * RV_REG_BYTES)
.equ RV_R25_OFF, (27 * RV_REG_BYTES)
.equ RV_R26_OFF, (28 * RV_REG_BYTES)
.equ RV_R27_OFF, (29 * RV_REG_BYTES)
.equ RV_R28_OFF, (30 * RV_REG_BYTES)
.equ RV_R29_OFF, (31 * RV_REG_BYTES)
.equ RV_R30_OFF, (32 * RV_REG_BYTES)

.macro save_context pc_from_csr=0
	addi sp, sp, -RV_FRAME_SIZE
	RV_STORE x1, RV_R0_OFF(sp)
	addi a1, sp, RV_FRAME_SIZE
	RV_STORE a1, RV_R1_OFF(sp)
	RV_STORE x3, RV_R2_OFF(sp)
	RV_STORE x4, RV_R3_OFF(sp)
	RV_STORE x5, RV_R4_OFF(sp)
	RV_STORE x6, RV_R5_OFF(sp)
	RV_STORE x7, RV_R6_OFF(sp)
	RV_STORE x8, RV_R7_OFF(sp)
	RV_STORE x9, RV_R8_OFF(sp)
	RV_STORE x10, RV_R9_OFF(sp)
	RV_STORE x11, RV_R10_OFF(sp)
	RV_STORE x12, RV_R11_OFF(sp)
	RV_STORE x13, RV_R12_OFF(sp)
	RV_STORE x14, RV_R13_OFF(sp)
	RV_STORE x15, RV_R14_OFF(sp)
	RV_STORE x16, RV_R15_OFF(sp)
	RV_STORE x17, RV_R16_OFF(sp)
	RV_STORE x18, RV_R17_OFF(sp)
	RV_STORE x19, RV_R18_OFF(sp)
	RV_STORE x20, RV_R19_OFF(sp)
	RV_STORE x21, RV_R20_OFF(sp)
	RV_STORE x22, RV_R21_OFF(sp)
	RV_STORE x23, RV_R22_OFF(sp)
	RV_STORE x24, RV_R23_OFF(sp)
	RV_STORE x25, RV_R24_OFF(sp)
	RV_STORE x26, RV_R25_OFF(sp)
	RV_STORE x27, RV_R26_OFF(sp)
	RV_STORE x28, RV_R27_OFF(sp)
	RV_STORE x29, RV_R28_OFF(sp)
	RV_STORE x30, RV_R29_OFF(sp)
	RV_STORE x31, RV_R30_OFF(sp)
.if \pc_from_csr
	csrr t0, mepc
	csrr t1, mstatus
.else
	mv t0, x1
	mv t1, a0
.endif
	RV_STORE t0, RV_PC_OFF(sp)
	RV_STORE t1, RV_STATUS_OFF(sp)
.endm

.macro restore_context
	RV_LOAD t0, RV_PC_OFF(sp)
	csrw mepc, t0
	RV_LOAD t0, RV_STATUS_OFF(sp)
	li t1, RISCV_MSTATUS_MPP
	or t0, t0, t1
	csrw mstatus, t0
	RV_LOAD x1, RV_R0_OFF(sp)
	RV_LOAD x3, RV_R2_OFF(sp)
	RV_LOAD x4, RV_R3_OFF(sp)
	RV_LOAD x5, RV_R4_OFF(sp)
	RV_LOAD x6, RV_R5_OFF(sp)
	RV_LOAD x7, RV_R6_OFF(sp)
	RV_LOAD x8, RV_R7_OFF(sp)
	RV_LOAD x9, RV_R8_OFF(sp)
	RV_LOAD x10, RV_R9_OFF(sp)
	RV_LOAD x11, RV_R10_OFF(sp)
	RV_LOAD x12, RV_R11_OFF(sp)
	RV_LOAD x13, RV_R12_OFF(sp)
	RV_LOAD x14, RV_R13_OFF(sp)
	RV_LOAD x15, RV_R14_OFF(sp)
	RV_LOAD x16, RV_R15_OFF(sp)
	RV_LOAD x17, RV_R16_OFF(sp)
	RV_LOAD x18, RV_R17_OFF(sp)
	RV_LOAD x19, RV_R18_OFF(sp)
	RV_LOAD x20, RV_R19_OFF(sp)
	RV_LOAD x21, RV_R20_OFF(sp)
	RV_LOAD x22, RV_R21_OFF(sp)
	RV_LOAD x23, RV_R22_OFF(sp)
	RV_LOAD x24, RV_R23_OFF(sp)
	RV_LOAD x25, RV_R24_OFF(sp)
	RV_LOAD x26, RV_R25_OFF(sp)
	RV_LOAD x27, RV_R26_OFF(sp)
	RV_LOAD x28, RV_R27_OFF(sp)
	RV_LOAD x29, RV_R28_OFF(sp)
	RV_LOAD x30, RV_R29_OFF(sp)
	RV_LOAD x31, RV_R30_OFF(sp)
	addi sp, sp, RV_FRAME_SIZE
.endm

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
	la sp, Csym(knl_tmp_stack)
	li t0, TMP_STACK_SIZE
	add sp, sp, t0
	la t0, Csym(knl_dispatch_disabled)
	li t1, 1
	RV_STORE t1, 0(t0)
	la t0, Csym(knl_ctxtsk)
	RV_STORE zero, 0(t0)
	j l_dispatch_loop

Csym(knl_dispatch_entry):
	li t0, RISCV_MSTATUS_MIE
	csrrc a0, mstatus, t0
	save_context 0
	la t0, Csym(knl_ctxtsk)
	RV_LOAD t1, 0(t0)
	RV_STORE sp, (TCB_tskctxb + CTXB_ssp)(t1)
	RV_STORE zero, 0(t0)

l_dispatch_loop:
	li t0, RISCV_MSTATUS_MIE
	csrrc a0, mstatus, t0
	la t0, Csym(knl_schedtsk)
	RV_LOAD t2, 0(t0)
	bne t2, zero, l_dispatch_switch
	la t3, Csym(knl_lowpow_discnt)
	RV_LOAD t3, 0(t3)
	bne t3, zero, l_no_lowpow
	li t0, RISCV_MSTATUS_MIE
	csrrs zero, mstatus, t0
	call Csym(low_pow)
	j l_dispatch_loop
l_no_lowpow:
	csrrs zero, mstatus, t0
	wfi
	j l_dispatch_loop

l_dispatch_switch:
	la t0, Csym(knl_ctxtsk)
	RV_STORE t2, 0(t0)
	RV_LOAD sp, TCB_tskctxb + CTXB_ssp(t2)
	restore_context
	la t0, Csym(knl_dispatch_disabled)
	RV_STORE zero, 0(t0)
	mret

Csym(knl_trap_handler):
	save_context 1
	csrr t2, mcause
	bltz t2, l_trap_interrupt
	mv a0, sp
	mv a1, t2
	call Csym(knl_exception_handler)
	j l_trap_return

l_trap_interrupt:
	la t0, Csym(knl_taskindp)
	RV_LOAD t1, 0(t0)
	addi t1, t1, 1
	RV_STORE t1, 0(t0)
	la t3, Csym(knl_int_nest)
	RV_LOAD t4, 0(t3)
	mv s1, t4
	addi t5, t4, 1
	RV_STORE t5, 0(t3)
	bne s1, zero, l_skip_sp_switch
	mv s2, sp
	la sp, Csym(knl_exc_stack)
	li t0, EXC_STACK_SIZE
	add sp, sp, t0
l_skip_sp_switch:
	csrr a0, mcause
	li t0, RISCV_MCAUSE_CODE_MASK
	and a0, a0, t0
	call Csym(knl_interrupt_handler)
	la t3, Csym(knl_int_nest)
	bne s1, zero, l_skip_sp_restore
	mv sp, s2
l_skip_sp_restore:
	RV_LOAD t4, 0(t3)
	addi t4, t4, -1
	RV_STORE t4, 0(t3)
	la t0, Csym(knl_taskindp)
	RV_LOAD t1, 0(t0)
	addi t1, t1, -1
	RV_STORE t1, 0(t0)

	beq t4, zero, l_check_dispatch
	j l_trap_return
l_check_dispatch:
	la t0, Csym(knl_dispatch_disabled)
	RV_LOAD t1, 0(t0)
	bne t1, zero, l_trap_return
	la t0, Csym(knl_ctxtsk)
	RV_LOAD t1, 0(t0)
	la t2, Csym(knl_schedtsk)
	RV_LOAD t2, 0(t2)
	beq t1, t2, l_trap_return
	beq t1, zero, l_dispatch_loop
	RV_STORE sp, (TCB_tskctxb + CTXB_ssp)(t1)
	RV_STORE zero, 0(t0)
	j l_dispatch_loop

l_trap_return:
	restore_context
	mret

#endif /* CPU_CORE_RISCV */
