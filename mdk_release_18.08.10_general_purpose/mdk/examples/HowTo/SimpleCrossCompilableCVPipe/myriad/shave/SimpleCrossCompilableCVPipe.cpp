///
/// @file SimpleCrossCompilableCVPipe.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute DOG algorythm
///

//---------------------------------------------------------------------------//
//------------------------------- INCLUDES ----------------------------------//
//---------------------------------------------------------------------------//

// C++ includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local Modules
#include "SimpleCrossCompilableCVPipe.h"
#include <Defines.h>

#include <absoluteDiff_intr.h>
#include <convolution3x3_intr.h>
#include <convolution7x7_intr.h>

// DMA
#include <swcCdma.h>
#include <svuCommonShave.h>

#ifndef __PC__
#include <swcWhoAmI.h>
#endif

//---------------------------------------------------------------------------//
//------------------------------- Variables ---------------------------------//
//---------------------------------------------------------------------------//


// Buffers
static u8   ALIGNED(8)	conv7x7buf[CONV7x7_KERNEL_SIZE + 1][MAX_WIDTH + 2 * MAX_PADDING_8];
static u8   ALIGNED(8) 	conv3x3buf[CONV3x3_KERNEL_SIZE + 1][MAX_WIDTH + 2 * MAX_PADDING_8];
static u8   ALIGNED(8)	conv7x7out_buf[MAX_WIDTH];
static u8   ALIGNED(8)	conv3x3out_buf[MAX_WIDTH];
static u8   ALIGNED(8)	result_buf[MAX_WIDTH * 2];
static u8*				con7x7out_pointer[1];
static u8*				con3x3out_pointer[1];
static u8*				result_pointer[2];

static u8*				linesPointerConv7x7[CONV7x7_KERNEL_SIZE + 1];
static u8*				linesPointerConv3x3[CONV3x3_KERNEL_SIZE + 1];

// Normalized values in hex for a blur effect 1/9
const u16  	ALIGNED(8) Conv3x3[9] =
{
    0x2f1c, 0x2f1c, 0x2f1c,
    0x2f1c, 0x2f1c, 0x2f1c,
    0x2f1c, 0x2f1c, 0x2f1c,
};

// Normalized values in hex for a blur effect 1/49
const u16  	ALIGNED(8) Conv7x7[49] =
{
	 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532,
	 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532,	
	 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532,
	 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532,
	 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532,
	 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532,
	 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532, 0x2532
};

dmaTransactionList_t DMA_DESCRIPTORS task1, task2, task3, task4;

//---------------------------------------------------------------------------//
//------------------------------- Functions ---------------------------------//
//---------------------------------------------------------------------------//

// Add padding
static void addPadding(u8* pointer, u32 width, u32 paddingSize);

// Function for rotating line pointers needed for DMA optimization
static void rotatePointers(void);

// Clamp function for clamping values when computing what line to gather
int clamp(int x, int y, int z);


// Init Function for BASE algo
ENTRYPOINT void SimpleCrossCompilableCVPipeInit(u32 width)
{
	u32 i;

	for (i = 0; i < CONV7x7_KERNEL_SIZE + 1; i++)
	{
		linesPointerConv7x7[i] = (u8*)conv7x7buf + MAX_PADDING_8 + i * (width + 2 * MAX_PADDING_8);
	}

	for (i = 0; i < CONV3x3_KERNEL_SIZE; i++)
	{
		linesPointerConv3x3[i] = (u8*)conv3x3buf + MAX_PADDING_8 + i * (width + 2 * MAX_PADDING_8);
	}
	
	con7x7out_pointer[0] = (u8*)conv7x7out_buf;
	con3x3out_pointer[0] = (u8*)conv3x3out_buf;
	result_pointer[0]    = (u8*)result_buf;
	result_pointer[1]    = (u8*)result_buf + width;

	SHAVE_HALT;
}


// Compute DOG Algo
ENTRYPOINT void SimpleCrossCompilableCVPipeRun(u8* inputImage, 
                                 u8 *outputImage,  
                                 u32 width,
                                 u32 height)

