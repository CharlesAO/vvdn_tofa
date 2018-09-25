#ifndef _cvtColorRGBtoUVNV12_ASM_TEST_H_
#define _cvtColorRGBtoUVNV12_TEST_H_

extern unsigned int cvtColorRGBtoChromaNV12CycleCount;
void cvtColorRGBtoChromaNV12_asm_test(unsigned char *input1RGB[3], unsigned char *input2RGB[3], unsigned char *UVout, float mat[3*3], float offset[1*3], unsigned int width);
#endif //_ccvtColorRGBtoUVNV12_ASM_TEST_H_
