//IntegralImageSumTest
//Asm function prototype:
//   void mvcvIntegralImageSumU16U32(unsigned int* out, unsigned short* in, unsigned int runNr, unsigned int **previsionOutLnPointer, unsigned int width);
//
//Asm test function prototype:
//   void integralImageSumU16U32_asm_test(unsigned int* out, unsigned short* in, unsigned int runNr, unsigned int **previsionOutLnPointer, unsigned int width);
//
//C function prototype:
//   void mvcvIntegralImageSumU16U32(unsigned int* out, unsigned short* in, unsigned int runNr, unsigned int **previsionOutLnPointer, unsigned int width);
//
//Parameter description:
// integral image kernel - this kernel makes the sum of all pixels before it and on the left of it's column  ( this particular case makes sum of pixels in u32 format)
// param[out] out        - array of pointers for output lines  U32 format
// param[in] in          - array of pointers to input lines U16 data format     
// param[runNr]in        - output line number
// param[in] sum         - pointer to prevision generated line, will be used for calculate current pixels, and will be updated at the end to point to new output generated line
// param[in] width       - width of input line

#include "gtest/gtest.h"
#include <integralImageSumU16U32.h>
#include "integralImageSumU16U32_unittest.h"
///
#include "integralImageSumU16U32_asm_test.h"
#include "InputGenerator.h"
#include "RandomGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class integralImageSumU16U32Test : public ::testing::Test {
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
    unsigned short* inLine;
    unsigned int*  outLineAsm;
    unsigned int*  outLineC;
    unsigned int *asmSum[10];
    unsigned int *cSum[10];
    ArrayChecker outputChecker;


    virtual void TearDown()
    {
    }
};

TEST_F(integralImageSumU16U32Test, TestAsmAllValues0)
{
    inputGen.SelectGenerator("uniform");
	width = 320;
	runNr = 0;
    inLine = inputGen.GetLineU16(width, 0);
  
    outLineAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLineC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    
    integralImageSumU16U32_asm_test(outLineAsm, inLine, runNr, (unsigned int**)asmSum, width);
	mvcvIntegralImageSumU16U32(outLineC, inLine, runNr, (unsigned int**)cSum, width);
    RecordProperty("CycleCount", integralImageSumU16U32CycleCount);
	
    CompareArrays(outLineC, outLineAsm, width);
}



