///
/// @file genLumaU8Fp16.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Generate Luma from RGB short tint (U8) input data in Fp16 output data
///

#ifndef __genLumaU8Fp16_MVCV_H__
#define __genLumaU8Fp16_MVCV_H__

#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// Calculate luma from rgb input lines obtained from debayer
/// @param[in]  inR      - array of pointers to input line of the image, 1 planes, R
/// @param[in]  inG      - array of pointers to input line of the image, 1 planes, G
/// @param[in]  inB      - array of pointers to input line of the image, 1 planes, B
/// @param[out] outY     - Luma output line, fp16 format
/// @param[in]  width    - width of the input lines
/// @return     Nothing

MVCV_FUNC(void, mvispGenLumaU8Fp16, u8* inR, u8 *inG, u8 *inB, half *outY, half* coefs, u32 width)
//!@}
	
#endif //__genLumaU8Fp16_MVCV_H__
