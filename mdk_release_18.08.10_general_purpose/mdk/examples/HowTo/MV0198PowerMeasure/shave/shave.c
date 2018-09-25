///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MV0198PowerMeasure source code
///


// 1: Includes
// ----------------------------------------------------------------------------

#include <swcCdma.h>
#include <swcWhoAmI.h>
#include "shave.h"
#include "mv_types.h"
#include <swcWhoAmI.h>
#include <svuCommonShave.h>
#include <swcCdma.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define DDR_DATA 			__attribute__((section(".ddr.data")))
#define ENTRYPOINT 			extern "C"
#define DMA_DESCRIPTORS     __attribute__((section(".cmx.cdmaDescriptors")))
#define KERNEL_NR_LINES		3
#define MAX_LINE_WIDTH 		4096

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern void mvcvDilate3x3(u8** src, u8** dst, u8** kernel, u32 width);

// 4: Static Local Data
// ----------------------------------------------------------------------------
static dmaTransactionList_t DMA_DESCRIPTORS task1, task2, task3, task4;
static dmaTransactionList_t *ref1, *ref2, *ref3, *ref4;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void shaveRun(u8 *inputImage, u8 *outputImage, u32 width, u32 height)
{
	u8 *inAddress;
	u8 *outAddress;
	u32 id1;
	u8* input[KERNEL_NR_LINES];
	u8* output[1];
	u32 line;
	u8 matrix[3][3]= { {1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	u8 bufferedInLineA[MAX_LINE_WIDTH*KERNEL_NR_LINES];
	u8 bufferedInLineB[MAX_LINE_WIDTH];

	u8** kernelDilate;
	u8* kernelLineDilate[3];
	u8 index;

	id1 = dmaInitRequester(1);

	for (line = 0; line < height; line++)
	{
	  	inAddress = (u8*) ((u32) inputImage + line * width);
	    outAddress = (u8*) ((u32) outputImage + line * width);

	    ref1 = dmaCreateTransaction(id1, &task1, (inAddress + 0*width), &bufferedInLineA[0 * MAX_LINE_WIDTH], width);
		dmaStartListTask(ref1);
		dmaWaitTask(ref1);

		ref2 = dmaCreateTransaction(id1, &task2, (inAddress + 1*width), &bufferedInLineA[1 * MAX_LINE_WIDTH], width);
		dmaStartListTask(ref2);
		dmaWaitTask(ref2);

		ref3 = dmaCreateTransaction(id1, &task3, (inAddress + 2*width), &bufferedInLineA[2 * MAX_LINE_WIDTH], width);
		dmaStartListTask(ref3);
		dmaWaitTask(ref3);

		//make dilate	A -> B
		input[0] = &bufferedInLineA[(0)*MAX_LINE_WIDTH];
		input[1] = &bufferedInLineA[(1)*MAX_LINE_WIDTH];
		input[2] = &bufferedInLineA[(2)*MAX_LINE_WIDTH];

		output[0]=&bufferedInLineB[0];

		for( index = 0;index < KERNEL_NR_LINES; index++)
		{
			kernelLineDilate[index] = matrix[index]; // pointer to the i-th row
		}

		kernelDilate = (u8**)kernelLineDilate;

    	mvcvDilate3x3(input,output, kernelDilate, width);

		ref4 = dmaCreateTransaction(id1, &task4, &bufferedInLineB[0], outAddress, width);
		dmaStartListTask(ref4);
		dmaWaitTask(ref4);
	}
	SHAVE_HALT;
}
