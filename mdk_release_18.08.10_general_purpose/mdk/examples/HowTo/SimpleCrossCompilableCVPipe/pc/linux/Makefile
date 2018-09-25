MV_COMMON_BASE ?= ../../../../../common
MV_PACKAGES_BASE = ../../../../../packages/movidius
APP_NAME = SimpleCrossCompilableCVPipe
CFLAGS = -Wall -Wno-unknown-pragmas -O2 -g -fpermissive -D__PC__

include $(MV_COMMON_BASE)/saferemove.mk

INCFLAGS = 	-I$(MV_COMMON_BASE)/components/kernelLib/MvCV/include \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/dmaModel \
		-I$(MV_COMMON_BASE)/swCommon/shave_code/myriad2/include \
		-I$(MV_COMMON_BASE)/shared/include \
		-I$(MV_COMMON_BASE)/swCommon/pcModel/half \
		-I$(MV_COMMON_BASE)/swCommon/include \
		-I$(MV_COMMON_BASE)/shared/include \
		-I$(MV_COMMON_BASE)/drivers/myriad2/socDrivers/shared/include \
		-I$(MV_PACKAGES_BASE)/pcModel/moviCompile/compilerIntrinsics/include \
		-I$(MV_PACKAGES_BASE)/pcModel/moviCompile/compilerVectorFunctions/include \
		-I../../myriad/shave/kernels/convolution3x3 \
		-I../../myriad/shave/kernels/convolution7x7 \
		-I../../myriad/shave/kernels/absoluteDiff \
		-I../Modules/Main \
		-I../../myriad/shave \
		-I../../myriad/shared

APP_SRC_DIRS = $(MV_COMMON_BASE)/swCommon/pcModel/dmaModel/ \
		../../myriad/shave/kernels/absoluteDiff/ \
		../../myriad/shave/kernels/convolution3x3/ \
		../../myriad/shave/kernels/convolution7x7/ \
		../../myriad/shave/ \
		../Modules/Main/ 

VPATH += $(APP_SRC_DIRS)		
APP_OBJ = svuCommonShave.o swcCdmaShaveM2.o absoluteDiff_intr.o convolution3x3_intr.o convolution7x7_intr.o SimpleCrossCompilableCVPipe.o Main.o 
APP_OBJS = $(patsubst %, output/%, $(APP_OBJ))

ifndef UNAME
UNAME := $(shell uname -o)
endif

ifeq "$(UNAME)" "GNU/Linux"
# Linux platform detected. Select appropriate path
LIB_PREBUILD_DIR = $(MV_COMMON_BASE)/../packages/movidius/pcModel/moviCompile/compilerIntrinsics/lib/linux64
RUN = ./$(APP_NAME)
else
# Not Linux
LIB_PREBUILD_DIR = $(MV_COMMON_BASE)/../packages/movidius/pcModel/moviCompile/compilerIntrinsics/lib/win64
RUN = ./$(APP_NAME).exe
endif

$(APP_NAME):	$(APP_OBJS)  $(LIB_PREBUILD_DIR)/libintrinsics.a
	c++ $(CFLAGS) -o $(APP_NAME) $(APP_OBJS) $(LIB_PREBUILD_DIR)/libintrinsics.a  $(INCFLAGS)

output/%.o:	%.c
	@mkdir -p output
	c++ $(CFLAGS) -c $< $(INCFLAGS) -o output/$*.o

output/%.o:	%.cpp
	@mkdir -p output
	c++ $(CFLAGS) -c $< $(INCFLAGS)	-o output/$*.o

all:	$(APP_NAME)

clean: 
	@$(call SAFE_RM, output)
	@$(call SAFE_RM, $(APP_NAME))
	@$(call SAFE_RM, $(APP_NAME).exe)
	@$(call SAFE_RM, *.yuv)
run:	
	$(RUN)

