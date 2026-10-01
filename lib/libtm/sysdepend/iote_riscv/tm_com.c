#include <tk/tkernel.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef RISCV_BOARD_RVCOMP
#define UART0_THR	((volatile unsigned char*)0x10000000)
#define UART0_TXFULL	((volatile unsigned char*)0x10000004)
#define UART0_RXEMPTY	((volatile unsigned char*)0x10000008)
#else
#define UART0_THR	((volatile unsigned char*)0x10000000)
#define UART0_LSR	((volatile unsigned char*)0x10000005)
#endif

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	INT i;
	for (i = 0; i < size; i++) {
#ifdef RISCV_BOARD_RVCOMP
		/* RVComp exposes TX-full as a separate status register. */
		while (*UART0_TXFULL != 0);
#else
		/* Wait for Transmitter Holding Register Empty (bit 5). */
		while ((*UART0_LSR & 0x20) == 0);
#endif
		*UART0_THR = buf[i];
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
#ifdef RISCV_BOARD_RVCOMP
	INT i;
	for (i = 0; i < size; i++) {
		while (*UART0_RXEMPTY != 0);
		buf[i] = *UART0_THR;
	}
#else
	/* Stub receive */
	(void)buf;
	(void)size;
#endif
}

EXPORT	void	tm_com_init(void)
{
	/* No init needed for default UART */
}

#endif /* USE_TMONITOR */
