//boxFilter kernel test
//Asm function prototype:
//     void mvispScale05BilinHV_Fp16U8_asm(fp16** in, u8** out, u32 width);
//
//Asm test function prototype:
//     void scale05BilinHV_Fp16U8_asm_test(fp16** in, u8** out, u32 width);
//
//C function prototype:
//     void mvispScale05BilinHV_Fp16U8(fp16** in, u8** out, u32 width);
//

#include "gtest/gtest.h"
#include "scale05BilinHV_Fp16U8.h"
#include "scale05BilinHV_Fp16U8_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 8
const unsigned short DELTA = 1;	

class scale05BilinHV_Fp16U8KernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	
	half **inLines;
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned char **outExpected;
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

 TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines32)
{
	width = 32;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 0.1f);
		
	inputGen.SelectGenerator("uniform");		
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);

	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
		
}  

TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines48)
{
	width = 48;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 1.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32,DELTA);
}


TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines64)
{
	width = 64;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 1.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32,DELTA);
}


TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines40)
{
	width = 40;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 1.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32,DELTA);
}


TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines72)
{
	width = 72;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 1.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
}


 TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines376)
{
	width = 376;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 1.0f);
	
	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32,DELTA);
}

 TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines320)
{
	width = 320;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 1.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
	
	//for (unsigned int i=0; i<width + 32; i++) {
	//printf("i:%D    outC=%d    outA=%d\n", i, outLinesC[0][i], outLinesAsm[0][i]);
	//}
	//outputChecker.DumpArrays("inline1", "inLine1.c", inLines[0], width);	
	//outputChecker.DumpArrays("inline2", "inLine2.c", inLines[1], width);	
	//outputChecker.DumpArrays("outC", "outC.c", outLinesC[0], width/2);	
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLinesAsm[0], width/2);
}

TEST_F(scale05BilinHV_Fp16U8KernelTest, TestUniformInputLines512)
{
	width = 512;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 255.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
}


TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines1920)
{
	width = 1920;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 255.0f);
	
	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
}


TEST_F(scale05BilinHV_Fp16U8KernelTest, TestRandomInputLines1920_1)
{
	width = 1920;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 255.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
}  

 INSTANTIATE_TEST_CASE_P(RandomInputs, scale05BilinHV_Fp16U8KernelTest,
		Values((unsigned int)32, (unsigned int)40, (unsigned int)48, (unsigned int)64, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

 TEST_P(scale05BilinHV_Fp16U8KernelTest, TestRandomData)
{
	inputGen.SelectGenerator("random");
	width = GetParam();
	
	inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 1.0f);

	inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + 32, 1, 0);
	outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
}  


 TEST_P(scale05BilinHV_Fp16U8KernelTest, TestRandomData1)
 {
	inputGen.SelectGenerator("random");
	 width = GetParam();
	
	 inLines = inputGen.GetLinesFloat16(width * 2, 2, 0.0f, 255.0f);

	 inputGen.SelectGenerator("uniform");
	 outLinesC = inputGen.GetLines(width + 32, 1, 0);
	 outLinesAsm = inputGen.GetLines(width + 32, 1, 0);

	 mvispScale05BilinHV_Fp16U8(inLines, outLinesC, width);
	 scale05BilinHV_Fp16U8_asm_test(inLines, outLinesAsm, width);
	
	 RecordProperty("CyclePerPixel", scale05BilinHV_Fp16U8CycleCount / width);
	 outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width + 32, DELTA);
 }   

