#ifndef __DOWNSAMPLEBILINEARLINE_H__
#define __DOWNSAMPLEBILINEARLINE_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvDownsampleBilinearLine  - performs a downsample using one line at input and switches depending on select(even line, odd line)
/// @param[in] inLine           - input line
/// @param[out] tempLine        - output buffer for even lines, input buffer for odd lines
/// @param[out] outLine         - output result for odd lines
/// @param[in] width            - width of the input lines
/// @param[in] select           - select on wich line the computation will occure (o-even, 1-odd)
/// @return    Nothing
MVCV_FUNC(void, mvcvDownsampleBilinearLine, u16 *inLine, u32 *tempLine, u16 *outLine, u32 inWidth, u32 select)

//!@}

#endif //__DOWNSAMPLEBILINEARLINE_H__
