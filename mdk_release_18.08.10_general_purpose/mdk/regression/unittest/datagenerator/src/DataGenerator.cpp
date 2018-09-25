#include <DataGenerator.h>

void DataGenerator::GenerateUInt(unsigned int* out, unsigned int size,
		unsigned int low, unsigned int high, unsigned int step)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateUInt(low, high, step);
	}
}

void DataGenerator::GenerateUShort(unsigned short* out, unsigned int size,
	unsigned short init)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateUShort(init);
	}
}

void DataGenerator::GenerateUShort(unsigned short* out, unsigned int size,
		unsigned short low, unsigned short high, unsigned int step)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateUShort(low, high, step);
	}
}

void DataGenerator::GenerateUShort(unsigned short** out, unsigned int lineSize,
		unsigned int lineNo, unsigned short init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateUShort(out[i], lineSize, init);
	}
}

void DataGenerator::GenerateUShort(unsigned short** out, unsigned int lineSize,
		unsigned int lineNo, unsigned short low, unsigned short high, unsigned int step)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateUShort(out[i], lineSize, low, high, step);
	}
}


void DataGenerator::GenerateUChar(unsigned char* out, unsigned int size,
	unsigned char init)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateUChar(init);
	}
}

void DataGenerator::GenerateUChar(unsigned char* out, unsigned int size,
	unsigned char low, unsigned char high, unsigned int step)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateUChar(low, high, step);
	}
}


void DataGenerator::GenerateUChar(unsigned char** out, unsigned int lineSize,
		unsigned int lineNo, unsigned char init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateUChar(out[i], lineSize, init);
	}
}

void DataGenerator::GenerateUChar(unsigned char** out, unsigned int lineSize,
		unsigned int lineNo, unsigned char low, unsigned char high, unsigned int step)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateUChar(out[i], lineSize, low, high, step);
	}
}


void DataGenerator::GenerateFloat(float* out, unsigned int size, float init)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateFloat(init);
	}
}

void DataGenerator::GenerateFloat(float* out, unsigned int size, float low,
		float high)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateFloat(low, high);
	}
}

void DataGenerator::GenerateFloat16(half* out, unsigned int size, half init)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateFloat16(init);
	}
}

void DataGenerator::GenerateFloat16(half* out, unsigned int size, half low, half high)
{
	for(unsigned int i = 0; i < size; i++)
	{
		out[i] = GenerateFloat16(low, high);
	}
}

void DataGenerator::GenerateFloat(float** out, unsigned int lineSize,
		unsigned int lineNo, float low, float high)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateFloat(out[i], lineSize, low, high);
	}
}

void DataGenerator::GenerateFloat(float** out, unsigned int lineSize,
		unsigned int lineNo, float init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateFloat(out[i], lineSize, init);
	}
}

void DataGenerator::GenerateFloat16(half** out, unsigned int lineSize,
		unsigned int lineNo, half low, half high)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateFloat16(out[i], lineSize, low, high);
	}
}

void DataGenerator::GenerateFloat16(half** out, unsigned int lineSize,
		unsigned int lineNo, half init)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		GenerateFloat16(out[i], lineSize, init);
	}
}

DataGenerator::~DataGenerator() {}

