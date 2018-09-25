#include "ArrayChecker.h"

ArrayChecker::ArrayChecker() {
	// TODO Auto-generated constructor stub

}

ArrayChecker::~ArrayChecker() {
	// TODO Auto-generated destructor stub
}

void ArrayChecker::CompareArraysFloat(float* data1, float* data2,
		unsigned int lineSize, float delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < lineSize; i++)
		{
		EXPECT_FLOAT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << lineSize
				<< ", Line 1 val: " << (float)data1[i]
				<< ", Line 2 val: " << (float)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < lineSize; i++)
		{
			//float diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << lineSize
				<< ", Line 1 val: " << (float)data1[i]
				<< ", Line 2 val: " << (float)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysFloat4(float4 data1, float4 data2, float delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
		{
		EXPECT_FLOAT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 4
				<< ", Line 1 val: " << (float)data1[i]
				<< ", Line 2 val: " << (float)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 4; i++)
		{
			//float diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 4
				<< ", Line 1 val: " << (float)data1[i]
				<< ", Line 2 val: " << (float)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysInt4(int4 data1, int4 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
		{
		EXPECT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 4
				<< ", Line 1 val: " << (int)data1[i]
				<< ", Line 2 val: " << (int)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 4; i++)
		{
			//int diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 4
				<< ", Line 1 val: " << (int)data1[i]
				<< ", Line 2 val: " << (int)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysUInt4(uint4 data1, uint4 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
		{
		EXPECT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 4
				<< ", Line 1 val: " << (unsigned int)data1[i]
				<< ", Line 2 val: " << (unsigned int)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 4; i++)
		{
			//int diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 4
				<< ", Line 1 val: " << (unsigned int)data1[i]
				<< ", Line 2 val: " << (unsigned int)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysShort8(short8 data1, short8 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 8; i++)
		{
		EXPECT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 8
				<< ", Line 1 val: " << (short)data1[i]
				<< ", Line 2 val: " << (short)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 8; i++)
		{
			//int diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 8
				<< ", Line 1 val: " << (short)data1[i]
				<< ", Line 2 val: " << (short)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysUShort8(ushort8 data1, ushort8 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 8; i++)
		{
		EXPECT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 8
				<< ", Line 1 val: " << (unsigned short)data1[i]
				<< ", Line 2 val: " << (unsigned short)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 8; i++)
		{
			//int diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 8
				<< ", Line 1 val: " << (unsigned short)data1[i]
				<< ", Line 2 val: " << (unsigned short)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysChar16(char16 data1, char16 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 16; i++)
		{
		EXPECT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 16
				<< ", Line 1 val: " << (char)data1[i]
				<< ", Line 2 val: " << (char)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 16; i++)
		{
			//int diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 16
				<< ", Line 1 val: " << (char)data1[i]
				<< ", Line 2 val: " << (char)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysUChar16(uchar16 data1, uchar16 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 16; i++)
		{
		EXPECT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 16
				<< ", Line 1 val: " << (unsigned char)data1[i]
				<< ", Line 2 val: " << (unsigned char)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 16; i++)
		{
			//int diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 16
				<< ", Line 1 val: " << (unsigned char)data1[i]
				<< ", Line 2 val: " << (unsigned char)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysHalf8(half8 data1, half8 data2, float delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 8; i++)
		{
		EXPECT_FLOAT_EQ(data1[i], data2[i]) << "Line position: " << i << "/" << 8
				<< ", Line 1 val: " << (half)data1[i]
				<< ", Line 2 val: " << (half)data2[i] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 8; i++)
		{
			//int diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1[i], data2[i], delta) << "Line position: " << i << "/" << 8
				<< ", Line 1 val: " << (half)data1[i]
				<< ", Line 2 val: " << (half)data2[i] << std::endl;
		}
}

void ArrayChecker::CompareArraysFloat4x4(float4x4 data1, float4x4 data2, float delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
		for(unsigned int j = 0; j < 4; j++)
		{
		EXPECT_FLOAT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*4+j << "/" << 16
				<< ", Line 1 val: " << (float)data1.rows[i][j]
				<< ", Line 2 val: " << (float)data2.rows[i][j] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 4; i++)
		for(unsigned int j = 0; j < 4; j++)
		{
			//float diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*4+j << "/" << 16
				<< ", Line 1 val: " << (float)data1.rows[i][j]
				<< ", Line 2 val: " << (float)data2.rows[i][j] << std::endl;
		}
}

void ArrayChecker::CompareArraysInt4x4(int4x4 data1, int4x4 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
		for(unsigned int j = 0; j < 4; j++)
		{
			EXPECT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*4+j << "/" << 16
				<< ", Line 1 val: " << (int)data1.rows[i][j]
				<< ", Line 2 val: " << (int)data2.rows[i][j] << std::endl;
		}
	else
		for(unsigned int i = 0; i < 4; i++)
		for(unsigned int j = 0; j < 4; j++)
		{
			//float diff = abs(data1[i] - data2[i]);
			EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*4+j << "/" << 16
				<< ", Line 1 val: " << (int)data1.rows[i][j]
				<< ", Line 2 val: " << (int)data2.rows[i][j] << std::endl;
		}
}

void ArrayChecker::CompareArraysUInt4x4(uint4x4 data1, uint4x4 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
			for(unsigned int j = 0; j < 4; j++)
			{
				EXPECT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*4+j << "/" << 16
					<< ", Line 1 val: " << (unsigned int)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned int)data2.rows[i][j] << std::endl;
			}
	else
		for(unsigned int i = 0; i < 4; i++)
			for(unsigned int j = 0; j < 4; j++)
			{
				//float diff = abs(data1[i] - data2[i]);
				EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*4+j << "/" << 16
					<< ", Line 1 val: " << (unsigned int)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned int)data2.rows[i][j] << std::endl;
			}
}

void ArrayChecker::CompareArraysLong4x4(long4x4 data1, long4x4 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
			for(unsigned int j = 0; j < 4; j++)
			{
				EXPECT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*4+j << "/" << 16
					<< ", Line 1 val: " << (unsigned int)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned int)data2.rows[i][j] << std::endl;
			}
	else
		for(unsigned int i = 0; i < 4; i++)
			for(unsigned int j = 0; j < 4; j++)
			{
				//float diff = abs(data1[i] - data2[i]);
				EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*4+j << "/" << 16
					<< ", Line 1 val: " << (unsigned int)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned int)data2.rows[i][j] << std::endl;
			}
}

void ArrayChecker::CompareArraysULong4x4(ulong4x4 data1, ulong4x4 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 4; i++)
			for(unsigned int j = 0; j < 4; j++)
			{
				EXPECT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*4+j << "/" << 16
					<< ", Line 1 val: " << (unsigned int)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned int)data2.rows[i][j] << std::endl;
			}
	else
		for(unsigned int i = 0; i < 4; i++)
			for(unsigned int j = 0; j < 4; j++)
			{
				//float diff = abs(data1[i] - data2[i]);
				EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*4+j << "/" << 16
					<< ", Line 1 val: " << (unsigned int)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned int)data2.rows[i][j] << std::endl;
			}
}

void ArrayChecker::CompareArraysShort8x8(short8x8 data1, short8x8 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 8; i++)
			for(unsigned int j = 0; j < 8; j++)
			{
				EXPECT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*8+j << "/" << 64
					<< ", Line 1 val: " << (short)data1.rows[i][j]
					<< ", Line 2 val: " << (short)data2.rows[i][j] << std::endl;
			}
	else
		for(unsigned int i = 0; i < 8; i++)
			for(unsigned int j = 0; j < 8; j++)
			{
				//float diff = abs(data1[i] - data2[i]);
				EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*8+j << "/" << 64
					<< ", Line 1 val: " << (short)data1.rows[i][j]
					<< ", Line 2 val: " << (short)data2.rows[i][j] << std::endl;
			}
}

void ArrayChecker::CompareArraysUShort8x8(ushort8x8 data1, ushort8x8 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 8; i++)
			for(unsigned int j = 0; j < 8; j++)
			{
				EXPECT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*8+j << "/" << 64
					<< ", Line 1 val: " << (unsigned short)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned short)data2.rows[i][j] << std::endl;
			}
	else
		for(unsigned int i = 0; i < 8; i++)
			for(unsigned int j = 0; j < 8; j++)
			{
				//float diff = abs(data1[i] - data2[i]);
				EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*8+j << "/" << 64
					<< ", Line 1 val: " << (unsigned short)data1.rows[i][j]
					<< ", Line 2 val: " << (unsigned short)data2.rows[i][j] << std::endl;
			}
}

