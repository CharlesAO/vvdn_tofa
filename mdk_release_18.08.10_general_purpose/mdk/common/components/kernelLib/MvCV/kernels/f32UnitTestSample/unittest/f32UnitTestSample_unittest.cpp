#include "gtest/gtest.h"
#include "f32UnitTestSample.h"
#include "f32UnitTestSample_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <stdlib.h>

class f32UnitTestSampleTest : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	

    
    
    float* a;
    float** b;
	float* outCReference;
	float* outCImplementation;
    unsigned int num_vectors;
    
    InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(f32UnitTestSampleTest, DefaultDeltaIs4ULPs)
{
	num_vectors = 4;
	inputGen.SelectGenerator("random");
	a = inputGen.GetLineFloat(4, 1, 10);
	b = inputGen.GetLinesFloat(4, 4, 1, 10);
	
	outCReference = inputGen.GetLineFloat(4, 1, 1);
	outCImplementation = inputGen.GetLineFloat(4, 1, 1);
	
	dotProductStreaming(a, b, outCImplementation, num_vectors);
	f32UnitTestSample_asm_test(a, b, outCReference, num_vectors);
	
	outputCheck.CompareArraysFloat(outCReference, outCImplementation, 4, 0.001);
}

TEST_F(f32UnitTestSampleTest, DeltaIsOneThousandth)
{
	num_vectors = 4;
	inputGen.SelectGenerator("random");
	a = inputGen.GetLineFloat(4, 1, 10);
	b = inputGen.GetLinesFloat(4, 4, 1, 10);
	
	outCReference = inputGen.GetLineFloat(4, 1, 1);
	outCImplementation = inputGen.GetLineFloat(4, 1, 1);
	
	dotProductStreaming(a, b, outCImplementation, num_vectors);
	f32UnitTestSample_asm_test(a, b, outCReference, num_vectors);
	
	outputCheck.CompareArraysFloat(outCReference, outCImplementation, 4, 0.001);
}

