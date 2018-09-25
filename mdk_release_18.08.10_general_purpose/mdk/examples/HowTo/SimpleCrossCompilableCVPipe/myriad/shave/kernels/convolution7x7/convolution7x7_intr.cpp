#include "convolution7x7_intr.h"

void Convolution7x7_intr(u8** in, u8** out, half *conv, unsigned int inWidth)
{

    int i,j;
    u8* lines[7];
	u8 *output_buf = *out;
	half8 convx[49], rez[49], input[49], output;


    //Initialize lines pointers
    lines[0] = in[0]; output[0] = (half)0;
    lines[1] = in[1]; output[1] = (half)0;
    lines[2] = in[2]; output[2] = (half)0;
    lines[3] = in[3]; output[3] = (half)0;
    lines[4] = in[4]; output[4] = (half)0;
    lines[5] = in[5]; output[5] = (half)0;
    lines[6] = in[6]; output[6] = (half)0;

	for(i=0; i<49; i++)
	{
		for(j=0;j<8;j++)
		{
			convx[i][j] = conv[i];
		}
	}


    //Go on the whole line
    for (i = 0; i < (s32)inWidth; i = i+8)
	{
		output =__builtin_shave_vau_sub_f16_rr ( output, output );
		input[0][0] = (half)lines[0][i-3]; input[1][0] = (half)lines[0][i-2]; input[2][0] = (half)lines[0][i-1]; input[3][0] = (half)lines[0][i-0]; input[4][0] = (half)lines[0][i+1]; input[5][0] = (half)lines[0][i+2]; input[6][0] = (half)lines[0][i+3];
		input[0][1] = (half)lines[0][i-2]; input[1][1] = (half)lines[0][i-1]; input[2][1] = (half)lines[0][i-0]; input[3][1] = (half)lines[0][i+1];	input[4][1] = (half)lines[0][i+2]; input[5][1] = (half)lines[0][i+3]; input[6][1] = (half)lines[0][i+4];
		input[0][2] = (half)lines[0][i-1]; input[1][2] = (half)lines[0][i-0]; input[2][2] = (half)lines[0][i+1]; input[3][2] = (half)lines[0][i+2];	input[4][2] = (half)lines[0][i+3]; input[5][2] = (half)lines[0][i+4]; input[6][2] = (half)lines[0][i+5];
		input[0][3] = (half)lines[0][i-0]; input[1][3] = (half)lines[0][i+1]; input[2][3] = (half)lines[0][i+2]; input[3][3] = (half)lines[0][i+3];	input[4][3] = (half)lines[0][i+4]; input[5][3] = (half)lines[0][i+5]; input[6][3] = (half)lines[0][i+6];
		input[0][4] = (half)lines[0][i+1]; input[1][4] = (half)lines[0][i+2]; input[2][4] = (half)lines[0][i+3]; input[3][4] = (half)lines[0][i+4];	input[4][4] = (half)lines[0][i+5]; input[5][4] = (half)lines[0][i+6]; input[6][4] = (half)lines[0][i+7];
		input[0][5] = (half)lines[0][i+2]; input[1][5] = (half)lines[0][i+3]; input[2][5] = (half)lines[0][i+4]; input[3][5] = (half)lines[0][i+5];	input[4][5] = (half)lines[0][i+6]; input[5][5] = (half)lines[0][i+7]; input[6][5] = (half)lines[0][i+8];
		input[0][6] = (half)lines[0][i+3]; input[1][6] = (half)lines[0][i+4]; input[2][6] = (half)lines[0][i+5]; input[3][6] = (half)lines[0][i+6];	input[4][6] = (half)lines[0][i+7]; input[5][6] = (half)lines[0][i+8]; input[6][6] = (half)lines[0][i+9];
		input[0][7] = (half)lines[0][i+4]; input[1][7] = (half)lines[0][i+5]; input[2][7] = (half)lines[0][i+6]; input[3][7] = (half)lines[0][i+7];	input[4][7] = (half)lines[0][i+8]; input[5][7] = (half)lines[0][i+9]; input[6][7] = (half)lines[0][i+10];

		input[7][0] = (half)lines[1][i-3]; input[8][0] = (half)lines[1][i-2]; input[9][0] = (half)lines[1][i-1]; input[10][0] = (half)lines[1][i-0];    input[11][0] = (half)lines[1][i+1]; input[12][0] = (half)lines[1][i+2]; input[13][0] = (half)lines[1][i+3];
		input[7][1] = (half)lines[1][i-2]; input[8][1] = (half)lines[1][i-1]; input[9][1] = (half)lines[1][i-0]; input[10][1] = (half)lines[1][i+1];	input[11][1] = (half)lines[1][i+2]; input[12][1] = (half)lines[1][i+3]; input[13][1] = (half)lines[1][i+4];
		input[7][2] = (half)lines[1][i-1]; input[8][2] = (half)lines[1][i-0]; input[9][2] = (half)lines[1][i+1]; input[10][2] = (half)lines[1][i+2];	input[11][2] = (half)lines[1][i+3]; input[12][2] = (half)lines[1][i+4]; input[13][2] = (half)lines[1][i+5];
		input[7][3] = (half)lines[1][i-0]; input[8][3] = (half)lines[1][i+1]; input[9][3] = (half)lines[1][i+2]; input[10][3] = (half)lines[1][i+3];	input[11][3] = (half)lines[1][i+4]; input[12][3] = (half)lines[1][i+5]; input[13][3] = (half)lines[1][i+6];
		input[7][4] = (half)lines[1][i+1]; input[8][4] = (half)lines[1][i+2]; input[9][4] = (half)lines[1][i+3]; input[10][4] = (half)lines[1][i+4];	input[11][4] = (half)lines[1][i+5]; input[12][4] = (half)lines[1][i+6]; input[13][4] = (half)lines[1][i+7];
		input[7][5] = (half)lines[1][i+2]; input[8][5] = (half)lines[1][i+3]; input[9][5] = (half)lines[1][i+4]; input[10][5] = (half)lines[1][i+5];	input[11][5] = (half)lines[1][i+6]; input[12][5] = (half)lines[1][i+7]; input[13][5] = (half)lines[1][i+8];
		input[7][6] = (half)lines[1][i+3]; input[8][6] = (half)lines[1][i+4]; input[9][6] = (half)lines[1][i+5]; input[10][6] = (half)lines[1][i+6];	input[11][6] = (half)lines[1][i+7]; input[12][6] = (half)lines[1][i+8]; input[13][6] = (half)lines[1][i+9];
		input[7][7] = (half)lines[1][i+4]; input[8][7] = (half)lines[1][i+5]; input[9][7] = (half)lines[1][i+6]; input[10][7] = (half)lines[1][i+7];	input[11][7] = (half)lines[1][i+8]; input[12][7] = (half)lines[1][i+9]; input[13][7] = (half)lines[1][i+10];

		input[14][0] = (half)lines[2][i-3]; input[15][0] = (half)lines[2][i-2]; input[16][0] = (half)lines[2][i-1]; input[17][0] = (half)lines[2][i-0]; input[18][0] = (half)lines[2][i+1]; input[19][0] = (half)lines[2][i+2]; input[20][0] = (half)lines[2][i+3];
		input[14][1] = (half)lines[2][i-2]; input[15][1] = (half)lines[2][i-1]; input[16][1] = (half)lines[2][i-0]; input[17][1] = (half)lines[2][i+1];	input[18][1] = (half)lines[2][i+2]; input[19][1] = (half)lines[2][i+3]; input[20][1] = (half)lines[2][i+4];
		input[14][2] = (half)lines[2][i-1]; input[15][2] = (half)lines[2][i-0]; input[16][2] = (half)lines[2][i+1]; input[17][2] = (half)lines[2][i+2];	input[18][2] = (half)lines[2][i+3]; input[19][2] = (half)lines[2][i+4]; input[20][2] = (half)lines[2][i+5];
		input[14][3] = (half)lines[2][i-0]; input[15][3] = (half)lines[2][i+1]; input[16][3] = (half)lines[2][i+2]; input[17][3] = (half)lines[2][i+3];	input[18][3] = (half)lines[2][i+4]; input[19][3] = (half)lines[2][i+5]; input[20][3] = (half)lines[2][i+6];
		input[14][4] = (half)lines[2][i+1]; input[15][4] = (half)lines[2][i+2]; input[16][4] = (half)lines[2][i+3]; input[17][4] = (half)lines[2][i+4];	input[18][4] = (half)lines[2][i+5]; input[19][4] = (half)lines[2][i+6]; input[20][4] = (half)lines[2][i+7];
		input[14][5] = (half)lines[2][i+2]; input[15][5] = (half)lines[2][i+3]; input[16][5] = (half)lines[2][i+4]; input[17][5] = (half)lines[2][i+5];	input[18][5] = (half)lines[2][i+6]; input[19][5] = (half)lines[2][i+7]; input[20][5] = (half)lines[2][i+8];
		input[14][6] = (half)lines[2][i+3]; input[15][6] = (half)lines[2][i+4]; input[16][6] = (half)lines[2][i+5]; input[17][6] = (half)lines[2][i+6];	input[18][6] = (half)lines[2][i+7]; input[19][6] = (half)lines[2][i+8]; input[20][6] = (half)lines[2][i+9];
		input[14][7] = (half)lines[2][i+4]; input[15][7] = (half)lines[2][i+5]; input[16][7] = (half)lines[2][i+6]; input[17][7] = (half)lines[2][i+7];	input[18][7] = (half)lines[2][i+8]; input[19][7] = (half)lines[2][i+9]; input[20][7] = (half)lines[2][i+10];

		input[21][0] = (half)lines[3][i-3]; input[22][0] = (half)lines[3][i-2]; input[23][0] = (half)lines[3][i-1]; input[24][0] = (half)lines[3][i-0]; input[25][0] = (half)lines[3][i+1]; input[26][0] = (half)lines[3][i+2]; input[27][0] = (half)lines[3][i+3];
		input[21][1] = (half)lines[3][i-2]; input[22][1] = (half)lines[3][i-1]; input[23][1] = (half)lines[3][i-0]; input[24][1] = (half)lines[3][i+1];	input[25][1] = (half)lines[3][i+2]; input[26][1] = (half)lines[3][i+3]; input[27][1] = (half)lines[3][i+4];
		input[21][2] = (half)lines[3][i-1]; input[22][2] = (half)lines[3][i-0]; input[23][2] = (half)lines[3][i+1]; input[24][2] = (half)lines[3][i+2];	input[25][2] = (half)lines[3][i+3]; input[26][2] = (half)lines[3][i+4]; input[27][2] = (half)lines[3][i+5];
		input[21][3] = (half)lines[3][i-0]; input[22][3] = (half)lines[3][i+1]; input[23][3] = (half)lines[3][i+2]; input[24][3] = (half)lines[3][i+3];	input[25][3] = (half)lines[3][i+4]; input[26][3] = (half)lines[3][i+5]; input[27][3] = (half)lines[3][i+6];
		input[21][4] = (half)lines[3][i+1]; input[22][4] = (half)lines[3][i+2]; input[23][4] = (half)lines[3][i+3]; input[24][4] = (half)lines[3][i+4];	input[25][4] = (half)lines[3][i+5]; input[26][4] = (half)lines[3][i+6]; input[27][4] = (half)lines[3][i+7];
		input[21][5] = (half)lines[3][i+2]; input[22][5] = (half)lines[3][i+3]; input[23][5] = (half)lines[3][i+4]; input[24][5] = (half)lines[3][i+5];	input[25][5] = (half)lines[3][i+6]; input[26][5] = (half)lines[3][i+7]; input[27][5] = (half)lines[3][i+8];
		input[21][6] = (half)lines[3][i+3]; input[22][6] = (half)lines[3][i+4]; input[23][6] = (half)lines[3][i+5]; input[24][6] = (half)lines[3][i+6];	input[25][6] = (half)lines[3][i+7]; input[26][6] = (half)lines[3][i+8]; input[27][6] = (half)lines[3][i+9];
		input[21][7] = (half)lines[3][i+4]; input[22][7] = (half)lines[3][i+5]; input[23][7] = (half)lines[3][i+6]; input[24][7] = (half)lines[3][i+7];	input[25][7] = (half)lines[3][i+8]; input[26][7] = (half)lines[3][i+9]; input[27][7] = (half)lines[3][i+10];

		input[28][0] = (half)lines[4][i-3]; input[29][0] = (half)lines[4][i-2]; input[30][0] = (half)lines[4][i-1]; input[31][0] = (half)lines[4][i-0]; input[32][0] = (half)lines[4][i+1]; input[33][0] = (half)lines[4][i+2]; input[34][0] = (half)lines[4][i+3];
		input[28][1] = (half)lines[4][i-2]; input[29][1] = (half)lines[4][i-1]; input[30][1] = (half)lines[4][i-0]; input[31][1] = (half)lines[4][i+1];	input[32][1] = (half)lines[4][i+2]; input[33][1] = (half)lines[4][i+3]; input[34][1] = (half)lines[4][i+4];
		input[28][2] = (half)lines[4][i-1]; input[29][2] = (half)lines[4][i-0]; input[30][2] = (half)lines[4][i+1]; input[31][2] = (half)lines[4][i+2];	input[32][2] = (half)lines[4][i+3]; input[33][2] = (half)lines[4][i+4]; input[34][2] = (half)lines[4][i+5];
		input[28][3] = (half)lines[4][i-0]; input[29][3] = (half)lines[4][i+1]; input[30][3] = (half)lines[4][i+2]; input[31][3] = (half)lines[4][i+3];	input[32][3] = (half)lines[4][i+4]; input[33][3] = (half)lines[4][i+5]; input[34][3] = (half)lines[4][i+6];
		input[28][4] = (half)lines[4][i+1]; input[29][4] = (half)lines[4][i+2]; input[30][4] = (half)lines[4][i+3]; input[31][4] = (half)lines[4][i+4];	input[32][4] = (half)lines[4][i+5]; input[33][4] = (half)lines[4][i+6]; input[34][4] = (half)lines[4][i+7];
		input[28][5] = (half)lines[4][i+2]; input[29][5] = (half)lines[4][i+3]; input[30][5] = (half)lines[4][i+4]; input[31][5] = (half)lines[4][i+5];	input[32][5] = (half)lines[4][i+6]; input[33][5] = (half)lines[4][i+7]; input[34][5] = (half)lines[4][i+8];
		input[28][6] = (half)lines[4][i+3]; input[29][6] = (half)lines[4][i+4]; input[30][6] = (half)lines[4][i+5]; input[31][6] = (half)lines[4][i+6];	input[32][6] = (half)lines[4][i+7]; input[33][6] = (half)lines[4][i+8]; input[34][6] = (half)lines[4][i+9];
		input[28][7] = (half)lines[4][i+4]; input[29][7] = (half)lines[4][i+5]; input[30][7] = (half)lines[4][i+6]; input[31][7] = (half)lines[4][i+7];	input[32][7] = (half)lines[4][i+8]; input[33][7] = (half)lines[4][i+9]; input[34][7] = (half)lines[4][i+10];

		input[35][0] = (half)lines[5][i-3]; input[36][0] = (half)lines[5][i-2]; input[37][0] = (half)lines[5][i-1]; input[38][0] = (half)lines[5][i-0]; input[39][0] = (half)lines[5][i+1]; input[40][0] = (half)lines[5][i+2]; input[41][0] = (half)lines[5][i+3];
		input[35][1] = (half)lines[5][i-2]; input[36][1] = (half)lines[5][i-1]; input[37][1] = (half)lines[5][i-0]; input[38][1] = (half)lines[5][i+1];	input[39][1] = (half)lines[5][i+2]; input[40][1] = (half)lines[5][i+3]; input[41][1] = (half)lines[5][i+4];
		input[35][2] = (half)lines[5][i-1]; input[36][2] = (half)lines[5][i-0]; input[37][2] = (half)lines[5][i+1]; input[38][2] = (half)lines[5][i+2];	input[39][2] = (half)lines[5][i+3]; input[40][2] = (half)lines[5][i+4]; input[41][2] = (half)lines[5][i+5];
		input[35][3] = (half)lines[5][i-0]; input[36][3] = (half)lines[5][i+1]; input[37][3] = (half)lines[5][i+2]; input[38][3] = (half)lines[5][i+3];	input[39][3] = (half)lines[5][i+4]; input[40][3] = (half)lines[5][i+5]; input[41][3] = (half)lines[5][i+6];
		input[35][4] = (half)lines[5][i+1]; input[36][4] = (half)lines[5][i+2]; input[37][4] = (half)lines[5][i+3]; input[38][4] = (half)lines[5][i+4];	input[39][4] = (half)lines[5][i+5]; input[40][4] = (half)lines[5][i+6]; input[41][4] = (half)lines[5][i+7];
		input[35][5] = (half)lines[5][i+2]; input[36][5] = (half)lines[5][i+3]; input[37][5] = (half)lines[5][i+4]; input[38][5] = (half)lines[5][i+5];	input[39][5] = (half)lines[5][i+6]; input[40][5] = (half)lines[5][i+7]; input[41][5] = (half)lines[5][i+8];
		input[35][6] = (half)lines[5][i+3]; input[36][6] = (half)lines[5][i+4]; input[37][6] = (half)lines[5][i+5]; input[38][6] = (half)lines[5][i+6];	input[39][6] = (half)lines[5][i+7]; input[40][6] = (half)lines[5][i+8]; input[41][6] = (half)lines[5][i+9];
		input[35][7] = (half)lines[5][i+4]; input[36][7] = (half)lines[5][i+5]; input[37][7] = (half)lines[5][i+6]; input[38][7] = (half)lines[5][i+7];	input[39][7] = (half)lines[5][i+8]; input[40][7] = (half)lines[5][i+9]; input[41][7] = (half)lines[5][i+10];

		input[42][0] = (half)lines[6][i-3]; input[43][0] = (half)lines[6][i-2]; input[44][0] = (half)lines[6][i-1]; input[45][0] = (half)lines[6][i-0]; input[46][0] = (half)lines[6][i+1]; input[47][0] = (half)lines[6][i+2]; input[48][0] = (half)lines[6][i+3];
		input[42][1] = (half)lines[6][i-2]; input[43][1] = (half)lines[6][i-1]; input[44][1] = (half)lines[6][i-0]; input[45][1] = (half)lines[6][i+1];	input[46][1] = (half)lines[6][i+2]; input[47][1] = (half)lines[6][i+3]; input[48][1] = (half)lines[6][i+4];
		input[42][2] = (half)lines[6][i-1]; input[43][2] = (half)lines[6][i-0]; input[44][2] = (half)lines[6][i+1]; input[45][2] = (half)lines[6][i+2];	input[46][2] = (half)lines[6][i+3]; input[47][2] = (half)lines[6][i+4]; input[48][2] = (half)lines[6][i+5];
		input[42][3] = (half)lines[6][i-0]; input[43][3] = (half)lines[6][i+1]; input[44][3] = (half)lines[6][i+2]; input[45][3] = (half)lines[6][i+3];	input[46][3] = (half)lines[6][i+4]; input[47][3] = (half)lines[6][i+5]; input[48][3] = (half)lines[6][i+6];
		input[42][4] = (half)lines[6][i+1]; input[43][4] = (half)lines[6][i+2]; input[44][4] = (half)lines[6][i+3]; input[45][4] = (half)lines[6][i+4];	input[46][4] = (half)lines[6][i+5]; input[47][4] = (half)lines[6][i+6]; input[48][4] = (half)lines[6][i+7];
		input[42][5] = (half)lines[6][i+2]; input[43][5] = (half)lines[6][i+3]; input[44][5] = (half)lines[6][i+4]; input[45][5] = (half)lines[6][i+5];	input[46][5] = (half)lines[6][i+6]; input[47][5] = (half)lines[6][i+7]; input[48][5] = (half)lines[6][i+8];
		input[42][6] = (half)lines[6][i+3]; input[43][6] = (half)lines[6][i+4]; input[44][6] = (half)lines[6][i+5]; input[45][6] = (half)lines[6][i+6];	input[46][6] = (half)lines[6][i+7]; input[47][6] = (half)lines[6][i+8]; input[48][6] = (half)lines[6][i+9];
		input[42][7] = (half)lines[6][i+4]; input[43][7] = (half)lines[6][i+5]; input[44][7] = (half)lines[6][i+6]; input[45][7] = (half)lines[6][i+7];	input[46][7] = (half)lines[6][i+8]; input[47][7] = (half)lines[6][i+9]; input[48][7] = (half)lines[6][i+10];


		for(j=0; j<49;j++)
		{
			rez[j] = __builtin_shave_vau_mul_f16_rr (input[j], convx[j]);
		}

		for(j=0; j<49; j++)
		{
			output = __builtin_shave_vau_add_f16_rr ( rez[j], output );
		}

		for (j = 0; j< 8; j++)
		{
			*output_buf = (output[j] > (half)255) ? 255 : (u8)output[j]; 
			*output_buf = (output[j] < (half)0) ? 0 : (u8)output[j]; 
			//*output = (u8)rez1[j];
			output_buf++;
		}


    }
    return;
}
