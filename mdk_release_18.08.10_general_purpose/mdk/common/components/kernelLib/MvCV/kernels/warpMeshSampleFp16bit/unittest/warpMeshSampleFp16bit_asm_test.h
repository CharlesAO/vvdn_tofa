#ifndef _warpMeshSampleFp16bit_ASM_TEST_H_
#define _warpMeshSampleFp16bit_ASM_TEST_H_

extern unsigned int warpMeshSampleFp16bitCycleCount;

void warpMeshSampleFp16bit_asm_test(half **input_tile, half *output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height);

#endif //_warpMeshSampleFp16bit_ASM_TEST_H_
