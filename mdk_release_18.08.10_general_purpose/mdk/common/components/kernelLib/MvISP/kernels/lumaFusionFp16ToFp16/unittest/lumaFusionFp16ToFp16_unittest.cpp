
#include "gtest/gtest.h"
#include "lumaFusionFp16ToFp16_asm_test.h"
#include "lumaFusionFp16ToFp16.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include "half.h"

#define PADDING 32

class lumaFusionFp16ToFp16Test : public ::testing::Test {
 protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}

	
	half *inLumaShortAsm;
	half *inLumaShortC;
	half *inLumaLong;
	float *params;
	
	half *outputAlphaC;
	half *outputAlphaAsm;
	half *outputLumaFusionC;
	half *outputLumaFusionAsm;
	unsigned int width;
    int nPoints; 
    
	RandomGenerator dataGenerator;
	InputGenerator inputGen;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;
	ArrayChecker outputChecker;

	virtual void TearDown() {}
};

half DELTA = 0.125;//accepted tolerance between C and ASM results

TEST_F(lumaFusionFp16ToFp16Test, TestRandomInput)
{
	inputGen.SelectGenerator("random");
    width = 16;
	 params   = inputGen.GetLineFloat(4, 0, 1);
	 params[0] = randGen->GenerateFloat(1,100);
	 params[0] = 1;
	 params[1] = randGen->GenerateFloat(0,1);
	 params[2] = randGen->GenerateFloat(0,1);
	 params[3] = randGen->GenerateFloat(0,1);
	inLumaShortAsm = inputGen.GetLineFloat16(width + PADDING, params[2], params[3]);
	inLumaShortC = inputGen.GetLineFloat16(width + PADDING, 0, 1);
	inLumaLong = inputGen.GetLineFloat16(width + PADDING, 0, 1);
	
	if (params[3] == params[4])
        params[4]+=0.01;		
	
	memcpy(inLumaShortC, inLumaShortAsm, (width + PADDING) * sizeof(half));
	outputAlphaC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputAlphaAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);
    //uncoment if dump is needed
	//outputChecker.DumpArrays("inLumaShortAsm", "inLumaShortAsm.c", inLumaShortAsm, width + PADDING);
	//outputChecker.DumpArrays("inLumaLong", "inLumaLong.c", inLumaLong, width + PADDING);
	//outputChecker.DumpArrays("params", "params.c", params, 4);

	
	mvispLumaFusionFp16ToFp16(inLumaShortC, inLumaLong, outputAlphaC, outputLumaFusionC, width, (LumaFusionParam *)params);
	lumaFusionFp16ToFp16_asm_test(inLumaShortAsm, inLumaLong, outputAlphaAsm, outputLumaFusionAsm, width, (LumaFusionParamT *)params);
	
	outputChecker.CompareArrays(outputAlphaC, outputAlphaAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(outputLumaFusionC, outputLumaFusionAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(inLumaShortC, inLumaShortAsm, width + PADDING,DELTA);
	
	//outputChecker.DumpArrays("outputAlphaC", "outputAlphaC.c", outputAlphaC, width + PADDING);
}

TEST_F(lumaFusionFp16ToFp16Test, TestRandomInputMinWidth)
{
	inputGen.SelectGenerator("random");
    width = 8;
     
	inLumaShortAsm = inputGen.GetLineFloat16(width + PADDING, 1, 255);
	inLumaShortC = inputGen.GetLineFloat16(width + PADDING, 1, 255);
	inLumaLong = inputGen.GetLineFloat16(width + PADDING, 1, 255);
	params   = inputGen.GetLineFloat(4, 1, 100);
	
	if (params[3] == params[4])
        params[4]++;		
	
	memcpy(inLumaShortC, inLumaShortAsm, (width + PADDING) * sizeof(half));
	outputAlphaC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputAlphaAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);

	mvispLumaFusionFp16ToFp16(inLumaShortC, inLumaLong, outputAlphaC, outputLumaFusionC, width, (LumaFusionParam *)params);
	lumaFusionFp16ToFp16_asm_test(inLumaShortAsm, inLumaLong, outputAlphaAsm, outputLumaFusionAsm, width, (LumaFusionParamT *)params);
	
	outputChecker.CompareArrays(outputAlphaC, outputAlphaAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(outputLumaFusionC, outputLumaFusionAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(inLumaShortC, inLumaShortAsm, width + PADDING,DELTA);
}

TEST_F(lumaFusionFp16ToFp16Test, TestRandomInputMaxWidth)
{
	inputGen.SelectGenerator("random");
    width = 1920;
     
	inLumaShortAsm = inputGen.GetLineFloat16(width + PADDING, 1, 255);
	inLumaShortC = inputGen.GetLineFloat16(width + PADDING, 1, 255);
	inLumaLong = inputGen.GetLineFloat16(width + PADDING, 1, 255);
	params   = inputGen.GetLineFloat(4, 1, 100);
	
	if (params[3] == params[4])
        params[4]++;		
	
	memcpy(inLumaShortC, inLumaShortAsm, (width + PADDING) * sizeof(half));
	outputAlphaC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputAlphaAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);

	mvispLumaFusionFp16ToFp16(inLumaShortC, inLumaLong, outputAlphaC, outputLumaFusionC, width, (LumaFusionParam *)params);
	lumaFusionFp16ToFp16_asm_test(inLumaShortAsm, inLumaLong, outputAlphaAsm, outputLumaFusionAsm, width, (LumaFusionParamT *)params);
	
	outputChecker.CompareArrays(outputAlphaC, outputAlphaAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(outputLumaFusionC, outputLumaFusionAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(inLumaShortC, inLumaShortAsm, width + PADDING,DELTA);
}

TEST_F(lumaFusionFp16ToFp16Test, TestRandomInputRandomData)
{
	inputGen.SelectGenerator("random");
    width = randGen->GenerateUInt(8, 1920, 8);
     
	inLumaShortAsm = inputGen.GetLineFloat16(width + PADDING, 0, 255);
	inLumaShortC = inputGen.GetLineFloat16(width + PADDING, 0, 255);
	inLumaLong = inputGen.GetLineFloat16(width + PADDING, 0, 255);
	params   = inputGen.GetLineFloat(4, 1, 100);
	
	if (params[3] == params[4])
        params[4]++;		
	
	memcpy(inLumaShortC, inLumaShortAsm, (width + PADDING) * sizeof(half));
	outputAlphaC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputAlphaAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionC = inputGen.GetLineFloat16(width + PADDING, 0, 0);
	outputLumaFusionAsm = inputGen.GetLineFloat16(width + PADDING, 0, 0);

	mvispLumaFusionFp16ToFp16(inLumaShortC, inLumaLong, outputAlphaC, outputLumaFusionC, width, (LumaFusionParam *)params);
	lumaFusionFp16ToFp16_asm_test(inLumaShortAsm, inLumaLong, outputAlphaAsm, outputLumaFusionAsm, width, (LumaFusionParamT *)params);
	
	outputChecker.CompareArrays(outputAlphaC, outputAlphaAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(outputLumaFusionC, outputLumaFusionAsm, width + PADDING,DELTA);
	outputChecker.CompareArrays(inLumaShortC, inLumaShortAsm, width + PADDING,DELTA);
}
