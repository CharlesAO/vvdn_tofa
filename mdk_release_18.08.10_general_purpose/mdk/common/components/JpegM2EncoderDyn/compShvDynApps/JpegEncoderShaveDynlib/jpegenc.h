#ifndef __JPEG_H__
#define __JPEG_H__

#include <swcCdmaCommonDefines.h>
#include "jpegDefines.h"
#include "huffman_encode.h"
#include "JpegEncoderApi.h"
#ifdef __cplusplus
extern "C"
{
#endif

/*
 RGB to YCbCr Conversion:
 */
// Y = 0.299*R + 0.587*G + 0.114*B
/*
 __inline color RGB2Y(const color r, const color g, const color b)
 {
 return (32768 + 19595*r + 38470*g + 7471*b) >> 16;
 }
 // Cb = 128 - 0.1687*R - 0.3313*G + 0.5*B
 __inline color RGB2Cb(const color r, const color g, const color b)
 {
 return (8421376 - 11058*r - 21709*g + 32767*b) >> 16;
 }
 // Cr = 128 + 0.5*R - 0.4187*G - 0.0813*B
 __inline color RGB2Cr(const color r, const color g, const color b)
 {
 return (8421376 + 32767*r - 27438*g - 5329*b) >> 16;
 }
 */

typedef struct
{
    u32 x;
    u32 y;
    short Y[4][8][8];
    short U[8][8];
    short V[8][8];
} imgspectype;

//---------------- J P E G ---------------
extern huffman_t huffman_ctx[3];

#define HUFFMAN_CTX_Y   &huffman_ctx[0]
#define HUFFMAN_CTX_Cb  &huffman_ctx[1]
#define HUFFMAN_CTX_Cr  &huffman_ctx[2]

void huffman_start(short height, short width, unsigned char *jpgbuff,
                   unsigned int *jpgn,  unsigned short subsampling);
void huffman_stop(unsigned char *jpgbuff, unsigned int *jpgn,
                  bitbuffer_t *extrabit);
void start_section(unsigned char *jpgbuff, unsigned int *jpgn,
                   const short macroBlocksNo);
void reset_interval(unsigned char *jpgbuff, unsigned int *jpgn,
                    const short restartMarker, bitbuffer_t *pbb);

#ifdef __cplusplus
}
#endif

#endif//__JPEG_H__
