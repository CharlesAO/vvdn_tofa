///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup ShaveLoader Shave Loader
/// @{
/// @brief  API for the Shave Loader module
///
/// Used for executing different functionalities on SHAVEs
///

#ifndef SWC_SHAVE_LOADER_H
#define SWC_SHAVE_LOADER_H

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdarg.h>
#include "mv_types.h"
#include <DrvIcb.h>
#include <swcDmaTypes.h>
#include "DrvCommon.h"
#include "theDynContext.h"
#include "swcLeonUtils.h"

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 2.5: Global definitions
// ----------------------------------------------------------------------------

/// @name
/// Shave dummy wrappers
/// @{
#define SVU(x) x
#define IRF(x) x
#define SRF(x) x
#define VRF(x) x
/// @}

/// @brief use DDR address through L2 cache. Force it's use.
#define ADDR_DDRL2(x) (((u32)(x)) & 0xF0FFFFFF)
#define ACCEPT_ALTERNATIVE_SHAVE_START_METHOD FALSE

#ifndef SHAVE_INTERRUPT_LEVEL
#define SHAVE_INTERRUPT_LEVEL 3
#endif

typedef u32 swcShaveUnit_t;

typedef enum
{
    SHVXDATA = 0,
    SHVZDATA,
    SHVDLIB
} context_t;

