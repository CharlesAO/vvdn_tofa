///
/// @file DrvSdio.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvSdio Sdio Driver
/// @{
/// @brief     SDIO driver header file Support for SDHC cards from V2.0
///



#ifndef _DRVSDIO_H_
#define _DRVSDIO_H_
#include "mv_types.h"
#include "DrvSdioDefines.h"

//remove deprecation warnings from the driver
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef __cplusplus
extern "C" {
#endif

/// This function initializes the driver and Mobile Storage Host
/// 
/// Configures standard parameters By default 4 bit bus width. Only support SDHC Cards.
/// No Voltage switch
/// @param[in] cclk_in Input Clock. It must be half or less than the system clock
/// @param[in] target_clk target Clock
/// @param[in] descriptors_base_address Base address for descriptors (Internal DMA) Must be 32-bit aligned
/// @return Success or Fail
/// @deprecated Use DrvSdioInitialise instead.
///
__Deprecated__("Use DrvSdioInitialise instead")
DRV_RETURN_TYPE DrvSdioInit(u32 cclk_in, u32 descriptors_base_address);


/// This function initializes the driver and Mobile Storage Host
/// 
/// Configures standard parameters By default 4 bit bus width. Only support SDHC Cards.
///
/// @warning DDR50 and SDIO_SDR104 speed mode not yet supported
///
/// @param[in] cclk_in                  Input Clock. It must be half or less than the system clock
/// @param[in] descriptors_base_address Base address for descriptors (Internal DMA) Must be 32-bit aligned
/// @param[in] opts                     Additional options (voltage switch utility, speed mode)
/// @return Success or Fail
///
DRV_RETURN_TYPE DrvSdioInitialise(u32 cclk_in, u32 descriptors_base_address, DrvSdioOpts *opts);


/// This function allows to setup the Irq priority
/// 
/// Install interrupt handler to attend SDIO operation it needs to be called along with DrvSdioInit
/// @param[in] priority of the interrupt attending SDIO 
/// @return DRVSDIO_SUCCESS
DRV_RETURN_TYPE DrvSdioSetupIrq(u32 interruptPriority);

/// This function allows to write 1 or x blocks of fixed 512 bytes starting at sdblocknumber
/// 
/// @param[in] sdblocknumber Start block number to perform the write
/// @param[in] sourceaddress Start address of the data. Must be 32-bit aligned 
/// @param[in] datasize Size of Data (Multiple of 512 !!!)
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *errorflags Flags associated with the operation (Low level)
/// @return success or fail
DRV_RETURN_TYPE DrvSdioWriteDataBlock(u32 sdblocknumber, u32 sourceaddress, u32 datasize, u32 cardslot, u32 *errorflags);

/// This function allows to read 1 or x blocks of fixed 512 bytes starting at sdblocknumber
/// 
/// @param[in] destaddress Destination address for the data. Must be 32-bit aligned 
/// @param[in] sdblocknumber start block number from which the read operation is to be performed
/// @param[in] datasize Size of the data to be read (Multiple of 512)
/// @param[in] cardslot  Slot where the card is located 0-15
/// @param[out] *errorflags  Flags associated with the operation (Low level)
/// @return  Success or Fail
DRV_RETURN_TYPE DrvSdioReadDataBlock(u32 destaddress, u32 sdblocknumber, u32 datasize, u32 cardslot, u32 *errorflags);

/// This function allows to start an enumeration process at the cardslot
/// 
/// Only valid for SDHC (v2.0 onwards). Voltage switch supported. Change of frequency
/// 
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *errorflags Flags associated with the operation (Low level)
/// @return success or fail
DRV_RETURN_TYPE DrvSdioEnumerate(u32 cardslot, u32 *errorflags);

/// Allows to erase a range of blocks passed as a parameters (block size == 512)
/// 
/// @param[in] firstblock First block to be erased
/// @param[in] lastblock Last block to be erased
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *errorflags Flags associated with the operation (Low level)
/// @return Success or Fail
DRV_RETURN_TYPE DrvSdioEraseDataBlock(u32 firstblock, u32 lastblock, u32 cardslot, u32 *errorflags);

/// Allows to retrieve the number of blocks of the card in cardslot
/// 
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *cardblocknum Number of blocks
/// @return Success or Fail
DRV_RETURN_TYPE DrvSdioGetCardBlockNumber(u32 cardslot, u64 *cardblocknum);


/// Allows to retrieve the capacity of the card in cardslot
/// 
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *cardcapacity Capacity in bytes
/// @return Success or Fail
DRV_RETURN_TYPE DrvSdioGetCardCapacity(u32 cardslot, u64 *cardcapacity);

/// Allows to retrieve the maximum transfer speed of the sd in cardslot
/// 
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *cardspeed Speed in KHz
/// @return Success or Fail
DRV_RETURN_TYPE DrvSdioGetCardTransferSpeed(u32 cardslot, u32 *cardtransferspeed);

/// Allows to retrieve access time of the sd in cardslot
/// 
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *cardaccesstime Access time in ns
/// @return Success or Fail
DRV_RETURN_TYPE DrvSdioGetCardAccessTime(u32 cardslot, u32 *cardaccesstime);

/// Allows to retrieve the maximum access time of the sd in cardslot
/// 
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[in] transfer_speed Transfer speed selected
/// @param[out] *cardmaxaccesstime Access time in ns
/// @return Success or Fail
DRV_RETURN_TYPE DrvSdioGetCardMaxAccessTime(u32 cardslot, u32 transfer_speed, u32 *cardmaxaccesstime);

/// Allows to reset the SDIO Host Controller
/// 
/// This function allows to reset the SDIO Host controller as well as initialize the driver
/// to a known state
/// @param[in] hardware TRUE if hardware reset is to be executed, FALSE to only reset Driver
/// @return None
void DrvSdioReset(u32 hardware);

/// This function allows to write to scattered buffers in memory from a specific block number
/// 
/// This function reads from the SD card placing the data into buffers scattered across the memory
/// @param[in] sdblocknumber Start block number to perform the write
/// @param[in] transactionList List of transaction. Sizes inside the transactions must be multiple of 512
/// @param[in] count Length of the List
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *errorflags Flags associated with the operation (Low level)
/// @return success or fail
DRV_RETURN_TYPE DrvSdioWriteDataBlockList(u32 sdblocknumber, tyDrvSdio_Transaction *transactionList, u32 count, u32 cardslot, u32 *errorflags);

/// This function allows to read from scattered buffers in memory and write this data from a specific block number
/// 
/// This function reads from scattered buffers spread across the memory and place this data into the sd card from a specific block number
/// @param[in] sdblocknumber Start block number to perform the write
/// @param[in] transactionList List of transaction. Sizes inside the transactions must be multiple of 512
/// @param[in] count Length of the List
/// @param[in] cardslot Slot where the card is located 0-15
/// @param[out] *errorflags Flags associated with the operation (Low level)
/// @return success or fail
DRV_RETURN_TYPE DrvSdioReadDataBlockList(u32 sdblocknumber, tyDrvSdio_Transaction *transactionList, u32 count, u32 cardslot, u32 *errorflags);

/// This function returns the maximum speed mode
///
/// This function returns the maximum sdio speed mode, the value may change after
/// after calling DrvSdioEnumerate
///
/// @param[in] cardslot Slot where the card is located 0-1
/// @return sdio spped mode
tyDrvSdioSpeedMode DrvGetMaxSpeed(u32 cardslot);

/// This function deinitializes the driver and Mobile Storage Host
///
/// @return None
///
void DrvSdioDeinitialise(void);

#ifdef __cplusplus
}
#endif

// restore deprecation warnings
#pragma GCC diagnostic pop

#endif

///@}
