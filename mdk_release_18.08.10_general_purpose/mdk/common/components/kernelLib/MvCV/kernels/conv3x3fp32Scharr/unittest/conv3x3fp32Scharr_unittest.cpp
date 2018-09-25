//conv3x3fp32 kernel test

//Asm function prototype:
//	 void mvcvconv3x3fp32_asm(const fp32* Ix, ClSize isz, mvCvPoint minI, const fp32* Iy, ClSize jsz, mvCvPoint minJ, mvScalar& G);

//Asm test function prototype:
//   void mvcvConv3x3fp32Scharr_asm_test(float* Ix, unsigned char* isz, unsigned char* minI, float* Iy, unsigned char* jsz, unsigned char* minJ, unsigned char* G);

//C function prototype:
//   void mvcvConv3x3fp32Scharr(const fp32* Ix, ClSize isz, mvCvPoint minI, const fp32* Iy, ClSize jsz, mvCvPoint minJ, mvScalar& G);


#include "gtest/gtest.h"
#include "conv3x3fp32Scharr.h"
#include "conv3x3fp32Scharr_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class conv3x3fp32ScharrTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	u8* src;
	float* dstX_Asm;
	float* dstY_Asm;
	float* dstX_C;
	float* dstY_C;
	float *smooth_k;
    int height;
    int width;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;

	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(conv3x3fp32ScharrTest, TestUniformData)
{
	inputGen.SelectGenerator("uniform");
	dstX_Asm = inputGen.GetLineFloat(11*11, 0);
	dstX_C = inputGen.GetLineFloat(11*11, 0);
	dstY_Asm = inputGen.GetLineFloat(11*11, 0);
	dstY_C = inputGen.GetLineFloat(11*11, 0);
	smooth_k = inputGen.GetLineFloat(2, 0);
	src = inputGen.GetLine(13*13, 10);
    height = 11;
    width = 13;

	smooth_k[0] =  0.09375;
	smooth_k[1] =  0.3125;

	mvcvConv3x3fp32Scharr(src, dstX_C, dstY_C, smooth_k, height, width);
	conv3x3fp32Scharr_asm_test(src, dstX_Asm, dstY_Asm, smooth_k, height,width);
	RecordProperty("CyclePerPixel", conv3x3fp32ScharrCycleCount);

	outputCheck.CompareArraysFloat(dstX_C, dstX_Asm, 11*11);
	outputCheck.CompareArraysFloat(dstY_C, dstY_Asm, 11*11);
}

TEST_F(conv3x3fp32ScharrTest, TestRandomData1)
{
	inputGen.SelectGenerator("uniform");
	dstX_Asm = inputGen.GetLineFloat(11*11, 0);
	dstX_C = inputGen.GetLineFloat(11*11, 0);
	dstY_Asm = inputGen.GetLineFloat(11*11, 0);
	dstY_C = inputGen.GetLineFloat(11*11, 0);
	smooth_k = inputGen.GetLineFloat(2, 0);
	height = 11;
	width = 13;
	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(13*13, 0, 255);

	smooth_k[0] =  0.09375;
	smooth_k[1] =  0.3125;

	mvcvConv3x3fp32Scharr(src, dstX_C, dstY_C, smooth_k, height,width);
	conv3x3fp32Scharr_asm_test(src, dstX_Asm, dstY_Asm, smooth_k, height,width);
	RecordProperty("CyclePerPixel", conv3x3fp32ScharrCycleCount);

	outputCheck.CompareArraysFloat(dstX_C, dstX_Asm, 11*11, 0.1);
	outputCheck.CompareArraysFloat(dstY_C, dstY_Asm, 11*11, 0.1);
}

TEST_F(conv3x3fp32ScharrTest, TestRandomData2)
{
	height = 21;
	width = 21;
	inputGen.SelectGenerator("uniform");
	dstX_Asm = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstX_C = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstY_Asm = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstY_C = inputGen.GetLineFloat((width-2)*(height-2), 0);
	smooth_k = inputGen.GetLineFloat(2, 0);


	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(width*height, 0, 255);

	smooth_k[0] =  0.09375;
	smooth_k[1] =  0.3125;

	mvcvConv3x3fp32Scharr(src, dstX_C, dstY_C, smooth_k, height,width);
	conv3x3fp32Scharr_asm_test(src, dstX_Asm, dstY_Asm, smooth_k, height,width);
	RecordProperty("CyclePerPixel", conv3x3fp32ScharrCycleCount);

	outputCheck.CompareArraysFloat(dstX_C, dstX_Asm, (width-2)*(height-2), 0.1);
	outputCheck.CompareArraysFloat(dstY_C, dstY_Asm, (width-2)*(height-2), 0.1);
}

TEST_F(conv3x3fp32ScharrTest, TestRandomData3)
{
	height = 24;
	width = 13;
	inputGen.SelectGenerator("uniform");
	dstX_Asm = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstX_C = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstY_Asm = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstY_C = inputGen.GetLineFloat((width-2)*(height-2), 0);
	smooth_k = inputGen.GetLineFloat(2, 0);


	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(width*height, 0, 255);

	smooth_k[0] =  0.09375;
	smooth_k[1] =  0.3125;

	mvcvConv3x3fp32Scharr(src, dstX_C, dstY_C, smooth_k, height,width);
	conv3x3fp32Scharr_asm_test(src, dstX_Asm, dstY_Asm, smooth_k, height,width);
	RecordProperty("CyclePerPixel", conv3x3fp32ScharrCycleCount);

	outputCheck.CompareArraysFloat(dstX_C, dstX_Asm, (width-2)*(height-2), 0.1);
	outputCheck.CompareArraysFloat(dstY_C, dstY_Asm, (width-2)*(height-2), 0.1);
}

TEST_F(conv3x3fp32ScharrTest, TestRandomData4)
{
	height = 15;
	width = 26;
	inputGen.SelectGenerator("uniform");
	dstX_Asm = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstX_C = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstY_Asm = inputGen.GetLineFloat((width-2)*(height-2), 0);
	dstY_C = inputGen.GetLineFloat((width-2)*(height-2), 0);
	smooth_k = inputGen.GetLineFloat(2, 0);


	inputGen.SelectGenerator("random");
	src = inputGen.GetLine(width*height, 0, 255);

	smooth_k[0] =  0.09375;
	smooth_k[1] =  0.3125;

	mvcvConv3x3fp32Scharr(src, dstX_C, dstY_C, smooth_k, height,width);
	conv3x3fp32Scharr_asm_test(src, dstX_Asm, dstY_Asm, smooth_k, height,width);
	RecordProperty("CyclePerPixel", conv3x3fp32ScharrCycleCount);

	outputCheck.CompareArraysFloat(dstX_C, dstX_Asm, (width-2)*(height-2), 0.1);
	outputCheck.CompareArraysFloat(dstY_C, dstY_Asm, (width-2)*(height-2), 0.1);
}
