#include "canny.h"
#include <math.h>

void mvcvCanny(u8** srcAddr, u8** dstAddr, u8* buffer,  u32 threshold1, u32 threshold2,  u32 width)
{

	//declaration for all necesary data
	u8* in_lines[9];
	u8  *in_aux1, *in_aux2, *in_aux3, *in_aux4, *in_aux5;
	u8 *gradient1, *gradient2, *gradient3;
	fp32 *tangent2;
	u8 *aux_line;
	u8* out_line;


	int i;
	float sum=0.0f;

	in_aux1   = buffer;
	in_aux2   = in_aux1 + width +8 ;
	in_aux3   = in_aux2 + width +8 ;
	in_aux4   = in_aux3 + width +8 ;
	in_aux5   = in_aux4 + width +8 ;



	gradient1   = in_aux5   + width +8 ;
	gradient2   = gradient1 + width +8 ;
	gradient3   = gradient2 + width +8 ;

	aux_line    = gradient3 + width + 8;
	tangent2    = (fp32*)(aux_line + width + 8);

	//init data for using
	fp32 sx=0, sy=0,s=0;
    out_line  = *dstAddr;
	fp32 interval1 = 0.4142135623730950488016887242097; //22.5
	fp32 interval2 = 2.4142135623730950488016887242097; //67.5
	fp32 interval3 =-2.4142135623730950488016887242097; //112.5
	fp32 interval4 =-0.4142135623730950488016887242097; //157.5

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

	//blur matrix
	int Blur[5][5]={
			{2, 4, 5, 4, 2},
			{4, 9,12, 9, 4},
			{5,12,15,12, 5},
			{4, 9,12, 9, 4},
			{2, 4, 5, 4, 2},
	};


	//init line pointers
	for(i=0;i<9;i++)
	{
		in_lines[i]=srcAddr[i] - 2;
	}




	//first step -blur
	for (i=0;i<(int)width+4;i++)
	{

		sum       = in_lines[0][i-2] * Blur[0][0] + in_lines[0][i-1] * Blur[0][1] + in_lines[0][i] * Blur[0][2] + in_lines[0][i+1] * Blur[0][3] + in_lines[0][i+2] * Blur[0][4] +
				    in_lines[1][i-2] * Blur[1][0] + in_lines[1][i-1] * Blur[1][1] + in_lines[1][i] * Blur[1][2] + in_lines[1][i+1] * Blur[1][3] + in_lines[1][i+2] * Blur[1][4] +
				    in_lines[2][i-2] * Blur[2][0] + in_lines[2][i-1] * Blur[2][1] + in_lines[2][i] * Blur[2][2] + in_lines[2][i+1] * Blur[2][3] + in_lines[2][i+2] * Blur[2][4] +
				    in_lines[3][i-2] * Blur[3][0] + in_lines[3][i-1] * Blur[3][1] + in_lines[3][i] * Blur[3][2] + in_lines[3][i+1] * Blur[3][3] + in_lines[3][i+2] * Blur[3][4] +
				    in_lines[4][i-2] * Blur[4][0] + in_lines[4][i-1] * Blur[4][1] + in_lines[4][i] * Blur[4][2] + in_lines[4][i+1] * Blur[4][3] + in_lines[4][i+2] * Blur[4][4];

		in_aux1[i] = (u8)(sum/159.0f);

		sum       = in_lines[1][i-2] * Blur[0][0] + in_lines[1][i-1] * Blur[0][1] + in_lines[1][i] * Blur[0][2] + in_lines[1][i+1] * Blur[0][3] + in_lines[1][i+2] * Blur[0][4] +
				    in_lines[2][i-2] * Blur[1][0] + in_lines[2][i-1] * Blur[1][1] + in_lines[2][i] * Blur[1][2] + in_lines[2][i+1] * Blur[1][3] + in_lines[2][i+2] * Blur[1][4] +
				    in_lines[3][i-2] * Blur[2][0] + in_lines[3][i-1] * Blur[2][1] + in_lines[3][i] * Blur[2][2] + in_lines[3][i+1] * Blur[2][3] + in_lines[3][i+2] * Blur[2][4] +
				    in_lines[4][i-2] * Blur[3][0] + in_lines[4][i-1] * Blur[3][1] + in_lines[4][i] * Blur[3][2] + in_lines[4][i+1] * Blur[3][3] + in_lines[4][i+2] * Blur[3][4] +
				    in_lines[5][i-2] * Blur[4][0] + in_lines[5][i-1] * Blur[4][1] + in_lines[5][i] * Blur[4][2] + in_lines[5][i+1] * Blur[4][3] + in_lines[5][i+2] * Blur[4][4];
		in_aux2[i] = (u8)(sum/159.0f);

		sum       = in_lines[2][i-2] * Blur[0][0] + in_lines[2][i-1] * Blur[0][1] + in_lines[2][i] * Blur[0][2] + in_lines[2][i+1] * Blur[0][3] + in_lines[2][i+2] * Blur[0][4] +
				    in_lines[3][i-2] * Blur[1][0] + in_lines[3][i-1] * Blur[1][1] + in_lines[3][i] * Blur[1][2] + in_lines[3][i+1] * Blur[1][3] + in_lines[3][i+2] * Blur[1][4] +
				    in_lines[4][i-2] * Blur[2][0] + in_lines[4][i-1] * Blur[2][1] + in_lines[4][i] * Blur[2][2] + in_lines[4][i+1] * Blur[2][3] + in_lines[4][i+2] * Blur[2][4] +
				    in_lines[5][i-2] * Blur[3][0] + in_lines[5][i-1] * Blur[3][1] + in_lines[5][i] * Blur[3][2] + in_lines[5][i+1] * Blur[3][3] + in_lines[5][i+2] * Blur[3][4] +
				    in_lines[6][i-2] * Blur[4][0] + in_lines[6][i-1] * Blur[4][1] + in_lines[6][i] * Blur[4][2] + in_lines[6][i+1] * Blur[4][3] + in_lines[6][i+2] * Blur[4][4];
		in_aux3[i] = (u8)(sum/159.0f);


		sum       = in_lines[3][i-2] * Blur[0][0] + in_lines[3][i-1] * Blur[0][1] + in_lines[3][i] * Blur[0][2] + in_lines[3][i+1] * Blur[0][3] + in_lines[3][i+2] * Blur[0][4] +
				    in_lines[4][i-2] * Blur[1][0] + in_lines[4][i-1] * Blur[1][1] + in_lines[4][i] * Blur[1][2] + in_lines[4][i+1] * Blur[1][3] + in_lines[4][i+2] * Blur[1][4] +
				    in_lines[5][i-2] * Blur[2][0] + in_lines[5][i-1] * Blur[2][1] + in_lines[5][i] * Blur[2][2] + in_lines[5][i+1] * Blur[2][3] + in_lines[5][i+2] * Blur[2][4] +
				    in_lines[6][i-2] * Blur[3][0] + in_lines[6][i-1] * Blur[3][1] + in_lines[6][i] * Blur[3][2] + in_lines[6][i+1] * Blur[3][3] + in_lines[6][i+2] * Blur[3][4] +
				    in_lines[7][i-2] * Blur[4][0] + in_lines[7][i-1] * Blur[4][1] + in_lines[7][i] * Blur[4][2] + in_lines[7][i+1] * Blur[4][3] + in_lines[7][i+2] * Blur[4][4];
		in_aux4[i] = (u8)(sum/159.0f);

		sum       = in_lines[4][i-2] * Blur[0][0] + in_lines[4][i-1] * Blur[0][1] + in_lines[4][i] * Blur[0][2] + in_lines[4][i+1] * Blur[0][3] + in_lines[4][i+2] * Blur[0][4] +
				    in_lines[5][i-2] * Blur[1][0] + in_lines[5][i-1] * Blur[1][1] + in_lines[5][i] * Blur[1][2] + in_lines[5][i+1] * Blur[1][3] + in_lines[5][i+2] * Blur[1][4] +
				    in_lines[6][i-2] * Blur[2][0] + in_lines[6][i-1] * Blur[2][1] + in_lines[6][i] * Blur[2][2] + in_lines[6][i+1] * Blur[2][3] + in_lines[6][i+2] * Blur[2][4] +
				    in_lines[7][i-2] * Blur[3][0] + in_lines[7][i-1] * Blur[3][1] + in_lines[7][i] * Blur[3][2] + in_lines[7][i+1] * Blur[3][3] + in_lines[7][i+2] * Blur[3][4] +
				    in_lines[8][i-2] * Blur[4][0] + in_lines[8][i-1] * Blur[4][1] + in_lines[8][i] * Blur[4][2] + in_lines[8][i+1] * Blur[4][3] + in_lines[8][i+2] * Blur[4][4];
		in_aux5[i] = (u8)(sum/159.0f);

	}
	in_aux1 = in_aux1 + 1;
	in_aux2 = in_aux2 + 1;
	in_aux3 = in_aux3 + 1;
	in_aux4 = in_aux4 + 1;
	in_aux5 = in_aux5 + 1;

	//second step -calculate gradient and tangent
	for(i=0;i<(int)(width+2);i++)
	{
		sx = 	 VertSobel[0][0]*in_aux1[i-1]+ VertSobel[0][1]*in_aux1[i] +VertSobel[0][2]*in_aux1[i+1]+
				 VertSobel[1][0]*in_aux2[i-1]+ VertSobel[1][1]*in_aux2[i] +VertSobel[1][2]*in_aux2[i+1]+
				 VertSobel[2][0]*in_aux3[i-1]+ VertSobel[2][1]*in_aux3[i] +VertSobel[2][2]*in_aux3[i+1];

		sy = 	 HorzSobel[0][0]*in_aux1[i-1]+ HorzSobel[0][1]*in_aux1[i] +HorzSobel[0][2]*in_aux1[i+1]+
				 HorzSobel[1][0]*in_aux2[i-1]+ HorzSobel[1][1]*in_aux2[i] +HorzSobel[1][2]*in_aux2[i+1]+
				 HorzSobel[2][0]*in_aux3[i-1]+ HorzSobel[2][1]*in_aux3[i] +HorzSobel[2][2]*in_aux3[i+1];

		s= sqrtf(sx*sx+ sy*sy);

		gradient1[i]=(u8)s;


		//**************************************//
		sx = 	 VertSobel[0][0]*in_aux2[i-1]+ VertSobel[0][1]*in_aux2[i] +VertSobel[0][2]*in_aux2[i+1]+
				 VertSobel[1][0]*in_aux3[i-1]+ VertSobel[1][1]*in_aux3[i] +VertSobel[1][2]*in_aux3[i+1]+
				 VertSobel[2][0]*in_aux4[i-1]+ VertSobel[2][1]*in_aux4[i] +VertSobel[2][2]*in_aux4[i+1];

		sy = 	 HorzSobel[0][0]*in_aux2[i-1]+ HorzSobel[0][1]*in_aux2[i] +HorzSobel[0][2]*in_aux2[i+1]+
				 HorzSobel[1][0]*in_aux3[i-1]+ HorzSobel[1][1]*in_aux3[i] +HorzSobel[1][2]*in_aux3[i+1]+
				 HorzSobel[2][0]*in_aux4[i-1]+ HorzSobel[2][1]*in_aux4[i] +HorzSobel[2][2]*in_aux4[i+1];

		s= sqrtf(sx*sx+ sy*sy);

		gradient2[i]=(u8)s;

		if (sy == 0) tangent2[i] = 2.5f;
		else tangent2[i] = sx/sy;

		//**************************************//
		sx = 	 VertSobel[0][0]*in_aux3[i-1]+ VertSobel[0][1]*in_aux3[i] +VertSobel[0][2]*in_aux3[i+1]+
				 VertSobel[1][0]*in_aux4[i-1]+ VertSobel[1][1]*in_aux4[i] +VertSobel[1][2]*in_aux4[i+1]+
				 VertSobel[2][0]*in_aux5[i-1]+ VertSobel[2][1]*in_aux5[i] +VertSobel[2][2]*in_aux5[i+1];

		sy = 	 HorzSobel[0][0]*in_aux3[i-1]+ HorzSobel[0][1]*in_aux3[i] +HorzSobel[0][2]*in_aux3[i+1]+
				 HorzSobel[1][0]*in_aux4[i-1]+ HorzSobel[1][1]*in_aux4[i] +HorzSobel[1][2]*in_aux4[i+1]+
				 HorzSobel[2][0]*in_aux5[i-1]+ HorzSobel[2][1]*in_aux5[i] +HorzSobel[2][2]*in_aux5[i+1];

		s= sqrtf(sx*sx+ sy*sy);

		gradient3[i]=(u8)s;


	}

gradient1 = gradient1 +1;
gradient2 = gradient2 +1;
gradient3 = gradient3 +1;

	//third step - Non-Maximum Surpression
	for(i=0;i<(int)width;i++)
	{
		//first line **************************************************************
		if (((tangent2[i]>=interval4)&&(tangent2[i]<=interval1)))
		{
			if (!((gradient2[i]>gradient1[i])&&(gradient2[i]>gradient3[i])))
			{
				aux_line[i]=0;
			}
			else
				aux_line[i] = gradient2[i];
		}

		if (((tangent2[i]>interval1)&&(tangent2[i]<interval2)))
		{
			if (!((gradient2[i]>gradient1[i+1])&&(gradient2[i]>gradient3[i-1])))
			{
				aux_line[i]=0;
			}
			else
				aux_line[i] = gradient2[i];
		}

		if (((tangent2[i]>=interval2)||(tangent2[i]<=interval3)))
		{
			if (!((gradient2[i]>gradient2[i+1])&&(gradient2[i]>gradient2[i-1])))
			{
				aux_line[i]=0;
			}
			else
				aux_line[i] = gradient2[i];
		}

		if (((tangent2[i]>interval3)&&(tangent2[i]<interval4)))
		{
			if(!((gradient2[i]>gradient1[i-1])&&(gradient2[i]>gradient3[i+1])))
			{
				aux_line[i]=0;
			}
			else
				aux_line[i] = gradient2[i];
		}

	}




	//forth step - Edge tracking by hysteresis
	for (i=0;i<(int)width;i++)
	{
		if (aux_line[i]>=threshold2)
		{
			out_line[i]=255;
		}
		if ((aux_line[i]<threshold2)&&(aux_line[i]>threshold1))
		{
			if ((gradient1[i-1]>=threshold2)||(gradient1[i]>=threshold2)||(gradient1[i+1]>=threshold2)||(aux_line[i-1]>=threshold2)||(aux_line[i+1]>=threshold2)||(gradient3[i-1]>=threshold2)||(gradient3[i]>=threshold2)||(gradient3[i+1]>=threshold2))
			{
				out_line[i]=255;
			}
			else
				out_line[i]=0;
		}
		if(aux_line[i]<=threshold1)
		{
			out_line[i]=0;
		}
	}

	return;
}
