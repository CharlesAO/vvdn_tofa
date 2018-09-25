#ifndef __CVTCOLORRGBTOUV_H__
#define __CVTCOLORRGBTOUV_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel makes a conversion from RGB to UV
/// @param[in] inR    - Input R channel
/// @param[in] inG    - Input G channel
/// @param[in] inB    - Input B channel
/// @param[out] uvOut - Output UV channels

MVCV_FUNC(void, mvcvCvtColorRGBtoUV, u8** inR, u8** inG, u8** inB, u8** uvOut, u32 width, u32 line)
//!@}

#endif //__CVTCOLORRGBTOUV_H__