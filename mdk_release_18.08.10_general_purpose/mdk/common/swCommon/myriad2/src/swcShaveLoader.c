/**
* This is simple module to manage shave loading and windowed addresses
 *
 * @File
 * @Brief     Simple Shave loader and window manager
 * @copyright All code copyright Movidius Ltd 2012, all rights reserved
 *            For License Warranty see: common/license.txt
 *
 * @TODO(Cormac):  Some svu support stuff is duplicated here which should find a new home
 */

#include <swcMutexUsage.h>
#include "swcShaveLoader.h"
#include "swcShaveLoaderPrivate.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <theDynContext.h>
#include "swcMemoryTransfer.h"
#include <swcLeonUtils.h>
#include "registersMyriad.h"
#include "DrvSvu.h"
#include "assert.h"
#include "mv_types.h"
#include <DrvIcb.h>
#include <stdarg.h>
#include <dbgTracerApi.h>
#include <DrvCpr.h>
#include "DrvMutex.h"
#include "mvMacros.h"
#include <DrvCommon.h>

#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
#include "MyriadMemInit.h"
#endif

#include "ShaveData.h"


#define SHAVE_ASSIGNED 1
#define SHAVE_NOT_ASSIGNED 0
#define INVALID_SHAVE 255

#ifndef MAX_APPLICATION_NAME
#define MAX_APPLICATION_NAME 260
#endif

// The following external constants are defined by the application linker script.
// The address of each symbol corresponds to the default window register base address for the respective shave window
// It is possible to override these defaults by creating symbols of the same name in your custom.ldscript
// These symbols are only used when the function swcSetShaveWindowsToDefault() is called
extern u32* const __WinRegShave0_winC,  __WinRegShave0_winD,  __WinRegShave0_winE,  __WinRegShave0_winF;
extern u32* const __WinRegShave1_winC,  __WinRegShave1_winD,  __WinRegShave1_winE,  __WinRegShave1_winF;
extern u32* const __WinRegShave2_winC,  __WinRegShave2_winD,  __WinRegShave2_winE,  __WinRegShave2_winF;
extern u32* const __WinRegShave3_winC,  __WinRegShave3_winD,  __WinRegShave3_winE,  __WinRegShave3_winF;
extern u32* const __WinRegShave4_winC,  __WinRegShave4_winD,  __WinRegShave4_winE,  __WinRegShave4_winF;
extern u32* const __WinRegShave5_winC,  __WinRegShave5_winD,  __WinRegShave5_winE,  __WinRegShave5_winF;
extern u32* const __WinRegShave6_winC,  __WinRegShave6_winD,  __WinRegShave6_winE,  __WinRegShave6_winF;
extern u32* const __WinRegShave7_winC,  __WinRegShave7_winD,  __WinRegShave7_winE,  __WinRegShave7_winF;
extern u32* const __WinRegShave8_winC,  __WinRegShave8_winD,  __WinRegShave8_winE,  __WinRegShave8_winF;
extern u32* const __WinRegShave9_winC,  __WinRegShave9_winD,  __WinRegShave9_winE,  __WinRegShave9_winF;
extern u32* const __WinRegShave10_winC, __WinRegShave10_winD, __WinRegShave10_winE, __WinRegShave10_winF;
extern u32* const __WinRegShave11_winC, __WinRegShave11_winD, __WinRegShave11_winE, __WinRegShave11_winF;
#if (defined(MA2480) || defined(MA2485))
extern u32* const __WinRegShave12_winC, __WinRegShave12_winD, __WinRegShave12_winE, __WinRegShave12_winF;
extern u32* const __WinRegShave13_winC, __WinRegShave13_winD, __WinRegShave13_winE, __WinRegShave13_winF;
extern u32* const __WinRegShave14_winC, __WinRegShave14_winD, __WinRegShave14_winE, __WinRegShave14_winF;
extern u32* const __WinRegShave15_winC, __WinRegShave15_winD, __WinRegShave15_winE, __WinRegShave15_winF;
#endif

const u32 shaveWindows[][4] =
{
    { (u32)&__WinRegShave0_winC,  (u32)&__WinRegShave0_winD,  (u32)&__WinRegShave0_winE,  (u32)&__WinRegShave0_winF },
    { (u32)&__WinRegShave1_winC,  (u32)&__WinRegShave1_winD,  (u32)&__WinRegShave1_winE,  (u32)&__WinRegShave1_winF },
    { (u32)&__WinRegShave2_winC,  (u32)&__WinRegShave2_winD,  (u32)&__WinRegShave2_winE,  (u32)&__WinRegShave2_winF },
    { (u32)&__WinRegShave3_winC,  (u32)&__WinRegShave3_winD,  (u32)&__WinRegShave3_winE,  (u32)&__WinRegShave3_winF },
    { (u32)&__WinRegShave4_winC,  (u32)&__WinRegShave4_winD,  (u32)&__WinRegShave4_winE,  (u32)&__WinRegShave4_winF },
    { (u32)&__WinRegShave5_winC,  (u32)&__WinRegShave5_winD,  (u32)&__WinRegShave5_winE,  (u32)&__WinRegShave5_winF },
    { (u32)&__WinRegShave6_winC,  (u32)&__WinRegShave6_winD,  (u32)&__WinRegShave6_winE,  (u32)&__WinRegShave6_winF },
    { (u32)&__WinRegShave7_winC,  (u32)&__WinRegShave7_winD,  (u32)&__WinRegShave7_winE,  (u32)&__WinRegShave7_winF },
    { (u32)&__WinRegShave8_winC,  (u32)&__WinRegShave8_winD,  (u32)&__WinRegShave8_winE,  (u32)&__WinRegShave8_winF },
    { (u32)&__WinRegShave9_winC,  (u32)&__WinRegShave9_winD,  (u32)&__WinRegShave9_winE,  (u32)&__WinRegShave9_winF },
    { (u32)&__WinRegShave10_winC, (u32)&__WinRegShave10_winD, (u32)&__WinRegShave10_winE, (u32)&__WinRegShave10_winF },
    { (u32)&__WinRegShave11_winC, (u32)&__WinRegShave11_winD, (u32)&__WinRegShave11_winE, (u32)&__WinRegShave11_winF },
#if (defined(MA2480) || defined(MA2485) || defined(MA2485))
    { (u32)&__WinRegShave12_winC, (u32)&__WinRegShave12_winD, (u32)&__WinRegShave12_winE, (u32)&__WinRegShave12_winF },
    { (u32)&__WinRegShave13_winC, (u32)&__WinRegShave13_winD, (u32)&__WinRegShave13_winE, (u32)&__WinRegShave13_winF },
    { (u32)&__WinRegShave14_winC, (u32)&__WinRegShave14_winD, (u32)&__WinRegShave14_winE, (u32)&__WinRegShave14_winF },
    { (u32)&__WinRegShave15_winC, (u32)&__WinRegShave15_winD, (u32)&__WinRegShave15_winE, (u32)&__WinRegShave15_winF }
#endif
};

// The following external constants are defined by the application linker script.
// The address of each symbol corresponds to the default Shave stack pointer for the respective shave.
// It is possible to override these defaults by creating symbols of the same name in your custom.ldscript
// These symbols are only used when the function swcSetAbsoluteDefaultStack() is called
extern void* const __SVE0_STACK_POINTER ;
extern void* const __SVE1_STACK_POINTER ;
extern void* const __SVE2_STACK_POINTER ;
extern void* const __SVE3_STACK_POINTER ;
extern void* const __SVE4_STACK_POINTER ;
extern void* const __SVE5_STACK_POINTER ;
extern void* const __SVE6_STACK_POINTER ;
extern void* const __SVE7_STACK_POINTER ;
extern void* const __SVE8_STACK_POINTER ;
extern void* const __SVE9_STACK_POINTER ;
extern void* const __SVE10_STACK_POINTER;
extern void* const __SVE11_STACK_POINTER;
#if (defined(MA2480) || defined(MA2485) || defined(MA2485))
extern void* const __SVE12_STACK_POINTER;
extern void* const __SVE13_STACK_POINTER;
extern void* const __SVE14_STACK_POINTER;
extern void* const __SVE15_STACK_POINTER;
#endif

const u32 shaveStackPtrs[] =
{
    (u32)&__SVE0_STACK_POINTER, (u32)&__SVE1_STACK_POINTER, (u32)&__SVE2_STACK_POINTER,  (u32)&__SVE3_STACK_POINTER,
    (u32)&__SVE4_STACK_POINTER, (u32)&__SVE5_STACK_POINTER, (u32)&__SVE6_STACK_POINTER,  (u32)&__SVE7_STACK_POINTER,
    (u32)&__SVE8_STACK_POINTER, (u32)&__SVE9_STACK_POINTER, (u32)&__SVE10_STACK_POINTER, (u32)&__SVE11_STACK_POINTER
#if (defined(MA2480) || defined(MA2485) || defined(MA2485))
    ,(u32)&__SVE12_STACK_POINTER,(u32)&__SVE13_STACK_POINTER,(u32)&__SVE14_STACK_POINTER,(u32)&__SVE15_STACK_POINTER
#endif
};

// The following external constants are defined by the application linker script.
// The address of each symbol corresponds to the last data in the stack for the respective shave.
// It is possible to override these defaults by creating symbols of the same name in your custom.ldscript
// These symbols are only used when the function swcSetAbsoluteDefaultStack() is called
extern  unsigned char  __shave0_cmx_data_end[];
extern  unsigned char  __shave1_cmx_data_end[];
extern  unsigned char  __shave2_cmx_data_end[];
extern  unsigned char  __shave3_cmx_data_end[];
extern  unsigned char  __shave4_cmx_data_end[];
extern  unsigned char  __shave5_cmx_data_end[];
extern  unsigned char  __shave6_cmx_data_end[];
extern  unsigned char  __shave7_cmx_data_end[];
extern  unsigned char  __shave8_cmx_data_end[];
extern  unsigned char  __shave9_cmx_data_end[];
extern  unsigned char  __shave10_cmx_data_end[];
extern  unsigned char  __shave11_cmx_data_end[];
#if (defined(MA2480) || defined(MA2485) || defined(MA2485))
extern  unsigned char  __shave12_cmx_data_end[];
extern  unsigned char  __shave13_cmx_data_end[];
extern  unsigned char  __shave14_cmx_data_end[];
extern  unsigned char  __shave15_cmx_data_end[];
#endif

