//cvtColorKernelRGBtoUV kernel test

//Asm function prototype:
//	void cvtColorRGBtoUV_asm(u8** inR, u8** inG, u8** inB, u8** yOut, u32 width, u32 line);

//Asm test function prototype:
//    	void cvtColorRGBtoUV_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Yout, unsigned int width,  unsigned int line);

//C function prototype:
//  	void cvtColorRGBtoUV(u8** inR, u8** inG, u8** inB, u8** yOut, u32 width, u32 line) ;


#include "gtest/gtest.h"
#include "cvtColorRGBtoUV.h"
#include "cvtColorRGBtoUV_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define DELTA 2


class cvtColorRGBtoUVTest : public ::testing::Test {
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
	unsigned char** outLineC_UV;
	unsigned char** outLineAsm_UV;
	unsigned char** outExpected;


	unsigned int width;
	unsigned int line;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(cvtColorRGBtoUVTest, TestUniformInputWidth8ExpectedValue)
{
	width = 8;
	line = 2;
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 18);
	inLineG = inputGen.GetLines(width, 1, 18);
	inLineB = inputGen.GetLines(width, 1, 18);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);
	outExpected = inputGen.GetEmptyLines(width, 1);
	 
	
	inLineR[0][0] = 0;      inLineG[0][0]  =  0;       inLineB[0][0]  = 0;       outExpected[0][0] = 128;
	inLineR[0][1] = 1;      inLineG[0][1]  =  1;       inLineB[0][1]  = 1;       outExpected[0][1] = 128; 
	inLineR[0][2] = 15;     inLineG[0][2]  =  23;      inLineB[0][2]  = 145;     outExpected[0][2] = 145; 
	inLineR[0][3] = 55;     inLineG[0][3]  =  158;     inLineB[0][3]  = 68;      outExpected[0][3] = 99; 
	inLineR[0][4] = 77;     inLineG[0][4]  =  35;      inLineB[0][4]  = 97;      outExpected[0][4] = 181; 
	inLineR[0][5] = 111;    inLineG[0][5]  =  48;      inLineB[0][5]  = 233;     outExpected[0][5] = 144; 
	inLineR[0][6] = 128;    inLineG[0][6]  =  78;      inLineB[0][6]  = 14;      outExpected[0][6] = 122; 
	inLineR[0][7] = 158;    inLineG[0][7]  =  214;     inLineB[0][7]  = 254;     outExpected[0][7] = 128; 
	
	
	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
	
	outputCheck.CompareArrays(outLineC_UV[0], outExpected[0], width, DELTA);
	outputCheck.CompareArrays(outLineAsm_UV[0], outExpected[0], width, DELTA);
	
}



TEST_F(cvtColorRGBtoUVTest, TestUniformInputWidth16)
{
	width = 16;
	line = 2;
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 18);
	inLineG = inputGen.GetLines(width, 1, 18);
	inLineB = inputGen.GetLines(width, 1, 18);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);
	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
	
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width, DELTA);
	
}


TEST_F(cvtColorRGBtoUVTest, TestUniformInputWidth64)
{
	width = 64;
	line = 2;

	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 18);
	inLineG = inputGen.GetLines(width, 1, 218);
	inLineB = inputGen.GetLines(width, 1, 78);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);
	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
	
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width, DELTA);
	
}


TEST_F(cvtColorRGBtoUVTest, TestRandomInput)
{
	width = 320;
	line = 2;
	
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);
	
	
	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
		
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width, DELTA);	
} 


TEST_F(cvtColorRGBtoUVTest, TestUniformInput255)
{
	width = 80;
	line = 0;
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 255);
	inLineG = inputGen.GetLines(width, 1, 255);
	inLineB = inputGen.GetLines(width, 1, 255);
	
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV,  width, line);	
	
	outputCheck.ExpectAllEQ(outLineC_UV[0], width, (unsigned char)128);
	outputCheck.ExpectAllEQ(outLineAsm_UV[0], width, (unsigned char)128);
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width);
}


TEST_F(cvtColorRGBtoUVTest, TestUniformInput128)
{
	width = 240;
	line = 0;
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 128);
	inLineG = inputGen.GetLines(width, 1, 128);
	inLineB = inputGen.GetLines(width, 1, 128);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
	
	
	outputCheck.ExpectAllEQ(outLineC_UV[0], width, (unsigned char)128);
	outputCheck.ExpectAllEQ(outLineAsm_UV[0], width, (unsigned char)128);
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width);
	
}


TEST_F(cvtColorRGBtoUVTest, TestUniformInput43)
{
	width = 8;
	line = 0;
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 43);
	inLineG = inputGen.GetLines(width, 1, 43);
	inLineB = inputGen.GetLines(width, 1, 43);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
	
	outputCheck.ExpectAllEQ(outLineC_UV[0], width, (unsigned char)128);
	outputCheck.ExpectAllEQ(outLineAsm_UV[0], width, (unsigned char)128);
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width);
}


TEST_F(cvtColorRGBtoUVTest, TestUniformInput96width32)
{
	width = 32;
	line = 0;
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 96);
	inLineG = inputGen.GetLines(width, 1, 96);
	inLineB = inputGen.GetLines(width, 1, 96);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
	
	outputCheck.ExpectAllEQ(outLineC_UV[0], width, (unsigned char)128);
	outputCheck.ExpectAllEQ(outLineAsm_UV[0], width, (unsigned char)128);
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width);
	
}


TEST_F(cvtColorRGBtoUVTest, TestRandomInputWidth88)
{
	width = 88;
	line = 0;
	
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
			
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width, DELTA);
}


TEST_F(cvtColorRGBtoUVTest, TestRandomInputMaxWidth)
{
	width = 1920;
	line = 0;
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	outLineC_UV = inputGen.GetEmptyLines(width, 1);
	outLineAsm_UV = inputGen.GetEmptyLines(width, 1);	

	mvcvCvtColorRGBtoUV(inLineR, inLineG, inLineB, outLineC_UV, width, line);	
	cvtColorRGBtoUV_asm_test(inLineR, inLineG, inLineB, outLineAsm_UV, width, line);	
			
	outputCheck.CompareArrays(outLineC_UV[0], outLineAsm_UV[0], width, DELTA);
}

