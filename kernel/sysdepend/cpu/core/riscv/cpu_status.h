#ifndef _SYSDEPEND_CPU_CORE_STATUS_
#define _SYSDEPEND_CPU_CORE_STATUS_

#include <tk/syslib.h>
#include <sys/sysdef.h>

#include "sysdepend.h"

/*
 * Start/End critical section
 */
#define BEGIN_CRITICAL_SECTION	{ UINT _mstatus_ = disint();
#define END_CRITICAL_SECTION	if ( !isDI(_mstatus_)			\
				  && knl_ctxtsk != knl_schedtsk		\
				  && !knl_dispatch_disabled ) {		\
					knl_dispatch();			\
				}					\
				restore_mstatus(_mstatus_); }

/*
 * Start/End interrupt disable section
 */
#define BEGIN_DISABLE_INTERRUPT	{ UINT _mstatus_ = disint();
#define END_DISABLE_INTERRUPT	restore_mstatus(_mstatus_); }

/*
 * Interrupt enable/disable
 */
#define ENABLE_INTERRUPT	{ asm volatile("csrrs x0, mstatus, 8"); }
#define DISABLE_INTERRUPT	{ disint(); }

/*
 * Enable interrupt nesting
 */
#define ENABLE_INTERRUPT_UPTO(level)	{ ENABLE_INTERRUPT; }

/*
 *  Task-independent control
 */
IMPORT	W	knl_taskindp;		/* Task independent status */

/*
 * If it is the task-independent part, TRUE
 */
Inline BOOL knl_isTaskIndependent( void )
{
	return ( knl_taskindp > 0 )? TRUE: FALSE;
}
/*
 * Move to/Restore task independent part
 */
Inline void knl_EnterTaskIndependent( void )
{
	knl_taskindp++;
}
Inline void knl_LeaveTaskIndependent( void )
{
	knl_taskindp--;
}

/*
 * Move to/Restore task independent part
 */
#define ENTER_TASK_INDEPENDENT	{ knl_EnterTaskIndependent(); }
#define LEAVE_TASK_INDEPENDENT	{ knl_LeaveTaskIndependent(); }

/* ----------------------------------------------------------------------- */
/*
 *	Check system state
 */

/* Helper to check if interrupts are disabled */
Inline BOOL knl_check_di(void)
{
	UW mstatus;
	asm volatile("csrr %0, mstatus" : "=r"(mstatus));
	return (mstatus & 8) == 0;
}

/*
 * When a system call is called from the task independent part, TRUE
 */
#define in_indp()	( knl_isTaskIndependent() || knl_ctxtsk == NULL )

/*
 * When a system call is called during dispatch disable, TRUE
 * Also include the task independent part as during dispatch disable.
 */
#define in_ddsp()	( knl_dispatch_disabled	\
			|| in_indp()		\
			|| knl_check_di() )

/*
 * When a system call is called during CPU lock (interrupt disable), TRUE
 * Also include the task independent part as during CPU lock.
 */
#define in_loc()	( knl_check_di()		\
			|| in_indp() )

/*
 * When a system call is called during executing the quasi task part, TRUE
 */
#define in_qtsk()	( knl_ctxtsk->sysmode > knl_ctxtsk->isysmode )

#endif /* _SYSDEPEND_CPU_CORE_STATUS_ */