#ifdef __cplusplus
extern "C" {
#endif

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// @brief Set absolute default stack for a specific shave
/// @param[in] shave_num - shave number whose stack is to be set
/// @return void
///
void swcSetAbsoluteDefaultStack(u32 shave_num);

/// @brief Allows the user to assert a stack size against which checks
/// may be implemented. This does not represent a guarantee that the system
/// will allocate this stack it only allows users to specify
/// how much space they themselves have considered and made available through
/// other means for the application. Calling this function allows the system
/// to perform checks which would detect if this size was overrun at any stage
/// @param[in] shaveNumber - shave number whose stack is to be set
/// @param[in] size - Size desired to limit one's application to
/// @return void
///
void swcStateConsideredShaveStackSize(u32 shaveNumber, u32 size);

/// @brief Reads back the stack size for a specified shave.
/// When calling either swcSetAbsoluteDefaultStack or swcSetWindowedDefaultStack
/// the stack size set to register i20 will be stored and can be read back with the help of this function.
/// @param[in] shaveNumber - shave number whose stack is to be checked
/// @return u32, stackSize - the stored stack size for the specified shave
///
u32 swcGetShaveStackSize(u32 shaveNumber);

/// @brief If stack painter was used, this function searches for the size
/// of unused stack given pattern checks
/// NOTE!: this function does nothing relevant if user did not call
/// swcStateConsideredShaveStackSize and swcStackPainter before running a shave application
/// @param[in] shaveNumber - shave number whose stack is to be checked
/// @param[in] canaryValue - canary value used for stack painting this particular shave
/// @return void
///
u32 swcGetUnusedShaveFreeStack( u32 shaveNumber, u32 canaryValue);

/// @brief Paint stack with a specific canary value. NOTE: one must have called
/// the swcStateConsideredShaveStackSize on the shaveNumber used here in advance
/// of calling this function
/// @param[in] shaveNumber - Shave number for shave to paint stack
/// @param[in] canaryValue - canary value for fill of stack area
/// @return void
///
void swcStackPainter( u32 shaveNumber, u32 canaryValue);

/// @brief Get Shave window register values
/// @param[in]  shaveNumber - shave number for which window register values are retrieved
/// @param[out] windows     - pointer to window registers
/// @return void
///
void swcGetShaveWindowRegs(u32 shaveNumber, u32* windows);

/// @brief Set a specific window register with a target window base address
/// @param[in] shave_num   - shave number for which window register will be set
/// @param[in] window_num  - window number that should be set
/// @param[in] window_addr - window address to be put in the window register
/// @return void
///
void swcSetShaveWindow(u32 shave_num, u32 window_num, u32 window_addr);

/// @brief Set each window register with the corresponding window base address
/// @param[in] shaveNumber - shave number for which window registers will be set
/// @param[in] windowA     - base address for window A
/// @param[in] windowB     - base address for window B
/// @param[in] windowC     - base address for window C
/// @param[in] windowD     - base address for window D
/// @return void
///
void swcSetShaveWindows(u32 shaveNumber, u32 windowA, u32 windowB, u32 windowC, u32 windowD);

/// @brief Reset windows to default values in case they are rewritten by other shaves
/// param[in] shaveNumber - shave number for which default value will be set
/// @return void
///
void swcSetShaveWindowsToDefault(u32 shaveNumber);

/// @brief Check if a specific Shave is running or it is stopped
/// @param[in] svu - shave number
/// @return
///			- 0 if stopped
///			- 1 if running
///
u32 swcShaveRunning(u32 svu);

/// @brief Start shave shave_nr from entry_point
/// @param[in] shave_nr    - shave number to be started
/// @param[in] entry_point - entry point
/// @return void
///
void swcRunShave( u32 shave_nr, u32 entry_point);

/// @brief Starts non blocking execution of a shave
/// @param[in] shave_nr u32 shave number to start
/// @param[in] entry_point u32 memory address to load in the shave instruction pointer before starting
/// @return void
///
void swcStartShave(u32 shave_nr, u32 entry_point);

/// @brief Starts non blocking execution of a shave using dynamic sub module alocator
/// @param[in] shave_nr u32 shave number to start
/// @param[in] Context u32 memory address of ModuleData structure
/// @return void
///
void swcDynStartShave(u32 shave_nr, u32 Context);

/// @brief Starts non blocking execution of a shave
/// @param[in] shave_nr u32 shave number to start
/// @param[in] entry_point u32 memory address to load in the shave instruction pointer before starting
/// @param[in] function to call when shave finished execution
/// @return void
///
void swcShaveStartAsync( u32 shave_nr, u32 entry_point, irq_handler function);
void swcStartShaveAsync( u32 shave_nr, u32 entry_point, irq_handler function) __Deprecated__("Please use swcShaveStartAsync instead.");

/// @brief Starts dynamic non blocking execution of a shave.
///        A master entry point is executed prior to jumping into shave entry point.
/// @param[in] shave_nr u32 shave number to start
/// @param[in] Context u32 memory address of ModuleData structure
/// @param[in] function to call when shave finished execution
/// @return void
///
void swcDynShaveStartAsync(u32 shave_nr, u32 Context, irq_handler function);

/// @brief Assigns a callback to a shave for end of execution. Alternative way to the swcStartShaveAsync way of working.
/// @param[in] shave_nr u32 shave number to start
/// @param[in] function to call when shave finished execution
/// @return void
///
void swcAssignShaveCallback(u32 shave_nr, irq_handler function);

void swcSetRegsCC(u32 shave_num, const char *fmt, va_list a_list);

/// @brief Write the value to a IRF/SRF/VRF Registers from a specific Shave
/// @param[in] shave_num - shave number to read T-Register value from
/// @param[in] pc - function called from the pc
/// @param[in] *fmt - string containing i, s, or v according to irf, srf or vrf ex. "iisv"
/// @param[in] ... - variable number of params according to fmt
/// @return void
///
void swcStartShaveCC(u32 shave_num, u32 pc, const char *fmt, ...);

/// @brief Disables the interrupt for shave end. Useful for cases where the shave needs to be run for a
/// few times in Async mode with interrupts but then the same shave needs to stop triggering interrupts
/// @param[in] shave_nr - u32 shave number to start
/// @param[in] function - to call when shave finished execution
/// @return void
///
void swcDisableShaveCallback(u32 shave_nr);

/// @brief Write the value to a IRF/SRF/VRF Registers from a specific Shave
/// @param[in] shave_num - shave number to read T-Register value from
/// @param[in] pc       - function called from the pc
/// @param[in] function - function to call when shave finished execution
/// @param[in] *fmt     - string containing i, s, or v according to irf, srf or vrf ex. "iisv"
/// @param[in] ...      - variable number of params according to fmt
/// @return void
///
void swcStartShaveAsyncCC(u32 shave_num, u32 pc, irq_handler function, const char *fmt, ...);

/// @brief Write the value to a IRF/SRF/VRF Registers from a specific Shave
/// @param[in] shave_num - shave number to read T-Register value from
/// @param[in] *fmt - string containing i, s, or v according to irf, srf or vrf ex. "iisv"
/// @param[in] ... - variable number of params according to fmt
/// @return void
///
void swcSetupShaveCC(u32 shave_num, const char *fmt, ...);

/// @brief Function that starts one shave but at the same time also sets rounding bits
/// @param[in] shave_no     - shave number to start
/// @param[in] roundingBits - rounding bits
/// @return void
///
void swcSetRounding( u32 shave_no, u32 roundingBits);

/// @brief Reset shave
/// @param[in] shaveNumber - shave number to be reset
/// @return void
///
void swcResetShave(u32 shaveNumber);

/// @brief Reset shave without resetting the fifo
/// @param[in] shaveNumber - shave number to be reset
/// @return void
///
void swcResetShaveLite(u32 shaveNumber);

/// @brief Function that waits for the shaves used to finish
/// @param[in]  no_of_shaves - number of shaves that are used
/// @param[in]  *shave_list  - list of shaves used(an array which contains all the shaves used within the application)
/// @return void
///
int swcWaitShaves(u32 no_of_shaves, swcShaveUnit_t *shave_list);

/// @brief Wait for a specific shave to finish execution
/// @param[in] shve_nr - shave number we wait for
/// @return void
///
int swcWaitShave(u32 shave_nr);

/// @brief Check if a list of shaves is running or not
/// @param[in] first - first shave in the list
/// @param[in] last  - last shave in the list
/// @return
///			- 0 if stopped
///			- 1 if running
///
u32 swcShavesRunning(u32 first, u32 last);

/// @brief Check if a list of shaves stored in an array is running or not
/// @param[in] arr - array in which are stored shave numbers
/// @param[in] N   - number of elements in the array
/// @return
///			- 0 if stopped
///			- 1 if running
///
u32 swcShavesRunningArr(u32 arr[], u32 N);


/// @brief Translate windowed address into real physical address
///
/// Non-windowed address are passed through.
/// @param[in] vAddr 	   - Input virtual(windowed) Address
/// @param[in] shaveNumber - Shave to which the virtual address relates
/// @return
u32 swcSolveShaveRelAddr(u32 vAddr, u32 shaveNumber);

/// @brief Translate windowed address into real physical address
///
/// Non-windowed address are passed through.
/// @note This API variant is provided simply for backwards
/// compatibility with Myriad1 code. Not advised for new code.
/// @param[in] vAddr       - Input virtual Address
/// @param[in] shaveNumber - Shave to which the virtual address relates
static inline u32 swcSolveShaveRelAddrAHB(u32 vAddr, u32 shaveNumber)
{
    return swcSolveShaveRelAddr(vAddr,shaveNumber);
}

/// @brief Load a mbin file to a specific target address on shave
/// @param[in] sAddr   - source address
/// @param[in] targetS - target shave number
/// @return void
///
void swcLoadMbin(u8 *sAddr, u32 targetS);

/// @brief Sets a default value for stack
/// @attention Only use this if you are using the default ldscript or really
/// know what you're doing!
/// @param[in] shave_num - Shave for which to set the default stack value
/// @return void
///
void swcSetWindowedDefaultStack(u32 shave_num);

/// @brief Dynamically load shvdlib file - These are elf object files stripped of any symbols
/// @param[in] sAddr   - starting address where to load the shvdlib file
/// @param[in] targetS - the target Shave
/// @return void
///
void swcLoadshvdlib(u8 *sAddr, u32 targetS);

/// @brief Dynamically load library file and return start memory address and length that need to be flushed - These are elf object files stripped of any symbols
/// @param[in] sAddr   - starting address where to load the library file
/// @param[in] targetS - the target Shave
/// @param[in] contextType - type of the loaded library
/// @param[out] vpProgrammedMemAddress - first memory address written 
/// @param[out] flushLength - the length of the data written
/// @return void
///
void swcLoadDynLibraryCacheAware(u8 *sAddr, u32 targetS, context_t contextType, u32 * vpProgrammedMemAddress, u32 * flushLength);

/// @brief Dynamically load library file - These are elf object files stripped of any symbols
/// @param[in] sAddr   - starting address where to load the library file
/// @param[in] targetS - the target Shave
/// @param[in] contextType - type of the loaded library
/// @return void
///
void swcLoadDynLibrary(u8 *sAddr, u32 targetS, context_t contextType);

/// @brief Sets up and launches one dynamic application instance.
///        Uses the shaves preliminary assigned by user via function swcSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[out] *shaveNumber - assigned shave number if operation is successful
/// @return operation status
///
s32 swcRunShaveAlgo(DynamicContext_t *moduleStData, int * const shaveNumber);

/// @brief Sets up and launches one dynamic application instance.
///        Uses the shaves preliminary assigned by user via function swcSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[out] *shaveNumber - assigned shave number if operation is successful
/// @param[in] *fmt - string containing i, s, or v according to irf, srf or vrf ex. "iisv"
/// @param[in] ... - variable number of params according to fmt
/// @return operation status
///
s32 swcRunShaveAlgoCC(DynamicContext_t *moduleStData, int * const shaveNumber, const char *fmt, ...);

/// @brief Sets up and launches one dynamic application instance on a specifically requested SHAVE
///        Uses the shaves preliminary assigned by user via function swcSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
///        Checks if the requested shave has bee configured in advance and if it is not running.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[in] shaveNumber  - specific shave requested by the user to run the algorithm on
/// @return operation status
///
s32 swcRunShaveAlgoOnAssignedShave(DynamicContext_t *moduleStData, u32 shaveNumber);

/// @brief Sets up and launches one dynamic application instance on a specifically requested SHAVE
///        Uses the shaves preliminary assigned by user via function swcSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
///        Checks if the requested shave has bee configured in advance and if it is not running.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[in] shaveNumber  - specific shave requested by the user to run the algorithm on
/// @param[in] *fmt - string containing i, s, or v according to irf, srf or vrf ex. "iisv"
/// @param[in] ... - variable number of params according to fmt
/// @return operation status
///
s32 swcRunShaveAlgoOnAssignedShaveCC(DynamicContext_t *moduleStData, u32 shaveNumber, const char *fmt, ...);

/// @brief This function allocates heap and group data memory for all configured instances of one application.
///        It must be called prior to using swcRunShaveAlgo().
///        Can be used from both Leons.
///        svuList below is not copied internally, instead just the pointer is
///        assigned to an internal structure. Please ensure the svuList memory
///        is alive until the call of swcCleanupDynShaveApps. Note: be careful
///        about stack declared svuList.
/// @param[in] moduleStData - DynamicContext_t pointer to application's module data structure
/// @param[in] svuList - pointer to a shave list to be used for all application instances
/// @param[in] instances - number of application instances; must correspond to number of shaves configured in svuList
/// @return operation status
///
s32 swcSetupDynShaveApps(DynamicContext_t *moduleStData, const swcShaveUnit_t *svuList, const uint32_t instances);

/// @brief This function frees the heap and group data memory for all configured instances of one application.
///        It can be called after usage of swcRunShaveAlgo().
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @return operation status
///
s32 swcCleanupDynShaveApps(DynamicContext_t *moduleStData);

/// @brief This function allows hinting how much code/data is desired to be allocated
/// TODO: add functionality to precompute these sizes based on .textCrit size
/// @param[in] moduleStData           - DynamicContext_t pointer to application's module data structure
/// @param[in] cmx critical code size - Desired value for the cmx critical code size. If not set, the default will accommodate 32K
/// @return operation status
///
s32 swcDynShaveAppSetWindows(DynamicContext_t *moduleStData, u32 cmxCriticalCodeSize);

/// @brief This function is used to check if the user has called a correct shave.
///        We define "correct" as: configured to be used by the current dyncontext
///        and not currently running.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] shaveNumber  - shave to be verified
/// @returns 0 if it is not a valid shave, or 1 if valid
///
u32 swcCheckFreeAndValidShave(DynamicContext_t *moduleStData, u32 shaveNumber);

/// @brief This functions gives a list of unallocated shaves in the system.
/// @param[in] svulist - pointer to a shave list in which will be assigned the unallocated shaves found in the system
/// @param[in] shavesNumber - number of unallocated shaves to find ins the system
/// @return operation status
///
s32 swcRequestUnallocatedShaves(swcShaveUnit_t *svuList, u32 shavesNumber);

/// @brief This function return the number of unallocated shave in the system
/// @return unallocated shaves number
s32 swcGetUnallocatedShavesNumber(void);

/// @brief This function frees the heap and group data memory for the specified instances of one application.
///        Can be used from both Leons.
/// @param[in] mData  - DynamicContext_t pointer to application's module data structure
/// @param[in] svuList - pointer to a shave list which will specify the shaves to be freed from the application
/// @param[in] elementsNumber - number of shaves in the list
/// @return operation status
///
s32 swcCleanupDynShaveListApps(DynamicContext_t *mData, swcShaveUnit_t *svuList, uint32_t elementsNumber);

/// @brief This function set a new heap location for a specific shave.
///        Can be used from both Leons.
/// @param[in] mData  - DynamicContext_t pointer to application's module data structure
/// @param[in] newAddress - pointer to the memory location of the new location
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @return void
///
void swcSetNewHeapLocation(DynamicContext_t *mData, unsigned char * newAddress, swcShaveUnit_t shaveNumber);

/// @brief This function set a new memory location where to load the application dynamic data.
///        Can be used from both Leons.
/// @param[in] mData  - DynamicContext_t pointer to application's module data structure
/// @param[in] newAddress - pointer to the memory location of the new location
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @return operation status
///
void swcSetNewAppDynDataLocation(DynamicContext_t *mData, unsigned char * newAddress, swcShaveUnit_t shaveNumber);

/// @brief This function set a new memory location where to load the grup dynamic data.
///        Can be used from both Leons.
/// @param[in] mData  - DynamicContext_t pointer to application's module data structure
/// @param[in] newAddress - pointer to the memory location of the new location
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @return operation status
///
void swcSetGrpDynDataLocation(DynamicContext_t *mData, unsigned char * newAddress, swcShaveUnit_t shaveNumber);

/// @brief This function allocates heap and group data memory for all configured instances of one application
///        and loads the dynamic library.
///        It must be called prior to using swcRunShaveAlgo().
///        Can be used from both Leons.
///        Please ensure the svuList memory is alive until the call of swcCleanupDynShaveApps.
///        Note: be careful about stack declared svuList.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] svuList - pointer to a shave list to be used for all application instances
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @param[in] paradigmType - the type of paradigm which is used for running the applications
/// @return operation status
///
int swcIsoSetupShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType);

/// @brief This function launch a shave application with a specific function as entry point.
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] functionName - pointer to a string containing the name of the entry point to be started on shave side.
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @return operation status
///
int swcStartEntryPointDC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* functionName);

/// @brief This function launch a shave application with a specific function as entry point.
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] functionName - pointer to a string containing the name of the entry point to be started on shave side.
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @param[in] *fmt - string containing i, s, or v according to irf, srf or vrf ex. "iisv"
/// @param[in] ... - variable number of params according to fmt
/// @return operation status
///
int swcStartEntryPointDCCC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* functionName, const char *fmt, ...);

/// @brief This function launch a shave application by calling the main function.
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @return operation status
///
int swcStartFC(DynamicContext_t *moduleStData, uint32_t shaveNumber);

/// @brief This function frees the heap and group data memory for all configured instances of one application.
///        It can be called after usage of swcRunShaveAlgo().
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] svuList - pointer to a shave list to be used for all application instances
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @param[in] paradigmType - the type of paradigm which is used for running the applications
/// @return operation status
///
int swcIsoCleanShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType);

/// @}
#ifdef __cplusplus
}
#endif

#endif // SWC_SHAVE_LOADER_H
