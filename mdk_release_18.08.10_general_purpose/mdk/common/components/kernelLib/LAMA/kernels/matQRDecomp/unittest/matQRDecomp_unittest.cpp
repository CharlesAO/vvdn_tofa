///matrix QR decomposition - Householder method
/// @param[in]  m        - structure that contains info regarding width, height, pointer to input matrix
/// @param[in] Q0, Q1   - structure that contains info regarding width, height, pointer to temporary matrices needed for computation
/// @param[in] Z0, Z1   - structure that contains info regarding width, height, pointer to temporary matrices needed for computation
/// @param[out] Q        - structure that contains info regarding width, height, pointer to Q matrix
/// @param[out] R        - structure that contains info regarding width, height, pointer to R matrix

#include "gtest/gtest.h"
///
#include <matQRDecomp.h>
#include "matQRDecomp_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include "math.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define DELTA          5.0e-2f //accepted tolerance between C and ASM results
#define EDGE 16

class matQRDecompKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}


	unsigned int width;
	unsigned int height;

	MAT *A;
	MAT *Q_c;
	MAT *Q_asm;
	MAT *R_c;
	MAT *R_asm;
	MAT *Q0, *Q1, *Z0, *Z1;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}

	void InitTempMat()
	{

		unsigned int max = width;
		if (height > max ) max = height;


		Q0      = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
		Q1      = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
		Z0      = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
		Z1      = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

		Q0->m   = Q0->n = height;
		Q1->m   = Q1->n = height;
		Z0->m   = Z1->m = height;
		Z0->n   = Z1->n = width;

		Q0->base     = (float*)inputGen.GetLineFloat(max * max, 0);
		Q1->base     = (float*)inputGen.GetLineFloat(max * max, 0);
		Z0->base     = (float*)inputGen.GetLineFloat(max * max, 0);
		Z1->base     = (float*)inputGen.GetLineFloat(max * max, 0);
	}
};

TEST_F(matQRDecompKernelTest, TestMatrix12x12)
{
	width = 12;
	height = 12;

	unsigned int max = width;
	if (height > max ) max = height;

	inputGen.SelectGenerator("random");

	A       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);


	A->m    = height;           Q_c->m    = height;           Q_asm->m    = height;            R_c->m    = height;		R_asm->m    = height;
	A->n    = width;            Q_c->n    = height;           Q_asm->n    = height;            R_c->n    = width;		R_asm->n    = width;

	A->base     = (float*)inputGen.GetLineFloat(max * max, 0, 255);
	Q_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	Q_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);

	InitTempMat();

	memcpy(Q_asm->base, Q_c->base, sizeof(float) * (max * max + EDGE));
	memcpy(R_asm->base, R_c->base, sizeof(float) * (max * max + EDGE));

	mvcvMatQRDecomp(A, Q_c, R_c, Q0, Q1, Z0, Z1);
	matQRDecomp_asm_test((MAT_test *)A, (MAT_test *)Q_asm, (MAT_test *)R_asm);

	RecordProperty("CycleCount", matQRDecompCycleCount);

	outputChecker.CompareArraysFloat(Q_c->base, Q_asm->base, max * max + EDGE, DELTA);
	outputChecker.CompareArraysFloat(R_c->base, R_asm->base, max * max + EDGE, DELTA);

}

TEST_F(matQRDecompKernelTest, TestMatrix32x32)
{
	width = 32;
	height = 32;

	unsigned int max = width;
	if (height > max ) max = height;


	inputGen.SelectGenerator("random");

	A       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

	A->m    = height;           Q_c->m    = height;           Q_asm->m    = height;            R_c->m    = height;		R_asm->m    = height;
	A->n    = width;            Q_c->n    = height;           Q_asm->n    = height;            R_c->n    = width;		R_asm->n    = width;

	A->base     = (float*)inputGen.GetLineFloat(max * max, 0, 255);
	Q_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	Q_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);

	InitTempMat();

	memcpy(Q_asm->base, Q_c->base, sizeof(float) * (max * max + EDGE));
	memcpy(R_asm->base, R_c->base, sizeof(float) * (max * max + EDGE));

	mvcvMatQRDecomp(A, Q_c, R_c, Q0, Q1, Z0, Z1);
	matQRDecomp_asm_test((MAT_test *)A, (MAT_test *)Q_asm, (MAT_test *)R_asm);

	RecordProperty("CycleCount", matQRDecompCycleCount);


	outputChecker.CompareArraysFloat(Q_c->base, Q_asm->base, max * max + EDGE, DELTA);
	outputChecker.CompareArraysFloat(R_c->base, R_asm->base, max * max + EDGE, DELTA);

}

TEST_F(matQRDecompKernelTest, TestMatrixRandom)
{
	width = randGen->GenerateUInt(4, 48, 4);
	height = randGen->GenerateUInt(4, 48, 4);

	unsigned int max = width;
	if (height > max ) max = height;

	inputGen.SelectGenerator("random");

	A       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

	A->m    = height;           Q_c->m    = height;           Q_asm->m    = height;            R_c->m    = height;		R_asm->m    = height;
	A->n    = width;            Q_c->n    = height;           Q_asm->n    = height;            R_c->n    = width;		R_asm->n    = width;

	A->base     = (float*)inputGen.GetLineFloat(max * max, 0, 255);
	Q_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	Q_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);

	InitTempMat();

	memcpy(Q_asm->base, Q_c->base, sizeof(float) * (max * max + EDGE));
	memcpy(R_asm->base, R_c->base, sizeof(float) * (max * max + EDGE));

	mvcvMatQRDecomp(A, Q_c, R_c, Q0, Q1, Z0, Z1);
	matQRDecomp_asm_test((MAT_test *)A, (MAT_test *)Q_asm, (MAT_test *)R_asm);

	RecordProperty("CycleCount", matQRDecompCycleCount);

	outputChecker.CompareArraysFloat(Q_c->base, Q_asm->base, max * max + EDGE, DELTA);
	outputChecker.CompareArraysFloat(R_c->base, R_asm->base, max * max + EDGE, DELTA);
}

TEST_F(matQRDecompKernelTest, TestMatrix48x48)
{
	width =  48;
	height = 48;

	unsigned int max = width;
	if (height > max ) max = height;

	inputGen.SelectGenerator("random");

	A       = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	Q_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_c     = (MAT *)inputGen.GetLine(sizeof(MAT), 0);
	R_asm   = (MAT *)inputGen.GetLine(sizeof(MAT), 0);

	A->m    = height;           Q_c->m    = height;           Q_asm->m    = height;            R_c->m    = height;		R_asm->m    = height;
	A->n    = width;            Q_c->n    = height;           Q_asm->n    = height;            R_c->n    = width;		R_asm->n    = width;

	A->base     = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	Q_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	Q_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_c->base   = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);
	R_asm->base = (float*)inputGen.GetLineFloat(max * max + EDGE, 0, 255);

	InitTempMat();

	memcpy(Q_asm->base, Q_c->base, sizeof(float) * (max * max + EDGE));
	memcpy(R_asm->base, R_c->base, sizeof(float) * (max * max + EDGE));

	mvcvMatQRDecomp(A, Q_c, R_c, Q0, Q1, Z0, Z1);
	matQRDecomp_asm_test((MAT_test *)A, (MAT_test *)Q_asm, (MAT_test *)R_asm);

	RecordProperty("CycleCount", matQRDecompCycleCount);

	outputChecker.CompareArraysFloat(Q_c->base, Q_asm->base, max * max + EDGE, DELTA);
	outputChecker.CompareArraysFloat(R_c->base, R_asm->base, max * max + EDGE, DELTA);

}

