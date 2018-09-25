#ifndef ARRAYCHECKER_H_
#define ARRAYCHECKER_H_

#include "gtest/gtest.h"
#include <fstream>
#include <typeinfo>
#include <half.h>
#include <VectorTypes.h>

class ArrayChecker {
public:
	ArrayChecker();
	virtual ~ArrayChecker();

	//checks if all arrays values are equal with value
	template<class T>
	void ExpectAllEQ(T* data, unsigned int size, T value);
	//checks that all arrays values are in range [low, high]
	template<class T>
	void ExpectAllInRange(T* data, unsigned int size, T low, T high);
	//checks that all arrays values are lower than value
	template<class T>
	void ExpectAllLT(T* data, unsigned int size, T value);
	//checks that all arrays values are lower or equal than value
	template<class T>
	void ExpectAllLE(T* data, unsigned int size, T value);
	//checks that all arrays values are greater than value
	template<class T>
	void ExpectAllGT(T* data, unsigned int size, T value);
	//checks that all arrays values are greater or equal with value
	template<class T>
	void ExpectAllGE(T* data, unsigned int size, T value);
	//compares two arrays of given size
	template<class T>
	void CompareArrays(T* data1, T* data2, unsigned int size);
	//compares two 2d arrays of given line size and line number
	template<class T>
	void CompareArrays(T** data1, T** data2, unsigned int lineSize, unsigned int lineNo);
    //
	template<class T>
	void CompareArrays(T* data1, T* data2, unsigned int lineSize, T delta);
	//
	template<class T>
	void CompareArrays(T** data1, T** data2, unsigned int lineSize, unsigned int lineNo, T delta);

	//compares two float arrays of given size; a tolerance value delta can be provided
	void CompareArraysFloat(float* data1, float* data2, unsigned int lineSize, float delta = 0);
	
	//compares two vectorial arrays of given size; a tolerance value delta can be provided
	void CompareArraysFloat4(float4 data1, float4 data2, float delta = 0);
	void CompareArraysInt4(int4 data1, int4 data2, int delta = 0);
	void CompareArraysUInt4(uint4 data1, uint4 data2, int delta = 0);
	void CompareArraysShort8(short8 data1, short8 data2, int delta = 0);
	void CompareArraysUShort8(ushort8 data1, ushort8 data2, int delta = 0);
	void CompareArraysChar16(char16 data1, char16 data2, int delta = 0);
	void CompareArraysUChar16(uchar16 data1, uchar16 data2, int delta = 0);
	void CompareArraysHalf8(half8 data1, half8, float delta = 0);

	void CompareArraysFloat4x4(float4x4 data1, float4x4 data2, float delta = 0);
	void CompareArraysInt4x4(int4x4 data1, int4x4 data2, int delta = 0);
	void CompareArraysUInt4x4(uint4x4 data1, uint4x4 data2, int delta = 0);
	void CompareArraysLong4x4(long4x4 data1, long4x4 data2, int delta = 0);
	void CompareArraysULong4x4(ulong4x4 data1, ulong4x4 data2, int delta = 0);
	
	void CompareArraysShort8x8(short8x8 data1, short8x8 data2, int delta = 0);
	void CompareArraysUShort8x8(ushort8x8 data1, ushort8x8 data2, int delta = 0);
	void CompareArraysHalf8x8(half8x8 data1, half8x8 data2, int delta = 0);

		
	//compares two arrays of given size; a tolerance value delta can be provided
	void CompareArrays(unsigned char* data1, unsigned char* data2, unsigned int lineSize, unsigned char delta = 0);
	
	//compares two 2d float arrays of given line size and line number; a tolerance value delta can be provided
	void CompareArraysFloat(float** data1, float** data2, unsigned int lineSize, unsigned int lineNo, float delta = 0);
		
	//compares two 2d arrays of given line size and line number; a tolerance value delta can be provided
	void CompareArrays(unsigned char** data1, unsigned char** data2, unsigned int lineSize, unsigned int lineNo, unsigned char delta = 0);

	//compare a slice between startPos and endPos from two arrays
	void CompareArraysRange(unsigned char* data1, unsigned char* data2, unsigned int lineSize,
			unsigned int startPos, unsigned int endPos, unsigned char delta = 0);

	// dump to a file the content of a buffer
	template<class T>
	void DumpArrays(char bufferName[], char fileName[], T** data1, unsigned int lineSize, unsigned int lineNo);
	template<class T>
	void DumpArrays(char bufferName[], char fileName[], T* data1, unsigned int lineSize);
};


template<class T>
void ArrayChecker::ExpectAllEQ(T* data, unsigned int size, T value)
{
	for(unsigned int i = 0; i < size; i++)
	{
		EXPECT_EQ(value, data[i]) << "Position: " << i << "/" << size - 1 << std::endl;
	}
}

template<class T>
void ArrayChecker::ExpectAllInRange(T* data, unsigned int size, T low, T high)
{
	for(unsigned int i = 0; i < size; i++)
	{
		EXPECT_LE(low, data[i]);
		EXPECT_GE(high, data[i]);
	}
}

template<class T>
void ArrayChecker::ExpectAllLT(T* data, unsigned int size, T value)
{
	for(unsigned int i = 0; i < size; i++)
	{
		EXPECT_GT(value, data[i]);
	}
}


template<class T>
void ArrayChecker::ExpectAllLE(T* data, unsigned int size, T value)
{
	for(unsigned int i = 0; i < size; i++)
	{
		EXPECT_GE(value, data[i]);
	}
}


