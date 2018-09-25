#include "censusMin32.h"
#include <stdio.h>
#include <stdlib.h>
    
void mvcvCensusMin32(u8 *in, u8 *out, u8 *outm, u32 width)
{
    u32 j, disp = 32;
    u8 temp[7][16];
    u8 pos [7][16];
    for (u32 i = 0; i < width; i++)
    { // for all pixels
        
        for (j=0;j<16;j++)
        {
            temp[0][j] = (in[i*disp +j] <= in[i*disp +j + 16]) ?  in[i*disp +j] : in[i*disp +j + 16];
             pos[0][j] = (in[i*disp +j] <= in[i*disp +j + 16]) ?  j : j+16; 
        }
    
        for (j=0;j<8;j++)
        {
            temp[3][j] = (temp[0][j] <= temp[0][j+8] ) ? temp[0][j] : temp[0][j+8];
             pos[3][j] = (temp[0][j] <= temp[0][j+8] ) ?  pos[0][j] :  pos[0][j+8];
        }
            
        for (j=0;j<4;j++)
        {
            temp[4][j] = (temp[3][j] <= temp[3][j+4] ) ? temp[3][j] : temp[3][j+4];
             pos[4][j] = (temp[3][j] <= temp[3][j+4] ) ? pos[3][j] : pos[3][j+4];
        }
        
        for (j=0;j<2;j++)
        {
            temp[5][j] = (temp[4][j] <= temp[4][j+2] ) ? temp[4][j] : temp[4][j+2];
             pos[5][j] = (temp[4][j] <= temp[4][j+2] ) ? pos[4][j] : pos[4][j+2];
        }
        
            temp[6][0] = (temp[5][1] < temp[5][0] ) ? temp[5][1] : temp[5][0];
             pos[6][0] = (temp[5][1] < temp[5][0] ) ? pos[5][1] : pos[5][0];
        // Store the position of min
        out[i] = pos[6][0];
        outm[i] = temp[6][0];
    }
}
