MV_COMMON_BASE   ?= ../../../../../common
MV_PACKAGES_BASE = ../../../../../packages/movidius
APP_NAME = DisparityMap
CFLAGS = -Wall -Wno-unknown-pragmas -O2 -g -fpermissive -D__PC__

include $(MV_COMMON_BASE)/saferemove.mk

INCFLAGS =      -I$(MV_COMMON_BASE)/components/kernelLib/MvCV/include \
		-I$(MV_COMMON_BASE)/drivers/myriad2/socDrivers/shared/include \
		-I$(MV_COMMON_BASE)/swCommon/include \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/dmaModel \
		-I$(MV_COMMON_BASE)/swCommon/shave_code/myriad2/include \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/crc \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/half \
		-I$(MV_PACKAGES_BASE)/pcModel/moviCompile/compilerIntrinsics/include \
		-I$(MV_PACKAGES_BASE)/pcModel/moviCompile/compilerVectorFunctions/include \
		-I$(MV_COMMON_BASE)/shared/include \
		-I$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusTransform5x5/shave/include \
		-I$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMatching64/shave/include \
		-I$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMin64/shave/include \
		-I$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/matrixInv3x3_fp32/shave/include \
		-I$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convertYUV400ToYUV422/shave/include \
		-I../src/ \
		-I../../myriad/shave \
		-I../../myriad/shared
		
APP_SRC_DIRS = $(MV_COMMON_BASE)/swCommon/pcModel/dmaModel/ \
		$(MV_COMMON_BASE)/swCommon/pcModel/crc/ \
		$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusTransform5x5/shave/src/cpp \
		$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMatching64/shave/src/cpp \
		$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/censusMin64/shave/src/cpp \
		$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/matrixInv3x3_fp32/shave/src/cpp \
		$(MV_COMMON_BASE)/components/kernelLib/MvCV/kernels/convertYUV400ToYUV422/shave/src/cpp \
		../../myriad/shave/ \
		../src/

VPATH += $(APP_SRC_DIRS)		
APP_OBJ = svuCommonShave.o swcCdmaShaveM2.o censusTransform5x5.o censusMatching64.o censusMin64.o Main.o DisparityMap.o Helpers.o matrixInv3x3_fp32.o Preprocessing.o convertYUV400ToYUV422.o swcCrc.o
APP_OBJS = $(patsubst %, output/%, $(APP_OBJ))
		
$(APP_NAME):	$(APP_OBJS)  
	c++ $(CFLAGS) -o $(APP_NAME) $(APP_OBJS) $(INCFLAGS)

	
output/%.o:	%.c
	@mkdir -p output
	c++ $(CFLAGS) -c $< $(INCFLAGS) -o output/$*.o

output/%.o:	%.cpp
	@mkdir -p output
	c++ $(CFLAGS) -c $< $(INCFLAGS) -o output/$*.o

all:	$(APP_NAME)

clean: 
	$(call SAFE_RM,output)
	$(call SAFE_RM,$(APP_NAME))
	
run:
	./$(APP_NAME)

TEST_TYPE := "MANUAL"
