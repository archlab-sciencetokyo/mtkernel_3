#include <sys/machine.h>
#ifdef IOTE_RISCV

#include <tk/tkernel.h>

EXPORT void startup_clock(void)
{
	/* No clock scaling needed for generic RISC-V target */
}

#endif /* IOTE_RISCV */
