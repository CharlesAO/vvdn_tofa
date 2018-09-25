///
/// @file OsDrvSvu.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvSvu OsSvu Driver
/// @{
/// @brief     Shave low level functionality for RTEMS.
///

#ifndef _OS_DRV_SVU_H_
#define _OS_DRV_SVU_H_

// System Includes
// ----------------------------------------------------------------------------

// Application Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include "OsDrvSvuDefines.h"
// Common for all OS drivers
#include <OsCommon.h>
// Bare-metal layer include
#include "DrvSvu.h"

#ifdef __cplusplus
extern "C" {
#endif

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Global function prototypes
// ----------------------------------------------------------------------------
/// Initializes the shave driver
/// @return
///     OS_MYR_DRV_SUCCESS - the operation finished successfully
///     OS_MYR_DRV_ERROR - an error has occurred during initialization
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///     OS_MYR_DRV_ALREADY_INITIALIZED - the driver was already initialized
///
int OsDrvSvuInit(void);

/// Tries to get access to use the shave given as parameter
/// @param[in] handler - shave handler
/// @param[in] shaveNumber - shave number
/// @param[in] protection - the protection type used for the shave
/// @return
///     OS_MYR_DRV_SUCCESS - the access to the shave was granted
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///     OS_MYR_DRV_ALREADY_OPENED - the shave was already opened by a previous call to OsDrvSvuOpen()
///     OS_MYR_DRV_RESOURCE_BUSY - the shave is currently used by another thread
///     OS_MYR_DRV_ERROR - an error has occurred
///
int OsDrvSvuOpenShave(osDrvSvuHandler_t* handler, u32 shaveNumber, OS_MYRIAD_DRIVER_PROTECTION protection);


/// Tries to get access to use the shave given as parameter
/// @param[in] svuList - pointer to a list of shaves to be closed
/// @param[in] shavesInList - number of shaves in svuList
/// @param[in] protection - the protection type used for the shave
/// @return
///     OS_MYR_DRV_SUCCESS - the access to the shave was granted
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///     OS_MYR_DRV_ALREADY_OPENED - the shave was already opened by a previous call to OsDrvSvuOpen()
///     OS_MYR_DRV_RESOURCE_BUSY - the shave is currently used by another thread
///     OS_MYR_DRV_ERROR - an error has occurred
///
int OsDrvSvuOpenShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList, OS_MYRIAD_DRIVER_PROTECTION protection);

/// Returns the window registers for the given shave
/// @param[in] handler - shave handler
/// @param[out] windowA - register value for window A
/// @param[out] windowB - register value for window B
/// @param[out] windowC - register value for window C
/// @param[out] windowD - register value for window D
/// @return
/// return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuGetShaveWindowRegs(osDrvSvuHandler_t* handler, u32* windows);

/// Set the default values for the widows register of the given shave
/// @param[in] handler      - shave handler
/// @return
/// return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuSetShaveWindowsToDefault(osDrvSvuHandler_t* handler);

/// Set the default values for the 0x1Exxxxxx and 0x1Fxxxxxx windows of one shave
/// and set application related windows values to 0x1Cxxxxxx and 0x1Dxxxxxx. Application
/// related refers to the fact a dynamic data SHAVE program allocation structure self configures
/// itself to contain a field keeping track of how much critical CMX code someone assigned for that
/// application through the use of CMX_CODE or __attribute__((section(".textCrit"))) decorators on
/// functions and configures the two said windows (0x1Cxxxxxx and 0x1Dxxxxxx) accordingly by
/// putting 0x1Dxxxxxx at the start of the needed shave and the 0x1Cxxxxxx at the first 1K boundary
/// after the code size
/// @param[in] handler      - shave handler
/// @param[in] moduleStData - DynamicContext_t pointer to application's module data structure
/// @return
/// return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuSetShaveWindowsToAppSettings(osDrvSvuHandler_t* handler, DynamicContext_t* moduleStData);

