///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup svuCommonShaveDefines Common Shave Defines
/// @ingroup  svuCommonShave
/// @{
/// @brief Definitions and types needed by common Shave fuctionalities
///
/// This file contains all the definitions of constants, typedefs,
/// structures, enums and exported variables for Common Shave functionalities
///

#ifndef __SVU_COMMON_SHAVE_DEFINES__
#define __SVU_COMMON_SHAVE_DEFINES__

//Some DMA defines stay in this file


//randevu
#define RANDEVU_MUTEX               0
#define DDR_ACCESS_MUTEX            4
// we asume that all the time, when we set a shave, will be the chace set at windows 0x1F
#define CMX_CACHE_PTR 0x1F000000
#define MASK_ACTIVATE_L2_CACHE 0xF0FFFFFF

#define CMXCODE(x) __attribute__((section(".textCrit"))) x

#ifdef NO_PRINT
#endif


/// @}
#endif
