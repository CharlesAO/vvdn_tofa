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

#ifndef __TESTHWCHROMADNS_H__
#define __TESTHWCHROMADNS_H__

#include <sipp.h>
#include <sippTestCommon.h>

#include "testHwChromaDnsSetup.h"


//The full-frame buffers
extern UInt8 DDR_DATA TestHwChromaDns_dmaIn0_buffer[ 792 * 645 * 3 ]ALIGNED(8);
extern UInt8 DDR_DATA TestHwChromaDns_dmaOut0_buffer[ 792 * 645 * 3 ]ALIGNED(8);
	
///

typedef struct TestHwChromaDns
{
	//Filter references for human readable shape
	SippPipeline *pl;

	//Declare references to all filters in the pipeline
	SippFilter *dmaIn0;
	SippFilter *chromadnsMa2x5x0;
	SippFilter *dmaOut0;

} TestHwChromaDns;

void readTestHwChromaDnsInput(TestHwChromaDns *pPl);
void writeTestHwChromaDnsOutput(TestHwChromaDns *pPl);

void buildTestHwChromaDns(TestHwChromaDns *pPl);
void configTestHwChromaDns(TestHwChromaDns *pPl);


#endif