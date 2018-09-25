#Append SippSvuObj with pipeline specific Sipp-Filters
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/arithmeticAdd/shave/svuArithmeticAdd.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/arithmeticAdd/arch/ma2x5x/shave/src/arithmeticAdd.asm)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/conv5x5/shave/svuConv5x5.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution5x5/arch/ma2x5x/shave/src/convolution5x5.asm)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/absdiff/shave/svuAbsdiff.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/absoluteDiff/arch/ma2x5x/shave/src/absoluteDiff.asm)
