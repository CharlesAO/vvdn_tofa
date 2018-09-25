#include "convolution3x3_intr.h"
#include <stdio.h>
#include <stdlib.h>

/// Convolution 3x3 kernel
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- array of pointers to output lines
/// @param[in] conv      	- array of values from convolution
/// @param[in] inWidth	- width of input line
void Convolution3x3_intr(u8** in, u8** out, half conv[9], u32 inWidth)
{
    int i,j;
    u8* lines[3];
	u8 *output = *out;
	
	half8 in1, in2, in3, in4, in5, in6, in7, in8, in9;
	half8 rez1, rez2, rez3, rez4, rez5, rez6, rez7, rez8, rez9;
	half8 conv1, conv2, conv3, conv4, conv5, conv6, conv7, conv8, conv9; 

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

	conv1[0] = conv[0];  conv2[0] = conv[1]; conv3[0] = conv[2]; conv4[0] = conv[3]; conv5[0] = conv[4]; conv6[0] = conv[5]; conv7[0] = conv[6]; conv8[0] = conv[7]; conv9[0] = conv[8];
	conv1[1] = conv[0];	 conv2[1] = conv[1]; conv3[1] = conv[2]; conv4[1] = conv[3]; conv5[1] = conv[4]; conv6[1] = conv[5]; conv7[1] = conv[6]; conv8[1] = conv[7]; conv9[1] = conv[8];
	conv1[2] = conv[0];	 conv2[2] = conv[1]; conv3[2] = conv[2]; conv4[2] = conv[3]; conv5[2] = conv[4]; conv6[2] = conv[5]; conv7[2] = conv[6]; conv8[2] = conv[7]; conv9[2] = conv[8];
	conv1[3] = conv[0];	 conv2[3] = conv[1]; conv3[3] = conv[2]; conv4[3] = conv[3]; conv5[3] = conv[4]; conv6[3] = conv[5]; conv7[3] = conv[6]; conv8[3] = conv[7]; conv9[3] = conv[8];
	conv1[4] = conv[0];	 conv2[4] = conv[1]; conv3[4] = conv[2]; conv4[4] = conv[3]; conv5[4] = conv[4]; conv6[4] = conv[5]; conv7[4] = conv[6]; conv8[4] = conv[7]; conv9[4] = conv[8];
	conv1[5] = conv[0];	 conv2[5] = conv[1]; conv3[5] = conv[2]; conv4[5] = conv[3]; conv5[5] = conv[4]; conv6[5] = conv[5]; conv7[5] = conv[6]; conv8[5] = conv[7]; conv9[5] = conv[8];
	conv1[6] = conv[0];	 conv2[6] = conv[1]; conv3[6] = conv[2]; conv4[6] = conv[3]; conv5[6] = conv[4]; conv6[6] = conv[5]; conv7[6] = conv[6]; conv8[6] = conv[7]; conv9[6] = conv[8];
	conv1[7] = conv[0];	 conv2[7] = conv[1]; conv3[7] = conv[2]; conv4[7] = conv[3]; conv5[7] = conv[4]; conv6[7] = conv[5]; conv7[7] = conv[6]; conv8[7] = conv[7]; conv9[7] = conv[8];





    //Go on the whole line
    for (i = 0; i < (s32)inWidth; i = i +8){
		in1[0] = (half)lines[0][i-1];  in2[0] = (half)lines[0][i+0]; in3[0] = (half)lines[0][i+1];
		in1[1] = (half)lines[0][i-0];  in2[1] = (half)lines[0][i+1]; in3[1] = (half)lines[0][i+2];
		in1[2] = (half)lines[0][i+1];  in2[2] = (half)lines[0][i+2]; in3[2] = (half)lines[0][i+3];
		in1[3] = (half)lines[0][i+2];  in2[3] = (half)lines[0][i+3]; in3[3] = (half)lines[0][i+4];
		in1[4] = (half)lines[0][i+3];  in2[4] = (half)lines[0][i+4]; in3[4] = (half)lines[0][i+5];
		in1[5] = (half)lines[0][i+4];  in2[5] = (half)lines[0][i+5]; in3[5] = (half)lines[0][i+6];
		in1[6] = (half)lines[0][i+5];  in2[6] = (half)lines[0][i+6]; in3[6] = (half)lines[0][i+7];
		in1[7] = (half)lines[0][i+6];  in2[7] = (half)lines[0][i+7]; in3[7] = (half)lines[0][i+8];

		in4[0] = (half)lines[1][i-1];  in5[0] = (half)lines[1][i+0]; in6[0] = (half)lines[1][i+1];
		in4[1] = (half)lines[1][i-0];  in5[1] = (half)lines[1][i+1]; in6[1] = (half)lines[1][i+2];
		in4[2] = (half)lines[1][i+1];  in5[2] = (half)lines[1][i+2]; in6[2] = (half)lines[1][i+3];
		in4[3] = (half)lines[1][i+2];  in5[3] = (half)lines[1][i+3]; in6[3] = (half)lines[1][i+4];
		in4[4] = (half)lines[1][i+3];  in5[4] = (half)lines[1][i+4]; in6[4] = (half)lines[1][i+5];
		in4[5] = (half)lines[1][i+4];  in5[5] = (half)lines[1][i+5]; in6[5] = (half)lines[1][i+6];
		in4[6] = (half)lines[1][i+5];  in5[6] = (half)lines[1][i+6]; in6[6] = (half)lines[1][i+7];
		in4[7] = (half)lines[1][i+6];  in5[7] = (half)lines[1][i+7]; in6[7] = (half)lines[1][i+8];
		
		in7[0] = (half)lines[2][i-1];  in8[0] = (half)lines[2][i+0]; in9[0] = (half)lines[2][i+1];
		in7[1] = (half)lines[2][i-0];  in8[1] = (half)lines[2][i+1]; in9[1] = (half)lines[2][i+2];
		in7[2] = (half)lines[2][i+1];  in8[2] = (half)lines[2][i+2]; in9[2] = (half)lines[2][i+3];
		in7[3] = (half)lines[2][i+2];  in8[3] = (half)lines[2][i+3]; in9[3] = (half)lines[2][i+4];
		in7[4] = (half)lines[2][i+3];  in8[4] = (half)lines[2][i+4]; in9[4] = (half)lines[2][i+5];
		in7[5] = (half)lines[2][i+4];  in8[5] = (half)lines[2][i+5]; in9[5] = (half)lines[2][i+6];
		in7[6] = (half)lines[2][i+5];  in8[6] = (half)lines[2][i+6]; in9[6] = (half)lines[2][i+7];
		in7[7] = (half)lines[2][i+6];  in8[7] = (half)lines[2][i+7]; in9[7] = (half)lines[2][i+8];
		



		rez1 = __builtin_shave_vau_mul_f16_rr (in1, conv1);
		rez2 = __builtin_shave_vau_mul_f16_rr (in2, conv2);
		rez3 = __builtin_shave_vau_mul_f16_rr (in3, conv3);
		rez4 = __builtin_shave_vau_mul_f16_rr (in4, conv4);
		rez5 = __builtin_shave_vau_mul_f16_rr (in5, conv5);
		rez6 = __builtin_shave_vau_mul_f16_rr (in6, conv6);
		rez7 = __builtin_shave_vau_mul_f16_rr (in7, conv7);
		rez8 = __builtin_shave_vau_mul_f16_rr (in8, conv8);
		rez9 = __builtin_shave_vau_mul_f16_rr (in9, conv9);

		rez2 = 	__builtin_shave_vau_add_f16_rr ( rez1, rez2 );
		rez4 = 	__builtin_shave_vau_add_f16_rr ( rez3, rez4 );
		rez6 = 	__builtin_shave_vau_add_f16_rr ( rez5, rez6 );
		rez8 = 	__builtin_shave_vau_add_f16_rr ( rez7, rez8 );

		rez4 = 	__builtin_shave_vau_add_f16_rr ( rez2, rez4 );
		rez8 = 	__builtin_shave_vau_add_f16_rr ( rez6, rez8 );

		rez8 = 	__builtin_shave_vau_add_f16_rr ( rez4, rez8 );
		rez9 = 	__builtin_shave_vau_add_f16_rr ( rez8, rez9 );

		for (j = 0; j< 8; j++)
		{
			*output = (rez9[j] > (half)255) ? 255 : (u8)rez9[j]; 
			*output = (rez9[j] < (half)0) ? 0 : (u8)rez9[j]; 
			//*output = (u8)rez1[j];
			output++;
		}

    }
    return;
}
