/* This file is generated from sys_rename.def by genrename. */

#ifndef _SYS_RENAME_H_
#define _SYS_RENAME_H_

#define make_priority_mask	_kernel_make_priority_mask
#define device_dispatcher	_kernel_device_dispatcher
#define boot_for_gdb		_kernel_boot_for_gdb

#ifdef LABEL_ASM

#define _make_priority_mask	__kernel_make_priority_mask
#define _device_dispatcher	__kernel_device_dispatcher
#define _boot_for_gdb		__kernel_boot_for_gdb

#endif /* LABEL_ASM */
#endif /* _SYS_RENAME_H_ */
