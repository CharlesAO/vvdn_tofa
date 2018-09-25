#ifndef _LOOKUPTABLE_ASM_TEST_H_
#define _LOOKUPTABLE_ASM_TEST_H_

extern unsigned int LookupTableCycleCount;

void LUT8to10_asm_test(unsigned char** src, unsigned short** dest, unsigned short* lut, unsigned int width);

#endif //_LOOKUPTABLE_ASM_TEST_H_
