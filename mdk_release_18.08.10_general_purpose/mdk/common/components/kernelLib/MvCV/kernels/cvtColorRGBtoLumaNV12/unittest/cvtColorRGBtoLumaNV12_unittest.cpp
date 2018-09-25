//cvtColorKernelYUV422ToRGB kernel test

//Asm function prototype:
//	void cvtColorRGBtoLuma_asm

//Asm test function prototype:
//    	void cvtColorRGBtoLuma_asm_test

//C function prototype:
//  	void cvtColorRGBtoLuma

#include "gtest/gtest.h"
#include "cvtColorRGBtoLumaNV12.h"
#include "cvtColorRGBtoLumaNV12_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

const unsigned short DELTA = 1;	
const unsigned short EXCEEDED_BOUNDS = 16;	

using ::testing::TestWithParam;
using ::testing::Values;

class cvtColorRGBtoLumaNV12Test : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char* inLineR;
	unsigned char* inLineG;
	unsigned char* inLineB;
	unsigned char* outLineC_Y;
	unsigned char* outLineAsm_Y;
	unsigned char* outExpected;


	unsigned int width;
	float mat[3*3];
	float offset[1*3];

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	
	
	virtual void TearDown() {}
};

TEST_F(cvtColorRGBtoLumaNV12Test, TestInput0MaxWidth)
{
	width = 56;

	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLine(width, 0, 5);
	inLineG = inputGen.GetLine(width, 0, 5);
	inLineB = inputGen.GetLine(width, 0, 5);
	
	mat[0] = randGen->GenerateFloat(0.0f, 1.0f);
	mat[1] = randGen->GenerateFloat(0.0f, 1.0f);
	mat[2] = randGen->GenerateFloat(0.0f, 1.0f);
	offset[0] = (randGen->GenerateFloat(0.0f, 255.0f))/255.0f;
	
	inputGen.SelectGenerator("uniform");
	outLineC_Y = inputGen.GetLine(width + 32, 0);
	outLineAsm_Y = inputGen.GetLine(width + 32, 0);
	
	mvcvCvtColorRGBtoLumaNV12(inLineR, inLineG, inLineB, outLineC_Y, mat, offset, width);	
	cvtColorRGBtoLumaNV12_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, mat, offset, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width + 32, DELTA);
	
	// outputCheck.DumpArrays("inlineR", "inLineR.c", inLineR, width);	
	// outputCheck.DumpArrays("inlineG", "inLineG.c", inLineG, width);	
	// outputCheck.DumpArrays("inlineB", "inLineB.c", inLineB, width);	
	
	// printf("mat[0]=%f\n",mat[0]);
	// printf("mat[1]=%f\n",mat[1]);
	// printf("mat[2]=%f\n",mat[2]);
	// printf("offset[0]=%f\n",offset[0]);
	

	// for (int i=0; i<width+32; i++)
	// printf("outC=%d   outAsm=%d\n", outLineC_Y[i], outLineAsm_Y[i]);
	
	
}


TEST_F(cvtColorRGBtoLumaNV12Test, TestRandomInputMaxWidth)
{
	width = 1920;

	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLine(width, 0, 255);
	inLineG = inputGen.GetLine(width, 0, 255);
	inLineB = inputGen.GetLine(width, 0, 255);
	
	mat[0] = randGen->GenerateFloat(0.0f, 1.0f);
	mat[1] = randGen->GenerateFloat(0.0f, 1.0f);
	mat[2] = randGen->GenerateFloat(0.0f, 1.0f);
	offset[0] = (randGen->GenerateFloat(0.0f, 255.0f))/255.0f;
	
	inputGen.SelectGenerator("uniform");
	outLineC_Y = inputGen.GetLine(width + 32, 0);
	outLineAsm_Y = inputGen.GetLine(width + 32, 0);
		
	mvcvCvtColorRGBtoLumaNV12(inLineR, inLineG, inLineB, outLineC_Y, mat, offset, width);	
	cvtColorRGBtoLumaNV12_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, mat, offset, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width + 32, DELTA);
	
}


INSTANTIATE_TEST_CASE_P(RandomInputs, cvtColorRGBtoLumaNV12Test,
		Values((unsigned int)16, (unsigned int)40, (unsigned int)56, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1288, (unsigned int)1448, (unsigned int)1920);
);

TEST_P(cvtColorRGBtoLumaNV12Test , TestRandomData)
{
		
	inputGen.SelectGenerator("random");
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLineR = inputGen.GetLine(width, 0, 255);
	inLineG = inputGen.GetLine(width, 0, 255);
	inLineB = inputGen.GetLine(width, 0, 255);
	
	mat[0] = randGen->GenerateFloat(0.0f, 1.0f);
	mat[1] = randGen->GenerateFloat(0.0f, 1.0f);
	mat[2] = randGen->GenerateFloat(0.0f, 1.0f);
	offset[0] = (randGen->GenerateFloat(0.0f, 255.0f))/255.0f;
	
	inputGen.SelectGenerator("uniform");
	outLineC_Y = inputGen.GetLine(width + 32, 0);
	outLineAsm_Y = inputGen.GetLine(width + 32, 0);
		
	mvcvCvtColorRGBtoLumaNV12(inLineR, inLineG, inLineB, outLineC_Y, mat, offset, width);	
	cvtColorRGBtoLumaNV12_asm_test(inLineR, inLineG, inLineB, outLineAsm_Y, mat, offset, width);	
	
	outputCheck.CompareArrays(outLineC_Y, outLineAsm_Y, width + 32, DELTA);
	

}

