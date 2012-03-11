# Generated by the VisualDSP++ IDDE

# Note:  Any changes made to this Makefile will be lost the next time the
# matching project file is loaded into the IDDE.  If you wish to preserve
# changes, rename this file and run it externally to the IDDE.

# The syntax of this Makefile is such that GNU Make v3.77 or higher is
# required.

# The current working directory should be the directory in which this
# Makefile resides.

# Supported targets:
#     sample1_ezkit_bf537_Debug
#     sample1_ezkit_bf537_Debug_clean

# Define this variable if you wish to run this Makefile on a host
# other than the host that created it and VisualDSP++ may be installed
# in a different directory.

ADI_DSP=C:\Program Files\Analog Devices\VisualDSP 5.0


# $VDSP is a gmake-friendly version of ADI_DIR

empty:=
space:= $(empty) $(empty)
VDSP_INTERMEDIATE=$(subst \,/,$(ADI_DSP))
VDSP=$(subst $(space),\$(space),$(VDSP_INTERMEDIATE))

RM=cmd /C del /F /Q

#
# Begin "sample1_ezkit_bf537_Debug" configuration
#

ifeq ($(MAKECMDGOALS),sample1_ezkit_bf537_Debug)

sample1_ezkit_bf537_Debug : ./Debug/sample1_ezkit_bf537.dxe 

Debug/kernel_cfg.doj :kernel_cfg.c ../../../include/kernel_cfg.h ../../../kernel/../kernel/jsp_kernel.h ../../../kernel/../kernel/jsp_rename.h ../../../include/kernel.h ../../../include/t_stddef.h ../../../config/blackfin/tool_defs.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/yvals.h ../../../include/itron.h $(VDSP)/Blackfin/include/stddef.h ../../../config/blackfin/ezkit_bf537/sys_defs.h ../../../config/blackfin/_common_bf537/chip_defs.h ../../../config/blackfin/cpu_defs.h ../../../include/t_syslog.h ../../../include/t_config.h ../../../config/blackfin/ezkit_bf537/sys_config.h ../../../config/blackfin/_common_bf537/sys_rename.h ../../../config/blackfin/_common_bf537/chip_config.h ../../../config/blackfin/cpu_config.h ../../../config/blackfin/api.h ../../../config/blackfin/cdef_lpblackfin.h ../../../config/blackfin/def_lpblackfin.h ../../../config/blackfin/cpu_rename.h ../../../config/blackfin/tool_config.h ../../../kernel/../kernel/task.h ../../../kernel/../kernel/queue.h ../../../kernel/../kernel/time_event.h ../../../kernel/../kernel/semaphore.h ../../../kernel/../kernel/eventflag.h ../../../kernel/../kernel/dataqueue.h ../../../kernel/../kernel/mailbox.h ../../../kernel/../kernel/mempfix.h ../../../kernel/../kernel/cyclic.h ../../../kernel/../kernel/interrupt.h ../../../kernel/../kernel/exception.h ../../../kernel/../kernel/jsp_unrename.h ../../../config/blackfin/_common_bf537/sys_unrename.h ../../../config/blackfin/cpu_unrename.h kernel_id.h sample1.h ../../../include/t_services.h ../../../include/serial.h ../../../config/blackfin/_common_bf537/hw_timer.h ../../../include/s_services.h ../../../include/sil.h $(VDSP)/Blackfin/include/cdefbf53x.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h ../../../config/blackfin/cdefBF537.h ../../../config/blackfin/cdefbf534.h ../../../config/blackfin/defbf534.h ../../../config/blackfin/defbf537.h ../../../include/timer.h ../../../config/blackfin/_common_bf537/hw_serial.h ../../../pdic/simple_sio/uart.h ../../../include/logtask.h 
	@echo ".\kernel_cfg.c"
	$(VDSP)/ccblkfn.exe -c .\kernel_cfg.c -file-attr ProjectName=sample1_ezkit_bf537 -g -structs-do-not-overlap -no-multiline -I ..\..\..\kernel -I ..\..\..\include -I ..\..\..\systask -I ..\..\..\pdic\simple_sio -I ..\..\..\config\blackfin -I ..\..\..\config\blackfin\ezkit_bf537 -I ..\..\..\config\blackfin\_common_bf537 -double-size-32 -decls-strong -warn-protos -si-revision 0.3 -proc ADSP-BF537 -o .\Debug\kernel_cfg.doj -MM

