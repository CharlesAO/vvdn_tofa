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

#ifndef __SIPPTUTA3MODEL_H__
#define __SIPPTUTA3MODEL_H__

#include <sipp.h>
#include <sippTestCommon.h>



//The full-frame buffers
extern UInt8 DDR_DATA SippTutA3Model_dmaIn0_buffer[ 512 * 512 * 1 ]ALIGNED(8);
extern UInt8 DDR_DATA SippTutA3Model_dmaOut1_buffer[ 512 * 512 * 1 ]ALIGNED(8);
	
///

typedef struct SippTutA3Model
{
	//Filter references for human readable shape
	SippPipeline *pl;

	//Declare references to all filters in the pipeline
	SippFilter *dmaIn0;
	SippFilter *dmaOut1;
	SippFilter *conv5x52;

} SippTutA3Model;

void readSippTutA3ModelInput(SippTutA3Model *pPl);
void writeSippTutA3ModelOutput(SippTutA3Model *pPl);

void buildSippTutA3Model(SippTutA3Model *pPl);
void configSippTutA3Model(SippTutA3Model *pPl);


#endif