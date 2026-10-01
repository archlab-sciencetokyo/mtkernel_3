#ifndef _SYSDEPEND_CPU_CORE_SYSTIMER_
#define _SYSDEPEND_CPU_CORE_SYSTIMER_

#include <tk/typedef.h>
#include <sys/sysdef.h>
#include "../../../iote_riscv/riscv_board.h"

#define TIMER_TICK_DIV (RISCV_TIMER_HZ / (1000 / TIMER_PERIOD))

Inline void knl_start_hw_timer( void )
{
	UINT imask;
	DI(imask);
	knl_riscv_timer_set_compare(knl_riscv_timer_read() + (UD)TIMER_TICK_DIV);
	asm volatile("csrs mie, %0" :: "r"((unsigned long)RISCV_MIE_MTIE) : "memory");
	EI(imask);
}

Inline void knl_clear_hw_timer_interrupt( void )
{
	knl_riscv_timer_set_compare(knl_riscv_timer_read() + (UD)TIMER_TICK_DIV);
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
}

Inline void knl_terminate_hw_timer( void )
{
	asm volatile("csrc mie, %0" :: "r"((unsigned long)RISCV_MIE_MTIE) : "memory");
}

Inline UW knl_get_hw_timer_nsec( void )
{
	UW ticks = (UW)knl_riscv_timer_read();
	return (UW)((ticks / RISCV_TIMER_NS_DEN) * RISCV_TIMER_NS_NUM
		+ ((ticks % RISCV_TIMER_NS_DEN) * RISCV_TIMER_NS_NUM)
		/ RISCV_TIMER_NS_DEN);
}

#endif /* _SYSDEPEND_CPU_CORE_SYSTIMER_ */
