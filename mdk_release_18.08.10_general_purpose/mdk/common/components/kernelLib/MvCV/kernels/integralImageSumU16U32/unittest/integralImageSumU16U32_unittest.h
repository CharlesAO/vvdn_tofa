#include "gtest/gtest.h"
#include <stdlib.h>
#include <math.h>


//void CompareArrays(unsigned short* data1, unsigned short* data2, unsigned int LineSize, unsigned char delta = 0);

template<class T>
void CompareArrays(T* data1, T* data2, unsigned int LineSize)
{
     
        for (unsigned int x = 0; x < LineSize; x++)
        {
			//T diff = abs(data1[x] - data2[x]);
         	EXPECT_EQ(data1[x], data2[x]) << "Line position: " << x << "/" << LineSize
				<< ", Line 1 val: " << (T)data1[x]
				<< ", Line 2 val: " << (T)data2[x] << std::endl;
        }
} 