template<class T>
void ArrayChecker::ExpectAllGT(T* data, unsigned int size, T value)
{
	for(unsigned int i = 0; i < size; i++)
	{
		EXPECT_LT(value, data[i]);
	}
}


template<class T>
void ArrayChecker::ExpectAllGE(T* data, unsigned int size, T value)
{
	for(unsigned int i = 0; i < size; i++)
	{
		EXPECT_LE(value, data[i]);
	}
}

template<class T>
void ArrayChecker::CompareArrays(T* data1, T* data2, unsigned int size)
{
	for(unsigned int i = 0; i < size; i++)
	{
		EXPECT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << size - 1
				<< ", Line 1 val: " << (unsigned int)data1[i]
		        << ", Line 2 val: " << (unsigned)data2[i] << std::endl;
	}
}

template<class T>
void ArrayChecker::CompareArrays(T** data1, T** data2, unsigned int lineSize, unsigned int lineNo)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		compareArrays(data1[i], data2[i], lineSize);
	}
}

//
template<class T>
void ArrayChecker::CompareArrays(T* data1, T* data2, unsigned int lineSize, T delta)
{
	printf("\n Compare line: \n");
	for(unsigned int x = 0; x < lineSize; x++)
	{
		//T diff = abs(data1[x] - data2[x]);
		EXPECT_NEAR(data1[x], data2[x], delta) << "Line position: " << x << "/" << lineSize
				<< ", Line 1 val: " << (T)data1[x]
				<< ", Line 2 val: " << (T)data2[x] << std::endl;
	}
}

template<class T>
void ArrayChecker::CompareArrays(T** data1, T** data2, unsigned int lineSize, unsigned int lineNo, T delta)
{
	printf("\n Compare lines: \n");
	for(unsigned int i = 0; i < lineNo; i++)
	{
		for(unsigned int x = 0; x < lineSize; x++)
		{
			T diff = abs(data1[i][x] - data2[i][x]);
			EXPECT_LE(diff, delta) << "Line position: " << x << "/" << lineSize
					<< ", Line 1 val: " << (T)data1[i][x]
					<< ", Line 2 val: " << (T)data2[i][x] << std::endl;
		}
	}
}

template<class T>
void ArrayChecker::DumpArrays(char bufferName[],char fileName[], T** data1, unsigned int lineSize, unsigned int lineNo)
{
	std::ofstream myfile;
	myfile.open (fileName);

	std::string typeName;
	if (typeid(T) == typeid(int)) {
		typeName = "int ";
    }
	if (typeid(T) == typeid(char)) {
		typeName = "char ";
    }
	if (typeid(T) == typeid(unsigned char)) {
		typeName = "unsigned char ";
    }
	if (typeid(T) == typeid(unsigned int)) {
		typeName = "unsigned int ";
    }
	if (typeid(T) == typeid(half)) {
		typeName = "half ";
    }
    if (typeName.empty())
    {
    	std::cout << "Add new suported types in DumpArrays function. \n";
    }

	myfile << typeName << " " << bufferName << "[" << lineNo << "][" << lineSize << "] = {" << std::endl;
	for(unsigned int i = 0; i < lineNo; i++)
	{
		myfile << "    { ";
		for(unsigned int j = 0; j < (lineSize - 1); j++)
		{
			if ((typeid(T) == typeid(char)) || (typeid(T) == typeid(unsigned char)))
			{
				myfile << ((unsigned int)data1[i][j]) << ", ";
			}
			else
			{
				myfile << (data1[i][j]) << ", ";
			}
		}
		if ((typeid(T) == typeid(char)) || (typeid(T) == typeid(unsigned char)))
		{
			myfile << ((unsigned int)data1[i][lineSize - 1]);
		}
		else
		{
			myfile << (data1[i][lineSize - 1]);
		}
		if ((lineNo-1) == i)
		{
			myfile << "}" << std::endl;
		}
		else
		{
			myfile << "}," << std::endl;
		}
	}
	myfile << "};" << std::endl;
}

template<class T>
void ArrayChecker::DumpArrays(char bufferName[],char fileName[], T* data1, unsigned int lineSize)
{
	std::ofstream myfile;
	myfile.open (fileName);

	std::string typeName;
	if (typeid(T) == typeid(int)) {
		typeName = "int ";
    }
	if (typeid(T) == typeid(char)) {
		typeName = "char ";
    }
	if (typeid(T) == typeid(unsigned char)) {
		typeName = "unsigned char ";
    }
	if (typeid(T) == typeid(unsigned int)) {
		typeName = "unsigned int ";
    }
	if (typeid(T) == typeid(half)) {
		typeName = "half ";
    }
	if (typeid(T) == typeid(unsigned short)) {
		typeName = "unsigned short ";
    }	
    if (typeName.empty())
    {
    	std::cout << "Add new suported types in DumpArrays function. \n";
    }

	myfile << typeName << " " << bufferName << "[" << lineSize << "] = {" << std::endl;
	myfile << "    ";
		for(unsigned int j = 0; j < (lineSize - 1); j++)
		{
			if ((typeid(T) == typeid(char)) || (typeid(T) == typeid(unsigned char)))
			{
				myfile << ((unsigned int)data1[j]) << ", ";
			}
			else
			{
				myfile << (data1[j]) << ", ";
			}
		}
		if ((typeid(T) == typeid(char)) || (typeid(T) == typeid(unsigned char)))
		{
			myfile << ((unsigned int)data1[lineSize - 1]);
		}
		else
		{
			myfile << (data1[lineSize - 1]);
		}
	myfile << "};" << std::endl;
}


#endif /* ARRAYCHECKER_H_ */
