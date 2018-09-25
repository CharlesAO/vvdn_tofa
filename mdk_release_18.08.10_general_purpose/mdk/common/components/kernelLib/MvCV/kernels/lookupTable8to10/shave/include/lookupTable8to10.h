#ifndef __LOOKUPTABLE8TO10_H__
#define __LOOKUPTABLE8TO10_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs a look-up table transform of a line. 
/// The function fills the destination line with values from the look-up table.
/// Indices of the entries are taken from the source line
/// @param[in]  src       - Input line
/// @param[out] dest      - Output line
/// @param[in]  lut       - Look-up table of 256 elements
/// @param[in]  width     - Width of input line

MVCV_FUNC(void, mvcvLUT8to10, u8** src, u16** dest, const u16* lut, u32 width)
//!@}

#endif //__LOOKUPTABLE8TO10_H__