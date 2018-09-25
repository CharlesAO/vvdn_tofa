/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the declaration of the @ref threshold SIPP filter API.
///

#ifndef __SIPP_DISP2DEPTH_H__
#define __SIPP_DISP2DEPTH_H__

#include <sipp.h>



/// @brief Parameter structure of the @ref threshold filter.
typedef struct
{
    half disp2depthLUT[3040] ALIGNED(16);
    bool flip_disp2depth;
    bool _16bitmode;
}
Disp2depthParam;



//Shave symbols that need to be understood by leon need to be declared through "SVU_SYM" MACRO,
//as moviCompile adds a leading SS_ to symbol exported
#ifdef __cplusplus
extern "C" {
#endif
void SVU_SYM(svudisp2depth)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svudisp2depth);
#ifdef __cplusplus
}
#endif



#endif //__SIPP_DISP2DEPTH_H__
