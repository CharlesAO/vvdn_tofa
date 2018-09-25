#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/fast9ScoreCv/fast9ScoreCv.h>
#include <string.h>
#include <stdlib.h>

/// Fast9 - corner detection
/// @param[in]  in_lines   - array of pointers to input lines
/// @param[out] score      - pointer to corner score buffer ;; first unsigned int element is the number of candidates,
/// @param[out] base       - pointer to corner candidates buffer ; first unsigned int element is the number of candidates, the rest are the position of coordinates
/// @param[in]  thresh     - threshold
/// @param[in]  width      - number of pixels to process

#ifdef SIPP_USE_MVCV
extern void mvcvfast9ScoreCv_asm(UInt8** row, UInt8* score, UInt16 *base, UInt32 thresh, UInt32 width, void* bulkBuff);
#endif

#define adiff(a,b) ((a)>(b)?((a)-(b)):((b)-(a)))

#ifndef SIPP_USE_MVCV
void fastExcludePos(UInt8** row, unsigned int* posValid, unsigned int *nrOfPoints, unsigned int thresh, unsigned int width);
void fastFlagBit(UInt8** row, unsigned int* posValid, UInt8* scores, UInt16 *cornerPositions, unsigned int thresh, unsigned int nrOfPoints);
void fastScore(UInt8 *score, UInt8* scoresInput, unsigned int thresh, unsigned int nrOfPoints);

void mvcvfast9ScoreCv(UInt8** row, UInt8* score, UInt16 *base, unsigned int thresh, unsigned int width, void* bulkBuff)
{
    unsigned int* posValid = (unsigned int*)bulkBuff;
    unsigned char* scoresInput =(unsigned char*) (((unsigned char*)bulkBuff) + 4 * width + 8);
    unsigned int nrOfPoints;
    fastExcludePos(row, posValid, &nrOfPoints, thresh, width);
    fastFlagBit(row, posValid, scoresInput, base, thresh, nrOfPoints);
    *((UInt32*)score) = *((UInt32*)(&base[0]));
    fastScore((score+4), scoresInput, thresh, *((UInt32*)(&base[0])));
}


void fastExcludePos(UInt8** row, unsigned int* posValid, unsigned int *nrOfPoints, unsigned int thresh, unsigned int width)
{
    int posx;
    int threshL = thresh & 0xFF;
    *nrOfPoints = 0;

    for (posx = 0; posx < (int)width; posx++)
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
    //printf("After fastExcludePos =%d\n",*nrOfPoints);
}

UInt8 satUInt8add(UInt8 a, UInt8 b)
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

UInt8 satUInt8sub(UInt8 a, UInt8 b)
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

void fastFlagBit(UInt8** row, unsigned int* posValid, UInt8* scores, UInt16 *cornerPositions, unsigned int thresh, unsigned int nrOfPoints)
{
    unsigned int itr;
    unsigned int counter = 0;
    UInt16 *position  = cornerPositions + 2;
    UInt16 bitFlag = 0x01FF;
    unsigned int *adrForCounter2 = (unsigned int*)cornerPositions;
    UInt8 *scoresOut = scores;

    for (itr = 0; itr < nrOfPoints; itr++)
    {
        int posx = posValid[itr];
        UInt8 origin = row[3][posx];
        UInt8 sample[17] = {0};
        UInt8 loLimit = satUInt8sub(origin,thresh);
        UInt8 hiLimit = satUInt8add(origin, thresh);
        unsigned int i = 0;
        UInt16 bitMaskLow = 0;
        UInt16 bitMaskHi = 0;
        UInt8 hiCount = 0;
        UInt8 loCount = 0;
        UInt8 localHiScore[16] = {0};
        UInt8 localLoScore[16] = {0};

        unsigned int finalCounter = 0;
        UInt16 bitMask      = 0;

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
            localHiScore[i] = satUInt8sub(sample[i], hiLimit);
            localLoScore[i] = satUInt8sub(loLimit,sample[i]);
            if (localHiScore[i])
            {
                bitMaskHi |= 1 << i;
                hiCount++;
            }
            if (localLoScore[i])
            {
                bitMaskLow |= (1 << i);
                loCount++;
            }

        }

        if (hiCount >loCount)
        {
            bitMask = bitMaskHi;
            memcpy(scoresOut, localHiScore, 16);
        }
        else
        {
            memcpy(scoresOut, localLoScore, 16);
            bitMask = bitMaskLow;
        }

        finalCounter = 0;
        bitFlag = 0x01FF;
        for (i = 0; i<16; i++)
        {
            if ((bitMask & bitFlag) == bitFlag)
            {

                finalCounter++;
                if(finalCounter>7)
                {
                    //printf("finalCounter=%d\n",finalCounter);
                }
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
            //printf("finalCounter=%d\n",finalCounter);
            *position = posx;
            position++;
            scoresOut += 16;
            counter++;
        }
    }
    *adrForCounter2 = counter;
}

unsigned short rightrot(unsigned short x, unsigned int n) {
    unsigned short low1 = 1;            /* 0x00..01 */
    unsigned short high1 = 0x8000;  /* 0x80..00 */
    unsigned short lowbit;

    while (n-- > 0) {        /* repeat n times */
        lowbit = x & low1;   /* save low bit */
        x = x >> 1;          /* right-shift by 1; 0 shifted into high bit */
        if (lowbit == low1)
            x = x | high1;   /* set high bit to 1 if low bit was 1 */
    }

    return x;
}

