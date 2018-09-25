#ifndef __HUFFMAN_ENCODE_H__
#define __HUFFMAN_ENCODE_H__

#include <mv_types.h>
#include <mvcv_macro.h>

typedef struct huffman_s
{
    const unsigned char (*haclen)[12];
    const unsigned short (*hacbit)[12];
    const unsigned char *hdclen;
    const unsigned short *hdcbit;
    const unsigned short *qtable;
    short dc;
} huffman_t;

typedef struct bitbuffer_s
{
    unsigned buf;
    unsigned n;
}
bitbuffer_t;

//!@{
/// Quantize and Encode a 8x8 DCT block by JPEG Huffman lossless coding. This function writes encoded bit-stream into bit-buffer.
/// @param[in]  ctx                - pointer to encoder context
/// @param[in] data                - pointer to 8x8 DCT block
/// @param[out]  jpgbuff           - output buffer
/// @param[out]  jpgn              - output index
/// @param[out]  bitbuf            - structure that contain the values and number of remaining bits
/// @param[in]  shave_buff_size    - buffer maximum size

MVCV_FUNC(void, mvcvHuffman_encode, huffman_t * const ctx, const short data[64], unsigned char *jpgbuff, unsigned int *jpgn, bitbuffer_t *bitbuf, unsigned int shave_buff_size)
//!@}

#endif //__HUFFMAN_ENCODE_H__
