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

#ifndef __TESTHWDOGLTM_H__
#define __TESTHWDOGLTM_H__

#include <sipp.h>
#include <sippTestCommon.h>

#include "testHwDogLtmSetup.h"


//The full-frame buffers
extern UInt8 DDR_DATA TestHwDoGLTM_dmaIn0_buffer[ 1584 * 1290 * 1 ]ALIGNED(8);
extern UInt8 DDR_DATA TestHwDoGLTM_dmaOut0_buffer[ 1584 * 1290 * 1 ]ALIGNED(8);
	
///

typedef struct TestHwDoGLTM
{
	//Filter references for human readable shape
	SippPipeline *pl;

	//Declare references to all filters in the pipeline
	SippFilter *dmaIn0;
	SippFilter *dmaOut0;
	SippFilter *dogLtmMa2x5x0;

} TestHwDoGLTM;

void readTestHwDoGLTMInput(TestHwDoGLTM *pPl);
void writeTestHwDoGLTMOutput(TestHwDoGLTM *pPl);

void buildTestHwDoGLTM(TestHwDoGLTM *pPl);
void configTestHwDoGLTM(TestHwDoGLTM *pPl);


#endif