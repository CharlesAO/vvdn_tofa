/* uhil_getput_macros.h	Sat Sep 08 2012 20:40:42 chwon */

/*

Module:  uhil_getput_macros.h

Function:
	Definition of UHIL_GET_xxx / UHIL_PUT_xxx macro

Version:
	V3.11b	Sat Sep 08 2012 20:40:42 chwon	Edit level 1

Copyright notice:
	This file copyright (C) 2012 by

		MCCI Corporation
		3520 Krums Corners Road
		Ithaca, NY  14850

	An unpublished work.  All rights reserved.

	This file is proprietary information, and may not be disclosed or
	copied without the prior permission of MCCI Corporation

Author:
	ChaeHee Won, MCCI Corporation	September 2012

Revision history:
   3.11b  Sat Sep 08 2012 20:40:42 chwon
	15846: Module created.

*/

#ifndef _UHIL_GETPUT_MACROS_H_		/* prevent multiple includes */
#define _UHIL_GETPUT_MACROS_H_

/****************************************************************************\
|
|	Function prototypes -- for historical reasons, the real names of
|	these functions are UHIL_..., but they should be UsbPumpLib_...
|
\****************************************************************************/

#ifndef	__TMS_UHIL_LE_GETUINT16
# define __TMS_UHIL_LE_GETUINT16(buf)					\
	(__TMS_UINT16) (((__TMS_UINT16)(((__TMS_UINT8 *)(buf))[0])) +	\
			((__TMS_UINT16)(((__TMS_UINT8 *)(buf))[1]) << 8))
#endif

#ifndef	__TMS_UHIL_LE_GETUINT32
# define __TMS_UHIL_LE_GETUINT32(buf)					\
	(((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[0])) +			\
	 ((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[1]) << 8) +		\
	 ((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[2]) << 16) +		\
	 ((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[3]) << 24))
#endif

#ifndef	__TMS_UHIL_LE_GETUINT64
# define __TMS_UHIL_LE_GETUINT64(buf)					\
	(((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[0])) +			\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[1]) << 8) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[2]) << 16) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[3]) << 24) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[4]) << 32) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[5]) << 40) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[6]) << 48) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[7]) << 56))
#endif

#ifndef	__TMS_UHIL_LE_PUTUINT16
# define __TMS_UHIL_LE_PUTUINT16(buf, val)				\
    do	{								\
	__TMS_LIBPORT_PACKED1(__TMS_UINT8 * __TMS_CONST v_Buf = (buf));	\
	__TMS_CONST __TMS_UINT16 v_Val = (val);				\
									\
	v_Buf[0] = (__TMS_UINT8) v_Val;					\
	v_Buf[1] = (__TMS_UINT8) (v_Val >> 8);				\
	} while (0)
#endif

#ifndef	__TMS_UHIL_LE_PUTUINT32
# define __TMS_UHIL_LE_PUTUINT32(buf, val)				\
    do	{								\
	__TMS_LIBPORT_PACKED1(__TMS_UINT8 * __TMS_CONST v_Buf = (buf));	\
	__TMS_CONST __TMS_UINT32 v_Val = (val);				\
									\
	v_Buf[0] = (__TMS_UINT8) v_Val;					\
	v_Buf[1] = (__TMS_UINT8) (v_Val >> 8);				\
	v_Buf[2] = (__TMS_UINT8) (v_Val >> 16);				\
	v_Buf[3] = (__TMS_UINT8) (v_Val >> 24);				\
	} while (0)
#endif

#ifndef	__TMS_UHIL_LE_PUTUINT64
# define __TMS_UHIL_LE_PUTUINT64(buf, val)				\
    do	{								\
	__TMS_LIBPORT_PACKED1(__TMS_UINT8 * __TMS_CONST v_Buf = (buf));	\
	__TMS_CONST __TMS_UINT64 v_Val = (val);				\
									\
	v_Buf[0] = (__TMS_UINT8) v_Val;					\
	v_Buf[1] = (__TMS_UINT8) (v_Val >> 8);				\
	v_Buf[2] = (__TMS_UINT8) (v_Val >> 16);				\
	v_Buf[3] = (__TMS_UINT8) (v_Val >> 24);				\
	v_Buf[4] = (__TMS_UINT8) (v_Val >> 32);				\
	v_Buf[5] = (__TMS_UINT8) (v_Val >> 40);				\
	v_Buf[6] = (__TMS_UINT8) (v_Val >> 48);				\
	v_Buf[7] = (__TMS_UINT8) (v_Val >> 56);				\
	} while (0)
