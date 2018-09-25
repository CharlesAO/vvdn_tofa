///
/// @file ShDrvCmxDma.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup ShDrvCmxDma
/// @{
/// @brief     CMX DMA driver implementation for SHAVE
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "ShaveData.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
__attribute__((weak)) ShDrvShaveDataStruct GlbShaveData;
ShDrvShaveDataStruct __attribute__((section(".data.__shaveData"))) *__globalShaveData = &GlbShaveData;


// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------


// 6: Functions Implementation
// ----------------------------------------------------------------------------



///@}
