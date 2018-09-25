//cvtColorKernelRGBfp16ToUV420U8 kernel test

//Asm function prototype:
//	void  cvtColorRGBfp16ToUV420U8(half* inRGB[3], u8* uOut, u8* vOut, u32 width)

//Asm test function prototype:
//    	void cvtColorRGBfp16ToUV420U8_asm_test(half **inputRGB, unsigned char **Uout, unsigned char **Vout,unsigned int width)

//C function prototype:
//  	void  cvtColorRGBfp16ToUV420U8(half* inRGB[3], u8* uOut, u8* vOut, u32 width)


#include "gtest/gtest.h"
#include "cvtColorRGBfp16ToUV420U8.h"
///
#include "cvtColorRGBfp16ToUV420U8_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>



const unsigned short DELTA = 1;	

using ::testing::TestWithParam;
using ::testing::Values;


class cvtColorRGBfp16ToUV420U8Test : public ::testing::TestWithParam< unsigned int > {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	half** inLine;
	unsigned char* outLineC_U;
	unsigned char* outLineC_V;
	unsigned char* outLineAsm_U;
	unsigned char* outLineAsm_V;


	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	ArrayChecker outputChecker;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};


TEST_F(cvtColorRGBfp16ToUV420U8Test, TestUniformInputWidth1920)
{
	width = 1920;
		
	inputGen.SelectGenerator("random");
	inLine = inputGen.GetLinesFloat16(width, 3, 0.0f, 1.0f);
	outLineC_U = inputGen.GetEmptyLine(width/2);
	outLineC_V = inputGen.GetEmptyLine(width/2);
	outLineAsm_U = inputGen.GetEmptyLine(width/2);
	outLineAsm_V = inputGen.GetEmptyLine(width/2);

	mvcvCvtColorRGBfp16ToUV420U8(inLine, outLineC_U, outLineC_V, width);	
	cvtColorRGBfp16ToUV420U8_asm_test(inLine, outLineAsm_U, outLineAsm_V, width);	
	
	outputCheck.CompareArrays(outLineC_U, outLineAsm_U, width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V, outLineAsm_V, width/2, DELTA);

	//outputCheck.DumpArrays("inline1", "inLine1.c", inLine[0], width);	
	//outputCheck.DumpArrays("inline2", "inLine2.c", inLine[1], width);	
	//outputCheck.DumpArrays("inline3", "inLine3.c", inLine[2], width);	
	
	//outputCheck.DumpArrays("OutU_C", "outU_C.c", outLineC_U, width/2);
	//outputCheck.DumpArrays("OutU_Asm", "outU_Asm.c", outLineAsm_U, width/2);
	//outputCheck.DumpArrays("OutV_C", "outV_C.c", outLineC_V, width/2);
	//outputCheck.DumpArrays("OutV_Asm", "outV_Asm.c", outLineAsm_V, width/2);
}

 INSTANTIATE_TEST_CASE_P(TestRandomData, cvtColorRGBfp16ToUV420U8Test,
		Values((unsigned int)8, (unsigned int)16, (unsigned int)32, (unsigned int)48, (unsigned int)160, (unsigned int)320, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1920);
);

TEST_P(cvtColorRGBfp16ToUV420U8Test, TestRandomData)
{

	inputGen.SelectGenerator("random");
	unsigned int width = GetParam();
	inLine = inputGen.GetLinesFloat16(width, 3, 0.0f, 1.0f);
	outLineC_U = inputGen.GetEmptyLine(width/2);
	outLineC_V = inputGen.GetEmptyLine(width/2);
	outLineAsm_U = inputGen.GetEmptyLine(width/2);
	outLineAsm_V = inputGen.GetEmptyLine(width/2);


	mvcvCvtColorRGBfp16ToUV420U8(inLine, outLineC_U, outLineC_V, width);	
	cvtColorRGBfp16ToUV420U8_asm_test(inLine, outLineAsm_U, outLineAsm_V, width);		
	
	outputCheck.CompareArrays(outLineC_U, outLineAsm_U, width/2, DELTA);
	outputCheck.CompareArrays(outLineC_V, outLineAsm_V, width/2, DELTA);

}   