const u32 shaveLastDataAdd[] =
{
    (u32)&__shave0_cmx_data_end, (u32)&__shave1_cmx_data_end, (u32)&__shave2_cmx_data_end, (u32)&__shave3_cmx_data_end,
    (u32)&__shave4_cmx_data_end, (u32)&__shave5_cmx_data_end, (u32)&__shave6_cmx_data_end, (u32)&__shave7_cmx_data_end,
    (u32)&__shave8_cmx_data_end, (u32)&__shave9_cmx_data_end, (u32)&__shave10_cmx_data_end, (u32)&__shave11_cmx_data_end
#if (defined(MA2480) || defined(MA2485) || defined(MA2485))
    ,(u32)&__shave12_cmx_data_end, (u32)&__shave13_cmx_data_end, (u32)&__shave14_cmx_data_end, (u32)&__shave15_cmx_data_end
#endif
};

//Choosing the default stack size if not already defined
#ifndef  SHV_DEFAULT_STACK_SIZE
#define SHV_DEFAULT_STACK_SIZE  8*1024
#endif

// Keep local copies of section headers...
// @TODO: should these just be assigned on the stack instead of globally
static tMofFileHeader    swc_mbinH;
static tMofSectionHeader swc_secH;

static swcShaveUnit_t shaveStatus[TOTAL_NUM_SHAVES];
//Declare shave stack sizes to keep track of user set-sizes for stack checking purposes
static u32 shaveStackSize[TOTAL_NUM_SHAVES] = {SHV_DEFAULT_STACK_SIZE, SHV_DEFAULT_STACK_SIZE, SHV_DEFAULT_STACK_SIZE,  SHV_DEFAULT_STACK_SIZE,
        SHV_DEFAULT_STACK_SIZE, SHV_DEFAULT_STACK_SIZE, SHV_DEFAULT_STACK_SIZE,  SHV_DEFAULT_STACK_SIZE,
        SHV_DEFAULT_STACK_SIZE, SHV_DEFAULT_STACK_SIZE, SHV_DEFAULT_STACK_SIZE, SHV_DEFAULT_STACK_SIZE
#if (defined(MA2480) || defined(MA2485) || defined(MA2485))
        ,SHV_DEFAULT_STACK_SIZE,SHV_DEFAULT_STACK_SIZE,SHV_DEFAULT_STACK_SIZE,SHV_DEFAULT_STACK_SIZE
#endif
};
//Declare shave stack sizes to keep track of user set stack tops for stack checking purposes
//Needs to be done like this instead of reading i19 because users may have turned off the
//shave before checking their stack usage
static u32 shaveStackTops[TOTAL_NUM_SHAVES] = {(u32)&__SVE0_STACK_POINTER, (u32)&__SVE1_STACK_POINTER, (u32)&__SVE2_STACK_POINTER,  (u32)&__SVE3_STACK_POINTER,
        (u32)&__SVE4_STACK_POINTER, (u32)&__SVE5_STACK_POINTER, (u32)&__SVE6_STACK_POINTER,  (u32)&__SVE7_STACK_POINTER,
        (u32)&__SVE8_STACK_POINTER, (u32)&__SVE9_STACK_POINTER, (u32)&__SVE10_STACK_POINTER, (u32)&__SVE11_STACK_POINTER
#if (defined(MA2480) || defined(MA2485) || defined(MA2485))
        ,(u32)&__SVE12_STACK_POINTER,(u32)&__SVE13_STACK_POINTER,(u32)&__SVE14_STACK_POINTER,(u32)&__SVE15_STACK_POINTER
#endif
};

#ifndef LEON_RT
    int lrt_freeShavesGlobalArrayElements[TOTAL_NUM_SHAVES] = {
        SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED,
        SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED,
        SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED
    #if (defined(MA2480) || defined(MA2485) || defined(MA2485))
        ,SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED, SHAVE_NOT_ASSIGNED
    #endif
    };

	int* freeShavesGlobalArray = &lrt_freeShavesGlobalArrayElements[0];
#else
    extern int freeShavesGlobalArrayElements[];
    int* freeShavesGlobalArray = &freeShavesGlobalArrayElements[0];
#endif

#ifndef LEON_RT
__attribute__((weak)) char lrt___ShvXdataContextLoaded[TOTAL_NUM_SHAVES][MAX_APPLICATION_NAME];
__attribute__((weak)) char lrt___ShvZdataContextLoaded[TOTAL_NUM_SHAVES][MAX_APPLICATION_NAME];
__attribute__((weak)) char lrt___ShvdlibContextLoaded[TOTAL_NUM_SHAVES][MAX_APPLICATION_NAME];

char (* __ShvXdataContextLoaded)[MAX_APPLICATION_NAME] = lrt___ShvXdataContextLoaded;
char (* __ShvZdataContextLoaded)[MAX_APPLICATION_NAME] = lrt___ShvZdataContextLoaded;
char (* __ShvdlibContextLoaded)[MAX_APPLICATION_NAME]  = lrt___ShvdlibContextLoaded;
#else
__attribute__((weak)) char __ShvXdataContextLoaded[TOTAL_NUM_SHAVES][MAX_APPLICATION_NAME];
__attribute__((weak)) char __ShvZdataContextLoaded[TOTAL_NUM_SHAVES][MAX_APPLICATION_NAME];
__attribute__((weak)) char __ShvdlibContextLoaded[TOTAL_NUM_SHAVES][MAX_APPLICATION_NAME];
#endif

ShDrvShaveDataStruct GlbShaveData = {.Xdata = NULL, .Zdata = NULL, .dyndata = NULL, .profileData = NULL};

#ifdef SHAVE_APP_ENABLE_DEBUG_CONTEXT
void
__attribute__((no_instrument_function))
__attribute__((constructor))
__leonGlobalDataInit(void) {
    GlbShaveData.Xdata = (char**)__ShvXdataContextLoaded;
    GlbShaveData.Zdata = (char**)__ShvZdataContextLoaded;
    GlbShaveData.dyndata = (char**)__ShvdlibContextLoaded;
}
#endif

void swcStackPainter(u32 shaveNumber, u32 canaryValue)
{
    u32 *stackTop = (u32*)(shaveStackTops[shaveNumber] - sizeof(u32));
    u32 *stackEnd  = (u32*)(stackTop - shaveStackSize[shaveNumber]);

    while (stackTop != stackEnd )
    {
        *stackTop--=canaryValue;
    }
}

u32 swcGetUnusedShaveFreeStack( u32 shaveNumber, u32 canaryValue)
{
    u32 *stackTop = (u32*)shaveStackTops[shaveNumber];
    u32 i =0;
    u32 freeW=0;
    u32 usedW=0;

    for(i = 0; i < shaveStackSize[shaveNumber]/4; i++)
       if(*(stackTop-i)==canaryValue)
           freeW++;
       else
           usedW++;

    return freeW;
}

void swcStateConsideredShaveStackSize(u32 shaveNumber, u32 size)
{
    assert(shaveNumber < TOTAL_NUM_SHAVES);

    shaveStackSize[shaveNumber] = size;

    return;
}

u32 swcGetShaveStackSize(u32 shaveNumber)
{
    assert(shaveNumber < TOTAL_NUM_SHAVES);

    return (u32)shaveStackSize[shaveNumber];
}

void swcSetAbsoluteDefaultStack(u32 shaveNumber)
{
    u32 defaultStackPointer=0;
    u32 lastDataAddr=0;

    assert(shaveNumber < TOTAL_NUM_SHAVES);

    defaultStackPointer = shaveStackPtrs[shaveNumber];
    lastDataAddr = shaveLastDataAdd[shaveNumber];

    DrvSvutIrfWrite(shaveNumber, 19, defaultStackPointer);
    DrvSvutIrfWrite(shaveNumber, 20, lastDataAddr);
    DrvSvutIrfWrite(shaveNumber, 21, 0);

    //Update stack top
    shaveStackTops[shaveNumber]=defaultStackPointer;
    swcStateConsideredShaveStackSize(shaveNumber,lastDataAddr);

    return;
}

void swcSetWindowedDefaultStack(u32 shaveNumber)
{
    u32 win_x[4];
    u32 ram_code_stack = 0;
    u32 stack_pointer = 0;
    u32 addr_next_shave = 0;

    if (shaveNumber <= (TOTAL_NUM_SHAVES - 1)){
        swcGetShaveWindowRegs(shaveNumber, win_x);
        const u32 cmx_slice_size = 0x20000;
        // Align to the next CMX slice boundary
        addr_next_shave = ((win_x[0] + cmx_slice_size) & (~(cmx_slice_size - 1)));
        // Set the top of the stack at the top of the CMX slice
        ram_code_stack = addr_next_shave - win_x[0];
        stack_pointer = 0x1c000000 | ram_code_stack;
    }

    DrvSvutIrfWrite(shaveNumber, 19, stack_pointer);

    int stack_base = stack_pointer - SHV_DEFAULT_STACK_SIZE;

    DrvSvutIrfWrite(shaveNumber, 20, stack_base);
    DrvSvutIrfWrite(shaveNumber, 21, 0);
    //Update stack top
    shaveStackTops[shaveNumber]=swcSolveShaveRelAddr(stack_pointer,shaveNumber);
    swcStateConsideredShaveStackSize(shaveNumber,stack_base);
}

void swcGetShaveWindowRegs(u32 shaveNumber, u32* windows)
{
    u32 windowRegAddr = SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * shaveNumber);

    assert(shaveNumber < TOTAL_NUM_SHAVES);

    windows[0] = GET_REG_WORD_VAL(windowRegAddr + SLC_TOP_OFFSET_WIN_A);
    windows[1] = GET_REG_WORD_VAL(windowRegAddr + SLC_TOP_OFFSET_WIN_B);
    windows[2] = GET_REG_WORD_VAL(windowRegAddr + SLC_TOP_OFFSET_WIN_C);
    windows[3] = GET_REG_WORD_VAL(windowRegAddr + SLC_TOP_OFFSET_WIN_D);
}

void swcSetShaveWindow(u32 shaveNumber, u32 windowNumber, u32 targetWindowBaseAddr)
{
    u32 windowRegAddr = SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * shaveNumber) + SLC_TOP_OFFSET_WIN_A + (windowNumber * 4);

    assert(shaveNumber < TOTAL_NUM_SHAVES);
    assert(windowNumber < 4);
    SET_REG_WORD(windowRegAddr, targetWindowBaseAddr);

    return;
}

void swcSetShaveWindows(u32 shaveNumber, u32 windowA, u32 windowB, u32 windowC, u32 windowD)
{
    //Calculate address of the WindowA register
    u32 address = SHAVE_0_BASE_ADR + SVU_SLICE_OFFSET * shaveNumber;
    //Set each register
    SET_REG_WORD(address + SLC_TOP_OFFSET_WIN_A, windowA);
    SET_REG_WORD(address + SLC_TOP_OFFSET_WIN_B, windowB);
    SET_REG_WORD(address + SLC_TOP_OFFSET_WIN_C, windowC);
    SET_REG_WORD(address + SLC_TOP_OFFSET_WIN_D, windowD);
    return;
}