./Debug/logtask.doj :../../../systask/logtask.c ../../../include/t_services.h ../../../include/kernel.h ../../../include/t_stddef.h ../../../config/blackfin/tool_defs.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/yvals.h ../../../include/itron.h $(VDSP)/Blackfin/include/stddef.h ../../../config/blackfin/ezkit_bf537/sys_defs.h ../../../config/blackfin/_common_bf537/chip_defs.h ../../../config/blackfin/cpu_defs.h ../../../include/t_syslog.h ../../../include/serial.h ../../../include/logtask.h ../../../include/t_config.h ../../../config/blackfin/ezkit_bf537/sys_config.h ../../../config/blackfin/_common_bf537/sys_rename.h ../../../config/blackfin/_common_bf537/chip_config.h ../../../config/blackfin/cpu_config.h ../../../config/blackfin/api.h ../../../config/blackfin/cdef_lpblackfin.h ../../../config/blackfin/def_lpblackfin.h ../../../config/blackfin/cpu_rename.h ../../../config/blackfin/tool_config.h 
	@echo "..\..\..\systask\logtask.c"
	$(VDSP)/ccblkfn.exe -c ..\..\..\systask\logtask.c -file-attr ProjectName=sample1_ezkit_bf537 -g -structs-do-not-overlap -no-multiline -I ..\..\..\kernel -I ..\..\..\include -I ..\..\..\systask -I ..\..\..\pdic\simple_sio -I ..\..\..\config\blackfin -I ..\..\..\config\blackfin\ezkit_bf537 -I ..\..\..\config\blackfin\_common_bf537 -double-size-32 -decls-strong -warn-protos -si-revision 0.3 -proc ADSP-BF537 -o .\Debug\logtask.doj -MM

Debug/sample1.doj :sample1.c ../../../include/t_services.h ../../../include/kernel.h ../../../include/t_stddef.h ../../../config/blackfin/tool_defs.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/yvals.h ../../../include/itron.h $(VDSP)/Blackfin/include/stddef.h ../../../config/blackfin/ezkit_bf537/sys_defs.h ../../../config/blackfin/_common_bf537/chip_defs.h ../../../config/blackfin/cpu_defs.h ../../../include/t_syslog.h ../../../include/serial.h kernel_id.h sample1.h 
	@echo ".\sample1.c"
	$(VDSP)/ccblkfn.exe -c .\sample1.c -file-attr ProjectName=sample1_ezkit_bf537 -g -structs-do-not-overlap -no-multiline -I ..\..\..\kernel -I ..\..\..\include -I ..\..\..\systask -I ..\..\..\pdic\simple_sio -I ..\..\..\config\blackfin -I ..\..\..\config\blackfin\ezkit_bf537 -I ..\..\..\config\blackfin\_common_bf537 -double-size-32 -decls-strong -warn-protos -si-revision 0.3 -proc ADSP-BF537 -o .\Debug\sample1.doj -MM

./Debug/serial.doj :../../../systask/serial.c ../../../include/t_services.h ../../../include/kernel.h ../../../include/t_stddef.h ../../../config/blackfin/tool_defs.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/yvals.h ../../../include/itron.h $(VDSP)/Blackfin/include/stddef.h ../../../config/blackfin/ezkit_bf537/sys_defs.h ../../../config/blackfin/_common_bf537/chip_defs.h ../../../config/blackfin/cpu_defs.h ../../../include/t_syslog.h ../../../include/serial.h ../../../config/blackfin/_common_bf537/hw_serial.h ../../../include/s_services.h ../../../include/sil.h ../../../include/t_config.h ../../../config/blackfin/ezkit_bf537/sys_config.h ../../../config/blackfin/_common_bf537/sys_rename.h ../../../config/blackfin/_common_bf537/chip_config.h ../../../config/blackfin/cpu_config.h ../../../config/blackfin/api.h ../../../config/blackfin/cdef_lpblackfin.h ../../../config/blackfin/def_lpblackfin.h ../../../config/blackfin/cpu_rename.h ../../../config/blackfin/tool_config.h ../../../pdic/simple_sio/uart.h $(VDSP)/Blackfin/include/cdefbf53x.h $(VDSP)/Blackfin/include/sys/platform.h $(VDSP)/Blackfin/include/sys/_adi_platform.h ../../../config/blackfin/cdefBF537.h ../../../config/blackfin/cdefbf534.h ../../../config/blackfin/defbf534.h ../../../config/blackfin/defbf537.h kernel_id.h 
	@echo "..\..\..\systask\serial.c"
	$(VDSP)/ccblkfn.exe -c ..\..\..\systask\serial.c -file-attr ProjectName=sample1_ezkit_bf537 -g -structs-do-not-overlap -no-multiline -I ..\..\..\kernel -I ..\..\..\include -I ..\..\..\systask -I ..\..\..\pdic\simple_sio -I ..\..\..\config\blackfin -I ..\..\..\config\blackfin\ezkit_bf537 -I ..\..\..\config\blackfin\_common_bf537 -double-size-32 -decls-strong -warn-protos -si-revision 0.3 -proc ADSP-BF537 -o .\Debug\serial.doj -MM

