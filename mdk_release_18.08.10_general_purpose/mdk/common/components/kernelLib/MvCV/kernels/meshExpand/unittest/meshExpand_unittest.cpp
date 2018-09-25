//meshExpand kernel test

//Asm function prototype:
//	void MeshExpand(u8 *out, u8 **in, u32 width, half **mesh, float my)

//Asm test function prototype:
//	void meshExpand_asm_test(unsigned char *output, unsigned char **input, unsigned int width, half** mesh, float my)

//C function prototype:
//void mvcvMeshExpand(u8 *out, u8 **in, u32 width, half **mesh, float my)

// out          - array of output line
// in 	       	- array of input line
// width	    - width of output line
// mesh    		- array of output_mesh line
// my    		- y coordinate

#include "gtest/gtest.h"
#include "meshExpand.h"
#include "meshGenerate.h"
#include "meshExpand_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>
#include <math.h>

#define OUT_PADDING 16
#define INPUT_PADDING 16
#define HEIGHT_PADDING 64

const char DELTA = 1;


using ::testing::TestWithParam;
using ::testing::Values;
 
class meshExpandTest : public ::testing::TestWithParam <unsigned long> {
protected:

	virtual void SetUp()
	{
		randGen.reset(new RandomGenerator);
		uniGen.reset(new UniformGenerator);
		inputGen.AddGenerator(std::string("random"), randGen.get());
		inputGen.AddGenerator(std::string("uniform"), uniGen.get());
	}	
	
	u8 **inLine;
	u8 *outLineC;
	u8 *outLineAsm;
	half **mesh;
	unsigned int width;
	InputGenerator inputGen;
	RandomGenerator randomGen;
	ArrayChecker outputCheck;
	
	std::auto_ptr<RandomGenerator>  randGen;
	std::auto_ptr<UniformGenerator>  uniGen;

	virtual void TearDown() {}
};

TEST_F(meshExpandTest, TestUniformInput32)
{
	width = 32;
	
	unsigned char p = randGen->GenerateUInt(0, 255);
	
	inputGen.SelectGenerator("uniform");
	u8**   inLine     = inputGen.GetLines(width + INPUT_PADDING, 64, p);
	inputGen.SelectGenerator("uniform");
	half** mesh       = inputGen.GetLinesFloat16(width, 2, 0.0f);
	u8* outLineC = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	u8* outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	
 	half m = randGen->GenerateFloat(-8.0f, 8.0f);
 	half m1 = randGen->GenerateFloat(-8.0f, 8.0f);
	
	for (int i=0; i<(int)width; i+=2) {
		mesh[0][i] = m;
		mesh[0][i+1] = m1; 
		mesh[1][i] = m;
		mesh[1][i+1] = m1;
	}
	
	float my = randGen->GenerateFloat(0, 32);

	mvcvMeshExpand(&outLineC[OUT_PADDING], &inLine[32], width, mesh, my);
	meshExpand_asm_test(outLineAsm, inLine, width, mesh, my);
	
	 if (m < (half)0){
		for (int i = OUT_PADDING; i < (OUT_PADDING + 1 + (-round(mesh[0][0]))); i++ ) {
			outLineC[i] = 0;
			outLineAsm[i] = 0;
		}
	}
	
	for (int i = (width + OUT_PADDING - (1+(round(mesh[0][0])))); i < (int)(width + OUT_PADDING); i++ ) {
		outLineC[i] = 0;
		outLineAsm[i] = 0;
	}
	
	RecordProperty("CyclePerPixel", meshExpandCycleCount / width);		
	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2*OUT_PADDING, DELTA);				
} 


