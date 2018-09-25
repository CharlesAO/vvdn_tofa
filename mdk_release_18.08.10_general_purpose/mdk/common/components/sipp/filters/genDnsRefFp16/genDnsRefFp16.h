///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref genDnsRefFp16 SIPP filter API.
///    

#ifndef __SIPP_GEN_DNS_REF_FP16_H__
#define __SIPP_GEN_DNS_REF_FP16_H__ 

#include <sipp.h>

/// @sf_definition genDnsRefFp16 Generate Reference for Luma Denoise(fp16 input)
/// @sf_description This filter generates an 8-bit reference plane, used by the Luma Denoise filter.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_preserve imgSize, numPlanes
/// @sf_inputs
/// 	- datatypes: half; kernels: 1x1
/// @sf_extasm $(MV_COMMON_BASE)/components/kernelLib/MvISP/kernels/genDnsRefFp16/arch/$(MV_SOC_REV_DIR)/shave/src/genDnsRefFp16.asm
/// @{

/// @brief Parameter structure of the @ref genDnsRefFp16 filter.
typedef struct
{
	/// @sf_desc 8-bit 256-entry table which can be used to apply a  gamma-like adjustment to the denoise reference image. 
    UInt8   *lutGamma;
	/// @sf_desc 8-bit 256-entry table which can be used to attenuate the denoise reference image as a function of distance from the centre pixel. 
    UInt8   *lutDist;
	/// @sf_desc Used  in  conjunction  with  lutDist to  attenuate  the  image  as  a  function  of distance from the centre pixel. 
    int      shift;

 //For still pipeline SPLIT scheme:
    int      xOffset; //= pipeline_X_offset - fullW/2
}
YDnsRefFp16Param;

/// @brief Shave function of the @ref genDnsRefFp16 filter.
void SVU_SYM(svuGenDnsRefFp16)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenDnsRefFp16);

#if defined(__sparc) && defined(DNS_REF_USE_LOCAL_DATA)
//Declare slice-local tables (1D.... addr)
extern UInt8 * SVU_SYM (slcLocalDistLut);
extern UInt8 * SVU_SYM (slcLocalGammaLut);

extern UInt8 * SVU_SYM0(slcLocalDistLut);
extern UInt8 * SVU_SYM0(slcLocalGammaLut);

extern UInt8 * SVU_SYM1(slcLocalDistLut);
extern UInt8 * SVU_SYM1(slcLocalGammaLut);

extern UInt8 * SVU_SYM2(slcLocalDistLut);
extern UInt8 * SVU_SYM2(slcLocalGammaLut);

extern UInt8 * SVU_SYM3(slcLocalDistLut);
extern UInt8 * SVU_SYM3(slcLocalGammaLut);

extern UInt8 * SVU_SYM4(slcLocalDistLut);
extern UInt8 * SVU_SYM4(slcLocalGammaLut);

extern UInt8 * SVU_SYM5(slcLocalDistLut);
extern UInt8 * SVU_SYM5(slcLocalGammaLut);

extern UInt8 * SVU_SYM6(slcLocalDistLut);
extern UInt8 * SVU_SYM6(slcLocalGammaLut);

extern UInt8 * SVU_SYM7(slcLocalDistLut);
extern UInt8 * SVU_SYM7(slcLocalGammaLut);
#endif


/// @}

#endif //__SIPP_GEN_DNS_REF_FP16_H__
