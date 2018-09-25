//genLumaFp16 test
//
// Test description:
//   generate luma from rgb input image
//

#include "gtest/gtest.h"
#include "genChromaFp16_asm_test.h"
#include <genChromaFp16.h>
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

unsigned char DELTA = 1;//accepted tolerance between C and ASM results

class appKernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned short **inRGB;
    half *inY;
	unsigned char **outLineC;
	unsigned char **outLineAsm;
    float eps;
    float normVal;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(appKernelTest, Test1)
{
	width = 16;
    normVal = 0.0033f;
    eps = 0.045f;
	inputGen.SelectGenerator("random");
	inRGB = inputGen.GetLinesU16(width, 3, 0, 16383);
    inY = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);
       

	outLineC = inputGen.GetEmptyLines(width, 3);
	outLineAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaFp16(outLineC, inRGB, inY, eps, normVal, width);
	genChromaFp16_asm_test(outLineAsm, inRGB, inY, eps, normVal, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, 3, DELTA);

    //outputChecker.DumpArrays("refInputRGB", "refInputRGB.c", inRGB, width, 3);
    //outputChecker.DumpArrays("refInputY", "refInputY.c", inY, width);
    //outputChecker.DumpArrays("refCOutput", "refCOutput.c", outLineC, width, 3);
    //outputChecker.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineAsm, width, 3);
}


TEST_F(appKernelTest, TestFullRandom)
{
    inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(0, 1921, 8);
    int inputBpp = randGen->GenerateUInt(8, 14, 1);
    normVal = (255.0/3.0)/((float)(1<<inputBpp)); 
    eps = randGen->GenerateFloat(0.0f, 0.1);

	inRGB = inputGen.GetLinesU16(width, 3, 0, (unsigned short)((1<<inputBpp)-1));
    inY = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);    
    
	outLineC = inputGen.GetEmptyLines(width, 3);
	outLineAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaFp16(outLineC, inRGB, inY, eps, normVal, width);
	genChromaFp16_asm_test(outLineAsm, inRGB, inY, eps, normVal, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, 3, DELTA);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, appKernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(appKernelTest, TestRandomInputLinesAllWidth)
{

    inputGen.SelectGenerator("random");
    width = GetParam();
    
    int inputBpp = randGen->GenerateUInt(8, 14, 1);
    normVal = (255.0/3.0)/((float)(1<<inputBpp)); 
    eps = randGen->GenerateFloat(0.0f, 0.1);

	inRGB = inputGen.GetLinesU16(width, 3, 0, (unsigned short)((1<<inputBpp)-1));
    inY = inputGen.GetLineFloat16(width, (half)0.0f, (half)1.0f);    
    
	outLineC = inputGen.GetEmptyLines(width, 3);
	outLineAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaFp16(outLineC, inRGB, inY, eps, normVal, width);
	genChromaFp16_asm_test(outLineAsm, inRGB, inY, eps, normVal, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, 3, DELTA);
}