/// Set the default values for the widows register of the given shave
/// @param[in] handler - shave handler
/// @param[in] windowA - register number for window A
/// @param[in] windowB - register number for window B
/// @param[in] windowC - register number for window C
/// @param[in] windowD - register number for window D
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuSetShaveWindows(osDrvSvuHandler_t* handler, u32 windowA, u32 windowB, u32 windowC, u32 windowD);

/// Sets a default value for stack
/// !WARNING: only use this if you are using the default ldscript or really
/// know what you're doing!
/// @param[in] handler - shave handler
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuSetWindowedDefaultStack(osDrvSvuHandler_t* handler);

/// @brief This function allows hinting how much code/data is desired to be allocated
/// TODO: add functionality to precompute these sizes based on .textCrit size
/// @param[in] moduleStData           - DynamicContext_t pointer to application's module data structure
/// @param[in] cmx critical code size - Desired value for the cmx critical code size. If not set, the default will accommodate 32K
/// @return operation status
///
s32 OsDrvDynShaveAppSetWindows(DynamicContext_t *moduleStData, u32 cmxCriticalCodeSize);

/// Sets a default value for stack with absolute address
/// !WARNING: only use this if you are using the default ldscript or really
/// know what you're doing!
/// @param[in] handler - shave handler
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuSetAbsoluteDefaultStack(osDrvSvuHandler_t* handler);

/// Starts the shave execution and waits for it to finish.
/// @param[in] handler - shave handler
/// @param[in] entryPoint memory address to load in the shave instruction pointer before starting
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuRunShave(osDrvSvuHandler_t* handler, u32 entryPoint);

/// Starts non blocking execution of a shave
/// @param[in] handler - shave handler
/// @param[in] entryPoint memory address to load in the shave instruction pointer before starting
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuStartShave(osDrvSvuHandler_t* handler, u32 entryPoint);

/// Write the value to a IRF/VRF Registers from a specific Shave and starts the shave
/// @param[in] handler - shave handler
/// @param[in] pc - function called from the program counter pc
/// @param[in] *fmt - string containing i or v according to irf or vrf ex. "iiv"
/// @param[in] ... - variable number of parameters according to fmt
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuStartShaveCC(osDrvSvuHandler_t* handler, u32 pc, const char *fmt, ...);

/// Write the value to a IRF/VRF Registers from a specific Shave
/// @param[in] handler - shave handler
/// @param[in] *fmt - string containing i or v according to irf or vrf ex. "iiv"
/// @param[in] ... - variable number of params according to fmt
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuSetupShaveCC(osDrvSvuHandler_t* handler, const char *fmt, ...);

/// Resets the given shave
/// @param[in] handler - shave handler
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuResetShave(osDrvSvuHandler_t* handler);

/// Waits for the shaves in the given list to finish execution
///
/// This function can only be used to wait for shaves that were started in the same thread. If this is
/// not the case OS_MYR_RESOURCE_NOT_OWNED will be returned.
/// @param[in]  noOfShaves the number of shaves in the list
/// @param[in]  shaveList a list of shave handlers
/// @param[in]  timeout the amount of time in system ticks to wait until checking for the shave
///     status
/// @param[in]  timeout the amount of time in system ticks to wait until checking for the shave
/// @param[out] runningShaves a bit field with the shaves that did not finish their execution
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///     OS_MYR_RESOURCE_NOT_OWNED - the shave for which is waited was started by other thread
///
///     When this code is returned, the runningShaves parameter contains the first shave that generated
///     the error
///
int OsDrvSvuWaitShaves(u32 noOfShaves, osDrvSvuHandler_t* shaveList, u32 timeout, u32 *runningShaves);

