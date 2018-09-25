#ifndef __PIXELPOS_H__
#define __PIXELPOS_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel returns the position of a given pixel value
/// @param[in] srcAddr         - Input lines
/// @param[in] maskAddr        - Mask filled with 1s and 0s which determines the image area to find position
/// @param[in] width           - Line's width(length)
/// @param[in] pixelValue      - Stores the pixel value to be searched
/// @param[out] pixelPosition  - Stores the position occupied by the searched value within line
/// @param[out] status         - Stores 0x11 if pixel value found, else 0x00
/// @return               	   - Nothing

MVCV_FUNC(void, mvcvPixelPos, u8** srcAddr, u8* maskAddr, u32 width, u8 pixelValue, u32* pixelPosition, u8* status)

//!@}

#endif //__PIXELPOS_H__