void swcSetShaveWindowsToDefault(u32 shaveNumber)
{
    assert(shaveNumber < TOTAL_NUM_SHAVES);

    swcSetShaveWindows(shaveNumber, shaveWindows[shaveNumber][0], shaveWindows[shaveNumber][1],
        shaveWindows[shaveNumber][2], shaveWindows[shaveNumber][3]);

    return;
}

u32 swcShaveRunning(u32 svu)
{
    //If stopped, return 0
    if (((GET_REG_WORD_VAL(SVU_CTRL_ADDR[svu] + SLC_OFFSET_SVU+SVU_OCR)) & 0x4) || DrvSvuSwiHalted(svu))
        return 0;

    //else, it's running
    return 1;
}

// Start shave shave_nr from entry_point
void swcRunShave(u32 shave_nr, u32 entry_point)
{
    // Run the program. Note the first 2 steps are optional
    // Run the program. Note the first 2 steps are optional
    SET_REG_WORD(DCU_OCR(shave_nr), OCR_STOP_GO | OCR_TRACE_ENABLE);// Set STOP bit in control register
    SET_REG_WORD(DCU_IRR(shave_nr), 0xFFFFFFFF); // Clear any interrupts from previous test
    SET_REG_WORD(DCU_ICR(shave_nr), ICR_SWI_ENABLE);       // Enable SWI interrupt

    ShDrvShaveDataStruct** aux;
    aux=(ShDrvShaveDataStruct**)swcSolveShaveRelAddr(0x1C000000,shave_nr);
    *aux=(ShDrvShaveDataStruct*)(&GlbShaveData);

    //Start Shave
    DrvSvuStart(shave_nr, entry_point);

    // Add the tracer functionality to visualize the shave wait:start
    dbgLogEvent(LOG_EVENT_WAIT_FOR_SHAVE_0 +  shave_nr, 1, DEBUG_LOG_LEVEL_HIGH);

    //Wait for completion (wait for program to halt)
    while (!DrvSvuSwiHalted(shave_nr))
        NOP;

    // Add the tracer functionality to visualize the shave wait:complete
    dbgLogEvent(LOG_EVENT_WAIT_FOR_SHAVE_0 +  shave_nr, 0, DEBUG_LOG_LEVEL_HIGH);

    // The run signal has begun to be traced in the DrvSvuStart() function
    // Add the tracer functionality to visualize the shave run:complete
    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RUN + shave_nr), 0, DEBUG_LOG_LEVEL_HIGH);
}

void swcSetRounding(u32 shave_no, u32 roundingBits)
{
    SET_REG_WORD(DCU_SVU_TRF(shave_no, P_CFG), roundingBits); // Float rounding mode
    return;
}

void swcStartShave(u32 shave_nr, u32 entry_point)
{
    // Run the program. Note the first 2 steps are optional
    SET_REG_WORD(DCU_OCR(shave_nr), OCR_STOP_GO | OCR_TRACE_ENABLE);// Set STOP bit in control register
    SET_REG_WORD(DCU_IRR(shave_nr), 0xFFFFFFFF); // Clear any interrupts from previous test
    SET_REG_WORD(DCU_ICR(shave_nr), ICR_SWI_ENABLE);       // Enable SWI interrupt

    //Start Shave
    DrvSvuStart(shave_nr, entry_point);
}

void swcDynStartShave(u32 shave_nr, u32 Context)
{
    // Run the program. Note the first 2 steps are optional
    SET_REG_WORD(DCU_OCR(shave_nr), OCR_STOP_GO | OCR_TRACE_ENABLE);// Set STOP bit in control register
    SET_REG_WORD(DCU_IRR(shave_nr), 0xFFFFFFFF); // Clear any interrupts from previous test
    SET_REG_WORD(DCU_ICR(shave_nr), ICR_SWI_ENABLE);       // Enable SWI interrupt

    ShDrvShaveDataStruct** aux;
    aux=(ShDrvShaveDataStruct**)swcSolveShaveRelAddr(0x1C000000,shave_nr);
    *aux=(ShDrvShaveDataStruct*)(&GlbShaveData);

    // Put in I21 the addres of the execution context for the current shave
    DrvSvutIrfWrite(shave_nr,21, (u32)(&(((DynamicContext_t*)Context)->crtContextInfo[shave_nr])));

    u32 topStackAddress = DrvSvutIrfRead(shave_nr, 19);

    // Set the last address of the stack for stack-overflow detection
    DrvSvutIrfWrite(shave_nr, 20, topStackAddress - ((DynamicContext_t*)Context)->crtContextInfo[shave_nr].stack_size);

    //Initializing entry point jump for backward compatibility
    //TODO: remove this once all apps in the world switch to the three main SHAVE paradigms
    DrvSvutIrfWrite(shave_nr, 29, (u32)((DynamicContext_t*)Context)->entryPoint);

    // Start Shave
    DrvSvuStart(shave_nr, (u32)((DynamicContext_t*)Context)->pse[shave_nr]);
}

static void swcClearPendingIrqs(u32 shave_nr)
{
    u32 isr_status;
    u8  isr_clearFlag = 0;

    isr_status = GET_REG_WORD_VAL(DCU_ISR(shave_nr));
    // SGI interrupts should not be cleared, before the shave is stopped. Bug 27116 : http://dub30.ir.intel.com/bugzilla/show_bug.cgi
    // if(IS_DCU_ISR_SGI_TRIGGERED(isr_status)) isr_clearFlag |= DCU_IRR_SGIIC_MASK;
    if(IS_DCU_ISR_DWI_TRIGGERED(isr_status))  isr_clearFlag |= DCU_IRR_DWIIC_MASK;
    if(IS_DCU_ISR_RMO_TRIGGERED(isr_status))  isr_clearFlag |= DCU_IRR_RMOIC_MASK;
    if(IS_DCU_ISR_IO_TRIGGERED(isr_status))   isr_clearFlag |= DCU_IRR_IOIC_MASK;
    if(IS_DCU_ISR_IE_TRIGGERED(isr_status))   isr_clearFlag |= DCU_IRR_IEIC_MASK;
    if(IS_DCU_ISR_FPE_TRIGGERED(isr_status))  isr_clearFlag |= DCU_IRR_FPEIC_MASK;

    SET_REG_WORD(DCU_IRR(shave_nr), isr_clearFlag);
}

void swcAssignShaveCallback(u32 shave_nr, irq_handler function)
{
    //Clear pending interrupts so we can have the next interrupt happening
    swcClearPendingIrqs(shave_nr);

    //Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqClear(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqUnsetup(IRQ_SVE_0 + shave_nr);

    //Configure interrupt handlers
    DrvIcbSetIrqHandler(IRQ_SVE_0 + shave_nr, function);
    //Enable interrupts on SHAVE done
    DrvIcbConfigureIrq(IRQ_SVE_0 + shave_nr, SHAVE_INTERRUPT_LEVEL, POS_EDGE_INT);
    DrvIcbEnableIrq(IRQ_SVE_0 + shave_nr);

    //Enable SWIH IRQ sources
    SET_REG_WORD(DCU_ICR(shave_nr), ICR_SWI_ENABLE);
    return;
}

void swcStartShaveAsync(u32 shave_nr, u32 entry_point, irq_handler function)
{

#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shave_nr))
        DrvCprEnableUpa(shave_nr);
#endif

    //Clear pending interrupts so we can have the next interrupt happening
    swcClearPendingIrqs(shave_nr);

    //Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqClear(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqUnsetup(IRQ_SVE_0 + shave_nr);

    //Configure interrupt handlers
    DrvIcbSetIrqHandler(IRQ_SVE_0 + shave_nr, function);
    //Enable interrupts on SHAVE done
    DrvIcbConfigureIrq(IRQ_SVE_0 + shave_nr, SHAVE_INTERRUPT_LEVEL, POS_EDGE_INT);
    DrvIcbEnableIrq(IRQ_SVE_0 + shave_nr);

    // Run the program. Note the first 2 steps are optional
    SET_REG_WORD(DCU_OCR(shave_nr), OCR_STOP_GO | OCR_TRACE_ENABLE);// Set STOP bit in control register
    SET_REG_WORD(DCU_IRR(shave_nr), 0xFFFFFFFF); // Clear any interrupts from previous test
    SET_REG_WORD(DCU_ICR(shave_nr), ICR_SWI_ENABLE);       // Enable SWI interrupt

    //Can enable the interrupt now
    swcLeonEnableTraps();

    //Start Shave
    DrvSvuStart(shave_nr, entry_point);
}

void swcShaveStartAsync(u32 shave_nr, u32 entry_point, irq_handler function)
{

#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shave_nr))
        DrvCprEnableUpa(shave_nr);
#endif

    //Clear pending interrupts so we can have the next interrupt happening
    swcClearPendingIrqs(shave_nr);

    //Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqClear(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqUnsetup(IRQ_SVE_0 + shave_nr);
    //Configure interrupt handlers
    DrvIcbSetIrqHandler(IRQ_SVE_0 + shave_nr, function);

    //Enable interrupts on SHAVE done
    DrvIcbConfigureIrq(IRQ_SVE_0 + shave_nr, SHAVE_INTERRUPT_LEVEL, POS_EDGE_INT);
    DrvIcbEnableIrq(IRQ_SVE_0 + shave_nr);

    // Run the program. Note the first 2 steps are optional
    SET_REG_WORD(DCU_OCR(shave_nr), OCR_STOP_GO | OCR_TRACE_ENABLE);// Set STOP bit in control register
    SET_REG_WORD(DCU_IRR(shave_nr), 0xFFFFFFFF); // Clear any interrupts from previous test
    SET_REG_WORD(DCU_ICR(shave_nr), ICR_SWI_ENABLE);       // Enable SWI interrupt

    //Start Shave
    DrvSvuStart(shave_nr, entry_point);
}

