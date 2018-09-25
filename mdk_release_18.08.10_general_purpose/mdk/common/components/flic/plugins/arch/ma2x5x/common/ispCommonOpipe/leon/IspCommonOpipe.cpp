///
/// @file      IspCommonOpipe.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Opipe common and serialization side Interface for the Isp flic plugins.
///            Platform(s) supported : ma2x5x
///

#include "IspCommonOpipe.h"
#include <rtems.h>
#include "Opipe.h"
#include "OpipeApps.h"
#include "ImgTypes.h"


pthread_mutex_t IspCommonOpipe::lockMtx;
uint32_t        IspCommonOpipe::plgIspMtxInstanceCnt = 0;

//#################################################################################################
//Opipe cmx circular buffers
uint16_t IspCommonOpipe::cSigma  [PLG_ISP_MAX_W * I_CBUFF_H    ] SECTION(".cmx.opipe") ALIGNED(8);  //Bayer is 10bit
uint16_t IspCommonOpipe::cDbyrY  [PLG_ISP_MAX_W * DBYR_Y_H     ] SECTION(".cmx.opipe") ALIGNED(8);
uint16_t IspCommonOpipe::cSharpY [PLG_ISP_MAX_W * SHARP_Y_H    ] SECTION(".cmx.opipe") ALIGNED(8);
uint8_t  IspCommonOpipe::cLut    [PLG_ISP_MAX_W * O_CBUFF_H * 3] SECTION(".cmx.opipe") ALIGNED(8);
uint8_t  IspCommonOpipe::cUpfirDn[PLG_ISP_MAX_W * O_CBUFF_H * 3] SECTION(".cmx.opipe") ALIGNED(8);  //as for yuv444



