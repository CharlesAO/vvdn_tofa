#include <mv_types.h>
#include "fast9M2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define adiff(a,b) ((a)>(b)?((a)-(b)):((b)-(a)))

// local function definition
void fastExclude(u8** row, unsigned int* posValid, unsigned int *nrOfPoints, unsigned int thresh, unsigned int width);
void fastBitFlag(u8** row, unsigned int* posValid, u8* scores, u16 *cornerPositions, unsigned int thresh, unsigned int nrOfPoints);

void mvcvFast9M2(u8** row, u8* score, u16 *base, unsigned int thresh, unsigned int width)
{
    unsigned int posValid[1920];
    unsigned int nrOfPoints;
    fastExclude(row, posValid, &nrOfPoints, thresh, width);
    fastBitFlag(row, posValid, score, base, thresh, nrOfPoints);
}

void fastExclude(u8** row, unsigned int* posValid, unsigned int *nrOfPoints, unsigned int thresh, unsigned int width)
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
}

u8 satu8add(u8 a, u8 b)
{
    int sum = (int)a + (int)b;

    if (sum > 255)
        return ((u8)255);
    else
        if (sum < 0)
            return ((u8)0);
        else
            return ((u8)sum);
}

u8 satu8sub(u8 a, u8 b)
{
    int sub = (int)a - (int)b;

    if (sub > 255)
        return ((u8)255);
    else
        if (sub < 0)
            return ((u8)0);
        else
            return ((u8)sub);
}

void fastBitFlag(u8** row, unsigned int* posValid, u8* scores, u16 *cornerPositions, unsigned int thresh, unsigned int nrOfPoints)
{
    unsigned int itr;
    unsigned int counter = 0;
    u16 *position  = cornerPositions + 2;
    u8 *scoreValue = (scores + 4);
    u16 bitFlag = 0x01FF;
    unsigned int *adrForCounter1 = (unsigned int*)scores;
    unsigned int *adrForCounter2 = (unsigned int*)cornerPositions;
	for (itr = 0; itr < nrOfPoints; itr++)
	{
        int posx = posValid[itr];
		u8 origin = row[3][posx];
        u8 sample[16];
        u8 loLimit = satu8sub(origin,thresh);
		u8 hiLimit = satu8add(origin, thresh);
        unsigned int i = 0;
        u16 bitMaskLow = 0;
        u16 bitMaskHi = 0;
        int hiScore = 0;
        int loScore = 0;
        u8 hiCount = 0;
        u8 loCount = 0;

        unsigned int finalCounter = 0;
        u16 bitMask      = 0;
        u16 scoreFin     = 0;

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
            u8 localHiScore = satu8sub(sample[i], hiLimit);
            u8 localLoScore = satu8sub(loLimit,sample[i]);
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
			*scoreValue = (u8)(scoreFin >> 4);
            scoreValue++;
            counter++;
		}
     }
    *adrForCounter1 = counter;
    *adrForCounter2 = counter;
}
