//Dilate5x5 test
//Asm function prototype:
//     void Dilate5x5_asm(u8** src, u8** dst, u8** kernel, u32 width, u32 height);
//
//Asm test function prototype:
//     void Dilate5x5_asm_test(u8** src, u8** dst, u8** kernel, u32 width, u32 height);
//
//C function prototype:
//     void Dilate5x5(u8** src, u8** dst, u8** kernel, u16 width, u16 height);
//
//Parameter description:
//  src      - array of pointers to input lines of the input image
//  dst      - array of pointers to output lines
//  kernel   - array of pointers to input kernel
//  width    - width  of the input line
//  height   - height of the input line

#include "gtest/gtest.h"
#include "dilate5x5.h"
#include "dilate5x5_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16


class Dilate5x5Test : public ::testing::Test {
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

TEST_F(Dilate5x5Test, TestAllValues0)
{
    width = 64;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	kernel = inputGen.GetLines(5, 5, 1);
	
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
    
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);	
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);

    outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}

TEST_F(Dilate5x5Test, TestAllValues255)
{
    width = 64;
	height = 5;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 255);
	kernel = inputGen.GetLines(5, 5, 1);
	
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
     
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLines, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);

    outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 255);
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);
}

TEST_F(Dilate5x5Test, TestLowestLineSize)
{
    width = 8;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 0);
	kernel = inputGen.GetLines(5, 5, 1);
	
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);

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
     
    inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);

    EXPECT_EQ(30, outLinesC[0][0]);
    EXPECT_EQ(30, outLinesC[0][1]);
    EXPECT_EQ(30, outLinesC[0][2]);
    EXPECT_EQ(30, outLinesC[0][3]);
    EXPECT_EQ(30, outLinesC[0][4]);
    EXPECT_EQ(30, outLinesC[0][5]);
    EXPECT_EQ(30, outLinesC[0][6]);
    EXPECT_EQ(30, outLinesC[0][7]);

    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}

TEST_F(Dilate5x5Test, TestAllKernelValues0)
{
    width = 32;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 1);
	kernel = inputGen.GetLines(5, 5, 0);
	
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
     
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
    
    inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    outputCheck.ExpectAllEQ<unsigned char>(outLinesAsm[0], width, 0);
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}

TEST_F(Dilate5x5Test, TestDiagonalKernel)
{
    width = 64;
	height = 5;
	unsigned char **inLinesOffsetted;
	
	inputGen.SelectGenerator("uniform");
	inLines = inputGen.GetLines(width + PADDING, height, 2);
	kernel = inputGen.GetLines(5, 5, 0);
	
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
   
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
    
    kernel[0][4] = 1;
    kernel[1][3] = 1;
    kernel[2][2] = 1;
    kernel[3][1] = 1;
    kernel[4][0] = 1;
  
    inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
	Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    EXPECT_EQ(20, outLinesAsm[0][0]);
	EXPECT_EQ(9,  outLinesAsm[0][1]);
	EXPECT_EQ(30, outLinesAsm[0][2]);	
	EXPECT_EQ(20, outLinesAsm[0][3]);
	EXPECT_EQ(15, outLinesAsm[0][4]);
	EXPECT_EQ(9,  outLinesAsm[0][5]);
    EXPECT_EQ(25, outLinesAsm[0][6]);
    EXPECT_EQ(19, outLinesAsm[0][7]);
        
    
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);
}

TEST_F(Dilate5x5Test, TestWidth24RandomData)
{
    width = 24;
	height = 5;
	unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);
}

TEST_F(Dilate5x5Test, TestWidth120RandomData)
{
    width = 120;
	height = 5;
	unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
  
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}

TEST_F(Dilate5x5Test, TestWidth640RandomData)
{
    width = 640;
	height = 5;
	unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}

TEST_F(Dilate5x5Test, TestWidth1280RandomData)
{
    width = 1280;
	height = 5;
	unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}

TEST_F(Dilate5x5Test, TestWidth1920RandomData)
{
    width = 1920;
	height = 5;
	unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
   
    inputGen.SelectGenerator("uniform");
	kernel = inputGen.GetLines(5, 5, 1);
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}

TEST_F(Dilate5x5Test, TestRandomDataRandomKernel)
{
    width = 240;
	height = 5;
	unsigned char **inLinesOffsetted;
	
    inputGen.SelectGenerator("random");
	inLines = inputGen.GetLines(width + PADDING, height, 0, 255);
    kernel = inputGen.GetLines(5, 5, 0, 2);

    inputGen.SelectGenerator("uniform");
	outLinesC = inputGen.GetLines(width + PADDING, 1, 5);
	outLinesAsm = inputGen.GetLines(width + PADDING, 1, 5);
	
	inLinesOffsetted = inputGen.GetOffsettedLines(inLines, height, 8);
	
    Dilate5x5_asm_test(inLines, outLinesAsm, kernel, width);
    mvcvDilate5x5(inLinesOffsetted, outLinesC, kernel, width);
	RecordProperty("CyclePerPixel", Dilate5x5CycleCount / width);
    
    outputCheck.CompareArrays(outLinesC[0], outLinesAsm[0], width + PADDING);

}