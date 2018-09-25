//IntegralImageSquareSumTest
//Asm function prototype:
//   void mvcvIntegralimage_sqsum_u32_M2_asm(u8** in, u8** out, u32* sum, u32 width);
//
//Asm test function prototype:
//   void integralImageSquareSum_asm_test(u8** in, u8** out, u32* sum, u32 width);
//
//C function prototype:
//   void mvcvIntegralimage_sqsum_u32_M2(u8** in, u8** out, u32* sum, u32 width);
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// sum       - sum of previous pixels . for this parameter we must have an array of u32 declared as global and having the width of the line
//             (this particular case makes square sum of pixels)
// width     - width of input line

#include "gtest/gtest.h"
#include <integralImageSquareSumM2.h>
#include "integralImageSquareSumM2_asm_test.h"
#include "integralImageSquareSumM2_unittest.h"
#include "InputGenerator.h"
#include "RandomGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

class IntegralImageSquareSumTest : public ::testing::Test {
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

TEST_F(IntegralImageSquareSumTest, TestAsmAllValues0)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);

    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

    EXPECT_EQ((unsigned int)0, outLinesAsm[319]);
    EXPECT_EQ((unsigned int)0, outLinesC[319]);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestAsmLowestLineSize)
{
	width = 16;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);

    inLines[0] = 6;
    inLines[15] = 10;
    
    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

    EXPECT_EQ((unsigned int)36, outLinesAsm[0]);
    EXPECT_EQ((unsigned int)36, outLinesC[0]);
    EXPECT_EQ((unsigned int)136, outLinesAsm[15]);
    EXPECT_EQ((unsigned int)136, outLinesC[15]);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestAsmFixedValues)
{
    width = 320;
	runNr = 0;
	
	inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 0);
    
    inLines[5] = 100;
    inLines[200] = 2;
    
	outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

     EXPECT_EQ((unsigned int)0, outLinesAsm[4]);
     EXPECT_EQ((unsigned int)0, outLinesC[4]);
     EXPECT_EQ((unsigned int)10000, outLinesAsm[5]);
     EXPECT_EQ((unsigned int)10000, outLinesC[5]);
     EXPECT_EQ((unsigned int)10000, outLinesAsm[199]);
     EXPECT_EQ((unsigned int)10000, outLinesC[199]);
     EXPECT_EQ((unsigned int)10004, outLinesAsm[200]);
     EXPECT_EQ((unsigned int)10004, outLinesC[200]);
     EXPECT_EQ((unsigned int)10004, outLinesAsm[319]);
     EXPECT_EQ((unsigned int)10004, outLinesC[319]);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestAsmAllValuesOne)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 1);
    
	outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

    EXPECT_EQ((unsigned int)320, outLinesAsm[319]);
    EXPECT_EQ((unsigned int)320, outLinesC[319]);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestAsmAllValues255)
{
	width = 320;
	runNr = 0;

    inputGen.SelectGenerator("uniform");
    inLines = inputGen.GetLine(width, 255);

	outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

    EXPECT_EQ((unsigned int)(320 * 255 * 255), outLinesAsm[319]);
    EXPECT_EQ((unsigned int)(320 * 255 * 255), outLinesC[319]);
    
    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestWidth240RandomData)
{
    width = 240;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);
    CompareArrays(outLinesC, outLinesAsm, width); 
}

TEST_F(IntegralImageSquareSumTest, TestWidth320RandomData)
{
	width = 320;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);
  
    CompareArrays(outLinesC, outLinesAsm, width); 
}

TEST_F(IntegralImageSquareSumTest, TestWidth640RandomData)
{
    width = 640;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestWidth1280RandomData)
{
    width = 1280;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
    
    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestWidth1920RandomData)
{
    width = 1920;
	runNr = 0;
  
    inputGen.SelectGenerator("random");
    inLines = inputGen.GetLine(width, 0, 255);
  
    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);

    CompareArrays(outLinesC, outLinesAsm, width); 
}

TEST_F(IntegralImageSquareSumTest, TestRandomWidthRandomData)
{
    inputGen.SelectGenerator("random");
    width = ranGen.GenerateUInt(0, 1920, 16);
    inLines = inputGen.GetLine(width, 0, 255);
    runNr = 0;
	
    outLinesAsm =  (unsigned int *)inputGen.GetEmptyLine(width * 4);
    outLinesC   =  (unsigned int *)inputGen.GetEmptyLine(width * 4);

	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);
    
    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestRandomDATABigWidthPrev0)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLines = inputGen.GetLine(width, 0, 255);
	
	unsigned int intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 0;
	intern[0] = (unsigned int *)intermbuff;
	interm = (unsigned int **)intern;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);
 
    CompareArrays(outLinesC, outLinesAsm, width);

}

TEST_F(IntegralImageSquareSumTest, TestRandomDATABigWidthPrevRandom)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLines = inputGen.GetLine(width, 0, 255);
	
		unsigned int intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = randGen->GenerateUInt(1, 1920, 1);;
	intern[0] = (unsigned int *)intermbuff;
	interm = (unsigned int **)intern;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);
 
    CompareArrays(outLinesC, outLinesAsm, width);
}

TEST_F(IntegralImageSquareSumTest, TestRandomDATABigWidthPrev255)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLines = inputGen.GetLine(width, 0, 255);
	
		unsigned int intermbuff[1920] , *intern[1], **interm;
	for(unsigned int i=0; i<width;i++)
		intermbuff[i] = 255;
	intern[0] = (unsigned int *)intermbuff;
	interm = (unsigned int **)intern;
	
	outLinesAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLinesC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    

    integralImageSquareSum_asm_test(outLinesAsm, inLines, runNr, interm, width);
    mvcvIntegralimage_sqsum_u32_M2(outLinesC, inLines, runNr, interm, width);
    RecordProperty("CycleCount", integralImageSquareSumCycleCount);
 
    CompareArrays(outLinesC, outLinesAsm, width);

}