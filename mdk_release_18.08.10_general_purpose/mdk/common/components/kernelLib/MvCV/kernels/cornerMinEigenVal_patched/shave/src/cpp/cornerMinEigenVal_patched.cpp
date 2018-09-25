#include "cornerMinEigenVal_patched.h"

void mvcvCornerMinEigenVal_patched(u8 **in_lines, int posx, u8 *out_pix, u32 width)
{
	 fp32 dx1[5], dx2[5], dx3[5];
	 fp32 dy1[5], dy2[5], dy3[5];
	 fp32 dxx1[5], dxx2[5], dxx3[5];
	 fp32 dxy1[5], dxy2[5], dxy3[5];
	 fp32 dyy1[5], dyy2[5], dyy3[5];
	 fp32 a, b, c, xu, x, y, number;
	 int i, j;
     width+=1;//remove warning, present to not change the API
		//sobel matrix
		fp32 VertSobel[3][3]={
				{1, 0, -1},
				{2, 0, -2},
				{1, 0, -1}
		};

		fp32 HorzSobel[3][3]={
				{ 1,  2,  1},
				{ 0,  0,  0},
				{-1, -2, -1}
		};

		j=0;
		for (i=posx-1; i<posx + 2; i++)
		{
			dx1[j] =    VertSobel[0][0]*in_lines[0][i-1]+ VertSobel[0][1]*in_lines[0][i-0] +VertSobel[0][2]*in_lines[0][i+1]+
						VertSobel[1][0]*in_lines[1][i-1]+ VertSobel[1][1]*in_lines[1][i-0] +VertSobel[1][2]*in_lines[1][i+1]+
						VertSobel[2][0]*in_lines[2][i-1]+ VertSobel[2][1]*in_lines[2][i-0] +VertSobel[2][2]*in_lines[2][i+1];

			dy1[j] = 	HorzSobel[0][0]*in_lines[0][i-1]+ HorzSobel[0][1]*in_lines[0][i-0] +HorzSobel[0][2]*in_lines[0][i+1]+
					    HorzSobel[1][0]*in_lines[1][i-1]+ HorzSobel[1][1]*in_lines[1][i-0] +HorzSobel[1][2]*in_lines[1][i+1]+
					    HorzSobel[2][0]*in_lines[2][i-1]+ HorzSobel[2][1]*in_lines[2][i-0] +HorzSobel[2][2]*in_lines[2][i+1];

			dx2[j] =    VertSobel[0][0]*in_lines[1][i-1]+ VertSobel[0][1]*in_lines[1][i-0] +VertSobel[0][2]*in_lines[1][i+1]+
						VertSobel[1][0]*in_lines[2][i-1]+ VertSobel[1][1]*in_lines[2][i-0] +VertSobel[1][2]*in_lines[2][i+1]+
						VertSobel[2][0]*in_lines[3][i-1]+ VertSobel[2][1]*in_lines[3][i-0] +VertSobel[2][2]*in_lines[3][i+1];

			dy2[j] = 	HorzSobel[0][0]*in_lines[1][i-1]+ HorzSobel[0][1]*in_lines[1][i-0] +HorzSobel[0][2]*in_lines[1][i+1]+
					    HorzSobel[1][0]*in_lines[2][i-1]+ HorzSobel[1][1]*in_lines[2][i-0] +HorzSobel[1][2]*in_lines[2][i+1]+
					    HorzSobel[2][0]*in_lines[3][i-1]+ HorzSobel[2][1]*in_lines[3][i-0] +HorzSobel[2][2]*in_lines[3][i+1];

			dx3[j] =    VertSobel[0][0]*in_lines[2][i-1]+ VertSobel[0][1]*in_lines[2][i-0] +VertSobel[0][2]*in_lines[2][i+1]+
						VertSobel[1][0]*in_lines[3][i-1]+ VertSobel[1][1]*in_lines[3][i-0] +VertSobel[1][2]*in_lines[3][i+1]+
						VertSobel[2][0]*in_lines[4][i-1]+ VertSobel[2][1]*in_lines[4][i-0] +VertSobel[2][2]*in_lines[4][i+1];

			dy3[j] = 	HorzSobel[0][0]*in_lines[2][i-1]+ HorzSobel[0][1]*in_lines[2][i-0] +HorzSobel[0][2]*in_lines[2][i+1]+
					    HorzSobel[1][0]*in_lines[3][i-1]+ HorzSobel[1][1]*in_lines[3][i-0] +HorzSobel[1][2]*in_lines[3][i+1]+
					    HorzSobel[2][0]*in_lines[4][i-1]+ HorzSobel[2][1]*in_lines[4][i-0] +HorzSobel[2][2]*in_lines[4][i+1];
			dxx1[j] = dx1[j]*dx1[j];
			dxx2[j] = dx2[j]*dx2[j];
			dxx3[j] = dx3[j]*dx3[j];

			dyy1[j] = dy1[j]*dy1[j];
			dyy2[j] = dy2[j]*dy2[j];
			dyy3[j] = dy3[j]*dy3[j];


			dxy1[j] = dx1[j]* dy1[j];
			dxy2[j] = dx2[j]* dy2[j];
			dxy3[j] = dx3[j]* dy3[j];
			j++;
		}
	
		i = 1;
		a    = ((dxx1[i-1] + dxx1[i+0] + dxx1[i+1] +
			     dxx2[i-1] + dxx2[i+0] + dxx2[i+1] +
			     dxx3[i-1] + dxx3[i+0] + dxx3[i+1])/9.0f) * 0.5f;
	    b    =  (dxy1[i-1] + dxy1[i+0] + dxy1[i+1] +
			     dxy2[i-1] + dxy2[i+0] + dxy2[i+1] +
			     dxy3[i-1] + dxy3[i+0] + dxy3[i+1])/9.0f;
	    c    = ((dyy1[i-1] + dyy1[i+0] + dyy1[i+1] +
			     dyy2[i-1] + dyy2[i+0] + dyy2[i+1] +
			     dyy3[i-1] + dyy3[i+0] + dyy3[i+1])/9.0f)* 0.5f;
	    number = (a - c)*(a - c) + b*b;
		x = number * 0.5F;
		y  = number;
		j  = * ( u32 * ) &y;
		j  = 0x5f3759df - ( j >> 1 );
		y  = * ( float * ) &j;
		y  = y * ( 1.5 - ( x * y * y ) );
		y  = y * ( 1.5 - ( x * y * y ) );
		number =  number * y;
		xu = (a + c) - number;
		xu = xu > 255 ? 255 : xu;
		*out_pix = (u8)(xu);

	return;
}
