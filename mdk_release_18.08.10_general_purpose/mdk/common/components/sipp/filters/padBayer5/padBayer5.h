///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///

#ifndef __SIPP_PAD_BAYER_5_H__
#define __SIPP_PAD_BAYER_5_H__ 

#include <sipp.h>

void SVU_SYM(svuPadBayer5)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuPadBayer5);

//===============================================================
#if !defined(__MOVICOMPILE__) //PC or SPARC
#include <sippInternal.h> //to customize schedule function
void clampInLines(SippFilter *fptr);
static void askBayerPad(SippFilter *fptr, int outLineNo)
{
    fptr->sch->reqInLns[0][ 0] = outLineNo - 11 +  0;
    fptr->sch->reqInLns[0][ 1] = outLineNo - 11 +  1;
    fptr->sch->reqInLns[0][ 2] = outLineNo - 11 +  2;
    fptr->sch->reqInLns[0][ 3] = outLineNo - 11 +  3;
    fptr->sch->reqInLns[0][ 4] = outLineNo - 11 +  4;
    fptr->sch->reqInLns[0][ 5] = outLineNo - 11 +  5;
    fptr->sch->reqInLns[0][ 6] = outLineNo - 11 +  6;
    fptr->sch->reqInLns[0][ 7] = outLineNo - 11 +  7;
    fptr->sch->reqInLns[0][ 8] = outLineNo - 11 +  8;
    fptr->sch->reqInLns[0][ 9] = outLineNo - 11 +  9;
    fptr->sch->reqInLns[0][10] = outLineNo - 11 + 10;
    fptr->sch->reqInLns[0][11] = outLineNo - 11 + 11;
    fptr->sch->reqInLns[0][12] = outLineNo - 11 + 12;
    clampInLines(fptr);
}
#endif

#endif
