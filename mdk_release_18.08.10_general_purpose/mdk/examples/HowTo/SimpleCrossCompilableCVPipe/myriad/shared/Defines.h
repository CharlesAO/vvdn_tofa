#ifndef __DEFINES_SPARSE_H__
#define __DEFINES_SPARSE_H__

// Image size
#define MAX_WIDTH		320//72//450
#define MAX_HEIGHT		240//60//375

// Algorithm parameters
#define CONV7x7_KERNEL_SIZE  7
#define CONV3x3_KERNEL_SIZE  3

#define MAX_PADDING_8   8

#ifdef __PC__
 	#define DDR_DATA
	#define DDR_BSS
	#define ALIGNED(x)
	#define DMA_DESCRIPTORS
	#define ENTRYPOINT
#else
 	#define DDR_DATA 	        __attribute__((section(".ddr.data")))
	#define DDR_BSS 	        __attribute__((section(".ddr.bss")))
 	#define ALIGNED(x) 	        __attribute__((aligned(x)))
	#define DMA_DESCRIPTORS     __attribute__((section(".cmx.cdmaDescriptors")))
	#define ENTRYPOINT extern "C"
#endif

#endif