/* usbpump_usbseri_params.h	Wed Sep 06 2006 14:43:45 chwon */

/*

Module:  usbpump_usbseri_params.h

Function:
	Parameters for configuring USBSERI.

Version:
	V1.97e	Wed Sep 06 2006 14:43:45 chwon	Edit level 2

Copyright notice:
	This file copyright (C) 2006 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.

Author:
	Terry Moore, MCCI Corporation	May 2006

Revision history:
   1.00a  Tue May 23 2006 06:24:21  tmm
	Module created.

   1.97e  Wed Sep 06 2006 14:43:45  chwon
	Change default QEBUFFER size from 256 to 512 for high speed.

*/

#ifndef _USBPUMP_USBSERI_PARAMS_H_		/* prevent multiple includes */
#define _USBPUMP_USBSERI_PARAMS_H_

/****************************************************************************\
|
|	parameters
|
\****************************************************************************/

#ifndef USBSERI_WMC_CTRL_RX_NUM_UBUFQES		/*PARAM*/
# define USBSERI_WMC_CTRL_RX_NUM_UBUFQES	2
#endif
#ifndef USBSERI_WMC_CTRL_TX_NUM_UBUFQES		/*PARAM*/
# define USBSERI_WMC_CTRL_TX_NUM_UBUFQES	2
#endif
#ifndef USBSERI_WMC_DATA_RX_NUM_UBUFQES		/*PARAM*/
# define USBSERI_WMC_DATA_RX_NUM_UBUFQES	6
#endif
#ifndef USBSERI_WMC_DATA_TX_NUM_UBUFQES		/*PARAM*/
# define USBSERI_WMC_DATA_TX_NUM_UBUFQES	6
#endif

#define USBSERI_WMCMODEM_CTRL_RX_NUM_UBUFQES	USBSERI_WMC_CTRL_RX_NUM_UBUFQES
#define USBSERI_WMCMODEM_CTRL_TX_NUM_UBUFQES	USBSERI_WMC_CTRL_TX_NUM_UBUFQES
#define USBSERI_WMCMODEM_DATA_RX_NUM_UBUFQES	USBSERI_WMC_DATA_RX_NUM_UBUFQES
#define USBSERI_WMCMODEM_DATA_TX_NUM_UBUFQES	USBSERI_WMC_DATA_TX_NUM_UBUFQES

/*
|| Buffer sizes
*/
#ifndef USBSERI_WMC_QEBUFFER_SIZE		/*PARAM*/
# define USBSERI_WMC_QEBUFFER_SIZE		512
#endif

#define	USBSERI_WMCMODEM_QEBUFFER_SIZE		USBSERI_WMC_QEBUFFER_SIZE


#ifndef USBSERI_MAX_UARTS_PER_MODEM		/*PARAM*/
# define USBSERI_MAX_UARTS_PER_MODEM		2
#endif


/*
|| Control the (untested) software interrupt mechanism
*/
#ifndef USBSERI_USE_SW_INTERRUPT		/*PARAM*/
# define USBSERI_USE_SW_INTERRUPT		0
#endif

/**** end of usbpump_usbseri_params.h ****/
#endif /* _USBPUMP_USBSERI_PARAMS_H_ */
