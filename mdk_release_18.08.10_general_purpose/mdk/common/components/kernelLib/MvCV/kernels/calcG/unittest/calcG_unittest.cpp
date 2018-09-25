//CalcG kernel test

//Asm function prototype:
//	 void mvcvCalcG_asm(const fp32* Ix, ClSize isz, mvCvPoint minI, const fp32* Iy, ClSize jsz, mvCvPoint minJ, mvScalar& G);

//Asm test function prototype:
//   void calcG_asm_test(float* Ix, unsigned char* isz, unsigned char* minI, float* Iy, unsigned char* jsz, unsigned char* minJ, unsigned char* G);

//C function prototype:
//   void mvcvCalcG(const fp32* Ix, ClSize isz, mvCvPoint minI, const fp32* Iy, ClSize jsz, mvCvPoint minJ, mvScalar& G);

#include "gtest/gtest.h"
#include "calcG.h"
#include "calcG_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>


class calcGTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	ClSizeW isz;
	ClSizeW jsz;
	mvCvPointW minI;
	mvCvPointW minJ;
	mvScalar gAsm;
	mvScalar gC;
	float* ix;
	float* iy;
	
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(calcGTest, Test1)
{
	gAsm = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);
	gC = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);
	isz.x = 8;
	isz.y = 16;
	jsz.x = 8;
	jsz.y = 16;
	minI.x = 7;
	minI.y = 4;
	minJ.x = 3;
	minJ.y = 4;
	
	inputGen.SelectGenerator("uniform");
	ix = inputGen.GetLineFloat(1920, 6);
	iy = inputGen.GetLineFloat(1920, 2);
	
	calcG_asm_test(ix, (u32*)&isz.x, (u32*)&minI.x, iy, (u32*)&jsz.x, (u32*)&minJ.x, (u32*)&gAsm[0]);
	RecordProperty("CyclePerPixel", calcGCycleCount);
	mvcvCalcG(&ix[500], isz, minI, &iy[500], jsz, minJ, gC);	
	
	ASSERT_NEAR(gC[0], gAsm[0], 0.1f);
	ASSERT_NEAR(gC[1], gAsm[1], 0.1f);
	ASSERT_NEAR(gC[2], gAsm[2], 0.1f);
	ASSERT_NEAR(gC[3], gAsm[3], 0.1f);
}

TEST_F(calcGTest, Test2)
{
	gAsm = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);
	gC = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);
	isz.x = 8;
	isz.y = 16;	
	jsz.x = 8;
	jsz.y = 16;	
	minI.x = 7;
	minI.y = 4;	
	minJ.x = 8;
	minJ.y = 4;
	
	inputGen.SelectGenerator("random");
	ix = inputGen.GetLineFloat(1920, 1, 3);
	iy = inputGen.GetLineFloat(1920, 1, 3);
	
	calcG_asm_test(ix, (u32*)&isz.x, (u32*)&minI.x, iy, (u32*)&jsz.x, (u32*)&minJ.x, (u32*)&gAsm[0]);
	RecordProperty("CyclePerPixel", calcGCycleCount);
	mvcvCalcG(&ix[500], isz, minI, &iy[500], jsz, minJ, gC);	
	
	ASSERT_NEAR(gC[0], gAsm[0], 0.5f);
	ASSERT_NEAR(gC[1], gAsm[1], 0.5f);
	ASSERT_NEAR(gC[2], gAsm[2], 0.5f);
	ASSERT_NEAR(gC[3], gAsm[3], 0.9f);
}

TEST_F(calcGTest, Test3)
{
	gAsm = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);
	gC = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);
	isz.x = 11;
	isz.y = 4;	
	jsz.x = 6;
	jsz.y = 5;	
	minI.x = 7;
	minI.y = 6;	
	minJ.x = 3;
	minJ.y = 4;
	
	inputGen.SelectGenerator("uniform");
	ix = inputGen.GetLineFloat(1920, 10);
	iy = inputGen.GetLineFloat(1920, 30);
	
	calcG_asm_test(ix, (u32*)&isz.x, (u32*)&minI.x, iy, (u32*)&jsz.x, (u32*)&minJ.x, (u32*)&gAsm[0]);
	RecordProperty("CyclePerPixel", calcGCycleCount);
	mvcvCalcG(&ix[500], isz, minI, &iy[500], jsz, minJ, gC);	
	
	ASSERT_NEAR(gC[0], gAsm[0], 0.1f);
	ASSERT_NEAR(gC[1], gAsm[1], 0.1f);
	ASSERT_NEAR(gC[2], gAsm[2], 0.1f);
	ASSERT_NEAR(gC[3], gAsm[3], 0.1f);
}

TEST_F(calcGTest, Test4)
{
	gAsm = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);
	gC = mvScalar(0.0f, 0.0f, 0.0f, 0.0f);

	isz.x = randGen->GenerateUInt(5, 14, 1);
	isz.y = randGen->GenerateUInt(2, 10, 1);
	jsz.x = randGen->GenerateUInt(4, isz.x, 1);
	jsz.y = randGen->GenerateUInt(1, isz.y, 1);	
	minI.x = randGen->GenerateUInt(0, isz.x, 1);
	minI.y = randGen->GenerateUInt(0, isz.y, 1);
	minJ.x = randGen->GenerateUInt(0, jsz.x, 1);
	minJ.y = randGen->GenerateUInt(0, jsz.y, 1);
	
	inputGen.SelectGenerator("random");
	ix = inputGen.GetLineFloat(1920, 0, 10);
	iy = inputGen.GetLineFloat(1920, 0, 10);

	calcG_asm_test(ix, (u32*)&isz.x, (u32*)&minI.x, iy, (u32*)&jsz.x, (u32*)&minJ.x, (u32*)&gAsm[0]);
	RecordProperty("CyclePerPixel", calcGCycleCount);
	mvcvCalcG(&ix[500], isz, minI, &iy[500], jsz, minJ, gC);	
	
	ASSERT_NEAR(gC[0], gAsm[0], 2.0f);
	ASSERT_NEAR(gC[1], gAsm[1], 2.0f);
	ASSERT_NEAR(gC[2], gAsm[2], 2.0f);
	ASSERT_NEAR(gC[3], gAsm[3], 3.0f);
}
