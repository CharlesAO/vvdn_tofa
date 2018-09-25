//convolution7x7 kernel test
//Asm function prototype:
//     void Convolution7x7s8hhhh_asm(half** in, half** out, half conv[49], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution7x7s8hhhh_asm(half** in, half** out, half conv[49], unsigned int inWidth);
//
//C function prototype:
//     void Convolution7x7s8hhhh(half** in, half** out, half conv[49], u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - array of pointers to output lines
// conv       - array of values from convolution
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "convolution7x7s8hhhh.h"
#include "convolution7x7s8hhhh_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;


#define PADDING 8
half DELTA = 3;//accepted tolerance between C and ASM results

class Convolution7x7KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half convMatAsm[49];
	half **inLines;
	half **outLinesC;
	half **outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(Convolution7x7KernelTest, TestRandomInputLinesRandomConvMatrix)
{
	width = randGen->GenerateUInt(1280, 1921, 8);
	half **inLinesOffseted;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2, 1,1);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 0.0, 255.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);


	half    convMat[49];
	inputGen.FillLine(convMat, 49, 0.01f, 0.03f);

	mvcvConvolution7x7s8hhhh(inLinesOffseted, outLinesC, convMat, width);
	Convolution7x7s8hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x7s8hhhhCycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);

	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 7);
	//half* mat = convMat;
	//half** mat1 = &mat;
	//outputChecker.DumpArrays("matInput", "matInput.c", mat1, 49 , 1);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", outLinesC, width , 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", outLinesAsm, width , 1);
}

