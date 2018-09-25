// No ifdef guards because this header is intended to be included in multiple Shave source files

// 1: Includes
// ----------------------------------------------------------------------------
#include "kernel_config.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
static u8 in7LinesBuffer1[8][FULLWIDTH]     __attribute__ ((aligned (16)));
static u8 in7LinesBuffer2[8][FULLWIDTH]     __attribute__ ((aligned (16)));
static u8 in7LinesBuffer3[8][FULLWIDTH]     __attribute__ ((aligned (16)));

static u8 outLinesBuffer[2][FULLWIDTH]      __attribute__ ((aligned (16)));

static u8* in7LinesAddress1[LINES_NUM];
static u8* in7LinesAddress2[LINES_NUM];
static u8* in7LinesAddress3[LINES_NUM];

static u8* outLinesAddress[2];

static u16 mask7[7 * 7] __attribute__ ((aligned (16))) = {
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x2C00, 0x3000, 0x2C00, 0x0000, 0x0000,
    0x0000, 0x0000, 0x3000, 0x3400, 0x3000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x2C00, 0x3000, 0x2C00, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

static void initPointers(u8* inPointers[], u8 inLinesBuff[][FULLWIDTH], u32 nrLines, u32 padding)
{
    for (u32 i = 0; i < nrLines; i++) 
    {
       inPointers[i] = (u8*)(((u32)(inLinesBuff[i] + padding)) & ~0xF);
    }
}

static void rotatePointers(u8* pointers[], u32 nrLines)
{
    u8* internalPointer  = pointers[0];
    
    for (u32 i = 1; i < nrLines; i++)
        pointers[i - 1] = pointers[i];

    pointers[nrLines - 1] = internalPointer;
}
