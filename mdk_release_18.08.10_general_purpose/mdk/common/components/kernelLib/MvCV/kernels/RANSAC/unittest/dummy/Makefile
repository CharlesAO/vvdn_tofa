RUN_ON_PC = yes
ComponentList += kernelLib/MvCV

# Include the common Makefile for the dummy application
include ../../../../shared/unittest/app_common.mk

ifeq ($(MV_SOC_REV),$(filter $(MV_SOC_REV),ma2480))
DirLDScript = config/myriad2/ma2x8x
LinkerScript = $(DirLDScript)/custom.ldscript
else
DirLDScript = config/myriad2
LinkerScript = $(DirLDScript)/custom.ldscript
endif
INT_DDR = yes