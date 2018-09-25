#ifndef _HUFFMAN_ENCODE_ASM_TEST_H_
#define _HUFFMAN_ENCODE_ASM_TEST_H_

extern unsigned int huffman_encodeCycleCount;
void huffman_encode_asm_test(unsigned char* haclen, unsigned short* hacbit,  unsigned char* hdclen,  
					unsigned short* hdcbit,  unsigned short* qtable,  short dc,  short* data,
					unsigned char* jpgbuff,  unsigned int* jpgn,  unsigned int* buf,  unsigned int* n, unsigned int shave_buff_size);

#endif //_HUFFMAN_ENCODE_ASM_TEST_H_
