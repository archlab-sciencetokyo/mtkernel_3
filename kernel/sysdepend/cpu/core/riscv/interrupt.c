#include "kernel.h"
#include "../../../sysdepend.h"

#include <stddef.h>

EXPORT	W	knl_int_nest = 0;	/* Interrupt nest counter */

Noinit(LOCAL FP knl_inthdr_tbl[32]);	/* Simple interrupt handler table */

/*
 * High-level interrupt handler called from assembly trap entry
 */
EXPORT void knl_interrupt_handler(UW intno)
{
	FP	inthdr;

	if (intno == 7) {
		/* Machine Timer Interrupt (MTIP) */
		knl_timer_handler();
	} else if (intno < 32) {
		inthdr = knl_inthdr_tbl[intno];
		if (inthdr != NULL) {
			(*inthdr)(intno);
		}
	}
}

/*
 * Set interrupt handler (Used in tk_def_int())
 */
EXPORT ER knl_define_inthdr( INT intno, ATR intatr, FP inthdr )
{
	if (intno < 0 || intno >= 32) {
		return E_PAR;
	}

	knl_inthdr_tbl[intno] = inthdr;
	return E_OK;
}

/*
 * Return interrupt handler (Used in tk_ret_int())
 */
EXPORT void knl_return_inthdr(void)
{
	/* No processing */
	return;
}

/*
 * Interrupt initialize
 */
EXPORT ER knl_init_interrupt( void )
{
	INT i;
	for (i = 0; i < 32; i++) {
		knl_inthdr_tbl[i] = NULL;
	}
	return E_OK;
}
