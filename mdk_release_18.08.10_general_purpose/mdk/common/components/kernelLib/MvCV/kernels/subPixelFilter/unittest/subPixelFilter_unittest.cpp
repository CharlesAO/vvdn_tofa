
//Parameter description:
// mvcvCensusMatching 
// @param[in] in1            - pointer to input lines of the left image
// @param[in] in2            - pointer to input lines of the right image
// @param[out] out           - array of disparity cost
// @param[in] width          - width of the input lines
// @return    Nothing




#include "gtest/gtest.h"
#include <subPixelFilter.h>
#include "subPixelFilter_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16

class mvcvSubPixelFilterKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char* inputDisparityMap;
	unsigned char* inputCostVolume;
	unsigned short* outputDisparityMapC;
	unsigned short* outputDisparityMapAsm;
	unsigned int width;
	unsigned char maxDisp;
	unsigned char numFractionalBit;
	unsigned char * lutToUse;


	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

TEST_F(mvcvSubPixelFilterKernelTest, TestWidth16RandommaxDisp64)
{
        maxDisp = 64;
	width = 16;
        numFractionalBit = 5;
	
	inputGen.SelectGenerator("random");
	inputDisparityMap = inputGen.GetLine((width + PADDING), 0, 255);
	inputCostVolume = inputGen.GetLine((width + PADDING) * maxDisp, 0, 255);
	lutToUse = inputGen.GetLine(4096, 0, 32);
	
	inputGen.SelectGenerator("uniform");
	outputDisparityMapC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
        outputDisparityMapAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
	inputGen.FillLine(outputDisparityMapC,   width + PADDING , 5);
	inputGen.FillLine(outputDisparityMapAsm, width + PADDING , 5);
	

	mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMapC, width, maxDisp, numFractionalBit, lutToUse);
	mvcvSubPixelFilter_asm_test(inputDisparityMap, inputCostVolume, outputDisparityMapAsm, width, maxDisp, numFractionalBit, lutToUse);
	
	RecordProperty("CyclePerPixel", mvcvSubPixelFilterCycleCount / (width));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	outputChecker.CompareArrays(outputDisparityMapC, outputDisparityMapAsm, width + PADDING);
}

TEST_F(mvcvSubPixelFilterKernelTest, TestWidth16RandommaxDisp96)
{
        maxDisp = 96;
	width = 16;
        numFractionalBit = 5;
	
	inputGen.SelectGenerator("random");
	inputDisparityMap = inputGen.GetLine((width + PADDING), 0, 255);
	inputCostVolume = inputGen.GetLine((width + PADDING) * maxDisp, 0, 255);
	lutToUse = inputGen.GetLine(4096, 0, 32);
	
	inputGen.SelectGenerator("uniform");
	outputDisparityMapC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
        outputDisparityMapAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
	inputGen.FillLine(outputDisparityMapC,   width + PADDING , 5);
	inputGen.FillLine(outputDisparityMapAsm, width + PADDING , 5);
	

	mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMapC, width, maxDisp, numFractionalBit, lutToUse);
	mvcvSubPixelFilter_asm_test(inputDisparityMap, inputCostVolume, outputDisparityMapAsm, width, maxDisp, numFractionalBit, lutToUse);
	
	RecordProperty("CyclePerPixel", mvcvSubPixelFilterCycleCount / (width));

	//outputChecker.DumpArrays("inputDisparityMap_copy", "inputDisparityMap.h", inputDisparityMap, (width + PADDING));
	//outputChecker.DumpArrays("lutToUse_copy", "lutToUse.h", lutToUse, 4096);
	//outputChecker.DumpArrays("inputCostVolume_copy", "inputCostVolume.h", inputCostVolume, (width + PADDING) * maxDisp);
	//outputChecker.DumpArrays("outputDisparityMapC_copy", "outputDisparityMapC.h", outputDisparityMapC, (width + PADDING));
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	outputChecker.CompareArrays(outputDisparityMapC, outputDisparityMapAsm, width + PADDING);
}

TEST_F(mvcvSubPixelFilterKernelTest, TestWidth320Uniform)
{
        maxDisp = 64;
	width = 320;
        numFractionalBit = 5;
	
	inputGen.SelectGenerator("uniform");
	inputDisparityMap = inputGen.GetLine((width + PADDING), 10);
	inputCostVolume = inputGen.GetLine((width + PADDING) * maxDisp, 10);
	lutToUse = inputGen.GetLine(4096, 0, 32);
	
	inputGen.SelectGenerator("uniform");
	outputDisparityMapC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
        outputDisparityMapAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
	inputGen.FillLine(outputDisparityMapC,   width + PADDING , 5);
	inputGen.FillLine(outputDisparityMapAsm, width + PADDING , 5);
	

	mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMapC, width, maxDisp, numFractionalBit, lutToUse);
	mvcvSubPixelFilter_asm_test(inputDisparityMap, inputCostVolume, outputDisparityMapAsm, width, maxDisp, numFractionalBit, lutToUse);
	
	RecordProperty("CyclePerPixel", mvcvSubPixelFilterCycleCount / (width));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	outputChecker.CompareArrays(outputDisparityMapC, outputDisparityMapAsm, width + PADDING);
}

