#include <math.h>
#include "sobel.h"

void mvcvSobel(u8** in, u8** out, u32 width)
{
    int i;
    u8* lines[5];
	fp32 sx=0, sy=0,s=0;
    unsigned char* aux;
    aux=(*out);

	//sobel matrix
	int VertSobel[3][3]={
			{-1, 0, 1},
			{-2, 0, 2},
			{-1, 0, 1}
	};

	int HorzSobel[3][3]={
			{-1, -2, -1},
			{ 0,  0,  0},
			{ 1,  2,  1}
	};


    //Initialize lines pointers
    lines[0]=*in;
    lines[1]=*(in+1);
    lines[2]=*(in+2);


    //Go on the whole line
    for (i=0;i<(int)width;i++){
		sx = 	 VertSobel[0][0]*lines[0][i-1]+ VertSobel[0][1]*lines[0][i] +VertSobel[0][2]*lines[0][i+1]+
				 VertSobel[1][0]*lines[1][i-1]+ VertSobel[1][1]*lines[1][i] +VertSobel[1][2]*lines[1][i+1]+
				 VertSobel[2][0]*lines[2][i-1]+ VertSobel[2][1]*lines[2][i] +VertSobel[2][2]*lines[2][i+1];

		sy = 	 HorzSobel[0][0]*lines[0][i-1]+ HorzSobel[0][1]*lines[0][i] +HorzSobel[0][2]*lines[0][i+1]+
				 HorzSobel[1][0]*lines[1][i-1]+ HorzSobel[1][1]*lines[1][i] +HorzSobel[1][2]*lines[1][i+1]+
				 HorzSobel[2][0]*lines[2][i-1]+ HorzSobel[2][1]*lines[2][i] +HorzSobel[2][2]*lines[2][i+1];
		s= sqrtf(sx*sx+ sy*sy);
		if (s>255)
			aux[i]=255;
		else
			aux[i] = (u8)s;
    }
    return;
}
