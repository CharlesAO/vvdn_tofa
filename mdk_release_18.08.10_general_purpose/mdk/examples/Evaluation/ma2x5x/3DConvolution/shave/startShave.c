///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     startShave file
///

#include <svuCommonShave.h>
#include <swcCdma.h>
#include <stdio.h>
#include <strings.h>

#include "mvTensorApi.h"
#include "mvTensorShaveDefines.h"

#define IN_BUFF_SIZE  7000
#define OUT_BUFF_SIZE 6500

// Ping pong input buffers
u8 __attribute__(( aligned (8))) localInput [IN_BUFF_SIZE];
u8 __attribute__(( aligned (8))) localInput1[IN_BUFF_SIZE];

// Output buffers
u8 __attribute__(( aligned (8))) localOutputTmp[OUT_BUFF_SIZE];
u8 __attribute__(( aligned (8))) localOutput   [OUT_BUFF_SIZE];
u8 __attribute__(( aligned (8))) localOutput1  [OUT_BUFF_SIZE];
u8 __attribute__(( aligned (8))) localOutput2  [OUT_BUFF_SIZE];
u8 __attribute__(( aligned (8))) localOutput3  [OUT_BUFF_SIZE];

// Array of pointers to be passed as input to conv kernel
input_t __attribute__(( aligned (8))) *inputPtr[MAX_KERNEL_SIZE];

dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1;
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task2;


