/// mvispChromaFusionU8ToU8 - performs an interpolation between RGB channels
/// @param[in] inCRshort -   Red channel multiplied with (1-alpha)
/// @param[in] inCGshort - Green channel multiplied with (1-alpha)
/// @param[in] inCBshort -  Blue channel multiplied with (1-alpha)
/// @param[in] inCRlong  -  Red  channel multiplied with alpha
/// @param[in] inCGlong  - Green channel multiplied with alpha
/// @param[in] inCBlong  -  Blue channel multiplied with alpha
/// @param[in] inAlpha   - coeficient
/// @param[out] outR     - output of red channel
/// @param[out] outG     - output of green channel
/// @param[out] outB     - output of blue channel
/// @param[in] inWidth    - Width of input lines

#include "gtest/gtest.h"
#include "chromaFusionU8ToU8.h"
#include "chromaFusionU8ToU8_asm_test.h"
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
#define SIZE 2
#define DELTA 1 //accepted tolerance between C and ASM results

class ChromaFusionU8ToU8KernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	//half convMatAsm[9];

	unsigned char*  inCRshort;
	unsigned char*  inCGshort;
	unsigned char*  inCBshort;
	unsigned char*   inCRlong;
	unsigned char*   inCGlong;
	unsigned char*   inCBlong;
	half*  inAlpha;

	unsigned char*   outRAsm;
	unsigned char*   outGAsm;
	unsigned char*   outBAsm;
	
	unsigned char*   outRC;
	unsigned char*   outGC;
	unsigned char*   outBC;
	
	unsigned int width;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};


TEST_F(ChromaFusionU8ToU8KernelTest, TestRandomInputRandomWidth)
{

	width = randGen->GenerateUInt(8, 1920, 8);

	inputGen.SelectGenerator("random");
	inCRshort = inputGen.GetLine(width + PADDING,0,255);
	inCGshort = inputGen.GetLine(width + PADDING,0,255);
	inCBshort = inputGen.GetLine(width + PADDING,0,255);
	 inCRlong = inputGen.GetLine(width + PADDING,0,255);
     inCGlong = inputGen.GetLine(width + PADDING,0,255);
	 inCBlong = inputGen.GetLine(width + PADDING,0,255);
	 inAlpha  = inputGen.GetLineFloat16(width + PADDING,0,1);
	
	inputGen.SelectGenerator("uniform");
	outRAsm = inputGen.GetLine(width + PADDING,5);
	outGAsm = inputGen.GetLine(width + PADDING,5);
	outBAsm = inputGen.GetLine(width + PADDING,5);
	outRC = inputGen.GetLine(width + PADDING,5);
	outGC = inputGen.GetLine(width + PADDING,5);
	outBC = inputGen.GetLine(width + PADDING,5);



	mvispChromaFusionU8ToU8(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRC, outGC, outBC, width);
	ChromaFusionU8ToU8_asm_test(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRAsm, outGAsm, outBAsm, width);
	RecordProperty("CyclePerPixel", ChromaFusionU8ToU8CycleCount / width);
    
	outputChecker.CompareArrays(outRAsm, outRC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outGAsm, outGC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outBAsm, outBC, width + PADDING, DELTA);
}

TEST_F(ChromaFusionU8ToU8KernelTest, TestRandomInputMinWidth)
{

	width = 8;

	inputGen.SelectGenerator("random");
	inCRshort = inputGen.GetLine(width + PADDING,0,255);
	inCGshort = inputGen.GetLine(width + PADDING,0,255);
	inCBshort = inputGen.GetLine(width + PADDING,0,255);
	 inCRlong = inputGen.GetLine(width + PADDING,0,255);
     inCGlong = inputGen.GetLine(width + PADDING,0,255);
	 inCBlong = inputGen.GetLine(width + PADDING,0,255);
	 inAlpha  = inputGen.GetLineFloat16(width + PADDING,0,1);
	
	inputGen.SelectGenerator("uniform");
	outRAsm = inputGen.GetLine(width + PADDING,5);
	outGAsm = inputGen.GetLine(width + PADDING,5);
	outBAsm = inputGen.GetLine(width + PADDING,5);
	outRC = inputGen.GetLine(width + PADDING,5);
	outGC = inputGen.GetLine(width + PADDING,5);
	outBC = inputGen.GetLine(width + PADDING,5);



	mvispChromaFusionU8ToU8(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRC, outGC, outBC, width);
	ChromaFusionU8ToU8_asm_test(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRAsm, outGAsm, outBAsm, width);
	RecordProperty("CyclePerPixel", ChromaFusionU8ToU8CycleCount / width);
    
	outputChecker.CompareArrays(outRAsm, outRC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outGAsm, outGC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outBAsm, outBC, width + PADDING, DELTA);
}

