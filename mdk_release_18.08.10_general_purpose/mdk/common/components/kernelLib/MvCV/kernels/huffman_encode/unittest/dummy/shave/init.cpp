    ///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     huffman_encode function call for unitary test
///

#include <huffman_encode.h>
#include <stdio.h>   
#include <stdlib.h>   
#include <mv_types.h>
#include <svuCommonShave.h>

#define SHAVE_BUFFER_SIZE 8*1024


 u8 haclen[16][12] __attribute__ ((aligned (16)));
u16 hacbit[16][12] __attribute__ ((aligned (16)));
 u8 hdclen[12] __attribute__ ((aligned (16)));
u16 hdcbit[12] __attribute__ ((aligned (16)));
u16 qtable[8][8] __attribute__ ((aligned (16)));
s16 dc __attribute__ ((aligned (16))); 
s16 data[8][8] __attribute__ ((aligned (16))); 


 u8 jpgbuff[SHAVE_BUFFER_SIZE] __attribute__ ((aligned (16)));

u32 jpgn;
u32 buf;
u32 n;
u32 shave_buff_size;

int main( void )
{
    huffman_t ctx = {haclen, hacbit, hdclen, hdcbit, (const unsigned short *)qtable,   dc};
    bitbuffer_t bitbuf = {buf, n};

    #ifdef UNIT_TEST_USE_C_VERSION
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff, &jpgn, &bitbuf, shave_buff_size);
    #else
    mvcvHuffman_encode_asm(&ctx, (s16*)data, jpgbuff, &jpgn, &bitbuf, shave_buff_size);
    #endif
    SHAVE_HALT; 
    return 0;
}
