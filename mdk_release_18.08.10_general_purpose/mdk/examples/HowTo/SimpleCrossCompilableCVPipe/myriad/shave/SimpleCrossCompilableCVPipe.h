///
/// @file SimpleCrossCompilableCVPipe.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute DOG algorythm
///


#ifndef SIMPLE_CROSS_H
#define SIMPLE_CROSS_H


// MvCV
#include <mvcv.h>

#include <Defines.h>

//---------------------------------------------------------------------------//
//-------------------------- Exported Functions -----------------------------//
//---------------------------------------------------------------------------//


/******************************************************************************

 @brief     : Init Function, used to allocate all internal structures for BASE_ALGO

 @param     : width                 : image width

******************************************************************************/
ENTRYPOINT void SimpleCrossCompilableCVPipeInit(u32 width);






/******************************************************************************

 @brief     : Compute DOG Algo

 @param     : inputBuffer             : pointer for input image
 @param     : outputBuffer            : pointer for output image
 @param     : width                 : image width
 @param     : height                : image height
 @return    : void

******************************************************************************/
ENTRYPOINT void SimpleCrossCompilableCVPipeRun(u8 *inputBuffer, 
                                 u8 *outputBuffer,  
                                 u32 width,
                                 u32 height);





#endif // SIMPLE_CROSS_H
