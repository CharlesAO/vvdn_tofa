//SAD 11x11 kernel test

//Asm function prototype:
//	void mvcvSumOfAbsDiff11x11_asm(u8** in1, u8** in2, u8** out, u32 width);

//Asm test function prototype:
//	void sumOfAbsDiff11x11_asm_test(unsigned char **input1, unsigned char **input2, unsigned char **output, unsigned int width);

//C function prototype:
//	void mvcvSumOfAbsDiff11x11(u8** in1, u8** in2, u8** out, u32 width);

//width	- width of input line
//in1		- first image
//in2		- the second image
//out		- the output line

#include "gtest/gtest.h"
#include "sumOfAbsDiff11x11.h"
#include "sumOfAbsDiff11x11_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define PADDING 16
#define SIZE 11

class sumOfAbsDiff11x11Test : public ::testing::Test {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	unsigned char** inLine1;
	unsigned char** inLine2;
	unsigned char** outLineC;
	unsigned char** outLineAsm;
	unsigned char** outLineExpected;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(sumOfAbsDiff11x11Test, TestOnePixelDistinct)
{
	width = 720;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 215);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 215);
	outLineExpected = inputGen.GetLines(width, 1, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	
	
	inLine1[2][11] = 15;
	outLineExpected[0][0] = 200;  
	outLineExpected[0][1] = 200;  
	outLineExpected[0][2] = 200;  
	outLineExpected[0][3] = 200;  
	outLineExpected[0][4] = 200;  
	outLineExpected[0][5] = 200;  
	outLineExpected[0][6] = 200;  
	outLineExpected[0][7] = 200;  
	outLineExpected[0][8] = 200;  
	
		
	sumOfAbsDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff11x11(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfAbsDiff11x11Test, TestLines1_minus_Line2_equal_1)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0);
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);	     
	outLineExpected = inputGen.GetLines(width, 1, 121);	  
	outLineExpected[0][0]  = 66;
	outLineExpected[0][1]  = 77;
	outLineExpected[0][2]  = 88;
	outLineExpected[0][3]  = 99;
	outLineExpected[0][4]  = 110;
	outLineExpected[0][11] = 110;
	outLineExpected[0][12] = 99;
	outLineExpected[0][13] = 88;
	outLineExpected[0][14] = 77;
	outLineExpected[0][15] = 66;	
	for(int i = 0; i < SIZE; i++)
		for(int j = 8; j < (int)(width + PADDING / 2); j++)
		{
			inLine1[i][j] = 0;
			inLine2[i][j] = 1;
		}

	sumOfAbsDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);	
	RecordProperty("CyclePerPixel", sumOfAbsDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);	
	mvcvSumOfAbsDiff11x11(inLine1, inLine2, outLineC, width);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
}


TEST_F(sumOfAbsDiff11x11Test, TestExpectedOutput)
{
	width = 16;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0);
	outLineC = inputGen.GetLines(width, 1, 0);
	outLineAsm = inputGen.GetLines(width, 1, 0);	     
	outLineExpected = inputGen.GetEmptyLines(width, 1);	  
	outLineExpected[0][0] = 49;		outLineExpected[0][8]  = 59;
	outLineExpected[0][1] = 53;		outLineExpected[0][9]  = 58;
	outLineExpected[0][2] = 58;     outLineExpected[0][10] = 59;
	outLineExpected[0][3] = 65;     outLineExpected[0][11] = 50;
	outLineExpected[0][4] = 71;     outLineExpected[0][12] = 46;
	outLineExpected[0][5] = 75;     outLineExpected[0][13] = 41;
	outLineExpected[0][6] = 68;     outLineExpected[0][14] = 34;
	outLineExpected[0][7] = 65;     outLineExpected[0][15] = 28;
	   
