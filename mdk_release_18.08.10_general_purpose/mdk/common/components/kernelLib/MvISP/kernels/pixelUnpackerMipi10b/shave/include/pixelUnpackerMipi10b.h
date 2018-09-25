///
/// @file      pixelUnpackerMipi10b.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Special kernel to interface to Myriad2 Mipi block
///            Converts 32bit value received as [12b | 10b | 10b]  to [6b | 10b | 6b | 10b]
///            

#ifndef __PIXELUNPACKERMIPI10b_H__
#define __PIXELUNPACKERMIPI10b_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{ 
/// @brief pixelUnpackerMipi10b
/// @detail

/// input format 32b: xxxxxxxxxxxxa0a1a2a3a4a5a6a7a8a9b0b1b2b3b4b5b6b7b8b9
/// output format 16b:	000000a0a1a2a3a4a5a6a7a8a9 000000b0b1b2b3b4b5b6b7b8b9

/// @param[in]	in		- Input line
/// @param[out] out		- Output line
/// @param[in] width	- Number of processed pixels
/// @param[in] lineNo	- Line number
/// @param[in] corfs	- Coefficients for black correction
/// @deprecated         - Use mvispPixelUnpackerMipi10b(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4], int noMipiRxWorkaround)  instead


#if (((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4))
__Deprecated__("Use mvispPixelUnpackerMipi10b(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4], int noMipiRxWorkaround)  instead")
#else
__Deprecated__()
#endif
MVCV_FUNC_OVERLOADED(void, mvispPixelUnpackerMipi10b, u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4])


/// @brief pixelUnpackerMipi10b
/// @detail

/// input format 32b: xxxxxxxxxxxxa0a1a2a3a4a5a6a7a8a9b0b1b2b3b4b5b6b7b8b9
/// output format 16b:	000000a0a1a2a3a4a5a6a7a8a9 000000b0b1b2b3b4b5b6b7b8b9

/// @param[in]	in		          - Input line
/// @param[out] out		          - Output line
/// @param[in] width	          - Number of processed pixels
/// @param[in] lineNo	          - Line number
/// @param[in] corfs	          - Coefficients for black correction
MVCV_FUNC_OVERLOADED(void, mvispPixelUnpackerMipi10b, u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4], int noMipiRxWorkaround)

//!@}

#endif // __PIXELUNPACKERMIPI10b_H__
