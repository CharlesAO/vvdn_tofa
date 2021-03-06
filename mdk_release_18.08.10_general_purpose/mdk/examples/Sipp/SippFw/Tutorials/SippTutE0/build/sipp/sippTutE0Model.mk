#Append SippSvuObj with pipeline specific Sipp-Filters
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/absdiff/shave/svuAbsdiff.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/absoluteDiff/arch/ma2x5x/shave/src/absoluteDiff.asm)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/conv3x3/shave/svuConv3x3.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/conv3x3/shave/myriad2/svuConv3x3Opt.asm)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convolution3x3/arch/ma2x5x/shave/src/convolution3x3.asm)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/dilate5x5/shave/svuDilate5x5.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dilate5x5/arch/ma2x5x/shave/src/dilate5x5.asm)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/scale05bilinHV/shave/svuScale05BilinHV.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05BilinHV_U8ToU8/arch/ma2x5x/shave/src/scale05BilinHV_U8ToU8.asm)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/scale05BilinHV_U16ToU16/arch/ma2x5x/shave/src/scale05BilinHV_U16ToU16.asm)