{
    dmaTransactionList_t *ref1, *ref2, *ref3, *ref4; 
    dmaRequesterId id1 = dmaInitRequester(0);
    u32 i;
    u32 crtInLn = -3;

	for (i = 0; i < CONV7x7_KERNEL_SIZE + 1; i++)
    {
        ref1 = dmaCreateTransaction(id1, &task1, inputImage + clamp(crtInLn, 0, height) * width, linesPointerConv7x7[i], width);
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);
		
		addPadding(linesPointerConv7x7[i],width, 8);
        crtInLn++;
    }
	linesPointerConv3x3[0] = linesPointerConv7x7[2];
	linesPointerConv3x3[1] = linesPointerConv7x7[3];
	linesPointerConv3x3[2] = linesPointerConv7x7[4];

	Convolution3x3_intr(linesPointerConv3x3, &con3x3out_pointer[0], (half*)Conv3x3, width);
	Convolution7x7_intr(linesPointerConv7x7, &con7x7out_pointer[0], (half*)Conv7x7, width);
	AbsoluteDiff_intr(&con3x3out_pointer[0], &con7x7out_pointer[0], &result_pointer[0], width);

	rotatePointers();

	for (i = 1; i < height; i++) 
    {
       	ref2 = dmaCreateTransaction(id1, &task2, inputImage + clamp(crtInLn, 0, height-1) * width, linesPointerConv7x7[7], width);
        ref3 = dmaCreateTransaction(id1, &task3, result_pointer[1], outputImage, width);
        dmaLinkTasks(ref2, 1, ref3);
        dmaStartListTask(ref2);

        crtInLn++;
        outputImage += width;

	    Convolution3x3_intr(linesPointerConv3x3, &con3x3out_pointer[0], (half *)Conv3x3, width);
	    Convolution7x7_intr(linesPointerConv7x7, &con7x7out_pointer[0], (half *)Conv7x7, width);
	    AbsoluteDiff_intr(&con3x3out_pointer[0], &con7x7out_pointer[0], &result_pointer[0], width);

		dmaWaitTask(ref2); 
		   
		addPadding(linesPointerConv7x7[7],width, 8);
        rotatePointers();
    }

	ref4 = dmaCreateTransaction(id1, &task4, result_pointer[1], outputImage, width);
    dmaStartListTask(ref4);
    dmaWaitTask(ref4);

    SHAVE_HALT;
}

static void addPadding(u8* pointer, u32 width, u32 paddingSize)
{
    u32 widthAux = width - 1;

    for (u32 i = 1; i <= paddingSize; i++)
    {
    	// Pad to the beginning and the end of the line
        *(pointer - i) = *pointer;
        *(pointer + widthAux + i) = *(pointer + widthAux);
    }
}

static void rotatePointers(void)
{
    // In lines pointers rotate
    u8* internalP  = linesPointerConv7x7[0];
    linesPointerConv7x7[0] = linesPointerConv7x7[1];
    linesPointerConv7x7[1] = linesPointerConv7x7[2];
    linesPointerConv7x7[2] = linesPointerConv7x7[3];
    linesPointerConv7x7[3] = linesPointerConv7x7[4];
    linesPointerConv7x7[4] = linesPointerConv7x7[5];
    linesPointerConv7x7[5] = linesPointerConv7x7[6];
    linesPointerConv7x7[6] = linesPointerConv7x7[7];
    linesPointerConv7x7[7] = internalP;

	linesPointerConv3x3[0] = linesPointerConv7x7[2];
	linesPointerConv3x3[1] = linesPointerConv7x7[3];
	linesPointerConv3x3[2] = linesPointerConv7x7[4];

    // out lines pointers rotate
    internalP       = result_pointer[0];
    result_pointer[0] = result_pointer[1];
    result_pointer[1] = internalP;
}

s32 clamp(s32 val, s32 lowVal, s32 highVal)
{
    if (val < lowVal) 
	{
		return lowVal;
    }
    else  
	{
		if (val > highVal) 
		{
			return highVal;
		}			
		else 
		{
			return val;
		}
	}
}
