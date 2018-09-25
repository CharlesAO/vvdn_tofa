//convolution1x7 kernel test
//Asm function prototype:
//     void Convolution1x7Fp16ToFp16_asm(half* in, half* out, half conv[7], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution1x7Fp16ToFp16_asm(half* in, half* out, half conv[7], unsigned int inWidth);
//
//C function prototype:
//     void mvcvConvolution1x7Fp16ToFp16(half* in, half* out, half conv[7], u32 inWidth);
//
//Parameter description:
// in         	- pointer to input line
// out        	- pointer output line
// conv       	- array of values from convolution
// inWidth  	- width of input line

#include "gtest/gtest.h"
///
#include "convolution1x7Fp16ToFp16_asm_test.h"
#include "convolution1x7Fp16ToFp16.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define MAT_SIZE 7
#define PADDING 16
half DELTA = 2;//accepted tolerance between C and ASM results


class Convolution1x7KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

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


TEST_F(Convolution1x7KernelTest, TestUniformInput)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLineFloat16(width + PADDING, 120.0);
	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);	
	half convMat[MAT_SIZE] = {0.02f, 0.02f, 0.06f, 0.8f, 0.06f, 0.02f, 0.02f};
	
	Convolution1x7Fp16ToFp16_asm_test(inLine, outLineAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7Fp16ToFp16CycleCount / width);
	mvcvConvolution1x7Fp16ToFp16(&inLine[8], outLineC, convMat, width);
		
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);

}

TEST_F(Convolution1x7KernelTest, TestRandomInputPadding0MinWidth)
{
	width = 1920;
	inputGen.SelectGenerator("uniform");
	inLine = inputGen.GetLineFloat16(width + PADDING, 0.0);
	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);
	half convMat[MAT_SIZE] = {0.02f, 0.02f, 0.06f, 0.8f, 0.06f, 0.02f, 0.02f};
	
	inputGen.SelectGenerator("random");
	half *aux;
	aux = inputGen.GetLineFloat16(width, 0.0, 255.0);	
	for(unsigned int i = 0; i < width; i++)
		inLine[i + PADDING/2] = aux[i];		
	
	Convolution1x7Fp16ToFp16_asm_test(inLine, outLineAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7Fp16ToFp16CycleCount / width);
	mvcvConvolution1x7Fp16ToFp16(&inLine[8], outLineC, convMat, width);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}


//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution1x7KernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution1x7KernelTest, TestRandomInputParameterizedWidth)
{
	width = GetParam();
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLineFloat16(width + PADDING, 0.0, 255.0);
	half *convMat = inputGen.GetLineFloat16(MAT_SIZE, 0.0, 0.14);
	inputGen.SelectGenerator("uniform");
	outLineC = inputGen.GetEmptyLineFp16(width);
	outLineAsm = inputGen.GetEmptyLineFp16(width);
	
	Convolution1x7Fp16ToFp16_asm_test(inLine, outLineAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution1x7Fp16ToFp16CycleCount / width);
	mvcvConvolution1x7Fp16ToFp16(&inLine[8], outLineC, convMat, width);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}