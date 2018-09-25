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
#include "cvtColorKernelChromaYUV420ToNV12.h"
#include "cvtColorKernelChromaYUV420ToNV12_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

void mvcvCvtColorKernelChromaYUV420ToNV12_REF(u8* inU, u8* inV, u8* outUV, u32 width);
#define mvcvCvtColorKernelChromaYUV420ToNV12 mvcvCvtColorKernelChromaYUV420ToNV12_REF

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

//void ChromaYUV420ToNV12_asm_test(unsigned char *inU, unsigned char *inV ,unsigned char *outUV , unsigned int width);

	unsigned char *inU;
	unsigned char *inV;
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

TEST_F(cvtColorTest , TestUniformInputLineChromaYUV420ToNV12)
{
	inputGen.SelectGenerator("uniform");
	width = 320;
	inU = inputGen.GetLine(width/2, 0xAB);
	inV = inputGen.GetLine(width/2, 0xCD);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV420ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV420ToNV12(inU, inV, outUV, width);

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);
}

TEST_F(cvtColorTest , TestRandomInputLineChromaYUV420ToNV12)
{
	inputGen.SelectGenerator("random");
	width = 320;
	inU = inputGen.GetLine(width/2, 0, 255);
	inV = inputGen.GetLine(width/2, 0, 255);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV420ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV420ToNV12(inU, inV, outUV, width);

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);
}

TEST_F(cvtColorTest , TestUniformInputLineOddWidthChromaYUV420ToNV12)
{
	inputGen.SelectGenerator("uniform");
	width = 33;
	inU = inputGen.GetLine(width/2, 0xAB);
	inV = inputGen.GetLine(width/2, 0xCD);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV420ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV420ToNV12(inU, inV, outUV, width);

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);
}

//---------------- parameterized tests -------------------------

INSTANTIATE_TEST_CASE_P(RandomInputs, cvtColorTest,
                        Values(8u, 24u, 32u, 40u, 56u, 64u, 72u, 96u, 104u, 120u, 320u, 640u, 800u, 1280u, 1920u);
);

TEST_P(cvtColorTest, TestWidthRandomData)
{
    inputGen.SelectGenerator("random");
    width = GetParam();
	inU = inputGen.GetLine(width/2, 0, 255);
	inV = inputGen.GetLine(width/2, 0, 255);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);
	outUV = inputGen.GetEmptyLine(width/2*2);
	outUV_ASM = inputGen.GetEmptyLine(width/2*2);

	ChromaYUV420ToNV12_asm_test(inU, inV, outUV_ASM, width);
	mvcvCvtColorKernelChromaYUV420ToNV12(inU, inV, outUV, width);

	arrCheck.CompareArrays(outUV, outUV_ASM, width/2*2);

}
