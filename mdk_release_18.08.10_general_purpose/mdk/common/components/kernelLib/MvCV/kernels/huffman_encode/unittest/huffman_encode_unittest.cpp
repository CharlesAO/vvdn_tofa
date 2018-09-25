// huffman_encode - unittest
//  --------------------------------------------------------------------------
//  Quantize and Encode a 8x8 DCT block by JPEG Huffman lossless coding.
//  This function writes encoded bit-stream into bit-buffer.
//
//  ARGUMENTS:
//      ctx     - pointer to encoder context;
//      data    - pointer to 8x8 DCT block;
//      jpgbuff - output buffer
//      jpgn    - output index
//      bitbuf  - structure that contain the values and number of remaining bits
//      shavebuffersize  - maximum shave output size

#include "gtest/gtest.h"
#include <huffman_encode.h>
#include "huffman_encode_asm_test.h"
#include "data.h"
#include "InputGenerator.h"
#include "RandomGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define SHAVE_BUFFER_SIZE 8*1024
class huffman_encodeTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned int width;
    InputGenerator inputGen;
    RandomGenerator ranGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    unsigned char* haclen;
    unsigned short* hacbit;
    unsigned char* hdclen;
    unsigned short* hdcbit;
    unsigned short* qtable;
    short dc;
    short* data;
    
    unsigned char* jpgbuff_asm;
    unsigned int  jpgn_asm;
    unsigned int  buf_asm;
    unsigned int  n_asm;
    
    unsigned char* jpgbuff_c;
    unsigned int jpgn_c;
    unsigned int buf_c;
    unsigned int n_c;
    unsigned int shave_buff_size;
    
    ArrayChecker outputChecker;
    
    virtual void TearDown()
    {
    }
};

TEST_F(huffman_encodeTest, TestAsmY)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HYAClen;
    hacbit  = (unsigned short* )HYACbits;
    hdclen  = (unsigned char*  )HYDClen;
    hdcbit  = (unsigned short* )HYDCbits;
    qtable  = (unsigned short*  )qtable_lum;
    dc      = 0;
    
    inputGen.SelectGenerator("random");
    jpgn_asm  = randGen->GenerateUInt(1, 1000, 1);;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HYAClen, HYACbits, HYDClen, HYDCbits, (const unsigned short *)qtable_lum,   0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64, 0, 255);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmU)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HCAClen;
    hacbit  = (unsigned short* )HCACbits;
    hdclen  = (unsigned char*  )HCDClen;
    hdcbit  = (unsigned short* )HCDCbits;
    qtable  = (unsigned short*  )qtable_chrom;
    dc      = 0;
    
    inputGen.SelectGenerator("random");
    jpgn_asm  = randGen->GenerateUInt(1, 1000, 1);;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HCAClen, HCACbits, HCDClen, HCDCbits, (const unsigned short*)qtable_chrom, 0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0,255);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmV)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HCAClen;
    hacbit  = (unsigned short* )HCACbits;
    hdclen  = (unsigned char*  )HCDClen;
    hdcbit  = (unsigned short* )HCDCbits;
    qtable  = (unsigned short*  )qtable_chrom;
    dc      = 0;
    
    inputGen.SelectGenerator("random");
    jpgn_asm  = randGen->GenerateUInt(1, 1000, 1);;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HCAClen, HCACbits, HCDClen, HCDCbits, (const unsigned short*)qtable_chrom, 0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0,255);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmY0)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HYAClen;
    hacbit  = (unsigned short* )HYACbits;
    hdclen  = (unsigned char*  )HYDClen;
    hdcbit  = (unsigned short* )HYDCbits;
    qtable  = (unsigned short*  )qtable_lum;
    dc      = 0;
    
    inputGen.SelectGenerator("uniform");
    jpgn_asm  = 0;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HYAClen, HYACbits, HYDClen, HYDCbits, (const unsigned short *)qtable_lum,   0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0,255);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmU0)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HCAClen;
    hacbit  = (unsigned short* )HCACbits;
    hdclen  = (unsigned char*  )HCDClen;
    hdcbit  = (unsigned short* )HCDCbits;
    qtable  = (unsigned short*  )qtable_chrom;
    dc      = 0;
    
    inputGen.SelectGenerator("uniform");
    jpgn_asm  = 0;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HCAClen, HCACbits, HCDClen, HCDCbits, (const unsigned short*)qtable_chrom, 0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmV0)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HCAClen;
    hacbit  = (unsigned short* )HCACbits;
    hdclen  = (unsigned char*  )HCDClen;
    hdcbit  = (unsigned short* )HCDCbits;
    qtable  = (unsigned short*  )qtable_chrom;
    dc      = 0;
    
    inputGen.SelectGenerator("uniform");
    jpgn_asm  = 0;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HCAClen, HCACbits, HCDClen, HCDCbits, (const unsigned short*)qtable_chrom, 0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmY00)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HYAClen;
    hacbit  = (unsigned short* )HYACbits;
    hdclen  = (unsigned char*  )HYDClen;
    hdcbit  = (unsigned short* )HYDCbits;
    qtable  = (unsigned short*  )qtable_lum;
    dc      = 0;
    
    inputGen.SelectGenerator("uniform");
    jpgn_asm  = 0;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HYAClen, HYACbits, HYDClen, HYDCbits, (const unsigned short *)qtable_lum,   0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0,255);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmU00)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HCAClen;
    hacbit  = (unsigned short* )HCACbits;
    hdclen  = (unsigned char*  )HCDClen;
    hdcbit  = (unsigned short* )HCDCbits;
    qtable  = (unsigned short*  )qtable_chrom;
    dc      = 0;
    
    inputGen.SelectGenerator("uniform");
    jpgn_asm  = 0;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HCAClen, HCACbits, HCDClen, HCDCbits, (const unsigned short*)qtable_chrom, 0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}

TEST_F(huffman_encodeTest, TestAsmV00)
{
    inputGen.SelectGenerator("uniform");
	shave_buff_size = SHAVE_BUFFER_SIZE;
    jpgbuff_asm = inputGen.GetLine(shave_buff_size, 0);
      jpgbuff_c = inputGen.GetLine(shave_buff_size, 0);
    
    haclen  = (unsigned char*  )HCAClen;
    hacbit  = (unsigned short* )HCACbits;
    hdclen  = (unsigned char*  )HCDClen;
    hdcbit  = (unsigned short* )HCDCbits;
    qtable  = (unsigned short*  )qtable_chrom;
    dc      = 0;
    
    inputGen.SelectGenerator("uniform");
    jpgn_asm  = 0;
    buf_asm   = 1;
    n_asm     = 1;
    
    jpgn_c = jpgn_asm;
    buf_c  = 1;
    n_c    = 1;
    
    huffman_t ctx = {HCAClen, HCACbits, HCDClen, HCDCbits, (const unsigned short*)qtable_chrom, 0};
    bitbuffer_t bitbuf = {buf_c, n_c};
    
    data = (short*)inputGen.GetLineU16(64,0);
    
    mvcvHuffman_encode(&ctx, (s16*)data, jpgbuff_c, &jpgn_c, &bitbuf, shave_buff_size);
    huffman_encode_asm_test(haclen, hacbit, hdclen,  hdcbit, qtable, dc, data, jpgbuff_asm, &jpgn_asm, &buf_asm, &n_asm, shave_buff_size);
    
    outputChecker.CompareArrays(jpgbuff_asm,jpgbuff_c, shave_buff_size);
    EXPECT_EQ(jpgn_c, jpgn_asm);
    EXPECT_EQ(buf_c, buf_asm);
    EXPECT_EQ(n_c, n_asm);
}


