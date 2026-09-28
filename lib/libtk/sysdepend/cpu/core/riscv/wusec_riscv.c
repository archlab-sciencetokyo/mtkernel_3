#include <sys/machine.h>
#ifdef CPU_CORE_RISCV

#include <tk/tkernel.h>

LOCAL unsigned long long rdtime(void)
{
	unsigned int lo, hi, hi2;
	do {
		asm volatile("rdtimeh %0" : "=r"(hi));
		asm volatile("rdtime %0" : "=r"(lo));
		asm volatile("rdtimeh %0" : "=r"(hi2));
	} while (hi != hi2);
	return ((unsigned long long)hi << 32) | lo;
}

EXPORT void WaitUsec( UW usec )
{
	unsigned long long start = rdtime();
	while ((rdtime() - start) < (unsigned long long)usec);
}

EXPORT void WaitNsec( UW nsec )
{
	WaitUsec((nsec + 999) / 1000);
}

#endif /* CPU_CORE_RISCV */
