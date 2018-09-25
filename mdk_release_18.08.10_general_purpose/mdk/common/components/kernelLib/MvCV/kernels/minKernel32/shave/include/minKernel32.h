#ifndef __MININUM_VALUE_32_H__
#define __MININUM_VALUE_32_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// Compute the minimum value from an input line of dimension 32
/// @param[in] in         - input line
/// @param[out] minVal    - minimum value from the input line
/// @return               - nothing

MVCV_FUNC(void, mvcvMinKernel32, u8* in, u8* minVal)
//!@}

#endif //__MININUM_VALUE_64_H__