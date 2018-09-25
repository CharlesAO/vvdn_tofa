//convolution5x5Fp16ToFp16 kernel test
//Asm function prototype:
//     void Convolution5x5Fp16ToFp16_asm(u8** in, u8** out, half conv[][3], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution5x5Fp16ToFp16_asm(unsigned char** in, unsigned char** out, half conv[][3], unsigned int inWidth);
//
//C function prototype:
//     void Convolution5x5Fp16ToFp16(u8** in, u8** out, float conv[][3], u32 inWidth);
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
#include "convolution5x5Fp16ToFp16.h"
#include "convolution5x5Fp16ToFp16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

using ::testing::TestWithParam;
using ::testing::Values;


#define PADDING 16
half DELTA=1; //accepted tolerance between C and ASM results
#define SIZE 5

class Convolution5x5Fp16ToFp16KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}


	

	half convMat[25];
	
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


TEST_F(Convolution5x5Fp16ToFp16KernelTest, TestUniformInputLinesWidth16)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines =inputGen.GetLinesFloat16 (width + PADDING, 5, 3);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	inputGen.FillLine(convMat, 25, 0.04f);
	
	//mvcvConvolution5x5Fp16ToFp16(inLines, outLinesAsm, convMat, width);
	Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5Fp16ToFp16(inLines, outLinesC, convMat, width);
	
	
	outputChecker.ExpectAllInRange<half>(outLinesAsm[0], width, 2, 3);

	outputChecker.CompareArrays(outLinesC,outLinesAsm, width,1, DELTA);

	
} 

TEST_F(Convolution5x5Fp16ToFp16KernelTest, TestMinimumWidth)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 5, 7);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);
	inputGen.FillLine(convMat, 25, 0.04f);

	//mvcvConvolution5x5Fp16ToFp16(inLines,outLinesAsm, convMat, width);
	Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5Fp16ToFp16(inLines,outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC,outLinesAsm, width,1, DELTA);
}

TEST_F(Convolution5x5Fp16ToFp16KernelTest, TestSobelFilter)
{
	width = 80;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 5, 0.0, 255.0);
	//outputChecker.DumpArrays("refInput","refInput.c",inLines,width+PADDING,5);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	half convMat[25] = { 2.0 / 25.0, 1.0 / 25.0, 0.0 / 25.0, -1.0 / 25.0, -2.0 / 25.0,
			             3.0 / 25.0, 2.0 / 25.0, 0.0 / 25.0, -2.0 / 25.0, -3.0 / 25.0,
			             4.0 / 25.0, 3.0 / 25.0, 0.0 / 25.0, -3.0 / 25.0, -4.0 / 25.0,
			             3.0 / 25.0, 2.0 / 25.0, 0.0 / 25.0, -2.0 / 25.0, -3.0 / 25.0,
			    	     2.0 / 25.0, 1.0 / 25.0, 0.0 / 25.0, -1.0 / 25.0, -2.0 / 25.0 };
	
	//mvcvConvolution5x5Fp16ToFp16(inLines, outLinesAsm, convMat, width);

	Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5Fp16ToFp16CycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5Fp16ToFp16(inLines, outLinesC, convMat, width);	
	
	outputChecker.CompareArrays(outLinesC,outLinesAsm, width,1, DELTA);

	//outputChecker.DumpArrays("refOutputAsm", "refOutputAsm.c", outLinesAsm, width,1 );
	//outputChecker.DumpArrays("refOutputC", "refOutputC.c", outLinesC, width,1);


	//outputChecker.DumpArrays("")
}

TEST_F(Convolution5x5Fp16ToFp16KernelTest, TestEmbossFilter)
{
	width = 320;
	inputGen.SelectGenerator("random");
	inLines =  inputGen.GetLinesFloat16(width + PADDING, 5, 0, 255);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	half convMat[25] = { -1.0 / 25.0, -1.0 / 25.0, -1.0 / 25.0, -1.0 / 25.0, 0.0 / 25.0,
			            -1.0 / 25.0, -1.0 / 25.0, -1.0 / 25.0,  0.0 / 25.0, 1.0 / 25.0,
			            -1.0 / 25.0, -1.0 / 25.0,  0.0 / 25.0,  1.0 / 25.0, 1.0 / 25.0,
			            -1.0 / 25.0,  0.0 / 25.0,  1.0 / 25.0,  1.0 / 25.0, 1.0 / 25.0,
			    	     0.0 / 25.0,  1.0 / 25.0,  1.0 / 25.0,  1.0 / 25.0, 1.0 / 25.0 };
	//mvcvConvolution5x5Fp16ToFp16(inLines, outLinesAsm, convMat, width);
	Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5Fp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5Fp16ToFp16(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC,outLinesAsm, width,1, DELTA);
}

//------------ parameterized tests ----------------------------------------


INSTANTIATE_TEST_CASE_P(RandomInputs, Convolution5x5Fp16ToFp16KernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution5x5Fp16ToFp16KernelTest, TestRandomInputLinesBlurFilter)
{

	unsigned int width = GetParam();

	inputGen.SelectGenerator("uniform");

	inputGen.FillLine(convMat, 25, 0.04f);
	inputGen.SelectGenerator("random");
	inLines =  inputGen.GetLinesFloat16(width + PADDING, 5, 0, 255);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	//mvcvConvolution5x5Fp16ToFp16(inLines, outLinesAsm, convMat, width);
	Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5Fp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5Fp16ToFp16(inLines, outLinesC, convMat, width);
	//Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesC, convMat, width);
	outputChecker.CompareArrays(outLinesC,outLinesAsm, width,1, DELTA);

}

TEST_P(Convolution5x5Fp16ToFp16KernelTest, TestRandomInputLinesRandomMatrix)
{
	unsigned int width = GetParam();
	inputGen.SelectGenerator("random");

	inputGen.FillLine(convMat, 25, 0.0f, 1.0f);
		
	inputGen.SelectGenerator("random");
	inLines =  inputGen.GetLinesFloat16(width + PADDING, 5, 0.0, 1.0f);
	outLinesC = inputGen.GetEmptyLinesFp16(width, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width, 1);

	//mvcvConvolution5x5Fp16ToFp16(inLines, outLinesAsm, convMat, width);
	Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5Fp16ToFp16CycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5Fp16ToFp16(inLines, outLinesC, convMat, width);
	//Convolution5x5Fp16ToFp16_asm_test(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC,outLinesAsm, width,1, DELTA);
}

