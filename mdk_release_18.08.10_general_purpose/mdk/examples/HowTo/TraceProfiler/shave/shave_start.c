///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main shave file
///

// 1: Includes
#include "mv_types.h"
#include <stdio.h>
#include <swcWhoAmI.h>
#include <svuCommonShave.h>
#include <swcCdma.h>
#include <shave_start.h>
#include <logMsg.h>
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

#define MYRIAD2_MAX_NUMBER_OF_TASKS 62

// Set the maximum kernel size
#define KERNEL_SIZE 7
#define LINES_NUM   (KERNEL_SIZE + 1)
#define MAXWIDTH    1280
#define PADDING     16 // for allignament reason is prefereable to keep this allign

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern void mvcvConvolution7x7_asm(u8** in, u8** out, unsigned short * conv, u32 inWidth);
extern void mvcvConvolution3x3_asm(u8** in, u8** out, unsigned short * conv, u32 inWidth);

// 4: Static Local Data
// ----------------------------------------------------------------------------

static unsigned char inLinesBuff[LINES_NUM][MAXWIDTH + PADDING * 2] __attribute__ ((aligned (16)));
// copy from CMX to out address on 2 lines
static  unsigned char outLinesBuff[2][MAXWIDTH + PADDING * 2] __attribute__ ((aligned (16)));
// in address lines from DDR, will be input for kernel
static  unsigned char* inLinesAddr[LINES_NUM];
// in address lines from DDR, will be input for kernel
static  unsigned char* outLinesAddr[LINES_NUM];

static s32 hightSl  = 0;
static s32 lnStart  = 0;
static u8* inAddr   = 0;
static  u8* outAddr  = 0;
static  s32 crtInLn  = 0;
static  s32 idx      = 0;
static unsigned int dbg=0;

//Blur
static unsigned short convMatrix7x7[49] =
{
		0x2538,  0x2538,  0x2538, 0x2538, 0x2538, 0x2538, 0x2538,
		0x2538,  0x2538,  0x2538, 0x2538, 0x2538, 0x2538, 0x2538,
		0x2538,  0x2538,  0x2538, 0x2538, 0x2538, 0x2538, 0x2538,
		0x2538,  0x2538,  0x2538, 0x2538, 0x2538, 0x2538, 0x2538,
		0x2538,  0x2538,  0x2538, 0x2538, 0x2538, 0x2538, 0x2538,
		0x2538,  0x2538,  0x2538, 0x2538, 0x2538, 0x2538, 0x2538,
		0x2538,  0x2538,  0x2538, 0x2538, 0x2538, 0x2538, 0x2538,
};

// Blur
static unsigned short convMatrix3x3[9] =
{
		0x2f1c, 0x2f1c, 0x2f1c,
		0x2f1c, 0x2f1c, 0x2f1c,
		0x2f1c, 0x2f1c, 0x2f1c,
};

dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1, task2, task3, task4;
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void initPointers(u32 kernelSize);
void rotatePointers(u32 kernelSize);


void ApplicationStart(u8* outFrBuff, u8* inFrBuff, u32 width, u32 height, u32 nrShaves, u32 sliceIdx, u32 kernel_size)
{
	dmaTransactionList_t *ref1, *ref2, *ref3, *ref4;

    LOG_INFO("ApplicationStart(%p, %p, %d, %d, %d, %d, %d) {",
            outFrBuff, inFrBuff, width, height, nrShaves, sliceIdx, kernel_size);

	initPointers(kernel_size);
	hightSl  = (height / nrShaves);
	lnStart  = hightSl * sliceIdx;
	inAddr   = inFrBuff;
	outAddr  = outFrBuff + lnStart * width;
	crtInLn  = 0;
	idx      = 0;
	if ((sliceIdx + 1) == nrShaves)
	{
		hightSl = height - lnStart;
	}


	crtInLn = lnStart - (kernel_size / 2);

	int shaveNumber = swcWhoAmI() - PROCESS_SHAVE0;
	u32 id1 = dmaInitRequester(shaveNumber);

	// get first 8 lines in the internal cmx buffer
	for(idx = 0; (u32)idx < kernel_size; idx++)
	{
		inAddr = inFrBuff + (clamp(crtInLn, 0, height) * width);
		ref1 = dmaCreateTransaction(id1, &task1, inAddr, inLinesAddr[idx], width);
		dmaStartListTask(ref1);
		dmaWaitTask(ref1);

		crtInLn++;
	}

	if(kernel_size == 7)
		mvcvConvolution7x7_asm(inLinesAddr, &outLinesAddr[0], convMatrix7x7, width);
	else if(kernel_size == 3)
		mvcvConvolution3x3_asm(inLinesAddr, &outLinesAddr[0], convMatrix3x3, width);

	//rotate pointers
	rotatePointers(kernel_size);

	// procesate all lines for this shave
	for(idx = 1; idx < hightSl; idx++) //hightSl
	{
		inAddr = inFrBuff + clamp(crtInLn, 0, height) * width;
		ref2 = dmaCreateTransaction(id1, &task2, inAddr, inLinesAddr[kernel_size], width);

		crtInLn++;

		ref3 = dmaCreateTransaction(id1, &task3, outLinesAddr[1], outAddr, width);

		dmaLinkTasks(ref2, 1, ref3);
		dmaStartListTask(ref2);

		outAddr += width;

		if(kernel_size == 7){
			mvcvConvolution7x7_asm(inLinesAddr, &outLinesAddr[0], convMatrix7x7, width);
		}
		else if(kernel_size == 3){
			mvcvConvolution3x3_asm(inLinesAddr, &outLinesAddr[0], convMatrix3x3, width);
		}
		dbg = idx;
		dmaWaitTask(ref2); //=?
				rotatePointers(kernel_size);
	}

	ref4 = dmaCreateTransaction(id1, &task4, outLinesAddr[1], outAddr, width);
	dmaStartListTask(ref4);
	dmaWaitTask(ref4);

	outAddr += width;

    LOG_INFO("} // ApplicationStart(%p, ...)", outFrBuff);

	SHAVE_HALT;//stopping the shave
}


void CopyLine(u32* src,u32* dst,u32 width)
{
	u32 i;
	for (i=0;i<(width>>2);i++){
		//for (i=0;i<7;i++){
		dst[i]=src[i];
	}
	return;
}

// local function implementation that use global variable
// moviCompile initialization error generate this necesity
void initPointers(u32 kernelSize)
{
	u32 i;

	for (i = 0; i<= kernelSize; i++)
	{
		inLinesAddr[i] = inLinesBuff[i] + PADDING;
	}

	outLinesAddr[0] = outLinesBuff[0];
	outLinesAddr[1] = outLinesBuff[1];
}

void rotatePointers(u32 kernelSize)
{
	u32 i;
	// in lines pointers rotate
	u8* internalP  = inLinesAddr[0];
	for (i = 0; i< kernelSize; i++)
	{
		inLinesAddr[i] = inLinesAddr[i + 1];
	}
	inLinesAddr[kernelSize] = internalP;
	// out lines pointers rotate
	internalP       = outLinesAddr[0];
	outLinesAddr[0] = outLinesAddr[1];
	outLinesAddr[1] = internalP;
}