void swcDynShaveStartAsync(u32 shave_nr, u32 Context, irq_handler function)
{
#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shave_nr))
        DrvCprEnableUpa(shave_nr);
#endif

    //Clear pending interrupts so we can have the next interrupt happening
    swcClearPendingIrqs(shave_nr);

    //Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqClear(IRQ_SVE_0 + shave_nr);
    DrvIcbIrqUnsetup(IRQ_SVE_0 + shave_nr);
    //Configure interrupt handlers
    DrvIcbSetIrqHandler(IRQ_SVE_0 + shave_nr, function);

    //Enable interrupts on SHAVE done
    DrvIcbConfigureIrq(IRQ_SVE_0 + shave_nr, SHAVE_INTERRUPT_LEVEL, POS_EDGE_INT);
    DrvIcbEnableIrq(IRQ_SVE_0 + shave_nr);

    // Run the program. Note the first 2 steps are optional
    SET_REG_WORD(DCU_OCR(shave_nr), OCR_STOP_GO | OCR_TRACE_ENABLE);// Set STOP bit in control register
    SET_REG_WORD(DCU_IRR(shave_nr), 0xFFFFFFFF); // Clear any interrupts from previous test
    SET_REG_WORD(DCU_ICR(shave_nr), ICR_SWI_ENABLE);       // Enable SWI interrupt

    ShDrvShaveDataStruct** aux;
    aux = (ShDrvShaveDataStruct**) swcSolveShaveRelAddr(0x1C000000, shave_nr);
    *aux=(ShDrvShaveDataStruct*)(&GlbShaveData);

    // Put in I21 the addres of the execution context for the current shave
    DrvSvutIrfWrite(shave_nr,21,(u32)(&(((DynamicContext_t*)Context)->crtContextInfo[shave_nr])));

    u32 topStackAddress = DrvSvutIrfRead(shave_nr, 19);

    // Set the last address of the stack for stack-overflow detection
    DrvSvutIrfWrite(shave_nr, 20, topStackAddress - (u32)((DynamicContext_t*)Context)->crtContextInfo[shave_nr].stack_size);
    //Initializing entry point jump for backward compatibility
    //TODO: remove this once all apps in the world switch to the three main SHAVE paradigms
    DrvSvutIrfWrite(shave_nr, 29, (u32)((DynamicContext_t*)Context)->entryPoint);

    //Start Shave
    DrvSvuStart(shave_nr, (u32)((DynamicContext_t*)Context)->pse[shave_nr]);
}

void swcDisableShaveCallback(u32 shave_nr)
{
    DrvIcbDisableIrq(IRQ_SVE_0 + shave_nr);
    return;
}

void swcSetRegsCC(u32 shave_num, const char *fmt, va_list a_list)
{
    u32 x, *v;
    int i = 0;
    int vrf = 23, irf = 18;
    while (fmt[i] != '\0')
    {

        if ((fmt[i] == 'i') || (fmt[i] == 'I'))
        {

            assert(irf != 10 && "Number of irf registers more than 8");
            x = va_arg(a_list, u32);
            DrvSvutIrfWrite(shave_num, irf, x);

            irf--;
        }

        if ((fmt[i] == 'v') || (fmt[i] == 'V'))
        {
            assert(vrf != 15 && "Number of vrf registers more than 8");
            v = (u32 *)va_arg(a_list, u32);
            assert(v != NULL && "Number of vrf registers more than 8");
            DrvSvutVrfWrite(shave_num, vrf, 0, * (v + 0));
            DrvSvutVrfWrite(shave_num, vrf, 1, * (v + 1));
            DrvSvutVrfWrite(shave_num, vrf, 2, * (v + 2));
            DrvSvutVrfWrite(shave_num, vrf, 3, * (v + 3));
            vrf--;
        }

        i++;
    }
    return;
}

void swcStartShaveCC(u32 shave_num, u32 pc, const char *fmt, ...)
{
    va_list a_list;
    va_start(a_list, fmt);
#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shave_num))
        DrvCprEnableUpa(shave_num);
#endif
    swcSetRegsCC(shave_num, fmt, a_list);

    va_end(a_list);
    swcStartShave(shave_num, (u32)pc);

}

void swcStartShaveAsyncCC(u32 shave_num, u32 pc, irq_handler function, const char *fmt, ...)
{
    va_list a_list;
    va_start(a_list, fmt);
#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shave_num))
        DrvCprEnableUpa(shave_num);
#endif

    swcSetRegsCC(shave_num, fmt, a_list);

    va_end(a_list);
    swcShaveStartAsync(shave_num, (u32)pc, function);

}

void swcSetupShaveCC(u32 shave_num, const char *fmt, ...)
{
    va_list a_list;
    va_start(a_list, fmt);
#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shave_num))
        DrvCprEnableUpa(shave_num);
#endif

    swcSetRegsCC(shave_num, fmt, a_list);

    va_end(a_list);

}

void swcResetShave(u32 shaveNumber)
{
#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shaveNumber))
        DrvCprEnableUpa(shaveNumber);
#endif

    // Set STOP and 128_IDC bits in OCR to ensure application doesn't restart when we reset
    SET_REG_WORD(DCU_OCR(shaveNumber), OCR_STOP_GO | OCR_TRACE_ENABLE); // Set STOP bit in control register

    // Now we try to run it again after a reset
    DrvSvuSliceResetAll(shaveNumber); // reset every single bit

    // Set the default RF RAM configuration
    SET_REG_WORD(DCU_RF_CFG(shaveNumber),DEFAULT_RF_CONFIG);
}

void swcResetShaveLite(u32 shaveNumber)
{

#ifdef POWER_AWARE
    if (!DrvCprGetUpaClk(shaveNumber))
        DrvCprEnableUpa(shaveNumber);
#endif
    // Set STOP in OCR to ensure application doesn't restart when we reset
    SET_REG_WORD(SVU_CTRL_ADDR[shaveNumber] + SLC_OFFSET_SVU + SVU_OCR, 0x04);       // Set STOP bit in control register

    // Now we try to run it again after a reset
    DrvSvuSliceResetOnlySvu(shaveNumber); // Try just resetting the shave

    //Configure Window registers
    swcSetShaveWindowsToDefault(shaveNumber);
}

//Wait for shaves form shave_list to finish execution; number of shaves within the list is given by no_of_shaves

int swcWaitShaves(u32 no_of_shaves, swcShaveUnit_t *shave_list)
{
    u32 svu;
    int done = 0;

    // Use a variable to log the shave run signal the first time a shave stops
    swcShaveUnit_t haltShaves[TOTAL_NUM_SHAVES];     // (It takes as many more bytes of code to use an adjustable call frame
                                                     // as there are words of data in the maximally sized array)
    for(svu = 0; svu< no_of_shaves ; svu++)
    {
        haltShaves[svu] = 0;
    }

    // Add the tracer functionality to visualize the shave wait:start
    for (svu = 0; svu < no_of_shaves; svu++)
    {
        dbgLogEvent(LOG_EVENT_WAIT_FOR_SHAVE_0 +  shave_list[svu], 1, DEBUG_LOG_LEVEL_HIGH);
    }

    do
    {
        done = 1; // assumes all shaves are done
        for (svu = 0; svu < no_of_shaves; svu++)
        {
            if (!DrvSvuSwiHalted(shave_list[svu])) // wait for shave to halt
            {
                done = 0;
#ifdef POWER_AWARE
                if (DrvCprGetUpaClk(shave_list[svu]))
                    DrvCprClearUpa(shave_list[svu]);
#endif
            }
            else
            {
                if(haltShaves[shave_list[svu]] == 0)
                {
                    // Log for the first time we find that the shave has stopped
                    haltShaves[shave_list[svu]] = svu;
                    // Add the tracer functionality to visualize the shave run:complete
                    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RUN + shave_list[svu]), 0, DEBUG_LOG_LEVEL_HIGH);
                }
                // clear status for dynamically started shaves
                if (shaveStatus[svu])
                    shaveStatus[svu]=0;
            }
        }
    }while(!done);


    for (svu = 0; svu < no_of_shaves; svu++)
    {
        // Add the tracer functionality to visualize the shave wait:complete
        dbgLogEvent(LOG_EVENT_WAIT_FOR_SHAVE_0 +  shave_list[svu], 0, DEBUG_LOG_LEVEL_HIGH);
    }

    //Read the exit code of the every shave runned stored in SWI tag
    for (svu = 0; svu < no_of_shaves; svu++)
    {
        int shaveExitCode = DrvSvutSwiTag(shave_list[svu]);

        if (shaveExitCode == SHAVEExitStackOverflow)
            return MYR_DYN_INFR_ERROR_STACK_OVERFLOW;
    }

    return MYR_DYN_INFR_SUCCESS;
}

int swcWaitShave(u32 shave_nr)
{
    //if STOPPED, return
    if ((GET_REG_WORD_VAL(SVU_CTRL_ADDR[shave_nr] + SLC_OFFSET_SVU + SVU_OCR)) & 0x4)
    {
#ifdef POWER_AWARE
        if (DrvCprGetUpaClk(shave_nr))
            DrvCprClearUpa(shave_nr);
#endif
        return -1;
    }

    // Add the tracer functionality to visualize the shave wait :start
    dbgLogEvent(LOG_EVENT_WAIT_FOR_SHAVE_0 +  shave_nr, 1, DEBUG_LOG_LEVEL_HIGH);
    //else, wait for HALT
    while (!DrvSvuSwiHalted(shave_nr)) // Wait for program to halt
    {
        NOP;
    }

    // clear status for dynamically started shaves
    if (shaveStatus[shave_nr])
        shaveStatus[shave_nr]=0;

    // Add the tracer functionality to visualize the shave run:complete
    dbgLogEvent((Event_t)((u32)LOG_EVENT_SHAVE_0_RUN + shave_nr), 0, DEBUG_LOG_LEVEL_HIGH);

#ifdef POWER_AWARE
    if (DrvCprGetUpaClk(shave_nr))
        DrvCprClearUpa(shave_nr);
#endif

    // Add tracer functionality to visualize the shave wait:complete
    dbgLogEvent(LOG_EVENT_WAIT_FOR_SHAVE_0 +  shave_nr, 0, DEBUG_LOG_LEVEL_HIGH);

    //Read the exit code from the shave stored in SWI tag
    int shaveExitCode = DrvSvutSwiTag(shave_nr);

    if (shaveExitCode == SHAVEExitStackOverflow)
        return MYR_DYN_INFR_ERROR_STACK_OVERFLOW;

    return MYR_DYN_INFR_SUCCESS;
}

u32 swcShavesRunning(u32 first, u32 last)
{
    u32 svu;

    for (svu = first; svu <= last; svu++)
    {
        if (swcShaveRunning(svu))
        {
            return 1;
        }
    }

    return 0;
}

u32 swcShavesRunningArr(u32 arr[], u32 N)
{
    u32 si;

    for (si = 0; si < N; si++)
    {
        if (swcShaveRunning(arr[si]))
        {
            //at least 1 shave from the group is running, so it's not over !
            return 1;
        }
    }

    return 0;
}

