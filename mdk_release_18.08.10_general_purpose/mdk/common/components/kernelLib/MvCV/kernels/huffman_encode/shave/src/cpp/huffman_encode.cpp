#include <huffman_encode.h>
#include <stdio.h>
#include <stdlib.h>

#define QTAB_SCALE	10
// zig-zag table
static const unsigned char zig[64] =
{
     0,
     1, 8,
    16, 9, 2,
     3,10,17,24,
    32,25,18,11, 4,
     5,12,19,26,33,40,
    48,41,34,27,20,13, 6,
     7,14,21,28,35,42,49,56,
    57,50,43,36,29,22,15,
    23,30,37,44,51,58,
    59,52,45,38,31,
    39,46,53,60,
    61,54,47,
    55,62,
    63
};

//local functions need them only for testing purpose
//***********************************
static short quantize(const short data, const unsigned short qt);
static unsigned huffman_bits(const short value);
static unsigned huffman_magnitude(const short value);
static void writebits(bitbuffer_t * const pbb, unsigned bits, unsigned nbits, unsigned char *jpgbuff, unsigned int *jpgn, unsigned int shave_buff_size);
static void writebyte(const unsigned char b, unsigned char *jpgbuff, unsigned int *jpgn, unsigned int shave_buff_size);
//***********************************


void mvcvHuffman_encode(huffman_t * const ctx, const short data[],
                           unsigned char *jpgbuff, unsigned int *jpgn, bitbuffer_t *bitbuf, unsigned int shave_buff_size)
{
    unsigned magn, bits;
    unsigned zerorun, i;
    short diff;

    short dc = quantize(data[0], ctx->qtable[0]);

    //short dc = data[0];
    // difference between old and new DC
    diff = dc - ctx->dc;
    ctx->dc = dc;

    bits = huffman_bits(diff); // VLI
    magn = huffman_magnitude(diff); // VLI length the number of bits that are enough to represent given value.

    // encode VLI length
    writebits(bitbuf, ctx->hdcbit[magn], ctx->hdclen[magn], jpgbuff, jpgn, shave_buff_size);

    // encode VLI itself
    writebits(bitbuf, bits, magn, jpgbuff, jpgn, shave_buff_size);


    for (zerorun = 0, i = 1; i < 64; i++)
    {
        const short ac = quantize(data[zig[i]], ctx->qtable[zig[i]]);
	

        if (ac)
        {
            while (zerorun >= 16)
            {
                zerorun -= 16;
                // ZRL
                writebits(bitbuf, ctx->hacbit[15][0], ctx->haclen[15][0],
                          jpgbuff, jpgn, shave_buff_size);
            }

			

            bits = huffman_bits(ac);
            magn = huffman_magnitude(ac);

            writebits(bitbuf, ctx->hacbit[zerorun][magn],
                      ctx->haclen[zerorun][magn], jpgbuff, jpgn, shave_buff_size);

            writebits(bitbuf, bits, magn, jpgbuff, jpgn, shave_buff_size);

            zerorun = 0;
        }
        else
            zerorun++;


    }
	

    if (zerorun)
    { // EOB - End Of Block
        writebits(bitbuf, ctx->hacbit[0][0], ctx->haclen[0][0], jpgbuff, jpgn, shave_buff_size);
    }
	
}

static short quantize(const short data, const unsigned short qt)
{

    //return (data*qt - (data>>15) + ((1<<(QTAB_SCALE-1))-1)) >> (QTAB_SCALE);
    return (data * qt - (data >> 15) + ((1 << (QTAB_SCALE - 1)) - 1)) >> (QTAB_SCALE);
    //return data/( (1 << QTAB_SCALE)/qt );
}

static unsigned huffman_bits(const short value)
{
    return value + (value >> 15);
}

static unsigned huffman_magnitude(const short value)
{
    unsigned x = (value < 0) ? -value : value;
    unsigned m = 0;

    while (x >> m)
        ++m;

    return m;
}

static void writebits(bitbuffer_t * const pbb, unsigned bits, unsigned nbits,
                      unsigned char *jpgbuff, unsigned int *jpgn, unsigned int shave_buff_size)
{
    // shift old bits to the left, add new to the right
    pbb->buf = (pbb->buf << nbits) | (bits & ((1 << nbits) - 1));

    // new number of bits
    nbits += pbb->n;

    // flush whole bytes
    while (nbits >= 8)
    {
        unsigned char b;

        nbits -= 8;
        b = pbb->buf >> nbits;

        writebyte(b, jpgbuff, jpgn, shave_buff_size);

        if (b == 0xFF)
            writebyte(0, jpgbuff, jpgn, shave_buff_size); // add 0x00 after 0xFF
    }

    // remember how many bits is remained
    pbb->n = nbits;
}

static void writebyte(const unsigned char b, unsigned char *jpgbuff,
                      unsigned int *jpgn, unsigned int shave_buff_size)
{
    jpgbuff[(*jpgn)++] = b;

    if (*jpgn >= shave_buff_size)
    { //sizeof(jpgbuff)) {
        *jpgn = 0xDEADBEEF;

        //write_jpeg(jpgbuff, sizeof(jpgbuff));
    }
}
