#ifndef __MININUM_VALUE_H__
#define __MININUM_VALUE_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// Compute the minimum value from an input line
/// @param[in] in         - input line
/// @param[out] minVal    - minimum value from the input line
/// @param[in]  width     - width of input line
/// @return               - nothing

MVCV_FUNC(void, mvcvMinKernel, u8* in, u8* minVal, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
    void  mvcvMinKernel_opt(u8* in, u8* minVal, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED
#endif //__MININUM_VALUE_H__
