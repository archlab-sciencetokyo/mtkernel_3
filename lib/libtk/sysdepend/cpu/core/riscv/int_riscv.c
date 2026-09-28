#include <sys/machine.h>
#ifdef CPU_CORE_RISCV

#include <tk/tkernel.h>

EXPORT void SetCpuIntLevel( INT level )
{
	/* No CPU interrupt level priority gating on generic RISC-V */
}

EXPORT INT GetCpuIntLevel( void )
{
	return INTLEVEL_EI;
}

/*
 * Physical Timer Stubs
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode )
{
	return E_NOSPT;
}

EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	return E_NOSPT;
}

EXPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	return E_NOSPT;
}

EXPORT ER DefinePhysicalTimerHandler( UINT ptmrno, CONST T_DPTMR *pk_dptmr )
{
	return E_NOSPT;
}

EXPORT ER GetPhysicalTimerConfig( UINT ptmrno, T_RPTMR *pk_rptmr )
{
	return E_NOSPT;
}

#endif /* CPU_CORE_RISCV */
