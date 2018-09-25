///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref genDnsRef SIPP filter API.
///    

#ifndef __SIPP_GEN_DNS_REF_H__
#define __SIPP_GEN_DNS_REF_H__ 

#include <sipp.h>

/// @sf_definition genDnsRef Generate Reference for Luma Denoise
/// @sf_description This filter generates an 8-bit reference plane, used by the Luma Denoise filter.
/// @sf_group ISP
/// @sf_outdatatype UInt8
/// @sf_inputs
/// 	- datatypes: UInt8; kernels: 1x1
/// @{

/// @brief Parameter structure of the @ref genDnsRef filter.
typedef struct
{
	/// @sf_desc 8-bit 256-entry table which can be used to apply a  gamma-like adjustment to the denoise reference image. 
    UInt8   *lutGamma;
	/// @sf_desc 8-bit 256-entry table which can be used to attenuate the denoise reference image as a function of distance from the centre pixel. 
    UInt8   *lutDist;
	/// @sf_desc Used  in  conjunction  with  lutDist to  attenuate  the  image  as  a  function  of distance from the centre pixel. 
    int      shift;
}
YDnsRefParam;

/// @brief Shave function of the @ref genDnsRef filter.
void SVU_SYM(svuGenDnsRef)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuGenDnsRef);

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

#endif //__SIPP_GEN_DNS_REF_H__
