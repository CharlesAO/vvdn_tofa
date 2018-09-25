#ifndef VECTORFUNCTIONS_H_
#define VECTORFUNCTIONS_H_
#include <mv_types.h>
#include "VectorTypes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

float4x4 mvuTransposeFloat4x4(float4x4 in);
int4x4 mvuTransposeInt4x4(int4x4 in);
uint4x4 mvuTransposeUInt4x4(uint4x4 in);
long4x4 mvuTransposeLong4x4(long4x4 in);
ulong4x4 mvuTransposeULong4x4(ulong4x4 in);

short8x8 mvuTransposeShort8x8(short8x8 in);
ushort8x8 mvuTransposeUShort8x8(ushort8x8 in);
half8x8 mvuTransposeHalf8x8(half8x8 in);

float4x4 mvuRotate_leftFloat4x4(float4x4 in);
int4x4 mvuRotate_leftInt4x4(int4x4 in);
uint4x4 mvuRotate_leftUInt4x4(uint4x4 in);
long4x4 mvuRotate_leftLong4x4(long4x4 in);
ulong4x4 mvuRotate_leftULong4x4(ulong4x4 in);

float4x4 mvuRotate_rightFloat4x4(float4x4 in);
int4x4 mvuRotate_rightInt4x4(int4x4 in);
uint4x4 mvuRotate_rightUInt4x4(uint4x4 in);
long4x4 mvuRotate_rightLong4x4(long4x4 in);
ulong4x4 mvuRotate_rightULong4x4(ulong4x4 in);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* VECTORFUNCTIONS_H_ */