///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _APP_CONFIG_H_
#define _APP_CONFIG_H_

// 1: Includes
// ----------------------------------------------------------------------------
// 2: Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define IMG_W 256
#define IMG_H  64

// 3: Static Local Data
// ----------------------------------------------------------------------------
// 4: Exported Functions (non-inline)
// ----------------------------------------------------------------------------
void BuildAndCfgPipe(Opipe *p, uint8_t *iBuf, uint8_t *oBuf);

#endif
