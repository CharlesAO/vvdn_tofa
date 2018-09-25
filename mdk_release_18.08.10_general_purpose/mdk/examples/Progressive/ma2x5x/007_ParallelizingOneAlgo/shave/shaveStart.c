///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <swcFrameTypes.h>
#include <svuCommonShave.h>
#include "effect.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void start(frameBuffer* inFrame, frameBuffer* outFrame)
{
    Brightness(inFrame->p1, outFrame->p1, inFrame->spec.width, inFrame->spec.height);
    CopyPlane(inFrame->p2, outFrame->p2, inFrame->spec.width / 2, inFrame->spec.height / 2);
    CopyPlane(inFrame->p3, outFrame->p3, inFrame->spec.width / 2, inFrame->spec.height / 2);

    SHAVE_HALT;

    return;
}
