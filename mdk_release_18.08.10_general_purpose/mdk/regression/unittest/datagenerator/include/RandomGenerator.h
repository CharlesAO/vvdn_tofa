#ifndef RANDOMGENERATOR_H_
#define RANDOMGENERATOR_H_

#include "DataGenerator.h"
#include "FunctionInfo.h"

class RandomGenerator : public DataGenerator {
public:
	using DataGenerator::GenerateUInt;
	using DataGenerator::GenerateUChar;
	using DataGenerator::GenerateFloat;

	RandomGenerator();
	virtual ~RandomGenerator();

	// generates a random unsigned int between low and high; optionally if step is
	// not 0 the generated value is a multiple of step
	unsigned int GenerateUInt(unsigned int low, unsigned int high, unsigned int step = 0);
	// generates a random unsigned int between 0 and 65535
	unsigned int GenerateUInt(unsigned int init);

	// generates a random unsigned short between low and high; optionally if step is
	// not 0 the generated value is a multiple of step
	unsigned short GenerateUShort(unsigned short low, unsigned short high, unsigned int step = 0);
	// generates a random unsigned short between 0 and 65535
    unsigned short GenerateUShort(unsigned short init);
    
	// generates a random unsigned char between low and high; optionally if step is
	// not 0 the generated value is a multiple of step
	unsigned char GenerateUChar(unsigned char low, unsigned char high, unsigned int step = 0);
	// generates a random unsigned char between 0 and 255
	unsigned char GenerateUChar(unsigned char init);

	// generates a random float value between low and high
	float GenerateFloat(float low, float high);
	// generates a random float value between 0.0 and 1.0
	float GenerateFloat(float init);
	// generates a random 16 bit float value between low and high
	half GenerateFloat16(half low, half high);
	// generates a random 16 bit float value between 0.0 and 1.0
	half GenerateFloat16(half init);
};

#endif /* RANDOMGENERATOR_H_ */
