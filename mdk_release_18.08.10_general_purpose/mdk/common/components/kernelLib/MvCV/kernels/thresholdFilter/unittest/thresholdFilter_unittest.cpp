//thresholdFilter kernel test

//Asm function prototype:
//	void mvcvThresholdFilter_asm(fp32* cornerScores, fp32 threshold, u32 width,
//                          		u32 posOffset, u16* candPos, u32* candCount)

//Asm test function prototype:
//	void thresholdFilter_asm_test(float *cornerScores, float threshold, unsigned int width, 
//									unsigned int posOffset, unsigned short *candPos, unsigned int candCount);

//C function prototype:
//   	void mvcvThresholdFilter(fp32* cornerScores, fp32 threshold, u32 width,
//                            		u32 posOffset, u16* candPos, u32* candCount)

#include "gtest/gtest.h"
#include "thresholdFilter.h"
#include "thresholdFilter_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class thresholdFilterTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	fp32 *cornerScores;
	fp32 threshold;
	u32 width;
	u32 posOffset;
	
	u16 *candPos_C;	
	u16 *candPos_Asm;
	u32 *candCount_C;
	u32 *candCount_Asm;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(thresholdFilterTest, TestDummy)
{	
	width = 12;
	posOffset = 0;
	threshold = 5;
	
	inputGen.SelectGenerator("uniform");
	cornerScores = inputGen.GetLineFloat(width, 5);
	candPos_C = inputGen.GetLineU16(width, 0);
	candPos_Asm = inputGen.GetLineU16(width, 0);	

	cornerScores[2] = 1;
	
	candCount_C = (u32*)malloc(sizeof(u32));
	candCount_Asm = (u32*)malloc(sizeof(u32));
	candCount_C[0] = 0;
	candCount_Asm[0] = 0;
	
	thresholdFilter_asm_test(cornerScores, threshold, width, posOffset, candPos_Asm, candCount_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterCycleCount / width);
	mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos_C, candCount_C);
	
	outputCheck.CompareArrays(candPos_C, candPos_Asm, width);
	EXPECT_EQ(candCount_C[0], candCount_Asm[0]);	
}

TEST_F(thresholdFilterTest, TestRandomInputIntoSmallRange)
{	
	posOffset = 1;
	width = 40;
	threshold = 100;

	inputGen.SelectGenerator("uniform");
	candPos_C = inputGen.GetLineU16(width + posOffset, 0);
	candPos_Asm = inputGen.GetLineU16(width + posOffset, 0);	
	
	inputGen.SelectGenerator("uniform");
	cornerScores = inputGen.GetLineFloat(width + posOffset, 99.9, 100.1);

	candCount_C = (u32*)malloc(sizeof(u32));
	candCount_Asm = (u32*)malloc(sizeof(u32));
	candCount_C[0] = 0;
	candCount_Asm[0] = 0;
	
	thresholdFilter_asm_test(cornerScores, threshold, width, posOffset, candPos_Asm, candCount_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterCycleCount / width);
	mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos_C, candCount_C);

	outputCheck.CompareArrays(candPos_C, candPos_Asm, width);
	EXPECT_EQ(candCount_C[0], candCount_Asm[0]);	
}

TEST_F(thresholdFilterTest, TestRandomOffset0)
{		
	posOffset = 0;
	width = 1600;
	threshold = 100;	
	
	inputGen.SelectGenerator("uniform");
	candPos_C = inputGen.GetLineU16(width + posOffset, 0);
	candPos_Asm = inputGen.GetLineU16(width + posOffset, 0);	
	
	inputGen.SelectGenerator("random");
	cornerScores = inputGen.GetLineFloat(width + posOffset, 0, 255);

	candCount_C = (u32*)malloc(sizeof(u32));
	candCount_Asm = (u32*)malloc(sizeof(u32));
	candCount_C[0] = 0;
	candCount_Asm[0] = 0;
	
	thresholdFilter_asm_test(cornerScores, threshold, width, posOffset, candPos_Asm, candCount_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterCycleCount / width);
	mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos_C, candCount_C);
	
	outputCheck.CompareArrays(candPos_C, candPos_Asm, width);
	EXPECT_EQ(candCount_C[0], candCount_Asm[0]);	
}

