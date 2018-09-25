#include "censusMatching32.h"
    
void mvcvCensusMatching32(u32 *in1, u32 *in2, u8 *out, u32 width, u32 flag)
{
    u32 resultXOR, disparities = 32;
    u8 bitCount;

    for (u32 positionL = 0; positionL < width; positionL++)
    { // for each pixels

        for (u32 indexR = 0; indexR < disparities; indexR++)
        { // for all disparities

            if (flag == 0) 
                resultXOR = *(in1 + positionL) ^ *(in2 + positionL - indexR);
            else
                resultXOR = *(in1 + positionL) ^ *(in2 + positionL + indexR);
            for(bitCount = 0; resultXOR; resultXOR >>= 1)
            {
                bitCount += resultXOR & 1;
            }

            out[positionL * disparities + indexR] = bitCount;
        } // Finish all disparities of one pixel

    } // Finish all pixels
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvCensusMatching32_opt(u32 *in1, u32 *in2, u8 *out, u32 width, u32 flag)
{
    int4 resultXor;
    u32 disparitiesShift = 5;
    u32 mask = 0x000000FF;

    for (u32 positionL = 0; positionL < width; positionL++)
    { // for each pixels

        for (u32 indexR = 0; indexR < 32; indexR+=4)
        { // for all disparities
            if (flag == 0) 
            {
            resultXor[0] = *(in1 + positionL) ^ *(in2 + positionL - indexR);
            resultXor[1] = *(in1 + positionL) ^ *(in2 + positionL - (indexR + 1));
            resultXor[2] = *(in1 + positionL) ^ *(in2 + positionL - (indexR + 2));
            resultXor[3] = *(in1 + positionL) ^ *(in2 + positionL - (indexR + 3));
            }
            else
            {
            resultXor[0] = *(in1 + positionL) ^ *(in2 + positionL + indexR);
            resultXor[1] = *(in1 + positionL) ^ *(in2 + positionL + (indexR + 1));
            resultXor[2] = *(in1 + positionL) ^ *(in2 + positionL + (indexR + 2));
            resultXor[3] = *(in1 + positionL) ^ *(in2 + positionL + (indexR + 3));    
            }
               
            u32 onesCount =  __builtin_shave_sau_onesx_x32_r(resultXor);

            out[(positionL << disparitiesShift) + indexR] = onesCount & mask;
            out[(positionL << disparitiesShift) + indexR + 1] = (onesCount >> 8) & mask;
            out[(positionL << disparitiesShift) + indexR + 2] = (onesCount >> 16) & mask;
            out[(positionL << disparitiesShift) + indexR + 3] = (onesCount >> 24) & mask;
        } // Finish all disparities of one pixel

    } // Finish all pixels
}
#endif //MOVICOMPILE_OPTIMIZED