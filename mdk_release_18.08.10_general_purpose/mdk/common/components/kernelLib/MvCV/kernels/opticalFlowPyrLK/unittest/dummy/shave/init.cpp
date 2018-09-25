#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/shave_system.h>
#include <mvcv.h>

#include "../shared/Defines.h"

#ifndef PC
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

u32 init = 0;

static u8 heap[HEAP_SIZE];

// Input/output buffers used by the algorithm.
// Need to be accessible by the C++ wrapper.
u8* pyrBufs[][MAX_PYRAMID_LEVELS + 1] =
{
    { NULL, NULL, NULL, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL },
};

mvPoint2f cornersA[MAX_FEATURES];
mvPoint2f cornersB[MAX_FEATURES];
u8 cornersStatus[MAX_FEATURES];
fp32 cornersError[MAX_FEATURES];

mvCvSize searchWinSz;
u32 width;
u32 height;
u32 pyrLevels;
mvCvTermCriteria stopCrit;
fp32 epsilon = STOP_CRIT_EPS;
u32 maxIterations = STOP_CRIT_MAX_ITER;
u32 flags = 0;
u32 pointsCount;

Mat pyrA[MAX_PYRAMID_LEVELS + 1];
Mat pyrB[MAX_PYRAMID_LEVELS + 1];

EXTERN void Run();

EXTERN void Init()
{
    if (init == 0)
    {
        assert((width > 0) && (width <= MAX_FRAME_WIDTH));
        assert((height > 0) && (height <= MAX_FRAME_WIDTH));

        // Initialize heap because the algorithm uses malloc
//        init_heap(heap, HEAP_SIZE);
//        clear_heap();
    __setheap(heap, HEAP_SIZE);

        // Setup algorithm input parameters
        stopCrit = mvcvTermCriteria(MVCV_TERMCRIT_ITER | MVCV_TERMCRIT_EPS, 9, 0.01f);

        // Initialize pyramid levels
        for (u32 i = 0; i < pyrLevels + 1; i++)
        {
//            printf("%x %x\n", pyrBufs[0][i], pyrBufs[1][i]);

            Mat ma(height / pow(2, i), width / pow(2, i), CV_8U, (u8*)pyrBufs[0][i], false,
                width / pow(2, i) + searchWinSz.width * 2);
            Mat mb(height / pow(2, i), width / pow(2, i), CV_8U, (u8*)pyrBufs[1][i], false,
                width / pow(2, i) + searchWinSz.width * 2);

            pyrA[i] = ma;
            pyrB[i] = mb;
        }
    }
    else
    {
        Run();
    }

    exit(0);
}

EXTERN void Run()
{
    // Run the point matching algorithm
    mvcvCalcOpticalFlowPyrLK_tuned(pyrA, pyrB, cornersA, cornersB, cornersStatus,
        cornersError, searchWinSz, pyrLevels, stopCrit, flags, pointsCount);

    exit(0);
}

#ifndef PC

EXTERN int main()
{
    // Dummy, just to respect the kernel unit test convention of having a main
    Init();
    Run();
    return 0;
}

// A call to this function is generated if there are any global objects
// and since it is not implemented in moviCompile's libraries, it needs
// to be done upon need.
EXTERN int atexit(void (*func)(void))
{
    func();
    return 0;
}

#endif