TEST_F(meshExpandTest, TestRandomInput)
{
	width = 32;
	
	unsigned char p = randGen->GenerateUInt(0, 255);
	inputGen.SelectGenerator("uniform");
	u8**   inLine     = inputGen.GetLines(width + INPUT_PADDING, 64, p);
	inputGen.SelectGenerator("uniform");
	half** mesh       = inputGen.GetLinesFloat16(width, 2, 0.0f);
	u8* outLineC = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	u8* outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	
	
 	half m = randGen->GenerateFloat(-8.0f, 8.0f);
 	half m1 = randGen->GenerateFloat(-8.0f, 8.0f);
	for (int i=0; i<(int)width; i+=2) {
		mesh[0][i] = m;
		mesh[0][i+1] = m; 
		mesh[1][i] = m;
		mesh[1][i+1] = m;
	}
	
	float my = 0.75;//randGen->GenerateFloat(0, 32);

	mvcvMeshExpand(&outLineC[OUT_PADDING], &inLine[32], width, mesh, my);
	meshExpand_asm_test(outLineAsm, inLine, width, mesh, my);
	
	if (m < (half)0){
		for (int i = OUT_PADDING; i < (int)(OUT_PADDING + 1 + (-round(mesh[0][0]))); i++ ) {
			outLineC[i] = 0;
			outLineAsm[i] = 0;
		}
	}
	
	for (int i = (width + OUT_PADDING - (1+(round(mesh[0][0])))); i < (int)(width + OUT_PADDING); i++ ) {
		outLineC[i] = 0;
		outLineAsm[i] = 0;
	}
	
	RecordProperty("CyclePerPixel", meshExpandCycleCount / width);		
	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2*OUT_PADDING, DELTA);
} 


TEST_F(meshExpandTest, TestUniformInput)
{
	width = 64;
	inputGen.SelectGenerator("uniform");
	u8**   inLine     = inputGen.GetLines(width + INPUT_PADDING, 64, 25);
	inputGen.SelectGenerator("uniform");
	half** mesh       = inputGen.GetLinesFloat16(width + INPUT_PADDING, 2, 0.0f);
	u8* outLineC = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	u8* outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	
 	half m = randGen->GenerateFloat(-8.0f, 8.0f);
 	half m1 = randGen->GenerateFloat(-8.0f, 8.0f);
	
	for (int i=0; i<(int)width; i+=2) {
		mesh[0][i] = m;
		mesh[0][i+1] = m1; 
		mesh[1][i] = m;
		mesh[1][i+1] = m1;
	}
	
	float my = randGen->GenerateFloat(0, 32);
 
	mvcvMeshExpand(&outLineC[OUT_PADDING], &inLine[32], width, mesh, my);
	meshExpand_asm_test(outLineAsm, inLine, width, mesh, my);
	
	if (m < (half)0){
		for (int i = OUT_PADDING; i < (OUT_PADDING + 1 + (-round(mesh[0][0]))); i++ ) {
			outLineC[i] = 0;
			outLineAsm[i] = 0;
		}
	}
 
	for (int i = (width + OUT_PADDING - (1+(round(mesh[0][0])))); i < (int)(width + OUT_PADDING); i++ ) {
	outLineC[i] = 0;
	outLineAsm[i] = 0;
	}

	RecordProperty("CyclePerPixel", meshExpandCycleCount / width);		
	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2*OUT_PADDING, DELTA);				
} 

TEST_F(meshExpandTest, TestMeshGenerate)
{
	width = 176;
	inputGen.SelectGenerator("uniform");
	u8** inLine = inputGen.GetLines(width + INPUT_PADDING, 64, 8);
	half* mesh = inputGen.GetLineFloat16(width*2, 0);
	u8* outLineC = inputGen.GetLine(width + 2 * OUT_PADDING,0);
	u8* outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING,0);

	int posx = 0;
	int posy = 0;
	float my = 0.875f;
	half *meshh[2];
	
	int mat[6] = {(int)0x3F7F069E, (int)0xBDB27EB6, (int)0x00000000, (int)0x3DB27EB6, (int)0x3F7F069E, (int)0x00000000};
	
	mvcvMeshGenerate(mesh, (float*)mat, width>>2, posx, posy);
	
	meshh[0] = mesh;
	meshh[1] = mesh;

	mvcvMeshExpand(&outLineC[OUT_PADDING], &inLine[32], width, meshh, my);
	meshExpand_asm_test(outLineAsm, inLine, width, meshh, my);
	
	for (int i = (width + OUT_PADDING - (1+(round(meshh[0][0])))); i < (int)(width + OUT_PADDING); i++ ) {
		outLineC[i] = 0;
		outLineAsm[i] = 0;
	}

	RecordProperty("CyclePerPixel", meshExpandCycleCount / width);		
	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2*OUT_PADDING, DELTA);
} 
 
