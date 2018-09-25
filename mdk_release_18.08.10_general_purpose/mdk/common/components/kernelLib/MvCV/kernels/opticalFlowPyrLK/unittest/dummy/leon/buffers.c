#include <stdio.h>
#include <string.h>

#ifndef PC
#include <swcLeonUtils.h>
#endif

#include <Buffers.h>
#include <Sections.h>
#include <Defines.h>

#define printf(...)

#ifndef PC


extern u8 dummy0_pyrBufs[];

UNIT_TEST void setBuffers()
{

	u32* p = (u32*)&dummy0_pyrBufs;
        #ifdef MYRIAD2
	p[0] = ((u32)inputFrameA);
	p[1] = ((u32)pyrAL1);
	p[2] = ((u32)pyrAL2);
	p[3] = ((u32)pyrAL3);
	p[4] = ((u32)pyrAL4);

	p[5] = ((u32)inputFrameB);
	p[6] = ((u32)pyrBL1);
	p[7] = ((u32)pyrBL2);
	p[8] = ((u32)pyrBL3);
	p[9] = ((u32)pyrBL4);
        #else
	p[0] = swcLeonSwapU32((u32)inputFrameA);
	p[1] = swcLeonSwapU32((u32)pyrAL1);
	p[2] = swcLeonSwapU32((u32)pyrAL2);
	p[3] = swcLeonSwapU32((u32)pyrAL3);
	p[4] = swcLeonSwapU32((u32)pyrAL4);

	p[5] = swcLeonSwapU32((u32)inputFrameB);
	p[6] = swcLeonSwapU32((u32)pyrBL1);
	p[7] = swcLeonSwapU32((u32)pyrBL2);
	p[8] = swcLeonSwapU32((u32)pyrBL3);
	p[9] = swcLeonSwapU32((u32)pyrBL4);
        #endif
}

#else

extern u8 pyrBufs[];

void setBuffers()
{

	uintptr_t* p = (uintptr_t*)pyrBufs;

	p[0] = (uintptr_t)inputFrameA;
	p[1] = (uintptr_t)pyrAL1;
	p[2] = (uintptr_t)pyrAL2;
	p[3] = (uintptr_t)pyrAL3;
	p[4] = (uintptr_t)pyrAL4;

	p[5] = (uintptr_t)inputFrameB;
	p[6] = (uintptr_t)pyrBL1;
	p[7] = (uintptr_t)pyrBL2;
	p[8] = (uintptr_t)pyrBL3;
	p[9] = (uintptr_t)pyrBL4;
}

#endif
