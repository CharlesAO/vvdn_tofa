//padKernel_u16 kernel test

//Asm function prototype:
//	void mvcvPadKernel_u16_asm(u16* iBuf, u32 iBufLen, u16* oBuf,
//                        u32 padSz, u32 padMode, u32 padType)

//Asm test function prototype:
//	void mvcvPadKernel_u16_asm_test(unsigned char* iBuf, unsigned int iBufLen,
//                        unsigned char* oBuf, unsigned int padSz, unsigned int padMode, unsigned int padType);

//C function prototype:
//	void mvcvpadKernel_u16(u16* iBuf, u32 iBufLen, u16* oBuf,
//                         u32 padSz, u32 padMode, u32 padType)

#include "gtest/gtest.h"
#include "padKernel_u16_asm_test.h"
#include "padKernel_u16.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <float.h>

#define MAX_PADDING 32

class padKernel_u16Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	unsigned short* iBuf;
	unsigned short* oBufC;
	unsigned short* oBufAsm;
	unsigned int iBufLen;
	unsigned int padSz;
	unsigned int padMode;
	unsigned int padType;
	unsigned short pixValue;

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

TEST_F(padKernel_u16Test, TestLeftModeRandomPadType)
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

	iBuf = inputGen.GetLineU16(iBufLen, 0, 1920);
	oBufC = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);

	padKernel_u16_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u16CycleCount / (padSz+iBufLen));
	mvcvPadKernel_u16(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz);
}

// -------------------------------------------------------------------------------------------
// 						case 2 Right Padding
// -------------------------------------------------------------------------------------------

TEST_F(padKernel_u16Test, TestRightModeRandomPadType)
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
	iBuf = inputGen.GetLineU16(iBufLen, 0, 1920);
	oBufC = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);

	padKernel_u16_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u16CycleCount / (padSz+iBufLen));
	mvcvPadKernel_u16(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz);
}

// //-------------------------------------------------------------------------------------------
// //						case 3 Left and Right Padding
// //-------------------------------------------------------------------------------------------


TEST_F(padKernel_u16Test, TestLeftAndRightModeRandomPadType)
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
	iBuf = inputGen.GetLineU16(iBufLen, 0, 1920);
	oBufC = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);

	padKernel_u16_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u16CycleCount / (padSz*2+iBufLen));
	mvcvPadKernel_u16(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz * 2 );
}


TEST_F(padKernel_u16Test, TestRandomModeRandomPadType)
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
	iBuf = inputGen.GetLineU16(iBufLen, 0, 1920);
	oBufC = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);
	oBufAsm = inputGen.GetLineU16(iBufLen + MAX_PADDING, 0);

	padKernel_u16_asm_test(iBuf, iBufLen, oBufAsm, padSz, padMode, padType, pixValue);
	RecordProperty("CyclePerPixel", padKernel_u16CycleCount / (padSz+iBufLen));
	mvcvPadKernel_u16(iBuf, iBufLen, oBufC, padSz, padMode, padType, pixValue);

	outputCheck.CompareArrays(oBufC, oBufAsm, iBufLen + padSz);
}