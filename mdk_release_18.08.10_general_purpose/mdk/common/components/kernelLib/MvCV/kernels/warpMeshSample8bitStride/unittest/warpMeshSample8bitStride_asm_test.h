#ifndef _warpMeshSample8bitStride_ASM_TEST_H_
#define _warpMeshSample8bitStride_ASM_TEST_H_

extern unsigned int warpMeshSample8bitStrideCycleCount;

void warpMeshSample8bitStride_asm_test(unsigned char **input_tile, unsigned char * output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height);

#endif //_warpMeshSample8bitStride_ASM_TEST_H_
