#include "gtest/gtest.h"
#include <stdlib.h>
#include <math.h>


//void CompareArrays(unsigned short* data1, unsigned short* data2, unsigned int LineSize, unsigned char delta = 0);

void CompareArrays(u16 *data1, u16 *data2, unsigned int LineSize, u16 delta)
{
  for (unsigned int x = 0; x < LineSize; x++)
  {
    EXPECT_NEAR(data1[x], data2[x], delta) << "Line position: " << x << "/" << LineSize
		<< ", Line 1 val: " << (int)data1[x]
		<< ", Line 2 val: " << (int)data2[x] << std::endl;
  }
} 
