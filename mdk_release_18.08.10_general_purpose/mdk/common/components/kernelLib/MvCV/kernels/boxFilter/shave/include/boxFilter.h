#ifndef __BOXFILTER_H__
#define __BOXFILTER_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{		
/// This kernel performs average on variable kernel size        
/// @param[in] in        - Input lines      
/// @param[out] out      - Output lines    
/// @param[in] k_size    - Kernel size for computing pixels. Ex. 3 for 3x3, 4 for 4x4  
/// @param[in] normalize - 1 form normalizing to kernel size, 0 otherwise 
/// @param[in] width     - Width of input line
MVCV_FUNC(void, mvcvBoxfilter, u8** in, u8** out, u32 k_size, u32 normalize, u32 width)
//!@}


#endif //__BOXFILTER_H__