#ifndef _MONOIMBALANCE_ASM_TEST_H_
#define _MONOIMBALANCE_ASM_TEST_H_

#include "half.h"

extern unsigned int monoImbalanceCycleCount;
void MonoImbalance_asm_test(u16** in, half** out,int thrDark, int thrBright, int inputBits, u32 inWidth);

#endif //_MONOIMBALANCE_ASM_TEST_H_
