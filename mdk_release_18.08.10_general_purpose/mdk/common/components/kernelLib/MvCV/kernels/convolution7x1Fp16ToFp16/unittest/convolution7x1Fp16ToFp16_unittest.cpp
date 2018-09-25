//convolution7x1Fp16ToFp16 kernel test
//Asm function prototype:
//     void convolution7x1Fp16ToFp16_asm(half** in, u8* out, half* conv, u32 inWidth);
//
//Asm test function prototype:
//     void convolution7x1Fp16ToFp16_asm_test(half** in, half* out, half* conv, unsigned int inWidth);
//
//C function prototype:
//     void convolution7x1Fp16ToFp16(half** in, half* out, half* conv, u32 inWidth);
//
//Parameter description:
// in         - array of pointers to input lines
// out        - pointer to the output line
// conv       - array of values from convolution
// inWidth    - width of input line


#include "gtest/gtest.h"
///
#include "convolution7x1Fp16ToFp16_asm_test.h"
#include "convolution7x1Fp16ToFp16.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;

#define KERNEL_SIZE 7
half DELTA = 2; //accepted tolerance between C and ASM results


class convolution7x1Fp16ToFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	half **inLines;
	half *outLineC;
	half *outLineAsm;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(convolution7x1Fp16ToFp16KernelTest, TestUniformInput)
{
	width = 32;	
	inputGen.SelectGenerator("uniform");	
	inLines 	= inputGen.GetLinesFloat16(width, KERNEL_SIZE, 10.0f);
	outLineC 	= inputGen.GetEmptyLineFp16(width);	
	outLineAsm 	= inputGen.GetEmptyLineFp16(width);
	half* convMat = inputGen.GetEmptyLineFp16(KERNEL_SIZE);
	
	convMat[0] = (half) 0.02;
	convMat[1] = (half) 0.08;
	convMat[2] = (half) 0.06;
	convMat[3] = (half) 0.5;
	convMat[4] = (half) 0.04;
	convMat[5] = (half) 0.1;
	convMat[6] = (half) 0.2;
	
	mvcvConvolution7x1Fp16ToFp16(inLines, outLineC, convMat, width);
	convolution7x1Fp16ToFp16_asm_test(inLines, outLineAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1Fp16ToFp16CycleCount / width);
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}

TEST_F(convolution7x1Fp16ToFp16KernelTest, TestRandomInputMinWidth)
{
	width = 8;	
	inputGen.SelectGenerator("uniform");	
	outLineC 	= inputGen.GetEmptyLineFp16(width);	
	outLineAsm 	= inputGen.GetEmptyLineFp16(width);
	half* convMat = inputGen.GetLineFloat16(KERNEL_SIZE, 0.1f);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width, KERNEL_SIZE, 0.0f, 255.0f);	

	convMat[3] = (half) 0.4;
	
	mvcvConvolution7x1Fp16ToFp16(inLines, outLineC, convMat, width);
	convolution7x1Fp16ToFp16_asm_test(inLines, outLineAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1Fp16ToFp16CycleCount / width);	
	
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}

//------------ parameterized tests ----------------------------------------
INSTANTIATE_TEST_CASE_P(UniformInputs, convolution7x1Fp16ToFp16KernelTest,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(convolution7x1Fp16ToFp16KernelTest, TestRandomInputParameterizedWidth)
{
	unsigned int width = GetParam();
	inputGen.SelectGenerator("random");
	inLines 	= inputGen.GetLinesFloat16(width, KERNEL_SIZE,  0.0f, 255.0f);
	outLineC 	= inputGen.GetEmptyLineFp16(width);
	outLineAsm 	= inputGen.GetEmptyLineFp16(width);
	half* convMat = inputGen.GetLineFloat16(KERNEL_SIZE, 0.0f, 0.14f);
		
	mvcvConvolution7x1Fp16ToFp16(inLines, outLineC, convMat, width);
	convolution7x1Fp16ToFp16_asm_test(inLines, outLineAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution7x1Fp16ToFp16CycleCount / width);
		
	outputChecker.CompareArrays(outLineC, outLineAsm, width, DELTA);
}