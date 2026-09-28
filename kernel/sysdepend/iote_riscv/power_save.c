#include <sys/machine.h>
#ifdef IOTE_RISCV

#include "kernel.h"

EXPORT void low_pow( void )
{
	/* Use RISC-V wait-for-interrupt */
	asm volatile("wfi");
}

EXPORT void off_pow( void )
{
	/* No off power support */
}

#endif /* IOTE_RISCV */
