#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/fast9M2/fast9M2.h>
//#include <mvcv.h>

/// Fast9 - corner detection
/// @param[in]  in_lines   - array of pointers to input lines
/// @param[out] score      - pointer to corner score buffer ;; first unsigned int element is the number of candidates,
/// @param[out] base       - pointer to corner candidates buffer ; first unsigned int element is the number of candidates, the rest are the position of coordinates
/// @param[in]  thresh     - threshold
/// @param[in]  width      - number of pixels to process

#ifdef SIPP_USE_MVCV
extern void mvcvFast9M2_asm(UInt8** row, UInt8* score, UInt32 *base, UInt32 thresh, UInt32 width);
#endif

#define adiff(a,b) ((a)>(b)?((a)-(b)):((b)-(a)))

#ifndef SIPP_USE_MVCV
void fastExclude(UInt8** row, UInt32* posValid, UInt32 *nrOfPoints, UInt32 thresh, UInt32 width)
{
    UInt32 posx;
    Int32 threshL = thresh & 0xFF;
    *nrOfPoints = 0;

    for (posx = 0; posx < width; posx++)
    {
        if (((adiff(row[3][posx], row[0][posx]) <= threshL) && (adiff(row[3][posx], row[6][posx]) <= threshL)) ||
            ((adiff(row[3][posx], row[3][posx + 3]) <= threshL) && (adiff(row[3][posx], row[3][posx - 3]) <= threshL)))
        {
            continue;
        }
        else
        {
            posValid[*nrOfPoints] = posx;
            (*nrOfPoints)++;
        }
    }
}

UInt8 satu8add(UInt8 a, UInt8 b)
{
    int sum = (int)a + (int)b;

    if (sum > 255)
        return ((UInt8)255);
    else
        if (sum < 0)
            return ((UInt8)0);
        else
            return ((UInt8)sum);
}

UInt8 satu8sub(UInt8 a, UInt8 b)
{
    int sub = (int)a - (int)b;

    if (sub > 255)
        return ((UInt8)255);
    else
        if (sub < 0)
            return ((UInt8)0);
        else
            return ((UInt8)sub);
}

void fastBitFlag(UInt8** row, UInt32* posValid, UInt8* scores, UInt16 *cornerPositions, UInt32 thresh, UInt32 nrOfPoints)
{
    UInt32 itr;
    UInt32 counter = 0;
    UInt16 *position  = cornerPositions + 2;
    UInt8 *scoreValue = (scores + 4);
    UInt16 bitFlag = 0x01FF;
    UInt32 *adrForCounter1 = (UInt32*)scores;
    UInt32 *adrForCounter2 = (UInt32*)cornerPositions;

    for (itr = 0; itr < nrOfPoints; itr++)
    {
        UInt32 posx = posValid[itr];
        UInt8 origin = row[3][posx];
        UInt8 sample[16];
        UInt8 loLimit = satu8sub(origin,thresh);
        UInt8 hiLimit = satu8add(origin, thresh);
        UInt32 i = 0;
        UInt16 bitMaskLow = 0;
        UInt16 bitMaskHi = 0;
        Int32 hiScore = 0;
        Int32 loScore = 0;
        UInt8 hiCount = 0;
        UInt8 loCount = 0;

        UInt32 finalCounter = 0;
        UInt16 bitMask      = 0;
        UInt16 scoreFin     = 0;

        sample[0] = row[0][posx - 1];
        sample[1] = row[0][posx];
        sample[2] = row[0][posx + 1];
        sample[3] = row[1][posx + 2];
        sample[4] = row[2][posx + 3];
        sample[5] = row[3][posx + 3];
        sample[6] = row[4][posx + 3];
        sample[7] = row[5][posx + 2];
        sample[8] = row[6][posx + 1];
        sample[9] = row[6][posx];
        sample[10] = row[6][posx - 1];
        sample[11] = row[5][posx - 2];
        sample[12] = row[4][posx - 3];
        sample[13] = row[3][posx - 3];
        sample[14] = row[2][posx - 3];
        sample[15] = row[1][posx - 2];

        for (i = 0; i<16; i++)
        {
            UInt8 localHiScore = satu8sub(sample[i], hiLimit);
            UInt8 localLoScore = satu8sub(loLimit,sample[i]);
            if (localHiScore)
            {
                bitMaskHi |= 1 << i;
                hiScore += localHiScore;
                hiCount++;
            }
            if (localLoScore)
            {
                bitMaskLow |= (1 << i);
                loScore += localLoScore;
                loCount++;
            }

        }

        if (hiCount >loCount)
        {
            bitMask = bitMaskHi;
            scoreFin = hiScore;
        }
        else
        {
            bitMask = bitMaskLow;
            scoreFin = loScore;
        }


        finalCounter = 0;
        bitFlag = 0x01FF;
        for (i = 0; i<16; i++)
        {

            if ((bitMask & bitFlag) == bitFlag)
            {
                finalCounter++;
            }
            if (bitFlag & 0x8000)
            {
                bitFlag = ((bitFlag & 0x7FFF) << 1) + 1;
            }
            else
            {
                bitFlag = bitFlag << 1;
            }
        }


        if (finalCounter != 0)
        {
            *position = posx;
            position++;
            *scoreValue = (UInt8)(scoreFin >> 4);
            scoreValue++;
            counter++;
        }
     }
    *adrForCounter1 = counter;
    *adrForCounter2 = counter;
}

void fast9M2(UInt8** row, UInt8* score, UInt16 *base, UInt32 thresh, UInt32 width)
{
    UInt32 posValid[1920];
    UInt32 nrOfPoints;
    fastExclude(row, posValid, &nrOfPoints, thresh, width);
    fastBitFlag(row, posValid, score, base, thresh, nrOfPoints);
}
#endif

void svuFast9M2(SippFilter *fptr)
{
    UInt16 *outputCoords;
    UInt8 *outputScores;
    UInt8 *iline[7];
    Fast9M2Param *param = (Fast9M2Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the  input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);
        iline[5]=(UInt8 *)getInPtr(fptr, 0, iterNum, 5, 0);
        iline[6]=(UInt8 *)getInPtr(fptr, 0, iterNum, 6, 0);

        //the output line
        outputCoords = (UInt16 *)getOutPtr(fptr, iterNum, 0);
        outputScores = (UInt8*)((UInt32)outputCoords + (4 + param->frameSliceWidth * 2));

        //printf("%d %x %x, %d\n", runNo, outputCoords, outputScores, param->frameSliceWidth);
    #ifdef SIPP_USE_MVCV
        mvcvFast9M2_asm(iline, outputScores, (UInt32*)outputCoords, param->threshold, param->frameSliceWidth);
    #else
        fast9M2(iline, outputScores, (UInt16*)outputCoords, param->threshold, param->frameSliceWidth);
    #endif
    }
}
