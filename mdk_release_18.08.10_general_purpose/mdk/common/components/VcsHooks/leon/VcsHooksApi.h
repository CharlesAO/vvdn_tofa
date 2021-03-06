///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup VcsHooksApi VCS Test Environment API
/// @ingroup  VcsHooksApi
/// @{
/// @brief     Set of functions to allow test case interaction with the VCS Test Environment
///
/// This module allows the use of optimised routines which speed up simulation of test cases.
///

#ifndef _VCS_HOOKS_API_H_
#define _VCS_HOOKS_API_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "VcsHooksApiDefines.h"
#include "DrvRegUtilsDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------


// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// @brief Quickly display a single 32 bit unsigned value in the VCS output console
/// @param[in] value - 32 bit value to print
/// @return  void
///
void printInt(u32 value);

/// @brief Quickly display a msg followed by a 32 bit value
/// @param[in] msg   - string containing message to display (without \n)
/// @param[in] value - 32 bit value to print
/// @return  void
///
void printMsgInt(const char * msg,u32 value);


/// @brief This function forces the AHB monitor register debug_test_state to a specific value
///
/// This function can be used to make it quicker to find a point of interest in waves.
/// The user simply calls the fuction with a value and then monitors:
/// tc_fragrak.leon_ahb_monitor_i.debug_test_state in dve.
/// @param[in] value - 32 bit value to use
/// @return  void
///
void testStateSet(u32 value);

/// @brief This function increments the AHB monitor register debug_test_state to a specific value
///
/// This function can be used to make it quicker to find a point of interest in waves.
/// In cases where multiple tests are run, the user can call this function to increment the
/// debug counter (which starts from 0). The debug_test_state variable can be monitored here:
/// tc_fragrak.leon_ahb_monitor_i.debug_test_state in dve.
///
/// Input: 32 bit value to use
/// @return  void
///
void testStateInc(void);


/// @brief This function adds a value to the AHB monitor register debug_test_state to a specific value
///
/// This function can be used to make it quicker to find a point of interest in waves.
/// In cases where multiple tests are run, the user can call this function to add to the
/// debug counter (which starts from 0). The debug_test_state variable can be monitored here:
/// tc_fragrak.leon_ahb_monitor_i.debug_test_state in dve.
/// @param[in] value - 32 bit value to use
/// @return  void
///
void testStateAdd(u32 value);

/// @brief This function does a dump to screen of the contents of a section of CMX memory
///
/// The results will be seen in the vcs run log and the memory will be displayed
/// in its native 128 bit format from: addr -> [127..0]
/// @note The address must be aligned to a 16 byte boundary
/// @param[in] address - start address in RAM
/// @param[in] length  - length of section to display
/// @return  void
///
void displayRawMemory(void * address, u32 length);

/// @brief This function does a dump to screen the contents of a CMX memory range
///
/// The output is address followed by value.
/// e.g.:
/// Addr: 7020e900 Val: 00000000000000000000000000000000
///
/// The dump will be in little endian byte order as per system endianness.
///
/// @note This function needs to be updated to add DDR support.
/// @param[in] address - start address in CMX must have 16 byte alignment
/// @param[in] length  - length of section to display
/// @return  void
///
void dumpMemoryToFile(u32 address, u32 length);

///@brief  This function does a dump to a file the contents of a memory range.
///
/// The output file is written in the simulation folder.
///
/// File name is 3 digit index_address_len_LE.bin
/// e.g.:
/// 001_0xA0000000_0x00000008_LE.bin on first call
/// 002_0xA0000000_0x00000008_LE.bin on second call
/// etc.
///
/// The dump will be in little endian byte order as per system endianness.
///
/// @note The file will be in little endian byte order as per system endianness
/// @param[in] address  - start address in CMX
/// @param[in] length   - length of section to display
/// @param[in] fileName - file name
/// @return  void
///
void saveMemoryToFile(u32 address, u32 length, const char *fileName);

