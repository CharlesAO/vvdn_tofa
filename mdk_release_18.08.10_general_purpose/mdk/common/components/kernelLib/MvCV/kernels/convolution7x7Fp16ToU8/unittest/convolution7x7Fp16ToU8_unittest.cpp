//convolution7x7 kernel test
//Asm function prototype:
//     void Convolution7x7Fp16ToU8_asm(half** in, u8** out, half conv[49], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution7x7Fp16ToU8_asm(half** in, u8** out, half conv[49], unsigned int inWidth);
//
//C function prototype:
//     void Convolution7x7Fp16ToU8(half** in, u8** out, half conv[49], u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines - fp16 in [0,1) interval
// out        - array of pointers to output lines - u8 values [0, 255]
// conv       - array of values from convolution
// inWidth    - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
///
#include "convolution7x7Fp16ToU8.h"
#include "convolution7x7Fp16ToU8_asm_test.h"
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
half DELTA = 2;//accepted tolerance between C and ASM results

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
	u8 **outLinesC;
	u8 **outLinesAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(Convolution7x7KernelTest, TestRandomInputLinesRandomConvMatrix)
{
	width = randGen->GenerateUInt(0, 1921, 8);
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 7, 0.0, 1.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);
	outLinesC = inputGen.GetEmptyLines(width, 1);
	outLinesAsm = inputGen.GetEmptyLines(width + PADDING, 1);

	half    convMat[49];
	inputGen.FillLine(convMat, 49, 0.01f, 0.03f);

	mvcvConvolution7x7Fp16ToU8(inLinesOffseted, outLinesC, convMat, width);
	Convolution7x7Fp16ToU8_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x7Fp16ToU8CycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);

	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 7);
	//half* mat = convMat;
	//half** mat1 = &mat;
	//outputChecker.DumpArrays("matInput", "matInput.c", mat1, 49 , 1);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", outLinesC, width , 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", outLinesAsm, width , 1);
}

