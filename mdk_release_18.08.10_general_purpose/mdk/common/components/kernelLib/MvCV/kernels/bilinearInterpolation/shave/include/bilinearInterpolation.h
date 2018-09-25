#ifndef __BILINEAR_INTERPOLATION_H__
#define __BILINEAR_INTERPOLATION_H__
#include <mv_types.h>
#include <mvcv_types.h>
#include <mvcv_macro.h>


//!@{ 
///This kernel does bilinear interpolation when there is a pattern 
///between bilinear factors
///if the horizontal scalefactor(srcWidth/destWidth)
///has one decimal point precision (for example: 1.4, 555.5, 0.1, 0.4) then
///the 4 factors that are used in calculation will repeat at least after every
///10 iteration, so we can create a lookup table which has the size of 10x10x4*2bytes
///the acces pattern will also respect the equidistant locality
///principle so every 10th pixel's coordinate will differ by a constant
///which is 10 * scaleHor
///bilinear interpolation is applied on 2 lines
///@param[in]   line1 - input elements, 8 bits unsigned chars
///@param[in]   line2 - input elements, 8 bits unsigned chars
///@param[out]  output     - Output line, 8 bits unsigned chars
///@param[in]   precalcPos - the first 10 pixels' adress (these with accesPatternStep)
///                     defines the acces pattern. 10x2 bytes
///@param[in]   lut - lookup table that holds the precalculated factors. 10x10x4x2bytes
///@param[in]   stopW - the pixel's number until we apply bilinear interpolation
///             should be higher than 20
///@param[in]   accesPatternStep - the acces pattern is defined by the horizontal scale
///             which is after every 10th iteration (at least) differs by 10 * scaleHor
MVCV_FUNC(void, bilinearInterpolation, 
                    u8 * line1,
                    u8 * line2,
                    u8 * output,
                    u16 * precalcPos,
                    u16 * lut,
                    int stopW,
                    int accesPatternStep)
//!@}


#endif // __BILINEAR_INTERPOLATION_H__
