#include <sys/machine.h>
#ifdef IOTE_RISCV

#include "kernel.h"

EXPORT ER knl_init_device( void )
{
	return E_OK;
}

EXPORT ER knl_start_device( void )
{
	return E_OK;
}

EXPORT ER knl_finish_device( void )
{
	return E_OK;
}

#endif /* IOTE_RISCV */
