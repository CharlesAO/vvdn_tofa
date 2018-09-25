//IntegralImageSumTest
//Asm function prototype:
//   void integralimage_sum_u32_asm(u8** in, u8** out, u32* sum, u32 width);
//
//Asm test function prototype:
//   void integralImageSum_asm_test(u8** in, u8** out, u32* sum, u32 width);
//
//C function prototype:
//   void integralimage_sum_u32(u8** in, u8** out, u32* sum, u32 width);
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// sum       - sum of previous pixels . for this parameter we must have an array of u32 declared as global and having the width of the line		  
// width     - width of input line

#include "gtest/gtest.h"
#include <integralImageSumM2.h>
#include "integralImageSumM2_asm_test.h"
#include "integralImageSumM2_unittest.h"
#include "InputGenerator.h"
#include "RandomGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

class IntegralImageSumTest : public ::testing::Test {
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
    unsigned int* outLinesAsm;
    unsigned int* outLinesC;
   

   ArrayChecker outputChecker;


    virtual void TearDown()
    {
    }
};

TEST_F(IntegralImageSumTest, TestAsmAllValues0)
{
	runNr = 0; 
	width = 320;
	
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width,  0);
    
	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);

  

    EXPECT_EQ((unsigned int)0, outLinesAsm[319]);
    EXPECT_EQ((unsigned int)0, outLinesC[319]);

   CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestAsmLowestLineSize)
{
	width = 16;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(16, 0);
    
    inLines[0] = 6;
    inLines[15] = 10;
    
   	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);

    EXPECT_EQ((unsigned int)6, outLinesAsm[0]);
    EXPECT_EQ((unsigned int)6, outLinesC[0]);
    EXPECT_EQ((unsigned int)16, outLinesAsm[15]);
    EXPECT_EQ((unsigned int)16, outLinesC[15]);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestAsmFixedValues)
{

	width = 320;
	runNr = 0;
	
    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);
    
    inLines[5] = 100;
    inLines[200] = 2;
    
   	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);



    EXPECT_EQ((unsigned int)0, outLinesAsm[4]);
    EXPECT_EQ((unsigned int)0, outLinesC[4]);
    EXPECT_EQ((unsigned int)100, outLinesAsm[5]);
    EXPECT_EQ((unsigned int)100, outLinesC[5]);
    EXPECT_EQ((unsigned int)100, outLinesAsm[199]);
    EXPECT_EQ((unsigned int)100, outLinesC[199]);
    EXPECT_EQ((unsigned int)102, outLinesAsm[200]);
    EXPECT_EQ((unsigned int)102, outLinesC[200]);
    EXPECT_EQ((unsigned int)102, outLinesAsm[319]);
    EXPECT_EQ((unsigned int)102, outLinesC[319]);

	CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestAsmAllValuesOne)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 1);

  	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);

    EXPECT_EQ((unsigned int)320, outLinesAsm[319]);
    EXPECT_EQ((unsigned int)320, outLinesC[319]);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestAsmAllValues255)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 255);

  	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);

    EXPECT_EQ((unsigned int)(320 * 255), outLinesAsm[319]);
    EXPECT_EQ((unsigned int)(320 * 255), outLinesC[319]);
    
    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestWidth240RandomData)
{
    width = 240;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
    	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);
 
    CompareArrays(outLinesC, outLinesAsm, width); 
}

TEST_F(IntegralImageSumTest, TestWidth320RandomData)
{
    width = 320;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
   	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);
  
    CompareArrays(outLinesC, outLinesAsm, width); 
}

TEST_F(IntegralImageSumTest, TestWidth640RandomData)
{
    width = 640;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
  	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestWidth1280RandomData)
{
    width = 1280;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);
	
    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestWidth1920RandomData)
{
    width = 1920;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);

    CompareArrays(outLinesC, outLinesAsm, width); 
}

TEST_F(IntegralImageSumTest, TestRandomWidthRandomData)
{
    inputGen.SelectGenerator("random");
    width = ranGen.GenerateUInt(0, 1920, 8);
    inLines = inputGen.GetLine(width, 0, 255);
  
	runNr = 0;
  	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);
    
    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSumTest, TestRandomDATABigWidth)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLines = inputGen.GetLine(width, 0, 255);
	
	unsigned int intermbuff[1920] , *intern[1], **interm;
	for(int i=0; i<(int)width;i++)
		intermbuff[i] = randGen->GenerateUInt(1, 1920, 1);;
	intern[0] = (unsigned int *)intermbuff;
	interm = (unsigned int **)intern;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);
 
    CompareArrays(outLinesC, outLinesAsm, width);
	
	
}

TEST_F(IntegralImageSumTest, TestRandomDATABigWidthPrev255)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLines = inputGen.GetLine(width, 0, 255);

	unsigned int intermbuff[1920] , *intern[1], **interm;
	for(int i=0; i<(int)width;i++)
		intermbuff[i] = 255;
	intern[0] = (unsigned int *)intermbuff;
	interm = (unsigned int **)intern;

	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
  
	
	integralImageSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSumCycleCount);
 
	CompareArrays(outLinesC, outLinesAsm, width);
	
}