///
/// @file DrvCmxDmaArch.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @date Mar 3, 2014
/// @defgroup DrvCmxDmaArch CmxDmaArch Driver
/// @{
/// @brief CmxDma Driver functionality.
///
#ifndef _CDMA_ARCH_H_
#define _CDMA_ARCH_H_

#ifndef USE_CMX_DMA_NEW_DRIVER

#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif


///Setup the LA configuration to be used by CMXDMA. Do not use this to initialize the driver. use #DrvCmxDmaInit instead
int DrvCmxDmaSetup(u32 interruptOffset, u32 nrOfAgents, u32 startingFrom);


#ifdef __cplusplus
}
#endif

#endif // USE_CMX_DMA_NEW_DRIVER

#endif // _CDMA_ARCH_H_

/// @}



