//BilinearInterpolationU16 kernel test

//Asm function prototype:
//	void mvcvBilinearInterpolation16u_asm(const u8* src, s32 src_step, u8* dst, s32 dst_step, ClSize win_size, mvCvPoint2D32f center);

//Asm test function prototype:
//   	void bilinearInterpolation16u_asm_test(const unsigned char* src, int src_step, unsigned char* dst, int dst_step, unsigned int* win_size, float* center);

//C function prototype:
//   	void mvcvBilinearInterpolation16u(const u8* src, s32 src_step, u8* dst, s32 dst_step, ClSize win_size, mvCvPoint2D32f center);

#include "gtest/gtest.h"
#include "bilinearInterpolationAligned_u16.h"
#include "bilinearInterpolationAligned_u16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class bilinearInterpolationAligned16uTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	mvCvPoint2D32f center;
	ClSize win_size;
	int src_step;
	int dst_step;

	unsigned char *src;
	unsigned char *dstC;
	unsigned char *dstAsm;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;

	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(bilinearInterpolationAligned16uTest, TestWin_XHigherThen8)
{
	src_step = 19;
	dst_step = 9;
	win_size.x = 13;
	win_size.y = 7;
	center.x = 1.512;
	center.y = 2.8;

	inputGen.SelectGenerator("uniform");
	dstC = inputGen.GetLine(MAX_SIZE, 0);
	dstAsm = inputGen.GetLine(MAX_SIZE, 0);

	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(MAX_SIZE, 0, 255);

	bilinearInterpolationAligned16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int*)&win_size.x, &center.x);
	//RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount / (size));
	mvcvBilinearInterpolationAligned16u(src, src_step, dstC, dst_step, win_size, center);

	outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}

TEST_F(bilinearInterpolationAligned16uTest, TestWin_Zsolt)
{
    src_step = 150;
    dst_step = 0xb;
    win_size.x = 11;
    win_size.y = 11;
    center.x = 58.0;
    center.y = 26.0;

    inputGen.SelectGenerator("uniform");
    dstC = inputGen.GetLine(MAX_SIZE, 0);
    dstAsm = inputGen.GetLine(MAX_SIZE, 0);

    inputGen.SelectGenerator("random");
    src = inputGen.GetLine(MAX_SIZE, 0, 255);

    bilinearInterpolationAligned16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int*)&win_size.x, &center.x);
    //RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount / (size));
    mvcvBilinearInterpolationAligned16u(src, src_step, dstC, dst_step, win_size, center);

    outputCheck.CompareArrays(dstC, dstAsm, MAX_SIZE);
}

