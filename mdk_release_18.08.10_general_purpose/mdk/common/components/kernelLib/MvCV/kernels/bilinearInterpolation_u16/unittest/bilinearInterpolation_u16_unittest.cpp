//BilinearInterpolationU16 kernel test

//Asm function prototype:
//	void mvcvBilinearInterpolation16u_asm(const u8* src, s32 src_step, u8* dst, s32 dst_step, ClSize win_size, mvCvPoint2D32f center);

//Asm test function prototype:
//   	void bilinearInterpolation16u_asm_test(const unsigned char* src, int src_step, unsigned char* dst, int dst_step, unsigned int* win_size, float* center);

//C function prototype:
//   	void mvcvBilinearInterpolation16u(const u8* src, s32 src_step, u8* dst, s32 dst_step, ClSize win_size, mvCvPoint2D32f center);

#include "gtest/gtest.h"
#include "bilinearInterpolation_u16.h"
#include "bilinearInterpolation_u16_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class bilinearInterpolation16uTest : public ::testing::Test {
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

TEST_F(bilinearInterpolation16uTest, Test_XHigherThen8)
{
	src_step = 15;
	dst_step = 9;
	win_size.x = 13;
	win_size.y = 7;
	center.x = 1.512;
	center.y = 2.999;

	inputGen.SelectGenerator("uniform");
	dstC = inputGen.GetLine(MAX_SIZE, 0);
	dstAsm = inputGen.GetLine(MAX_SIZE, 0);

	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(MAX_SIZE, 0, 255);

	bilinearInterpolation16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int* )(&win_size.x), &center.x);
	RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount);
	mvcvBilinearInterpolation16u(src, src_step, dstC, dst_step, win_size, center);

	outputCheck.CompareArrays(dstC, dstAsm, win_size.x*win_size.y);
}

TEST_F(bilinearInterpolation16uTest, Test2)
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

    bilinearInterpolation16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int* )&win_size.x, &center.x);
    RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount);
    mvcvBilinearInterpolation16u(src, src_step, dstC, dst_step, win_size, center);

    outputCheck.CompareArrays(dstC, dstAsm, win_size.x*win_size.y);
}

TEST_F(bilinearInterpolation16uTest, Test3)
{
	src_step = 1;
	dst_step = 1;
	win_size.x = 3;
	win_size.y = 18;
	center.x = 3.246;
	center.y = 1.852;

	inputGen.SelectGenerator("uniform");
	dstC = inputGen.GetLine(MAX_SIZE, 0);
	dstAsm = inputGen.GetLine(MAX_SIZE, 0);

	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(MAX_SIZE, 0, 255);

	bilinearInterpolation16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int* )&win_size.x, &center.x);
	RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount);
	mvcvBilinearInterpolation16u(src, src_step, dstC, dst_step, win_size, center);

	outputCheck.CompareArrays(dstC, dstAsm, win_size.x*win_size.y);
}

TEST_F(bilinearInterpolation16uTest, Test_XSmallerThen8)
{
	src_step = 3;
	dst_step = 2;
	win_size.x = 5;
	win_size.y = 12;
	center.x = 1.512;
	center.y = 2.999;

	inputGen.SelectGenerator("uniform");
	dstC = inputGen.GetLine(MAX_SIZE, 0);
	dstAsm = inputGen.GetLine(MAX_SIZE, 0);

	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(MAX_SIZE, 0, 255);

	bilinearInterpolation16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int* )&win_size.x, &center.x);
	RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount);
	mvcvBilinearInterpolation16u(src, src_step, dstC, dst_step, win_size, center);

	outputCheck.CompareArrays(dstC, dstAsm, win_size.x*win_size.y);
}

TEST_F(bilinearInterpolation16uTest, Test_19x19)
{
	src_step = 24;
	dst_step = 21;
	win_size.x = 19;
	win_size.y = 19;
	center.x = 1.512;
	center.y = 2.999;

	inputGen.SelectGenerator("uniform");
	dstC = inputGen.GetLine(MAX_SIZE, 0);
	dstAsm = inputGen.GetLine(MAX_SIZE, 0);

	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(MAX_SIZE, 0, 255);

	bilinearInterpolation16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int* )&win_size.x, &center.x);
	RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount);
	mvcvBilinearInterpolation16u(src, src_step, dstC, dst_step, win_size, center);

	outputCheck.CompareArrays(dstC, dstAsm, win_size.x*win_size.y);
}

TEST_F(bilinearInterpolation16uTest, Test_21x21)
{
	src_step = 41;
	dst_step = 32;
	win_size.x = 21;
	win_size.y = 21;
	center.x = 1.512;
	center.y = 2.999;

	inputGen.SelectGenerator("uniform");
	dstC = inputGen.GetLine(MAX_SIZE, 0);
	dstAsm = inputGen.GetLine(MAX_SIZE, 0);

	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(MAX_SIZE, 0, 255);

	bilinearInterpolation16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int* )&win_size.x, &center.x);
	RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount);
	mvcvBilinearInterpolation16u(src, src_step, dstC, dst_step, win_size, center);

	outputCheck.CompareArrays(dstC, dstAsm, win_size.x*win_size.y);
}

TEST_F(bilinearInterpolation16uTest, TestRandomData)
{
	src_step =   randGen->GenerateUInt(10, 20, 1);
	dst_step =   randGen->GenerateUInt(3, 15, 1);
	win_size.x = randGen->GenerateUInt(3, dst_step + 1, 1);
	win_size.y = randGen->GenerateUInt(3, 50, 1);
	center.x =   randGen->GenerateFloat(win_size.x/2-1, win_size.x/2+1);
	center.y =   randGen->GenerateFloat(win_size.y/2-1, win_size.y/2+1);

	/* printf("%d\n", src_step );
	printf("%d\n", dst_step );
	printf("%d\n", win_size.x );
	printf("%d\n", win_size.y );
	printf("%f\n", center.x );
	printf("%f\n", center.y ); */
	
	inputGen.SelectGenerator("uniform");
	dstC = inputGen.GetLine(MAX_SIZE, 0);
	dstAsm = inputGen.GetLine(MAX_SIZE, 0);

	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(MAX_SIZE, 0, 255);

	bilinearInterpolation16u_asm_test(src, src_step, dstAsm, dst_step, (unsigned int* )&win_size.x, &center.x);
	RecordProperty("CyclePerPixel", bilinearInterpolation16uCycleCount);
	mvcvBilinearInterpolation16u(src, src_step, dstC, dst_step, win_size, center);

	outputCheck.CompareArrays(dstC, dstAsm, win_size.x*win_size.y);
}
