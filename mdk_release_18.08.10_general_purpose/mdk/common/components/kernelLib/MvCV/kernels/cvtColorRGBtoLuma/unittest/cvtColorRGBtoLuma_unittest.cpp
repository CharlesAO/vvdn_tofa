//cvtColorKernelYUV422ToRGB kernel test

//Asm function prototype:
//	void cvtColorRGBtoLuma_asm(u8** inR, u8** inG, u8** inB, u8** yOut, u32 width);

//Asm test function prototype:
//    	void cvtColorRGBtoLuma_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned int width);

//C function prototype:
//  	void cvtColorRGBtoLuma(u8** inR, u8** inG, u8** inB, u8** yOut, u32 width) ;

//width	- width of input line
//input	- the input RGB line
//rOut	- out R plane values
//gOut	- out G plane values
//bOut	- out B plane values

#include "gtest/gtest.h"
#include "cvtColorRGBtoLuma.h"
#include "cvtColorRGBtoLuma_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define DELTA 1


class cvtColorRGBtoLumaTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLineR;
	unsigned char** inLineG;
	unsigned char** inLineB;
	unsigned char** outLineC_Y;
	unsigned char** outLineAsm_Y;
	unsigned char** outExpected;


	unsigned int width;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(cvtColorRGBtoLumaTest, TestUniformInputWidth16)
{
	width = 16;

	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 18);
	inLineG = inputGen.GetLines(width, 1, 18);
	inLineB = inputGen.GetLines(width, 1, 18);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	

	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width, DELTA);
}


TEST_F(cvtColorRGBtoLumaTest, TestUniformInputWidth64)
{
	width = 64;

	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 18);
	inLineG = inputGen.GetLines(width, 1, 218);
	inLineB = inputGen.GetLines(width, 1, 98);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	

	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width, DELTA);
	
}

TEST_F(cvtColorRGBtoLumaTest, TestExpectedValueWidth16)
{
	width = 16;

	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 1);
	inLineG = inputGen.GetLines(width, 1, 1);
	inLineB = inputGen.GetLines(width, 1, 1);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	outExpected = inputGen.GetEmptyLines(width, 1);
	
	inLineR[0][0] = 0;      inLineG[0][0]  =  0;       inLineB[0][0]  = 0;       outExpected[0][0] = 0;
	inLineR[0][1] = 1;      inLineG[0][1]  =  1;       inLineB[0][1]  = 1;       outExpected[0][1] = 1; 
	inLineR[0][2] = 15;     inLineG[0][2]  =  23;      inLineB[0][2]  = 145;     outExpected[0][2] = 34; 
	inLineR[0][3] = 55;     inLineG[0][3]  =  158;     inLineB[0][3]  = 68;      outExpected[0][3] = 116; 
	inLineR[0][4] = 77;     inLineG[0][4]  =  35;      inLineB[0][4]  = 97;      outExpected[0][4] = 54; 
	inLineR[0][5] = 111;    inLineG[0][5]  =  48;      inLineB[0][5]  = 233;     outExpected[0][5] = 87; 
	inLineR[0][6] = 128;    inLineG[0][6]  =  78;      inLineB[0][6]  = 14;      outExpected[0][6] = 85; 
	inLineR[0][7] = 158;    inLineG[0][7]  =  214;     inLineB[0][7]  = 254;     outExpected[0][7] = 201; 
	inLineR[0][8] = 198;    inLineG[0][8]  =  125;     inLineB[0][8]  = 147;     outExpected[0][8] = 149; 
	inLineR[0][9] = 200;    inLineG[0][9]  =  235;     inLineB[0][9]  = 89;      outExpected[0][9] = 207; 
	inLineR[0][10] = 214;   inLineG[0][10] =  98;      inLineB[0][10] = 148;     outExpected[0][10] = 138;
	inLineR[0][11] = 255;   inLineG[0][11] =  255;     inLineB[0][11] = 255;     outExpected[0][11] = 255;
	inLineR[0][12] = 178;   inLineG[0][12] =  48;      inLineB[0][12] = 39;      outExpected[0][12] = 85;
	inLineR[0][13] = 169;   inLineG[0][13] =  69;      inLineB[0][13] = 125;     outExpected[0][13] = 105;
	inLineR[0][14] = 58;    inLineG[0][14] =  145;     inLineB[0][14] = 45;      outExpected[0][14] = 107;
	inLineR[0][15] = 124;   inLineG[0][15] =  219;     inLineB[0][15] = 37;      outExpected[0][15] = 169;
	

	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
	
	outputCheck.CompareArrays(outLineC_Y[0], outExpected[0], width, DELTA);
	outputCheck.CompareArrays(outLineAsm_Y[0], outExpected[0], width);
}


