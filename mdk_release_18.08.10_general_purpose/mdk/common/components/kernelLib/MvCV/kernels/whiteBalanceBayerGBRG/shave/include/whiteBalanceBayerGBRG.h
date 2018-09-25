#ifndef __WHITEBALANCEBAYERGBRG_H__
#define __WHITEBALANCEBAYERGBRG_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// This kernel applies white balance gains for BayerGBRG input.
/// @param[in]  inGBRG     - Input line 
/// @param[Out] outGBRG    - Output line 	
/// @param[in]  awbCoef    - Awb gains coefs
/// @param[in]  clamp      - Clamp value
/// @param[in]  width      - Line width in pixels
/// @param[in]  line       - Line number for line selection
/// @return     Nothing

MVCV_FUNC(void, mvcvWhiteBalanceBayerGBRG, u16** inGBRG, u16 awbCoef[3], u16** outGBRG, u16 clamp[1], u32 width, u8 line)

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	 void mvcvWhiteBalanceBayerGBRG_opt(unsigned short ** inGBRG, unsigned short awbCoef[3], unsigned short ** outGBRG,
                                                 unsigned short clamp[1], unsigned int width, unsigned char line); 
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__WHITEBALANCEBAYERGBRG_H__
 