TEST_F(ChromaFusionU8ToU8KernelTest, TestRandomInputMaxWidth)
{

	width = 1920;

	inputGen.SelectGenerator("random");
	inCRshort = inputGen.GetLine(width + PADDING,0,255);
	inCGshort = inputGen.GetLine(width + PADDING,0,255);
	inCBshort = inputGen.GetLine(width + PADDING,0,255);
	 inCRlong = inputGen.GetLine(width + PADDING,0,255);
     inCGlong = inputGen.GetLine(width + PADDING,0,255);
	 inCBlong = inputGen.GetLine(width + PADDING,0,255);
	 inAlpha  = inputGen.GetLineFloat16(width + PADDING,0,1);
	
	inputGen.SelectGenerator("uniform");
	outRAsm = inputGen.GetLine(width + PADDING,5);
	outGAsm = inputGen.GetLine(width + PADDING,5);
	outBAsm = inputGen.GetLine(width + PADDING,5);
	outRC = inputGen.GetLine(width + PADDING,5);
	outGC = inputGen.GetLine(width + PADDING,5);
	outBC = inputGen.GetLine(width + PADDING,5);



	mvispChromaFusionU8ToU8(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRC, outGC, outBC, width);
	ChromaFusionU8ToU8_asm_test(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRAsm, outGAsm, outBAsm, width);
	RecordProperty("CyclePerPixel", ChromaFusionU8ToU8CycleCount / width);
    
	outputChecker.CompareArrays(outRAsm, outRC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outGAsm, outGC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outBAsm, outBC, width + PADDING, DELTA);
}

TEST_F(ChromaFusionU8ToU8KernelTest, TestUniformMaxInputRandomWidth)
{

	width = randGen->GenerateUInt(8, 1920, 8);

	inputGen.SelectGenerator("uniform");
	inCRshort = inputGen.GetLine(width + PADDING,255);
	inCGshort = inputGen.GetLine(width + PADDING,255);
	inCBshort = inputGen.GetLine(width + PADDING,255);
	 inCRlong = inputGen.GetLine(width + PADDING,255);
     inCGlong = inputGen.GetLine(width + PADDING,255);
	 inCBlong = inputGen.GetLine(width + PADDING,255);
	 inAlpha  = inputGen.GetLineFloat16(width + PADDING,1);
	
	inputGen.SelectGenerator("uniform");
	outRAsm = inputGen.GetLine(width + PADDING,5);
	outGAsm = inputGen.GetLine(width + PADDING,5);
	outBAsm = inputGen.GetLine(width + PADDING,5);
	outRC = inputGen.GetLine(width + PADDING,5);
	outGC = inputGen.GetLine(width + PADDING,5);
	outBC = inputGen.GetLine(width + PADDING,5);



	mvispChromaFusionU8ToU8(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRC, outGC, outBC, width);
	ChromaFusionU8ToU8_asm_test(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRAsm, outGAsm, outBAsm, width);
	RecordProperty("CyclePerPixel", ChromaFusionU8ToU8CycleCount / width);
    
	outputChecker.CompareArrays(outRAsm, outRC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outGAsm, outGC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outBAsm, outBC, width + PADDING, DELTA);
}

TEST_F(ChromaFusionU8ToU8KernelTest, TestUniformMinInputRandomWidth)
{

	width = randGen->GenerateUInt(8, 1920, 8);

	inputGen.SelectGenerator("uniform");
	inCRshort = inputGen.GetLine(width + PADDING,0);
	inCGshort = inputGen.GetLine(width + PADDING,0);
	inCBshort = inputGen.GetLine(width + PADDING,0);
	 inCRlong = inputGen.GetLine(width + PADDING,0);
     inCGlong = inputGen.GetLine(width + PADDING,0);
	 inCBlong = inputGen.GetLine(width + PADDING,0);
	 inAlpha  = inputGen.GetLineFloat16(width + PADDING,0);
	
	inputGen.SelectGenerator("uniform");
	outRAsm = inputGen.GetLine(width + PADDING,5);
	outGAsm = inputGen.GetLine(width + PADDING,5);
	outBAsm = inputGen.GetLine(width + PADDING,5);
	outRC = inputGen.GetLine(width + PADDING,5);
	outGC = inputGen.GetLine(width + PADDING,5);
	outBC = inputGen.GetLine(width + PADDING,5);



	mvispChromaFusionU8ToU8(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRC, outGC, outBC, width);
	ChromaFusionU8ToU8_asm_test(inCRshort, inCGshort, inCBshort, 
	inCRlong, inCGlong, inCBlong, 
	inAlpha, outRAsm, outGAsm, outBAsm, width);
	RecordProperty("CyclePerPixel", ChromaFusionU8ToU8CycleCount / width);
    
	outputChecker.CompareArrays(outRAsm, outRC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outGAsm, outGC, width + PADDING, DELTA);
	outputChecker.CompareArrays(outBAsm, outBC, width + PADDING, DELTA);
}
