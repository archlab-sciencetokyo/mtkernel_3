#ifndef __TK_DBGSPT_DEPEND_CORE_H__
#define __TK_DBGSPT_DEPEND_CORE_H__

typedef struct td_calinf {
	void *ssp; /* System stack pointer */
	void *fp;  /* Frame pointer (x8/s0) when calling */
} TD_CALINF;

#endif /* __TK_DBGSPT_DEPEND_CORE_H__ */
