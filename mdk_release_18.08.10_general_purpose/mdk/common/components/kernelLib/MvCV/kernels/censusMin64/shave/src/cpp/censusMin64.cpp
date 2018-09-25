#include "censusMin64.h"
#include <stdio.h>
#include <stdlib.h>

void mvcvCensusMin64(u8 *in, u8 *out, u8 *outm, u32 width)
{
	u32 j, disp = 64;
	u8 temp[7][16];
	u8 pos [7][16];
	for (u32 i = 0; i < width; i++)
    { // for all pixels


		for (j=0;j<16;j++)
		{
			temp[0][j] = (in[i*disp +j] <= in[i*disp +j + 16]) ?  in[i*disp +j] : in[i*disp +j + 16];
			 pos[0][j] = (in[i*disp +j] <= in[i*disp +j + 16]) ?  j : j+16;
			 
			temp[1][j] = (in[i*disp +j + 32] <= in[i*disp +j + 48]) ?  in[i*disp +j + 32] : in[i*disp +j + 48];
			 pos[1][j] = (in[i*disp +j + 32] <= in[i*disp +j + 48]) ?  j + 32 : j + 48; 	 
		}
			
		for (j=0;j<16;j++)
		{
			temp[2][j] = (temp[0][j] <= temp[1][j] ) ? temp[0][j] : temp[1][j];
			pos[2][j] =  (temp[0][j] <= temp[1][j] ) ? pos[0][j] : pos[1][j];
		}
	
		for (j=0;j<8;j++)
		{
			temp[3][j] = (temp[2][j] <= temp[2][j+8] ) ? temp[2][j] : temp[2][j+8];
			 pos[3][j] = (temp[2][j] <= temp[2][j+8] ) ? pos[2][j] : pos[2][j+8];
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
