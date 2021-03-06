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

#ifndef __TESTHWLUMADNS_H__
#define __TESTHWLUMADNS_H__

#include <sipp.h>
#include <sippTestCommon.h>

#include "testHwLumaDnsSetup.h"


//The full-frame buffers
extern UInt8 DDR_DATA TestHwLumaDns_dmaIn0_buffer[ SIPP_HW_LUM_DNS_BUFFER_WIDTH * SIPP_HW_LUM_DNS_BUFFER_HEIGHT * SIPP_HW_LUM_DNS_BUFFER_NUM_PLANES ]ALIGNED(8);
extern UInt8 DDR_DATA TestHwLumaDns_dmaOut0_buffer[ SIPP_HW_LUM_DNS_BUFFER_WIDTH * SIPP_HW_LUM_DNS_BUFFER_HEIGHT * SIPP_HW_LUM_DNS_BUFFER_NUM_PLANES ]ALIGNED(8);
	
///

typedef struct TestHwLumaDns
{
	//Filter references for human readable shape
	SippPipeline *pl;

	//Declare references to all filters in the pipeline
	SippFilter *dmaIn0;
	SippFilter *dmaOut0;
	SippFilter *ydnsMa2x5x0;

} TestHwLumaDns;

void readTestHwLumaDnsInput(TestHwLumaDns *pPl);
void writeTestHwLumaDnsOutput(TestHwLumaDns *pPl);

void buildTestHwLumaDns(TestHwLumaDns *pPl);
void configTestHwLumaDns(TestHwLumaDns *pPl);


#endif