//#######################################################################
// Converts a Shave -Relative address to a Systeme solved address (in 0x10000000 view), based on the [Target CMX Slice]
//              and current widnow it relates to
// Inputs : inAddr :shave relative address (can be code/data/absolute tyep of address)
// Return : the resolved addr
//#######################################################################
u32 swcSolveShaveRelAddr(u32 inAddr, u32 shaveNumber)
{
    // Map to window number considering that 0x1C is the first one
    const u32 window = (inAddr >> 24) - 0x1C;
    const u32* windowRegPtr = (u32 *)(SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * shaveNumber) + SLC_TOP_OFFSET_WIN_A);
    u32 windowBase;
    u32 resolved;

    // Absolute address, no translation is to be done
    if (window > 3)
        return inAddr;

    windowBase = windowRegPtr[window];
    assert(windowBase != 0);                // Making sure the caller has first called swcSetShaveWindows
    resolved = ((inAddr & 0x00FFFFFF) + windowBase);
    return resolved;
}

void swcLoadMbin(u8 *sAddr, u32 targetS)
{
    int sec;
    u32 srcA32;
    u32 dstA32;

    //get the program header
    memcpy(&swc_mbinH, sAddr, sizeof(tMofFileHeader));
    //dma_memcpy((u32*)&swc_mbinH,     (u32*)sAddr, sizeof(tMofFileHeader));

    //parse sections:
    for (sec = 0; sec < swc_mbinH.shCount; sec++)
    {
        //get current section
        memcpy(&swc_secH, (sAddr) + ((swc_mbinH.shOffset + sec * swc_mbinH.shEntrySize) >> 2), sizeof(tMofSectionHeader));

        //if this is a BSS type of section, just continue
        if (swc_secH.type == MSHT_NOBITS)
            continue;
        srcA32   = (u32)(sAddr + swc_secH.offset);
        dstA32   = swcSolveShaveRelAddr(swc_secH.address, targetS);

        memcpy((void*)dstA32, (const void*)srcA32, swc_secH.size);
    }
}

static inline void moveStringUncached(const u8 * restrict src, int max, char* restrict dst)
{
    if ((u32)dst & 0x80000000)
        dst = ((char *)((u32) dst | 0x40000000));
    else
        dst = ((char *)((u32) dst | 0x08000000)); // Assume CMX

    while(max--){
        *dst++=*src++;
    }
    *dst = '\0';
}

static inline uint32_t getUnsigned(const u8 * src)
{
    uint32_t val=0;
    for(uint32_t i=0;i<4;i++){
        val=val*10+src[i]-'0';
        //TODO: if(src[i]<'0' || src[i]>'9') treat this also
    }
    return val;
}

/// Dynamically load shvdlib file - These are elf object files stripped of any symbols
///@param sAddr starting address where to load the shvdlib file
///@param targetS the target Shave
void swcLoadshvdlib(u8 *sAddr, u32 targetS)
{
    Elf32Ehdr ElfHeader;
    u32 SecHeaders;
    u32 phAddr;
    u32 *srcAddr;
    u32 *dstAddr;
    u32 SecSize;

    // check if the loaded context has debug information at the beginning of the ELF
    // magic number of the ELF header
    if (*(u32*) sAddr != 0x464c457f) {
        unsigned int stringLength;

        // get the length of the name from the loaded library
        stringLength = getUnsigned(sAddr);
        // set the address to the beginning of the string name
        sAddr += 4;

        assert(stringLength <= (MAX_APPLICATION_NAME-4)); //Application file name longer then supported, rename file name.

        moveStringUncached(sAddr, stringLength, __ShvdlibContextLoaded[targetS]);

        // set the address to the beginning of the elf section header
        sAddr += (stringLength + 3) & 0xFFFFFFFC;
    }

    //get the section header
    memcpy(&ElfHeader, sAddr, sizeof(Elf32Ehdr));

    //Reading section headers table offset
    phAddr = (u32)sAddr + ElfHeader.eShoff;

    //parse section headers:
    for (SecHeaders = 0; SecHeaders < ElfHeader.eShnum; SecHeaders++)
    {
        Elf32Section ElfSecHeader;
        u32 SecOffset;
        u32 SecDataOffset;

        SecOffset = phAddr + sizeof(Elf32Section) * SecHeaders;
        memcpy(&ElfSecHeader, (const void*)SecOffset, sizeof(Elf32Section));
        SecDataOffset = (u32)sAddr + ElfSecHeader.shOffset;

        srcAddr = (u32*)SecDataOffset;
        dstAddr = (u32*)swcSolveShaveRelAddr(ElfSecHeader.shAddr, targetS);
        SecSize = ElfSecHeader.shSize;
        //Only load PROGBITS sections
        if (ElfSecHeader.shType == SHT_PROGBITS)
        {
            swcLeonMemCpy((u32*)dstAddr, le_pointer,(u32*)srcAddr,le_pointer, SecSize);
        }
    }
}

void swcLoadDynLibraryCacheAware(u8 *sAddr, u32 targetS, context_t contextType, u32 * vpProgrammedMemAddress, u32 * flushLength){
    Elf32Ehdr ElfHeader;
    u32 SecHeaders;
    u32 phAddr;
    u32 *srcAddr;
    u32 *dstAddr=(u32 *)NULL;
    u32 SecSize = 0;
    u32 LastAddressWritten = 0;
    u32 FirstAddressWritten = 0;
    u32 LastSizeWritten = 0;

#ifdef SHAVE_APP_ENABLE_DEBUG_CONTEXT
    // check if the loaded context has debug information at the beginning of the ELF
    // magic number of the ELF header
    if ((u32)sAddr & 0x80000000)
        sAddr = ((u8 *)((u32) sAddr | 0x40000000));
    else
        sAddr = ((u8 *)((u32) sAddr | 0x08000000)); // Assume CMX
    
    if(*(u32*)sAddr != 0x464c457f){
        unsigned int stringLength;

        // get the length of the name from the loaded library
        stringLength = getUnsigned(sAddr);
        sAddr += 4;

        assert(stringLength <= (MAX_APPLICATION_NAME-4)); //Application file name longer then supported, rename file name

        // save the name of the application library
        switch (contextType) {
            case SHVXDATA:
                moveStringUncached(sAddr, stringLength, __ShvXdataContextLoaded[targetS]);
                break;
            case SHVZDATA:
                moveStringUncached(sAddr, stringLength, __ShvZdataContextLoaded[targetS]);
                break;
            case SHVDLIB:
                moveStringUncached(sAddr, stringLength, __ShvdlibContextLoaded[targetS]);
                break;
            default:
                break;
        }

        // set the address to the beginning of the elf section header
        sAddr += (stringLength + 3) & 0xFFFFFFFC;
    }
#else
    UNUSED(contextType);
#endif

    //Clear the variable for the starting mem address
    FirstAddressWritten = 0;

    //get the section header
    swcU32memcpy((u32*)&ElfHeader, (u32*)sAddr, sizeof(Elf32Ehdr));

    //Reading section headers table offset
    phAddr = (u32)sAddr + ElfHeader.eShoff;

    //parse section headers:
    for (SecHeaders = 0; SecHeaders < ElfHeader.eShnum; SecHeaders++)
    {
        Elf32Section ElfSecHeader;
        u32 SecOffset;
        u32 SecDataOffset;
        SecOffset = phAddr + sizeof(Elf32Section) * SecHeaders;
        swcU32memcpy((u32*)&ElfSecHeader, (u32*)SecOffset, sizeof(Elf32Section));
        SecDataOffset = (u32)sAddr + ElfSecHeader.shOffset;

        srcAddr = (u32*)SecDataOffset;
        dstAddr = (u32*)swcSolveShaveRelAddr(ElfSecHeader.shAddr, targetS);
        SecSize = ElfSecHeader.shSize;
        //Only load PROGBITS sections
        // printf("section start 0x%p and length %u, ElfSecHeader.shType 0x%08X \n",(u32*)srcAddr,(unsigned int)SecSize, (unsigned int)ElfSecHeader.shType);

        if (ElfSecHeader.shType == SHT_PROGBITS)
        {
            swcLeonMemCpy((u32*)dstAddr, le_pointer,(u32*)srcAddr,le_pointer, SecSize);
            // Store the first written address
            if(FirstAddressWritten == 0) {
                FirstAddressWritten = (u32)dstAddr;
            }
            LastAddressWritten = (u32)dstAddr;
            LastSizeWritten = SecSize;
        // } else if (ElfSecHeader.shType == SHT_NOBITS)
        // {
        //     // printf("src BSS section start 0x%p and length %u \n",(u32*)srcAddr,(unsigned int)SecSize);
        //     // printf("dst BSS section start 0x%p and length %u \n",(u32*)dstAddr,(unsigned int)SecSize);
        //     for(u32 index=0; index<(u32)(SecSize/4);index++)
        //         *(u32*)(dstAddr++) =(uint32_t) 0;  // write in L2 cache then flush it together with the memcopy part
        //     LastAddressWritten = (u32)dstAddr;
        //     LastSizeWritten = SecSize;
        }
    }

    //Calculate the cache flush range for the written data
    *flushLength = ((LastAddressWritten > FirstAddressWritten)?(LastAddressWritten - FirstAddressWritten):(FirstAddressWritten - LastAddressWritten)) + LastSizeWritten;
    //Store first address written
    *vpProgrammedMemAddress = FirstAddressWritten;
}

