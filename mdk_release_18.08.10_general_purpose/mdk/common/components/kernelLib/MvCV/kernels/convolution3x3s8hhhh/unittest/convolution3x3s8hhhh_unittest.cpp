//convolution3x3s8hhhh kernel test
//Asm function prototype:
//     void Convolution3x3s8hhhh_asm(half** in, half** out, half conv[9], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution3x3s8hhhh_asm(half** in, half** out, half conv[9], unsigned int inWidth);
//
//C function prototype:
//     void Convolution3x3s8hhhh(half** in, half** out, half conv[9], u32 inWidth);
//
// This kernel performs a convolution on the input image using the given 3x3 matrix
// stride = 8(s2); input data type = half(h); coefficient data type = half(h); computation/accumulation datatype = half(h); output data type = half(h);
// @param[in] in         - Input lines, 16-bits floating point
// @param[in] out        - Output line, 16-bits floating point
// @param[in] conv       - 9 elements array with fp16 values containing the 3x3 convolution matrix
// @param[in] inWidth    - Width of input line

#include "gtest/gtest.h"
///
#include "convolution3x3s8hhhh.h"
#include "convolution3x3s8hhhh_asm_test.h"
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
half DELTA =4; //accepted tolerance between C and ASM results, as we give at input big values we expect some differences between .c and .asm that are greater then 2 at 
// higher values
#define SIZE 3

class Convolution3x3s8hhhhKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	//half convMatAsm[9];
	half convAsm[9];
	half convC[9];
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


TEST_F(Convolution3x3s8hhhhKernelTest, TestRandomInputLinesRandomConvMatrix)
{
	width = randGen->GenerateUInt(0, 1921, 8);
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	
	half **inLinesOffseted;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING * 2, 3, 0.0, 255.0);
	inLinesOffseted = inputGen.GetOffsettedLines(inLines, 7, 8);

	half    convMat[9];
	inputGen.FillLine(convMat, 9, 0.01f, 0.03f);

	mvcvConvolution3x3s8hhhh(inLinesOffseted, outLinesC, convMat, width);
	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);

	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width + PADDING * 2, 7);
	//half* mat = convMat;
	//half** mat1 = &mat;
	//outputChecker.DumpArrays("matInput", "matInput.c", mat1, 9 , 1);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", outLinesC, width , 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", outLinesAsm, width , 1);
}


TEST_F(Convolution3x3s8hhhhKernelTest, TestExperimental)
{
	width = 1920;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0, 255);

	half   convMatC[9] = {8.0 / 9, 9.0 / 9, 0.0 / 9, 55.0 / 9, 5.0 / 9, -1.0 / 9, 0.0 / 9, 9.0 / 9, 0.0 / 9};
	half  convMatAsm[9] = {8.0 / 9, 9.0 / 9, 0.0 / 9, 55.0 / 9, 5.0 / 9, -1.0 / 9, 0.0 / 9, 9.0 / 9, 0.0 / 9};

	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(Convolution3x3s8hhhhKernelTest, TestUniformInputLinesWidth16)
{
	width = 17;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 3);

	inputGen.FillLine(convC, 9, 1.0f / 9);
	inputGen.FillLine(convAsm, 9, 1.0f / 9);
	
	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convC, width);
	
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}


TEST_F(Convolution3x3s8hhhhKernelTest, TestUniformInputLinesAllValues255)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 255);


	half   convMatC[9] = {1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9};
	half  convMatAsm[9] = {1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9, 1.0 / 9}; //emboss

	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convMatC, width);
	
	//outputChecker.ExpectAllInRange<half>(outLinesAsm[0], width, 254, 255);
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

//------------ test different convolution matrices ------------------------

TEST_F(Convolution3x3s8hhhhKernelTest, TestRandomLinesSobel)
{
	width =64;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0, 255);


	half   convMatC[9] = {-1.0 / 9, 0.0 / 9, 1.0 / 9, -2.0 / 9, 0.0 / 9, 2.0 / 9, -1.0 / 9, 0.0 / 9, 1.0 / 9};
	half  convMatAsm[9] = {-1.0 / 9, 0.0 / 9, 1.0 / 9, -2.0 / 9, 0.0 / 9, 2.0 / 9, -1.0 / 9, 0.0 / 9, 1.0 / 9};

	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(Convolution3x3s8hhhhKernelTest, TestRandomLinesEdgeDetect)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0, 255);


	half   convMatC[9] = {0.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, -4.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 0.0 / 9};
	half  convMatAsm[9] = {0.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, -4.0 / 9, 1.0 / 9, 0.0 / 9, 1.0 / 9, 0.0 / 9};

	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(Convolution3x3s8hhhhKernelTest, TestRandomLinesSharpen)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0, 255);


	half   convMatC[9] = {0.0 / 9, -1.0 / 9, 0.0 / 9, -1.0 / 9, 5.0 / 9, -1.0 / 9, 0.0 / 9, -1.0 / 9, 0.0 / 9};
	half  convMatAsm[9] = {0.0 / 9, -1.0 / 9, 0.0 / 9, -1.0 / 9, 5.0 / 9, -1.0 / 9, 0.0 / 9, -1.0 / 9, 0.0 / 9};

	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

TEST_F(Convolution3x3s8hhhhKernelTest, TestRandomLinesRandomKernel)
{
	width = 640;
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0, 255);


	half   convMatC[9];
	half  convMatAsm[9];

	for(int i = 0; i < 9; i++)
	{
		convMatC[i] = randGen->GenerateFloat16(-20, 20) / 9;
		convMatAsm[i] = (half)convMatC[i];

	}

	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}

//------------ parameterized tests ----------------------------------------

INSTANTIATE_TEST_CASE_P(UniformInputs, Convolution3x3s8hhhhKernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution3x3s8hhhhKernelTest, TestRandomInputLinesEmbossFilter)
{
	half   convMatC[9] = {-18.0 / 9, -9.0 / 9, 0.0 / 9, -9.0 / 9, 9.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 18.0 / 9};
	half  convMatAsm[9] = {-18.0 / 9, -9.0 / 9, 0.0 / 9, -9.0 / 9, 9.0 / 9, 9.0 / 9, 0.0 / 9, 9.0 / 9, 18.0 / 9};

	width = GetParam();
	inputGen.SelectGenerator("uniform");
	outLinesC = (half **)inputGen.GetLines(width*2, 1,1);
	outLinesAsm = (half **)inputGen.GetLines(width*2,1,1);
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 3, 0, 255);


	Convolution3x3s8hhhh_asm_test(inLines, outLinesAsm, convMatAsm, width);
	RecordProperty("CyclePerPixel", convolution3x3s8hhhhCycleCount / width);

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution3x3s8hhhh(inLines, outLinesC, convMatC, width);
	
	outputChecker.CompareArrays(outLinesC, outLinesAsm, width, 1, DELTA);
}
