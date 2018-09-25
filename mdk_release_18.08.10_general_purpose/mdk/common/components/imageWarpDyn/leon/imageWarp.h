///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup imageWarpDyn 	Image Warp
/// @ingroup  imageWarpDyn
/// @{
/// @brief    Component used to warp a frameBuffer dynamically
///

#ifndef COMPONENT_IMAGEWARP_H
#define COMPONENT_IMAGEWARP_H

// 1: Includes
// ----------------------------------------------------------------------------
#include <swcShaveLoader.h>
#include <swcFrameTypes.h>
#include "imageWarpDefines.h"


// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 2.5: Global definitions
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// @brief Sets up and launches one dynamic image warp on a specifically requested SHAVE
/// @param[in] svu - shave index to use
/// @param[in] mesh - pointer to the mesh to use
/// @param[in] tileNodes - pointer to the tileNodes to use
/// @param[in] inputFb - input framebuffer to process
/// @param[in] outputFb - output buffer that will be written with the result
/// @param[in] padding - outside mesh padding
/// @return operation status - 0 if success, error code otherwise (to be enhanced in the future)
///
u32 IMGWARP_start(swcShaveUnit_t svu, meshStruct* mesh, tileList* tileNodes, frameBuffer *inputFb, frameBuffer *outputFb, unsigned short padding);


/// @brief Cleans up and prepare already used shave for running other dynamic apps
/// @return void
///
void IMGWARP_cleanup(void);


/// }@
#endif
