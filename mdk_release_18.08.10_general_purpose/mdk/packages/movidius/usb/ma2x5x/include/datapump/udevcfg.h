/* udevcfg.h	Fri Dec 17 1999 07:39:56  tmm */

/*

Module:  udevcfg.h

Function:
	A simple header file for auto-configuring single-chip applications.

Version:
	V1.50c	Fri Dec 17 1999 07:39:56	tmm	Edit level 1

Copyright notice:
	This file copyright (C) 1999 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	December 1999

Revision history:
   1.50c  Fri Dec 17 1999 07:39:56  tmm
	Module created.

*/

#ifndef _UDEVCFG_H_		/* prevent multiple includes */
#define _UDEVCFG_H_

#include "udevindx.h"

/****************************************************************************\
|
|  Based on the selected index, prepare the "chip-neutral" definitions:
|
|	UDEVICE_CHIP	defined as the chip-specific structure
|	CHIP_PumpInit	defined as the chip-specific pump-init routine
|
\****************************************************************************/

#ifndef UDEVICE_INDEX
# error	"UDEVICE_INDEX is not defined -- assuming zero"
# define	UDEVICE_INDEX	0
#endif

#if	UDEVICE_IS_USS820(UDEVICE_INDEX)
# include "u820kern.h"
# define	UDEVICE_CHIP	UDEV820
# define	CHIP_PumpInit	uss820_PumpInit

#elif	UDEVICE_IS_D12(UDEVICE_INDEX)
# include "d12kern.h"
# define	UDEVICE_CHIP	UDEVD12
# define	CHIP_PumpInit	d12_PumpInit

#elif	UDEVICE_IS_BCM3310(UDEVICE_INDEX)
# include "ubcmkern.h"
# define	UDEVICE_CHIP	UDEV3310
# define	CHIP_PumpInit	bcm3310_PumpInit

#else
# error "Unknown/unsupported device index selected"
#endif

/**** end of udevcfg.h ****/
#endif /* _UDEVCFG_H_ */
