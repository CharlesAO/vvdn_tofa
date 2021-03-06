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

#ifndef __TESTHWLUT_H__
#define __TESTHWLUT_H__

#include <sipp.h>
#include <sippTestCommon.h>

#include "testHwLutSetup.h"


//The full-frame buffers
extern UInt16 DDR_DATA TestHwLut_dmaIn0_buffer[ SIPP_HW_LUT_BUFFER_WIDTH * SIPP_HW_LUT_BUFFER_HEIGHT * SIPP_HW_LUT_BUFFER_NUM_IN_PLANES ]ALIGNED(8);
extern UInt8 DDR_DATA TestHwLut_dmaOut0_buffer[ SIPP_HW_LUT_BUFFER_WIDTH * SIPP_HW_LUT_BUFFER_HEIGHT * SIPP_HW_LUT_BUFFER_NUM_IN_PLANES ]ALIGNED(8);
	
///

typedef struct TestHwLut
{
	//Filter references for human readable shape
	SippPipeline *pl;

	//Declare references to all filters in the pipeline
	SippFilter *dmaIn0;
	SippFilter *dmaOut0;
	SippFilter *lutMa2x5x0;

} TestHwLut;

void readTestHwLutInput(TestHwLut *pPl);
void writeTestHwLutOutput(TestHwLut *pPl);

void buildTestHwLut(TestHwLut *pPl);
void configTestHwLut(TestHwLut *pPl);


#endif