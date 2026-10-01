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

EXPORT void WaitUsec( UW usec )
{
	UD start = rdtime_value();
	while ((rdtime_value() - start) < (UD)usec);
}

EXPORT void WaitNsec( UW nsec )
{
	WaitUsec((nsec + 999) / 1000);
}

#endif /* CPU_CORE_RISCV */
