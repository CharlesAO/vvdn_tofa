//randNoiseFp16 test
//
// Test description:
//   generate a random noise pattern on a fp16 input data with 
//   a custom version of lfsr pseudo random generate algorithm
//

#include "gtest/gtest.h"
#include "randNoiseFp16_asm_test.h"
#include "randNoiseFp16.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

half DELTA = (half)0.001;//accepted tolerance between C and ASM results

class randNoiseFp16KernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	float noiseStrength;
	half *inLine;
	half *outLineC;
	half *outLineAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(randNoiseFp16KernelTest, Test1)
{
	width = 16;
    noiseStrength = 0.05f;
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLineFloat16(width, 0.0, 1.0);

	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);

	mvispRandNoiseFp16(outLineC, inLine, noiseStrength, width);
	randNoiseFp16_asm_test(outLineAsm, inLine, noiseStrength, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);

    //outputChecker.DumpArrays("refInput", "refInput.c", inLine, width);
    //outputChecker.DumpArrays("refCOutput", "refCOutput.c", outLineC, width);
    //outputChecker.DumpArrays("refAsmOutput", "refAsmOutput.c", outLineAsm, width);
}



TEST_F(randNoiseFp16KernelTest, TestFullRandom)
{
    inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(0, 1921, 8);
    noiseStrength = randGen->GenerateFloat(0.05f, 1.0f);
	inLine = inputGen.GetLineFloat16(width, 0.0, 1.0);

	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);

	mvispRandNoiseFp16(outLineC, inLine, noiseStrength, width);
	randNoiseFp16_asm_test(outLineAsm, inLine, noiseStrength, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}


//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, randNoiseFp16KernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(randNoiseFp16KernelTest, TestRandomInputLinesAllWidth)
{

    inputGen.SelectGenerator("random");
    width = GetParam();
    noiseStrength = randGen->GenerateFloat(0.05f, 1.0f);
	inLine = inputGen.GetLineFloat16(width, 0.0, 1.0);

	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);

	mvispRandNoiseFp16(outLineC, inLine, noiseStrength, width);
	randNoiseFp16_asm_test(outLineAsm, inLine, noiseStrength, width);
    
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}


