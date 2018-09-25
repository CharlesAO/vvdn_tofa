// ***************************************************************************
// Copyright (C) 2011 Movidius Ltd. All rights reserved
// ---------------------------------------------------------------------------
// File       : moviDebugDll.h
// Description: moviDebug DLL API header
// ---------------------------------------------------------------------------
// HISTORY
// Version        | Date       | Owner         | Purpose
// ---------------------------------------------------------------------------
// 1.0            | 15.08.2008 | Cristian Cuna | Initial version
// ***************************************************************************
//#define COMAND_LINE_LENGTH 1024
//#include "moviDebugTypes.h"

#ifdef __cplusplus
#include <string>
using namespace std;
#endif
//
#ifdef _WIN32
#   ifdef MOVIDEBUG_BUILD_DLL
#       define MOVIDEBUG_DLLAPI __declspec(dllexport)
#   else
#       define MOVIDEBUG_DLLAPI __declspec(dllimport)
#   endif
#else
#if __GNUC__ >= 4
#   define MOVIDEBUG_DLLAPI __attribute__ ((visibility ("default")))
#else
#   define MOVIDEBUG_DLLAPI
#endif
#endif

#define MAX_STR_SIZE          2048
#define LAST_OUTPUT_SIZE (40 * MAX_STR_SIZE)

//
typedef enum DllDebugInterface
{
    DLL_DBGI_NONE = 0,                //No interface selected
    DLL_DBGI_JTAG = 1,                //JTAG interface (Olimex JTAG key)
    DLL_DBGI_UART = 2,                //UART interface (Not recommended)
    DLL_DBGI_MSIM = 4                 //MSIM interface (moviSim must be started with '-tcpip' command line switch
} DllDebugInterface;

struct MoviDebugDllOption
{
    const char *name;
    const char *value;
};

#define MOVIDEBUG_OPT_CHIP_VERSION "cv"
#define MOVIDEBUG_OPT_INIT_CHIP "initChip"
#define MOVIDEBUG_OPT_SYS_RESET "sysReset"
#define MOVIDEBUG_OPT_WORKING_DIRECTORY "workingDirectory"
#define MOVIDEBUG_OPT_UART_POLLING "uartPollingEnabled"
#define MOVIDEBUG_OPT_OSC_IN0_KHZ "oscIn0Khz"
#define MOVIDEBUG_OPT_OSC_IN1_KHZ "oscIn1Khz"

#define MOVIDEBUG_OPT_YES "yes"
#define MOVIDEBUG_OPT_NO "no"

