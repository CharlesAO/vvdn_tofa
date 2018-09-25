#include "InputGenerator.h"
#include "RandomGenerator.h"
#include <string>
#include <memory>
#include <map>
#include <cstdio>
#include "half.h"

InputGenerator::InputGenerator()
{
	std::auto_ptr<RandomGenerator> randGen;
	randGen.reset(new RandomGenerator);
	dataGenerators["random"] = randGen.get();
	defaultGenerator = randGen.get();
	currentGenerator = randGen.get();
	defaultName = std::string("random");
}

void InputGenerator::AddGenerator(std::string name, DataGenerator* dGen)
{
	dataGenerators[name] = dGen;
}

void InputGenerator::SelectGenerator(std::string name)
{
	std::map<std::string, DataGenerator*>::iterator it = dataGenerators.find(name);
	if(it != dataGenerators.end())
	{
		currentGenerator = dataGenerators[name];
	}
	else {
		fprintf(stderr, "generator \"%s\" not found. Default generator \"%s\" was selected\n",
				name.c_str(), defaultName.c_str());
	}
}

void InputGenerator::RemoveGenerator(std::string name)
{
	dataGenerators.erase(name);
}

unsigned char* InputGenerator::GetEmptyLine(unsigned int lineSize)
{
	unsigned char** line = allocateLines(lineSize, 1);
	return *line;
}

unsigned short* InputGenerator::GetEmptyLineU16(unsigned int lineSize)
{
	unsigned short** line = allocateLinesU16(lineSize, 1);
	return *line;
}

half* InputGenerator::GetEmptyLineFp16(unsigned int lineSize)
{
	unsigned char** line = allocateLines(lineSize * sizeof(half), 1);
	return (half*)*line;
}

float4 InputGenerator::GetEmptyLineFloat4()
{
	float4* line = allocateLineFloat4();
	return *line;
}

int4 InputGenerator::GetEmptyLineInt4()
{
	int4* line = allocateLineInt4();
	return *line;
}

uint4 InputGenerator::GetEmptyLineUInt4()
{
	uint4* line = allocateLineUInt4();
	return *line;
}

short8 InputGenerator::GetEmptyLineShort8()
{
	short8* line = allocateLineShort8();
	return *line;
}

ushort8 InputGenerator::GetEmptyLineUShort8()
{
	ushort8* line = allocateLineUShort8();
	return *line;
}

char16 InputGenerator::GetEmptyLineChar16()
{
	char16* line = allocateLineChar16();
	return *line;
}

uchar16 InputGenerator::GetEmptyLineUChar16()
{
	uchar16* line = allocateLineUChar16();
	return *line;
}

half8 InputGenerator::GetEmptyLineHalf8()
{
	half8* line = allocateLineHalf8();
	return *line;
}

unsigned char** InputGenerator::GetEmptyLines(unsigned int lineSize, unsigned int lineNo)
{
	unsigned char** line = allocateLines(lineSize, lineNo);
	return line;
}

unsigned short** InputGenerator::GetEmptyLinesU16(unsigned int lineSize, unsigned int lineNo)
{
	unsigned short** line = allocateLinesU16(lineSize, lineNo);
	return line;
}

half** InputGenerator::GetEmptyLinesFp16(unsigned int lineSize, unsigned int lineNo)
{
	unsigned char** line = allocateLines(lineSize * sizeof(half), lineNo);
	return (half**)line;
}

//returns one line initialized with data from the current selected data generator
unsigned char* InputGenerator::GetLine(unsigned int lineSize, unsigned char init)
{
	unsigned char** line = allocateLines(lineSize, 1);
	currentGenerator->GenerateUChar(*line, lineSize, init);
	return *line;
}

unsigned short* InputGenerator::GetLineU16(unsigned int lineSize, unsigned short init)
{
	unsigned char** line = allocateLines(lineSize * sizeof(unsigned short), 1);
	currentGenerator->GenerateUShort((unsigned short*)*line, lineSize, init);
	return (unsigned short*)*line;
}

half* InputGenerator::GetLineFloat16(unsigned int lineSize, half init)
{
	unsigned char** line = allocateLines(lineSize * sizeof(half), 1);
	currentGenerator->GenerateFloat16((half*)*line, lineSize, init);
	return (half*)*line;
}

