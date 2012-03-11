/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H_
#define _CPU_RENAME_H_

#define activate_r		_kernel_activate_r
#define dispatch		_kernel_dispatch
#define dev_vector		_kernel_dev_vector
#define exc_vector		_kernel_exc_vector
#define device_dispatcher	_kernel_device_dispatcher
#define interrupt_dispatcher	_kernel_interrupt_dispatcher
#define task_context		_kernel_task_context
#define expEntry		_kernel_expEntry
#define nmiEntry		_kernel_nmiEntry
#define ivTMREntry		_kernel_ivTMREntry
#define ivHWEntry		_kernel_ivHWEntry
#define ivg7Entry		_kernel_ivg7Entry
#define ivg8Entry		_kernel_ivg8Entry
#define ivg9Entry		_kernel_ivg9Entry
#define ivg10Entry		_kernel_ivg10Entry
#define ivg11Entry		_kernel_ivg11Entry
#define ivg12Entry		_kernel_ivg12Entry
#define ivg13Entry		_kernel_ivg13Entry
#define ivg14Entry		_kernel_ivg14Entry

#ifdef LABEL_ASM

#define _activate_r		__kernel_activate_r
#define _dispatch		__kernel_dispatch
#define _dev_vector		__kernel_dev_vector
#define _exc_vector		__kernel_exc_vector
#define _device_dispatcher	__kernel_device_dispatcher
#define _interrupt_dispatcher	__kernel_interrupt_dispatcher
#define _task_context		__kernel_task_context
#define _expEntry		__kernel_expEntry
#define _nmiEntry		__kernel_nmiEntry
#define _ivTMREntry		__kernel_ivTMREntry
#define _ivHWEntry		__kernel_ivHWEntry
#define _ivg7Entry		__kernel_ivg7Entry
#define _ivg8Entry		__kernel_ivg8Entry
#define _ivg9Entry		__kernel_ivg9Entry
#define _ivg10Entry		__kernel_ivg10Entry
#define _ivg11Entry		__kernel_ivg11Entry
#define _ivg12Entry		__kernel_ivg12Entry
#define _ivg13Entry		__kernel_ivg13Entry
#define _ivg14Entry		__kernel_ivg14Entry

#endif /* LABEL_ASM */
#endif /* _CPU_RENAME_H_ */
