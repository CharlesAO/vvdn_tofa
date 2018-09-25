//cvtColorKernelRGBtoChromaNV12 kernel test

//Asm function prototype:
//	void cvtColorRGBtoChromaNV12_asm

//Asm test function prototype:
//    	void cvtColorRGBtoChromaNV12_asm_test

//C function prototype:
//  	void mavcvcvtColorRGBtoChromaNV12


#include "gtest/gtest.h"
#include "cvtColorRGBtoChromaNV12.h"
///
#include "cvtColorRGBtoChromaNV12_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

const unsigned short DELTA = 3;	

using ::testing::TestWithParam;
using ::testing::Values;


class cvtColorRGBtoChromaNV12Test : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLine1RGB;
	unsigned char** inLine2RGB;
	
	unsigned char* outLineC_UV;
	unsigned char* outLineAsm_UV;
	unsigned char* outExpected_UV;

	unsigned int width;
	float mat[3*3];
	float offset[1*3];
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	ArrayChecker outputChecker;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(cvtColorRGBtoChromaNV12Test, TestRandomInputWidth64Input255)
{
	width = 40;
	
	
	inputGen.SelectGenerator("random");
	inLine1RGB = inputGen.GetLines(width, 3, 0, 255);
	inLine2RGB = inputGen.GetLines(width, 3, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	outLineC_UV = inputGen.GetLine(width + 32,0);
	outLineAsm_UV = inputGen.GetLine(width + 32,0);
	
	 mat[3] = randGen->GenerateFloat(-1.0f, 1.0f);
	 mat[4] = randGen->GenerateFloat(-1.0f, 1.0f);
	 mat[5] = randGen->GenerateFloat(-1.0f, 1.0f);
	 mat[6] = randGen->GenerateFloat(-1.0f, 1.0f);
	 mat[7] = randGen->GenerateFloat(-1.0f, 1.0f);
	 mat[8] = randGen->GenerateFloat(-1.0f, 1.0f);
	
	 offset[1] = randGen->GenerateFloat(0.0f, 255.0f) / 255.0f;
	 offset[2] = randGen->GenerateFloat(0.0f, 255.0f) / 255.0f;
	
	
	mvcvCvtColorRGBtoChromaNV12(inLine1RGB, inLine2RGB, outLineC_UV, mat, offset, width);	
	cvtColorRGBtoChromaNV12_asm_test(inLine1RGB, inLine2RGB, outLineAsm_UV, mat, offset, width);	
	
	outputCheck.CompareArrays(outLineC_UV, outLineAsm_UV, width + 32, DELTA);
	
}
  
INSTANTIATE_TEST_CASE_P(RandomInputs, cvtColorRGBtoChromaNV12Test,
		Values( (unsigned int)32, (unsigned int)40, (unsigned int)48, (unsigned int)56, (unsigned int)64, (unsigned int)320, (unsigned int)640, (unsigned int)832, (unsigned int)848, (unsigned int)960, (unsigned int)1288, (unsigned int)1296, (unsigned int)1304, (unsigned int)1920);
);

TEST_P(cvtColorRGBtoChromaNV12Test , TestRandomData)
{
		
	inputGen.SelectGenerator("random");
	width = GetParam();
	inLine1RGB = inputGen.GetLines(width, 3, 0, 255);
	inLine2RGB = inputGen.GetLines(width, 3, 0, 255);
	
	mat[3] = randGen->GenerateFloat(-1.0f, 1.0f);
	mat[4] = randGen->GenerateFloat(-1.0f, 1.0f);
	mat[5] = randGen->GenerateFloat(-1.0f, 1.0f);
	mat[6] = randGen->GenerateFloat(-1.0f, 1.0f);
	mat[7] = randGen->GenerateFloat(-1.0f, 1.0f);
	mat[8] = randGen->GenerateFloat(-1.0f, 1.0f);
	
	offset[1] = randGen->GenerateFloat(0.0f, 255.0f) / 255.0f;
	offset[2] = randGen->GenerateFloat(0.0f, 255.0f) / 255.0f;
		
	inputGen.SelectGenerator("uniform");
	outLineC_UV = inputGen.GetLine(width + 32,0);
	outLineAsm_UV = inputGen.GetLine(width + 32,0);
	 
	mvcvCvtColorRGBtoChromaNV12(inLine1RGB, inLine2RGB, outLineC_UV, mat, offset, width);	
	cvtColorRGBtoChromaNV12_asm_test(inLine1RGB, inLine2RGB, outLineAsm_UV, mat, offset, width);
	
	outputCheck.CompareArrays(outLineC_UV, outLineAsm_UV, width + 32, DELTA);
	
}