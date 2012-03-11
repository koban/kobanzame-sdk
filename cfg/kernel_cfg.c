/* Configured with [test.cfg ] */

#include "kernel_cfg.h"
#include "kernel_id.h"

#if TKERNEL_PRVER >= 0x1040
#define CFG_INTHDR_ENTRY(inthdr) INTHDR_ENTRY(inthdr)
#define CFG_EXCHDR_ENTRY(exchdr) EXCHDR_ENTRY(exchdr)
#define CFG_INT_ENTRY(inthdr) INT_ENTRY(inthdr)
#define CFG_EXC_ENTRY(exchdr) EXC_ENTRY(exchdr)
#else
#error "This configuration file has no compatibility with TOPPERS/JSP rel 1.3 or earlier."
#endif

#ifndef __EMPTY_LABEL
#define __EMPTY_LABEL(x,y) x y[0]
#endif

#if TKERNEL_PRID != 0x0001u /* TOPPERS/JSP */
#error "You can not use this configuration file without TOPPERS/JSP"
#endif

	/* User specified include files*/


	/* Object initializer [task] */

#define TNUM_TSKID 1

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

NULL static __STK_UNIT __stack_TASK1[__TCOUNT_STK_UNIT(128)];

const TINIB _kernel_tinib_table[TNUM_TSKID] = {
	{TA_HLNG, (VP_INT)(( VP_INT ) 1), (FP)(task), INT_PRIORITY(1), __TROUND_STK_UNIT(128), __stack_TASK1, TA_NULL, (FP)(NULL)}
};

const ID _kernel_torder_table[TNUM_TSKID] = {1};

TCB _kernel_tcb_table[TNUM_TSKID];


	/* Object initializer [semaphore] */

#define TNUM_SEMID 0

const ID _kernel_tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

__EMPTY_LABEL(const SEMINIB, _kernel_seminib_table);
__EMPTY_LABEL(SEMCB, _kernel_semcb_table);


	/* Object initializer [eventflag] */

#define TNUM_FLGID 0

const ID _kernel_tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

__EMPTY_LABEL(const FLGINIB, _kernel_flginib_table);
__EMPTY_LABEL(FLGCB, _kernel_flgcb_table);


	/* Object initializer [dataqueue] */

#define TNUM_DTQID 0

const ID _kernel_tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

__EMPTY_LABEL(const DTQINIB, _kernel_dtqinib_table);
__EMPTY_LABEL(DTQCB, _kernel_dtqcb_table);


	/* Object initializer [mailbox] */

#define TNUM_MBXID 0

const ID _kernel_tmax_mbxid = (TMIN_MBXID + TNUM_MBXID - 1);

__EMPTY_LABEL(const MBXINIB, _kernel_mbxinib_table);
__EMPTY_LABEL(MBXCB, _kernel_mbxcb_table);


	/* Object initializer [mempfix] */

#define TNUM_MPFID 0

const ID _kernel_tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

__EMPTY_LABEL(const MPFINIB, _kernel_mpfinib_table);
__EMPTY_LABEL(MPFCB, _kernel_mpfcb_table);


	/* Object initializer [cyclic] */

#define TNUM_CYCID 0

const ID _kernel_tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);

__EMPTY_LABEL(const CYCINIB, _kernel_cycinib_table);
__EMPTY_LABEL(CYCCB, _kernel_cyccb_table);


	/* Object initializer [interrupt] */

#define TNUM_INHNO 0

const UINT _kernel_tnum_inhno = TNUM_INHNO;

__EMPTY_LABEL(const INHINIB, _kernel_inhinib_table);

	/* Object initializer [exception] */

#define TNUM_EXCNO 0

const UINT _kernel_tnum_excno = TNUM_EXCNO;

__EMPTY_LABEL(const EXCINIB, _kernel_excinib_table);
	/* Initialization handler */

void
_kernel_call_inirtn(void)
{
}

void
_kernel_call_terrtn(void)
{
}

	/* Object initialization routine */

void
_kernel_object_initialize(void)
{
	_kernel_task_initialize();
}

TMEVTN   _kernel_tmevt_heap[TNUM_TSKID + TNUM_CYCID];

