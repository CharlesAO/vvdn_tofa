#ifndef __ChromaFusionU8ToU8_H__
#define __ChromaFusionU8ToU8_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// mvispChromaFusionU8ToU8 - performs an interpolation between RGB channels
/// @param[in] inCRshort -   Red channel multiplied with (1-alpha)
/// @param[in] inCGshort - Green channel multiplied with (1-alpha)
/// @param[in] inCBshort -  Blue channel multiplied with (1-alpha)
/// @param[in] inCRlong  -  Red  channel multiplied with alpha
/// @param[in] inCGlong  - Green channel multiplied with alpha
/// @param[in] inCBlong  -  Blue channel multiplied with alpha
/// @param[in] inAlpha   - coeficient
/// @param[out] outR     - output of red channel
/// @param[out] outG     - output of green channel
/// @param[out] outB     - output of blue channel
/// @param[in] inWidth    - Width of input lines
	
MVCV_FUNC(void, mvispChromaFusionU8ToU8, u8* inCRshort, u8* inCGshort, u8* inCBshort, u8* inCRlong, u8* inCGlong, u8* inCBlong, half* inAlpha, u8* outR, u8* outG, u8* outB, u32 width)
//!@}

#endif //__ChromaFusionU8ToU8_H__