#ifndef _lumaFusionFp16ToFp16_asm_test_H_ 
#define _lumaFusionFp16ToFp16_asm_test_H_ 
#include "half.h"

typedef struct
{
    float expRat;
    float eps;
    float lslope1;
    float lslope2;
}
LumaFusionParamT;

extern unsigned int cycleCount;
void lumaFusionFp16ToFp16_asm_test(half* inLumaShort, half* inLumaLong, half* outputAlpha, half* outputLumaFusion, unsigned int width, LumaFusionParamT *params);

#endif //_lumaFusionFp16ToFp16_asm_test_H_
