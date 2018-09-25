#include "lutfiller.h"
#include <Fp16Convert.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

UInt8 companding_lut[96] = {
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
 40, 41, 42, 43, 44, 45, 46, 47, 49, 51,
 53, 55, 57, 59, 61, 63, 65, 67, 69, 71,
 73, 75, 77, 79, 81, 83, 85, 87, 89, 91,
 93, 95, 97, 99,101,103,105,107,109,111,
115,119,123,127,131,135,139,143,147,151,
155,159,163,167,171,175
};

float expandDisparity16Bit(UInt16 current_disp_fix, UInt8 num_fractional_bits, UInt8 max_compressed_disp, UInt8 * companding_lut){
	//Computing all necessary parameters
	UInt16 current_disp = current_disp_fix >> num_fractional_bits;

	//Expanding the integer part
	UInt16 current_disp_mapping = companding_lut[current_disp];
	UInt16 current_disp_mapping_fix = current_disp_mapping << num_fractional_bits;

	//If it's the last disparity, for sure it has no fractional part
	if(current_disp == max_compressed_disp - 1)
		return ((float)current_disp_mapping_fix);

	//Computing delta and expanding 
	UInt16 current_disp_plus_one = current_disp + 1;
	UInt16 current_disp_plus_one_mapping = companding_lut[current_disp_plus_one];
	UInt16 delta = current_disp_plus_one_mapping - current_disp_mapping;
	UInt16 a = delta >> 1;

	//Computing fractional part
	UInt16 current_disp_int_fix = current_disp << num_fractional_bits;
	UInt16 current_disp_dec_fix = current_disp_fix - current_disp_int_fix;

	//Expanding fractional part
	UInt16 fractional_part_expanded = current_disp_dec_fix << a;

	//Computing final result 
	return ((float)(current_disp_mapping_fix + fractional_part_expanded));	
}
	

void fillDisp2DepthLut(half * LUT, float focal, float baseline, UInt8 numDisp, UInt8 _16bitmode, UInt8 numFractionalBit, UInt8 companding){
	UInt16 i, endLoop;
	UInt8 divisor = pow(2, numFractionalBit);
	float tmp;
	const float constantProduct = baseline * focal;
	divisor = pow(2, numFractionalBit);

	if(_16bitmode)
		endLoop = ((numDisp-1)*divisor)+1;
	else
		endLoop = numDisp;

	for(i = 0; i < endLoop; ++i){
		if(companding){
			if(_16bitmode)
				tmp = expandDisparity16Bit(i, numFractionalBit, numDisp, &companding_lut[0]);
			else{
				tmp = expandDisparity16Bit(i << numFractionalBit, numFractionalBit, numDisp, &companding_lut[0]);
				tmp /= divisor;
			}
		}
		else
			tmp = (float)i;
		if(_16bitmode)
			tmp /= divisor;
		LUT[i] = f32Tof16(constantProduct / tmp);
	}
}

#ifdef __cplusplus
}
#endif
