//thresholdFilterS16 kernel test
//
//Asm function prototype:
//	void mvcvThresholdFilterS16_asm(s16* line, u16 threshold, u32 width, u32* indexList, u32* count)
//
//Asm test function prototype:
//void thresholdFilterS16_asm_test(short *line, unsigned short threshold, unsigned int width, 
//								 unsigned int *indexList, unsigned int *count);
//
//C function prototype:
//void mvcvThresholdFilterS16(s16* line, u16 threshold, u32 width, u32* indexList, u32* count)

#include "gtest/gtest.h"
#include "thresholdFilterS16.h"
#include "thresholdFilterS16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class thresholdFilterS16Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
short *line;
u32 threshold;
u32 width;

u32 *indexList_C;
u32 *indexList_Asm;
u32 *count_C;
u32 *count_Asm;

	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(thresholdFilterS16Test, TestDummy)
{	
	width = 24;
	threshold = 15;
	
	inputGen.SelectGenerator("random");
	line = (short *)inputGen.GetLineU16(width, 1, 5);	
	indexList_C =  (unsigned int *)inputGen.GetLineU16(2*width, 0);
	indexList_Asm =(unsigned int *)inputGen.GetLineU16(2*width, 0);
	
	count_C      = (u32*)malloc(sizeof(u32));
	count_Asm    = (u32*)malloc(sizeof(u32));
	count_C[0]   = 0;
	count_Asm[0] = 0;
	
	thresholdFilterS16_asm_test(line, threshold, width, indexList_Asm, count_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterS16CycleCount / width);
	mvcvThresholdFilterS16(line, threshold, width, indexList_C, count_C);

	outputCheck.CompareArrays((unsigned char*)indexList_C,(unsigned char*) indexList_Asm, count_Asm[0]);
	EXPECT_EQ(count_C[0], count_Asm[0]);	
}

TEST_F(thresholdFilterS16Test, TestAllInputEqualsThreshold)
{	
	width = 320;
	threshold = 25;
	
	inputGen.SelectGenerator("uniform");
	line = (short *)inputGen.GetLineU16(width, 5);	
	indexList_C =  (unsigned int *)inputGen.GetLineU16(2*width, 0);
	indexList_Asm =(unsigned int *)inputGen.GetLineU16(2*width, 0);
	
	count_C      = (u32*)malloc(sizeof(u32));
	count_Asm    = (u32*)malloc(sizeof(u32));
	count_C[0]   = 0;
	count_Asm[0] = 0;
	
	thresholdFilterS16_asm_test(line, threshold, width, indexList_Asm, count_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterS16CycleCount / width);
	mvcvThresholdFilterS16(line, threshold, width, indexList_C, count_C);

	outputCheck.CompareArrays((unsigned char*)indexList_C,(unsigned char*) indexList_Asm, count_Asm[0]);
	EXPECT_EQ(count_C[0], count_Asm[0]);	
}

TEST_F(thresholdFilterS16Test, TestThresholdMinimum)
{	
	width = 320;
	threshold = 0;
	
	inputGen.SelectGenerator("random");
	line = (short *)inputGen.GetLineU16(width, 1, 10);	
	indexList_C =  (unsigned int *)inputGen.GetLineU16(2*width, 0);
	indexList_Asm =(unsigned int *)inputGen.GetLineU16(2*width, 0);
	
	count_C      = (u32*)malloc(sizeof(u32));
	count_Asm    = (u32*)malloc(sizeof(u32));
	count_C[0]   = 0;
	count_Asm[0] = 0;
	
	thresholdFilterS16_asm_test(line, threshold, width, indexList_Asm, count_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterS16CycleCount / width);
	mvcvThresholdFilterS16(line, threshold, width, indexList_C, count_C);

	outputCheck.CompareArrays((unsigned char*)indexList_C,(unsigned char*) indexList_Asm, count_Asm[0]);
	EXPECT_EQ(count_C[0], count_Asm[0]);	
}

