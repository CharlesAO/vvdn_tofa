#ifndef __FASTATAN2POSITIVE_H__
#define __FASTATAN2POSITIVE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel calculates the principal value of the arctangent of y/x,
/// using the signs of the two arguments to determine the quadrant of the result.
/// @param[in] -  y - the y coordinate of the pixel.
/// @param[in] -  x - the x coordinate of the pixel.
/// @return    -  The value of the angle in radians between [0, 2 * PI]
MVCV_FUNC(half, mvcvFastAtan2Positive, half y, half x)
//!@}

#endif //__FASTATAN2POSITIVE_H__
