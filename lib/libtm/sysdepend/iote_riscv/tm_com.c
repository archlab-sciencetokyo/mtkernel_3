#include <tk/tkernel.h>

#if USE_TMONITOR
#include "../../libtm.h"

#define UART0_THR	((volatile unsigned char*)0x10000000)
#define UART0_LSR	((volatile unsigned char*)0x10000005)

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	INT i;
	for (i = 0; i < size; i++) {
		/* Wait for Transmitter Holding Register Empty (bit 5) */
		while ((*UART0_LSR & 0x20) == 0);
		*UART0_THR = buf[i];
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	/* Stub receive */
}

EXPORT	void	tm_com_init(void)
{
	/* No init needed for default UART */
}

#endif /* USE_TMONITOR */
