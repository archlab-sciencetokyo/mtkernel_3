#ifndef _SYSDEPEND_CPU_CORE_SYSTIMER_
#define _SYSDEPEND_CPU_CORE_SYSTIMER_

#define CLINT_MTIME_L		(*((volatile _UW*)0x6000bff8))
#define CLINT_MTIME_H		(*((volatile _UW*)0x6000bffc))
#define CLINT_MTIMECMP_L	(*((volatile _UW*)0x60004000))
#define CLINT_MTIMECMP_H	(*((volatile _UW*)0x60004004))

/* Timer tick count: Assuming 1MHz timer clock (1000 ticks = 1ms) */
#define TIMER_TICK_DIV		(1000000 / (1000 / TIMER_PERIOD))

/*
 * Timer start processing
 */
Inline void knl_start_hw_timer( void )
{
	UINT	imask;

	DI(imask);

	/* Set first compare value */
	UW low = CLINT_MTIME_L;
	UW high = CLINT_MTIME_H;

	UW next_low = low + TIMER_TICK_DIV;
	UW next_high = high + (next_low < low ? 1 : 0);

	CLINT_MTIMECMP_L = 0xFFFFFFFF; /* prevent spurious interrupt */
	CLINT_MTIMECMP_H = next_high;
	CLINT_MTIMECMP_L = next_low;

	/* Enable Machine Timer Interrupt (MTIE = bit 7 in mie) */
	asm volatile("csrs mie, %0" :: "r"(1 << 7));

	EI(imask);
}

/*
 * Clear timer interrupt
 */
Inline void knl_clear_hw_timer_interrupt( void )
{
	UW low = CLINT_MTIME_L;
	UW high = CLINT_MTIME_H;

	UW next_low = low + TIMER_TICK_DIV;
	UW next_high = high + (next_low < low ? 1 : 0);

	CLINT_MTIMECMP_L = 0xFFFFFFFF;
	CLINT_MTIMECMP_H = next_high;
	CLINT_MTIMECMP_L = next_low;
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
	/* No processing */
}

/*
 * Timer stop processing
 */
Inline void knl_terminate_hw_timer( void )
{
	/* Disable Machine Timer Interrupt (MTIE = bit 7 in mie) */
	asm volatile("csrc mie, %0" :: "r"(1 << 7));
}

/*
 * Get processing time from the previous timer interrupt to current (nanosecond)
 */
Inline UW knl_get_hw_timer_nsec( void )
{
	return 0;
}

#endif /* _SYSDEPEND_CPU_CORE_SYSTIMER_ */
