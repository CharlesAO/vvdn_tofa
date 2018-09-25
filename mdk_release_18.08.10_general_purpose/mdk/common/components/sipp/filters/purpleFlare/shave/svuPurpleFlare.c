#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/purpleFlare/purpleFlare.h>

#define MIN(a,b)         ((a) < (b) ? (a) : (b))
#define MAX(a,b)         ((a) > (b) ? (a) : (b))
/*
 * The "purple flare" effect shows up most strongly on a coke can - the
 * white letters are surrounded by a purple halo (the red areas are affected).
 * One of the causes is longitudinal chromatic aberration - the blue
 * channel is out of focus with respect to the green channel, and the
 * blue spreads to the surrounding areas.  Could also happen with the
 * red channel but it hasn't been observed to cause noticeable artifacts.
 * The lighting is likely also a factor - would expect to see it with
 * "blue" light (it has been observed strongly under D50).
 * We apply a modified unsharp mask on the blue channel only to reduce it.
 */
#ifdef SIPP_USE_MVCV
void mvispPurpleFlare_asm(UInt16 *inGreen, UInt16 *blue[3], UInt16 *outBlue, UInt8 strength, UInt32 width);
#else
void purpleFlare         (UInt16 *inGreen, UInt16 *blue[3], UInt16 *outBlue, UInt8 strength, UInt32 width)
{
    unsigned i;
    int         in, bFilt, hp, out;

    for (i = 0; i < width; i++) {
    in = blue[1][i]; /* Centre pixel */

    /* Apply kernel [ 1 2 1; 2 4 2; 1 2 1] */
        bFilt = (blue[0][i-1] << 0) + (blue[0][i] << 1) + (blue[0][i+1] << 0) +
                (blue[1][i-1] << 1) + (in         << 2) + (blue[1][i+1] << 1) +
                (blue[2][i-1] << 0) + (blue[2][i] << 1) + (blue[2][i+1] << 0);

    /* High pass (blurred version subtracted from centre pixel) */
        hp = in - (bFilt >> 4);

    /* Only darken blue pixels, never brighten them */
    if (hp > 0) {
        hp = 0;
    }

    out = in + hp * strength;

    /*
    * Never darken the blue channel such that its value is less than
    * that of the green channel.  To achieve this, clamp the new blue
    * value to be between the old blue and green values.
    */
    outBlue[i] = MIN(in, MAX(inGreen[i], out));
    }
}
#endif
//##########################################################################################
void svuPurpleFlare(SippFilter *fptr)
{
    UInt16  *in[2], *out[3], *blue[3];
    UInt8   strength = ((PurpleFlareParam *)fptr->params)->strength;
    unsigned i;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        /* Red and Green input */
        in[0] = (UInt16 *)getInPtr(fptr, 0, iterNum, 1, 0);
        in[1] = (UInt16 *)getInPtr(fptr, 0, iterNum, 1, 1);

        /* Blue input (need three lines) */
        blue[0] = (UInt16 *)getInPtr(fptr, 0, iterNum, 0, 2);
        blue[1] = (UInt16 *)getInPtr(fptr, 0, iterNum, 1, 2);
        blue[2] = (UInt16 *)getInPtr(fptr, 0, iterNum, 2, 2);

        out[0]  = (UInt16 *)getOutPtr(fptr, iterNum, 0);
        out[1]  = (UInt16 *)getOutPtr(fptr, iterNum, 1);
        out[2]  = (UInt16 *)getOutPtr(fptr, iterNum, 2);

        for (i = 0; i < fptr->sliceWidth; i++)
        {
            /* Copy Red and Green planes directly to output */
            out[0][i] = in[0][i];
            out[1][i] = in[1][i];
        }

    #ifdef SIPP_USE_MVCV
        mvispPurpleFlare_asm(in[1], blue, out[2], strength, fptr->sliceWidth);
    #else
        purpleFlare(in[1], blue, out[2], strength, fptr->sliceWidth);
    #endif
    }
}
