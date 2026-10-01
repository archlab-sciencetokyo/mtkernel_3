#ifndef __TK_CPUDEF_CORE_H__
#define __TK_CPUDEF_CORE_H__

#include <sys/sysdef.h>

#if RISCV_XLEN == 64
typedef UD RISCV_REG;
#else
typedef UW RISCV_REG;
#endif

#define TA_COPS			0
#define TA_FPU			0

typedef struct t_regs {
	RISCV_REG r[31]; /* General purpose registers x1 - x31 */
} T_REGS;

typedef struct t_eit {
	void *pc;
	RISCV_REG mstatus;
} T_EIT;

typedef struct t_cregs {
	void *ssp; /* System Stack Pointer */
} T_CREGS;

#endif /* __TK_CPUDEF_CORE_H__ */
