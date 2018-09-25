//cvtColorKernel test
//Asm function prototype:
//    void cvtColorKernelYUVToRGB_asm(u8* yIn, u8* uIn, u8* vIn, u8* out, u32 width);
//
//Asm test function prototype:
//    void YUVtoRGB_asm_test(unsigned char *rgb, unsigned char *y ,unsigned char *u ,unsigned char *v , unsigned int width)
//
//C function prototype:
//    void cvtColorKernelYUVToRGB(u8* yIn, u8* uIn, u8* vIn, u8* out, u32 width);
//
//Parameter description:
// YUVToRGB
// yIn input Y channel
// uIn input U channel
// vIn input V channel
// out output RGB channel
// width - image width in pixels


#include "gtest/gtest.h"
#include "cvtColorKernelYUVToRGB.h"
#include "cvtColorKernelYUVToRGB_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 8
#define DELTA   1

class cvtColorKernelYUVToRGBTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char *yIn;
	unsigned char *uIn;
	unsigned char *vIn;
	unsigned char *rgbC;
	unsigned char *rgbAsm;
	unsigned int width;

	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(cvtColorKernelYUVToRGBTest, TestRandomDATASmallWidth)
{
	width = randGen->GenerateUInt(16, 1281, 8);
	inputGen.SelectGenerator("random");
	yIn = inputGen.GetLine(width, 0, 255);
	uIn = inputGen.GetLine(width/2, 0, 255);
	vIn = inputGen.GetLine(width/2, 0, 255);

	rgbC   = inputGen.GetEmptyLine(width * 3);
	rgbAsm = inputGen.GetEmptyLine(width * 3);


	mvcvCvtColorKernelYUVToRGB(yIn, uIn, vIn, rgbC, width);
	cvtColorKernelYUVToRGB_asm_test(yIn, uIn, vIn, rgbAsm, width);
	RecordProperty("CyclePerPixel", cvtColorKernelYUVToRGBCycleCount / width);

	outputChecker.CompareArrays(rgbC, rgbAsm, width * 3, DELTA);
}


TEST_F(cvtColorKernelYUVToRGBTest, TestRandomDATABigWidth)
{
	width = randGen->GenerateUInt(1281, 1920, 8);
	inputGen.SelectGenerator("random");
	yIn = inputGen.GetLine(width, 0, 255);
	uIn = inputGen.GetLine(width/2, 0, 255);
	vIn = inputGen.GetLine(width/2, 0, 255);

	rgbC   = inputGen.GetEmptyLine(width * 3);
	rgbAsm = inputGen.GetEmptyLine(width * 3);


	mvcvCvtColorKernelYUVToRGB(yIn, uIn, vIn, rgbC, width);
	cvtColorKernelYUVToRGB_asm_test(yIn, uIn, vIn, rgbAsm, width);
	RecordProperty("CyclePerPixel", cvtColorKernelYUVToRGBCycleCount / width);

	outputChecker.CompareArrays(rgbC, rgbAsm, width * 3, DELTA);
	
	//outputChecker.DumpArrays("refInputy", "refInputy.c", &yIn, width, 1);
	//outputChecker.DumpArrays("refInputu", "refInputu.c", &uIn, width/2, 1);
	//outputChecker.DumpArrays("refInputv", "refInputv.c", &vIn, width/2, 1);
	//
	//outputChecker.DumpArrays("refOutput", "refOutput.c", &rgbC, width * 3, 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", &rgbAsm, width * 3, 1);
}