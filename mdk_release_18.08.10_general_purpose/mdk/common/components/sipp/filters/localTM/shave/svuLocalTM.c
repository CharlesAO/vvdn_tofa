#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/localTM/localTM.h>

#if defined(LTM_USE_LOCAL_DATA)
UInt16 slcLocalcurves [16*10] ALIGNED(8);
#endif

//###################################################################################
#if defined(SIPP_USE_MVCV)
void mvispLocalTM_asm(half** luma_in, UInt8** bg8, half** output, half curves[160], UInt32 width, UInt32 run_no);
#else
void localTM         (half** luma_in, UInt8** bg8, half** output, half curves[160], UInt32 width, UInt32 run_no)
{
    half *luma, *out;
    half bg, csel, psel;
    half alphaCol, alphaRow;

    unsigned i, wi;
    unsigned bgidx;
    int csel1, csel2, psel1, psel2;
    static half wt[4][4] = {
        { 3.0/4096, 9.0/4096, 1.0/4096, 3.0/4096 },
        { 9.0/4096, 3.0/4096, 3.0/4096, 1.0/4096 },
        { 1.0/4096, 3.0/4096, 3.0/4096, 9.0/4096 },
        { 3.0/4096, 1.0/4096, 9.0/4096, 3.0/4096 },
    };

    luma = *luma_in;
    out  = *output;

    bgidx = -1;
    for (i = 0; i < width; i++) {
        /*
        * Bilinear upsample of "background" input, and conversion
        * from U8f to FP16.
        */
        wi = (run_no & 1) << 1 | (i & 1);
        bg = (half)(float)bg8[0][bgidx]   * wt[wi][0] +
            (half)(float)bg8[0][bgidx+1] * wt[wi][1] +
            (half)(float)bg8[1][bgidx]   * wt[wi][2] +
            (half)(float)bg8[1][bgidx+1] * wt[wi][3];

        /* Pick two curves - two adjacent columns */
        csel = bg*8;
        csel1 = csel;
        alphaCol = csel - csel1;
        csel2 = csel1+1;

        /*
        * Pick points in curves to interpolate between (row indices).
        *
        * Note: It's important that "luma" is >= 0.  Otherwise we
        * could exceed the bounds of the array.  Might want to
        * explicitly clamp "luma" to 0, but it isn't needed if the
        * input is guaranteed to be >= 0.
        */
        psel = luma[i]*14;
        psel1 = psel;
        alphaRow = psel - psel1;
        psel2 = psel1+1;


        out[i] = curves[psel1 * 10 + csel1] * (1-alphaRow) * (1-alphaCol) +
            curves[psel1 * 10 + csel2] * (1-alphaRow) * alphaCol +
            curves[psel2 * 10 + csel1] * alphaRow * (1-alphaCol) +
            curves[psel2 * 10 + csel2] * alphaRow * alphaCol;

        if ((i & 1) == 0) {
            bgidx++;
        }
    }
}
#endif

//###################################################################################
void svuLocalTM(SippFilter *fptr)
{
    half *luma;
    UInt8   *bg8[2];
    half *out;
    LocalTMParam *param = (LocalTMParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 lpi              = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo            = fptr->exeNo * lpi;
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        //the 3 input lines
        luma   = (half  *)getInPtr(fptr, 0, iterNum, 0, 0);
        bg8[0] = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);
        bg8[1] = (UInt8 *)getInPtr(fptr, 1, iterNum, 1, 0);

    #if defined(LTM_USE_LOCAL_DATA)
        //Note: application needs to initialize these luts before use !
        param->curves = slcLocalcurves;
    #endif

        //the output line
        out    = (half  *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvispLocalTM_asm(&luma, (UInt8 **)bg8, &out, (half*)param->curves, fptr->sliceWidth, runNo);
    #else
        localTM         (&luma, (UInt8 **)bg8, &out, (half*)param->curves, fptr->sliceWidth, runNo);
    #endif
    }
}