inLine1[0] [8]  =  2; 	inLine1[0] [9] =  1;   	inLine1[0] [10] =  1;   inLine1[0] [11] =  1;  	inLine1[0] [12] =  0;  	
inLine1[1] [8]  =  0; 	inLine1[1] [9] =  1;   	inLine1[1] [10] =  1;   inLine1[1] [11] =  1;  	inLine1[1] [12] =  0;  	
inLine1[2] [8]  =  1; 	inLine1[2] [9] =  1;   	inLine1[2] [10] =  0;   inLine1[2] [11] =  1;  	inLine1[2] [12] =  1;  	
inLine1[3] [8]  =  1; 	inLine1[3] [9] =  0;   	inLine1[3] [10] =  1;   inLine1[3] [11] =  0;  	inLine1[3] [12] =  1;  	
inLine1[4] [8]  =  1; 	inLine1[4] [9] =  0;   	inLine1[4] [10] =  1;   inLine1[4] [11] =  1;  	inLine1[4] [12] =  1;  	
inLine1[5] [8]  =  1; 	inLine1[5] [9] =  1;   	inLine1[5] [10] =  0;   inLine1[5] [11] =  0;  	inLine1[5] [12] =  1;  	
inLine1[6] [8]  =  0; 	inLine1[6] [9] =  0;   	inLine1[6] [10] =  3;   inLine1[6] [11] =  1;  	inLine1[6] [12] =  1;  	
inLine1[7] [8]  =  4; 	inLine1[7] [9] =  0;   	inLine1[7] [10] =  0;   inLine1[7] [11] =  0;  	inLine1[7] [12] =  1;  	
inLine1[8] [8]  =  0; 	inLine1[8] [9] =  1;   	inLine1[8] [10] =  1;   inLine1[8] [11] =  0;  	inLine1[8] [12] =  1;  	
inLine1[9] [8]  =  0; 	inLine1[9] [9] =  0;   	inLine1[9] [10] =  1;   inLine1[9] [11] =  1;  	inLine1[9] [12] =  0;  	
inLine1[10][8] =   0; 	inLine1[10][9] =  0;   	inLine1[10][10] =  1;   inLine1[10][11] =  1;  	inLine1[10][12] =  1;  	

inLine1[0] [13] =  1;  	inLine1[0] [14] =  0;	inLine1[0] [15] =  1;   inLine1[0] [16] =  1;   inLine1[0] [17] =  1;    	
inLine1[1] [13] =  1;  	inLine1[1] [14] =  1;	inLine1[1] [15] =  0;   inLine1[1] [16] =  0;   inLine1[1] [17] =  0;    	
inLine1[2] [13] =  1;  	inLine1[2] [14] =  0;	inLine1[2] [15] =  1;   inLine1[2] [16] =  1;   inLine1[2] [17] =  0;    	
inLine1[3] [13] =  0;  	inLine1[3] [14] =  0;	inLine1[3] [15] =  1;   inLine1[3] [16] =  0;   inLine1[3] [17] =  0;    	
inLine1[4] [13] =  0;  	inLine1[4] [14] =  1;	inLine1[4] [15] =  0;   inLine1[4] [16] =  1;   inLine1[4] [17] =  1;    	
inLine1[5] [13] =  0;  	inLine1[5] [14] =  0;	inLine1[5] [15] =  1;   inLine1[5] [16] =  1;   inLine1[5] [17] =  1;    	
inLine1[6] [13] =  0;  	inLine1[6] [14] =  1;	inLine1[6] [15] =  1;   inLine1[6] [16] =  0;   inLine1[6] [17] =  0;    	
inLine1[7] [13] =  1;  	inLine1[7] [14] =  1;	inLine1[7] [15] =  0;   inLine1[7] [16] =  0;   inLine1[7] [17] =  1;    	
inLine1[8] [13] =  1;  	inLine1[8] [14] =  0;	inLine1[8] [15] =  0;   inLine1[8] [16] =  1;   inLine1[8] [17] =  1;    	
inLine1[9] [13] =  0;  	inLine1[9] [14] =  0;	inLine1[9] [15] =  1;   inLine1[9] [16] =  0;   inLine1[9] [17] =  1;    	
inLine1[10][13] =  1;  	inLine1[10][14] =  0;	inLine1[10][15] =  1;   inLine1[10][16] =  1;   inLine1[10][17] =  1;    	

