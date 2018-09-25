MDK_INSTALL_DIR ?= ../../../common

include $(MDK_INSTALL_DIR)/mdk.mk

MAT_SIZE ?= 2048
MAT_SEED ?= 1
matrix:
	gcc -o PCmatrixGenerator/multiplyNxN PCmatrixGenerator/multiplyNxN.c
	./PCmatrixGenerator/multiplyNxN $(MAT_SIZE) $(MAT_SEED)


TEST_TYPE        := AUTO
TEST_TAGS        := "MA2150, LONG_RUN_TEST, MA2450"
