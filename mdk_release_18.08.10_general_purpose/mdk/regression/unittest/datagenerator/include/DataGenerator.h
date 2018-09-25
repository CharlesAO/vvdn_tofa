#ifndef DATA_GENERATOR_H_
#define DATA_GENERATOR_H_

#include "half.h"

class DataGenerator {
public:
	DataGenerator() {}
	void Setup();

	virtual unsigned int GenerateUInt(unsigned int/* init*/)
	{
		return 0;
	}

	virtual unsigned int GenerateUInt(unsigned int/* low*/, unsigned int/* high*/, unsigned int/* step*/)
	{
		return 0;
	}
	void GenerateUInt(unsigned int* out, unsigned int size,	unsigned int low, unsigned int high, unsigned int step);

           
           
	virtual unsigned short GenerateUShort(unsigned short/* low*/, unsigned short/* high*/, unsigned int/* step*/)
	{
		return 0;
	}

	virtual unsigned short GenerateUShort(unsigned short/* init*/)
	{
		return 0;
	}
	void GenerateUShort(unsigned short* out, unsigned int size, unsigned short init);
	void GenerateUShort(unsigned short* out, unsigned int size, unsigned short low, unsigned short high, unsigned int step);
	void GenerateUShort(unsigned short** out, unsigned int lineSize, unsigned int lineNo, unsigned short init);
	void GenerateUShort(unsigned short** out, unsigned int lineSize, unsigned int lineNo, unsigned short low, unsigned short high, unsigned int step);          
          
          
	virtual unsigned char GenerateUChar(unsigned char/* low*/, unsigned char/* high*/, unsigned int/* step*/)
	{
		return 0;
	}


	virtual unsigned char GenerateUChar(unsigned char/* init*/)
	{
		return 0;
	}
	void GenerateUChar(unsigned char* out, unsigned int size, unsigned char init);
	void GenerateUChar(unsigned char* out, unsigned int size, unsigned char low, unsigned char high, unsigned int step);
	void GenerateUChar(unsigned char** out, unsigned int lineSize, unsigned int lineNo, unsigned char init);
	void GenerateUChar(unsigned char** out, unsigned int lineSize, unsigned int lineNo, unsigned char low, unsigned char high, unsigned int step);

	virtual float GenerateFloat(float/* init*/)
	{
		return 0.0f;
	}
	virtual float GenerateFloat(float/* low*/, float/* high*/)
	{
		return 0.0f;
	}

	virtual half GenerateFloat16(half/* init*/)
	{
		return 0.0f;
	}

	virtual half GenerateFloat16(half/* low*/, half/* high*/)
	{
		return 0.0f;
	}
	void GenerateFloat(float* out, unsigned int size, float init);
	void GenerateFloat(float** out, unsigned int lineSize, unsigned int lineNo, float init);
	void GenerateFloat(float* out, unsigned int size, float low, float high);
	void GenerateFloat(float** out, unsigned int lineSize, unsigned int lineNo, float low, float high);
    
	void GenerateFloat16(half* out, unsigned int size, half init);
	void GenerateFloat16(half** out, unsigned int lineSize, unsigned int lineNo, half init);
	void GenerateFloat16(half* out, unsigned int size, half low, half high);
	void GenerateFloat16(half** out, unsigned int lineSize, unsigned int lineNo, half low, half high);

	virtual ~DataGenerator();
};

#endif /* DATA_GENERATOR_H_ */
