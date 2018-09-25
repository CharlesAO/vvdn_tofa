///
/// @file Helpers.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Helpers Functions
///

#include "Helpers.h"

//----------------------------------------------------------------------------------//
// addPadding - helpers function to add padding to input lines for the MvCV kernels //
//----------------------------------------------------------------------------------//
void addPadding(u8 *pointer, u32 width, u32 paddingSize)
{
    u32 widthAux = width - 1;

    for (u32 i = 1; i <= paddingSize; i++)
    {
        *(pointer - i) = *pointer;
        *(pointer + widthAux + i) = *(pointer + widthAux);
    }
}

//---------------------------------------------------------------------//
// alignLine - helpers function to align each u8 line to multiple of x //
//---------------------------------------------------------------------//
void alignLine(u8 *pointer, u32 width, u32 paddingSize)
{
    for (u32 i = 0; i < paddingSize; i++)
    {
        *(pointer + width - 1 + i) = *(pointer + width -1);
    }
}

//----------------------------------------------------------------------//
// alignLine - helpers function to align each u32 line to multiple of x //
//----------------------------------------------------------------------//
void alignLineU32(u32 *pointer, u32 width, u32 paddingSize)
{
    for (u32 i = 0; i < paddingSize; i++)
    {
        *(pointer + width - 1 + i) = *(pointer + width -1);
    }
}

//---------------------------------------------------------------//
// rotateBuffersOnePosition - helpers function to rotate buffers //
//---------------------------------------------------------------//
void rotateBuffersOnePosition(u8* ptrs[], u32 num)
{
	 // Keep the first pointer because it will be overwritten
	 u8* tmp = ptrs[0];

	 // Shift all pointers to the left location
	 for (u32 i = 0; i < num; i++)
     {
	    ptrs[i] = ptrs[i + 1];
     }
	 // Last pointer becomes the first
     ptrs[5] = tmp;
}

//---------------------------------------------------------------------//
// rotateBuffers - helpers function to rotate buffers with 2 positions //
//---------------------------------------------------------------------//
void rotateBuffers(u8* ptrs[], u32 num)
{
	 // Keep the first pointer because it will be overwritten
	 u8* tmp1 = ptrs[0];
     u8* tmp2 = ptrs[1];

	 // Shift all pointers to the left location
	 for (u32 i = 0; i < num - 2; i += 2)
     {
	    ptrs[i] = ptrs[i + 2];
        ptrs[i + 1] = ptrs[i + 3];
     }
	 // Last pointer becomes the first
     ptrs[2] = ptrs[4];
     ptrs[3] = tmp1;
     ptrs[4] = tmp2;
}