TEST_F(thresholdFilterTest, TestRandomOffset1)
{		
	posOffset = 2;
	width = 1600;
	threshold = 150;
	
	inputGen.SelectGenerator("uniform");
	candPos_C = inputGen.GetLineU16(width + posOffset, 0);
	candPos_Asm = inputGen.GetLineU16(width + posOffset, 0);	
	
	inputGen.SelectGenerator("random");
	cornerScores = inputGen.GetLineFloat(width + posOffset, 0, 255);

	candCount_C = (u32*)malloc(sizeof(u32));
	candCount_Asm = (u32*)malloc(sizeof(u32));
	candCount_C[0] = 0;
	candCount_Asm[0] = 0;
	
	thresholdFilter_asm_test(cornerScores, threshold, width, posOffset, candPos_Asm, candCount_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterCycleCount / width);
	mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos_C, candCount_C);
	
	outputCheck.CompareArrays(candPos_C, candPos_Asm, width);
	EXPECT_EQ(candCount_C[0], candCount_Asm[0]);	
}

TEST_F(thresholdFilterTest, TestRandomOffset2)
{		
	posOffset = 2;
	width = 800;
	threshold = 100;
	
	inputGen.SelectGenerator("uniform");
	candPos_C = inputGen.GetLineU16(width + posOffset, 0);
	candPos_Asm = inputGen.GetLineU16(width + posOffset, 0);	
	
	inputGen.SelectGenerator("random");
	cornerScores = inputGen.GetLineFloat(width + posOffset, 0, 255);
	
	candCount_C = (u32*)malloc(sizeof(u32));
	candCount_Asm = (u32*)malloc(sizeof(u32));
	candCount_C[0] = 0;
	candCount_Asm[0] = 0;
	
	thresholdFilter_asm_test(cornerScores, threshold, width, posOffset, candPos_Asm, candCount_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterCycleCount / width);
	mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos_C, candCount_C);
	
	outputCheck.CompareArrays(candPos_C, candPos_Asm, width);
	EXPECT_EQ(candCount_C[0], candCount_Asm[0]);	
}

TEST_F(thresholdFilterTest, TestRandomOffset3)
{		
	posOffset = 3;
	width = 1680;
	threshold = 68;	
	
	inputGen.SelectGenerator("uniform");
	candPos_C = inputGen.GetLineU16(width + posOffset, 0);
	candPos_Asm = inputGen.GetLineU16(width + posOffset, 0);	
	
	inputGen.SelectGenerator("random");
	cornerScores = inputGen.GetLineFloat(width + posOffset, 0, 255);

	candCount_C = (u32*)malloc(sizeof(u32));
	candCount_Asm = (u32*)malloc(sizeof(u32));
	candCount_C[0] = 0;
	candCount_Asm[0] = 0;
	
	thresholdFilter_asm_test(cornerScores, threshold, width, posOffset, candPos_Asm, candCount_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterCycleCount / width);
	mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos_C, candCount_C);
	
	outputCheck.CompareArrays(candPos_C, candPos_Asm, width);
	EXPECT_EQ(candCount_C[0], candCount_Asm[0]);	
}

TEST_F(thresholdFilterTest, TestRandomData)
{		
	posOffset = randGen->GenerateUInt(0, 100, 1);
	width = randGen->GenerateUInt(100, 1920, 4);
	threshold = randGen->GenerateUInt(0, 100, 1);
	
	inputGen.SelectGenerator("uniform");
	candPos_C = inputGen.GetLineU16(width + posOffset, 0);
	candPos_Asm = inputGen.GetLineU16(width + posOffset, 0);	
	
	inputGen.SelectGenerator("random");
	cornerScores = inputGen.GetLineFloat(width + posOffset, 0, 255);

	candCount_C = (u32*)malloc(sizeof(u32));
	candCount_Asm = (u32*)malloc(sizeof(u32));
	candCount_C[0] = 0;
	candCount_Asm[0] = 0;
	
	thresholdFilter_asm_test(cornerScores, threshold, width, posOffset, candPos_Asm, candCount_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterCycleCount / width);
	mvcvThresholdFilter(cornerScores, threshold, width, posOffset, candPos_C, candCount_C);
	
	outputCheck.CompareArrays(candPos_C, candPos_Asm, width);
	EXPECT_EQ(candCount_C[0], candCount_Asm[0]);	
}


