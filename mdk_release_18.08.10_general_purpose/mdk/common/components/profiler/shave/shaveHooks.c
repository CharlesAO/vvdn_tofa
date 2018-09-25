/*
 * shaveHooks.c
 *
 *  Created on: Feb 1, 2016
 */

#ifdef SHAVE_FPROFILE
// not supported #pragma GCC optimize ("O3")

#include "functionProfiler.h"

/// Interesting SVU PC counters (max 4):
/// Stalls (b16-26)
/// Branch taken count (b3 - 8) ????
/// Clock cycle count - replace timer (b2 - 4)
/// Executed instruction count (b1 - 2)
/// Stall cycle count. (b0 - 1) Stall cycles only counted when SVU_OCR[2] is zero (shave running)

// for this, global time sync is needed since there is
// one register for all shaves
// L2C_MISS_CNT, L2C_HIT_CNT

/**
 * TODO: read and send also SVU_PTH - last 16 predication decisions
 *
 */
#define SVU_DCR        0x1800		// Debug Control Register
#define SVU_PC0        0x1830		// Performance Counter 0
#define SVU_PCC0       0x1834		// Performance Counter 0 Control Register
#define SVU_PC1        0x1838		// Performance Counter 1
#define SVU_PCC1       0x183c		// Performance Counter 1 Control Register
#define SVU_PC2        0x1844		// Performance Counter 2
#define SVU_PCC2       0x1848		// Performance Counter 2 Control Register
#define SVU_PC3        0x184c		// Performance Counter 3
#define SVU_PCC3       0x1850		// Performance Counter 3 Control Register

// TODO: write data to profile buffer may introduce further stalls. Minimize the impact!

/*static*/ void
__attribute__((no_instrument_function))
__attribute__((nothrow))
__attribute__((noinline))
__shvProfilerInit(void) {

#ifdef FP_SHV_INIT_HOOK
    FP_SHV_INIT_HOOK
#endif

#if defined(FP_ENABLE_COUNTER0) || defined(FP_ENABLE_COUNTER1) \
        || defined(FP_ENABLE_COUNTER2) || defined(FP_ENABLE_COUNTER3)
    // init performance counter registers
    SET_REG_WORD(SVU_DCR, 0x1); // debug mode

#if defined(FP_ENABLE_COUNTER0)
    SET_REG_WORD(SVU_PCC0, FP_ENABLE_COUNTER0); // Usually clock cycle count (0x4)
    SET_REG_WORD(SVU_PC0, 0); // reset
#endif
#if defined(FP_ENABLE_COUNTER1)
    SET_REG_WORD(SVU_PCC1, FP_ENABLE_COUNTER1);
    SET_REG_WORD(SVU_PC1, 0); // reset
#endif
#if defined(FP_ENABLE_COUNTER2)
    SET_REG_WORD(SVU_PCC2, FP_ENABLE_COUNTER2);
    SET_REG_WORD(SVU_PC2, 0); // reset
#endif
#if defined(FP_ENABLE_COUNTER3)
    SET_REG_WORD(SVU_PCC3, FP_ENABLE_COUNTER3);
    SET_REG_WORD(SVU_PC3, 0); // reset
#endif
#endif // if any performance counter defined
}

// FIXME: current workaround for constructors list not being called on all SHAVE programming paradigms
// TODO: remove this when converging on SHAVE proper initialisation support
void _preRunInit();
void (*preRunInit)() = _preRunInit;
__attribute__((no_instrument_function)) void null() {}

__attribute__((no_instrument_function))
__attribute__((constructor))
void _preRunInit() {
    preRunInit = null; // don't come back
    __functionProfilerInit();
    __shvProfilerInit();
    __computeOverhead();
    enableProfiler();
}

/// hook called at each function entry
void
__attribute__((no_instrument_function))
__attribute__((nothrow))
// not yet supported by the compiler __attribute__((optimize(3)))
__cyg_profile_func_enter (void *func, void *caller __attribute__((unused)))
{
#ifndef FP_SHV_CTORS_CALLED
    preRunInit();
#endif
    IN(func);

    // call UDF hook (if defined)
#ifdef FP_SHV_FCT_ENTER_HOOK
    FP_SHV_FCT_ENTER_HOOK
#endif

    // by design, PC0 is configured for clock cycle, thus this is the last read
    // on function entry and the first on function exit
    // on future release, PC0 will replace the timer.
#if defined(FP_ENABLE_COUNTER3)
    PUSH_MEM(SVU_PC3, GET_REG_WORD_VAL(SVU_PC3));
#endif
#if defined(FP_ENABLE_COUNTER2)
    PUSH_MEM(SVU_PC2, GET_REG_WORD_VAL(SVU_PC2));
#endif
#if defined(FP_ENABLE_COUNTER1)
    PUSH_MEM(SVU_PC1, GET_REG_WORD_VAL(SVU_PC1));
#endif
#if defined(FP_ENABLE_COUNTER0)
    PUSH_MEM(SVU_PC0, GET_REG_WORD_VAL(SVU_PC0));
#endif

}

void
__attribute__((no_instrument_function))
__attribute__((nothrow))
// not yet supported by the  compiler __attribute__((optimize(3)))
__cyg_profile_func_exit (void *func, void *caller __attribute__((unused)))
{
#if defined(FP_ENABLE_COUNTER0)
    PUSH_MEM(SVU_PC0, GET_REG_WORD_VAL(SVU_PC0));
#endif
#if defined(FP_ENABLE_COUNTER1)
    PUSH_MEM(SVU_PC1, GET_REG_WORD_VAL(SVU_PC1));
#endif
#if defined(FP_ENABLE_COUNTER2)
    PUSH_MEM(SVU_PC2, GET_REG_WORD_VAL(SVU_PC2));
#endif
#if defined(FP_ENABLE_COUNTER3)
    PUSH_MEM(SVU_PC3, GET_REG_WORD_VAL(SVU_PC3));
#endif

    // call UDF hook
#ifdef FP_SHV_FCT_EXIT_HOOK
    FP_SHV_FCT_EXIT_HOOK
#endif

    OUT(func);
}
#endif
