#include <math.h>
#include "cvtColorKernelChromaYUV420ToNV12.h"

//#define IMPL_REFERENCE
//#define IMPL_UNROLLED8
#define IMPL_BUILTINS

void mvcvCvtColorKernelChromaYUV420ToNV12_REF(u8* inU, u8* inV, u8* outUV, u32 width)
{
	u32 i;

	for(i=0; i < width / 2; i++)
	{
		*outUV++ = *inU++;
		*outUV++ = *inV++;
	}
}

#if defined(IMPL_REFERENCE)
void mvcvCvtColorKernelChromaYUV420ToNV12(u8* inU, u8* inV, u8* outUV, u32 width)
{
	u32 i;

	for(i=0; i < width / 2; i++)
	{
		*outUV++ = *inU++;
		*outUV++ = *inV++;
	}
}
#elif defined(IMPL_UNROLLED8)
void mvcvCvtColorKernelChromaYUV420ToNV12(u8* inU, u8* inV, u8* outUV, u32 width)
{
	u16 NoOfIterations;

	NoOfIterations = width / 2;

	while(NoOfIterations >= 8)
	{
		*outUV++ = *inU++;
		*outUV++ = *inV++;

		*outUV++ = *inU++;
		*outUV++ = *inV++;

		*outUV++ = *inU++;
		*outUV++ = *inV++;

		*outUV++ = *inU++;
		*outUV++ = *inV++;

		*outUV++ = *inU++;
		*outUV++ = *inV++;

		*outUV++ = *inU++;
		*outUV++ = *inV++;

		*outUV++ = *inU++;
		*outUV++ = *inV++;

		*outUV++ = *inU++;
		*outUV++ = *inV++;

		NoOfIterations -= 8;
	}

	while(NoOfIterations--)
	{
		*outUV++ = *inU++;
		*outUV++ = *inV++;
	}
}
#elif defined(IMPL_BUILTINS)
void mvcvCvtColorKernelChromaYUV420ToNV12(u8* inU, u8* inV, u8* outUV, u32 width)
{
	u16 NoOfIterations;

	NoOfIterations = width / 2;

	while(NoOfIterations >= 8)
	{
		*((int4*)outUV) = __builtin_shave_vau_or_32_rr( \
							__builtin_shave_cmu_vszmbyte_riiii_int4(mvuConvert_int4(*((short4*)inU)), 1, 0, 5, 0), \
							__builtin_shave_cmu_vszmbyte_riiii_int4(mvuConvert_int4(*((short4*)inV)), 0, 4, 0, 5));

		inU+=8;inV+=8;outUV+=16;
		NoOfIterations -= 8;
	}

	while(NoOfIterations--)
	{
		*outUV++ = *inU++;
		*outUV++ = *inV++;
	}
}
#else
#error "Missing definition for mvcvCvtColorKernelChromaYUV420ToNV12!"
#endif
