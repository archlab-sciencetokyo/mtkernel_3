#ifndef __TK_CPUDEF_CORE_H__
#define __TK_CPUDEF_CORE_H__

#define TA_COPS			0
#define TA_FPU			0

typedef struct t_regs {
	UW r[31]; /* General purpose registers x1 - x31 */
} T_REGS;

typedef struct t_eit {
	void *pc;
	UW mstatus;
} T_EIT;

typedef struct t_cregs {
	void *ssp; /* System Stack Pointer */
} T_CREGS;

#endif /* __TK_CPUDEF_CORE_H__ */
