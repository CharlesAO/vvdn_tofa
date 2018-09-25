//IntegralImageSumFloatTest
//Asm function prototype:
//   void mvcvIntegralimage_sum_f32_M2_asm(u8** in, u8** out, u32* sum, u32 width);
//
//Asm test function prototype:
//   void integralImageSumF_asm_test(u8** in, u8** out, u32* sum, u32 width);
//
//C function prototype:
//   void mvcvIntegralimage_sum_f32_M2(u8** in, u8** out, u32* sum, u32 width);
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// sum       - sum of previous pixels . for this parameter we must have an array of u32 declared as global and having the width of the line
// width     - width of input line

#include "gtest/gtest.h"
#include <integralImageSumFloatM2.h>
#include "integralImageSumFloatM2_asm_test.h"
#include "InputGenerator.h"
#include "RandomGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

class IntegralImageSumFloatTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    unsigned int width;
    unsigned int runNr;
    InputGenerator inputGen;
    RandomGenerator ranGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    unsigned char* inLines;
    float* outLinesAsm;
    float* outLinesC;
    ArrayChecker outputChecker;

    virtual void TearDown()
    {
    }
};

TEST_F(IntegralImageSumFloatTest, TestAsmAllValues0)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);

    float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);

    EXPECT_FLOAT_EQ(0.0F, outLinesAsm[319]);
    EXPECT_FLOAT_EQ(0.0F, outLinesC[319]);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestAsmLowestLineSize)
{
	width = 16;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);

    inLines[0] = 6;
    inLines[15] = 10;
    
	float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);

    EXPECT_FLOAT_EQ(6.0F, outLinesAsm[0]);
    EXPECT_FLOAT_EQ(6.0F, outLinesC[0]);
    EXPECT_FLOAT_EQ(16.0F, outLinesAsm[15]);
    EXPECT_FLOAT_EQ(16.0F, outLinesC[15]);

   outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestAsmFixedValues)
{
	width = 320;
	runNr = 0;
	
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);
    
    inLines[5] = 100;
    inLines[200] = 2;
    
   float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);

    EXPECT_FLOAT_EQ(0.0F, outLinesAsm[4]);
    EXPECT_FLOAT_EQ(0.0F, outLinesC[4]);
    EXPECT_FLOAT_EQ(100.0F, outLinesAsm[5]);
    EXPECT_FLOAT_EQ(100.0F, outLinesC[5]);
    EXPECT_FLOAT_EQ(100.0F, outLinesAsm[199]);
    EXPECT_FLOAT_EQ(100.0F, outLinesC[199]);
    EXPECT_FLOAT_EQ(102.0F, outLinesAsm[200]);
    EXPECT_FLOAT_EQ(102.0F, outLinesC[200]);
    EXPECT_FLOAT_EQ(102.0F, outLinesAsm[319]);
    EXPECT_FLOAT_EQ(102.0F, outLinesC[319]);

   outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestAsmAllValuesOne)
{

	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 1);
    
    float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);
	
    EXPECT_FLOAT_EQ(320.0F, outLinesAsm[319]);
    EXPECT_FLOAT_EQ(320.0F, outLinesC[319]);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestAsmAllValues255)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 255);

	float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);
		
    EXPECT_FLOAT_EQ(81600.0F, outLinesAsm[319]);
    EXPECT_FLOAT_EQ(81600.0F, outLinesC[319]);
    
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}


TEST_F(IntegralImageSumFloatTest, TestWidth240RandomData)
{
    width = 240;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
    float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);
 
   outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width); 
}

TEST_F(IntegralImageSumFloatTest, TestWidth320RandomData)
{
    width = 320;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
    float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);
  
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestWidth640RandomData)
{
    width = 640;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
    float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestWidth1280RandomData)
{
    width = 1280;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
	float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);

   outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestWidth1920RandomData)
{
    width = 1920;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
	float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumFloatTest, TestRandomWidthRandomData)
{
    inputGen.SelectGenerator("random");
    width = ranGen.GenerateUInt(8, 1920, 8);
    inLines = inputGen.GetLine(width, 0, 255);
    runNr = 0;
    float intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (float *)intermbuff;
	
		
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);
    
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}


TEST_F(IntegralImageSumFloatTest, TestRandomDATABigWidth)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLines = inputGen.GetLine(width, 0, 255);
	
	float intermbuff[1920] , *intern[1], **interm;
	for(int i=0; i<(int)width;i++)
		intermbuff[i] = randGen->GenerateUInt(1, 1920, 1);;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);
    
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
	
}


TEST_F(IntegralImageSumFloatTest, TestRandomDATABigWidthPrev255)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLines = inputGen.GetLine(width, 0, 255);
	
	float intermbuff[1920] , *intern[1], **interm;
	for(int i=0; i<(int)width;i++)
		intermbuff[i] = 255;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
	outLinesAsm = (float*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (float*)inputGen.GetEmptyLine(width * 4);
    	
    integralImageSumF_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumFCycleCount);
    
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
		
}

