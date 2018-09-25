#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/monoImbalance/monoImbalance.h>

#ifdef SIPP_USE_MVCV
void mvcvMonoImbalance_asm(UInt16** in, half** out,int thrDark, int thrBright, int inputBits, UInt32 inWidth);
#else
void mvcvMonoImbalance    (UInt16** in, half** out,int thrDark, int thrBright, int inputBits, UInt32 inWidth)
{
    int avg, luma, diff,thr;
    int i;
    int rshift = inputBits + 1;
    UInt16* lines[3];
    float  result;
    float  normVal= 1.0f/((1<<inputBits)-1);

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){

        /* Average 4-connected neighbours */
        avg = (lines[0][i] + lines[2][i] + lines[1][i-1] + lines[1][i+1]) >> 2;

        /* Compute Luma*2 */
        luma = avg + lines[1][i];

        /* Compute luma-adapted threshold and re-normalize to input pixel range */
        thr = thrDark + (((thrBright - thrDark) * luma) >> rshift);

        diff = avg - lines[1][i];

        /* Filter the difference between current pixel and neighbours */
        if (diff > thr) {
            diff = 2*thr - diff;
            if (diff < 0) {
                diff = 0;
            }
        } else if (diff < -thr) {
            diff = -2*thr - diff;
            if (diff > 0) {
                diff = 0;
            }
        }
        /* Add half of modified difference to input */
        result = (float)lines[1][i] + .5 + diff/2;

        /* Normalize to range [0, 1.0] */
        out[0][i] = result * normVal;
    }
}
#endif

//#######################################################################################
void svuMonoImbalance(SippFilter *fptr)
{
    UInt16 *in[3];
    half *output[0];
    MonoImbalanceParam *param = (MonoImbalanceParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        in[0] = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, 0);
        in[1] = (UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
        in[2] = (UInt16 *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output[0] = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvMonoImbalance_asm(in, output, param->thrDark, param->thrBright, param->inputBits, fptr->sliceWidth);
    #else
        mvcvMonoImbalance(in, output, param->thrDark, param->thrBright, param->inputBits, fptr->sliceWidth);
    #endif
    }
}