/// @brief This function loads some of all of a binary file into memory
///
/// The input file path is relative to simulation folder.
///
/// For a less complex version of this see @ref loadMemFromFileSimple.
/// @par Modes of operation of this API:
/// 1. Fully qualified filename and path given
///      - This mode means optIndex = 0
///      - file loaded is exactly as per the string pFileNameOpt
/// 2. Filename base + index
///      - This mode means optIndex > 0
///      - File loaded is built from sprintf(filename,"%s_%05d.bin",pFileNameOpt,optIndex)
///      - Note: In this chase pFileNameOpt only represents the base of the filename
/// 3. No Filename string passed and optIndex = 0
///      - This mode means pFileNameOpt = 0
///      - Filename is hardcoded to vector/vector_in.bin
/// 4. No Filename string passed and optIndex > 0
///      - This mode means pFileNameOpt = 0
///      - Filename is hardcoded to sprintf(filename,"vector/vector_%05d.bin",optIndex)
///
/// @note This function needs to be updated to add DDR support.
/// @param[in] pFileNameOpt      -- See above for description
/// @param[in] optIndex          -- See above for description
/// @param[in] fileOffset        -- if this parameter is non zero the load occurs from fileOffset within the target file
/// @param[in] bytesToLoad       -- Number of bytes to load from file into memory. If this param is 0 then the whole file is loaded
/// @param[in] targetLoadAddress -- Address in memory where the memory should be loaded
/// @return  void
///
void loadMemFromFile(char * pFileNameOpt, u32 optIndex, u32 fileOffset, u32 bytesToLoad, void * targetLoadAddress);



/// @brief Simple function to load memory from file
///
/// The input file path is relative to simulation folder.
/// see @ref loadMemFromFile for more options
///
/// @note This function needs to be updated to add DDR support.
/// @param[in] filename          -- name of file to be loaded
/// @param[in] bytesToLoad       -- Number of bytes to load from file into memory. If this param is 0 then the whole file is loaded
/// @param[in] targetLoadAddress -- Address in memory where the memory should be loaded
/// @return  void
///
static inline void loadMemFromFileSimple(char * filename, u32 bytesToLoad, void * targetLoadAddress)
{
    loadMemFromFile(filename,0,0,bytesToLoad,targetLoadAddress);
}


/// This function performs a fast memory copy in verilog
///
/// Operates on a byte basis. There are no alignment requirements
/// @note Beware when using this function during profiling as
/// this function behaves like an instant memory copy and takes no time.
/// It is only intendend as a performnace boost during test development and
/// should be replaced with DMA operations when tests are running to be representative
/// of how the chip will really operate.
/// @note This function operates on the native memory. The user must ensure cache coherency.
/// @param[in] dst    - Address of destination buffer in RAM
/// @param[in] src    - Address of source buffer in RAM
/// @param[in] length - size of copy in bytes
/// @return  void
///
void vcsHookFastMemCpy(void * dst, void * src, u32 length);


/// This function performs a fast memory set in verilog
///
/// Operates on a byte basis. There are no alignment requirements
/// @note Beware when using this function during profiling as
/// this function behaves like an instant memory set and takes no time.
/// @note This function operates on the native memory. The user must ensure cache coherency.
/// @param[in] dst    - Address of destination buffer in RAM
/// @param[in] value  - Value to set each byte to
/// @param[in] length - Number of bytes to set
/// @return  void
///
void vcsHookFastMemSet(void * dst, u32 value, u32 length);


/// @brief Trigger a verilog event (e.g. Power monitor)
///
/// Singals verilog testbench using an event code
/// @param[in] eventCode - (e.g. VCS_HOOK_EVENT_POWER_MONITOR )
/// @return  void
///
void vcsHookVerilogEventTrigger(u32 eventCode);

/// @brief Fast version of puts
///
/// Uses direct access to memory to avoid
/// the need for leon looping through every byte
/// @note The reason this isn't a direct replacement for puts
/// is that the user needs to be very careful about cache-coherency.
/// This issue means it is not a good fit for general purpose use.
/// @note Specialist use only
/// @param[in] pString - Address of string to be printed in memory
/// @return  void
///
void vcsFastPuts(char * pString);

/// @brief This function implements a mechanism by which messages can be passed from software to the VCS hardware SOC simulator.
/// @note On real silicon calls to this function have no effect.
/// @return void
///
void vcsHookFunctionCallParam6(u32 function,u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);

static inline void vcsHookFunctionCallParam4(u32 function,u32 param1, u32 param2, u32 param3, u32 param4)
{
    vcsHookFunctionCallParam6(function,param1,param2,param3,param4,0,0);
}


/// @}
#ifdef __cplusplus
}
#endif


#endif // _VCS_HOOKS_API_H_
