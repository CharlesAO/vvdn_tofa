#ifndef __CVTCOLORKERNELRGBTOUV420_H__
#define __CVTCOLORKERNELRGBTOUV420_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel performs color space conversion RGB to UV420 for one line in an image
/// @param[in]  inR     - First line from input R plane 
/// @param[in]  inG     - First line from input G plane
/// @param[in]  in      - First line from input B plane 	
/// @param[Out] uOut    - U plane 
/// @param[Out] vOut    - V plane 
/// @param[in]  width   - Line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvcvCvtColorRGBtoUV420, u8** inR, u8** inG, u8** inB, u8** uOut, u8** vOut, u32 width)
//!@}

#endif //__CVTCOLORKERNELRGBTOUV420_H__
