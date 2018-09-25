///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Default GPIO configuration for the MV0191 Board
///
/// Using the structure defined by this board it is possible to initialize
/// some of the GPIOS on the MV0191 PCB to good safe initial defaults
///
#ifndef BRD_MV0191_GPIO_DEFAULTS_H_
#define BRD_MV0191_GPIO_DEFAULTS_H_


// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvGpioDefines.h>

// 2: Defines
// ----------------------------------------------------------------------------

// 3: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// 4: Local const declarations     NB: ONLY const declarations go here
// ----------------------------------------------------------------------------


const drvGpioInitArrayType brdMV0191GpioCfgDefault =

{

		{22, 22 , ACTION_UPDATE_ALL   //
				,
				PIN_LEVEL_HIGH              //
				,
				D_GPIO_MODE_7            |  //
				D_GPIO_DIR_OUT           |  // Drive out
				D_GPIO_DATA_INV_OFF      |
				D_GPIO_WAKEUP_OFF
				,
				D_GPIO_PAD_DEFAULTS         // Uses the default PAD configuration
                 , NULL
		},
		// -----------------------------------------------------------------------
		// Finally we terminate the Array
		// -----------------------------------------------------------------------
		{0,0    , ACTION_TERMINATE_ARRAY      // Do nothing but simply termintate the Array
				,
				PIN_LEVEL_LOW               // Won't actually be updated
				,
				D_GPIO_MODE_0               // Won't actually be updated
				,
				D_GPIO_PAD_DEFAULTS         // Won't actually be updated
                 , NULL
		}

};

#endif //BRD_MV0191_GPIO_DEFAULTS_H_