#ifdef __cplusplus
#define MOVIDEBUG_ARGUMENT_DEFAULT_VALUE(value) = value
#define MOVIDEBUG_BOOL bool
#define MOVIDEBUG_TRUE true
#define MOVIDEBUG_FALSE false
extern "C" {
#else
#define MOVIDEBUG_ARGUMENT_DEFAULT_VALUE(value)
#define MOVIDEBUG_BOOL unsigned char
#define MOVIDEBUG_TRUE 1
#define MOVIDEBUG_FALSE 0
#endif

#define MOVIDEBUG_TARGETS_MYRIAD_1VX_FLAG 0x00000000
#define MOVIDEBUG_TARGETS_MYRIAD_2V1_FLAG 0x80000000
#define MOVIDEBUG_TARGETS_MYRIAD_2V2_FLAG 0xA0000000

MOVIDEBUG_DLLAPI MOVIDEBUG_BOOL DllMoviDebugInitLibrary(
    int targets,
    const char *workingDirectory,
    MOVIDEBUG_BOOL noInitChip,
    unsigned int jtagFrequency,
    const char *moviDebugServerIP,
    const char *moviDebugServerPort,
    MOVIDEBUG_BOOL allowUartPolling MOVIDEBUG_ARGUMENT_DEFAULT_VALUE(MOVIDEBUG_TRUE)
);

MOVIDEBUG_DLLAPI MOVIDEBUG_BOOL DllMoviDebugInitLibraryOpt(
    int targets,
    const char *moviDebugServerIP,
    const char *moviDebugServerPort,
    const MoviDebugDllOption * options MOVIDEBUG_ARGUMENT_DEFAULT_VALUE(0),
    unsigned optionCount MOVIDEBUG_ARGUMENT_DEFAULT_VALUE(0)
);


MOVIDEBUG_DLLAPI bool  DllMoviDebugTerminateSabreStateThread(); // DEPRECATED

MOVIDEBUG_DLLAPI void  DllMoviDebugCloseLibrary();
MOVIDEBUG_DLLAPI void  DllMoviDebugDisconnect(int targets);
MOVIDEBUG_DLLAPI void  DllMoviDebugGetVersionString(char *versionString);
MOVIDEBUG_DLLAPI void  DllMoviDebugGetCurrentTargetName(char *targetName);
MOVIDEBUG_DLLAPI void  DllMoviDebugSetCurrentTarget(int target, int board);
MOVIDEBUG_DLLAPI int   DllMoviDebugParseString(char *inputLine);

/// @brief Get value
/// @param address Source address to read from, must be word-aligned
/// @param transferSize Number of bytes, maximum 4
/// @param retValue poiter to destination word.
MOVIDEBUG_DLLAPI void  DllMoviDebugGet(unsigned int address, unsigned int transferSize, unsigned int *retValue);

/// @brief Set value
/// @param address Destinat address to write to, must be word-aligned
/// @param value Value to write.
/// @param transferSize Number of bytes, maximum 4
MOVIDEBUG_DLLAPI void  DllMoviDebugSet(unsigned int address, unsigned int value, unsigned int transferSize);

/// @brief Send burst read message
/// @param address Should be word-aligned
/// @param data pointer to beginning of array of 32-bit words
/// @param wordCount the number of 32-bit words to get
MOVIDEBUG_DLLAPI void  DllMoviDebugGetBurst(unsigned address, unsigned int *data, unsigned wordCount);

/// @brief Send burst write message
/// @param address Should be word-aligned
/// @param data pointer to beginning of array of 32-bit words
/// @param wordCount the number of 32-bit words to set
MOVIDEBUG_DLLAPI void  DllMoviDebugSetBurst(unsigned address, const unsigned int *data, unsigned wordCount);

/// @param address Source address (32-bit, no alignment requirements)
/// @param data Pointer to destination
/// @param size Size in bytes
MOVIDEBUG_DLLAPI void  DllMoviDebugGetMemBlock(unsigned address, void *data, unsigned size);

/// @param address Destination address (32-bit, no alignment requirements)
/// @param data Pointer to source
/// @param size Size in bytes
MOVIDEBUG_DLLAPI void  DllMoviDebugSetMemBlock(unsigned address, const void *data, unsigned size);

MOVIDEBUG_DLLAPI int   DllMoviDebugGetAddress(const char *registerName);
MOVIDEBUG_DLLAPI int   DllMoviDebugGetLine(unsigned int core, unsigned int address, char *fileName, unsigned int *lineNumber);
MOVIDEBUG_DLLAPI int   DllMoviDebugCoreStep(int core, unsigned int numberOfSteps);
MOVIDEBUG_DLLAPI void  DllMoviDebugSetDebugMaster(int core);
MOVIDEBUG_DLLAPI void  DllMoviDebugUnsetDebugMaster();

//executionHistory[CORE_NUMBER]
//        coreNumber = { SHAVE 0, ...., Leon }
MOVIDEBUG_DLLAPI void  DllMoviDebugGetExecutionHistory(int coreNumber, unsigned int *historyAddresses, char *opcodeStrings, char *mnemonicsStrings);
MOVIDEBUG_DLLAPI bool  DllMoviDebugLeonDisassemble(unsigned int opcode, unsigned int base, char *retString, unsigned int ip);
MOVIDEBUG_DLLAPI int   DllMoviDebugShaveDisassemble(char *inputHexStream, char *outputTextBuffer, int *numberOfDasmErrors, int maxNumberOfBytes);

#ifdef __cplusplus
}

MOVIDEBUG_DLLAPI std::string DLLMoviDebugGetVariableValue(char *inputVariableName, unsigned int target, unsigned int numBase);

extern "C" {
#endif
MOVIDEBUG_DLLAPI void  DLLMoviDebugSetVariableValue(char *inputVariableName, unsigned int inputVariableValue, unsigned int target);
MOVIDEBUG_DLLAPI MOVIDEBUG_BOOL DLLMoviDebugGetAvailableVariables(char *fileName, unsigned int lineNumber, char *listOfVariables);
MOVIDEBUG_DLLAPI MOVIDEBUG_BOOL DLLMoviDebugGetFunctionIP(int core, const char *functionName, int *lowPC, int *highPC);
MOVIDEBUG_DLLAPI MOVIDEBUG_BOOL DLLMoviDebugGetFunctionName(int core, unsigned int IP, char *functionName, unsigned int *functionAddress);

MOVIDEBUG_DLLAPI int DLLMoviDebugGetLastMessage(char * message, unsigned bufferSize);
MOVIDEBUG_DLLAPI unsigned DLLMoviDebugGetLastMessageLength();

#ifdef __cplusplus
}
#endif

