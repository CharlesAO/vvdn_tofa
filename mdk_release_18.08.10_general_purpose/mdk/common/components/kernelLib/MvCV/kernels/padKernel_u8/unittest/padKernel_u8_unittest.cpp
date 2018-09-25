//padKernel_u8 kernel test

//Asm function prototype:
//	void mvcvPadKernel_u8_asm(u8* iBuf, u32 iBufLen, u8* oBuf,
//                        u32 padSz, u32 padMode, u32 padType)

//Asm test function prototype:
//	void mvcvPadKernel_u8_asm_test(unsigned char* iBuf, unsigned int iBufLen,
//                        unsigned char* oBuf, unsigned int padSz, unsigned int padMode, unsigned int padType);

//C function prototype:
//	void mvcvpadKernel_u8(u8* iBuf, u32 iBufLen, u8* oBuf,
//                         u32 padSz, u32 padMode, u32 padType)

#include "gtest/gtest.h"
#include "padKernel_u8_asm_test.h"
#include "padKernel_u8.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <float.h>

#define MAX_PADDING 32

class padKernel_u8Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned char* iBuf;
	unsigned char* oBufC;
	unsigned char* oBufAsm;
	unsigned int iBufLen;
	unsigned int padSz;
	unsigned int padMode;
	unsigned int padType;
	unsigned char pixValue;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


//-------------------------------------------------------------------------------------------
//						case 1 Left Padding
//-------------------------------------------------------------------------------------------

TEST_F(padKernel_u8Test, TestLeftModeRandomPadType)
{
	iBufLen = 320;
	padSz = randomGen.GenerateUInt(1, 16, 1);
	padMode = Left;
	pixValue = 0;

	inputGen.SelectGenerator("random");
	padType = randomGen.GenerateUInt(0, 5, 1);
	if(padType == 5){
		pixValue = randomGen.GenerateUChar(0, 255);
	}

	iBuf = inputGen.GetLine(iBufLen, 0, 255);
	oBufC = inputGen.GetLine(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLine(iBufLen + MAX_PADDING, 0);

	padKernel_u8_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u8CycleCount / (padSz+iBufLen));
	mvcvPadKernel_u8(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz);
}

// -------------------------------------------------------------------------------------------
// 						case 2 Right Padding
// -------------------------------------------------------------------------------------------

TEST_F(padKernel_u8Test, TestRightModeRandomPadType)
{
	iBufLen = 640;
	padSz = randomGen.GenerateUInt(1, 16, 1);
	padMode = Right;
	pixValue = 0;

	inputGen.SelectGenerator("random");
	padType = randomGen.GenerateUInt(0, 5, 1);
	if(padType == 5){
		pixValue = randomGen.GenerateUChar(0, 255);
	}
	iBuf = inputGen.GetLine(iBufLen, 0, 255);
	oBufC = inputGen.GetLine(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLine(iBufLen + MAX_PADDING, 0);

	padKernel_u8_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u8CycleCount / (padSz+iBufLen));
	mvcvPadKernel_u8(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz);
}

// //-------------------------------------------------------------------------------------------
// //						case 3 Left and Right Padding
// //-------------------------------------------------------------------------------------------


TEST_F(padKernel_u8Test, TestLeftAndRightModeRandomPadType)
{
	iBufLen = 1280;
	padSz = randomGen.GenerateUInt(1, 16, 1);
	padMode = LeftAndRight;
	pixValue = 0;

	inputGen.SelectGenerator("random");
	padType = randomGen.GenerateUInt(0, 5, 1);
	if(padType == 5){
		pixValue = randomGen.GenerateUChar(0, 255);
	}
	iBuf = inputGen.GetLine(iBufLen, 0, 255);
	oBufC = inputGen.GetLine(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLine(iBufLen + MAX_PADDING, 0);

	padKernel_u8_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u8CycleCount / (padSz*2+iBufLen));
	mvcvPadKernel_u8(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz * 2 );
}


TEST_F(padKernel_u8Test, TestRandomModeRandomPadType)
{
	iBufLen = 1280;
	pixValue = 0;
	padSz = randomGen.GenerateUInt(1, 16, 1);

	inputGen.SelectGenerator("random");
	padMode = randomGen.GenerateUInt(0, 2, 1);
	padType = randomGen.GenerateUInt(0, 5, 1);
	if(padType == 5){
		pixValue = randomGen.GenerateUChar(0, 255);
	}
	if(padMode == 2){
		padSz = 2* padSz;
	}
	iBuf = inputGen.GetLine(iBufLen, 0, 255);
	oBufC = inputGen.GetLine(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLine(iBufLen + MAX_PADDING, 0);

	padKernel_u8_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u8CycleCount / (padSz+iBufLen));
	mvcvPadKernel_u8(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz);
}

