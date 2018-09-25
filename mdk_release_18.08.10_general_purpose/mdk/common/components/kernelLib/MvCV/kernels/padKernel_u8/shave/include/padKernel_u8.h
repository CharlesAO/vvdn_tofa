#ifndef PAD_KERNEL_U8_H_
#define PAD_KERNEL_U8_H_

#include <mv_types.h>
#include <mvcv_macro.h>

enum
{
	Left         = 0,
	Right        = 1,
	LeftAndRight = 2,
};

enum
{
	AllZero    = 0,
	AllOne     = 1,
	Mirror     = 2,
	BlackPixel = 3,
	WhitePixel = 4,
	PixelValue = 5, 
};

//!@{
/// This kernel adds pad to a line with pixels at left and/or right side
/// @param[in]  iBuf     –  input buffer 
/// @param[in]  iBufLen  –  input buffer length
/// @param[out] oBuf     -  output buffer 
/// @param[in]  padSz    –  number of pixels to pad
/// @param[in]  padMode  -  Left / Right / LeftAndRight
/// @param[in]  padType  -  AllZero / AllOne / Mirror / BlackPixel / WhitePixel / PixelValue (passed as parmeter)
/// @param[in]  pixValue -  used only for PixelValue padType

MVCV_FUNC(void, mvcvPadKernel_u8, u8* iBuf, u32 iBufLen, u8* oBuf, u32 padSz, u32 padMode, u32 padType, u8 pixValue)

//!@}

#endif /* PAD_KERNEL_U8_H_ */