TEST_F(meshExpandTest, TestWidth512)
{
	width = 512;
	inputGen.SelectGenerator("uniform");
	u8**   inLine     = inputGen.GetLines(width + INPUT_PADDING, 64, 25);
	half** mesh       = inputGen.GetLinesFloat16(width, 2, 0.0f);
	u8* outLineC = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	u8* outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	
 	half m = randGen->GenerateFloat(-8.0f, 8.0f);
 	half m1 = randGen->GenerateFloat(-8.0f, 8.0f);
	for (int i=0; i<(int)width; i+=2) {
		mesh[0][i] = m;
		mesh[0][i+1] = m1; 
		mesh[1][i] = m;
		mesh[1][i+1] = m1;
	}
	
	float my = randGen->GenerateFloat(0, 32);
	
	mvcvMeshExpand(&outLineC[OUT_PADDING], &inLine[32], width, mesh, my);
	meshExpand_asm_test(outLineAsm, inLine, width, mesh, my);
 
    if (mesh[0][0] < (half)0){
		for (int i = OUT_PADDING; i < (OUT_PADDING + 1 + (-round(mesh[0][0]))); i++ ) {
			outLineC[i] = 0;
			outLineAsm[i] = 0;
		}
	}
	 
	for (int i = (width + OUT_PADDING - (1+(round(mesh[0][0])))); i < (int)(width + OUT_PADDING); i++ ) {
		outLineC[i] = 0;
		outLineAsm[i] = 0;
	}
	 
	 RecordProperty("CyclePerPixel", meshExpandCycleCount / width);		
	 outputCheck.CompareArrays(outLineC, outLineAsm, width + 2*OUT_PADDING, DELTA);
 } 

 
INSTANTIATE_TEST_CASE_P(RandomInputs, meshExpandTest,
	 Values((unsigned int)512, (unsigned int)640, (unsigned int)816, (unsigned int)960, (unsigned int)1280, (unsigned int)1640, (unsigned int)1920); );

TEST_P(meshExpandTest , TestRandomData)
{
	inputGen.SelectGenerator("random");
	width = GetParam();
	
	inputGen.SelectGenerator("uniform");
	u8**   inLine     = inputGen.GetLines(width + INPUT_PADDING, 64, 25);
	half** mesh       = inputGen.GetLinesFloat16(width, 2, 0.0f);
	u8* outLineC = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	u8* outLineAsm = inputGen.GetLine(width + 2 * OUT_PADDING, 0);
	
 	half m = randGen->GenerateFloat(-8.0f, 8.0f);
 	half m1 = randGen->GenerateFloat(-8.0f, 8.0f);
	for (int i=0; i<(int)width; i+=2) {
		mesh[0][i] = m;
		mesh[0][i+1] = m1; 
		mesh[1][i] = m;
		mesh[1][i+1] = m1;
	}
	
	float my = randGen->GenerateFloat(0, 32);
	
	mvcvMeshExpand(&outLineC[OUT_PADDING], &inLine[32], width, mesh, my);
	meshExpand_asm_test(outLineAsm, inLine, width, mesh, my);
	
	if (mesh[0][0] < (half)0){
		for (int i = OUT_PADDING; i < (OUT_PADDING + 1 + (-round(mesh[0][0]))); i++ ) {
			outLineC[i] = 0;
			outLineAsm[i] = 0;
		}
	}
	
	for (int i = (width + OUT_PADDING - (1+(round(mesh[0][0])))); i < (int)(width + OUT_PADDING); i++ ) {
		outLineC[i] = 0;
		outLineAsm[i] = 0;
	}
	
	RecordProperty("CyclePerPixel", meshExpandCycleCount / width);		
	outputCheck.CompareArrays(outLineC, outLineAsm, width + 2*OUT_PADDING, DELTA);
}  