//cvtColorKernelRGBtoUV420 kernel test

//Asm function prototype:
//	void cvtColorRGBtoUV420_asm(u8** inR, u8** inG, u8** inB, u8** uOut, u8** vOut, u32 width);

//Asm test function prototype:
//    	void cvtColorRGBtoUV420_asm_test(unsigned char **inputR, unsigned char **inputG, unsigned char **inputB, unsigned char **Uout, unsigned char **Vout, unsigned int width);

//C function prototype:
//  	void cvtColorRGBtoUV420(u8** inR, u8** inG, u8** inB, u8** uOut, u8** vOut, u32 width) ;


#include "gtest/gtest.h"
#include "cvtColorRGBtoUV420.h"
///
#include "cvtColorRGBtoUV420_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>



#define DELTA 2

using ::testing::TestWithParam;
using ::testing::Values;


class cvtColorRGBtoUV420Test : public ::testing::TestWithParam< unsigned int > {
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
	unsigned char** outLineC_U;
	unsigned char** outLineC_V;
	unsigned char** outLineAsm_U;
	unsigned char** outLineAsm_V;
	unsigned char** outExpected_U;
	unsigned char** outExpected_V;


	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	ArrayChecker outputChecker;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(cvtColorRGBtoUV420Test, TestUniformInputWidth8)
{
	width = 8;
	
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 1);
	inLineG = inputGen.GetLines(width, 1, 1);
	inLineB = inputGen.GetLines(width, 1, 1);
	
	outLineC_U = inputGen.GetEmptyLines(width/2, 1);
	outLineC_V = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_U = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_V = inputGen.GetEmptyLines(width/2, 1);

	
	inLineR[0][0] = 95;     inLineG[0][0]  =  198;    inLineB[0][0]  = 15; 
	inLineR[0][1] = 147;    inLineG[0][1]  =  107;    inLineB[0][1]  = 167; 
	inLineR[0][2] = 55;     inLineG[0][2]  =  100;    inLineB[0][2]  = 191; 
	inLineR[0][3] = 210;    inLineG[0][3]  =  39;     inLineB[0][3]  = 217; 
	inLineR[0][4] = 104;    inLineG[0][4]  =  19;     inLineB[0][4]  = 127; 
	inLineR[0][5] = 16;     inLineG[0][5]  =  73;     inLineB[0][5]  = 249; 
	inLineR[0][6] = 231;      inLineG[0][6]  =  2;    inLineB[0][6]  = 19; 
	inLineR[0][7] = 249;    inLineG[0][7]  =  57;    inLineB[0][7]  = 9; 
	
	mvcvCvtColorRGBtoUV420(inLineR, inLineG, inLineB, outLineC_U, outLineC_V, width);	
	cvtColorRGBtoUV420_asm_test(inLineR, inLineG, inLineB, outLineAsm_U, outLineAsm_V, width);	
	
	outputCheck.CompareArrays(outLineC_U[0], outLineAsm_U[0], width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V[0], outLineAsm_V[0], width/2, DELTA);

}

TEST_F(cvtColorRGBtoUV420Test, TestUniformInputWidth16Input0)
{
	width =16;
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 0);
	inLineG = inputGen.GetLines(width, 1, 0);
	inLineB = inputGen.GetLines(width, 1, 0);
	
	outLineC_U = inputGen.GetEmptyLines(width/2, 1);
	outLineC_V = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_U = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_V = inputGen.GetEmptyLines(width/2, 1);

	
	inLineR[0][0] = 95;     inLineG[0][0]  =  198;    inLineB[0][0]  = 15; 
	inLineR[0][1] = 147;    inLineG[0][1]  =  107;    inLineB[0][1]  = 167; 
	inLineR[0][2] = 55;     inLineG[0][2]  =  100;    inLineB[0][2]  = 191; 
	inLineR[0][3] = 210;    inLineG[0][3]  =  39;     inLineB[0][3]  = 217; 
	inLineR[0][4] = 104;    inLineG[0][4]  =  19;     inLineB[0][4]  = 127; 
	inLineR[0][5] = 16;     inLineG[0][5]  =  73;     inLineB[0][5]  = 249; 
	inLineR[0][6] = 231;    inLineG[0][6]  =  2;      inLineB[0][6]  = 19; 
	inLineR[0][7] = 249;    inLineG[0][7]  =  57;     inLineB[0][7]  = 9; 
	
	mvcvCvtColorRGBtoUV420(inLineR, inLineG, inLineB, outLineC_U, outLineC_V, width);	
	cvtColorRGBtoUV420_asm_test(inLineR, inLineG, inLineB, outLineAsm_U, outLineAsm_V, width);	
	
	outputCheck.CompareArrays(outLineC_U[0], outLineAsm_U[0], width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V[0], outLineAsm_V[0], width/2, DELTA);

}