/// Waits for the shaves in the given list to finish execution.
/// This function is similar to OsDrvSvuWaitShaves and can
/// be used for dynamic applications started with OsDrvSvuRunShaveAlgo variants
/// Uses internal osDrvSvuHandler_t handler.
/// @param[in] svuList - pointer to a list of shaves to be waited
/// @param[in] shavesInList - number of shaves in svuList
/// @param[in] timeout the amount of time in system ticks to wait until checking
///            for the shave status
/// @param[out] runningShaves a bit field with the shaves that did not finish their execution
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///     OS_MYR_RESOURCE_NOT_OWNED - the shave for which is waited was started by other thread
///
///     When this code is returned, the runningShaves parameter contains the first shave that generated
///     the error
///
int OsDrvSvuDynWaitShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList, u32 timeout, u32 *runningShaves);

/// Sets the rounding bits for a particular shave
/// @param[in]  handler shave handler
/// @param[in]  roundingBits rounding bits
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///     OS_MYR_DRV_NOTOPEND - the shave was not opened
int OsDrvSvuSetRounding(osDrvSvuHandler_t* handler, u32 roundingBits);

/// Converts a shave relative address to a System solved address based on the target CMX slice
/// and current window it relates to
/// @param[in]  inAddr shave relative address (can be code/data/absolute type of address)
/// @param[in]  shaveNumber shave number
/// @param[in]  outAddr the resolved address
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
int OsDrvSvuSolveShaveRelAddr(u32 inAddr, u32 shaveNumber, u32* outAddr);

/// Performs an L1 data cache operation on the given shave
/// @param[in]  handler shave handler
/// @param[in] action - the type of the action that needs to be performed
///
/// It can have one of the following values:
/// SVUL1DATACACHE_ENABLE - enables shave L1 data cache
/// SVUL1DATACACHE_DISABLE - disables shave L1 data cache
/// SVUL1DATACACHE_STOP - stops shave L1 data cache
/// SVUL1DATACACHE_INVALIDATE_ALL - invalidates all cache
/// SVUL1DATACACHE_FLUSH_ALL - flushes all cache
/// SVUL1DATACACHE_FLUSH_INVALIDATE_ALL - flushes and invalidates all cache
///
/// @return - MYR_DRV_SUCCESS the operations finished successfully
///         - MYR_DRV_ERROR the operation provided as parameter is invalid
///
int OsDrvSvuL1DataCacheCtrl(osDrvSvuHandler_t* handler, u32 action);

/// Performs an L1 instruction cache operation on the given shave
/// @param[in] handler shave handler
/// @param[in] action the type of the action that needs to be performed
///
/// It can have one of the following values:
/// SVUL1INSTRCACHE_DEFAULT enables shave L1 instruction cache
/// SVUL1INSTRCACHE_BYPASS bypasses shave L1 instruction cache
/// SVUL1INSTRCACHE_LOCK locks shave L1 instruction cache
/// SVUL1INSTRCACHE_INVALIDATE invalidates L1 instruction cache
///
/// @return - MYR_DRV_SUCCESS the operations finished successfully
///         - MYR_DRV_ERROR the operation provided as parameter is invalid
///
int OsDrvSvuL1InstrCacheCtrl(osDrvSvuHandler_t* handler, u32 action);

/// Stops the execution of the given shaves
/// By default the Shave L1 ICache is enabled unless this function
/// is called
/// @param[in] svuList - pointer to a list of shaves to be closed
/// @param[in] shavesInList - number of shaves in svuList
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuStopShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList);

/// Stops the execution of the given shave
/// By default the Shave L1 ICache is enabled unless this function
/// is called
/// @param[in] handler - shave handler
/// @return
///     OS_MYR_DRV_SUCCESS - the operation completed successfully
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be accessed because it is used by another thread
///
int OsDrvSvuStop(osDrvSvuHandler_t* handler);

/// Releases the resources allocated for the given shave
/// @param[in] handler - shave handler
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation finished successfully
///     OS_MYR_DRV_ERROR - an error has occurred
///     OS_MYR_DRV_TIMEOUT - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_NOTOPENED - cannot close the driver because it was not opened
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be closed because other thread is using it
///
int OsDrvSvuCloseShave(osDrvSvuHandler_t* handler);

