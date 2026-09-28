/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

/*
 * Entry routine for the user application.
 * At this point, Initialize and start the user application.
 *
 * Entry routine is called from the initial task for Kernel,
 * so system call for stopping the task should not be issued 
 * from the contexts of entry routine.
 * We recommend that:
 * (1)'usermain()' only generates the user initial task.
 * (2)initialize and start the user application by the user
 * initial task.
 */

void task1_main(INT stacd, void *exinf)
{
	int count = 0;
	while(count < 5) {
		tm_printf((UB*)"[Task 1] Hello! Count = %d\n", count++);
		tk_dly_tsk(100); // Sleep for 100 ms
	}
	tm_printf((UB*)"[Task 1] Finished.\n");
	tk_ext_tsk();
}

void task2_main(INT stacd, void *exinf)
{
	int count = 0;
	while(count < 3) {
		tm_printf((UB*)"[Task 2] Hello! Count = %d\n", count++);
		tk_dly_tsk(150); // Sleep for 150 ms
	}
	tm_printf((UB*)"[Task 2] Finished.\n");
	tk_ext_tsk();
}

WEAK_FUNC EXPORT INT	usermain( void )
{
	T_CTSK ctsk1 = {
		.task = task1_main,
		.itskpri = 10,
		.stksz = 1024,
	};
	ID tsk1_id = tk_cre_tsk(&ctsk1);
	tk_sta_tsk(tsk1_id, 0);

	T_CTSK ctsk2 = {
		.task = task2_main,
		.itskpri = 10,
		.stksz = 1024,
	};
	ID tsk2_id = tk_cre_tsk(&ctsk2);
	tk_sta_tsk(tsk2_id, 0);

	/* Wait until both tasks finish, or just wait for a fixed time */
	tk_dly_tsk(600);
	
	tm_printf((UB*)"[usermain] Both tasks should be finished by now. Exiting...\n");
	return 0;
}
