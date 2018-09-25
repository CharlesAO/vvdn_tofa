#ifndef _warpMeshSample8bit_ASM_TEST_H_
#define _warpMeshSample8bit_ASM_TEST_H_

extern unsigned int warpMeshSample8bitCycleCount;

void warpMeshSample8bit_asm_test(unsigned char **input_tile, unsigned char * output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height);

#endif //_warpMeshSample8bit_ASM_TEST_H_
