///
/// @file AsmOptimisation.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// 
///

//---------------------------------------------------------------------------//
//------------------------------- INCLUDES ----------------------------------//
//---------------------------------------------------------------------------//

// C++ includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local Modules
#include "AsmOptimisation.h"
#include <Defines.h>

#include <convolution1x7_step.h>

// DMA
#include <swcCdma.h>
#include <swcWhoAmI.h>
#include <svuCommonShave.h>

typedef void (*fp)( u8**, u8**, half*, u32);

//array of function pointer that contain all of the used functions
fp convolution[5] = 
{
	Convolution1x7_step0,
	Convolution1x7_step1,
	Convolution1x7_step2,
	Convolution1x7_step3,
	Convolution1x7_step4,
};


//---------------------------------------------------------------------------//
//------------------------------- Variables ---------------------------------//
//---------------------------------------------------------------------------//


// Buffers
static u8   ALIGNED(8)	conv1x7buf[LINES_NUMBER][MAX_WIDTH + 2 * MAX_PADDING_8];
static u8   ALIGNED(8)	result_buf[MAX_WIDTH * 2];
static u8*				linesPointerConv1x7[LINES_NUMBER];
static u8*				result_pointer[2];


// Normalized values in hex for a blur effect 1/7
const u16  	ALIGNED(8) Conv1x7[7] =
{
	 0x3092, 0x3092, 0x3092, 0x3092, 0x3092, 0x3092, 0x3092
};


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
ENTRYPOINT void AsmOptimisationInit(u32 width)
{	
	linesPointerConv1x7[0] = (u8*)conv1x7buf + MAX_PADDING_8 ;
	linesPointerConv1x7[1] = (u8*)conv1x7buf + MAX_PADDING_8 + (width + 2 * MAX_PADDING_8);
	
	result_pointer[0]    = (u8*)result_buf;
	result_pointer[1]    = (u8*)result_buf + width;

	SHAVE_HALT;
}


// Compute Convolution1x7
ENTRYPOINT void AsmOptimisationRun(u8* inputImage, 
                                 u8 *outputImage,  
                                 u32 width,
                                 u32 height, 
				 u32 step_used)

{
    dmaTransactionList_t task1, task2, task3, task4;
    dmaTransactionList_t *ref1, *ref2, *ref3, *ref4; 
    dmaRequesterId id1 = dmaInitRequester(0);
    u32 i;
    u32 crtInLn = 0;

    ref1 = dmaCreateTransaction(id1, &task1, inputImage + crtInLn * width, linesPointerConv1x7[0], width);
	crtInLn++;
    ref2 = dmaCreateTransaction(id1, &task2, inputImage + crtInLn * width, linesPointerConv1x7[1], width);
	crtInLn++;
    dmaLinkTasks(ref1, 1, ref2);
    dmaStartListTask(ref1);
    dmaWaitTask(ref1);
	
    addPadding(linesPointerConv1x7[0],width, 8);
    addPadding(linesPointerConv1x7[1],width, 8);
    
    convolution[step_used](linesPointerConv1x7, &result_pointer[0], (half*)Conv1x7, width);
	
    rotatePointers();

    for (i = 1; i < height; i++) 
    {
       	ref2 = dmaCreateTransaction(id1, &task2, inputImage + crtInLn * width, linesPointerConv1x7[1], width);
        ref3 = dmaCreateTransaction(id1, &task3, result_pointer[1], outputImage, width);
        dmaLinkTasks(ref2, 1, ref3);
        dmaStartListTask(ref2);

        crtInLn++;
        outputImage += width;

		convolution[step_used](linesPointerConv1x7, &result_pointer[0], (half*)Conv1x7, width);

		dmaWaitTask(ref2); 
		   
		addPadding(linesPointerConv1x7[1],width, 8);
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
    u8* internalP  = linesPointerConv1x7[0];
    linesPointerConv1x7[0] = linesPointerConv1x7[1];
    linesPointerConv1x7[1] = internalP;


    // out lines pointers rotate
    internalP       = result_pointer[0];
    result_pointer[0] = result_pointer[1];
    result_pointer[1] = internalP;
}



