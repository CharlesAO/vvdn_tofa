//cvtColorKernel test
//Asm function prototype:
//    void cvtColorKernelRGBToYUV_asm(u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width);
//
//Asm test function prototype:
//    void RGBtoYUV_asm_test(unsigned char *rgb, unsigned char *y ,unsigned char *u ,unsigned char *v , unsigned int width)
//
//C function prototype:
//    void cvtColorKernelRGBToYUV(u8* in, u8* yOut, u8* uOut, u8* vOut, u32 width);
//
//Parameter description:
// RGBToYUV
// in	- input RGB channel
// yOut	- output Y channel
// uOut	- output U channel
// vOut	- output V channel
// width- image width in pixels


#include "gtest/gtest.h"
#include "cvtColorKernelRGBToYUV.h"
#include "cvtColorKernelRGBToYUV_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"


class cvtColorTest : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	void RGBtoYUVcheck(u8 *Y, u8 *U, u8 *V, int size, u8 y, u8 u, u8 v)
	{
		int i,a;
		for(i=0;i<size;i++)
		{
			if(Y[i]>y)
				a=Y[i]-y;
			else
				a=y-Y[i];
			EXPECT_LE(a,2);
		}
		for(i=0;i<size/4;i++)
		{
			if(U[i]>u)
				a=U[i]-u;
			else
				a=u-U[i];
			EXPECT_LE(a,2);
		}
		for(i=0;i<size/4;i++)
		{
			if(V[i]>v)
				a=V[i]-v;
			else
				a=v-V[i];
			EXPECT_LE(a,2);
		}
	}

	void DeltaCheck(u8* A, u8* B, int size)
	{
		int i,m;
		for(i=0;i<size;i++)
		{
			if(A[i]>=B[i])
				m=A[i]-B[i];
			else
				m=B[i]-A[i];
			EXPECT_LE(m,2);
		}
	}

    u8* y;
    u8* u;
    u8* v;
    u8* yASM;
    u8* uASM;
    u8* vASM;
    u8* rgb;

    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker arrCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

  // virtual void TearDown() {}
};

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUV1)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(32);
	u = inputGen.GetEmptyLine(8);
	v = inputGen.GetEmptyLine(8);
	yASM = inputGen.GetEmptyLine(32);
	uASM = inputGen.GetEmptyLine(8);
	vASM = inputGen.GetEmptyLine(8);
	rgb = inputGen.GetLine(96,55);
	int size = 32;

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 16);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 16);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,32,55,128,128);

	RGBtoYUVcheck(y,u,v,32,55,128,128);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);

}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUV2)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(32);
	u = inputGen.GetEmptyLine(8);
	v = inputGen.GetEmptyLine(8);
	yASM = inputGen.GetEmptyLine(32);
	uASM = inputGen.GetEmptyLine(8);
	vASM = inputGen.GetEmptyLine(8);
	rgb = inputGen.GetLine(96,184);

	int i=0;

	int size=32;
	for(i=0;i<96; i++)
	{
		if(i % 3 == 1)
		{
			rgb[i] = 77;
		}
		if(i % 3 == 2)
		{
			rgb[i] = 120;
		}
	}

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 16);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 16);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,32,114,131,177);

	RGBtoYUVcheck(y,u,v,32,114,131,177);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);
}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUVnullU)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(32);
	u = inputGen.GetEmptyLine(8);
	v = inputGen.GetEmptyLine(8);
	yASM = inputGen.GetEmptyLine(32);
	uASM = inputGen.GetEmptyLine(8);
	vASM = inputGen.GetEmptyLine(8);
	rgb = inputGen.GetLine(96,184);

	int i=0;

	int size=32;
	for(i=0;i<96; i++)
	{
		if(i % 3 == 1)
		{
			rgb[i] = 77;
		}
		if(i % 3 == 2)
		{
			rgb[i] = 120;
		}
	}

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 16);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 16);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,32,114,131,177);

	RGBtoYUVcheck(y,u,v,32,114,131,177);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);
}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUVNullR)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(32);
	u = inputGen.GetEmptyLine(8);
	v = inputGen.GetEmptyLine(8);
	yASM = inputGen.GetEmptyLine(32);
	uASM = inputGen.GetEmptyLine(8);
	vASM = inputGen.GetEmptyLine(8);
	rgb = inputGen.GetLine(96,0);

	int i=0;

	int size=32;
	for(i=0;i<96; i++)
	{
		if(i % 3 == 1)
		{
			rgb[i] = 167;
		}
		if(i % 3 == 2)
		{
			rgb[i] = 20;
		}
	}

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 16);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 16);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,32,100,82,56);

	RGBtoYUVcheck(y,u,v,32,100,82,56);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);
}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUVNullG)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(80);
	u = inputGen.GetEmptyLine(20);
	v = inputGen.GetEmptyLine(20);
	yASM = inputGen.GetEmptyLine(80);
	uASM = inputGen.GetEmptyLine(20);
	vASM = inputGen.GetEmptyLine(20);
	rgb = inputGen.GetLine(240,67);

	int i=0;

	int size=32;
	for(i=0;i<240; i++)
	{
		if(i % 3 == 1)
		{
			rgb[i] = 0;
		}
		if(i % 3 == 2)
		{
			rgb[i] = 200;
		}
	}

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 40);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 40);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,80,42,216,145);

	RGBtoYUVcheck(y,u,v,80,42,216,145);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);
}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUVNullB)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(132);
	u = inputGen.GetEmptyLine(33);
	v = inputGen.GetEmptyLine(33);
	yASM = inputGen.GetEmptyLine(132);
	uASM = inputGen.GetEmptyLine(33);
	vASM = inputGen.GetEmptyLine(33);
	rgb = inputGen.GetLine(396,196);

	int i=0;

	int size=132;

	i=0;


	for(i=0;i<396; i++)
	{
		if(i % 3 == 1)
		{
			rgb[i] = 180;
		}
		if(i % 3 == 2)
		{
			rgb[i] = 0;
		}
	}

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 66);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 66);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,132,164,35,150);
	RGBtoYUVcheck(y,u,v,132,165,35,150);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);

}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUVNullInput)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(64);
	u = inputGen.GetEmptyLine(16);
	v = inputGen.GetEmptyLine(16);
	yASM = inputGen.GetEmptyLine(64);
	uASM = inputGen.GetEmptyLine(16);
	vASM = inputGen.GetEmptyLine(16);
	rgb = inputGen.GetLine(192,0);
	int size=64;

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 32);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 32);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,64,0,128,128);

	RGBtoYUVcheck(y,u,v,64,0,128,128);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);

}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUVMaxInputMaxY)
{
	inputGen.SelectGenerator("uniform");
	y = inputGen.GetEmptyLine(64);
	u = inputGen.GetEmptyLine(16);
	v = inputGen.GetEmptyLine(16);
	yASM = inputGen.GetEmptyLine(64);
	uASM = inputGen.GetEmptyLine(16);
	vASM = inputGen.GetEmptyLine(16);
	rgb = inputGen.GetLine(192,255);
	int size=64;

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 32);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 32);
	RecordProperty("CycleCount", cvtColorRGBtoYUVCycleCount);

	RGBtoYUVcheck(yASM,uASM,vASM,64,255,128,128);

	RGBtoYUVcheck(y,u,v,64,255,128,128);
	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);

}

TEST_F(cvtColorTest , TestUniformInputLineRGBtoYUVRandomInput)
{

	y = inputGen.GetEmptyLine(64);
	u = inputGen.GetEmptyLine(16);
	v = inputGen.GetEmptyLine(16);

	yASM = inputGen.GetEmptyLine(64);
	uASM = inputGen.GetEmptyLine(16);
	vASM = inputGen.GetEmptyLine(16);

	rgb = inputGen.GetEmptyLine(192);

	int i,size=64;

	for(i=0;i<size*3;i++)
	{
		rgb[i]=dataGenerator.GenerateUInt(0,255,1);
	}

	RGBtoYUV_asm_test(rgb,  yASM,  uASM,  vASM, 32);
	mvcvCvtColorKernelRGBToYUV(rgb,  y,  u,  v, 32);

	DeltaCheck(y,yASM,size);
	DeltaCheck(u,uASM,size/4);
	DeltaCheck(v,vASM,size/4);
}
