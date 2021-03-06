//Erode7x7 kernel test

//Asm function prototype:
//      void mvcvErode7x7(u8** src, u8** dst, u8** kernel, u16 width, u16 height)

//Asm test function prototype:
//      void Erode7x7_asm_test(u8** src, u8** dst, u8** kernel, u16 width, u16 height)

//C function prototype:
//      void mvcvErode7x7(u8** src, u8** dst, u8** kernel, u16 width, u16 height)

// src      - array of pointers to input lines of the input image
// dst      - array of pointers to output lines
// kernel   - array of pointers to input kernel
// width    - width  of the input line
// height   - height of the input line

#include "gtest/gtest.h"
#include <erode7x7.h>
#include "erode7x7_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16

class Erode7x7Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	

    
    
    unsigned char** kernel;
    unsigned char** outLinesC;
	unsigned char** outLinesAsm;
    unsigned char** inLines;
	unsigned int width;
    unsigned int height;
    InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(Erode7x7Test, TestUniformInputAll0)
{
	width = 64;
	height = 7;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	kernel = inputGen.GetLines(7, 7, 1);
	
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
		
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
	
	outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestUniformInputAll255)
{
	width = 64;
	height = 7;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 255);
	kernel = inputGen.GetLines(7, 7, 1);
	
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
	
	outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 255);
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestLowestLineSize)
{
	width = 8;
	height = 7;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 2);
	kernel = inputGen.GetLines(7, 7, 1);
    
	inLines[0][0] = 2;  inLines[0][1] = 2; inLines[0][2] = 2; inLines[0][3] = 2;
	inLines[0][4] = 2;  inLines[0][5] = 2; inLines[0][6] = 2; inLines[0][7] = 2;
	inLines[0][8] = 7;  inLines[0][9] = 13; inLines[0][10] = 20; inLines[0][11] = 9; 
    inLines[0][12] = 3;  inLines[0][13] = 8;  inLines[0][14] = 12; inLines[0][15] = 3; 
	
	inLines[1][0] = 2;  inLines[1][1] = 2; inLines[1][2] = 2; inLines[1][3] = 2;
	inLines[1][4] = 2;  inLines[1][5] = 2; inLines[1][6] = 2; inLines[1][7] = 2;
	inLines[1][8] = 1;  inLines[1][9] = 10; inLines[1][10] = 6;  inLines[1][11] = 11; 
    inLines[1][12] = 15; inLines[1][13] = 10; inLines[1][14] = 9;  inLines[1][15] = 9; 
	
	inLines[2][0] = 2;  inLines[2][1] = 2; inLines[2][2] = 2; inLines[2][3] = 2;
	inLines[2][4] = 2;  inLines[2][5] = 2; inLines[2][6] = 2; inLines[2][7] = 2;
	inLines[2][8] = 9;  inLines[2][9] = 8;  inLines[2][10] = 30; inLines[2][11] = 4; 
    inLines[2][12] = 7;  inLines[2][13] = 4;  inLines[2][14] = 1;  inLines[2][15] = 6; 
    
	inLines[3][0] = 2;  inLines[3][1] = 2; inLines[3][2] = 2; inLines[3][3] = 2;
	inLines[3][4] = 2;  inLines[3][5] = 2; inLines[3][6] = 2; inLines[3][7] = 2;
	inLines[3][8] = 8;  inLines[3][9] = 12; inLines[3][10] = 7;  inLines[3][11] = 8; 
    inLines[3][12] = 3;  inLines[3][13] = 25; inLines[3][14] = 19; inLines[3][15] = 4; 
    
	inLines[4][0] = 2;  inLines[4][1] = 2; inLines[4][2] = 2; inLines[4][3] = 2;
	inLines[4][4] = 2;  inLines[4][5] = 2; inLines[4][6] = 2; inLines[4][7] = 2;
	inLines[4][8] = 5;  inLines[4][9] = 20; inLines[4][10] = 15; inLines[4][11] = 6; 
    inLines[4][12] = 8;  inLines[4][13] = 12; inLines[4][14] = 30; inLines[4][15] = 2; 
    
	inLines[5][0] = 2;  inLines[5][1] = 2; inLines[5][2] = 2; inLines[5][3] = 2;
	inLines[5][4] = 2;  inLines[5][5] = 2; inLines[5][6] = 2; inLines[5][7] = 2;
	inLines[5][8] = 10; inLines[5][9] = 1;  inLines[5][10] = 5;  inLines[5][11] = 6; 
    inLines[5][12] = 3;  inLines[5][13] = 20; inLines[5][14] = 11; inLines[5][15] = 7; 
    
	inLines[6][0] = 2;  inLines[6][1] = 2; inLines[6][2] = 2; inLines[6][3] = 2;
	inLines[6][4] = 2;  inLines[6][5] = 2; inLines[6][6] = 2; inLines[6][7] = 2;
	inLines[6][8] = 0;  inLines[6][9] = 6;  inLines[6][10] = 14; inLines[6][11] = 35; 
    inLines[6][12] = 9;  inLines[6][13] = 24; inLines[6][14] = 2;  inLines[6][15] = 18; 
    
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);

    EXPECT_EQ(0, outLinesAsm[0][0]);
	EXPECT_EQ(0, outLinesAsm[0][1]);
	EXPECT_EQ(0, outLinesAsm[0][2]);
	EXPECT_EQ(0, outLinesAsm[0][3]);
    EXPECT_EQ(1, outLinesAsm[0][4]);
    EXPECT_EQ(1, outLinesAsm[0][5]);
    EXPECT_EQ(1, outLinesAsm[0][6]);
    EXPECT_EQ(1, outLinesAsm[0][7]);
 
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestAllKernelValues0)
{
	width = 8;
	height = 7;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	kernel = inputGen.GetLines(7, 7, 0);

    inLines[0][0] = 7;  inLines[0][1] = 13; inLines[0][2] = 20; inLines[0][3] = 9; 
    inLines[0][4] = 3;  inLines[0][5] = 8;  inLines[0][6] = 12; inLines[0][7] = 3; 
	inLines[1][0] = 1;  inLines[1][1] = 10; inLines[1][2] = 6;  inLines[1][3] = 11; 
    inLines[1][4] = 15; inLines[1][5] = 10; inLines[1][6] = 9;  inLines[1][7] = 9; 
	inLines[2][0] = 9;  inLines[2][1] = 8;  inLines[2][2] = 30; inLines[2][3] = 4; 
    inLines[2][4] = 7;  inLines[2][5] = 4;  inLines[2][6] = 1;  inLines[2][7] = 6; 
    inLines[3][0] = 8;  inLines[3][1] = 12; inLines[3][2] = 7;  inLines[3][3] = 8; 
    inLines[3][4] = 3;  inLines[3][5] = 25; inLines[3][6] = 19; inLines[3][7] = 4; 
    inLines[4][0] = 5;  inLines[4][1] = 20; inLines[4][2] = 15; inLines[4][3] = 6; 
    inLines[4][4] = 8;  inLines[4][5] = 12; inLines[4][6] = 30; inLines[4][7] = 0; 
    inLines[5][0] = 10; inLines[5][1] = 1;  inLines[5][2] = 5;  inLines[5][3] = 6; 
    inLines[5][4] = 3;  inLines[5][5] = 20; inLines[5][6] = 11; inLines[5][7] = 7; 
    inLines[6][0] = 0;  inLines[6][1] = 6;  inLines[6][2] = 14; inLines[6][3] = 35; 
    inLines[6][4] = 9;  inLines[6][5] = 24; inLines[6][6] = 2;  inLines[6][7] = 18; 
    
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
    
 	outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 255);
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestDiagonalKernel)
{
	width = 16;
	height = 7;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 2);
	kernel = inputGen.GetLines(7, 7, 0);
	
    kernel[0][6] = 1;
    kernel[1][5] = 1;
    kernel[2][4] = 1;
    kernel[3][3] = 1;
    kernel[4][2] = 1;
    kernel[5][1] = 1;
    kernel[6][0] = 1;
    
	inLines[0][0] = 2;  inLines[0][1] = 2; inLines[0][2] = 2; inLines[0][3] = 2;
	inLines[0][4] = 2;  inLines[0][5] = 2; inLines[0][6] = 2; inLines[0][7] = 2;
	inLines[0][8] = 7;  inLines[0][9] = 13; inLines[0][10] = 20; inLines[0][11] = 9; 
    inLines[0][12] = 3;  inLines[0][13] = 8;  inLines[0][14] = 12; inLines[0][15] = 3; 
	
	inLines[1][0] = 2;  inLines[1][1] = 2; inLines[1][2] = 2; inLines[1][3] = 2;
	inLines[1][4] = 2;  inLines[1][5] = 2; inLines[1][6] = 2; inLines[1][7] = 2;
	inLines[1][8] = 1;  inLines[1][9] = 10; inLines[1][10] = 6;  inLines[1][11] = 11; 
    inLines[1][12] = 15; inLines[1][13] = 10; inLines[1][14] = 9;  inLines[1][15] = 9; 
	
	inLines[2][0] = 2;  inLines[2][1] = 2; inLines[2][2] = 2; inLines[2][3] = 2;
	inLines[2][4] = 2;  inLines[2][5] = 2; inLines[2][6] = 2; inLines[2][7] = 2;
	inLines[2][8] = 9;  inLines[2][9] = 8;  inLines[2][10] = 30; inLines[2][11] = 4; 
    inLines[2][12] = 7;  inLines[2][13] = 4;  inLines[2][14] = 1;  inLines[2][15] = 6; 
    
	inLines[3][0] = 2;  inLines[3][1] = 2; inLines[3][2] = 2; inLines[3][3] = 2;
	inLines[3][4] = 2;  inLines[3][5] = 2; inLines[3][6] = 2; inLines[3][7] = 2;
	inLines[3][8] = 8;  inLines[3][9] = 12; inLines[3][10] = 7;  inLines[3][11] = 8; 
    inLines[3][12] = 3;  inLines[3][13] = 25; inLines[3][14] = 19; inLines[3][15] = 4; 
    
	inLines[4][0] = 2;  inLines[4][1] = 2; inLines[4][2] = 2; inLines[4][3] = 2;
	inLines[4][4] = 2;  inLines[4][5] = 2; inLines[4][6] = 2; inLines[4][7] = 2;
	inLines[4][8] = 5;  inLines[4][9] = 20; inLines[4][10] = 15; inLines[4][11] = 6; 
    inLines[4][12] = 8;  inLines[4][13] = 12; inLines[4][14] = 30; inLines[4][15] = 2; 
    
	inLines[5][0] = 2;  inLines[5][1] = 2; inLines[5][2] = 2; inLines[5][3] = 2;
	inLines[5][4] = 2;  inLines[5][5] = 2; inLines[5][6] = 2; inLines[5][7] = 2;
	inLines[5][8] = 10; inLines[5][9] = 1;  inLines[5][10] = 5;  inLines[5][11] = 6; 
    inLines[5][12] = 3;  inLines[5][13] = 20; inLines[5][14] = 11; inLines[5][15] = 7; 
    
	inLines[6][0] = 2;  inLines[6][1] = 2; inLines[6][2] = 2; inLines[6][3] = 2;
	inLines[6][4] = 2;  inLines[6][5] = 2; inLines[6][6] = 2; inLines[6][7] = 2;
	inLines[6][8] = 0;  inLines[6][9] = 6;  inLines[6][10] = 14; inLines[6][11] = 35; 
    inLines[6][12] = 9;  inLines[6][13] = 24; inLines[6][14] = 2;  inLines[6][15] = 18; 
    
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);

    EXPECT_EQ(2, outLinesAsm[0][0]);
	EXPECT_EQ(2, outLinesAsm[0][1]);
	EXPECT_EQ(2, outLinesAsm[0][2]);
	EXPECT_EQ(0, outLinesAsm[0][3]);
    EXPECT_EQ(3, outLinesAsm[0][4]);
    EXPECT_EQ(1, outLinesAsm[0][5]);
    EXPECT_EQ(2, outLinesAsm[0][6]);
    EXPECT_EQ(2, outLinesAsm[0][7]);
 
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestWidth24RandomData)
{
	width = 24;
	height = 7;
    unsigned char **inLinesOffsetted;
   
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(7, 7, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);

}

TEST_F(Erode7x7Test, TestWidth120RandomData)
{
	width = 120;
	height = 7;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(7, 7, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
	
	/*outputCheck.DumpArrays("refInput", "refInput.c", inLines, width + PADDING, height);
	outputCheck.DumpArrays("refAsmOutput", "refAsmOutput.c", outLinesAsm, width , 1);
	outputCheck.DumpArrays("refCOutput", "refCOutput.c", outLinesC, width , 1);*/
}

TEST_F(Erode7x7Test, TestWidth640RandomData)
{
	width = 640;
	height = 7;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(7, 7, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestWidth1280RandomData)
{
	width = 1280;
	height = 7;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(7, 7, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestWidth1920RandomData)
{
	width = 1920;
	height = 7;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(7, 7, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode7x7Test, TestRandomDataRandomKernel)
{
	width = 240;
	height = 7;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   	kernel = inputGen.GetLines(7, 7, 0, 2);

    inputGen.SelectGenerator("uniform");
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode7x7(inLinesOffsetted, outLinesC, kernel, width);
	Erode7x7_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode7x7CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}