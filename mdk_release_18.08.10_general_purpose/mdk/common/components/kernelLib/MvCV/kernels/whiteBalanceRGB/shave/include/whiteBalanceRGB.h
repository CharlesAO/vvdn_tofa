#ifndef __WHITEBALANCERGB_H__
#define __WHITEBALANCERGB_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel applies white balance gains for RGB input 
/// @param[in]  inR   	- Input line, R plane 
/// @param[in]  inG   	- Input line, G plane
/// @param[in]  inB   	- Input line, B plane 	
/// @param[out] oR    	- Output line, R plane 	
/// @param[out] oG    	- Output line, G plane 	 
/// @param[out] oB    	- Output line, B plane 	 
/// @param[in]  aebCoef - Awb gains coefs
/// @param[in]  clamp   - Clamp value
/// @param[in]  width   - Line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvcvWhiteBalanceRGB, u16** inR, u16** inG, u16** inB, u16 awbCoef[3], u16** oR, u16** oG, u16** oB, u16 clamp[1], u32 width)

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
            void mvcvWhiteBalanceRGB_opt(unsigned short ** inR, unsigned short ** inG, unsigned short ** inB, unsigned short awbCoef[3], unsigned short ** outR,
                                           unsigned short ** outG, unsigned short ** outB, unsigned short clamp[1], unsigned int width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__WHITEBALANCERGB_H__