inLine1[0] [18] =  0;  	inLine1[0] [19] =  0; 	inLine1[0] [20] =  1;  	inLine1[0] [21] =  1;	inLine1[0] [22] =  0;  	inLine1[0] [23] =  0;  
inLine1[1] [18] =  1;  	inLine1[1] [19] =  1; 	inLine1[1] [20] =  1;  	inLine1[1] [21] =  0;	inLine1[1] [22] =  0;  	inLine1[1] [23] =  1;  
inLine1[2] [18] =  1;  	inLine1[2] [19] =  1; 	inLine1[2] [20] =  1;  	inLine1[2] [21] =  1;	inLine1[2] [22] =  1;  	inLine1[2] [23] =  1;  
inLine1[3] [18] =  0;  	inLine1[3] [19] =  0; 	inLine1[3] [20] =  1;  	inLine1[3] [21] =  1;	inLine1[3] [22] =  1;  	inLine1[3] [23] =  0;  
inLine1[4] [18] =  1;  	inLine1[4] [19] =  1; 	inLine1[4] [20] =  0;  	inLine1[4] [21] =  0;	inLine1[4] [22] =  0;  	inLine1[4] [23] =  1;  
inLine1[5] [18] =  1;  	inLine1[5] [19] =  0; 	inLine1[5] [20] =  0;  	inLine1[5] [21] =  0;	inLine1[5] [22] =  1;  	inLine1[5] [23] =  0;  
inLine1[6] [18] =  1;  	inLine1[6] [19] =  1; 	inLine1[6] [20] =  1;  	inLine1[6] [21] =  1;	inLine1[6] [22] =  0;  	inLine1[6] [23] =  1;  
inLine1[7] [18] =  1;  	inLine1[7] [19] =  1; 	inLine1[7] [20] =  0;  	inLine1[7] [21] =  1;	inLine1[7] [22] =  1;  	inLine1[7] [23] =  1;  
inLine1[8] [18] =  1;  	inLine1[8] [19] =  1; 	inLine1[8] [20] =  0;  	inLine1[8] [21] =  1;	inLine1[8] [22] =  0;  	inLine1[8] [23] =  0;  
inLine1[9] [18] =  1;  	inLine1[9] [19] =  0; 	inLine1[9] [20] =  1;  	inLine1[9] [21] =  1;	inLine1[9] [22] =  0;  	inLine1[9] [23] =  0;  
inLine1[10][18] =  0;  	inLine1[10][19] =  1; 	inLine1[10][20] =  0;  	inLine1[10][21] =  1;	inLine1[10][22] =  0;  	inLine1[10][23] =  0;  

	
inLine2[0] [8]  =  0;   inLine2[0] [9] =  1;   	inLine2[0] [10] =  1;   inLine2[0] [11] = 0;  	inLine2[0] [12] = 1;  	
inLine2[1] [8]  =  0;   inLine2[1] [9] =  0;   	inLine2[1] [10] =  0;   inLine2[1] [11] = 1;  	inLine2[1] [12] = 1;  	
inLine2[2] [8]  =  1;   inLine2[2] [9] =  0;   	inLine2[2] [10] =  0;   inLine2[2] [11] = 0;  	inLine2[2] [12] = 1;  	
inLine2[3] [8]  =  1;   inLine2[3] [9] =  0;   	inLine2[3] [10] =  1;   inLine2[3] [11] = 1;  	inLine2[3] [12] = 0;  	
inLine2[4] [8]  =  0;   inLine2[4] [9] =  3;   	inLine2[4] [10] =  0;   inLine2[4] [11] = 0;  	inLine2[4] [12] = 0;  	
inLine2[5] [8]  =  0;   inLine2[5] [9] =  0;   	inLine2[5] [10] =  1;   inLine2[5] [11] = 1;  	inLine2[5] [12] = 1;  	
inLine2[6] [8]  =  0;   inLine2[6] [9] =  0;   	inLine2[6] [10] =  1;   inLine2[6] [11] = 1;  	inLine2[6] [12] = 1;  	
inLine2[7] [8]  =  0;   inLine2[7] [9] =  1;   	inLine2[7] [10] =  1;   inLine2[7] [11] = 0;  	inLine2[7] [12] = 1;  	
inLine2[8] [8]  =  0;   inLine2[8] [9] =  1;   	inLine2[8] [10] =  0;   inLine2[8] [11] = 0;  	inLine2[8] [12] = 0;  	
inLine2[9] [8]  =  4;   inLine2[9] [9] =  0;   	inLine2[9] [10] =  0;   inLine2[9] [11] = 1;  	inLine2[9] [12] = 1;  	
inLine2[10][8] =   0;   inLine2[10][9] =  1;   	inLine2[10][10] =  0;   inLine2[10][11] = 1;  	inLine2[10][12] = 1;  	

