//Erode5x5 kernel test

//Asm function prototype:
//      void mvcvErode5x5(u8** src, u8** dst, u8** kernel, u16 width, u16 height, u8 k)

//Asm test function prototype:
//      void Erode5x5_asm_test(u8** src, u8** dst, u8** kernel, u16 width, u16 height, u8 k)

//C function prototype:
//      void mvcvErode5x5(u8** src, u8** dst, u8** kernel, u16 width, u16 height)

// src      - array of pointers to input lines of the input image
// dst      - array of pointers to output lines
// kernel   - array of pointers to input kernel
// width    - width  of the input line
// height   - height of the input line

#include "gtest/gtest.h"
#include <erode5x5.h>
#include "erode5x5_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16

class Erode5x5Test : public ::testing::Test {
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
    unsigned int height;
    unsigned int width;

	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(Erode5x5Test, TestUniformInputAll0)
{
	width = 64;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	kernel = inputGen.GetLines(5, 5, 1);
	
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
	
	outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestUniformInputAll255)
{
	width = 64;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 255);
	kernel = inputGen.GetLines(5, 5, 1);
	
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
	
	outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 255);
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}


TEST_F(Erode5x5Test, TestNonUniformInput)
{
	width = 64;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 7);
	kernel = inputGen.GetLines(5, 5, 1);

	// Line 0
	inLines[0][0] = 0xFF;  inLines[0][1] = 0xFF; inLines[0][2] = 0xFF; inLines[0][3] = 0xFF;
	inLines[0][4] = 0xFF;  inLines[0][5] = 0xFF; inLines[0][6] = 0xFF; inLines[0][7] = 0xFF;
	inLines[0][8] = 7;     inLines[0][9] = 13;   inLines[0][10] = 4;   inLines[0][11] = 4;  inLines[0][12] = 8;  inLines[0][13] = 16;
	
	// Line 1
	inLines[1][0] = 0xFF;  inLines[1][1] = 0xFF; inLines[1][2] = 0xFF; inLines[1][3] = 0xFF;
	inLines[1][4] = 0xFF;  inLines[1][5] = 0xFF; inLines[1][6] = 0xFF; inLines[1][7] = 0xFF;
	inLines[1][8] = 5;     inLines[1][9] = 2;    inLines[1][10] = 6;   inLines[1][11] = 3;  inLines[1][12] = 10; inLines[1][13] = 20;
	
	// Line 2
	inLines[2][0] = 0xFF;  inLines[2][1] = 0xFF; inLines[2][2] = 0xFF; inLines[2][3] = 0xFF;
	inLines[2][4] = 0xFF;  inLines[2][5] = 0xFF; inLines[2][6] = 0xFF; inLines[2][7] = 0xFF;
	inLines[2][8] = 9;     inLines[2][9] = 8;    inLines[2][10] = 4;   inLines[2][11] = 7;  inLines[2][12] = 8;  inLines[2][13] = 0;
	
	// Line 3
	inLines[3][0] = 0xFF;  inLines[3][1] = 0xFF; inLines[3][2] = 0xFF; inLines[3][3] = 0xFF;
	inLines[3][4] = 0xFF;  inLines[3][5] = 0xFF; inLines[3][6] = 0xFF; inLines[3][7] = 0xFF;
	inLines[3][8] = 2;     inLines[3][9] = 10;   inLines[3][10] = 1;   inLines[3][11] = 11; inLines[3][12] = 16;  inLines[3][13] = 11;
	
	// Line 4
	inLines[4][0] = 0xFF;  inLines[4][1] = 0xFF; inLines[4][2] = 0xFF; inLines[4][3] = 0xFF;
	inLines[4][4] = 0xFF;  inLines[4][5] = 0xFF; inLines[4][6] = 0xFF; inLines[4][7] = 0xFF;
	inLines[4][8] = 6;     inLines[4][9] = 20;   inLines[4][10] = 9;   inLines[4][11] = 30; inLines[4][12] = 18;  inLines[4][13] = 9;

	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
	EXPECT_EQ(1, outLinesAsm[0][0]);
	EXPECT_EQ(1, outLinesAsm[0][1]);
	EXPECT_EQ(1, outLinesAsm[0][2]);
	EXPECT_EQ(0, outLinesAsm[0][3]);
    EXPECT_EQ(0, outLinesAsm[0][4]);
    EXPECT_EQ(0, outLinesAsm[0][5]);
    EXPECT_EQ(0, outLinesAsm[0][6]);
    EXPECT_EQ(0, outLinesAsm[0][7]);
    EXPECT_EQ(7, outLinesAsm[0][8]);
    EXPECT_EQ(7, outLinesAsm[0][9]);
    EXPECT_EQ(7, outLinesAsm[0][10]);

	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestAllKernelValues1)
{
	width = 8;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 7);
	kernel = inputGen.GetLines(5, 5, 1);
	
	inLines[0][8] = 7;   inLines[0][9] = 13;  inLines[0][10] = 20; inLines[0][11] = 9; 
    inLines[0][12] = 3;  inLines[0][13] = 8;  inLines[0][14] = 12; inLines[0][15] = 3; 
	inLines[1][8] = 1;   inLines[1][9] = 10;  inLines[1][10] = 6;  inLines[1][11] = 11; 
    inLines[1][12] = 15; inLines[1][13] = 10; inLines[1][14] = 9;  inLines[1][15] = 9; 
	inLines[2][8] = 9;   inLines[2][9] = 8;   inLines[2][10] = 30; inLines[2][11] = 4; 
    inLines[2][12] = 7;  inLines[2][13] = 4;  inLines[2][14] = 1;  inLines[2][15] = 6; 
    inLines[3][8] = 8;   inLines[3][9] = 12;  inLines[3][10] = 7;  inLines[3][11] = 8; 
    inLines[3][12] = 3;  inLines[3][13] = 25; inLines[3][14] = 19; inLines[3][15] = 4; 
    inLines[4][8] = 5;   inLines[4][9] = 20;  inLines[4][10] = 15; inLines[4][11] = 6; 
    inLines[4][12] = 8;  inLines[4][13] = 12; inLines[4][14] = 30; inLines[4][15] = 0;  

	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
        
	EXPECT_EQ(1, outLinesAsm[0][0]);
	EXPECT_EQ(1, outLinesAsm[0][1]);
	EXPECT_EQ(1, outLinesAsm[0][2]);
	EXPECT_EQ(3, outLinesAsm[0][3]);
	EXPECT_EQ(1, outLinesAsm[0][4]);
    EXPECT_EQ(0, outLinesAsm[0][5]);
    EXPECT_EQ(0, outLinesAsm[0][6]);
    EXPECT_EQ(0, outLinesAsm[0][7]);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestAllKernelValues0)
{
	width = 8;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	kernel = inputGen.GetLines(5, 5, 0);

    inLines[0][0] = 7;  inLines[0][1] = 13; inLines[0][2] = 20; inLines[0][3] = 9; 
    inLines[0][4] = 2;  inLines[0][5] = 8;  inLines[0][6] = 12; inLines[0][7] = 3; 
	inLines[1][0] = 1;  inLines[1][1] = 10; inLines[1][2] = 6;  inLines[1][3] = 11;
    inLines[1][4] = 15; inLines[1][5] = 10; inLines[1][6] = 9;  inLines[1][7] = 9; 
	inLines[2][0] = 9;  inLines[2][1] = 8;  inLines[2][2] = 3;  inLines[2][3] = 4; 
    inLines[2][4] = 7;  inLines[2][5] = 4;  inLines[2][6] = 1;  inLines[2][7] = 0; 
    inLines[3][0] = 8;  inLines[3][1] = 12;  inLines[3][2] = 7;   inLines[3][3] = 8; 
    inLines[3][4] = 3;  inLines[3][5] = 25;  inLines[3][6] = 19;  inLines[3][7] = 4; 
    inLines[4][0] = 5;  inLines[4][1] = 20;  inLines[4][2] = 15;  inLines[4][3] = 6; 
    inLines[4][4] = 8;  inLines[4][5] = 12;  inLines[4][6] = 30;  inLines[4][7] = 0; 
    
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);

	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);	
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
 	outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 255);
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestDiagonalKernel)
{
	width = 8;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 2);
	kernel = inputGen.GetLines(5, 5, 0);
	
    kernel[0][4] = 1;
    kernel[1][3] = 1;
    kernel[2][2] = 1;
    kernel[3][1] = 1;
    kernel[4][0] = 1;
    
	inLines[0][8] = 7;   inLines[0][9] = 13;  inLines[0][10] = 20; inLines[0][11] = 9; 
    inLines[0][12] = 3;  inLines[0][13] = 8;  inLines[0][14] = 12; inLines[0][15] = 3; 
	inLines[1][8] = 1;   inLines[1][9] = 10;  inLines[1][10] = 6;  inLines[1][11] = 11; 
    inLines[1][12] = 15; inLines[1][13] = 10; inLines[1][14] = 9;  inLines[1][15] = 9; 
	inLines[2][8] = 9;   inLines[2][9] = 8;   inLines[2][10] = 30; inLines[2][11] = 4; 
    inLines[2][12] = 7;  inLines[2][13] = 4;  inLines[2][14] = 1;  inLines[2][15] = 6; 
    inLines[3][8] = 8;   inLines[3][9] = 12;  inLines[3][10] = 7;  inLines[3][11] = 8; 
    inLines[3][12] = 3;  inLines[3][13] = 25; inLines[3][14] = 19; inLines[3][15] = 4; 
    inLines[4][8] = 5;   inLines[4][9] = 20;  inLines[4][10] = 15; inLines[4][11] = 6; 
    inLines[4][12] = 8;  inLines[4][13] = 12; inLines[4][14] = 30; inLines[4][15] = 0; 
    
	outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
		
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
    EXPECT_EQ(2, outLinesAsm[0][0]);
	EXPECT_EQ(2, outLinesAsm[0][1]);
	EXPECT_EQ(3, outLinesAsm[0][2]);
	EXPECT_EQ(4, outLinesAsm[0][3]);
	EXPECT_EQ(7, outLinesAsm[0][4]);
    EXPECT_EQ(3, outLinesAsm[0][5]);
    EXPECT_EQ(1, outLinesAsm[0][6]);
    EXPECT_EQ(2, outLinesAsm[0][7]);
     
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestWidth24RandomData)
{
	width = 24;
	height = 5;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
		
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestWidth120RandomData)
{
	width = 120;
	height = 5;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);	
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
        
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestWidth640RandomData)
{
	width = 640;
	height = 5;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);	
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestWidth1280RandomData)
{
	width = 1280;
	height = 5;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestWidth1920RandomData)
{
	width = 1920;
	height = 5;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);	
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}

TEST_F(Erode5x5Test, TestRandomDataRandomKernel)
{
	width = 240;
	height = 5;
    unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   	kernel = inputGen.GetLines(5, 5, 0, 2);

    inputGen.SelectGenerator("uniform");
    outLinesC = inputGen.GetEmptyLines(width , 1);
	outLinesAsm = inputGen.GetEmptyLines(width, 1);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	mvcvErode5x5(inLinesOffsetted, outLinesC, kernel, width);
	Erode5x5_asm_test(inLines, outLinesAsm, kernel, width);
	RecordProperty("CyclePerPixel", Erode5x5CycleCount / width);
    
	outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width);
}