#endif

#ifndef	__TMS_UHIL_BE_GETUINT16
# define __TMS_UHIL_BE_GETUINT16(buf)					\
	(__TMS_UINT16) (((__TMS_UINT16)(((__TMS_UINT8 *)(buf))[1])) +	\
			((__TMS_UINT16)(((__TMS_UINT8 *)(buf))[0]) << 8))
#endif

#ifndef __TMS_UHIL_BE_GETUINT32
# define __TMS_UHIL_BE_GETUINT32(buf)					\
	(((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[3])) +			\
	 ((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[2]) << 8) +		\
	 ((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[1]) << 16) +		\
	 ((__TMS_UINT32)(((__TMS_UINT8 *)(buf))[0]) << 24))
#endif

#ifndef	__TMS_UHIL_BE_GETUINT64
# define __TMS_UHIL_BE_GETUINT64(buf)					\
	(((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[7])) +			\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[6]) << 8) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[5]) << 16) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[4]) << 24) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[3]) << 32) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[2]) << 40) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[1]) << 48) +		\
	 ((__TMS_UINT64)(((__TMS_UINT8 *)(buf))[0]) << 56))
#endif

#ifndef	__TMS_UHIL_BE_PUTUINT16
# define __TMS_UHIL_BE_PUTUINT16(buf, val)				\
    do	{								\
	__TMS_LIBPORT_PACKED1(__TMS_UINT8 * __TMS_CONST v_Buf = (buf));	\
	__TMS_CONST __TMS_UINT32 v_Val = (val);				\
									\
	v_Buf[1] = (__TMS_UINT8) v_Val;					\
	v_Buf[0] = (__TMS_UINT8) (v_Val >> 8);				\
	} while (0)
#endif

#ifndef	__TMS_UHIL_BE_PUTUINT32
# define __TMS_UHIL_BE_PUTUINT32(buf, val)				\
    do	{								\
	__TMS_LIBPORT_PACKED1(__TMS_UINT8 * __TMS_CONST v_Buf = (buf));	\
	__TMS_CONST __TMS_UINT32 v_Val = (val);				\
									\
	v_Buf[3] = (__TMS_UINT8) v_Val;					\
	v_Buf[2] = (__TMS_UINT8) (v_Val >> 8);				\
	v_Buf[1] = (__TMS_UINT8) (v_Val >> 16);				\
	v_Buf[0] = (__TMS_UINT8) (v_Val >> 24);				\
	} while (0)
#endif

#ifndef	__TMS_UHIL_BE_PUTUINT64
# define __TMS_UHIL_BE_PUTUINT64(buf, val)				\
    do	{								\
	__TMS_LIBPORT_PACKED1(__TMS_UINT8 * __TMS_CONST v_Buf = (buf));	\
	__TMS_CONST __TMS_UINT64 v_Val = (val);				\
									\
	v_Buf[7] = (__TMS_UINT8) v_Val;					\
	v_Buf[6] = (__TMS_UINT8) (v_Val >> 8);				\
	v_Buf[5] = (__TMS_UINT8) (v_Val >> 16);				\
	v_Buf[4] = (__TMS_UINT8) (v_Val >> 24);				\
	v_Buf[3] = (__TMS_UINT8) (v_Val >> 32);				\
	v_Buf[2] = (__TMS_UINT8) (v_Val >> 40);				\
	v_Buf[1] = (__TMS_UINT8) (v_Val >> 48);				\
	v_Buf[0] = (__TMS_UINT8) (v_Val >> 56);				\
	} while (0)
#endif

/**** end of uhil_getput_macros.h ****/
#endif /* _UHIL_GETPUT_MACROS_H_ */
