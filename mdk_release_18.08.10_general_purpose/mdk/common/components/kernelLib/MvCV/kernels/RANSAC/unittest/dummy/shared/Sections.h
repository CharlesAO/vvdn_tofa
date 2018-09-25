///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
#ifndef __PROJECT_SECTIONS_H
#define __PROJECT_SECTIONS_H

#ifdef __PC__

#define UNIT_TEST
#define CMX_TEXT
#define CMX_CONST_DATA
#define DDR_DATA
#define DDR_HEAP
#define DDR_BSS
#define DDR_DIRECT_BSS
#define CMX_BSS
#define CMX_DATA_S0
#define CMX_DATA_S1
#define CMX_DATA_CASCADE_HID
#define CMX_DATA_GLOBAL
#define CMX_DATA_INTEGRAL_IMG
#define CMX_DMA_DESCRIPTORS

#else

#define UNIT_TEST		__attribute__((section(".text.unittest")))
#define CMX_TEXT		__attribute__((section(".cmx.text")))
#define CMX_DATA		__attribute__((section(".cmx.data")))
#define CMX_CONST_DATA		__attribute__((section(".cmx.const.data")))
#define DDR_DATA		__attribute__((section(".ddr.data")))
#define DDR_HEAP		__attribute__((section(".ddr.heap")))
#define DDR_BSS			__attribute__((section(".ddr.bss")))
#define DDR_DIRECT_BSS	__attribute__((section(".ddr_direct.bss")))
#define CMX_BSS			__attribute__((section(".cmx.bss")))
#define CMX_DATA_S0		__attribute__((section(".shv0.S.data")))
#define CMX_DATA_S1		__attribute__((section(".shv1.S.data")))
#define CMX_DATA_CASCADE_HID	__attribute__((section(".cmx.data.cascade_hid")))
#define CMX_DATA_GLOBAL        __attribute__((section(".cmx.data.global")))
#define CMX_DATA_INTEGRAL_IMG  __attribute__((section(".cmx.data.integralImage")))
#define LCD_OVERLAY		__attribute__((section(".ddr_direct.bss.overlay")))
#define OSD			__attribute__((section(".osd.gl2.data")))
#define CMX_DMA_DESCRIPTORS     __attribute__((section(".cmx.cdmaDescriptors")))
#endif

#endif
