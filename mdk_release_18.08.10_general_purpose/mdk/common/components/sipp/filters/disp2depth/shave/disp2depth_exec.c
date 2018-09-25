#include "VectorTypes.h"

void flip_dsp2depth_explic_vect(uint8_t *input, half8 *output,half *LUT, uint32_t width)
{
    output += (width>>3)-1; // Right most group of pixels
    for (uint32_t px=0;px<width;px+=8)
    {
        half8 vout;
        vout[7] = LUT[*input++];
        vout[6] = LUT[*input++];
        vout[5] = LUT[*input++];
        vout[4] = LUT[*input++];
        vout[3] = LUT[*input++];
        vout[2] = LUT[*input++];
        vout[1] = LUT[*input++];
        vout[0] = LUT[*input++];
        *output-- = vout;
    }
}

void dsp2depth_explic_vect(uint8_t *input, half8 *output,half *LUT, uint32_t width)
{
    for (uint32_t px=0;px<width;px+=8)
    {
        half8 vout;
        vout[0] = LUT[*input++];
        vout[1] = LUT[*input++];
        vout[2] = LUT[*input++];
        vout[3] = LUT[*input++];
        vout[4] = LUT[*input++];
        vout[5] = LUT[*input++];
        vout[6] = LUT[*input++];
        vout[7] = LUT[*input++];
        *output++ = vout;
    }
}


void flip_mvcvDisp2depth(uint8_t *input, half8 *output,half *LUT, uint32_t width)
{
    // Assumptions:
    //   o  The output data-set pointed to by 'out' does not overlap any of the input data-sets
    //   o  The index table 'in' is a multiple of 16
    //   o  The index table 'in' is aligned on an 8-byte boundary
    //   o  The output data-set referred to by 'out' is aligned on an 8-byte boundary
    //   o  The value of 'width' is a multiple of 16
    const uchar16* __restrict__ pIndexTable = (const uchar16* __restrict__)input;
    half8* __restrict__ pOutput = (half8* __restrict__)&output[(width>>3)-1];//End of line

    for (uint32_t i = 0; i < width; i += 16u)
    {
        // Grab 16 indices at a time
        uchar16 indices = *pIndexTable++;

        // Capture 16 values from the LUT as 2 reversed vectors, and write them to the output
        *pOutput-- = (half8){LUT[indices[ 7]],
            LUT[indices[ 6]],
            LUT[indices[ 5]],
            LUT[indices[ 4]],
            LUT[indices[ 3]],
            LUT[indices[ 2]],
            LUT[indices[ 1]],
            LUT[indices[ 0]]};

        *pOutput-- = (half8){LUT[indices[15]],
            LUT[indices[14]],
            LUT[indices[13]],
            LUT[indices[12]],
            LUT[indices[11]],
            LUT[indices[10]],
            LUT[indices[ 9]],
            LUT[indices[ 8]]};
    }
}

void flip_mvcvDisp2depth16bit(uint16_t *input, half8 *output,half *LUT, uint32_t width)
{
    // Assumptions:
    //   o  The output data-set pointed to by 'out' does not overlap any of the input data-sets
    //   o  The index table 'in' is a multiple of 16
    //   o  The index table 'in' is aligned on an 8-byte boundary
    //   o  The output data-set referred to by 'out' is aligned on an 8-byte boundary
    //   o  The value of 'width' is a multiple of 16

    const ushort16* __restrict__ pIndexTable = (const ushort16* __restrict__)input;
    half8* __restrict__ pOutput = (half8* __restrict__)&output[(width>>3)-1];//End of line

    for (uint32_t i = 0; i < width; i += 16u)
    {
        // Grab 16 indices at a time
        ushort16 indices = *pIndexTable++;

        // Capture 16 values from the LUT as 2 reversed vectors, and write them to the output
        *pOutput-- = (half8){LUT[indices[ 7]],
            LUT[indices[ 6]],
            LUT[indices[ 5]],
            LUT[indices[ 4]],
            LUT[indices[ 3]],
            LUT[indices[ 2]],
            LUT[indices[ 1]],
            LUT[indices[ 0]]};

        *pOutput-- = (half8){LUT[indices[15]],
            LUT[indices[14]],
            LUT[indices[13]],
            LUT[indices[12]],
            LUT[indices[11]],
            LUT[indices[10]],
            LUT[indices[ 9]],
            LUT[indices[ 8]]};
    }
}

void mvcvDisp2depth(uint8_t *input, half8 *output,half *LUT, uint32_t width)
{
    // Assumptions:
    //   o  The output data-set pointed to by 'out' does not overlap any of the input data-sets
    //   o  The index table 'in' is a multiple of 16
    //   o  The index table 'in' is aligned on an 8-byte boundary
    //   o  The output data-set referred to by 'out' is aligned on an 8-byte boundary
    //   o  The value of 'width' is a multiple of 16

    const uchar16* __restrict__ pIndexTable = (const uchar16* __restrict__)input;
    half8* __restrict__ pOutput = (half8* __restrict__)&output[0];

    for (uint32_t i = 0; i < width; i += 16u)
    {
        // Grab 16 indices at a time
        uchar16 indices = *pIndexTable++;

        // Capture 16 values from the LUT as 2 vectors, and write them to the output
        *pOutput++ = (half8){LUT[indices[0]],
            LUT[indices[1]],
            LUT[indices[2]],
            LUT[indices[3]],
            LUT[indices[4]],
            LUT[indices[5]],
            LUT[indices[6]],
            LUT[indices[7]]};

        *pOutput++ = (half8){LUT[indices[ 8]],
            LUT[indices[ 9]],
            LUT[indices[10]],
            LUT[indices[11]],
            LUT[indices[12]],
            LUT[indices[13]],
            LUT[indices[14]],
            LUT[indices[15]]};
    }
}

void mvcvDisp2depth16bit(uint16_t *input, half8 *output,half *LUT, uint32_t width)
{
    // Assumptions:
    //   o  The output data-set pointed to by 'out' does not overlap any of the input data-sets
    //   o  The index table 'in' is a multiple of 16
    //   o  The index table 'in' is aligned on an 8-byte boundary
    //   o  The output data-set referred to by 'out' is aligned on an 8-byte boundary
    //   o  The value of 'width' is a multiple of 16

    const ushort16* __restrict__ pIndexTable = (const ushort16* __restrict__)input;
    half8* __restrict__ pOutput = (half8* __restrict__)&output[0];

    for (uint32_t i = 0; i < width; i += 16u)
    {
        // Grab 16 indices at a time
        ushort16 indices = *pIndexTable++;

        // Capture 16 values from the LUT as 2 vectors, and write them to the output
        *pOutput++ = (half8){LUT[indices[0]],
            LUT[indices[1]],
            LUT[indices[2]],
            LUT[indices[3]],
            LUT[indices[4]],
            LUT[indices[5]],
            LUT[indices[6]],
            LUT[indices[7]]};

        *pOutput++ = (half8){LUT[indices[ 8]],
            LUT[indices[ 9]],
            LUT[indices[10]],
            LUT[indices[11]],
            LUT[indices[12]],
            LUT[indices[13]],
            LUT[indices[14]],
            LUT[indices[15]]};
    }
}
