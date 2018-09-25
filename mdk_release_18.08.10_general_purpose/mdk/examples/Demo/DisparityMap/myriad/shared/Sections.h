///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
#ifndef __SPARSE_SECTIONS_H
#define __SPARSE_SECTIONS_H
#ifndef __PC__
#include "sippPlatform.h"
#endif

#ifdef __PC__

#define CMX_DMA
#define CMX_TEXT
#define CMX_DATA
#define CMX_DIRECT_DATA
#define CMX_DATA_GLOBAL
#define DDR_DATA
#define DDR_BSS

#else

#define CMX_DMA                 __attribute__((section(".cmx.cdmaDescriptors")))
#define CMX_DIRECT_DATA			__attribute__((section(".cmx_direct.data")))
#define CMX_DATA_GLOBAL         __attribute__((section(".cmx.data.global")))
#define DDR_BSS					__attribute__((section(".ddr.bss")))

#endif

#endif
