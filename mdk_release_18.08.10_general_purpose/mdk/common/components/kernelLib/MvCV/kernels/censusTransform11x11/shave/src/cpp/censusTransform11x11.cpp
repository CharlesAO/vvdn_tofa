#include "censusTransform11x11.h"
#include <stdio.h>

#define load_ushort8(x) *((ushort8 *) x)

void mvcvCensusTransform11x11(u16 **in, u8 *out, u32 width)
{
    int bit, k;

    for(unsigned int i = 0; i < width*16; i++)
        out[i] = 0;

    for (unsigned int i = 0; i < width; i++)
    {      
        k = 0;
        bit = 0;
        for (int line = 0; line < 11; line++)
            for (int col = -5; col <= 5; col++)
                {
                    if(bit == 8)
                    {
                        bit = 0;
                        k++;     
                    }
                    out[16*i+k] *= 2;
                    if (*(in[line] + i + col) < *(in[5] + i))
                    {
                        out[16*i+k] += 1; 
                    } 

                    bit ++;
                } 
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvCensusTransform11x11_opt(unsigned short ** __restrict in, unsigned char * __restrict out, unsigned int width) {
  for (int i = 0; i < (int) width; i++) {      
    uint4 out4;
    uchar4 result4=0;
    unsigned short cmpVal = in[5][i];
    
    // line0: [0, 1, 2, 3, 4, 5, 6, 7, x, x, x]
    ushort8 in8 = load_ushort8(&in[0][i-5]);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 0);
    
    // line0: [x, x, x, x, x, x, x, x, 0, 1, 2]
    // line1: [3, 4, 5, 6, 7, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[0][i-2]), load_ushort8(&in[1][i-5]), 5, 6, 7, 8, 9, 10, 11, 12);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 1);
    
    // line1: [x, x, x, x, x, 0, 1, 2, 3, 4, 5]
    // line2: [6, 7, x, x, x, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[1][i-2]), load_ushort8(&in[2][i-5]), 2, 3, 4, 5, 6, 7, 8, 9);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 2);
    
    // line2: [x, x, 0, 1, 2, 3, 4, 5, 6, 7, x]
    in8 = load_ushort8(&in[2][i-3]);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 3);
    
    out4[0] = mvuAs_uint(result4);
            
    // line2: [x, x, x, x, x, x, x, x, x, x, 0]
    // line3: [1, 2, 3, 4, 5, 6, 7, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[2][i-2]), load_ushort8(&in[3][i-5]), 7, 8, 9, 10, 11, 12, 13, 14);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 0);
    
    // line3: [x, x, x, x, x, x, x, 0, 1, 2, 3]
    // line4: [4, 5, 6, 7, x, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[3][i-2]), load_ushort8(&in[4][i-5]), 4, 5, 6, 7, 8, 9, 10, 11);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 1);
    
    // line4: [x, x, x, x, 0, 1, 2, 3, 4, 5, 6]
    // line5: [7, x, x, x, x, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[4][i-2]), load_ushort8(&in[5][i-5]), 1, 2, 3, 4, 5, 6, 7, 8);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 2);
    
    // line5: [x, 0, 1, 2, 3, 4, 5, 6, 7, x, x]
    in8 = load_ushort8(&in[5][i-4]);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 3);
    
    out4[1] = mvuAs_uint(result4);
        
    // line5: [x, x, x, x, x, x, x, x, 0, 1, 2]
    // line6: [3, 4, 5, 6, 7, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[5][i-2]), load_ushort8(&in[6][i-5]), 6, 7, 8, 9, 10, 11, 12, 13);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 0);
    
    // line6: [x, x, x, x, x, x, 0, 1, 2, 3, 4]
    // line7: [5, 6, 7, x, x, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[6][i-2]), load_ushort8(&in[7][i-5]), 3, 4, 5, 6, 7, 8, 9, 10);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 1);
    
    // line7: [x, x, x, 0, 1, 2, 3, 4, 5, 6, 7]
    in8 = load_ushort8(&in[7][i-2]);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 2);
    
    // line8: [0, 1, 2, 3, 4, 5, 6, 7, x, x, x]
    in8 = load_ushort8(&in[8][i-5]);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 3);
    
    out4[2] = mvuAs_uint(result4);
        
    // line8: [x, x, x, x, x, x, x, x, 0, 1, 2]
    // line9: [3, 4, 5, 6, 7, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[8][i-2]), load_ushort8(&in[9][i-5]), 5, 6, 7, 8, 9, 10, 11, 12);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 0);
    
    // line9:  [x, x, x, x, x, 0, 1, 2, 3, 4, 5]
    // line10: [6, 7, x, x, x, x, x, x, x, x, x]
    in8 = __builtin_shufflevector(load_ushort8(&in[9][i-2]), load_ushort8(&in[10][i-5]), 2, 3, 4, 5, 6, 7, 8, 9);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 1);
    
    // line10: [x, x, 0, 1, 2, 3, 4, 5, 6, 7, x]
    in8 = load_ushort8(&in[10][i-3]);
    result4 = __builtin_shave_cmu_vnz_x16_rri_short8((in8 < cmpVal).s76543210, result4, 2);
    
    // line10: [x, x, x, x, x, x, x, x, x, x, 0]
    result4[3] = in[10][i+5] < cmpVal ? 1 : 0;
    
    out4[3] = mvuAs_uint(result4);

    *((uint4 *) &out[16*i]) = out4;
  }
}
#endif