TEST_F(thresholdFilterS16Test, TestThresholdMaximum)
{	
	width = 1280;
	threshold = 105;
	
	inputGen.SelectGenerator("random");
	line = (short *)inputGen.GetLineU16(width, 1, 10);	
	indexList_C =  (unsigned int *)inputGen.GetLineU16(2*width, 0);
	indexList_Asm =(unsigned int *)inputGen.GetLineU16(2*width, 0);
	
	count_C      = (u32*)malloc(sizeof(u32));
	count_Asm    = (u32*)malloc(sizeof(u32));
	count_C[0]   = 0;
	count_Asm[0] = 0;
	
	thresholdFilterS16_asm_test(line, threshold, width, indexList_Asm, count_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterS16CycleCount / width);
	mvcvThresholdFilterS16(line, threshold, width, indexList_C, count_C);

	outputCheck.CompareArrays((unsigned char*)indexList_C,(unsigned char*) indexList_Asm, count_Asm[0]);
	EXPECT_EQ(count_C[0], count_Asm[0]);	
}

TEST_F(thresholdFilterS16Test, TestRandomInputIntoSmallRange)
{	
	
	width = 16;
	threshold = 36;
	
	inputGen.SelectGenerator("random");
	line = (short *)inputGen.GetLineU16(width, 0, 10);	
	indexList_C =  (unsigned int *)inputGen.GetLineU16(2*width, 0);
	indexList_Asm =(unsigned int *)inputGen.GetLineU16(2*width, 0);
	
	count_C      = (u32*)malloc(sizeof(u32));
	count_Asm    = (u32*)malloc(sizeof(u32));
	count_C[0]   = 0;
	count_Asm[0] = 0;
	
	thresholdFilterS16_asm_test(line, threshold, width, indexList_Asm, count_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterS16CycleCount / width);
	mvcvThresholdFilterS16(line, threshold, width, indexList_C, count_C);
	
	outputCheck.CompareArrays(indexList_C, indexList_Asm, count_Asm[0]);
	EXPECT_EQ(count_C[0], count_Asm[0]);	
}

TEST_F(thresholdFilterS16Test, TestRandomInputIntoBigRange)
{		
	width = 1920;
	threshold = 2000;
	
	inputGen.SelectGenerator("random");
	line = (short *)inputGen.GetLineU16(width, 0, 2000);
	indexList_C =  (unsigned int *)inputGen.GetLineU16(2*width, 0);
	indexList_Asm =(unsigned int *)inputGen.GetLineU16(2*width, 0);

	count_C      = (u32*)malloc(sizeof(u32));
	count_Asm    = (u32*)malloc(sizeof(u32));
	count_C[0]   = 0;
	count_Asm[0] = 0;
	
	thresholdFilterS16_asm_test(line, threshold, width, indexList_Asm, count_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterS16CycleCount / width);
	mvcvThresholdFilterS16(line, threshold, width, indexList_C, count_C);
	
	outputCheck.CompareArrays((unsigned char*)indexList_C,(unsigned char*) indexList_Asm, count_C[0]);
	EXPECT_EQ(count_C[0], count_Asm[0]);	
}

TEST_F(thresholdFilterS16Test, TestThresholdEqualsSomeInputs)
{		
	width = 640;
	threshold = 4*256*256;
	
	inputGen.SelectGenerator("random");
	line = (short *)inputGen.GetLineU16(width, 0, 2000);
	indexList_C =  (unsigned int *)inputGen.GetLineU16(2*width, 0);
	indexList_Asm =(unsigned int *)inputGen.GetLineU16(2*width, 0);

	line[1] = 2*256;
	line[50] = 2*256;
	line[100] = 2*256;
	line[200] = 2*256;
	count_C      = (u32*)malloc(sizeof(u32));
	count_Asm    = (u32*)malloc(sizeof(u32));
	count_C[0]   = 0;
	count_Asm[0] = 0;
	
	thresholdFilterS16_asm_test(line, threshold, width, indexList_Asm, count_Asm);
	RecordProperty("CyclePerPixel", thresholdFilterS16CycleCount / width);
	mvcvThresholdFilterS16(line, threshold, width, indexList_C, count_C);
	
	outputCheck.CompareArrays((unsigned char*)indexList_C,(unsigned char*) indexList_Asm, count_C[0]);
	EXPECT_EQ(count_C[0], count_Asm[0]);	
}