void start(mvTensorInfo *info, u32 firstMapNo, u32 lastMapNo, s32 firstLineNo, u32 imgHeight)
{
    UNUSED(imgHeight);

    if (firstLineNo != 0) {
        printf("fatal error, firstLineNo must be zero\n");
        exit(1);
    }

    // Adjust stride based on convolution stride enum
    u32 strideSize = info->convStride + 1;
    if (info->convStride == stride8)
        strideSize = 8;

    // Compute sizes for memory transfers
    u32 tapsSize = info->kernelSize * info->kernelSize * info->inputBPP;
    u32 inputSize = info->inputHeight * info->inputWidth * info->inputBPP;
    u32 firstPadd = 32;
    u32 paddBetweenLines = info->kernelSize / 2 * info->inputBPP;


    u32 mapLineBytes        = info->inputWidth * info->inputBPP;
    u32 mapLinePaddedBytes  = mapLineBytes + paddBetweenLines;
    u32 mapSizeBytes        = mapLineBytes * info->inputHeight;
    u32 mapSizePaddedBytes  = mapLinePaddedBytes * info->inputHeight + 2 * firstPadd;

    // Check there is enough memory for the compute size buffers
    if (mapSizePaddedBytes > sizeof(localInput)) {
        printf("FATAL error: buffer needed (%i) exceeds available (%i)\n", mapSizePaddedBytes, sizeof(localInput));
        exit(1);
    }

    if (mapSizeBytes > sizeof(localOutput)) {
         printf("FATAL error, buffer needed (%i) exceeds available (%i)\n", mapSizeBytes, sizeof(localOutput));
           printf("\n");
           exit(1);
    }

    // Set scratch pointers
    u8 *localOutputBasePtr        = &localOutput[0];
    u8 *localOutputBasePtr1       = &localOutput1[0];
    u8 *localOutputBasePtr2       = &localOutput2[0];
    u8 *localOutputBasePtr3       = &localOutput3[0];

    u8 *inputBuffer    = localInput;
    u8 *prefetchBuffer = localInput;

    // Initialize task handle
    u32 id = dmaInitRequester(1);
    dmaTransactionList_t *inputref;
    dmaTransactionList_t *ref2;

    // Choose the right convolution function based on the input parameters
    FUNCPTR_T f = convMatrix[info->convSize][info->convStride][info->convDataType];

    // Loop over the number of maps
    u32 mapNo = firstMapNo;
    while (mapNo < lastMapNo)
    {
        bzero(localOutput,  sizeof(localOutput));
        bzero(localOutput1, sizeof(localOutput1));
        bzero(localOutput2, sizeof(localOutput2));
        bzero(localOutput3, sizeof(localOutput3));

        u32 outputLineNo  = info->inputHeight;
        u32 pp_idx = 0;  // Ping-pong index

        // Prefetch the first ping pong input buffer to warm up
        u8* inputAddr = (u8*)info->input[0];

        inputref = dmaCreateTransactionFullOptions(
                                     id,
                                     &task1,
                                     inputAddr,                                 // src
                                     prefetchBuffer + mapLineBytes + firstPadd, // dst
                                     inputSize,                                 // byte length
                                     mapLineBytes,                              // src line width
                                     mapLineBytes,                              // dst line width
                                     mapLineBytes,                              // src stride
                                     mapLineBytes + paddBetweenLines);          // dst stride
        dmaStartListTask(inputref);

        // Now go in loop
        for(int channelIndex = 0; channelIndex < info->channelsNo; channelIndex++)
        {
            // Ensure our last input DMA task completed
            dmaWaitTask(inputref);

            // Ping pong for buffers
            if (pp_idx == 0) 
	    {
                inputBuffer    = localInput;
                prefetchBuffer = localInput1;
                pp_idx = 1;
            } 
	    else 
	    {
                inputBuffer    = localInput1;
                prefetchBuffer = localInput;
                pp_idx = 0;
            }

            // Prefetch next buffer if not the last one
            if ((channelIndex+1) < (info->channelsNo))
            {
                inputAddr = (u8*)info->input[(channelIndex+1)];

                inputref = dmaCreateTransactionFullOptions(
                                            id,
                                            &task1,
                                            inputAddr,                                 // src
                                            prefetchBuffer + mapLineBytes + firstPadd, // dst
                                            inputSize,                                 // byte length
                                            mapLineBytes,                              // src line width
                                            mapLineBytes,                              // dst line width
                                            mapLineBytes,                              // src stride
                                            mapLineBytes + paddBetweenLines);          // dst stride

                dmaStartListTask(inputref);
                // Do not wait for DMA completion here; we do this at top of next loop
            }

	    // Adjust input pointers for padding
	    inputPtr[0] = (input_t*)(&inputBuffer[0 * (info->inputWidth * info->inputBPP) + firstPadd - paddBetweenLines]);
            inputPtr[1] = (input_t*)(&inputBuffer[1 * (info->inputWidth * info->inputBPP) + firstPadd]);
            inputPtr[2] = (input_t*)(&inputBuffer[2 * (info->inputWidth * info->inputBPP  + paddBetweenLines) + firstPadd]);

	    // Align to the CMX tile boundaries to minimize stalls
            inputPtr[0] = (input_t*)(((u32)inputPtr[0]) & ~7);
            inputPtr[1] = (input_t*)(((u32)inputPtr[1]) & ~7);
            inputPtr[2] = (input_t*)(((u32)inputPtr[2]) & ~7);

            u8 *ptrAux = &localOutputTmp[0];
            u8 **doublePtrAux = &ptrAux;
            u32 tapsOffset = tapsSize * info->channelsNo * mapNo + channelIndex * tapsSize;

            ptrAux = (u8*)(((u32)ptrAux) & ~7);

            // Call a single 1D convolution 
            f((void**)inputPtr, (void**)doublePtrAux, &info->convTaps[tapsOffset], info->inputWidth*info->inputHeight + info->inputHeight);

            // Accumulate the output in a single call
            mvcvAccumulateFp16_asm((half**)&localOutputBasePtr, (half**)doublePtrAux, (info->inputWidth / strideSize)*outputLineNo + info->inputHeight);

            // Process mapNo+1 if it is
            if((mapNo + 1) < lastMapNo)
            {
                u32 tapsOffset = tapsSize * info->channelsNo * (mapNo + 1) + channelIndex * tapsSize;
                // Call a single 1D convolution 
                f((void**)inputPtr, (void**)doublePtrAux, &info->convTaps[tapsOffset], info->inputWidth*info->inputHeight + info->inputHeight);
                // Accumulate the output in a single call
                mvcvAccumulateFp16_asm((half**)&localOutputBasePtr1, (half**)doublePtrAux, (info->inputWidth / strideSize)*outputLineNo + info->inputHeight);
            }

            // Process mapNo+2 if it is
             if((mapNo + 2) < lastMapNo)
             {
                 u32 tapsOffset = tapsSize * info->channelsNo * (mapNo + 2) + channelIndex * tapsSize;
                 // Call a single 1D convolution 
                 f((void**)inputPtr, (void**)doublePtrAux, &info->convTaps[tapsOffset], info->inputWidth*info->inputHeight + info->inputHeight);
                 // Accumulate the output in a single call
                 mvcvAccumulateFp16_asm((half**)&localOutputBasePtr2, (half**)doublePtrAux, (info->inputWidth / strideSize)*outputLineNo + info->inputHeight);
             }

             // Process mapNo+3 if it is
             if((mapNo + 3) < lastMapNo)
             {
                 u32 tapsOffset = tapsSize * info->channelsNo * (mapNo + 3) + channelIndex * tapsSize;
                 // Call a single 1D convolution 
                 f((void**)inputPtr, (void**)doublePtrAux, &info->convTaps[tapsOffset], info->inputWidth*info->inputHeight + info->inputHeight);
                 // Accumulate the output in a single call
                 mvcvAccumulateFp16_asm((half**)&localOutputBasePtr3, (half**)doublePtrAux, (info->inputWidth / strideSize)*outputLineNo + info->inputHeight);
             }
        } // end for channels in map

        u32 outputLineSize = (info->inputWidth / strideSize) * info->outputBPP;

	// DMA results to the output buffers 
        ref2 = dmaCreateTransactionSrcStride(
                         id,
                         &task1,
                         localOutput,                        // src
                         info->output[mapNo],                // dst
                         outputLineSize * outputLineNo,      // byte length
                         outputLineSize,                     // line width
                         outputLineSize + paddBetweenLines); // src stride

        dmaStartListTask(ref2);
        dmaWaitTask(ref2);
        mapNo++;

        if(mapNo < lastMapNo)
        {
            ref2 = dmaCreateTransactionSrcStride(
                                     id,
                                     &task1,
                                     localOutput1,                       // src
                                     info->output[mapNo],                // dst
                                     outputLineSize * outputLineNo,      // byte length
                                     outputLineSize,                     // line width
                                     outputLineSize + paddBetweenLines); // src stride

            dmaStartListTask(ref2);
            dmaWaitTask(ref2);
            mapNo++;
        }

        if(mapNo < lastMapNo)
        {
            ref2 = dmaCreateTransactionSrcStride(
                                     id,
                                     &task1,
                                     localOutput2,                       // src
                                     info->output[mapNo],                // dst
                                     outputLineSize * outputLineNo,      // byte length
                                     outputLineSize,                     // line width
                                     outputLineSize + paddBetweenLines); // src stride

            dmaStartListTask(ref2);
            dmaWaitTask(ref2);
            mapNo++;
        }

        if(mapNo < lastMapNo)
        {
            ref2 = dmaCreateTransactionSrcStride(
                                     id,
                                     &task1,
                                     localOutput3,                       // src
                                     info->output[mapNo],                // dst
                                     outputLineSize * outputLineNo,      // byte length
                                     outputLineSize,                     // line width
                                     outputLineSize + paddBetweenLines); // src stride

            dmaStartListTask(ref2);
            dmaWaitTask(ref2);
            mapNo++;
        }
    }

    SHAVE_HALT;
}
