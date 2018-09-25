#ifndef CONVOLUTIONSEPARABLE3X3FP16TOFP16_ASM_TEST_H_
#define CONVOLUTIONSEPARABLE3X3FP16TOFP16_ASM_TEST_H_

#include "half.h"

extern unsigned int cycleCount;
void convSeparable3x3Fp16ToFp16_asm_test(half* out, half** in, half conv[2], unsigned int width);


#endif /* CONVOLUTIONSEPARABLE3X3FP16TOFP16_ASM_TEST_H_ */
