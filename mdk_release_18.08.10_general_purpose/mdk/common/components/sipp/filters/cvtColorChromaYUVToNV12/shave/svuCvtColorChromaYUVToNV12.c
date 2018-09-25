#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorChromaYUVToNV12/cvtColorChromaYUVToNV12.h>

//#######################################################################################
/// cvtColorKernel conversion YUV420p to NV12 chroma part only
/// @param[in]  inU     - line from input U plane
/// @param[in]  inV     - line from input V plane
/// @param[Out] outUV   - UV plane in the first of the line the output NV12 image
/// @param[in]  width   - line width in pixels
/// @return     Nothing

#ifdef SIPP_USE_MVCV
void mvcvCvtColorKernelChromaYUV420ToNV12_asm(u8* inU, u8* inV, u8* outUV, u32 width);
#else
void cvtColorChromaYUV420ToNV12(u8* inU, u8* inV, u8* outUV, u32 width)
{
	u32 i;

	for(i=0; i < width / 2; i++)
	{
		*outUV++ = *inU++;
		*outUV++ = *inV++;
	}
}
#endif

//#######################################################################################
/// cvtColorKernel conversion YUV444p to NV12 chroma part only
/// @param[in]  inU[2]  - 2 lines from input U plane
/// @param[in]  inV[2]  - 2 lines from input V plane
/// @param[Out] outUV   - UV plane in the first of the line the output NV12 image
/// @param[in]  width   - line width in pixels
/// @return     Nothing

#ifdef SIPP_USE_MVCV
void mvcvCvtColorKernelChromaYUV444ToNV12_asm(u8* inU[2], u8* inV[2], u8* outUV, u32 width);
#else

void cvtColorChromaYUV444ToNV12(u8* inU[2], u8* inV[2], u8* outUV, u32 width)
{
	u32 i;

	for(i=0; i < width/2*2; i+=2)
	{
		*outUV++ = (u8)((((inU[0][i] 	+ inU[1][i] 	+ 1) >> 1) + \
				         ((inU[0][i+1] 	+ inU[1][i+1] 	+ 1) >> 1) + \
						 1) >> 1);
		*outUV++ = (u8)((((inV[0][i] 	+ inV[1][i] 	+ 1) >> 1) + \
				         ((inV[0][i+1] 	+ inV[1][i+1] 	+ 1) >> 1) + \
						 1) >> 1);
	}
}
#endif

//#######################################################################################
void svuCvtColorChromaYUVToNV12(SippFilter *fptr)
{
    UInt8 *outUV;
    CvtColorChromaYUVToNV12Param *param = (CvtColorChromaYUVToNV12Param*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    switch(param->inputFrameType)
    {
    case YUV420p:
    {
        UInt8 *inU;
        UInt8 *inV;

		for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
		{
			inU = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
			if(param->needs2Parents)
				inV = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);
			else
				inV = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);

			outUV = (UInt8 *)getOutPtr(fptr, iterNum, 0);

		#ifdef SIPP_USE_MVCV
			mvcvCvtColorKernelChromaYUV420ToNV12_asm(inU, inV, outUV, fptr->sliceWidth);
		#else
			cvtColorChromaYUV420ToNV12(inU, inV, outUV, fptr->sliceWidth);
		#endif
		}
		break;
    }
    case YUV444p:
    {
        UInt8 *inU[2];
        UInt8 *inV[2];

		for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
		{
			inU[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
			inU[1] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
			if(param->needs2Parents)
			{
	            inV[0] = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);
	            inV[1] = (UInt8 *)getInPtr(fptr, 1, iterNum, 1, 0);
			} else
			{
	            inV[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 1);
	            inV[1] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 1);
			}

			outUV = (UInt8 *)getOutPtr(fptr, iterNum, 0);

		#ifdef SIPP_USE_MVCV
			mvcvCvtColorKernelChromaYUV444ToNV12_asm(inU, inV, outUV, fptr->sliceWidth);
		#else
			cvtColorChromaYUV444ToNV12(inU, inV, outUV, fptr->sliceWidth);
		#endif
		}
		break;
    }
    default: //other formats not supported
    	break;

    }
}