void swcLoadDynLibrary(u8 *sAddr, u32 targetS, context_t contextType)
{
    Elf32Ehdr ElfHeader;
    u32 SecHeaders;
    u32 phAddr;
    u32 *srcAddr;
    u32 *dstAddr;
    u32 SecSize;

#ifdef SHAVE_APP_ENABLE_DEBUG_CONTEXT
    // check if the loaded context has debug information at the beginning of the ELF
    // magic number of the ELF header
    if(*(u32*)sAddr != 0x464c457f){
        unsigned int stringLength;

        // get the length of the name from the loaded library
        stringLength = getUnsigned(sAddr);
        sAddr += 4;

        assert(stringLength <= (MAX_APPLICATION_NAME-4)); //Application file name longer then supported, rename file name

        // save the name of the application libary
        switch (contextType) {
            case SHVXDATA:
                moveStringUncached(sAddr, stringLength, __ShvXdataContextLoaded[targetS]);
                break;
            case SHVZDATA:
                moveStringUncached(sAddr, stringLength, __ShvZdataContextLoaded[targetS]);
                break;
            case SHVDLIB:
                moveStringUncached(sAddr, stringLength, __ShvdlibContextLoaded[targetS]);
                break;
            default:
                break;
        }

        // set the address to the beginning of the elf section header
        sAddr += (stringLength + 3) & 0xFFFFFFFC;
    }
#else
    UNUSED(contextType);
#endif
    
    //get the section header
    memcpy(&ElfHeader, sAddr, sizeof(Elf32Ehdr));

    //Reading section headers table offset
    phAddr = (u32) sAddr + ElfHeader.eShoff;
    //parse section headers:
    for (SecHeaders = 0; SecHeaders < ElfHeader.eShnum; SecHeaders++) {
        Elf32Section ElfSecHeader;
        u32 SecOffset;
        u32 SecDataOffset;

        SecOffset = phAddr + sizeof(Elf32Section) * SecHeaders;
        memcpy(&ElfSecHeader, (const void*)SecOffset, sizeof(Elf32Section));
        SecDataOffset = (u32)sAddr + ElfSecHeader.shOffset;

        srcAddr = (u32*)SecDataOffset;
        dstAddr = (u32*)swcSolveShaveRelAddr(ElfSecHeader.shAddr, targetS);
        SecSize = ElfSecHeader.shSize;
        //Only load PROGBITS sections
        if (ElfSecHeader.shType == SHT_PROGBITS)
        {
            swcLeonMemCpy((u32*)dstAddr, le_pointer,(u32*)srcAddr,le_pointer, SecSize);
        }
    }
}

static int swcDynFindFreeShave(DynamicContext_t const *modData)
{
    u32 i=0;
    int shaveNumber = -1;

    if (modData != NULL) {
        while (i < modData->instancesData->appInstances)
        {
            /* search configured shaves to be used */
            if (!swcShaveRunning(modData->instancesData->shaveList[i])){
                shaveNumber = modData->instancesData->shaveList[i];
                return shaveNumber;
            } /* else continue searching */
            i++;
        }
    }
    return shaveNumber;
}

//This function is used to check if the user has called a correct shave
//we define "correct" as: configured to be used by the current dyncontext
//and: it's not currently running
//returns 0 if it is not a valid shave, or 1 if it is
u32 swcCheckFreeAndValidShave(DynamicContext_t *modData, u32 shaveNumber)
{
    unsigned int i;

    if (modData != NULL){
        for (i=0;i<modData->instancesData->appInstances;i++){
            // Check if we find the shave in the configured instances AND
            // if it's running or not
            if ( (modData->instancesData->shaveList[i] == shaveNumber)  &&
                 !swcShaveRunning(modData->instancesData->shaveList[i])  )
                return 1;
        }
    }
    return 0;
}

#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
static s32 swcFreeAppDataPools(DynamicContext_t *mData, u32 shaveNumber)
{
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status;

    /* free group data and heap space for this instance */
    if (mData != NULL){
        if ((mData->cmxCriticalCodeAllocAddr[shaveNumber] != NULL) && (mData->cmxCriticalCodeSize > 0)) {
            MemMgrFree(mData->cmxCriticalCodeAllocAddr[shaveNumber]);
        }

        if ((mData->appdyndataAllocAddr[shaveNumber] != NULL) && (mData->appdyndatasize > 0)) {
            MemMgrFree(mData->appdyndataAllocAddr[shaveNumber]);
        }

        if ((mData->instancesData->HeapPoolsStart[shaveNumber] != NULL) && (mData->heap_size > 0)) {
            MemMgrFree(mData->instancesData->HeapPoolsStart[shaveNumber]);
        }

        if ((mData->instancesData->GrpDataPoolsStart[shaveNumber] != NULL) && (mData->groupappdyndatasize > 0)) {
            MemMgrFree(mData->instancesData->GrpDataPoolsStart[shaveNumber]);
        }

        status = MYR_DYN_INFR_SUCCESS;
    } else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;
    return status;
}

#else
static s32 swcFreeAppDataPools(DynamicContext_t *mData, u32 shaveNumber)
{
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status;

    /* free group data and heap space for this instance */
    if (mData != NULL){
        free(mData->instancesData->HeapPoolsStart[shaveNumber]);
        free(mData->instancesData->GrpDataPoolsStart[shaveNumber]);
        status = MYR_DYN_INFR_SUCCESS;
    } else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;
    return status;
}
#endif

static s32 swcSetupAppDataPools(DynamicContext_t *mData, u32 shaveNumber)
{
    const size_t RoundedUpGroupDataSize = (size_t)ROUND_UP(mData->groupappdyndatasize + 1024, 64);
    const size_t RoundedUpHeapSize = (size_t)ROUND_UP(mData->heap_size + 64, 64);

#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
    if(mData->cmxCriticalCodeSize > 0) {
        mData->cmxCriticalCodeAllocAddr[shaveNumber] = MemMgrAlloc(mData->cmxCriticalCodeSize, shaveNumber, 0x10u);
        if (mData->cmxCriticalCodeAllocAddr[shaveNumber] == NULL) {
            return MYR_DYN_INFR_ERROR;
        }
    }

    if(mData->appdyndatasize > 0) {
        if((mData->appdyndataAllocAddr[shaveNumber] = MemMgrAlloc(mData->appdyndatasize, shaveNumber, 0x10u)) == NULL)  {
            mData->appdyndataAllocAddr[shaveNumber] = MemMgrAlloc(mData->appdyndatasize, DDR_AREA, 0x10u);
        }
        if (mData->appdyndataAllocAddr[shaveNumber] == NULL) {
            return MYR_DYN_INFR_ERROR;
        }
    } else {
        mData->appdyndataAllocAddr[shaveNumber] = mData->cmxCriticalCodeAllocAddr[shaveNumber] + mData->cmxCriticalCodeSize;
    }

    // allocate group data for this instance
    // Group data is used in its separate SHAVE window and each window needs 1024 (1K alignments) so using 1024 below
    // Round up the size to the cache line size (64)
    if ((mData->instancesData->GrpDataPoolsStart[shaveNumber] = MemMgrAlloc(RoundedUpGroupDataSize, shaveNumber, 1024)) == NULL)  {
        mData->instancesData->GrpDataPoolsStart[shaveNumber] = MemMgrAlloc(RoundedUpGroupDataSize, DDR_AREA, 1024);
    }

#else
    mData->cmxCriticalCodeAllocAddr[shaveNumber] = (void *)CMX_BASE_ADR+shaveNumber*CMX_SLICE_SIZE;
    mData->appdyndataAllocAddr[shaveNumber] = (void *)mData->cmxCriticalCodeAllocAddr[shaveNumber] + mData->cmxCriticalCodeSize;
    mData->instancesData->GrpDataPoolsStart[shaveNumber] = malloc(RoundedUpGroupDataSize);
#endif

    if (mData->instancesData->GrpDataPoolsStart[shaveNumber] == NULL)
        return MYR_DYN_INFR_CANNOT_ALLOC_GROUPDATA;

    // And update the module fields so that the next instantiation starts from the proper addresses
    // Round up to sensible 1024 bytes alignments

#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
    mData->instancesData->GrpDataPools[shaveNumber] = mData->instancesData->GrpDataPoolsStart[shaveNumber];
#else
    mData->instancesData->GrpDataPools[shaveNumber] = ALIGN_UP(mData->instancesData->GrpDataPoolsStart[shaveNumber], 1024);
#endif

    if((mData->initHeap) == DYNCONTEXT_HEAP_INIT) {
        if(mData->heap_size != 0) {
            // allocate heap space for this instance
            // add space for 64 bytes for possible alignment to cache line
            // round up the size to the cache line size (64)

#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
            if((mData->instancesData->HeapPoolsStart[shaveNumber] = MemMgrAlloc(RoundedUpHeapSize, shaveNumber, 64)) == NULL) {
                mData->instancesData->HeapPoolsStart[shaveNumber] = MemMgrAlloc(RoundedUpHeapSize, DDR_AREA, 64);
            }
#else
            mData->instancesData->HeapPoolsStart[shaveNumber] = malloc(RoundedUpHeapSize);
#endif

            if (mData->instancesData->HeapPoolsStart[shaveNumber] == NULL)
                return MYR_DYN_INFR_CANNOT_ALLOC_HEAP;

            // And update the module fields so that the next instantiation starts from the proper addresses
            // Round up to sensible 64 bytes alignments
#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
            mData->instancesData->HeapPools[shaveNumber] = mData->instancesData->HeapPoolsStart[shaveNumber];
#else
            mData->instancesData->HeapPools[shaveNumber] = ALIGN_UP(mData->instancesData->HeapPoolsStart[shaveNumber], 64);
#endif
            mData->crtContextInfo[shaveNumber].heap_address = mData->instancesData->HeapPools[shaveNumber];
        } else {
            mData->instancesData->HeapPools[shaveNumber] = NULL;
            mData->crtContextInfo[shaveNumber].heap_address = NULL;

            return MYR_DYN_INFR_HEAP_SIZE_NOT_SET;
        }
    } else {
        mData->instancesData->HeapPools[shaveNumber] = NULL;
        mData->crtContextInfo[shaveNumber].heap_address = NULL;

        if(mData->heap_size != 0) {
            return MYR_DYN_INFR_HEAP_NOT_SELECTED;
        }
    }

    // Configure bss data sections for the shave to be able to initialize it
    mData->crtContextInfo[shaveNumber].appbssData_address = (uint64_t *)mData->appdynbssdatastart;
    mData->crtContextInfo[shaveNumber].appbssData_size = (uint32_t)mData->appdynbssdataend - (uint32_t)mData->appdynbssdatastart;
    mData->crtContextInfo[shaveNumber].groupbssData_address = (uint64_t *)mData->groupappdynbssdatastart;
    mData->crtContextInfo[shaveNumber].groupbssData_size = (uint32_t)mData->groupappdynbssdataend - (uint32_t)mData->groupappdynbssdatastart;

    return MYR_DYN_INFR_SUCCESS;
}


static s32 swcIsShaveAllocationValid(DynamicContext_t *moduleData, swcShaveUnit_t shaveNumber)
{
    uint32_t i, j, perAppInstances;

    if (shaveNumber >= TOTAL_NUM_SHAVES)
        return MYR_DYN_INFR_INVALID_PARAMETERS;

    for (i=0;i<(unsigned int)GlobalContextData.DynamicContextAppsNumber;i++){

        perAppInstances = GlobalContextData.DynamicContextGlobalArray[i].module->instancesData->appInstances;
        /* we have to exclude current caller application */
        if (moduleData->entryPoint != GlobalContextData.DynamicContextGlobalArray[i].module->entryPoint)
            for(j=0;j < perAppInstances;j++)
            {
                if (GlobalContextData.DynamicContextGlobalArray[i].module->instancesData->shaveList[j] == shaveNumber)
                    /* error: shave already allocated to another app */
                    return MYR_DYN_INFR_MULTIPLE_SHAVE_ALLOCATION;
            }
    }
    return MYR_DYN_INFR_SUCCESS;
}

