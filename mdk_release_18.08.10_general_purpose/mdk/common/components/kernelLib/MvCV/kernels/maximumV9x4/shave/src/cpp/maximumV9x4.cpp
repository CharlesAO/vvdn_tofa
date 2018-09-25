#include "maximumV9x4.h"

void mvcvMaximumV9x4(half** in, half** out, u32 width)
{
    unsigned int i;
	half max1, max2, max3, max4;

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
		max1 = in[0][i];                   max2 = in[1][i];                 
		if(in[1][i]>max1) {max1 = in[1][i];} if(in[2][i]>max2) {max2 = in[2][i];}
		if(in[2][i]>max1) {max1 = in[2][i];} if(in[3][i]>max2) {max2 = in[3][i];}
		if(in[3][i]>max1) {max1 = in[3][i];} if(in[4][i]>max2) {max2 = in[4][i];}
		if(in[4][i]>max1) {max1 = in[4][i];} if(in[5][i]>max2) {max2 = in[5][i];}
		if(in[5][i]>max1) {max1 = in[5][i];} if(in[6][i]>max2) {max2 = in[6][i];}
		if(in[6][i]>max1) {max1 = in[6][i];} if(in[7][i]>max2) {max2 = in[7][i];}
		if(in[7][i]>max1) {max1 = in[7][i];} if(in[8][i]>max2) {max2 = in[8][i];}
		if(in[8][i]>max1) {max1 = in[8][i];} if(in[9][i]>max2) {max2 = in[9][i];}
		
		max3 = in[2][i];                     max4 = in[3][i];                 
		if(in[3][i]>max3) {max3  = in[3][i];}  if(in[4][i]>max4) {max4  = in[4][i];}
		if(in[4][i]>max3) {max3  = in[4][i];}  if(in[5][i]>max4) {max4  = in[5][i];}
		if(in[5][i]>max3) {max3  = in[5][i];}  if(in[6][i]>max4) {max4  = in[6][i];}
		if(in[6][i]>max3) {max3  = in[6][i];}  if(in[7][i]>max4) {max4  = in[7][i];}
		if(in[7][i]>max3) {max3  = in[7][i];}  if(in[8][i]>max4) {max4  = in[8][i];}
		if(in[8][i]>max3) {max3  = in[8][i];}  if(in[9][i]>max4) {max4  = in[9][i];}
		if(in[9][i]>max3) {max3  = in[9][i];}  if(in[10][i]>max4) {max4 = in[10][i];}
		if(in[10][i]>max3) {max3 = in[10][i];} if(in[11][i]>max4) {max4 = in[11][i];}
				
		
		out[0][i] = max1;
		out[1][i] = max2;
		out[2][i] = max3;
		out[3][i] = max4;
		
    }
    return;
}
