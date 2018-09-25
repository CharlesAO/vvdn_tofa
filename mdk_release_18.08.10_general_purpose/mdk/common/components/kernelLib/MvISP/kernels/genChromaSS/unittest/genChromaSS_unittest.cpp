//genChromaSS kernel test
//Asm function prototype:
//     void genChromaSS_asm(u8** in, u8** out, u32 normalize, u32 width);
//
//Asm test function prototype:
//     void genChromaSS_asm_test(u8** in, u8** out, u32 normalize, u32 width);
//
//C function prototype:
//     void genChromaSS(u8** in, u8** out, u32 normalize, u32 width);
//
//Parameter description:
// in        - array of pointers to input lines
// out       - array of pointers for output lines
// normalize - parameter to check if we want to do a normalize boxfilter or not 1 for normalized values , 0 in the other case
// width     - width of input line
//
// each line needs a padding o kernel_size - 1 values at the end; the value of the padding is equal
// with the last value of line without padding

#include "gtest/gtest.h"
#include "genChromaSS.h"
#include "genChromaSS_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


using ::testing::TestWithParam;
using ::testing::Values;


#define DELTA 1

class mvispGenChromaSSKernelTest : public ::testing::TestWithParam< unsigned long > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char **inLines;
	unsigned char **outLinesC;
	unsigned char **outLinesAsm;
	unsigned int width;
	int eps;
	float chromaScale[3];
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


// ------------- normalized variant -------------------

TEST_F(mvispGenChromaSSKernelTest, TestUniformInputLines32)
{
	width = 32;
	eps = 1;
	chromaScale[0] = 1;
	chromaScale[1] = 1;
	chromaScale[2] = 1;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width , 6, 0);
	
	inputGen.FillLine(inLines[0], width, 32);
    
	outLinesC = inputGen.GetEmptyLines(width, 3);
	outLinesAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaSS(outLinesC, inLines, eps, chromaScale, width/2 );
	mvispGenChromaSS_asm_test(outLinesAsm, inLines, eps , chromaScale, width/2 );
    
	RecordProperty("CyclePerPixel", mvispGenChromaSSCycleCount / width/2);
    
	//outputChecker.DumpArrays("refInput", "refInput.c", inLines, width , 6);
	//outputChecker.DumpArrays("refOutputA", "refOutputA.c", outLinesAsm, width/2 , 3);
	//outputChecker.DumpArrays("refOutputC", "refOutputC.c", outLinesC, width/2 , 3);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[1], outLinesAsm[1], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[2], outLinesAsm[2], width/2, DELTA);

}


TEST_F(mvispGenChromaSSKernelTest, TestAllValues255)
{
	width = 32;
	eps = 1;
	chromaScale[0] = 1;
	chromaScale[1] = 1;
	chromaScale[2] = 1;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width , 6, 255);
	
    
	outLinesC = inputGen.GetEmptyLines(width, 3);
	outLinesAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaSS(outLinesC, inLines, eps, chromaScale, width/2 );
	mvispGenChromaSS_asm_test(outLinesAsm, inLines, eps , chromaScale, width/2 );
    
	RecordProperty("CyclePerPixel", mvispGenChromaSSCycleCount / width/2);
	
    //outputChecker.DumpArrays("refInput", "refInput.c", inLines, width , 6);
	//outputChecker.DumpArrays("refOutputA", "refOutputA.c", outLinesAsm, width/2 , 3);
	//outputChecker.DumpArrays("refOutputC", "refOutputC.c", outLinesC, width/2 , 3);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[1], outLinesAsm[1], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[2], outLinesAsm[2], width/2, DELTA);
}

TEST_F(mvispGenChromaSSKernelTest, TestAllValuesZero)
{
	width = 320;
	eps = 1;
	chromaScale[0] = 1;
	chromaScale[1] = 1;
	chromaScale[2] = 1;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width , 6, 0);
	
    
	outLinesC = inputGen.GetEmptyLines(width, 3);
	outLinesAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaSS(outLinesC, inLines, eps, chromaScale, width/2 );
	mvispGenChromaSS_asm_test(outLinesAsm, inLines, eps , chromaScale, width/2 );
    
	RecordProperty("CyclePerPixel", mvispGenChromaSSCycleCount / width/2);
	
    //outputChecker.DumpArrays("refInput", "refInput.c", inLines, width , 6);
	//outputChecker.DumpArrays("refOutputA", "refOutputA.c", outLinesAsm, width/2 , 3);
	//outputChecker.DumpArrays("refOutputC", "refOutputC.c", outLinesC, width/2 , 3);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[1], outLinesAsm[1], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[2], outLinesAsm[2], width/2, DELTA);
}


TEST_F(mvispGenChromaSSKernelTest, TestSmallWidthSizeRandomData)
{
	width = randGen->GenerateUInt(16, 1281, 8);
	//width = 32;
	eps = 2;
	chromaScale[0] = 1.25;
	chromaScale[1] = 2.25;
	chromaScale[2] = 1.75;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width , 6, 0, 255);
	
    
	outLinesC = inputGen.GetEmptyLines(width, 3);
	outLinesAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaSS(outLinesC, inLines, eps, chromaScale, width/2 );
	mvispGenChromaSS_asm_test(outLinesAsm, inLines, eps , chromaScale, width/2 );
    
	RecordProperty("CyclePerPixel", mvispGenChromaSSCycleCount / width/2);
	
    //outputChecker.DumpArrays("refInput", "refInput.c", inLines, width , 6);
	//outputChecker.DumpArrays("refOutputA", "refOutputA.c", outLinesAsm, width/2 , 3);
	//outputChecker.DumpArrays("refOutputC", "refOutputC.c", outLinesC, width/2 , 3);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[1], outLinesAsm[1], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[2], outLinesAsm[2], width/2, DELTA);
}

TEST_F(mvispGenChromaSSKernelTest, TestBigWidthSizeRandomData)
{
	width = randGen->GenerateUInt(1281, 1920, 8);
	eps = 1;
	chromaScale[0] = 2.25;
	chromaScale[1] = 1.75;
	chromaScale[2] = 1.25;
	
	inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width , 6, 0, 255);
	
    
	outLinesC = inputGen.GetEmptyLines(width, 3);
	outLinesAsm = inputGen.GetEmptyLines(width, 3);
    
	mvispGenChromaSS(outLinesC, inLines, eps, chromaScale, width/2 );
	mvispGenChromaSS_asm_test(outLinesAsm, inLines, eps , chromaScale, width/2 );
    
	RecordProperty("CyclePerPixel", mvispGenChromaSSCycleCount / width/2);
	
    //outputChecker.DumpArrays("refInput", "refInput.c", inLines, width , 6);
	//outputChecker.DumpArrays("refOutputA", "refOutputA.c", outLinesAsm, width/2 , 3);
	//outputChecker.DumpArrays("refOutputC", "refOutputC.c", outLinesC, width/2 , 3);
	
	outputChecker.CompareArrays(outLinesC[0], outLinesAsm[0], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[1], outLinesAsm[1], width/2, DELTA);
	outputChecker.CompareArrays(outLinesC[2], outLinesAsm[2], width/2, DELTA);
}