inLine2[0] [13] =  0;   inLine2[0] [14] =  0;	inLine2[0] [15] =  1;   inLine2[0] [16] =  0;   inLine2[0] [17] =  1;   
inLine2[1] [13] =  0;   inLine2[1] [14] =  1;	inLine2[1] [15] =  0;   inLine2[1] [16] =  1;   inLine2[1] [17] =  1;   
inLine2[2] [13] =  0;   inLine2[2] [14] =  0;	inLine2[2] [15] =  1;   inLine2[2] [16] =  0;   inLine2[2] [17] =  1;   
inLine2[3] [13] =  1;   inLine2[3] [14] =  0;	inLine2[3] [15] =  0;   inLine2[3] [16] =  1;   inLine2[3] [17] =  0;   
inLine2[4] [13] =  0;   inLine2[4] [14] =  1;	inLine2[4] [15] =  1;   inLine2[4] [16] =  0;   inLine2[4] [17] =  1;   
inLine2[5] [13] =  1;   inLine2[5] [14] =  1;	inLine2[5] [15] =  1;   inLine2[5] [16] =  0;   inLine2[5] [17] =  0;   
inLine2[6] [13] =  0;   inLine2[6] [14] =  1;	inLine2[6] [15] =  0;   inLine2[6] [16] =  1;   inLine2[6] [17] =  1;   
inLine2[7] [13] =  0;   inLine2[7] [14] =  0;	inLine2[7] [15] =  1;   inLine2[7] [16] =  0;   inLine2[7] [17] =  0;   
inLine2[8] [13] =  0;   inLine2[8] [14] =  1;	inLine2[8] [15] =  1;   inLine2[8] [16] =  1;   inLine2[8] [17] =  1;   
inLine2[9] [13] =  1;   inLine2[9] [14] =  0;	inLine2[9] [15] =  1;   inLine2[9] [16] =  0;   inLine2[9] [17] =  0;   
inLine2[10][13] =  0;   inLine2[10][14] =  1;	inLine2[10][15] =  1;   inLine2[10][16] =  1;   inLine2[10][17] =  1;   