s32 swcDynShaveAppSetWindows(DynamicContext_t *moduleStData, u32 cmxCriticalCodeSize)
{
    if (moduleStData!=NULL){
        moduleStData->cmxCriticalCodeSize=cmxCriticalCodeSize;
        return MYR_DYN_INFR_SUCCESS;
    }else{
        return MYR_DYN_INFR_INVALID_PARAMETERS;
    }
}

s32 swcSetupDynShaveApps(DynamicContext_t *mData, const swcShaveUnit_t *svuList, const uint32_t instances)
{
    uint32_t i=0, totalInstances=0;
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status = MYR_DYN_INFR_ERROR;

#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
    MemMgrInitialize();
#endif

    DrvMutexLock(DYNCONTEXT_MUTEX_23);

    if ( (mData != NULL) && (svuList != NULL) && \
         (instances > 0) && (instances <= TOTAL_NUM_SHAVES) )
    {

        totalInstances =  mData->instancesData->appInstances + instances;

        if(totalInstances > TOTAL_NUM_SHAVES) {
            DrvMutexUnlock(DYNCONTEXT_MUTEX_23);
            return MYR_DYN_INFR_INVALID_PARAMETERS;
        } else {
            uint32_t svuListCnt = 0;

            for(i=mData->instancesData->appInstances; i < totalInstances; i++) {
                /* check if shave allocation is valid */
                status = swcIsShaveAllocationValid(mData, svuList[svuListCnt]);
                if (status == MYR_DYN_INFR_SUCCESS) {
                    mData->instancesData->shaveList[i] = svuList[svuListCnt];
                    svuListCnt++;
                } else {
                    DrvMutexUnlock(DYNCONTEXT_MUTEX_23);
                    return status;
                }
            }

            mData->instancesData->appInstances = totalInstances;

            for (i=0; i < instances; i++) {
                mData->crtContextInfo[svuList[i]].version       = __EXECUTIONCONTEXTVERSION;
                mData->crtContextInfo[svuList[i]].ctors_start   = mData->ctors_start;
                mData->crtContextInfo[svuList[i]].ctors_end     = mData->ctors_end;
                mData->crtContextInfo[svuList[i]].dtors_start   = mData->dtors_start;
                mData->crtContextInfo[svuList[i]].dtors_end     = mData->dtors_end;
                mData->crtContextInfo[svuList[i]].heap_address  = NULL;
                mData->crtContextInfo[svuList[i]].heap_size     = mData->heap_size;
                mData->crtContextInfo[svuList[i]].stack_size    = mData->stack_size;

                mData->pse[svuList[i]] = mData->groupEntryPoint;

                /* allocate needed memory for this instance */
                status = swcSetupAppDataPools(mData, svuList[i]);
                if (status != MYR_DYN_INFR_SUCCESS) {
                    //Initialize all windows to the historical default Myriad code size -> 32K
                    mData->cmxCriticalCodeSize=(u32)DEFAULT_CMX_CODE_SIZE;

                    DrvMutexUnlock(DYNCONTEXT_MUTEX_23);
                    return status;
                }
            }
        }
    } else {
        status = MYR_DYN_INFR_INVALID_PARAMETERS;
    }

    DrvMutexUnlock(DYNCONTEXT_MUTEX_23);
    return status;
}

s32 swcCleanupDynShaveApps(DynamicContext_t *mData)
{
    uint32_t i=0;
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status;

    if (mData != NULL)
    {
        if ((mData->instancesData->appInstances > 0) && (mData->instancesData->appInstances <= TOTAL_NUM_SHAVES)){
            for (i=0; i < mData->instancesData->appInstances; i++)
            {
                /* clear needed memory for this instance */
                status = swcFreeAppDataPools(mData, mData->instancesData->shaveList[i]);
                if (status != MYR_DYN_INFR_SUCCESS)
                    return status;
            }
            mData->instancesData->appInstances = 0;
        } else
            status = MYR_DYN_INFR_INVALID_SAVED_CONFIG;
    }
    else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;
    return status;
}

static void swcSetupAndStartDynApp(DynamicContext_t *moduleStData, u32 shaveNumber)
{
    swcSetShaveWindowsToDefault(shaveNumber);
    //And set data and code windows according to code size needs
    swcSetShaveWindow(shaveNumber,WINDOW_A_NO,(u32)moduleStData->appdyndataAllocAddr[shaveNumber]);
    swcSetShaveWindow(shaveNumber,WINDOW_B_NO,(u32)moduleStData->cmxCriticalCodeAllocAddr[shaveNumber]);
    swcLoadDynLibrary((u8*)moduleStData->appdyndata,shaveNumber, SHVXDATA);
    /*0x1Exxxxxx window needs to be set to the module group address before we load data*/
    swcSetShaveWindow(shaveNumber,WINDOW_C_NO,(u32)moduleStData->instancesData->GrpDataPools[shaveNumber]);
    swcLoadDynLibrary((u8*)moduleStData->groupappdyndata,shaveNumber, SHVZDATA);
    swcSetWindowedDefaultStack(shaveNumber);
    swcDynStartShave(shaveNumber, (u32)moduleStData);
}

//This function runs algorithm on the assigned shave
s32 swcRunShaveAlgoOnAssignedShave(DynamicContext_t *moduleStData, u32 shaveNumber)
{
    //Check that the user has called this with a valid shave
    if (swcCheckFreeAndValidShave(moduleStData,shaveNumber)==0){
        return MYR_DYN_INFR_SHAVE_BUSY_OR_INVALID;
    }
    swcResetShave(shaveNumber);
    swcSetupAndStartDynApp(moduleStData,shaveNumber);
    return MYR_DYN_INFR_SUCCESS;
}

//This function runs algorithm on the assigned shave
s32 swcRunShaveAlgoOnAssignedShaveCC(DynamicContext_t *moduleStData, u32 shaveNumber, const char *fmt, ...)
{
    va_list a_list;
    va_start(a_list, fmt);
    //Check that the user has called this with a valid shave
    if (swcCheckFreeAndValidShave(moduleStData,shaveNumber) == 0){
        return MYR_DYN_INFR_SHAVE_BUSY_OR_INVALID;
    }
    swcResetShave(shaveNumber);
    swcSetRegsCC(shaveNumber, fmt, a_list);
    swcSetupAndStartDynApp(moduleStData,shaveNumber);
    va_end(a_list);
    return MYR_DYN_INFR_SUCCESS;
}

s32 swcRunShaveAlgo(DynamicContext_t *moduleStData, int * const shaveNumber)
{
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status = MYR_DYN_INFR_SUCCESS;

    if (shaveNumber != NULL)
    {
        *shaveNumber = swcDynFindFreeShave(moduleStData);
        if ( (*shaveNumber >= 0) && (*shaveNumber < TOTAL_NUM_SHAVES)) {
            swcResetShave(*shaveNumber);
            swcSetupAndStartDynApp(moduleStData, *shaveNumber);
        }
        else
            status = MYR_DYN_INFR_RUNALGO_SHAVES_BUSY;
    } else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;

    return status;
}

s32 swcRunShaveAlgoCC(DynamicContext_t *moduleStData, int * const shaveNumber, const char *fmt, ...)
{
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status = MYR_DYN_INFR_SUCCESS;
    va_list a_list;
    va_start(a_list, fmt);

    if (shaveNumber != NULL)
    {
        *shaveNumber = swcDynFindFreeShave(moduleStData);
        if ( (*shaveNumber >= 0) && (*shaveNumber < TOTAL_NUM_SHAVES)){
            swcResetShave(*shaveNumber);
            swcSetRegsCC(*shaveNumber, fmt, a_list);
            swcSetupAndStartDynApp(moduleStData, *shaveNumber);
        }
        else
            status = MYR_DYN_INFR_RUNALGO_SHAVES_BUSY;
    } else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;

    va_end(a_list);
    return status;
}

s32 swcRequestUnallocatedShaves(swcShaveUnit_t *svuList, u32 shavesNumber)
{
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status = MYR_DYN_INFR_SUCCESS;
    uint32_t perAppInstances, totalNumberOfShavesAssigned = 0;
    uint8_t shaveFreeArr[TOTAL_NUM_SHAVES];

    memset(shaveFreeArr, (uint8_t)0, TOTAL_NUM_SHAVES);

    if((svuList != NULL) && (shavesNumber <= TOTAL_NUM_SHAVES) && (shavesNumber > 0)){

        DrvMutexLock(DYNCONTEXT_MUTEX_23);

        // find all used shaves by the applications
        for(uint32_t i=0; i < GlobalContextData.DynamicContextAppsNumber; i++){
            perAppInstances = GlobalContextData.DynamicContextGlobalArray[i].module->instancesData->appInstances;

            for(uint32_t j=0; j < perAppInstances; j++){
                shaveFreeArr[GlobalContextData.DynamicContextGlobalArray[i].module->instancesData->shaveList[j]] = SHAVE_ASSIGNED;
            }
        }

        // assign the number of requested shaves to the application
        for(uint32_t i=0; (i < TOTAL_NUM_SHAVES) && (totalNumberOfShavesAssigned < shavesNumber); i++){
            if((shaveFreeArr[i] == SHAVE_NOT_ASSIGNED) && (freeShavesGlobalArray[i] == SHAVE_NOT_ASSIGNED)){
                svuList[totalNumberOfShavesAssigned] = i;
                freeShavesGlobalArray[i] = SHAVE_ASSIGNED;
                totalNumberOfShavesAssigned++;
            }
        }

        // set return error code if the shaves number requested was not found
        if(totalNumberOfShavesAssigned != shavesNumber){
            status = MYR_DYN_INFR_NUMBER_OF_FREE_SHAVE_NOT_AVAILABLE;
        }

        DrvMutexUnlock(DYNCONTEXT_MUTEX_23);
    } else {
        status = MYR_DYN_INFR_INVALID_PARAMETERS;

    }

    return status;
}

s32 swcGetUnallocatedShavesNumber(void)
{
    uint32_t i, unallocatedShvCnt=0;

    for(i=0; i < TOTAL_NUM_SHAVES; i++){
        if(freeShavesGlobalArray[i] == SHAVE_NOT_ASSIGNED){
            unallocatedShvCnt++;
        }
    }

    return unallocatedShvCnt;
}

