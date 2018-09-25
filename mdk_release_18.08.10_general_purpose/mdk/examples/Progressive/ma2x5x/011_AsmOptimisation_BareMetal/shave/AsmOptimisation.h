///
/// @file Coptimisation.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute DOG algorythm
///


#ifndef ASMOPTIMISATION_H
#define ASMOPTIMISATION_H


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
ENTRYPOINT void AsmOptimisationInit(u32 width);






/******************************************************************************


 @param     : inputBuffer             : pointer for input image
 @param     : outputBuffer            : pointer for output image
 @param     : width                 : image width
 @param     : height                : image height
 @return    : void

******************************************************************************/
ENTRYPOINT void AsmOptimisationRun(u8 *inputBuffer, 
                                 u8 *outputBuffer,  
                                 u32 width,
                                 u32 height,
								 u32 step_used);





#endif // COPTIMISATION_H
