#ifndef __DEFINES_SPARSE_H__
#define __DEFINES_SPARSE_H__

// Image size
#define MAX_WIDTH		320
#define MAX_HEIGHT		240

// Algorithm parameters
#define LINES_NUMBER    2
#define MAX_PADDING_8   8


#define DDR_DATA 	__attribute__((section(".ddr.data")))
#define DDR_BSS 	__attribute__((section(".ddr.bss")))
#define ALIGNED(x) 	__attribute__((aligned(x)))	
#define ENTRYPOINT extern "C"


#endif