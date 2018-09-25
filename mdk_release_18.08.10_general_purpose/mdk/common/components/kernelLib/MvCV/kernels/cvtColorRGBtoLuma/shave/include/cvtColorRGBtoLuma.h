#ifndef __CVTCOLORRGBTOLUMA_H__
#define __CVTCOLORRGBTOLUMA_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel makes a conversion from RGB to Luma
///@param[in] inR 	- Input R channel
///@param[in] inG	- Input G channel
///@param[in] inB	- Input B channel
///@param[out] yOut	- Output Y channel

MVCV_FUNC(void, mvcvCvtColorRGBtoLuma, u8** inR, u8** inG, u8** inB, u8** yOut, u32 width)
//!@}

#endif //__CVTCOLORRGBTOLUMA_H__