float* InputGenerator::GetLineFloat(unsigned int lineSize, float init)
{
	unsigned char** line = allocateLines(lineSize * sizeof(float), 1);
	currentGenerator->GenerateFloat((float*)*line, lineSize, init);
	return (float*)*line;
}

unsigned char* InputGenerator::GetLine(unsigned int lineSize, unsigned char low,
		unsigned char high, unsigned char step)
{
	unsigned char** line = allocateLines(lineSize, 1);
	currentGenerator->GenerateUChar(*line, lineSize, low, high, step);
	return *line;
}

unsigned short* InputGenerator::GetLineU16(unsigned int lineSize, unsigned short low, unsigned short high)
{
	unsigned char** line = allocateLines(lineSize * sizeof(unsigned short), 1);
	currentGenerator->GenerateUShort((unsigned short*)*line, lineSize, low, high, 0);
	return (unsigned short*)*line;
}

half* InputGenerator::GetLineFloat16(unsigned int lineSize, half low, half high)
{
	unsigned char** line = allocateLines(lineSize * sizeof(half), 1);
	currentGenerator->GenerateFloat16((half*)*line, lineSize, low, high);
	return (half*)*line;
}

float* InputGenerator::GetLineFloat(unsigned int lineSize, float low, float high)
{
	unsigned char** line = allocateLines(lineSize * sizeof(float), 1);
	currentGenerator->GenerateFloat((float*)*line, lineSize, low, high);
	return (float*)*line;
}

//reuturns one or more lines initialized with data from the current selected data generator
unsigned char** InputGenerator::GetLines(unsigned int lineSize,
		unsigned int lineNo, unsigned char init)
{
	unsigned char** lines = allocateLines(lineSize, lineNo);
	currentGenerator->GenerateUChar(lines, lineSize, lineNo, init);
	return lines;
}

unsigned char** InputGenerator::GetLines(unsigned int lineSize,
		unsigned int lineNo, unsigned char low, unsigned char high, unsigned char step)
{
	unsigned char** lines = allocateLines(lineSize, lineNo);
	currentGenerator->GenerateUChar(lines, lineSize, lineNo, low, high, step);
	return lines;
}

unsigned short** InputGenerator::GetLinesU16(unsigned int lineSize, unsigned int lineNo, unsigned short init)
{
	unsigned char** lines = allocateLines(lineSize * sizeof(unsigned short), lineNo);
	currentGenerator->GenerateUShort((unsigned short**)lines, lineSize, lineNo, init);
	return (unsigned short**)lines;
}

unsigned short** InputGenerator::GetLinesU16(unsigned int lineSize, unsigned int lineNo, unsigned short low, unsigned short high)
{
	unsigned char** lines = allocateLines(lineSize * sizeof(unsigned short), lineNo);
	currentGenerator->GenerateUShort((unsigned short**)lines, lineSize, lineNo, low, high, 0);
	return (unsigned short**)lines;
}

half** InputGenerator::GetLinesFloat16(unsigned int lineSize, unsigned int lineNo, half init)
{
	unsigned char** lines = allocateLines(lineSize * sizeof(half), lineNo);
	currentGenerator->GenerateFloat16((half**)lines, lineSize, lineNo, init);
	return (half**)lines;
}

half** InputGenerator::GetLinesFloat16(unsigned int lineSize, unsigned int lineNo, half low, half high)
{
	unsigned char** lines = allocateLines(lineSize * sizeof(half), lineNo);
	currentGenerator->GenerateFloat16((half**)lines, lineSize, lineNo, low, high);
	return (half**)lines;
}

float** InputGenerator::GetLinesFloat(unsigned int lineSize, unsigned int lineNo, float init)
{
	unsigned char** lines = allocateLines(lineSize * sizeof(float), lineNo);
	currentGenerator->GenerateFloat((float**)lines, lineSize, lineNo, init);
	return (float**)lines;
}

float** InputGenerator::GetLinesFloat(unsigned int lineSize, unsigned int lineNo, float low, float high)
{
	unsigned char** lines = allocateLines(lineSize * sizeof(float), lineNo);
	currentGenerator->GenerateFloat((float**)lines, lineSize, lineNo, low, high);
	return (float**)lines;
}



InputGenerator::~InputGenerator()
{
	freeLines();
}

