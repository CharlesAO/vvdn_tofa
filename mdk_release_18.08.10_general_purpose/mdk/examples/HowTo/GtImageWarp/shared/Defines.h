#ifndef __DEFINES_SPARSE_H__
#define __DEFINES_SPARSE_H__
#include "imageWarpDefines.h"

// Image size
#define IMG_WIDTH  240
#define IMG_HEIGHT 320

#define OUT_IMG_WIDTH  240
#define OUT_IMG_HEIGHT 320

#define MESH_WIDTH (IMG_WIDTH / MESH_CELL_SIZE + 1)
#define MESH_HEIGHT (IMG_HEIGHT / MESH_CELL_SIZE + 1)

#ifdef __PC__
 	#define CMX_DMA
	#define DDR_DATA
	#define DDR_BSS
	#define CMX_DATA
	#define CMX_DATA_GLOBAL
	#define ALIGNED(x)
	#define DMA_DESCRIPTORS
	#define ENTRYPOINT
	#define SHAVE_HALT
#else
	#define CMX_DMA             __attribute__((section(".cmx.cdmaDescriptors")))
 	#define DDR_DATA 	        __attribute__((section(".ddr.data")))
	#define DDR_BSS 	        __attribute__((section(".ddr.bss")))
	#define CMX_DATA			__attribute__((section(".cmx.data")))
	#define CMX_DATA_GLOBAL     __attribute__((section(".cmx.data.global")))
 	#define ALIGNED(x) 	        __attribute__((aligned(x)))
	#define DMA_DESCRIPTORS     __attribute__((section(".cmx.cdmaDescriptors")))
	#define ENTRYPOINT extern "C"
#endif

#endif
