///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Default GPIO configuration for the MV0212 Board
///
/// Using the structure defined by this board it is possible to initialize
/// some of the GPIOS on the MV0212 PCB to good safe initial defaults
///

#ifndef BRD_MV0212_GPIO_DEFAULTS_H_
#define BRD_MV0212_GPIO_DEFAULTS_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvGpioDefines.h>

// 2: Defines
// ----------------------------------------------------------------------------

// 3: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 4: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------

extern const drvGpioInitArrayType brdMV0212GpioCfgDefault;

#endif // BRD_MV0212_GPIO_DEFAULTS_H_
