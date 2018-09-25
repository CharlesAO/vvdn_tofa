#ifndef __WARPMESHSAMPLEFP16BIT_H__
#define __WARPMESHSAMPLEFP16BIT_H__
#include <mv_types.h>
#include <mvcv_macro.h>
    
//!@{
/// mvcvWarpMeshSampleFp16bit - in order to compute everything ok we need one extra pixel to the right and one extra line at the bottom for interpolation
/// @param[in] tile                 - Input tile of data, (fp16 format) it has variable width/height size
/// @param[out] out_tile            - Output tile (fp16 format) it has a 16*16 fixed dimension
/// @param[in] mx                   - array of x coordinates relative to input tile
/// @param[in] my                   - array of y coordinates relative to input tile
/// @param[in] tile_width           - tile_width of the input tile + the eventualy padding when making the interpolation
/// @param[in] tile_height          - tile_height of the input tile + the eventualy padding when making the interpolation
/// @return    Nothing

MVCV_FUNC(void, mvcvWarpMeshSampleFp16bit, half **tile, half *out_tile, half *mx, half *my, u32 tile_width, u32 tile_height)

//!@}

#endif //__WARPMESHSAMPLEFP16BIT_H__