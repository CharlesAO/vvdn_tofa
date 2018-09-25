
MV_COMMON_BASE ?= ../../../..

MEST_COMP_API_DIR = $(MV_COMMON_BASE)/components/MEST/api

MEST_INC_DIR     = $(MEST_COMP_API_DIR)/pc
MEST_INC_DIR    += $(MEST_COMP_API_DIR)/shared
MEST_PC_SRC_DIR  = $(MEST_COMP_API_DIR)/pc

MEST_INC  = $(foreach d, $(MEST_INC_DIR), -I$d)
MEST_INC += -I${MV_COMMON_BASE}/shared/include/

CC_WRAPPER     = cl
GCC_WRAPPER    = g++

SRC_DIR_WRAPPER  = $(MEST_PC_SRC_DIR)
SRC_WRAPPER      = $(SRC_DIR_WRAPPER)/MotEst_wrapper.cpp
OBJS_WRAPPER     = $(SRC_DIR_WRAPPER)/MotEst_wrapper.o
MOTEST_WRAPPER_CXXFLAGS = $(MEST_INC) -fPIC -O2 -std=c++11 -Wall
MEST_LIB_NAME    = libmest.a
MOTEST_OBJS      = CModel.o Interpolation.o FME.o irqManager.o MotEstAccelerator.o
MEST_OBJS        = MestPcUse.o $(MOTEST_OBJS) $(OBJS_WRAPPER)

%.o: $(MEST_PC_SRC_DIR)/%.cpp
	$(GCC_WRAPPER) $(MOTEST_WRAPPER_CXXFLAGS) -c $^ -o $@

$(MEST_LIB_NAME) : $(OBJS_WRAPPER)
	$(AR) -x $(MEST_PC_SRC_DIR)/libmotest.a
	$(AR) -Urcs $@ $^ *.o
	$(RM) $(MOTEST_OBJS)

clean:
	$(RM) $(MEST_OBJS) $(MEST_LIB_NAME)

.PHONY: $(MEST_LIB_NAME) clean
