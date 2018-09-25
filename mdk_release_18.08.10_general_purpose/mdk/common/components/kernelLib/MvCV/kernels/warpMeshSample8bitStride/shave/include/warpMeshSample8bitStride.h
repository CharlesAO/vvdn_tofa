#ifndef __WARPMESHSAMPLE8BITSTRIDE_H__
#define __WARPMESHSAMPLE8BITSTRIDE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvWarpMeshSample8bit - in order to compute everything ok we need one extra pixel to the right and one extra line at the bottom for interpolation
/// @param[in] tile                 - Input tile of data, (u8 format) it has variable width/height size
/// @param[out] out_tile            - Output tile (u8 format) it has a 16*16 fixed dimension
/// @param[in] mx                   - array of x coordinates relative to input tile
/// @param[in] my                   - array of y coordinates relative to input tile
/// @param[in] out_stride           - is the stride for output tile
/// @param[in] tile_height          - tile_height of the input tile + the eventualy padding when making the interpolation
/// @return    Nothing

MVCV_FUNC(void, mvcvWarpMeshSample8bitStride, u8 **tile, u8 *out_tile, half *mx, half *my, u32 out_stride, u32 tile_height)

//!@}

#endif //__WARPMESHSAMPLE8BIT_H__
