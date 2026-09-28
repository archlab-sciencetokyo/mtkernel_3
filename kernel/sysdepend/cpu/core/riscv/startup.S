#include <sys/machine.h>
#ifdef CPU_CORE_RISCV

	.section .text.startup
	.global _start
_start:
	/* Disable interrupts */
	csrw	mstatus, zero
	csrw	mie, zero

	/* Set stack pointer */
	la	sp, _stack_top

	/* Set gp (global pointer) */
	.option push
	.option norelax
	la	gp, __global_pointer$
	.option pop

	/* Call Reset_Handler */
	call	Reset_Handler

	/* Loop forever if it returns */
l_loop:
	j	l_loop

#endif /* CPU_CORE_RISCV */
