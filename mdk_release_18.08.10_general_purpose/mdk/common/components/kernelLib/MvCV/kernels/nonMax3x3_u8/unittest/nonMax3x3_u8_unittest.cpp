//nonMax3x3_u8 kernel test

//Asm function prototype:
//	void mvcvNonMax3x3_u8_asm(u32 width, u8* corners1, u8* corners2,
//								u8* corners3, u16* candOutPos, u32 *candOutCount)

//Asm test function prototype:
//	void mvcvNonMax3x3_u8_asm_test(u32 width, u8* corners1, u8* corners2,
//									u8* corners3, u16* candOutPos, u32 *candOutCount)

//C function prototype:
//	void mvcvNonMax3x3_u8(u32 width, u8* corners1, u8* corners2,
//							u8* corners3, u16* candOutPos, u32 *candOutCount)

#include "gtest/gtest.h"
#include "nonMax3x3_u8_asm_test.h"
#include "nonMax3x3_u8.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <float.h>

#define PADDING 16

class NonMax3x3_u8Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	u32 width;
	u8* corners1;
	u8* corners2;
	u8* corners3;
	
	u32 candOutCount_C;
	u32 candOutCount_Asm;
	u16* candOutPos_C;
	u16* candOutPos_Asm;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(NonMax3x3_u8Test, TestUniformData)
{
	width = 320;	
	
	inputGen.SelectGenerator("uniform");
	corners1 = inputGen.GetLine(width + PADDING, 10);
	corners2 = inputGen.GetLine(width + PADDING, 20);
	corners3 = inputGen.GetLine(width + PADDING, 15);
	
	corners2[20] = 40;
	
	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(width + PADDING, 0);
	candOutPos_Asm = inputGen.GetLineU16(width + PADDING, 0);
		
	nonMax3x3_u8_asm_test(width, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);
	RecordProperty("CyclePerPixel", nonMax3x3_u8CycleCount / width);
	mvcvNonMax3x3_u8(width, corners1 + PADDING/2, corners2 + PADDING/2, corners3 + PADDING/2, candOutPos_C, &candOutCount_C);
	
	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	ASSERT_EQ(candOutPos_C[0], candOutPos_C[0]);
}

TEST_F(NonMax3x3_u8Test, TestMinimuWidth)
{
	width = 1;	
	
	inputGen.SelectGenerator("random");
	corners1 = inputGen.GetLine(width + PADDING, 10, 20);
	corners2 = inputGen.GetLine(width + PADDING, 50, 100);
	corners3 = inputGen.GetLine(width + PADDING, 10, 20);
	
	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(width + PADDING, 0);
	candOutPos_Asm = inputGen.GetLineU16(width + PADDING, 0);
		
	nonMax3x3_u8_asm_test(width, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);
	RecordProperty("CyclePerPixel", nonMax3x3_u8CycleCount / width);
	mvcvNonMax3x3_u8(width, corners1 + PADDING/2, corners2 + PADDING/2, corners3 + PADDING/2, candOutPos_C, &candOutCount_C);
	
	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}

TEST_F(NonMax3x3_u8Test, TestMaximumWidth)
{
	width = 1280;	
	
	inputGen.SelectGenerator("random");
	corners1 = inputGen.GetLine(width + PADDING, 0, 255);
	corners2 = inputGen.GetLine(width + PADDING, 0, 255);
	corners3 = inputGen.GetLine(width + PADDING, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(width + PADDING, 0);
	candOutPos_Asm = inputGen.GetLineU16(width + PADDING, 0);
		
	nonMax3x3_u8_asm_test(width, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);
	RecordProperty("CyclePerPixel", nonMax3x3_u8CycleCount / width);
	mvcvNonMax3x3_u8(width, corners1+ PADDING/2, corners2 + PADDING/2, corners3+ PADDING/2, candOutPos_C, &candOutCount_C);
		
	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}

TEST_F(NonMax3x3_u8Test, TestZero)
{
	width = 64;	
	
	inputGen.SelectGenerator("uniform");
	corners1 = inputGen.GetLine(width + PADDING, 0);
	corners2 = inputGen.GetLine(width + PADDING, 0);
	corners3 = inputGen.GetLine(width + PADDING, 0);
	
	candOutPos_C = inputGen.GetLineU16(width + PADDING, 0);
	candOutPos_Asm = inputGen.GetLineU16(width + PADDING, 0);
		
	nonMax3x3_u8_asm_test(width, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);
	RecordProperty("CyclePerPixel", nonMax3x3_u8CycleCount / width);	
	mvcvNonMax3x3_u8(width, corners1 + PADDING/2, corners2 + PADDING/2, corners3 + PADDING/2, candOutPos_C, &candOutCount_C);
	
	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}

TEST_F(NonMax3x3_u8Test, TestRandomData)
{
	width = randGen->GenerateUInt(3, 1920, 1);	
	
	inputGen.SelectGenerator("random");
	corners1 = inputGen.GetLine(width + PADDING, 0, 255);
	corners2 = inputGen.GetLine(width + PADDING, 0, 255);
	corners3 = inputGen.GetLine(width + PADDING, 0, 255);
	
	inputGen.SelectGenerator("uniform");
	candOutPos_C = inputGen.GetLineU16(width + PADDING, 0);
	candOutPos_Asm = inputGen.GetLineU16(width + PADDING, 0);
		
	nonMax3x3_u8_asm_test(width, corners1, corners2, corners3, candOutPos_Asm, &candOutCount_Asm);
	RecordProperty("CyclePerPixel", nonMax3x3_u8CycleCount / width);
	mvcvNonMax3x3_u8(width, corners1 + PADDING/2, corners2 + PADDING/2, corners3 + PADDING/2, candOutPos_C, &candOutCount_C);
	
	ASSERT_EQ(candOutCount_C, candOutCount_Asm);
	outputCheck.CompareArrays(candOutPos_C, candOutPos_Asm, candOutCount_C);
}
