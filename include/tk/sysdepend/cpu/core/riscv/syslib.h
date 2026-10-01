#ifndef __TK_SYSLIB_DEPEND_CORE_H__
#define __TK_SYSLIB_DEPEND_CORE_H__

#include <tk/errno.h>
#include <sys/sysdef.h>

Inline unsigned long disint(void)
{
	unsigned long mstatus;
	asm volatile("csrrci %0, mstatus, 8" : "=r"(mstatus));
	return mstatus;
}

Inline void restore_mstatus(unsigned long mstatus)
{
	if (mstatus & 8) {
		asm volatile("csrrs x0, mstatus, 8"); /* Set MIE */
	} else {
		asm volatile("csrrc x0, mstatus, 8"); /* Clear MIE */
	}
}

#define DI(intsts)		( (intsts) = disint() )
#define EI(intsts)		( restore_mstatus(intsts) )
#define isDI(intsts)		( ((intsts) & 8) == 0 )

#define INTLEVEL_DI		(0)
#define INTLEVEL_EI		(1)

#define DINTNO(intvec)		(intvec)

#endif /* __TK_SYSLIB_DEPEND_CORE_H__ */
