
#include "gtest/gtest.h"
#include <minAggregate_line.h>
#include "minAggregate_line_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

using ::testing::TestWithParam;
using ::testing::Values;

#define PADDING 16
#define OFFSET PADDING/2

class mvcvMinAggregate_lineKernelTest : public ::testing::TestWithParam< unsigned int > {
protected:
    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

	unsigned char *input1;
	unsigned char *input2;
	unsigned char *outputAsm;
	unsigned char *outputC;
	unsigned int *penaltyP1;
	unsigned int *penaltyP2;
	unsigned int width;
	unsigned int disparities;
	unsigned char activateClamp;
	unsigned char direction;
	


    InputGenerator inputGen;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;
    ArrayChecker outputChecker;

    virtual void TearDown() {}
};


TEST_F(mvcvMinAggregate_lineKernelTest, TestAll0)
{
	activateClamp = 0;	
	direction = 0;
	disparities = 32;
	width = 32;

	inputGen.SelectGenerator("random");
	
	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	
	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width);// * disparities + PADDING);
}

TEST_F(mvcvMinAggregate_lineKernelTest, TestAllBigValues)
{
	activateClamp = 0;
	direction = 0;
	disparities = 64;
	width = 64;

	inputGen.SelectGenerator("random");

	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	

	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}




TEST_F(mvcvMinAggregate_lineKernelTest, TestAllRandomValues)
{

inputGen.SelectGenerator("random");

	activateClamp = randGen->GenerateUInt(0, 1, 1);
	direction= randGen->GenerateUInt(0,1,1);
	disparities= randGen->GenerateUInt(32,64,32);
	width= randGen->GenerateUInt(32,64,32);

	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	
	

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);

	
	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvMinAggregate_lineKernelTest, TestRandomDATARandomWidth)
{ 
	disparities = 64; 

	inputGen.SelectGenerator("random");

	activateClamp = randGen->GenerateUInt(0, 1, 1);
	width= randGen->GenerateUInt(32,64,32);
	activateClamp = randGen->GenerateUInt(0, 1, 1);

	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);

	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);

}

TEST_F(mvcvMinAggregate_lineKernelTest, TestRandomDATARandom2Width)
{
	inputGen.SelectGenerator("random");
	activateClamp = randGen->GenerateUInt(0, 1, 1);
	direction= randGen->GenerateUInt(0,1,1);
	disparities= randGen->GenerateUInt(32,64,32);
	width= randGen->GenerateUInt(32,64,32);
	
	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);

	inputGen.SelectGenerator("random");
	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	
	
	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvMinAggregate_lineKernelTest, TestRandomWithRandomInputLines)
{
	inputGen.SelectGenerator("random");

	activateClamp = randGen->GenerateUInt(0, 1, 1);
	direction= randGen->GenerateUInt(0,1,1);
	disparities= randGen->GenerateUInt(32,64,32);
	width= randGen->GenerateUInt(32,64,32);

	inputGen.SelectGenerator("uniform");
	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);
	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);

	inputGen.SelectGenerator("random");
	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);
	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	
	
	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);

}


TEST_F(mvcvMinAggregate_lineKernelTest, TestUniformInputLines)
{

	activateClamp = 0;	
	direction = 0;
	disparities = 32;
	width = 32;

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);

	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvMinAggregate_lineKernelTest, TestUniformInputLinesAll255)
{
	activateClamp = 1;	
	direction = 1;
	disparities = 64;
	width = 64;

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);

	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);
}

TEST_F(mvcvMinAggregate_lineKernelTest, TestUniformInputLinesMinimumWidth)
{
	activateClamp = 0;	
	direction = 0;
	disparities = 64;
	width = 32;

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);

	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);


}

TEST_F(mvcvMinAggregate_lineKernelTest, TestMaxWidthSize)
{
	activateClamp = 0;	
	direction = 0;
	disparities = 64;
	width = 64;

	inputGen.SelectGenerator("uniform");

	outputC   = inputGen.GetEmptyLine(width * disparities + PADDING);
   	outputAsm = inputGen.GetEmptyLine(width * disparities + PADDING);

	inputGen.FillLine(outputC,  width * disparities + PADDING , 5);
	inputGen.FillLine(outputAsm,width * disparities + PADDING , 5);
	
	input1 = inputGen.GetLine((width * disparities), 0, 255);
	input2 = inputGen.GetLine((width * disparities), 0, 255);

	penaltyP1 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);
	penaltyP2 = (unsigned int*) inputGen.GetLine((width * sizeof (unsigned int)), 0, 255);

	mvcvMinAggregate_line(input1, input2, outputC + OFFSET, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	mvcvMinAggregate_line_asm_test(input1, input2, outputAsm, penaltyP1, penaltyP2, width, activateClamp, direction, disparities);
	
	RecordProperty("CyclePerPixel", mvcvMinAggregate_lineCycleCount / (width));
	outputChecker.CompareArrays(outputC, outputAsm, width * disparities + PADDING);


}






















