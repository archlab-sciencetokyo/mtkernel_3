#include <sys/machine.h>
#ifdef CPU_CORE_RISCV

#include <tk/tkernel.h>

#ifdef RISCV_BOARD_RVCOMP
#define PLIC_THRESH_C0	(*(volatile unsigned int *)0x0c200000UL)
#endif

EXPORT void SetCpuIntLevel( INT level )
{
#ifdef RISCV_BOARD_RVCOMP
	/* PLIC Context 0 priority threshold: only interrupts with priority > threshold trigger */
	if (level < 0) {
		level = 0;
	} else if (level > 15) {
		level = 15;
	}
	PLIC_THRESH_C0 = (unsigned int)level;
#else
	/* No CPU interrupt level priority gating on generic RISC-V */
	(void)level;
#endif
}

EXPORT INT GetCpuIntLevel( void )
{
#ifdef RISCV_BOARD_RVCOMP
	return (INT)PLIC_THRESH_C0;
#else
	return INTLEVEL_EI;
#endif
}

/* Physical timer API is compiled only for targets that advertise it. */
#if USE_PTMR
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
#endif /* USE_PTMR */

#endif /* CPU_CORE_RISCV */