TEST_F(mvcvSubPixelFilterKernelTest, TestRandomDataDisp64Fractional5)
{
        maxDisp = 64;
	width = randGen->GenerateUInt(8, 1280, 8);
        numFractionalBit = 5;
	
	inputGen.SelectGenerator("random");
	inputDisparityMap = inputGen.GetLine((width + PADDING), 10);
	inputCostVolume = inputGen.GetLine((width + PADDING) * maxDisp, 10);
	lutToUse = inputGen.GetLine(4096, 0, 32);
	
	inputGen.SelectGenerator("uniform");
	outputDisparityMapC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
        outputDisparityMapAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
	inputGen.FillLine(outputDisparityMapC,   width + PADDING , 5);
	inputGen.FillLine(outputDisparityMapAsm, width + PADDING , 5);
	

	mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMapC, width, maxDisp, numFractionalBit, lutToUse);
	mvcvSubPixelFilter_asm_test(inputDisparityMap, inputCostVolume, outputDisparityMapAsm, width, maxDisp, numFractionalBit, lutToUse);
	
	RecordProperty("CyclePerPixel", mvcvSubPixelFilterCycleCount / (width));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	outputChecker.CompareArrays(outputDisparityMapC, outputDisparityMapAsm, width + PADDING);
}

TEST_F(mvcvSubPixelFilterKernelTest, TestRandomDataDisp64Fractional4)
{
        maxDisp = 64;
	width = randGen->GenerateUInt(8, 1280, 8);
        numFractionalBit = 4;
	
	inputGen.SelectGenerator("random");
	inputDisparityMap = inputGen.GetLine((width + PADDING), 10);
	inputCostVolume = inputGen.GetLine((width + PADDING) * maxDisp, 10);
	lutToUse = inputGen.GetLine(4096, 0, 32);
	
	inputGen.SelectGenerator("uniform");
	outputDisparityMapC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
        outputDisparityMapAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
	inputGen.FillLine(outputDisparityMapC,   width + PADDING , 5);
	inputGen.FillLine(outputDisparityMapAsm, width + PADDING , 5);
	

	mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMapC, width, maxDisp, numFractionalBit, lutToUse);
	mvcvSubPixelFilter_asm_test(inputDisparityMap, inputCostVolume, outputDisparityMapAsm, width, maxDisp, numFractionalBit, lutToUse);
	
	RecordProperty("CyclePerPixel", mvcvSubPixelFilterCycleCount / (width));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	outputChecker.CompareArrays(outputDisparityMapC, outputDisparityMapAsm, width + PADDING);
}

TEST_F(mvcvSubPixelFilterKernelTest, TestRandomDataDisp96Fractional5)
{
        maxDisp = 96;
	width = randGen->GenerateUInt(1280, 1920, 8);
        numFractionalBit = 5;
	
	inputGen.SelectGenerator("random");
	inputDisparityMap = inputGen.GetLine((width + PADDING), 10);
	inputCostVolume = inputGen.GetLine((width + PADDING) * maxDisp, 10);
	lutToUse = inputGen.GetLine(4096, 0, 32);
	
	inputGen.SelectGenerator("uniform");
	outputDisparityMapC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
        outputDisparityMapAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
	inputGen.FillLine(outputDisparityMapC,   width + PADDING , 5);
	inputGen.FillLine(outputDisparityMapAsm, width + PADDING , 5);
	

	mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMapC, width, maxDisp, numFractionalBit, lutToUse);
	mvcvSubPixelFilter_asm_test(inputDisparityMap, inputCostVolume, outputDisparityMapAsm, width, maxDisp, numFractionalBit, lutToUse);
	
	RecordProperty("CyclePerPixel", mvcvSubPixelFilterCycleCount / (width));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	outputChecker.CompareArrays(outputDisparityMapC, outputDisparityMapAsm, width + PADDING);
}

TEST_F(mvcvSubPixelFilterKernelTest, TestRandomDataDisp96Fractional4)
{
        maxDisp = 96;
	width = randGen->GenerateUInt(1280, 1920, 8);
        numFractionalBit = 4;
	
	inputGen.SelectGenerator("random");
	inputDisparityMap = inputGen.GetLine((width + PADDING), 10);
	inputCostVolume = inputGen.GetLine((width + PADDING) * maxDisp, 10);
	lutToUse = inputGen.GetLine(4096, 0, 32);
	
	inputGen.SelectGenerator("uniform");
	outputDisparityMapC   = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
        outputDisparityMapAsm = (unsigned short *)inputGen.GetEmptyLine((width + PADDING)*sizeof(unsigned short));
	inputGen.FillLine(outputDisparityMapC,   width + PADDING , 5);
	inputGen.FillLine(outputDisparityMapAsm, width + PADDING , 5);
	

	mvcvSubPixelFilter(inputDisparityMap, inputCostVolume, outputDisparityMapC, width, maxDisp, numFractionalBit, lutToUse);
	mvcvSubPixelFilter_asm_test(inputDisparityMap, inputCostVolume, outputDisparityMapAsm, width, maxDisp, numFractionalBit, lutToUse);
	
	RecordProperty("CyclePerPixel", mvcvSubPixelFilterCycleCount / (width));

	//outputChecker.DumpArrays("refInput1", "refInput1.c", inLine1, width + PADDING);
	//outputChecker.DumpArrays("refInput2", "refInput2.c", inLine2, width + PADDING);
	//outputChecker.DumpArrays("outC", "outC.c", outLineC, width * disparities + PADDING);
	//outputChecker.DumpArrays("outAsm", "outAsm.c", outLineAsm, width * disparities + PADDING);
	
	outputChecker.CompareArrays(outputDisparityMapC, outputDisparityMapAsm, width + PADDING);
}