inLine2[0] [18] =  0;  	inLine2[0] [19] =  0;  	inLine2[0] [20] =  1;  	inLine2[0] [21] =  1;	inLine2[0] [22] =  1;  inLine2[0] [23] =  1;  	
inLine2[1] [18] =  0;  	inLine2[1] [19] =  1;  	inLine2[1] [20] =  1;  	inLine2[1] [21] =  0;	inLine2[1] [22] =  0;  inLine2[1] [23] =  0;  	
inLine2[2] [18] =  0;  	inLine2[2] [19] =  1;  	inLine2[2] [20] =  0;  	inLine2[2] [21] =  1;	inLine2[2] [22] =  0;  inLine2[2] [23] =  0;  	
inLine2[3] [18] =  0;  	inLine2[3] [19] =  0;  	inLine2[3] [20] =  1;  	inLine2[3] [21] =  1;	inLine2[3] [22] =  0;  inLine2[3] [23] =  0;  	
inLine2[4] [18] =  0;  	inLine2[4] [19] =  0;  	inLine2[4] [20] =  1;  	inLine2[4] [21] =  0;	inLine2[4] [22] =  0;  inLine2[4] [23] =  1;  	
inLine2[5] [18] =  1;  	inLine2[5] [19] =  0;  	inLine2[5] [20] =  1;  	inLine2[5] [21] =  1;	inLine2[5] [22] =  1;  inLine2[5] [23] =  0;  	
inLine2[6] [18] =  1;  	inLine2[6] [19] =  1;  	inLine2[6] [20] =  0;  	inLine2[6] [21] =  0;	inLine2[6] [22] =  0;  inLine2[6] [23] =  1;  	
inLine2[7] [18] =  1;  	inLine2[7] [19] =  0;  	inLine2[7] [20] =  0;  	inLine2[7] [21] =  1;	inLine2[7] [22] =  1;  inLine2[7] [23] =  0;  	
inLine2[8] [18] =  1;  	inLine2[8] [19] =  0;  	inLine2[8] [20] =  0;  	inLine2[8] [21] =  1;	inLine2[8] [22] =  1;  inLine2[8] [23] =  1;  	
inLine2[9] [18] =  1;  	inLine2[9] [19] =  1;  	inLine2[9] [20] =  1;  	inLine2[9] [21] =  1;	inLine2[9] [22] =  0;  inLine2[9] [23] =  1;  	
inLine2[10][18] =  1;  	inLine2[10][19] =  0;  	inLine2[10][20] =  1;  	inLine2[10][21] =  0;	inLine2[10][22] =  0;  inLine2[10][23] =  1;  	

	sumOfAbsDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);	
	mvcvSumOfAbsDiff11x11(inLine1, inLine2, outLineC, width);	
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
	outputCheck.CompareArrays(outLineC[0], outLineExpected[0], width);
}


TEST_F(sumOfAbsDiff11x11Test, TestRandomInput)
{
	width = 320;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 2);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 2);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	     
	
	sumOfAbsDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff11x11(inLine1, inLine2, outLineC, width);				
		
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfAbsDiff11x11Test, TestRandomInputMaxWidth)
{
	width = 1920;
	inputGen.SelectGenerator("random");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0, 3);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	     
		
	sumOfAbsDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff11x11(inLine1, inLine2, outLineC, width);		

	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfAbsDiff11x11Test, TestRandomInputMinWidth0Padding)
{
	width = 8;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0);
	inLine2 = inputGen.GetLines(width + PADDING, SIZE, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	  
	inputGen.SelectGenerator("random");	
	for(int i = 0; i < SIZE; i++)
	{
		inputGen.FillLineRange(inLine1[i], width + PADDING, PADDING/2 - 1, width + PADDING / 2 - 1, 0, 3, 1);
		inputGen.FillLineRange(inLine2[i], width + PADDING, PADDING/2 - 1, width + PADDING / 2 - 1, 0, 3, 1);
	}
	
	sumOfAbsDiff11x11_asm_test(inLine1, inLine2, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	inLine2 = inputGen.GetOffsettedLines(inLine2, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff11x11(inLine1, inLine2, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}


TEST_F(sumOfAbsDiff11x11Test, TestInput1EqInput2)
{
	width = 240;
	inputGen.SelectGenerator("uniform");
	inLine1 = inputGen.GetLines(width + PADDING, SIZE, 0);
	outLineC = inputGen.GetEmptyLines(width, 1);
	outLineAsm = inputGen.GetEmptyLines(width, 1);	    
	inputGen.SelectGenerator("random");	
	inputGen.FillLines(inLine1, width, SIZE, 0, 2);	
	
	sumOfAbsDiff11x11_asm_test(inLine1, inLine1, outLineAsm, width);
	RecordProperty("CyclePerPixel", sumOfAbsDiff11x11CycleCount / width);
	
	inLine1 = inputGen.GetOffsettedLines(inLine1, SIZE, PADDING / 2);
	mvcvSumOfAbsDiff11x11(inLine1, inLine1, outLineC, width);
	
	outputCheck.CompareArrays(outLineC[0], outLineAsm[0], width);
}