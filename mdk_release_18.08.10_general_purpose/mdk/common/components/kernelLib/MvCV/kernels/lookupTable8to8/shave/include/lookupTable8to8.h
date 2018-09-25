#ifndef __LOOKUPTABLE8TO8_H__
#define __LOOKUPTABLE8TO8_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a look-up table transform of a line. 
/// The function fills the destination line with values from the look-up table.
/// Indices of the entries are taken from the source line
/// @param[in]  src       - Input line
/// @param[out] dest      - Output line
/// @param[in]  lut       - Look-up table of 256 elements; should have the same depth as the input line. 
/// In the case of multi  - Channel source and destination lines, the table should either have a single-channel 
/// (in this case the same table is used for all channels) or the same number of channels as the source/destination line.
/// @param[in]  width     - Width of input line
/// @param[in]  height    - The number of lines (defaulted to one line)

MVCV_FUNC(void, mvcvLUT8to8, u8** src, u8** dest, const u8* lut, u32 width, u32 height)
//!@}

#endif //__LOOKUPTABLE8TO8_H__