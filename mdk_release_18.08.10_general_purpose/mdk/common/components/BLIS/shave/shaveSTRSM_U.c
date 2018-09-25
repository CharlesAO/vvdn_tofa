///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "stdlib.h"
#include <mv_types.h>
#include <svuCommonShave.h>

// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void lamaStrsm_u4x4_asm(float* a, float* b, float* c, int rs_c, int cs_c);

void startSTRSM_U(float* a, float* b, float* c, int rs_c, int cs_c)
{
   lamaStrsm_u4x4_asm(a, b, c, rs_c, cs_c);

   exit(0);
   return;
}

