/* hilbusio_fns.h	Fri Jun 07 2013 09:20:43 ssyang */

/*

Module:  hilbusio_fns.h

Function:
	Define the function prototypes for the hilbusio.h functions.

Version:
	V3.11c	Fri Jun 07 2013 09:20:43 ssyang	Edit level 3

Copyright notice:
	This file copyright (C) 2007, 2013 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.
	
	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation.
 
Author:
	Terry Moore, MCCI Corporation	November 2007

Revision history:
   1.97j  Sun Nov 18 2007 05:58:20  tmm
	Module created.

   1.97j  Wed Nov 21 2007 13:04:22  tmm
	Design rules say we should include the things that we absolutely
	need; in our case it's basehiltypes.h.

   3.11c  Fri Jun 07 2013 09:20:43  ssyang
	17340: Add cpu memory barrier API function prototype

*/

#ifndef _HILBUSIO_FNS_H_		/* prevent multiple includes */
#define _HILBUSIO_FNS_H_

#ifndef _BASEHILENV_H_
# include "basehilenv.h"
#endif

/*

MODULE DOCUMENTATION:  UHIL I/O accessor function prototypes

	The header file hilbusio_fns.h defines the prototypes for
	functional versions of each of the UHIL_BUSHANDLE I/O access
	methods.  

	In order to do this sanely in the face of #defines for each
	of the names, we have to assume that the UHIL_inb(), etc. accessors,
	if defined as macros, are defined as FUNCTION-like, not NAME-like
	macros.  This means that we can use (UHIL_inb), etc, as the name of
	the real underlying function.

	Implementation modules in common, common/uhil_inb.c (etc), also
	use this trick, so that (for example) UHIL_inb() is implemented
	as:

	UINT8 (UHIL_inb)(
		UHIL_BUSHANDLE bus,
		ARG_IOPORT port
		)
		{
		return UHIL_inb(bus, port);
		}

	We justify the provision of these functions as follows.  First,
	providing functions that call the macros satisfies the principle
	of least surprise for programmers new to the DataPump -- they will
	be able to take the address of UHIL_inb, etc.  Second, this allows
	compile-time testing of each of the macros.  Finally, this header
	file serves as a convenient reference for tools that generate
	hilbusio.h starting points.

*/

__TMS_BEGIN_DECLS

/* usbkern-generate-hal-fns: begin */
__TMS_UINT8 
(UHIL_inb)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */
	);

__TMS_VOID 
(UHIL_outb)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_ARG_UINT8		/* value */
	);


__TMS_UINT16 
(UHIL_inw)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */
	);


__TMS_VOID 
(UHIL_outw)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_ARG_UINT16	/* value */
	);


__TMS_UINT32 
(UHIL_ind)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */
	);

__TMS_VOID
(UHIL_outd)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_ARG_UINT32	/* value */
	);

__TMS_VOID
(UHIL_insb)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_UINT8 *		/* pBuffer */,
	__TMS_BYTES		/* nBytes */
	);


__TMS_VOID
(UHIL_outsb)(
	__TMS_UHIL_BUSHANDLE		/* hBus */,
	__TMS_ARG_IOPORT		/* port */,
	__TMS_CONST __TMS_UINT8 *	/* pBuffer */,
	__TMS_BYTES			/* nBytes */
	);


__TMS_VOID
(UHIL_insbi)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_UINT8 *		/* pBuffer */,
	__TMS_BYTES		/* nBytes */
	);


__TMS_VOID
(UHIL_outsbi)(
	__TMS_UHIL_BUSHANDLE		/* hBus */,
	__TMS_ARG_IOPORT		/* port */,
	__TMS_CONST __TMS_UINT8 *	/* pBuffer */,
	__TMS_BYTES			/* nBytes */
	);

__TMS_VOID
(UHIL_insw)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_UINT16 *		/* pBuffer */,
	__TMS_BYTES		/* nWords */
	);

__TMS_VOID
(UHIL_outsw)(
	__TMS_UHIL_BUSHANDLE		/* hBus */,
	__TMS_ARG_IOPORT		/* port */,
	__TMS_CONST __TMS_UINT16 *	/* pBuffer */,
	__TMS_BYTES			/* nWords */
	);

__TMS_VOID
(UHIL_inswi)(
	__TMS_UHIL_BUSHANDLE	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_UINT16 *		/* pBuffer */,
	__TMS_BYTES		/* nWords */
	);

__TMS_VOID
(UHIL_outswi)(
	__TMS_UHIL_BUSHANDLE		/* hBus */,
	__TMS_ARG_IOPORT		/* port */,
	__TMS_CONST __TMS_UINT16 *	/* pBuffer */,
	__TMS_BYTES			/* nWords */
	);


__TMS_VOID
(UHIL_insd)(
	__TMS_UHIL_BUSHANDLE 	/* hBus */,
	__TMS_ARG_IOPORT	/* port */,
	__TMS_UINT32 *		/* pBuffer */,
	__TMS_BYTES		/* nDwords */
	);

__TMS_VOID
(UHIL_outsd)(
	__TMS_UHIL_BUSHANDLE		/* hBus */,
	__TMS_ARG_IOPORT		/* port */,
	__TMS_CONST __TMS_UINT32 *	/* pBuffer */,
	__TMS_BYTES			/* nDwords */
	);



__TMS_VOID
(UHIL_insdi)(
	__TMS_UHIL_BUSHANDLE		/* hBus */,
	__TMS_ARG_IOPORT		/* port */,
	__TMS_UINT32 *			/* pBuffer */,
	__TMS_BYTES			/* nDwords */
	);


__TMS_VOID
(UHIL_outsdi)(
	__TMS_UHIL_BUSHANDLE		/* hBus */,
	__TMS_ARG_IOPORT		/* port */,
	__TMS_CONST __TMS_UINT32 *	/* pBuffer */,
	__TMS_BYTES			/* nDwords */
	);

__TMS_VOID
(UHIL_CpuBarrierRead)(
	__TMS_UHIL_BUSHANDLE		/* hBus */
	);

__TMS_VOID
(UHIL_CpuBarrierWrite)(
	__TMS_UHIL_BUSHANDLE		/* hBus */
	);

__TMS_VOID
(UHIL_CpuBarrierGeneral)(
	__TMS_UHIL_BUSHANDLE		/* hBus */
	);
/* usbkern-generate-hal-fns: end */

__TMS_END_DECLS


/**** end of hilbusio_fns.h ****/
#endif	/* _HILBUSIO_FNS_H_ */
