#ifndef __JPEGGetBlockY420_H__
#define __JPEGGetBlockY420_H__
#include <mv_types.h>
#include <mvcv_macro.h>
//!@{
/// mvcvjpegGetBlockY420
/// @param[in]
/// mode		 	- has one of the following values: 0, 1, 2, 3 - selects one 8x8 block from the 16x16 macroblock
/// imginfo_Y		- input buffer with  imginfo_width*(imginfo_height) dimension, where (imginfo_height)modulo 16 =0
/// imginfo_width - width of input buffer
/// imgspec_x     - macroblock x coordinate
/// imgspec_y     - macroblock y coordinate
/// @param[out]
/// imgspecY[8][8]- output 8x8 block
/// @return       0

MVCV_FUNC(void, mvcvjpegGetBlockY420, u32 mode, u8 *imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8])

//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
    void mvcvjpegGetBlockY420_opt(u32 mode, u8 *imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8]);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__JPEGGetBlockY420_H__