/// Releases the resources allocated for the given list of shaves
/// @param[in] svuList - pointer to a list of shaves to be closed
/// @param[in] shavesInList - number of shaves in svuList
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation finished successfully
///     OS_MYR_DRV_ERROR - an error has occurred
///     OS_MYR_DRV_TIMEOUT - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_NOTOPENED - cannot close the driver because it was not opened
///     OS_MYR_DRV_RESOURCE_BUSY - the shave cannot be closed because other thread is using it
///
int OsDrvSvuCloseShaves(const swcShaveUnit_t *svuList, const uint32_t shavesInList);

/// Releases the driver resources if no shave is running
///
/// @return
///     OS_MYR_DRV_SUCCESS - the operation finished successfully, the driver is closed
///     OS_MYR_DRV_ERROR - an error has occurred
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///     OS_MYR_DRV_RESOURCE_BUSY - all the shaves owned but the current thread were closed, but
///     there are another thread working with shaves, so the resources cannot be released
///     OS_MYR_DRV_NOTOPENED - trying to close an uninitialized driver
///
int OsDrvSvuClose(void);

/// Dynamically load shvdlib file - These are elf object files stripped of any symbols
/// @param[in] startAddr - starting address where to load the shvdlib file
/// @param[in] handler   - shave handler
/// @return
///
int OsDrvSvuLoadShvdlib(u8 *startAddr, osDrvSvuHandler_t* handler);

/// Dynamically load library file - These are elf object files stripped of any symbols
/// @param[in] startAddr    - starting address where to load the library file
/// @param[in] handler      - shave handler
/// @param[in] contextType  - type of the loaded library
/// @return
///
int OsDrvSvuLoadDynLibrary(u8 *startAddr, osDrvSvuHandler_t* handler, context_t contextType);

/// @brief Sets up and launches one dynamic application instance.
///        Uses the shaves preliminary assigned by user via function OsDrvSvuSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[out] *shaveNumber - assigned shave number if operation is successful
/// @return operation status
///
int OsDrvSvuRunShaveAlgo(DynamicContext_t *moduleStData, int * const shaveNumber);

/// @brief Sets up and launches one dynamic application instance.
///        Writes the variable parameters to IRF/VRF registers from a specific shave.
///        Uses the shaves preliminary assigned by user via function OsDrvSvuSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[out] *shaveNumber - assigned shave number if operation is successful
/// @param[in] *fmt - string containing i or v according to irf or vrf ex. "iiv"
/// @param[in] ... - variable number of parameters according to fmt
/// @return operation status
///
int OsDrvSvuRunShaveAlgoCC(DynamicContext_t *moduleStData, int * const shaveNumber, const char *fmt, ...);

/// @brief This function frees the heap and group data memory for all configured instances of one application.
///        It can be called after usage of OsDrvSvuRunShaveAlgo().
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @return operation status
///
int OsDrvSvuCleanupDynShaveApps(DynamicContext_t *moduleStData);

/// @brief This function allocates heap and group data memory for all configured instances of one application.
///        It must be called prior to using OsDrvSvuRunShaveAlgo().
///        Can be used from both Leons.
/// @param[in] moduleStData - DynamicContext_t pointer to application's module data structure
/// @param[in] svuList - pointer to a shave list to be used for all application instances
/// @param[in] instances - number of application instances; must correspond to number of shaves configured in svuList
/// @return operation status
///
int OsDrvSvuSetupDynShaveApps(DynamicContext_t *mData, const swcShaveUnit_t *svuList, const uint32_t instances);

/// @brief Sets up and launches one dynamic application instance.
///        Targets a specific shave designated.
///        Target shave needs to be preliminary assigned via function OsDrvSvuSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[in] shaveNumber - specific shave to be used
/// @return operation status
///
int OsDrvSvuRunShaveAlgoOnAssignedShave(DynamicContext_t *moduleStData, u32 shaveNumber);