TEST_F(cvtColorRGBtoLumaTest, TestRandomInputline)
{
	width = 320;
	
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	
	
	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
		
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width, DELTA);
}

TEST_F(cvtColorRGBtoLumaTest, TestUniformInput255)
{
	width = 8;
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 255);
	inLineG = inputGen.GetLines(width, 1, 255);
	inLineB = inputGen.GetLines(width, 1, 255);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	
	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y,  width);	
	
	
	outputCheck.ExpectAllEQ(outLineC_Y[0], width, (unsigned char)255);
	outputCheck.ExpectAllEQ(outLineAsm_Y[0], width, (unsigned char)255);
	
	
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width);
	
}

TEST_F(cvtColorRGBtoLumaTest, TestUniformInput128)
{
	width = 32;

	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 128);
	inLineG = inputGen.GetLines(width, 1, 128);
	inLineB = inputGen.GetLines(width, 1, 128);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	

	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
	
	
	outputCheck.ExpectAllInRange(outLineC_Y[0], width, (unsigned char)127, (unsigned char)129);
	outputCheck.ExpectAllInRange(outLineAsm_Y[0], width, (unsigned char)127, (unsigned char)129);
	
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width);
	
}


TEST_F(cvtColorRGBtoLumaTest, TestUniformInput43)
{
	width = 512;
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 43);
	inLineG = inputGen.GetLines(width, 1, 43);
	inLineB = inputGen.GetLines(width, 1, 43);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	

	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
	
	
	outputCheck.ExpectAllInRange(outLineC_Y[0], width, (unsigned char)42, (unsigned char)45);
	outputCheck.ExpectAllInRange(outLineAsm_Y[0], width, (unsigned char)42, (unsigned char)45);
	
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width);
	
}

TEST_F(cvtColorRGBtoLumaTest, TestUniformInput96width56)
{
	width = 56;

	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 96);
	inLineG = inputGen.GetLines(width, 1, 96);
	inLineB = inputGen.GetLines(width, 1, 96);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	

	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
	
	
	outputCheck.ExpectAllInRange(outLineC_Y[0], width, (unsigned char)95, (unsigned char)97);
	outputCheck.ExpectAllInRange(outLineAsm_Y[0], width, (unsigned char)95, (unsigned char)97);
	
	
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width);
}

TEST_F(cvtColorRGBtoLumaTest, TestRandomInputWidth88)
{
	width = 88;

	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);	
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);

	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, width);	
			
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width, DELTA);
}


TEST_F(cvtColorRGBtoLumaTest, TestRandomInputMaxWidth)
{
	width = 1920;

	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	outLineC_Y = inputGen.GetEmptyLines(width, 1);
	outLineAsm_Y = inputGen.GetEmptyLines(width, 1);
	
	mvcvCvtColorRGBtoLuma(inLineR, inLineG, inLineB, outLineC_Y, width);	
	cvtColorRGBtoLuma_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y,  width);	
			
	outputCheck.CompareArrays(outLineC_Y[0], outLineAsm_Y[0], width, DELTA);
	
}

