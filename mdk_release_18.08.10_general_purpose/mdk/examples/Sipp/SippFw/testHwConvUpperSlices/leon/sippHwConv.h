///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the source code of the SIPP pipeline
///            generated by the SIPP Graph Designer.
///
///
///  ************************ DO NOT EDIT, THIS IS A GENERATED FILE!!!! ************************
/// 

#ifndef __SIPPHWCONV_H__
#define __SIPPHWCONV_H__

#include <sipp.h>
#include <sippTestCommon.h>

#include "sippHwConvDefs.h"


//The full-frame buffers
extern UInt8 DDR_DATA SippHwConv_dmaOut0_buffer[ SIPP_HW_CONVOLUTION_BUFFER_WIDTH * SIPP_HW_CONVOLUTION_BUFFER_HEIGHT * SIPP_HW_CONVOLUTION_BUFFER_NUM_PLANES ]ALIGNED(8);
	
///

typedef struct SippHwConv
{
	//Filter references for human readable shape
	SippPipeline *pl;

	//Declare references to all filters in the pipeline
	SippFilter *dmaIn0;
	SippFilter *dmaOut0;
	SippFilter *convolution0;

} SippHwConv;

void readSippHwConvInput(SippHwConv *pPl);
void writeSippHwConvOutput(SippHwConv *pPl);

void buildSippHwConv(SippHwConv *pPl);
void configSippHwConv(SippHwConv *pPl);


#endif