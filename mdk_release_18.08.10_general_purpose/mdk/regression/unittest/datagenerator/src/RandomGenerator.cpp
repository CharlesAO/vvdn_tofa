#include "RandomGenerator.h"
#include "DataGenerator.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>

RandomGenerator::RandomGenerator()
{
	unsigned int rand_seed;
	FunctionInfo info = FunctionInfo::Instance();
	unsigned int user_seed = info.getUserSeed();
	if (user_seed != 0)
	{
		rand_seed = user_seed;
	}
	else
	{
		rand_seed = time(NULL);
	}
	srand(rand_seed);

	info.setRandSeed(rand_seed);

}

RandomGenerator::~RandomGenerator() {
	// TODO Auto-generated destructor stub
}

unsigned int RandomGenerator::GenerateUInt(unsigned int low,
		unsigned int high, unsigned int step)
{
	unsigned int value = low + rand() % (high - low);;
	if((step > 0) && (step < high))
	{
		//set value to the nearest lower value multiple of step
		value -= value % step;
	}
	return value;
}

unsigned short RandomGenerator::GenerateUShort(unsigned short low,
		unsigned short high, unsigned int step)
{
	return static_cast<unsigned short>(GenerateUInt(low, high, step));
}

unsigned char RandomGenerator::GenerateUChar(unsigned char low,
		unsigned char high, unsigned int step)
{
	return static_cast<char>(GenerateUInt(low, high, step));
}

float RandomGenerator::GenerateFloat(float low, float high)
{
	return low + ((high - low) * ((float)rand() / (float)RAND_MAX));
}


unsigned int RandomGenerator::GenerateUInt(unsigned int)
{
	return GenerateUInt(0, 65536, 0);
}

unsigned short RandomGenerator::GenerateUShort(unsigned short)
{
	return GenerateUShort(0, 65535, 0);
}

unsigned char RandomGenerator::GenerateUChar(unsigned char)
{
	return GenerateUChar((unsigned char)0, (unsigned char)255, 0);
}

float RandomGenerator::GenerateFloat(float)
{
	return GenerateFloat(0.0, 1.0);
}

half RandomGenerator::GenerateFloat16(half low, half high)
{
	return (half)GenerateFloat(low, high);
}

half RandomGenerator::GenerateFloat16(half)
{
	return GenerateFloat16(0.0, 1.0);
}
