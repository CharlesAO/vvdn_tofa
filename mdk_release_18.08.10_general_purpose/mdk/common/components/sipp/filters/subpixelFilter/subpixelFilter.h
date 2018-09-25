/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref subpixel SIPP filter API.
///

#ifndef __SIPP_SUBPIXELFILTER_H__
#define __SIPP_SUBPIXELFILTER_H__

#include <sipp.h>



/// @brief Parameter structure of the @ref threshold filter.
typedef struct SubpixelFilterParam
{
    uint8_t numDisp;
    uint8_t numFractionalBits;
    uint8_t * LUT;
} SubpixelFilterParam;



//Shave symbols that need to be understood by leon need to be declared through "SVU_SYM" MACRO,
//as moviCompile adds a leading SS_ to symbol exported
#ifdef __cplusplus
extern "C" {
#endif
void SVU_SYM(svuSubpixelFilter)(SippFilter *fptr);
#ifdef __cplusplus
}
#endif



#endif //__SIPP_SUBPIXELFILTER_H__
