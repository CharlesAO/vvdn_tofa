//cvtColorKernel test
//Asm function prototype:
//    void cvtColorKernelRGBToYUV_asm(u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width);
//
//Asm test function prototype:
//    void RGBtoYUV_asm_test(unsigned char *rgb, unsigned char *y ,unsigned char *u ,unsigned char *v , unsigned int width)
//
//C function prototype:
//    void cvtColorKernelRGBToYUV(u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width);
//
//Parameter description:
// RGBToYUV
// in	- input RGB channel
// yOut	- output Y channel
// uOut	- output U channel
// vOut	- output V channel
// width- image width in pixels


#include "gtest/gtest.h"
#include <convertFrom12BppTo8Bpp.h>
#include "convertFrom12BppTo8Bpp_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"


class convertFrom12BppTo8BppTest : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

    u8* input;
    u8* outC;
    u8* outAsm;
    unsigned int width;
	
    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(convertFrom12BppTo8BppTest , TestUniformInputLine)
{
	inputGen.SelectGenerator("uniform");
	width = 32;
	input = inputGen.GetLine(width * 2, 50);
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);
	
	ConvertFrom12BppTo8Bpp_asm_test(outAsm, input, width);
	mvcvConvert12BppTo8Bpp(outC, input, width);
	
	arrCheck.CompareArrays(outC, outAsm, width);
	//arrCheck.DumpArrays("output", "outC.c", outC, width);
	//arrCheck.DumpArrays("output", "outAsm.c", outAsm, width);
	//arrCheck.DumpArrays("input", "input.c", input, width * 2);

}

TEST_F(convertFrom12BppTo8BppTest , TestRandomInputLine)
{
	inputGen.SelectGenerator("random");
	width = 1280;
	input = inputGen.GetLine(width * 2, 0, 255);
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);
	
	ConvertFrom12BppTo8Bpp_asm_test(outAsm, input, width);
	mvcvConvert12BppTo8Bpp(outC, input, width);
	
	arrCheck.CompareArrays(outC, outAsm, width);


}

TEST_F(convertFrom12BppTo8BppTest , TestZerosInputLine)
{
	inputGen.SelectGenerator("random");
	width = 32;
	input = inputGen.GetLine(width * 2, 0);
	outC = inputGen.GetEmptyLine(width);
	outAsm = inputGen.GetEmptyLine(width);
	
	ConvertFrom12BppTo8Bpp_asm_test(outAsm, input, width);
	mvcvConvert12BppTo8Bpp(outC, input, width);
	
	arrCheck.CompareArrays(outC, outAsm, width);
}



