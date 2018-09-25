/// @file DrvAmcMa2x5x.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvAmcMa2x5x
/// @{
/// @brief     Functions definitions

#ifndef _DRV_AMC_MA2X5X_H_
#define _DRV_AMC_MA2X5X_H_

#include <stdint.h>
#include "registersMyriadMa2x5x.h"
#include "DrvRegUtilsDefines.h"


void DrvSetAmcPriorities(u32 amcRdData, u32 amcWrData, u32 amcSupeRdData, u32 amcSuperWrData);

#endif /* _DRV_AMC_MA2X5X_H_ */


//}@
