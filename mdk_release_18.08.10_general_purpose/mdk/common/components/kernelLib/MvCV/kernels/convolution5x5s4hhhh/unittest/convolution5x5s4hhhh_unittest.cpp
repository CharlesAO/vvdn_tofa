//convolution5x5s4hhhh kernel test
//Asm function prototype:
//     void Convolution5x5s4hhhh_asm(half** in, half** out, half conv[][3], u32 inWidth);
//
//Asm test function prototype:
//     void Convolution5x5s4hhhh_asm(half** in, half** out, half conv[][3], unsigned int inWidth);
//
//C function prototype:
//     void Convolution5x5s4hhhh(half** in, half** out, half conv[][3], u32 inWidth);
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
#include "convolution5x5s4hhhh.h"
#include "convolution5x5s4hhhh_asm_test.h"
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

class Convolution5x5s4hhhhKernelTest : public ::testing::TestWithParam< unsigned int > {
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


TEST_F(Convolution5x5s4hhhhKernelTest, TestUniformInputLinesWidth16)
{   
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLines =inputGen.GetLinesFloat16 (width + PADDING, 5, 3);
	outLinesC = inputGen.GetEmptyLinesFp16(width/4, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width/4, 1);

	inputGen.FillLine(convMat, 25, 0.04f);
	
	
	Convolution5x5s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5s4hhhhCycleCount / (width/4));

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5s4hhhh(inLines, outLinesC, convMat, width);

	
	outputChecker.CompareArrays(outLinesC,outLinesAsm, width/4,1, DELTA);

	
} 



TEST_F(Convolution5x5s4hhhhKernelTest, TestMinimumWidth)
{ 
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 5, 7);
	outLinesC = inputGen.GetEmptyLinesFp16(width/4, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width/4, 1);
	inputGen.FillLine(convMat, 25, 0.04f);

	Convolution5x5s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5s4hhhhCycleCount / (width/4));

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5s4hhhh(inLines,outLinesC, convMat, width);

    
	outputChecker.CompareArrays(outLinesC,outLinesAsm, width/4,1, DELTA);
}

TEST_F(Convolution5x5s4hhhhKernelTest, TestSobelFilter)
{
	width = 80;
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width + PADDING, 5, 0.0, 255.0);
	//outputChecker.DumpArrays("refInput","refInput.c",inLines,width+PADDING,5);
	outLinesC = inputGen.GetEmptyLinesFp16(width/4, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width/4, 1);

	half convMat[25] = { 2.0 / 25.0, 1.0 / 25.0, 0.0 / 25.0, -1.0 / 25.0, -2.0 / 25.0,
			             3.0 / 25.0, 2.0 / 25.0, 0.0 / 25.0, -2.0 / 25.0, -3.0 / 25.0,
			             4.0 / 25.0, 3.0 / 25.0, 0.0 / 25.0, -3.0 / 25.0, -4.0 / 25.0,
			             3.0 / 25.0, 2.0 / 25.0, 0.0 / 25.0, -2.0 / 25.0, -3.0 / 25.0,
			    	     2.0 / 25.0, 1.0 / 25.0, 0.0 / 25.0, -1.0 / 25.0, -2.0 / 25.0 };
	

	Convolution5x5s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5s4hhhhCycleCount / (width/4));

	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5s4hhhh(inLines, outLinesC, convMat, width);	
	
	outputChecker.CompareArrays(outLinesC,outLinesAsm, width/4,1, DELTA);

	//outputChecker.DumpArrays("refOutputAsm", "refOutputAsm.c", outLinesAsm, width,1 );
	//outputChecker.DumpArrays("refOutputC", "refOutputC.c", outLinesC, width,1);



}

TEST_F(Convolution5x5s4hhhhKernelTest, TestEmbossFilter)
{
	width = 320;
	inputGen.SelectGenerator("random");
	inLines =  inputGen.GetLinesFloat16(width + PADDING, 5, 0, 255);
	outLinesC = inputGen.GetEmptyLinesFp16(width/4, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width/4, 1);

	half convMat[25] = { -1.0 / 25.0, -1.0 / 25.0, -1.0 / 25.0, -1.0 / 25.0, 0.0 / 25.0,
			            -1.0 / 25.0, -1.0 / 25.0, -1.0 / 25.0,  0.0 / 25.0, 1.0 / 25.0,
			            -1.0 / 25.0, -1.0 / 25.0,  0.0 / 25.0,  1.0 / 25.0, 1.0 / 25.0,
			            -1.0 / 25.0,  0.0 / 25.0,  1.0 / 25.0,  1.0 / 25.0, 1.0 / 25.0,
			    	     0.0 / 25.0,  1.0 / 25.0,  1.0 / 25.0,  1.0 / 25.0, 1.0 / 25.0 };

	Convolution5x5s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5s4hhhhCycleCount / (width/4));
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5s4hhhh(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC,outLinesAsm, width/4,1, DELTA);
}

////////------------ parameterized tests ----------------------------------------


INSTANTIATE_TEST_CASE_P(RandomInputs, Convolution5x5s4hhhhKernelTest,
		Values((unsigned int)16, (unsigned int)32, (unsigned int)320, (unsigned int)640, (unsigned int)800, (unsigned int)1280, (unsigned int)1920);
);

TEST_P(Convolution5x5s4hhhhKernelTest, TestRandomInputLinesBlurFilter)
{

	unsigned int width = GetParam();

	inputGen.SelectGenerator("uniform");

	inputGen.FillLine(convMat, 25, 0.04f);
	inputGen.SelectGenerator("random");
	inLines =  inputGen.GetLinesFloat16(width + PADDING, 5, 0, 255);
	outLinesC = inputGen.GetEmptyLinesFp16(width/4, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width/4, 1);

	Convolution5x5s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
	RecordProperty("CyclePerPixel", convolution5x5s4hhhhCycleCount / (width/4));
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5s4hhhh(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC,outLinesAsm, width/4,1, DELTA);

}

TEST_P(Convolution5x5s4hhhhKernelTest, TestRandomInputLinesRandomMatrix)
{
	unsigned int width = GetParam();
	inputGen.SelectGenerator("random");

	inputGen.FillLine(convMat, 25, 0.0f, 1.0f);
		
	inputGen.SelectGenerator("random");
	inLines =  inputGen.GetLinesFloat16(width + PADDING, 5, 0.0, 1.0f);
	outLinesC = inputGen.GetEmptyLinesFp16(width/4, 1);
	outLinesAsm = inputGen.GetEmptyLinesFp16(width/4, 1);


	Convolution5x5s4hhhh_asm_test(inLines, outLinesAsm, convMat, width);
    RecordProperty("CyclePerPixel", convolution5x5s4hhhhCycleCount / (width/4));
	
	inLines = inputGen.GetOffsettedLines(inLines, SIZE, PADDING / 2);
	mvcvConvolution5x5s4hhhh(inLines, outLinesC, convMat, width);

	outputChecker.CompareArrays(outLinesC,outLinesAsm, width/4,1, DELTA);
}

