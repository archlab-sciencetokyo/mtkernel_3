#ifndef __SYS_PROFILE_CORE_H__
#define __SYS_PROFILE_CORE_H__

#define TK_ALLOW_MISALIGN	(0)		/* Memory misalign access is not permitted */
#define TK_BIGENDIAN		(0)		/* Is Little Endian */

/* FPU and co-processors */
#define TK_SUPPORT_FPU		FALSE		/* Support of FPU */
#define TK_SUPPORT_COP0		FALSE		/* Support of co-processor-0 */
#define TK_SUPPORT_COP1		FALSE
#define TK_SUPPORT_COP2		FALSE
#define TK_SUPPORT_COP3		FALSE

/* low-level function */
#define TK_SUPPORT_REGOPS	TRUE		/* Support of get/set register operation */
#define TK_SUPPORT_ASM		FALSE

/* Interrupt */
#define TK_SUPPORT_INTCTRL	FALSE		/* Support of interrupt controller management */
#define TK_HAS_ENAINTLEVEL	FALSE
#define TK_SUPPORT_CPUINTLEVEL	FALSE
#define TK_SUPPORT_CTRLINTLEVEL	FALSE
#define TK_SUPPORT_INTMODE	FALSE

/* Cache control */
#define TK_SUPPORT_CACHECTRL	FALSE
#define TK_SUPPORT_SETCACHEMODE	FALSE
#define TK_SUPPORT_WBCACHE	FALSE
#define TK_SUPPORT_WTCACHE	FALSE

/* Real memory protection level of TA_RNGn (0..3) */
#define TK_MEM_RNG0		0
#define TK_MEM_RNG1		0
#define TK_MEM_RNG2		0
#define TK_MEM_RNG3		0

/* Device Support */
#define TK_SUPPORT_MICROWAIT	TRUE		/* Support of micro wait */

/* Physical Timer Support */
#if USE_PTMR
#define TK_SUPPORT_PTIMER	TRUE
#else
#define TK_SUPPORT_PTIMER	FALSE
#endif

#endif /* __SYS_PROFILE_CORE_H__ */