TEST_F(cvtColorRGBtoUV420Test, TestUniformInputWidth64Input255)
{
	width = 64;
	
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 1);
	inLineG = inputGen.GetLines(width, 1, 1);
	inLineB = inputGen.GetLines(width, 1, 1);
	
	outLineC_U = inputGen.GetEmptyLines(width/2, 1);
	outLineC_V = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_U = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_V = inputGen.GetEmptyLines(width/2, 1);

	inLineR[0][0] = 95;     inLineG[0][0]  =  198;    inLineB[0][0]  = 15; 
	inLineR[0][1] = 147;    inLineG[0][1]  =  107;    inLineB[0][1]  = 167; 
	inLineR[0][2] = 55;     inLineG[0][2]  =  100;    inLineB[0][2]  = 191; 
	inLineR[0][3] = 210;    inLineG[0][3]  =  39;     inLineB[0][3]  = 217; 
	inLineR[0][4] = 104;    inLineG[0][4]  =  19;     inLineB[0][4]  = 127; 
	inLineR[0][5] = 16;     inLineG[0][5]  =  73;     inLineB[0][5]  = 249; 
	inLineR[0][6] = 231;    inLineG[0][6]  =  2;      inLineB[0][6]  = 19; 
	inLineR[0][7] = 249;    inLineG[0][7]  =  57;     inLineB[0][7]  = 9; 
	
	mvcvCvtColorRGBtoUV420(inLineR, inLineG, inLineB, outLineC_U, outLineC_V, width);	
	cvtColorRGBtoUV420_asm_test(inLineR, inLineG, inLineB, outLineAsm_U, outLineAsm_V, width);	
	
	outputCheck.CompareArrays(outLineC_U[0], outLineAsm_U[0], width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V[0], outLineAsm_V[0], width/2, DELTA);

}
  
TEST_F(cvtColorRGBtoUV420Test, TestUniformInputWidth8ExpectedValue)
{
	width = 8;
	
	
	inputGen.SelectGenerator("uniform");
	inLineR = inputGen.GetLines(width, 1, 1);
	inLineG = inputGen.GetLines(width, 1, 1);
	inLineB = inputGen.GetLines(width, 1, 1);
	
	outLineC_U = inputGen.GetEmptyLines(width/2, 1);
	outLineC_V = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_U = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_V = inputGen.GetEmptyLines(width/2, 1);
		
	mvcvCvtColorRGBtoUV420(inLineR, inLineG, inLineB, outLineC_U, outLineC_V, width);	
	cvtColorRGBtoUV420_asm_test(inLineR, inLineG, inLineB, outLineAsm_U, outLineAsm_V, width);	
	
	outputCheck.CompareArrays(outLineC_U[0], outLineAsm_U[0], width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V[0], outLineAsm_V[0], width/2, DELTA);

}
  
  
  
TEST_F(cvtColorRGBtoUV420Test, TestUniformInputWidth1920)
{
	width = 1920;
	
	
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC_U = inputGen.GetEmptyLines(width/2, 1);
	outLineC_V = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_U = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_V = inputGen.GetEmptyLines(width/2, 1);

	mvcvCvtColorRGBtoUV420(inLineR, inLineG, inLineB, outLineC_U, outLineC_V, width);	
	cvtColorRGBtoUV420_asm_test(inLineR, inLineG, inLineB, outLineAsm_U, outLineAsm_V, width);	
	
	outputCheck.CompareArrays(outLineC_U[0], outLineAsm_U[0], width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V[0], outLineAsm_V[0], width/2, DELTA);
		
	//outputChecker.DumpArrays("refInput", "refInputR.c", inLineR, width, 1);
	//outputChecker.DumpArrays("refInput", "refInputG.c", inLineG, width, 1);
	//outputChecker.DumpArrays("refInput", "refInputB.c", inLineB, width, 1);
} 

INSTANTIATE_TEST_CASE_P(RandomInputs, cvtColorRGBtoUV420Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(cvtColorRGBtoUV420Test , TestRandomData)
{
	
	inputGen.SelectGenerator("random");
	width = GetParam();
	
	inLineR = inputGen.GetLines(width, 1, 0, 255);
	inLineG = inputGen.GetLines(width, 1, 0, 255);
	inLineB = inputGen.GetLines(width, 1, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC_U = inputGen.GetEmptyLines(width/2, 1);
	outLineC_V = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_U = inputGen.GetEmptyLines(width/2, 1);
	outLineAsm_V = inputGen.GetEmptyLines(width/2, 1);
	 

	mvcvCvtColorRGBtoUV420(inLineR, inLineG, inLineB, outLineC_U, outLineC_V, width);	
	cvtColorRGBtoUV420_asm_test(inLineR, inLineG, inLineB, outLineAsm_U, outLineAsm_V, width);	
	
	
	outputCheck.CompareArrays(outLineC_U[0], outLineAsm_U[0], width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V[0], outLineAsm_V[0], width/2, DELTA);
}