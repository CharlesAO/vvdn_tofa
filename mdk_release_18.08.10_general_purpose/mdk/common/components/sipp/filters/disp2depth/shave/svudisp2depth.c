#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/disp2depth/disp2depth.h>
#include <filters/disp2depth/shave/disp2depth_exec.h>
/// disp2depth kernel

extern void flip_dsp2depth_explic_vect(uint8_t *input, half8 *output,half *LUT, uint32_t width);
extern void dsp2depth_explic_vect(uint8_t *input, half8 *output,half *LUT, uint32_t width);


#ifdef __cplusplus
extern "C" {
#endif

void svudisp2depth(SippFilter *fptr)
{
    Disp2depthParam *param = (Disp2depthParam*)fptr->params;
    UInt8 * restrict input;
    UInt16 * restrict input16bit;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt8 _16bitmode = param->_16bitmode;
    if (param->flip_disp2depth)
    {
        for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
        {
            half8 * restrict output = (half8 * restrict)getOutPtr(fptr, iterNum, 0);
            if(!_16bitmode)
            {
                input=(UInt8 * restrict )getInPtr(fptr, 0, iterNum, 0, 0);
                //flip_dsp2depth_explic_vect(input,output,param->disp2depthLUT,fptr->sliceWidth);
                flip_mvcvDisp2depth(input,output,param->disp2depthLUT,fptr->sliceWidth);
            }
            else
            {
                input16bit=(UInt16 * restrict )getInPtr(fptr, 0, iterNum, 0, 0);
                //flip_dsp2depth_explic_vect(input,output,param->disp2depthLUT,fptr->sliceWidth);
                flip_mvcvDisp2depth16bit(input16bit,output,param->disp2depthLUT,fptr->sliceWidth);
            }
        }
    }
    else
    {
        for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
        {
            half8 * restrict output = (half8 * restrict)getOutPtr(fptr, iterNum, 0);
            if(!_16bitmode)
            {
                input=(UInt8 * restrict )getInPtr(fptr, 0, iterNum, 0, 0);
                //dsp2depth_explic_vect(input,output,param->disp2depthLUT,fptr->sliceWidth);
                mvcvDisp2depth(input,output,param->disp2depthLUT,fptr->sliceWidth);
            }
            else
            {
                input16bit=(UInt16 * restrict )getInPtr(fptr, 0, iterNum, 0, 0);
                //dsp2depth_explic_vect(input,output,param->disp2depthLUT,fptr->sliceWidth);
                mvcvDisp2depth16bit(input16bit,output,param->disp2depthLUT,fptr->sliceWidth);
            }


        }

    }

}
#ifdef __cplusplus
}
#endif