./Debug/uart.doj :../../../pdic/simple_sio/uart.c ../../../include/s_services.h ../../../include/sil.h ../../../include/t_stddef.h ../../../config/blackfin/tool_defs.h $(VDSP)/Blackfin/include/limits.h $(VDSP)/Blackfin/include/yvals.h ../../../include/itron.h $(VDSP)/Blackfin/include/stddef.h ../../../config/blackfin/ezkit_bf537/sys_defs.h ../../../config/blackfin/_common_bf537/chip_defs.h ../../../config/blackfin/cpu_defs.h ../../../include/t_syslog.h ../../../include/t_config.h ../../../config/blackfin/ezkit_bf537/sys_config.h ../../../config/blackfin/_common_bf537/sys_rename.h ../../../config/blackfin/_common_bf537/chip_config.h ../../../config/blackfin/cpu_config.h ../../../config/blackfin/api.h ../../../config/blackfin/cdef_lpblackfin.h ../../../config/blackfin/def_lpblackfin.h ../../../config/blackfin/cpu_rename.h ../../../config/blackfin/tool_config.h ../../../pdic/simple_sio/uart.h 
	@echo "..\..\..\pdic\simple_sio\uart.c"
	$(VDSP)/ccblkfn.exe -c ..\..\..\pdic\simple_sio\uart.c -file-attr ProjectName=sample1_ezkit_bf537 -g -structs-do-not-overlap -no-multiline -I ..\..\..\kernel -I ..\..\..\include -I ..\..\..\systask -I ..\..\..\pdic\simple_sio -I ..\..\..\config\blackfin -I ..\..\..\config\blackfin\ezkit_bf537 -I ..\..\..\config\blackfin\_common_bf537 -double-size-32 -decls-strong -warn-protos -si-revision 0.3 -proc ADSP-BF537 -o .\Debug\uart.doj -MM

./Debug/sample1_ezkit_bf537.dxe :./sample1.ldf $(VDSP)/Blackfin/lib/bf534_rev_0.3/libprofile532y.dlb ./Debug/kernel_cfg.doj ../../../config/blackfin/ezkit_bf537/libkernel.dlb ./Debug/logtask.doj ./Debug/sample1.doj ./Debug/serial.doj ./Debug/uart.doj $(VDSP)/Blackfin/lib/cplbtab537.doj $(VDSP)/Blackfin/lib/bf534_rev_0.3/crtn532y.doj $(VDSP)/Blackfin/lib/bf534_rev_0.3/libsmall532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libio532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libc532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libevent532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libx532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libcpp532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libcpprt532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libf64ieee532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libdsp532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libsftflt532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/libetsi532y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/Debug/libssl537y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/Debug/libdrv537y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/Debug/libusb537y.dlb $(VDSP)/Blackfin/lib/bf534_rev_0.3/idle532mty.doj $(VDSP)/Blackfin/lib/bf534_rev_0.3/librt_fileio532y.dlb 
	@echo "Linking..."
	$(VDSP)/ccblkfn.exe .\Debug\kernel_cfg.doj ..\..\..\config\blackfin\ezkit_bf537\libkernel.dlb .\Debug\logtask.doj .\Debug\sample1.doj .\Debug\serial.doj .\Debug\uart.doj -T .\sample1.ldf -L .\Debug -flags-link -MDUSER_CRT=ADI_QUOTEsample1_ezkit_bf537_basiccrt.dojADI_QUOTE,-MDUSE_FILEIO -add-debug-libpaths -flags-link -od,.\Debug -o .\Debug\sample1_ezkit_bf537.dxe -proc ADSP-BF537 -si-revision 0.3 -flags-link -MM

endif

ifeq ($(MAKECMDGOALS),sample1_ezkit_bf537_Debug_clean)

sample1_ezkit_bf537_Debug_clean:
	-$(RM) "Debug\kernel_cfg.doj"
	-$(RM) ".\Debug\logtask.doj"
	-$(RM) "Debug\sample1.doj"
	-$(RM) ".\Debug\serial.doj"
	-$(RM) ".\Debug\uart.doj"
	-$(RM) ".\Debug\sample1_ezkit_bf537.dxe"
	-$(RM) ".\Debug\*.ipa"
	-$(RM) ".\Debug\*.opa"
	-$(RM) ".\Debug\*.ti"
	-$(RM) ".\Debug\*.pgi"
	-$(RM) ".\*.rbld"

endif