TEST_F(integralImageSumU16U32Test, TestAsmFixedValues)
{
    inputGen.SelectGenerator("uniform");
	width = 320;
	runNr = 0;
    inLine = inputGen.GetLineU16(width, 0);
    
    inLine[5] = 100;
    inLine[200] = 2;
    
    outLineAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLineC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);

    integralImageSumU16U32_asm_test(outLineAsm, inLine, runNr, (unsigned int**)asmSum, width);
	mvcvIntegralImageSumU16U32(outLineC, inLine, runNr, (unsigned int**)cSum, width);
    RecordProperty("CycleCount", integralImageSumU16U32CycleCount);

    EXPECT_EQ((unsigned int)0,   outLineAsm[4]);
    EXPECT_EQ((unsigned int)0,   outLineC[4]);
    EXPECT_EQ((unsigned int)100, outLineAsm[5]);
    EXPECT_EQ((unsigned int)100, outLineC[5]);
    EXPECT_EQ((unsigned int)100, outLineAsm[199]);
    EXPECT_EQ((unsigned int)100, outLineC[199]);
    EXPECT_EQ((unsigned int)102, outLineAsm[200]);
    EXPECT_EQ((unsigned int)102, outLineC[200]);
    EXPECT_EQ((unsigned int)102, outLineAsm[319]);
    EXPECT_EQ((unsigned int)102, outLineC[319]);
    CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(integralImageSumU16U32Test, TestAsmAllValuesOne)
{
    inputGen.SelectGenerator("uniform");
	width = 320;
	runNr = 0;
    inLine = inputGen.GetLineU16(width, 1);

    outLineAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLineC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);

    integralImageSumU16U32_asm_test(outLineAsm, inLine, runNr, (unsigned int**)asmSum, width);
    mvcvIntegralImageSumU16U32(outLineC, inLine, runNr, (unsigned int**)cSum, width);
    RecordProperty("CycleCount", integralImageSumU16U32CycleCount);

    EXPECT_EQ((unsigned int)320, outLineAsm[319]);
    EXPECT_EQ((unsigned int)320, outLineC[319]);
    CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(integralImageSumU16U32Test, TestAsmAllValues255)
{
    inputGen.SelectGenerator("uniform");
	width = 320;
	runNr = 0;
    inLine = inputGen.GetLineU16(width, 255);

    outLineAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLineC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);

    integralImageSumU16U32_asm_test(outLineAsm, inLine, runNr, (unsigned int**)asmSum, width);
    mvcvIntegralImageSumU16U32(outLineC, inLine, runNr, (unsigned int**)cSum, width);
    RecordProperty("CycleCount", integralImageSumU16U32CycleCount);

    EXPECT_EQ((unsigned int)(320 * 255), outLineAsm[319]);
    EXPECT_EQ((unsigned int)(320 * 255), outLineC[319]);    
    CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(integralImageSumU16U32Test, TestWidth240RandomData)
{
    inputGen.SelectGenerator("random");
	width = 240;
	runNr = 0;
    inLine = inputGen.GetLineU16(width, 0, 255);
  
    outLineAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLineC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);

    integralImageSumU16U32_asm_test(outLineAsm, inLine, runNr, (unsigned int**)asmSum, width);
    mvcvIntegralImageSumU16U32(outLineC, inLine, runNr, (unsigned int**)cSum, width);
    RecordProperty("CycleCount", integralImageSumU16U32CycleCount);
 
    CompareArrays(outLineC, outLineAsm, width);
}

TEST_F(integralImageSumU16U32Test, TestRandomDATASmallWidth)
{
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(16, 1281, 8);
	runNr = 0;
	inLine = inputGen.GetLineU16(width, 0, 255);
	
	outLineAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLineC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);

    integralImageSumU16U32_asm_test(outLineAsm, inLine, runNr, (unsigned int**)asmSum, width);
    mvcvIntegralImageSumU16U32(outLineC, inLine, runNr, (unsigned int**)cSum, width);
    RecordProperty("CycleCount", integralImageSumU16U32CycleCount);
 
    CompareArrays(outLineC, outLineAsm, width);

}


TEST_F(integralImageSumU16U32Test, TestRandomDATABigWidth)
{
	
	inputGen.SelectGenerator("random");
	width = randGen->GenerateUInt(1280, 1920, 8);
	runNr = randGen->GenerateUInt(1, 1920, 1);
	inLine = inputGen.GetLineU16(width, 0, 65025);
	unsigned int intermbuff[1920] , *interm[10];
	for(int i=0; i<1920;i++)
		intermbuff[i] = 0;
	interm[0] = (unsigned int *)intermbuff;
	
	outLineAsm = (unsigned int*)inputGen.GetEmptyLine(width * 4);
    outLineC   = (unsigned int*)inputGen.GetEmptyLine(width * 4);

    mvcvIntegralImageSumU16U32(outLineC, inLine, runNr, (unsigned int**)interm, width);
	integralImageSumU16U32_asm_test(outLineAsm, inLine, runNr, (unsigned int**)asmSum, width);
    RecordProperty("CycleCount", integralImageSumU16U32CycleCount);
 
    CompareArrays(outLineC, outLineAsm, width);
	
	//outputChecker.DumpArrays("refInput", "refInput.c", &inLine, width, 1);
	//outputChecker.DumpArrays("refOutput", "refOutput.c", &outLineC, width, 1);
	//outputChecker.DumpArrays("asmOutput", "asmOutput.c", &outLineAsm, width , 1);
}

