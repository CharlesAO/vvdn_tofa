#ifndef __DCT16BIT_H__ 
#define __DCT16BIT_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvDCT16bit - 
/// @param[in] pixel[8][8]            - Input tile of data, (i16 format) 8x8 block
/// @param[out] data[8][8]            - Output tile (i16 format) 8x8 block
/// @return    Nothing

MVCV_FUNC(void, mvcvDCT16bit, short pixels[8][8], short data[8][8])

//!@}

#endif //__DCT16BIT_H__