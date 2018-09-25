#Append SippSvuObj with pipeline specific Sipp-Filters
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/erode3x3/shave/svuErode3x3.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/erode3x3/arch/$(MV_SOC_REV_DIR)/shave/src/erode3x3.asm)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/dilate3x3/shave/svuDilate3x3.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/dilate3x3/arch/$(MV_SOC_REV_DIR)/shave/src/dilate3x3.asm)
SippSvuObj += $(patsubst %.c, $(DirAppObjBase)%_shave.o, $(SIPP_PATH_1)/filters/sobel/shave/svuSobel.c)
SippSvuObj += $(patsubst %.asm, $(DirAppObjBase)%_shave.o,$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/sobel/arch/$(MV_SOC_REV_DIR)/shave/src/sobel.asm)
