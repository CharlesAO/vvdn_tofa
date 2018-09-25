#include "DataGenerator.h"
#include "half.h"

#ifndef UNIFORMGENERATOR_H_
#define UNIFORMGENERATOR_H_

class UniformGenerator : public DataGenerator {
public:
	UniformGenerator();
	virtual ~UniformGenerator();

	unsigned int GenerateUInt(unsigned int init);
    unsigned short GenerateUShort(unsigned short init);
	unsigned char GenerateUChar(unsigned char init);
	float GenerateFloat(float init);
	half GenerateFloat16(half init);

};

#endif /* UNIFORMGENERATOR_H_ */
