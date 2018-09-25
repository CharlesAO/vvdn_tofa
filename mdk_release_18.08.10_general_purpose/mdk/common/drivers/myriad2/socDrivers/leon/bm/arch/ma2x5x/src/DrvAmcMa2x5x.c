/// @file DrvAmcMa2x5x.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvAmcMa2x5x
/// @{
/// @brief     AMC  BM Driver configurations

#include "DrvAmcMa2x5x.h"

void DrvSetAmcPriorities(u32 amcRdData, u32 amcWrData, u32 amcSupeRdData, u32 amcSuperWrData)
{
  SET_REG_WORD(MSS_AMC_RD_PRI_ADR,   amcRdData);
  SET_REG_WORD(MSS_AMC_WR_PRI_ADR,   amcWrData);
  SET_REG_WORD(MSS_SUPER_RD_PRI_ADR, amcSupeRdData);
  SET_REG_WORD(MSS_SUPER_WR_PRI_ADR, amcSuperWrData);
}