void vectorRotate(UInt8* vect, UInt32 rotVal) {
        unsigned char vectIntern[16];
        UInt32 i;
        for (i = 0; i<16; i++)
        {
            vectIntern[i] = vect[(i+rotVal)%16];
        }
        memcpy(vect, vectIntern, 16);
}

UInt8 minimumCalc(UInt8 a, UInt8 b) {
    if (a < b) return a;
    return b;
}

void vectorShift(UInt8* vect, UInt32 shiftVal) {
        unsigned char vectIntern[16];
        memset(vectIntern, 0, 16);
        UInt32 i;
        for (i = 0; i<(16-shiftVal); i++)
        {
            vectIntern[i] = vect[(i+shiftVal)];
        }
        memcpy(vect, vectIntern, 16);
}

UInt16 ones(UInt16 f) {
    UInt16 i;
    UInt16 ret = 0;
    for (i = 0; i<16; i++) {
        if ((f & (1<< i)) == (1<<i)) { ret++;}
    }
    return(ret);
}

void fastScore(UInt8 *score, UInt8* scoresInput, unsigned int thresh, unsigned int nrOfPoints) {
    unsigned int itr;
    unsigned short bitMask;
    UInt32 i;
    //printf("nrOfPoints 0x%x \n", nrOfPoints);
    for (itr = 0; itr < nrOfPoints; itr++)
    {
        //printf("\n\n\n itr 0x%x \n", itr);

        //cmu.vnz
        bitMask = 0;
        for (i = 0; i<16; i++) {
            if(scoresInput[i]) bitMask |= 1 << i;
            //printf("scoresInput 0x%x \n", scoresInput[i]);
        }
        //printf("bitMask 0x%x \n", bitMask);
        // found first valid position
        // 2 vau and after compare TRF registers will keep the mask,
        // mask have to be continue, impossible to have gap
        //VAU.AND v5, v8, v2
        //VAU.AND v6, v8, v3
        //CMU.CMVV.UInt16 v2, v5
        //CMU.CMVV.UInt16 v3, v6 || cmu.cpti i0 C_CMU0
        //                       cmu.cpti i1 C_CMU0
        // iau.and i0 EQ (0b01000100010001000100010001000100)
        // iau.and i1 EQ (0b01000100010001000100010001000100)
        //
        unsigned short finalCounter = 0;
        unsigned short bitFlag = 0x01FF;
        for (i = 0; i<16; i++)
        {

            if ((bitMask & bitFlag) == bitFlag)
            {

                finalCounter |= 1 << i;
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
        //printf("finalCounter 0x%x \n", finalCounter);

        UInt8 maxScore = 0;
        UInt8 min = 0;
        int  nrRep = ones(finalCounter);
        for(; nrRep; nrRep--) {
           // IAU.BSF Id, Is
            i = 0;
            while ((finalCounter & (1<< i)) != (1<<i)) { i++;}
            unsigned int bfs = i;

            // rotate scores to
            // vau.rol
            vectorRotate(scoresInput, bfs);
            //for (i = 0; i<16; i++) {
            //    printf("scoresInput 0x%x \n", scoresInput[i]);
            //}
            // minimum calculation 9 values
            {
                // min 1 1 instruction
                UInt8 min1_1 = minimumCalc(scoresInput[0], scoresInput[1]);
                UInt8 min1_2 = minimumCalc(scoresInput[2], scoresInput[3]);
                UInt8 min1_3 = minimumCalc(scoresInput[4], scoresInput[5]);
                UInt8 min1_4 = minimumCalc(scoresInput[6], scoresInput[7]);

                // min 2
                UInt8 min2_1 = minimumCalc(min1_1, min1_2);
                UInt8 min2_2 = minimumCalc(min1_3, min1_4);

                //min 3
                UInt8 min3_1 = minimumCalc(min2_1, min2_2);

                //min 4 final
                min = minimumCalc(min3_1, scoresInput[8]);
            }
            vectorRotate(scoresInput, 1);
            finalCounter = finalCounter >> (bfs+1);
            if(maxScore < min) maxScore = min;
        }

        scoresInput += 16;
        *score = maxScore + thresh - 1;
        score++;
    }
}
#endif

UInt8 bulkBuff[20*1920];
void svuFast9ScoreCv(SippFilter *fptr)
{
    UInt16 *outputCoords;
    UInt8 *outputScores;
    UInt8 *iline[7];
    Fast9ScoreCvParam *param = (Fast9ScoreCvParam*)fptr->params;
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

        UInt32 i;
        for(i = 0; i < 20*1920; i++)
            bulkBuff[i] = 0xff;

    #ifdef SIPP_USE_MVCV
        mvcvfast9ScoreCv_asm(iline, outputScores, outputCoords, param->threshold, param->frameSliceWidth, (void*)bulkBuff);
    #else
        mvcvfast9ScoreCv    (iline, outputScores, outputCoords, param->threshold, param->frameSliceWidth, (void*)bulkBuff);
    #endif
    }
}