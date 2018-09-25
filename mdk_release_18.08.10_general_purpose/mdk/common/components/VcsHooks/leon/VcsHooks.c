///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Set of functions to allow test case interaction with the VCS Test Environment
///
/// This module allows the use of optimised routines which speed up simulation of test cases.
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <VcsHooksApi.h>
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <swcLeonUtils.h>
#include <stdio.h>
#include <string.h>
#include <swcWhoAmI.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
// This is a local copy of the debug_test_state hardware register
static u32 cache_debug_test_state = 0;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void printInt(u32 value)
{
    tyHwPlatform platformType;
    platformType = GET_PLATFORM_TYPE;
    if ((platformType == PLATFORM_VCS) || (platformType == PLATFORM_MOVISIM) )
    {
        SET_REG_WORD((u32)(ACT_DATA_ADR),value);
    }
    else
    {
        prefixed_printf("Actual Data   =   0x%08lx\n", value);
    }
}

void printMsgInt(const char * msg,u32 value)
{
    tyHwPlatform platformType;
    platformType = GET_PLATFORM_TYPE;
    if ((platformType == PLATFORM_VCS) || (platformType == PLATFORM_MOVISIM) )
    {
        vcsFastPuts((char *)msg);
        SET_REG_WORD((u32)(DEBUG_MSG_INT),value);
    }
    else
    {
        prefixed_printf("%s 0x%08lx\n",msg,value);
    }
}

void displayRawMemory(void * address, u32 length)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_MEM_DISPLAY,(u32)address, length, length, 0);
    }
    else
    {
        #ifndef NO_PRINT
        if ((length % 16) != 0)
        {
            length = (length / 16 + 1) * 16;
        }
        for (;length >= 16;length-=16)
        {
            u32 d0 = GET_REG_WORD_VAL(address);
            u32 d1 = GET_REG_WORD_VAL(((char *)address)+4);
            u32 d2 = GET_REG_WORD_VAL(((char *)address)+8);
            u32 d3 = GET_REG_WORD_VAL(((char *)address)+12);

            prefixed_printf("Addr: %08x Val: %08lx%08lx%08lx%08lx\n", (int)address, d3, d2, d1, d0);
            address = ((char *)address) + 12;
        }
        #endif
    }
}

void vcsHookFastMemCpy(void * dst, void * src, u32 length)
{
    tyHwPlatform platformType;
    platformType = GET_PLATFORM_TYPE;
    if ((platformType == PLATFORM_VCS) || (platformType == PLATFORM_MOVISIM) )
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_MEM_CPY,(u32) dst, (u32) src, length, 0);
    }
    else
    {
        swcLeonMemCpy(dst, le_pointer, src, le_pointer, length);
    }
}

void vcsHookFastMemSet(void * address, const u32 value, u32 length)
{
    tyHwPlatform platformType;
    platformType = GET_PLATFORM_TYPE;
    if ((platformType == PLATFORM_VCS) || (platformType == PLATFORM_MOVISIM) )
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_MEM_SET,(u32) address, value, length, 0);
    }
    else
    {
        memset(address, value, length);
    }
}

#pragma weak dumpMemoryToFile
void dumpMemoryToFile(u32 address, u32 length)
{
    // filename is auto-generated NNN_0xXADDRESS_0xXLENGTHX_LE.bin
    vcsHookFunctionCallParam4(VCS_HOOK_FUNC_MEM_FILE_DUMP,address, length, length, 0);
}

#pragma weak saveMemoryToFile
void saveMemoryToFile(u32 address, u32 length, const char *fileName)
{
    vcsHookFunctionCallParam4(VCS_HOOK_FUNC_MEM_FILE_DUMP,address, length, length, (u32)fileName);
}

#pragma weak loadMemFromFile
void loadMemFromFile(char * pFileNameOpt, u32 optIndex, u32 fileOffset, u32 bytesToLoad, void * targetLoadAddress)
{
    // this function can't be emulated by software, so this is hook-only:
    vcsHookFunctionCallParam6(VCS_HOOK_FUNC_MEM_FILE_LOAD,(u32)pFileNameOpt, optIndex, fileOffset, bytesToLoad, (u32) targetLoadAddress,0);
}

void vcsFastPuts(char * pString)
{
    if (!VCSHOOKS_USE_SOFTWARE)
    {
        vcsHookFunctionCallParam4(VCS_HOOK_FUNC_FAST_PUTS,(u32)pString,0,0,0);
    }
    else
    {
        prefixed_printf("%s\n", pString);
    }
}

void testStateSet(u32 value)
{
    cache_debug_test_state = value;
    tyHwPlatform platformType;
    platformType = GET_PLATFORM_TYPE;
    if ((platformType == PLATFORM_VCS) || (platformType == PLATFORM_MOVISIM) )
    {
        SET_REG_WORD(DEBUG_TEST_STATE,cache_debug_test_state);
    }
    else
    {
        prefixed_printf("TEST_STATE -> %08lx\n", cache_debug_test_state);
    }
}

void vcsHookVerilogEventTrigger(u32 eventCode)
{
    // this function can't be emulated by software, so this is hook-only:
    vcsHookFunctionCallParam4(VCS_HOOK_FUNC_VERILOG_TRIGGER,eventCode, 0 , 0 , 0);
}

void testStateInc(void)
{
    tyHwPlatform platformType;
    cache_debug_test_state++;

    platformType = GET_PLATFORM_TYPE;
    if ((platformType == PLATFORM_VCS) || (platformType == PLATFORM_MOVISIM) )
    {
        SET_REG_WORD(DEBUG_TEST_STATE,cache_debug_test_state);
    }
    else
    {
        prefixed_printf("TEST_STATE -> %08lx\n", cache_debug_test_state);
    }
}

void testStateAdd(u32 value)
{
    tyHwPlatform platformType;
    cache_debug_test_state += value;

    platformType = GET_PLATFORM_TYPE;
    if ((platformType == PLATFORM_VCS) || (platformType == PLATFORM_MOVISIM) )
    {
        SET_REG_WORD(DEBUG_TEST_STATE,cache_debug_test_state);
    }
    else
    {
        prefixed_printf("TEST_STATE -> %08lx\n", cache_debug_test_state);
    }
}

__attribute__ ((noinline))  void vcsHookFunctionCallParam6(u32 function,u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6)
{
    SET_REG_WORD(VCS_HOOK_PARAM1,param1);
    SET_REG_WORD(VCS_HOOK_PARAM2,param2);
    SET_REG_WORD(VCS_HOOK_PARAM3,param3);
    SET_REG_WORD(VCS_HOOK_PARAM4,param4);
    SET_REG_WORD(VCS_HOOK_PARAM5,param5);
    SET_REG_WORD(VCS_HOOK_PARAM6,param6);
    SET_REG_WORD(VCS_HOOK_FCALL,function);
}
