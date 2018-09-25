//cvtColorKernelYUV422ToRGB kernel test

//Asm function prototype:
//	void cvtColorRGBfp16ToLumaU8(half* inRGB[3], u8* yOut, u32 width) 

//Asm test function prototype:
// void cvtColorRGBfp16ToLumaU8_asm_test(half *inputRGB, unsigned char *Yout, unsigned int width)

//C function prototype:
//  void cvtColorRGBfp16ToLumaU8(half* inRGB[3], u8* yOut, u32 width) 


#include "gtest/gtest.h"
#include "cvtColorRGBfp16ToLumaU8.h"
#include "cvtColorRGBfp16ToLumaU8_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

const unsigned short DELTA = 1;	

using ::testing::TestWithParam;
using ::testing::Values;

class cvtColorRGBfp16ToLumaU8Test :public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	half** inLine;
	unsigned char* outLineC_Y;
	unsigned char* outLineAsm_Y;
	unsigned char* outExpected;
	unsigned int width;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(cvtColorRGBfp16ToLumaU8Test, TestUniformInputWidth16)
{
	unsigned int width = 16;

	inputGen.SelectGenerator("random");
	half** inLine = inputGen.GetLinesFloat16(width, 3, 0.0f, 1.0f);

	unsigned char* outLineC_Y = inputGen.GetEmptyLine(width);
	unsigned char* outLineAsm_Y = inputGen.GetEmptyLine(width);	
		
	mvcvCvtColorRGBfp16ToLumaU8(inLine, outLineC_Y, width);	
	cvtColorRGBfp16ToLumaU8_asm_test(inLine, outLineAsm_Y, width);	

	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width, DELTA);
} 


TEST_F(cvtColorRGBfp16ToLumaU8Test, TestUniformInputWidth24)
{
	unsigned int width = 24;

	inputGen.SelectGenerator("random");
	half** inLine = inputGen.GetLinesFloat16(width, 3, 0.0f, 1.0f);

	unsigned char* outLineC_Y = inputGen.GetEmptyLine(width);
	unsigned char* outLineAsm_Y = inputGen.GetEmptyLine(width);	

	mvcvCvtColorRGBfp16ToLumaU8(inLine, outLineC_Y, width);	
	cvtColorRGBfp16ToLumaU8_asm_test(inLine, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width, DELTA);
} 

 TEST_F(cvtColorRGBfp16ToLumaU8Test, TestInput0Width320)
{
	unsigned int width = 320;

	inputGen.SelectGenerator("uniform");
	half** inLine = inputGen.GetLinesFloat16(width, 3, 0.0f);

	unsigned char* outLineC_Y = inputGen.GetEmptyLine(width);
	unsigned char* outLineAsm_Y = inputGen.GetEmptyLine(width);	

	mvcvCvtColorRGBfp16ToLumaU8(inLine, outLineC_Y, width);	
	cvtColorRGBfp16ToLumaU8_asm_test(inLine, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width, DELTA);
}


TEST_F(cvtColorRGBfp16ToLumaU8Test, TestInput1Width240)
{
	unsigned int width = 240;

	inputGen.SelectGenerator("uniform");
	half** inLine = inputGen.GetLinesFloat16(width, 3, 1.0f);

	unsigned char* outLineC_Y = inputGen.GetEmptyLine(width);
	unsigned char* outLineAsm_Y = inputGen.GetEmptyLine(width);	

	mvcvCvtColorRGBfp16ToLumaU8(inLine, outLineC_Y, width);	
	cvtColorRGBfp16ToLumaU8_asm_test(inLine, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width, DELTA);
}
 

TEST_F(cvtColorRGBfp16ToLumaU8Test, TestRandomInputWidth1920)
{
	unsigned int width = 1920;

	inputGen.SelectGenerator("random");
	half** inLine = inputGen.GetLinesFloat16(width, 3, 0.0f, 1.0f);

	unsigned char* outLineC_Y = inputGen.GetEmptyLine(width);
	unsigned char* outLineAsm_Y = inputGen.GetEmptyLine(width);	

	mvcvCvtColorRGBfp16ToLumaU8(inLine, outLineC_Y, width);	
	cvtColorRGBfp16ToLumaU8_asm_test(inLine, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width, DELTA);
}

 INSTANTIATE_TEST_CASE_P(TestRandomData, cvtColorRGBfp16ToLumaU8Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(cvtColorRGBfp16ToLumaU8Test, TestRandomData)
{

	inputGen.SelectGenerator("random");
	unsigned int width = GetParam();
	
	half** inLine = inputGen.GetLinesFloat16(width, 3, 0.0f, 1.0f);
	
	unsigned char* outLineC_Y = inputGen.GetEmptyLine(width);
	unsigned char* outLineAsm_Y = inputGen.GetEmptyLine(width);	
	
	//outputCheck.DumpArrays("inline1", "inLine1.c", inLine[0], width);	
	//outputCheck.DumpArrays("inline2", "inLine2.c", inLine[1], width);	
	//outputCheck.DumpArrays("inline3", "inLine3.c", inLine[2], width);

	mvcvCvtColorRGBfp16ToLumaU8(inLine, outLineC_Y, width);	
	cvtColorRGBfp16ToLumaU8_asm_test(inLine, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width, DELTA);

}   