unsigned char** InputGenerator::allocateLines(unsigned int lineSize, unsigned int lineNo)
{
	unsigned char** lines;

	// allocate memory for the given number of lines
	// with the given size
	lines = new unsigned char* [lineNo];
	for(unsigned int i = 0; i < lineNo; i++)
	{
		lines[i] = new unsigned char[lineSize];
	}

	data.push_back(std::make_pair(lineNo, lines));
	return lines;
}

unsigned short** InputGenerator::allocateLinesU16(unsigned int lineSize, unsigned int lineNo)
{
	unsigned short** lines;

	// allocate memory for the given number of lines
	// with the given size
	lines = new unsigned short* [lineNo];
	for(unsigned int i = 0; i < lineNo; i++)
	{
		lines[i] = new unsigned short[lineSize];
	}

	data.push_back(std::make_pair(lineNo, (unsigned char **)lines));
	return lines;
}

half** InputGenerator::allocateLinesFp16(unsigned int lineSize, unsigned int lineNo)
{
	half** lines;

	// allocate memory for the given number of lines
	// with the given size
	lines = new half* [lineNo];
	for(unsigned int i = 0; i < lineNo; i++)
	{
		lines[i] = new half[lineSize];
	}

	data.push_back(std::make_pair(lineNo, (unsigned char **)lines));
	return lines;
}

float4* InputGenerator::allocateLineFloat4()
{
	float4* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new float4 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

int4* InputGenerator::allocateLineInt4()
{
	int4* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new int4 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

uint4* InputGenerator::allocateLineUInt4()
{
	uint4* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new uint4 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

short8* InputGenerator::allocateLineShort8()
{
	short8* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new short8 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

ushort8* InputGenerator::allocateLineUShort8()
{
	ushort8* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new ushort8 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

char16* InputGenerator::allocateLineChar16()
{
	char16* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new char16 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

uchar16* InputGenerator::allocateLineUChar16()
{
	uchar16* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new uchar16 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

half8* InputGenerator::allocateLineHalf8()
{
	half8* line;

	// allocate memory for the given number of lines
	// with the given size
	line = new half8 [1];

	//data.push_back(std::make_pair(lineNo, lines));
	return line;
}

void InputGenerator::freeLines()
{
	for(unsigned int i = 0; i < data.size(); i++)
	{
		for(unsigned int j = 0; j < data[i].first; j++)
		{
			delete[] (data[i].second)[j];
		}
		delete[] data[i].second;
	}
}

void InputGenerator::FillLine(unsigned char* line, unsigned int lineSize, unsigned char init)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = currentGenerator->GenerateUChar(init);
	}
}

void InputGenerator::FillLineRange(unsigned char* line, unsigned int lineSize, unsigned int startPos, unsigned int endPos, unsigned char init)
{
	if(startPos > lineSize - 1)
	{
		startPos = 0;
	}
	if(endPos > lineSize - 1)
	{
		endPos = lineSize - 1;
	}
	FillLine(&line[startPos], endPos - startPos, init);
}

void InputGenerator::FillLine(unsigned char* line, unsigned int lineSize, unsigned char low, unsigned char high, unsigned char step)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = currentGenerator->GenerateUChar(low, high, step);
	}
}

void InputGenerator::FillLineRange(unsigned char* line, unsigned int lineSize, unsigned int startPos, unsigned int endPos, unsigned char low, unsigned char high, unsigned char step)
{
	if(startPos > lineSize - 1)
	{
		startPos = 0;
	}
	if(endPos > lineSize - 1)
	{
		endPos = lineSize - 1;
	}
	FillLine(&line[startPos], endPos - startPos, low, high, step);
}

void InputGenerator::FillLines(unsigned char** lines, unsigned int lineSize, unsigned int lineNo, unsigned int init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, init);
	}
}

void InputGenerator::FillLines(unsigned char** lines, unsigned int lineSize, unsigned int lineNo, unsigned char low, unsigned char high)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, low, high);
	}
}

void InputGenerator::FillLine(unsigned short* line, unsigned int lineSize, unsigned short init)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = currentGenerator->GenerateUShort(init);
	}
}

