#Append SippSvuObj with pipeline specific Sipp-Filters
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/scale05bilinHV/shave/svuScale05BilinHV.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05BilinHV_U8ToU8/arch/ma2x5x/shave/src/scale05BilinHV_U8ToU8.asm)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05BilinHV_U16ToU16/arch/ma2x5x/shave/src/scale05BilinHV_U16ToU16.asm)
