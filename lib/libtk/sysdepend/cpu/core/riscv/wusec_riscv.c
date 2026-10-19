#include <sys/machine.h>
#ifdef CPU_CORE_RISCV

#include <tk/tkernel.h>
#include <sys/sysdef.h>

LOCAL UD rdtime_value(void)
{
#if RISCV_XLEN == 64
	UD value;
	asm volatile("rdtime %0" : "=r"(value));
	return value;
#else
	unsigned int lo, hi, hi2;
	do {
		asm volatile("rdtimeh %0" : "=r"(hi));
		asm volatile("rdtime %0" : "=r"(lo));
		asm volatile("rdtimeh %0" : "=r"(hi2));
	} while (hi != hi2);
	return ((UD)hi << 32) | lo;
#endif
}

#ifndef RISCV_TIMER_TICKS_PER_USEC
# ifdef RISCV_TIMER_HZ
#  define RISCV_TIMER_TICKS_PER_USEC (RISCV_TIMER_HZ / 1000000U)
# else
#  define RISCV_TIMER_TICKS_PER_USEC 1U
# endif
#endif

EXPORT void WaitUsec( UW usec )
{
	UD start = rdtime_value();
	UD ticks = (UD)usec * (UD)RISCV_TIMER_TICKS_PER_USEC;
	while ((rdtime_value() - start) < ticks);
}

EXPORT void WaitNsec( UW nsec )
{
	WaitUsec((nsec + 999) / 1000);
}

#endif /* CPU_CORE_RISCV */