void ArrayChecker::CompareArraysHalf8x8(half8x8 data1, half8x8 data2, int delta)
{
	if( delta == 0)
		// by default we consider delta to be 4 ULPS (Units in the Last Place)
		for(unsigned int i = 0; i < 8; i++)
			for(unsigned int j = 0; j < 8; j++)
			{
				EXPECT_EQ(data1.rows[i][j], data2.rows[i][j]) << "Line position: " << i*8+j << "/" << 64
					<< ", Line 1 val: " << (half)data1.rows[i][j]
					<< ", Line 2 val: " << (half)data2.rows[i][j] << std::endl;
			}
	else
		for(unsigned int i = 0; i < 8; i++)
			for(unsigned int j = 0; j < 8; j++)
			{
				//float diff = abs(data1[i] - data2[i]);
				EXPECT_NEAR(data1.rows[i][j], data2.rows[i][j], delta) << "Line position: " << i*8+j << "/" << 64
					<< ", Line 1 val: " << (half)data1.rows[i][j]
					<< ", Line 2 val: " << (half)data2.rows[i][j] << std::endl;
			}
}

void ArrayChecker::CompareArraysFloat(float** data1, float** data2,
		unsigned int lineSize, unsigned int lineNo, float delta)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		CompareArraysFloat(data1[i], data2[i], lineSize, delta);
	}
}


void ArrayChecker::CompareArrays(unsigned char* data1, unsigned char* data2,
		unsigned int lineSize, unsigned char delta)
{
	for(unsigned int i = 0; i < lineSize; i++)
	{
		unsigned int diff = abs(data1[i] - data2[i]);
		EXPECT_LE(diff, delta) << "Line position: " << i << "/" << lineSize
				<< ", Line 1 val: " << (unsigned int)data1[i]
				<< ", Line 2 val: " << (unsigned int)data2[i] << std::endl;
	}
}

void ArrayChecker::CompareArrays(unsigned char** data1, unsigned char** data2,
		unsigned int lineSize, unsigned int lineNo, unsigned char delta)
{
	for(unsigned int i = 0; i < lineNo; i++)
	{
		CompareArrays(data1[i], data2[i], lineSize, delta);
	}
}

void ArrayChecker::CompareArraysRange(unsigned char* data1, unsigned char* data2,
		unsigned int lineSize, unsigned int startPos, unsigned int endPos,
		unsigned char delta)
{
	if(startPos > lineSize)
	{
		startPos = 0;
	}
	if(endPos > lineSize)
	{
		endPos = lineSize;
	}

	CompareArrays(&data1[startPos], &data2[startPos], endPos - startPos, delta);
}
