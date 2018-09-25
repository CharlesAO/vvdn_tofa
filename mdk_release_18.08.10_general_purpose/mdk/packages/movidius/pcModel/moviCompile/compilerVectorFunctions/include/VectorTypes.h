#ifndef __MV_VECTOR_TYPES__
#define __MV_VECTOR_TYPES__

#ifdef __MOVICOMPILE__

#ifdef __OLD_MOVICOMPILE__
// This was defined only in mvision project to keep compatibility with pre-vectorization era
// moviCompile 50.20
#include <moviVectorTypes.h>
#else
#include <moviVectorTypes.h>
#endif

#else // PC vectorization mapping of supported Shave operations

#include <mv_types.h>
#include "assert.h"
#include <half.h>

// Class for vectors with 4 elements
template<typename T, unsigned int L>
class Vec
{
private:
	T s[L];

public:
	// Constructors ---------------------------------------------------

	// Subscripting operators -----------------------------------------
	inline T& operator [] (const int i)
	{
		assert(i >= 0 && i < (int) L);

		return s[i];
	}

	inline T operator [] (const int i) const
	{
		assert(i >= 0 && i < (int) L);

		return s[i];
	}

	// Binary vector operators ------------------------------------------
	inline Vec operator + (const Vec& v) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] + v[i];

		return res;
	}

	inline Vec operator - (const Vec& v) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] - v[i];

		return res;
	}

	inline Vec operator * (const Vec& v) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] * v[i];

		return res;
	}

	inline Vec operator / (const Vec& v) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] / v[i];

		return res;
	}

	// Binary scalar operators ------------------------------------------
	inline Vec operator + (const T scalar) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] + scalar;

		return res;
	}

	inline Vec operator - (const T scalar) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] - scalar;

		return res;
	}

	inline Vec operator * (const T scalar) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] * scalar;

		return res;
	}

	inline Vec operator / (const T scalar) const
	{
		Vec res;

		for (u32 i = 0; i < L; i++)
			res[i] = s[i] / scalar;

		return res;
	}
};

template<typename T>
class Mat4x4
{
public:
	T rows[4][4];
};

template<typename T>
class Mat8x8
{
public:
	T rows[8][8];
};

// Cardinal 2 vectors
typedef Vec<s32, 2>		int2;
typedef Vec<u32, 2>		uint2;
typedef Vec<s16, 2>		short2;
typedef Vec<u16, 2>		ushort2;
typedef Vec<s8, 2>		char2;
typedef Vec<u8, 2>		uchar2;
typedef Vec<fp32, 2>	float2;
typedef Vec<half, 2>	half2;

// Cardinal 3 vectors
typedef Vec<s32, 3>		int3;
typedef Vec<u32, 3>		uint3;
typedef Vec<s16, 3>		short3;
typedef Vec<u16, 3>		ushort3;
typedef Vec<s8, 3>		char3;
typedef Vec<u8, 3>		uchar3;
typedef Vec<fp32, 3>	float3;
typedef Vec<half, 3>	half3;

// Cardinal 4 vectors
typedef Vec<s32, 4>		long4;
typedef Vec<u32, 4>		ulong4;
typedef Vec<s32, 4>		int4;
typedef Vec<u32, 4>		uint4;
typedef Vec<s16, 4>		short4;
typedef Vec<u16, 4>		ushort4;
typedef Vec<s8, 4>		char4;
typedef Vec<u8, 4>		uchar4;
typedef Vec<fp32, 4>	float4;
typedef Vec<half, 4>	half4;

// Cardinal 8 vectors
typedef Vec<s32, 8>		int8;
typedef Vec<u32, 8>		uint8;
typedef Vec<s16, 8>		short8;
typedef Vec<u16, 8>		ushort8;
typedef Vec<s8, 8>		char8;
typedef Vec<u8, 8>		uchar8;
typedef Vec<fp32, 8>	float8;
typedef Vec<half, 8>	half8;

// Cardinal 16 vectors
typedef Vec<s32, 16>	int16;
typedef Vec<u32, 16>	uint16;
typedef Vec<s16, 16>	short16;
typedef Vec<u16, 16>	ushort16;
typedef Vec<s8, 16>		char16;
typedef Vec<u8, 16>		uchar16;
typedef Vec<half, 16>	half16;


// Native matrix data types - 128bit vectors as rows
typedef Mat4x4<fp32>	float4x4;
typedef Mat4x4<s32>		int4x4;
typedef Mat4x4<u32>		uint4x4;
typedef Mat4x4<s64>		long4x4;
typedef Mat4x4<u64> 	ulong4x4;

typedef Mat8x8<s16>		short8x8;
typedef Mat8x8<u16>		ushort8x8;
typedef Mat8x8<half>	half8x8;

#endif

#endif
