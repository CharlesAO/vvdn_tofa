#ifndef __LOOKUPTABLE12TO16_H__
#define __LOOKUPTABLE12TO16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a look-up table transform of a line. Destination line is filled with values from the look-up table. Indices of the entries are taken from the source line.
/// @param[in]  src      - Input line
/// @param[out] dest     - Output line
/// @param[in]  lut      - Look-up table of 8193 elements; should have the same depth as the input line. 
/// In the case of multi - Channel source and destination lines, the table should either have a single-channel 
///(in this case the same table is used for all channels) or the same number of channels as the source/destination line.
/// @param[in]  width    - Width of input line
/// @param[in]  height   - The number of lines (defaulted to one line)

MVCV_FUNC(void, mvcvLUT12to16, u16** src, u16** dest, const u16* lut, u32 width, u32 height)
//!@}

#endif //__LOOKUPTABLE12TO16_H__