void InputGenerator::FillLineRange(unsigned short* line, unsigned int lineSize, unsigned int startPos, unsigned int endPos, unsigned short init)
{
	if(startPos > lineSize - 1)
	{
		startPos = 0;
	}
	if(endPos > lineSize - 1)
	{
		endPos = lineSize - 1;
	}
	FillLine(&line[startPos], endPos - startPos, init);
}

void InputGenerator::FillLine(unsigned short* line, unsigned int lineSize, unsigned short low, unsigned short high, unsigned short step)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = currentGenerator->GenerateUShort(low, high, step);
	}
}

void InputGenerator::FillLineRange(unsigned short* line, unsigned int lineSize, unsigned int startPos, unsigned int endPos, unsigned short low, unsigned short high, unsigned short step)
{
	if(startPos > lineSize - 1)
	{
		startPos = 0;
	}
	if(endPos > lineSize - 1)
	{
		endPos = lineSize - 1;
	}
	FillLine(&line[startPos], endPos - startPos, low, high, step);
}

void InputGenerator::FillLines(unsigned short** lines, unsigned int lineSize, unsigned int lineNo, unsigned int init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, init);
	}
}

void InputGenerator::FillLines(unsigned short** lines, unsigned int lineSize, unsigned int lineNo, unsigned short low, unsigned short high)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, low, high, 0);
	}
}

void InputGenerator::FillLine(float* line, unsigned int lineSize, float init)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = currentGenerator->GenerateFloat(init);
	}
}

void InputGenerator::FillLine(half* line, unsigned int lineSize, half init)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = (half)currentGenerator->GenerateFloat(init);
	}
}

void InputGenerator::FillLine(half* line, unsigned int lineSize, half low, half high)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = (half)currentGenerator->GenerateFloat(low, high);
	}
}

void InputGenerator::FillLineRange(float* line, unsigned int lineSize,
		unsigned int startPos, unsigned int endPos, float init)
{
	if(startPos > lineSize - 1)
	{
		startPos = 0;
	}
	if(endPos > lineSize - 1)
	{
		endPos = lineSize - 1;
	}
	FillLine(&line[startPos], endPos - startPos, init);
}


void InputGenerator::FillLine(float* line, unsigned int lineSize,
		float low, float high)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		line[i] = currentGenerator->GenerateFloat(low, high);
	}
}

void InputGenerator::FillLineRange(float* line, unsigned int lineSize,
		unsigned int startPos, unsigned int endPos, float low, float high)
{
	if(startPos > lineSize - 1)
	{
		startPos = 0;
	}
	if(endPos > lineSize - 1)
	{
		endPos = lineSize - 1;
	}
	FillLine(&line[startPos], endPos - startPos, low, high);
}

void InputGenerator::FillLines(float** lines, unsigned int lineSize, unsigned int lineNo, float init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, init);
	}
}

void InputGenerator::FillLines(float** lines, unsigned int lineSize, unsigned int lineNo, float low, float high)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, low, high);
	}
}

void InputGenerator::FillLines(half** lines, unsigned int lineSize, unsigned int lineNo, half init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, init);
	}
}

void InputGenerator::FillLines(half** lines, unsigned int lineSize, unsigned int lineNo, half low, half high)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		FillLine(lines[i], lineSize, low, high);
	}
}

unsigned char** InputGenerator::GetOffsettedLines(unsigned char** lines, unsigned int lineNo, unsigned int offset)
{
	unsigned char** outLines = new unsigned char* [lineNo];
	data.push_back(std::make_pair(0, outLines));
	for(unsigned int i = 0; i < lineNo; i++)
	{
		outLines[i] = lines[i] + offset;
	}
	return outLines;
}

unsigned short** InputGenerator::GetOffsettedLines(unsigned short** lines, unsigned int lineNo, unsigned int offset)
{
	unsigned short** outLines = new unsigned short* [lineNo];
	data.push_back(std::make_pair(0, (unsigned char **)outLines));
	for(unsigned int i = 0; i < lineNo; i++)
	{
		outLines[i] = lines[i] + offset;
	}
	return outLines;
}

half** InputGenerator::GetOffsettedLines(half** lines, unsigned int lineNo, unsigned int offset)
{
	half** outLines = new half* [lineNo];
	data.push_back(std::make_pair(0, (unsigned char **)outLines));
	for(unsigned int i = 0; i < lineNo; i++)
	{
		outLines[i] = lines[i] + offset;
	}
	return outLines;
}

