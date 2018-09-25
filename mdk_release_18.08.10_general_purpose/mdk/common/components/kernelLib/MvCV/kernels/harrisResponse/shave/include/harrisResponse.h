#ifndef __HARRISRESPONSE_H__
#define __HARRISRESPONSE_H__

#include <mv_types.h>
#include <mvcv_macro.h>

///This kernel computes Harris response over a patch of the image with a radius of 3. The patch
/// size is actually 8x8 to account for borders
/// @param[in] data			- Input patch including borders
/// @param[in] x			- X coordinate inside the patch. Only a value of 3 supported
/// @param[in] y			- Y coordinate inside the patch. Only a value of 3 supported
/// @param[in] step_width	- Step of the patch. Only a value 8 supported (2xradius + 2xborder)
/// @param[in] k			- Constant that changes the response to the edges. Typically 0.02 is used
/// return					- Corner response value

MVCV_FUNC(fp32, mvcvHarrisResponse, u8 *data, u32 x, u32 y, u32 step_width, fp32 k)

#endif //__HARRISRESPONSE_H__