/// @brief Sets up and launches one dynamic application instance.
///        Targets a specific shave designated.
///        Writes the variable parameters to IRF/VRF shave's registers.
///        Target shave needs to be preliminary assigned via function OsDrvSvuSetupDynShaveApps().
///        Allocates all necessary memory, loads the dynamic library, then starts the shave.
/// @param[in] moduleStData - DynamicContext_t pointer to ModuleData structure
/// @param[in] shaveNumber - specific shave to be used
/// @param[in] *fmt - string containing i or v according to irf or vrf ex. "iiv"
/// @param[in] ... - variable number of parameters according to fmt
/// @return operation status
///
int OsDrvSvuRunShaveAlgoOnAssignedShaveCC(DynamicContext_t *moduleStData, u32 shaveNumber, const char *fmt, ...);

/// @brief This functions gives a list of unallocated shaves in the system.
///        It could be called prior to using OsDrvSvuSetupDynShaveApps to get the svuList.
/// @param[in] svulist - pointer to a shave list in which will be assigned the unallocated shaves found in the system
/// @param[in] shaveNumber - number of unallocated shaves to find ins the system
/// @return operation status
///
int OsDrvRequestUnallocatedShaves(swcShaveUnit_t *svuList, u32 shavesNumber);

/// @brief This function return the number of unallocated shave in the system
/// @return unallocated shaves number
int OsDrvGetUnallocatedShavesNumber(void);

/// @brief This function frees the heap and group data memory for the specified instances of one application.
///        It can be called after usage of OsDrvSvuRunShaveAlgo().
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] svuList - pointer to a shave list which will specify the shaves to be freedfrom the application
/// @param[in] shavesNumber - number of shaves in the list
/// @return operation status
///
int OsDrvSvuCleanupDynShaveListApps(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber);

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
int OsDrvIsoSetupShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType);

/// @brief This function launch a shave application with a specific function as entry point.
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] functionName - pointer to a string containing the name of the entry point to be started on shave side.
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @return operation status
///
int OsDrvStartEntryPointDC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* fName);

/// @brief This function launch a shave application with a specific function as entry point.
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] functionName - pointer to a string containing the name of the entry point to be started on shave side.
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @param[in] *fmt - string containing i or v according to irf or vrf ex. "iiv"
/// @param[in] ... - variable number of parameters according to fmt
/// @return operation status
///
int OsDrvStartEntryPointDCCC(DynamicContext_t *moduleStData, uint32_t shaveNumber, const char* fName, const char *fmt, ...);

/// @brief This function launch a shave application by calling the main function.
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @return operation status
///
int OsDrvStartFC(DynamicContext_t *moduleStData, uint32_t shaveNumber);

/// @brief This function frees the heap and group data memory for all configured instances of one application.
///        It can be called after usage of swcRunShaveAlgo().
///        Can be used from both Leons.
/// @param[in] moduleStData  - DynamicContext_t pointer to application's module data structure
/// @param[in] svuList - pointer to a shave list to be used for all application instances
/// @param[in] shaveNumber - the shave number for which to set the new location
/// @param[in] paradigmType - the type of paradigm which is used for running the applications
/// @return operation status
///
int OsDrvIsoCleanShaveApplication(DynamicContext_t *moduleStData, swcShaveUnit_t *svuList, uint32_t shavesNumber, MISA_PARADIGM_TYPE paradigmType);

//Temporary workaround
int OsDrvprotectionLock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType);
int OsDrvprotectionUnlock(u32 channel, OS_MYRIAD_DRIVER_PROTECTION protectionType);
void OsDrvSvuIrqHandler(u32 source);
void OsDrvupdateTaskId(osDrvSvuHandler_t* handler);
#ifdef __cplusplus
}
#endif


#endif

/// @}

