#ifndef __TILE_SAMPLER_H__
#define __TILE_SAMPLER_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// tile_sampleru8 - in order to compute everything ok we need one extra pixel to the right and one extra line at the bottom for interpolation
/// @param[in] tile                 - Input tile of data, (u16 format) it has variable width/height size
/// @param[out] out_tile            - Output tile (u16 format) it has a 64*64 fixed dimension
/// @param[in] xcoord               - array of xcoords of the 4 points
/// @param[in] ycoord               - array of ycoords of the 4 points
/// @param[in] tile_width           - tile_width of the input tile + the eventualy padding when making the interpolation
/// @param[in] tile_height          - tile_height of the input tile + the eventualy padding when making the interpolation
/// @return    Nothing

MVCV_FUNC(void, mvcvtile_sampleru8, u8 *tile, u8 * out_tile, float *xcoord, float *ycoord, u32 tile_width, u32 tile_height)

//!@}

#endif //__TILE_SAMPLER_H__