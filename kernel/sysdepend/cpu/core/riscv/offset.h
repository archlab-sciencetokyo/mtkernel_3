#ifndef _OFFSET_
#define _OFFSET_

/*----------------------------------------------------------------------*/
/*	offset data in TCB						*/
/*----------------------------------------------------------------------*/

#define TCB_tskid	8
#define TCB_tskatr	16
#if RISCV_XLEN == 64
#define TCB_tskctxb	48
#else
#define TCB_tskctxb	24
#endif
#define TCB_state	39

#define CTXB_ssp	0

#endif /* _OFFSET_ */
