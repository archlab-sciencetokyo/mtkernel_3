#ifndef __SYS_SYSDEF_DEPEND_CORE_H__
#define __SYS_SYSDEF_DEPEND_CORE_H__

/* Definition of minimum system stack size */
#define MIN_SYS_STACK_SIZE	256
#define DEFAULT_SYS_STKSZ	MIN_SYS_STACK_SIZE

/* Internal Memory (Main RAM) */
#define INTERNAL_RAM_START	0x80000000
#define INTERNAL_RAM_SIZE	0x00040000	/* 256KB */
#define INTERNAL_RAM_END	(INTERNAL_RAM_START + INTERNAL_RAM_SIZE)

/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

/* Coprocessor and physical timer capabilities */
#define CPU_HAS_PTMR		1
#define CPU_HAS_FPU		0
#define CPU_HAS_DSP		0
#define NUM_COPROCESSOR		0

#define INTPRI_BITWIDTH		3

/* Number of Interrupt vectors */
#define N_INTVEC		32
#define N_SYSVEC		0

#endif /* __SYS_SYSDEF_DEPEND_CORE_H__ */
