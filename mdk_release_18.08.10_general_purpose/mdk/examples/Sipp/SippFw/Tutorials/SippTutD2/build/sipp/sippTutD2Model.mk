#Append SippSvuObj with pipeline specific Sipp-Filters
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/cvtColorRGBtoLuma/shave/svuCvtColorRGBtoLuma.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/cvtColorRGBtoLuma/arch/ma2x5x/shave/src/cvtColorRGBtoLuma.asm)
