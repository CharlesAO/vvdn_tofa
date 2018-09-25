///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SampleProfilerExample source code
///

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <svuCommonShave.h>
#include <config.h>

volatile int cnt = 0;

static inline void __attribute__((always_inline)) loopIncrement(volatile int count) {
    for (volatile int x=0; x<count; x++) {
        asm volatile("" : : : "memory");
        cnt++;
    }
}

int __attribute__ ((noinline)) func1(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(1));
    return 0;
}
int __attribute__ ((noinline)) func2(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(2));
    return 0;
}
int __attribute__ ((noinline)) func3(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(3));
    return 0;
}
int __attribute__ ((noinline)) func4(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(4));
    return 0;
}
int __attribute__ ((noinline)) func5(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(5));
    return 0;
}
int __attribute__ ((noinline)) func6(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(6));
    return 0;
}
int __attribute__ ((noinline)) func7(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(7));
    return 0;
}
int __attribute__ ((noinline)) func8(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(8));
    return 0;
}
int __attribute__ ((noinline)) func9(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(9));
    return 0;
}
int __attribute__ ((noinline)) func10(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(10));
    return 0;
}
int __attribute__ ((noinline)) func11(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(11));
    return 0;
}
int __attribute__ ((noinline)) func12(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(12));
    return 0;
}
int __attribute__ ((noinline)) func13(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(13));
    return 0;
}
int __attribute__ ((noinline)) func14(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(14));
    return 0;
}
int __attribute__ ((noinline)) func15(void)
{
    loopIncrement(LOOP_LENGTH_FUNC(15));
    return 0;
}

// Entry point of the shave. This function will start functions with different, predefined durations.
// The profiler will measure the runtime of the functions. Because the function durations are precalculated,
// we can decide if the profiler is working properly.
int profile(void)
{
    printf("Hello from Shave\n");

    for(int i=0; i<10; i++){
        func1();
        func2();
        func3();
        func4();
        func5();
        func6();
        func7();
        func8();
        func9();
        func10();
        func11();
        func12();
        func13();
        func14();
        func15();
    }

    SHAVE_HALT;
    return 0;
}