s32 swcCleanupDynShaveListApps(DynamicContext_t *mData, swcShaveUnit_t *svuList, uint32_t elementsNumber)
{
    uint32_t i, j, firstShavePosition = INVALID_SHAVE;
    int shavePosition;

    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status;

    if ((mData != NULL) && (svuList != NULL)) {
        if ((mData->instancesData->appInstances > 0) && (mData->instancesData->appInstances <= TOTAL_NUM_SHAVES)) {
            DrvMutexLock(DYNCONTEXT_MUTEX_23);

            for (i=0; i < elementsNumber; i++) {
                shavePosition = -1;

                for(j=0; (j < mData->instancesData->appInstances) && (shavePosition==-1); j++) {
                    if(mData->instancesData->shaveList[j] == svuList[i]) {
                        shavePosition = j;
                    }
                }

                if(shavePosition != -1) {
                    /* clear needed memory for this instance */
                    status = swcFreeAppDataPools(mData, mData->instancesData->shaveList[shavePosition]);
                    if (status != MYR_DYN_INFR_SUCCESS) {
                        return status;
                    }
                } else {
                    return MYR_DYN_INFR_SHAVE_NOT_FOUND_IN_CONTEXT_LIST;
                }
            }

            /*find the position of the first shave from svuList into the DynamicContext list*/
            for(i=0; (i < mData->instancesData->appInstances) && (firstShavePosition == INVALID_SHAVE); i++) {
                if(mData->instancesData->shaveList[i] == svuList[0]) {
                    firstShavePosition = i;
                }
            }

            /*copy the shaves after the deleted list*/
            if(firstShavePosition != INVALID_SHAVE) {
                while((firstShavePosition+elementsNumber) < mData->instancesData->appInstances) {
                    mData->instancesData->shaveList[firstShavePosition] = mData->instancesData->shaveList[firstShavePosition+elementsNumber];
                    firstShavePosition++;
                }
                mData->instancesData->appInstances -= elementsNumber;

                for(i=0; i < elementsNumber; i++) {
                    freeShavesGlobalArray[svuList[i]] = SHAVE_NOT_ASSIGNED;
                }
            } else {
                status = MYR_DYN_INFR_INVALID_PARAMETERS;
            }

            DrvMutexUnlock(DYNCONTEXT_MUTEX_23);
        } else
            status = MYR_DYN_INFR_INVALID_SAVED_CONFIG;
    }
    else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;

    return status;
}

void swcSetNewHeapLocation(DynamicContext_t *mData, unsigned char * newAddress, swcShaveUnit_t shaveNumber)
{
	if(mData->instancesData->HeapPools[shaveNumber] != NULL) {
#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
		MemMgrFree(mData->instancesData->HeapPools[shaveNumber]);
#else
        free(mData->instancesData->HeapPools[shaveNumber]);
        free(mData->instancesData->HeapPoolsStart[shaveNumber]);
#endif
	}
	mData->instancesData->HeapPools[shaveNumber] = newAddress;
}

void swcSetNewAppDynDataLocation(DynamicContext_t *mData, unsigned char * newAddress, swcShaveUnit_t shaveNumber)
{
	if(mData->appdyndataAllocAddr[shaveNumber] != NULL) {
#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
		MemMgrFree(mData->appdyndataAllocAddr[shaveNumber]);
#endif
    }
	mData->appdyndataAllocAddr[shaveNumber] = newAddress;
}

void swcSetGrpDynDataLocation(DynamicContext_t *mData, unsigned char * newAddress, swcShaveUnit_t shaveNumber)
{
    if (mData->instancesData->GrpDataPools[shaveNumber] != NULL) {
#ifdef SHAVE_APP_ENABLE_MEMORY_MANAGER_ALLOCATION
        MemMgrFree(mData->instancesData->GrpDataPools[shaveNumber]);
#else
        free(mData->instancesData->GrpDataPools[shaveNumber]);
        free(mData->instancesData->GrpDataPoolsStart[shaveNumber]);
#endif
    }
    mData->instancesData->GrpDataPools[shaveNumber] = newAddress;
}

static int* findApplicationEntryPoint(DynamicContext_t *moduleStData, const char* fName)
{
    // The IAT names table contains a NULL pointer as the final entry, so iterate
    // through the table until either NULL is reached or the entry-point is found
    for (int i = 0; ((unsigned int *) moduleStData->iatnames)[i]; i++) {
        int comparison = strcmp((char*) ((unsigned int *) moduleStData->iatnames)[i], fName);

        // The IAT names lookup table is sorted alphabetically by entry-point name,
        // so if the comparison result is greater-than zero, the entry-point does not
        // exist on this SHAVE
        if (comparison > 0)
            break;
        else if (comparison == 0)
            //  Found the entry-point, so return to the caller
            return (int*) ((int*) moduleStData->iat)[i];
    }

    //  Not found
    return NULL;
}

static int* findGroupEntryPoint(DynamicContext_t *moduleStData, const char* fName)
{
    // The IAT names table contains a NULL pointer as the final entry, so iterate
    // through the table until either NULL is reached or the entry-point is found
    for (int i = 0; ((unsigned int *) moduleStData->iatnames_group)[i]; i++) {
        int comparison = strcmp((char*) ((unsigned int *) moduleStData->iatnames_group)[i], fName);

        // The IAT names lookup table is sorted alphabetically by entry-point name,
        // so if the comparison result is greater-than zero, the entry-point does not
        // exist on this SHAVE
        if (comparison > 0)
            break;
        else if (comparison == 0)
            //  Found the entry-point, so return to the caller
            return (int*) ((int*) moduleStData->iat_group)[i];
    }

    //  Not found
    return NULL;
}

int swcIsoSetupShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType)
{
    int status;

    if ((moduleStData != NULL) && (svuList != NULL) && (shavesNumber < TOTAL_NUM_SHAVES)) {
        status = swcSetupDynShaveApps(moduleStData, svuList, shavesNumber);
        if (status != MYR_DYN_INFR_SUCCESS)
            return status;

        for (uint32_t i = 0; i < shavesNumber; i++) {
            /*0x1Exxxxxx window needs to be set to the module group address before we load data*/
            swcSetShaveWindow(svuList[i], WINDOW_C_NO, (u32) moduleStData->instancesData->GrpDataPools[svuList[i]]);
            //And set data and code windows according to code size needs
            swcSetShaveWindow(svuList[i],WINDOW_A_NO,(u32)moduleStData->appdyndataAllocAddr[svuList[i]]);
            swcSetShaveWindow(svuList[i],WINDOW_B_NO,(u32)moduleStData->cmxCriticalCodeAllocAddr[svuList[i]]);

            //Load the application data
            swcLoadDynLibrary((u8*) moduleStData->appdyndata, svuList[i], SHVXDATA);
            //Load the group data
            swcLoadDynLibrary((u8*) moduleStData->groupappdyndata, svuList[i], SHVZDATA);

            swcSetWindowedDefaultStack(svuList[i]);
        }

        if (paradigmType == MISA_DECOUPLED) {
            int *crtinit = findGroupEntryPoint(moduleStData, "_EP_crtinit");
            if (crtinit == NULL)
                return MYR_DYN_INFR_FUNCTION_NOT_FOUND;

            for(uint32_t i = 0; i < shavesNumber; i++){
                moduleStData->pse[svuList[i]] = (int)crtinit;
                swcDynStartShave(svuList[i], (u32) moduleStData);
            }

            status = swcWaitShaves(shavesNumber, svuList);
        }
    } else
        return MYR_DYN_INFR_INVALID_PARAMETERS;

    return status;
}

int swcStartEntryPointDC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* fName)
{
    if ((moduleStData != NULL) && (fName != NULL) && (shaveNumber < TOTAL_NUM_SHAVES)) {
        int *entry = findApplicationEntryPoint(moduleStData, fName);
        if (entry == NULL)
            return MYR_DYN_INFR_FUNCTION_NOT_FOUND;

        moduleStData->pse[shaveNumber] = (int)entry;

        swcDynStartShave(shaveNumber, (u32) moduleStData);
    } else
        return MYR_DYN_INFR_INVALID_PARAMETERS;

    return MYR_DYN_INFR_SUCCESS;
}

int swcStartEntryPointDCCC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* fName, const char *fmt, ...)
{
    MYRIAD_DYNAMIC_INFRASTR_STATUS_CODE status = MYR_DYN_INFR_SUCCESS;
    va_list a_list;
    va_start(a_list, fmt);

    if ((moduleStData != NULL) && (fName != NULL) && (shaveNumber < TOTAL_NUM_SHAVES)) {
        int *entry = findApplicationEntryPoint(moduleStData, fName);

        if (entry != NULL) {
            moduleStData->pse[shaveNumber] = (int)entry;
            swcSetRegsCC(shaveNumber, fmt, a_list);
            swcDynStartShave(shaveNumber, (u32) moduleStData);
        } else
            status = MYR_DYN_INFR_FUNCTION_NOT_FOUND;
    } else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;

    va_end(a_list);
    return status;
}

int swcStartFC(DynamicContext_t *moduleStData, uint32_t shaveNumber)
{
    if ((moduleStData != NULL) && (shaveNumber < TOTAL_NUM_SHAVES)) {
        int *mainEntry = findGroupEntryPoint(moduleStData, "_EP_start");
        if (mainEntry == NULL)
            return MYR_DYN_INFR_FUNCTION_NOT_FOUND;

        moduleStData->pse[shaveNumber] = (int)mainEntry;

        swcDynStartShave(shaveNumber, (u32) moduleStData);
    } else
        return MYR_DYN_INFR_INVALID_PARAMETERS;

    return MYR_DYN_INFR_SUCCESS;
}

int swcIsoCleanShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType)
{
    int status;

    if ((moduleStData != NULL) && (svuList != NULL) && (shavesNumber < TOTAL_NUM_SHAVES)) {
        status = swcCleanupDynShaveListApps(moduleStData, svuList, shavesNumber);
        if (status != MYR_DYN_INFR_SUCCESS)
            return status;

        if (paradigmType == MISA_DECOUPLED) {
            int *crtfini = findGroupEntryPoint(moduleStData, "_EP_crtfini");
            if (crtfini == NULL)
                return MYR_DYN_INFR_FUNCTION_NOT_FOUND;

            for (uint32_t i = 0; i < shavesNumber; i++) {
                moduleStData->pse[svuList[i]] = (int) crtfini;
                swcDynStartShave(svuList[i], (u32) moduleStData);
            }

            status = swcWaitShaves(shavesNumber, svuList);
        }
    } else
        status = MYR_DYN_INFR_INVALID_PARAMETERS;

    return status;
}
