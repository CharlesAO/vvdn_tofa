//IntegralImageSquareSumFloatTest
//Asm function prototype:
//   void mvcvIntegralimage_sqsum_f32_M2_asm(u8** in, u8** out, u32* sum, u32 width);
//
//Asm test function prototype:
//   void integralImageSquareSumFloat_asm_test(u8** in, u8** out, u32* sum, u32 width);
//
//C function prototype:
//   void mvcvIntegralimage_sqsum_f32_M2(u8** in, u8** out, u32* sum, u32 width);
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// sum       - sum of previous pixels . for this parameter we must have an array of u32 declared as global and having the width of the line
//             (this particular case makes square sum of pixels in f32 format)
// width     - width of input line

#include "gtest/gtest.h"
#include <integralImageSquareSumFloatM2.h>
#include "integralImageSquareSumFloatM2_asm_test.h"
#include "InputGenerator.h"
#include "RandomGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

class IntegralImageSquareSumFloatTest : public ::testing::Test {
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

TEST_F(IntegralImageSquareSumFloatTest, TestAsmAllValues0)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);

	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    EXPECT_FLOAT_EQ(0.0F, outLinesAsm[319]);
    EXPECT_FLOAT_EQ(0.0F, outLinesC[319]);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestAsmLowestLineSize)
{
	width = 16;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 255);

    inLines[0] = 6;
    inLines[15] = 10;
    
  	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    EXPECT_FLOAT_EQ(36.0F, outLinesAsm[0]);
    EXPECT_FLOAT_EQ(36.0F, outLinesC[0]);
    EXPECT_FLOAT_EQ(910486.0F, outLinesAsm[15]);
    EXPECT_FLOAT_EQ(910486.0F, outLinesC[15]);
	
	outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}


TEST_F(IntegralImageSquareSumFloatTest, TestAsmFixedValues)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);
    
    inLines[5] = 100;
    inLines[200] = 2;
    
  	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    EXPECT_FLOAT_EQ(0.0F, outLinesAsm[4]);
    EXPECT_FLOAT_EQ(0.0F, outLinesC[4]);
    EXPECT_FLOAT_EQ(10000.0F, outLinesAsm[5]);
    EXPECT_FLOAT_EQ(10000.0F, outLinesC[5]);
    EXPECT_FLOAT_EQ(10000.0F, outLinesAsm[199]);
    EXPECT_FLOAT_EQ(10000.0F, outLinesC[199]);
    EXPECT_FLOAT_EQ(10004.0F, outLinesAsm[200]);
    EXPECT_FLOAT_EQ(10004.0F, outLinesC[200]);
    EXPECT_FLOAT_EQ(10004.0F, outLinesAsm[319]);
    EXPECT_FLOAT_EQ(10004.0F, outLinesC[319]);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestAsmAllValues255)
{
	width = 32;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 255);
    
    float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    EXPECT_FLOAT_EQ(2080800.0F, outLinesAsm[31]);
    EXPECT_FLOAT_EQ(2080800.0F, outLinesC[31]);
	
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}


TEST_F(IntegralImageSquareSumFloatTest, TestAsmAllValues1)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 1);
    
    float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    EXPECT_FLOAT_EQ(320.0F, outLinesAsm[319]);
    EXPECT_FLOAT_EQ(320.0F, outLinesC[319]);
	
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}


TEST_F(IntegralImageSquareSumFloatTest, TestAsmAllValues255MaximWidth)
{
    width = 1920;
	runNr = 0;
  
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 255);
  
	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);
    
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestWidth240RandomData)
{
    width = 240;
	runNr = 0;
	
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);
 
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestWidth320RandomData)
{
    width = 320;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);
  
    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestWidth640RandomData)
{
    width = 640;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestWidth1280RandomData)
{
    width = 1280;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestWidth1920RandomData)
{
    width = 1920;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 255;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));

    integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumFCycleCount);

    outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumFloatTest, TestRandomWidthRandomData)
{
    inputGen.SelectGenerator("random");
    width = 1920;//randGen->GenerateUInt(0, 1920, 16);
	runNr = 2;//randGen->GenerateUInt(1, 1920, 1);
    inLines = inputGen.GetLine(width, 0, 255);
   	
	float intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = randGen->GenerateUInt(1, 1920, 1);;
	intern[0] = (float *)intermbuff;
	interm = (float **)intern;
	
	
    outLinesAsm = (float*)inputGen.GetEmptyLine(width * sizeof(float));
    outLinesC = (float*)inputGen.GetEmptyLine(width * sizeof(float));
	
	RecordProperty("CycleCount", integralImageSquareSumFCycleCount);
	
	integralImageSquareSumFloat_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_f32_M2(outLinesC, inLines, runNr, interm, width);
        
   outputChecker.CompareArraysFloat(outLinesC, outLinesAsm, width);	
	
}
