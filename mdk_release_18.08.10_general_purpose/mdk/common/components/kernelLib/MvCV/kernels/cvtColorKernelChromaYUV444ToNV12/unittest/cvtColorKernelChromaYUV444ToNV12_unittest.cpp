//cvtColorKernel test
//Asm function prototype:
//    void mvcvCvtColorKernelChromaYUV420ToNV12_asm(u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width);
//
//Asm test function prototype:
//    void ChromaYUV420ToNV12_asm_test(unsigned char *rgb, unsigned char *y ,unsigned char *u ,unsigned char *v , unsigned int width)
//
//C function prototype:
//    void mvcvCvtColorKernelChromaYUV420ToNV12(u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width);
//
//Parameter description:
// ChromaYUV420ToNV12
// inU	- line from input U plane
// inV	- line from input V plane
// outUV- UV plane in the first of the line the output NV12 image
// width- Line width in pixels


#include "gtest/gtest.h"
#include "cvtColorKernelChromaYUV444ToNV12.h"
#include "cvtColorKernelChromaYUV444ToNV12_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

using ::testing::TestWithParam;
using ::testing::Values;

class cvtColorTest : public ::testing::TestWithParam< unsigned int > {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

//void ChromaYUV444ToNV12_asm_test(unsigned char *inU[2], unsigned char *inV[2] ,unsigned char *outUV , unsigned int width)

	unsigned char *inU[2];
	unsigned char *inV[2];
	unsigned char *outUV;
	unsigned char *outUV_ASM;
	unsigned int width;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(cvtColorTest , TestUniformInputLinesChromaYUV444ToNV12)
{
	inputGen.SelectGenerator("uniform");
	width = 320;
	inU[0] = inputGen.GetLine(width, 0xDE);
	inU[1] = inputGen.GetLine(width, 0xAD);
	inV[0] = inputGen.GetLine(width, 0xBE);
	inV[1] = inputGen.GetLine(width, 0xEF);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV444ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV444ToNV12(inU, inV, outUV, width);
	RecordProperty("CyclePerPixel", cvtColorChromaYUV444ToNV12CycleCount / width);

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);
}

TEST_F(cvtColorTest , TestRandomInputLineChromaYUV444ToNV12)
{
	inputGen.SelectGenerator("random");
	width = 320;
	inU[0] = inputGen.GetLine(width, 0, 255);
	inU[1] = inputGen.GetLine(width, 0, 255);
	inV[0] = inputGen.GetLine(width, 0, 255);
	inV[1] = inputGen.GetLine(width, 0, 255);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV444ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV444ToNV12(inU, inV, outUV, width);

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);
}

TEST_F(cvtColorTest , TestUniformInputLineOddWidthChromaYUV444ToNV12)
{
	inputGen.SelectGenerator("uniform");
	width = 33;
	inU[0] = inputGen.GetLine(width, 0xDE);
	inU[1] = inputGen.GetLine(width, 0xAD);
	inV[0] = inputGen.GetLine(width, 0xBE);
	inV[1] = inputGen.GetLine(width, 0xEF);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV444ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV444ToNV12(inU, inV, outUV, width);

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);
}

//---------------- parameterized tests -------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, cvtColorTest,
                        Values(8u, 16u, 24u, 32u, 40u, 48u, 56u, 320u, 640u, 800u, 1280u, 1920u);
);

TEST_P(cvtColorTest, TestWidthRandomData)
{
    inputGen.SelectGenerator("random");
    width = GetParam();
	inU[0] = inputGen.GetLine(width, 0, 255);
	inU[1] = inputGen.GetLine(width, 0, 255);
	inV[0] = inputGen.GetLine(width, 0, 255);
	inV[1] = inputGen.GetLine(width, 0, 255);

	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV444ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV444ToNV12(inU, inV, outUV, width);

/*
	if(width==24)
	{
		for(unsigned int i=0; i<width; i++)
			printf("outUV[%2d]=0x%02x - outUV_ASM[%2d]=0x%02x\n", i, outUV[i], i, outUV_ASM[i]);
	}
*/

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);
}
