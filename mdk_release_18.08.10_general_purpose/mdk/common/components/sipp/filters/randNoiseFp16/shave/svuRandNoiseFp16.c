#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/randNoiseFp16/randNoiseFp16.h>

unsigned short seedlist[12*8] = {
 48983, 45013, 19197, 10539, 13476,  3369, 31472, 22447,
 30400, 22649, 46471, 44385, 60118, 49067, 18879, 42139,
 23493, 18725, 21065, 19968, 12366, 25348, 52182, 63519,
 25325, 21617,   337, 20501, 18752, 29257,   457, 12295,
 27449,  9644, 50556, 31831, 40725, 53234, 49595, 64496,
 12042,  8516, 29204, 24711, 51586, 31526, 18924, 58697,
 23228, 25686, 44433, 44726, 58042, 22242, 50523, 41777,
  6150, 50968,  5916,   369,  2049,  4224, 55504, 58211,
 46402, 45323, 62350, 41020, 54912, 17622, 55249, 30196,
  9648, 50460,  3153, 17172, 44300,  3994, 52286, 58207,
  4872, 13075,  2892, 45101, 32394,  8084, 32543, 54467,
  8175, 25086, 16030, 36775,  7742, 29155, 41415, 32929
};

//########################################################################################################################################################
//    uint16_t lfsr = 0xACE1u;
//    unsigned bit;
//    unsigned period = 0;
//
//    do
//    {
//        /* taps: 16 14 13 11; feedback polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
//        bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
//        lfsr =  (lfsr >> 1) | (bit << 15);
//        ++period;
//    } while(lfsr != 0xACE1u);
//
//    return 0;
//unsigned short seedU16 = 0xACE1;

const unsigned int mskBit = ((1<<15) + (1<<13) + (1<<4) + (1<<0)); //0xA011; // other polinom that in examples

half normValue = (half)0.003921509; //(1/255 = 0x1C04)

//########################################################################################################################################################
#if defined(SIPP_USE_MVCV)
extern void mvispRandNoiseFp16_asm(half *output, half *input, float noiseStrength, unsigned int width);
#else
// this is one instruction in asm,
unsigned short sauOnesX16(unsigned short in)
{
    int i;
    unsigned short count = 0;
    for (i = 0; i < 16; i++)
    {
        if (((in>>i) & 0x01)==0x1)
        {
            count++;
        }
    }
    return count;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
unsigned short genRand(unsigned short seed)
{
    //unsigned bit  = ((seed >> 15) ^ (seed >> 13) ^ (seed >> 4) ^ (seed >> 0) ) & 1;
    unsigned short bit = sauOnesX16((seed & mskBit)) & 0x1;
    return  ((seed >> 1) | (bit << 15));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void randNoiseFp16(half *output, half *input, float noiseStrength, unsigned int width)
{
    unsigned int i;
    unsigned int j;
    // identify the seed list involve in current shave, in asm read trf with this information
    unsigned int shaveId = scGetShaveNumber();
    unsigned short *seed = (unsigned short *)((shaveId * 8/*val*/ * 2/*size of short*/) + ((unsigned char*)seedlist));
    //Noise
    for(i=0; i<width; i+=8)
    {
        for (j = 0; j < 8; j++)
        {
            seed[j] = genRand(seed[j]);
            signed char value = ((signed short) seed[j])>>8;
            half val = ((half)((float)value)) * normValue;
            output[i+j] = clamp01((input[i+j] + (val * (half)input[i+j] * (half)noiseStrength)));
        }
    }
    // 3 extra seed calculation calculation,
    // for similitude with asm implementation where this
    // is generated automatically for parallelism reason
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 8; j++)
        {
            seed[j] = genRand(seed[j]);
        }
    }
}
#endif

//########################################################################################################################################################
void svuGenNoiseFp16(SippFilter *fptr)
{
    RandNoiseFp16Param *param = (RandNoiseFp16Param *)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        half *in  = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        half *out = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvispRandNoiseFp16_asm(out, in, param->strength, fptr->sliceWidth);
    #else
        randNoiseFp16(out, in, param->strength, fptr->sliceWidth);
    #endif
    }
}

