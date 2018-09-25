///
/// @file DrvSdio.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSdio
/// @{
/// @brief     Driver to control Clocks, Power and Reset
///


// #define DRVSDIO_DEBUG

#ifdef DRVSDIO_DEBUG
#define DPRINTF1(...) printf(__VA_ARGS__)
#else
#define DPRINTF1(...)
#endif

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DrvSdio.h"
#include "DrvSdioDefines.h"
#include "assert.h"
#include "swcLeonUtils.h"
#include "DrvIcb.h"
#include "DrvIcbDefines.h"
#include "DrvCommon.h"
#include <assert.h>
#include <DrvTimer.h>
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

/* FIFO Watermarks  */
#define DRVSDIO_RX_WMARK                        (DRVSDIO_FIFO_DEPTH/2 - 1)
#define DRVSDIO_TX_WMARK                        (DRVSDIO_FIFO_DEPTH/2)
/* DMA burst size 64 */
#define DRVSDIO_DMASIZE                         5
/* Only for Dual Buffer, distance between two unchained descriptors in bus-width units */
#define DRVSDIO_DMASKIP                         0
/* Control for amount of phase shift on cclk_in_drv clock */
#define DRVSDIO_DEFAULT_CLK_DRV_PHASE_CTRL      0x01
/* Control for amount of phase shift on cclk_in_sample clock */
#define DRVSDIO_DEFAULT_CLK_SMPL_PHASE_CTRL     0x00
/* Delay set after sample clock phase has been changed */
#define DRVSDIO_CLK_SHIFT_PHASE_DELAY           1000000
/* Delay set after voltage value has been changed */
#define DRVSDIO_VOLTAGE_SW_DELAY                2000000

// TIMEOUTS TBD
#define DRVSDIO_WAITCOMMANDDONETIMEOUTUNITS     10
// Default Block Size, the block size can be 512, 1024, 4096 for read/write DMA transfers
#define DRVSDIO_DEFAULTBLOCKSIZE                512
// Increase descriptor buffer size
#define DRVSDIO_INCREASEBLKSIZE                 8

#define DRV_NOTIMEOUT                           0

#define DRVSDIO_FINETUNING_TIMEOUT              1000000

#define DRVSDIO_TIMEOUT_VALUE                   100000

// Masks
#ifndef DRV_DDRMASK
    #define DRV_DDRMASK                         0x80000000
#endif
#ifndef DRV_DDRBYPASSMASK
    #define DRV_DDRBYPASSMASK                   0x40000000
#endif

#ifndef DRV_CMXBYPASSMASK
    #define DRV_CMXBYPASSMASK                   0x08000000
#endif

volatile u32 DrvSdio_RInterrupts = 0;
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Structure to control cards
static tyDrvSdioCardConfig drvsdioCards[DRVSDIO_MAXDRIVERCARDS];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// Driver Status
static struct {
    u8 status;
    u8 reserved_1;
    u16 reserved_2;
    u32 cclk_in_khz;
}tyDrvSdio_Driver;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static void drvSdioSwitchToHighSpeedMode(u32 cardslot, tyDrvSdioSpeedMode mode, u32 *errorflags);
static void drvSdioProgramFIFO(u32 dma_Msize, u32 rx_wmark, u32 tx_wmark);
static u32 drvSdioGetScatteredTransactionSize(tyDrvSdio_Transaction *transactionList, u32 count);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

/// Sleeps for a number of ticks.
///
/// @param[in] ticks Number of clock ticks to pass
/// @return none
static inline void drvSdiodelay(u64 ticks)
{
    volatile u32 i;
    for (i = 0; i < (ticks >> 2); i ++) {
        NOP;
    }
}

/// Allows to wait for any flag no interrupts
///
/// This function waits till the card returns the specified flags or a timeout
///
/// @param[in] flags These are the flags that the card should return.
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @return resp The value of the RINTSTS (Raw Interrupt Status Register) or
/// DRVSDIO_TIMEOUT in case of timeout
u32 __attribute__((weak)) DrvSdioWaitForFlags(u32 flag, u32 timeOut)
{
    u32 resp = 0;

    // This macros gets the value of the RINTSTS register
    resp = DRVSDIO_GETFLAGS;
    // Check if the flags provided as input matches the flags set by the card in RINTSTS
    if(timeOut) {
        while ((!(resp & flag)) && --timeOut) {
            resp |= DRVSDIO_GETFLAGS;
        }
        if(timeOut == 0)
            resp = DRVSDIO_TIMEOUT;
    }
    else {
        while (!(resp & flag)) {
            resp |= DRVSDIO_GETFLAGS;
        }
    }

    // Clear flags, only those which were expected
    // The flags are cleared when you write a 1 to the flag
    DRVSDIO_CLEARFLAGS(flag);
    // Return the value of the RINTSTS register
    return resp;
}

/// This function sends a command to the card if the card is not busy
///
/// The command is not send until the card is busy. After the command
/// is sent to the CIU the function waits for the start_cmd bit to be
/// lowered. When the CIU send the command to the card it lowers the
/// start_cmd bit. Sending the command to card does not mean it is DONE!
/// Command DONE is indicated by the card in the RINTSTS and it is not
/// handled here.
///
/// @param[in] command The command that need to be placed in the CMD register
/// @param[in] checkbusy Indicates whether to wait for the card or not
/// @param[in] card This is the card number
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @param[in] checkDmaBusy Indicates whether to wait for the DMA or not
/// @return rints The value of the RINTSTS (Raw Interrupt Status Register) or
/// DRVSDIO_TIMEOUT in case of timeout
static u32 drvSdioLoadCommand(u32 command, u32 checkbusy, u32 card, u32 timeOut, u32 checkDmaBusy)
{
    u32 read_data = 0, rints = 0, load_data = 0;

    DPRINTF1("\ndrvSdioLoadCommand Command 0x%lx CardSlot %ld\n", command, card);
    if(checkDmaBusy) {
        // Wait until the DMA engine is not busy. There are case, during a single
        // block read operation, where a DTO interrupt is received despite the DMA
        // engine has not completed the transaction. The transaction may not
        // terminate before this function is called, this may trigger unpredicted
        // behaviour. The following code avoid it.
        read_data = GET_REG_WORD_VAL(SDIOH1_STATUS_S1);
        if(timeOut) {
        while((read_data & (1 << DRVSDIO_STATUS_DATASTATEMCBUSY)) && --timeOut)
            read_data = GET_REG_WORD_VAL(SDIOH1_STATUS_S1);
        if(timeOut == 0)
            return DRVSDIO_TIMEOUT;
        }
        else
            while(read_data & (1 << DRVSDIO_STATUS_DATASTATEMCBUSY))
                read_data = GET_REG_WORD_VAL(SDIOH1_STATUS_S1);
    }
    // If we need to wait until the card is busy enter the Wait routine
    if(checkbusy > 0) {
        // Wait routine that check for the busy flag in the status register
        read_data = GET_REG_WORD_VAL(SDIOH1_STATUS_S1);
        while(read_data & (1 << DRVSDIO_STATUS_DATABUSY))
            read_data = GET_REG_WORD_VAL(SDIOH1_STATUS_S1);
    }
    // Prepare the command that will be send
    // Take into account card number and the use of hold register
    load_data = (command & (~((1 << DRVSDIO_CMD_USE_HOLDREGISTER) | (DRVSDIO_CMD_CARD_NUMBER_MASK << DRVSDIO_CMD_CARD_NUMBER))));
    if (drvsdioCards[card].useHoldRegister)
        load_data |= (1 << DRVSDIO_CMD_USE_HOLDREGISTER);
    load_data |= ((card & DRVSDIO_CMD_CARD_NUMBER_MASK) << DRVSDIO_CMD_CARD_NUMBER);
    DPRINTF1("\nProgramming CMD reg with 0x%lx\n", load_data);
    // After the command is put into the CMD register it is executed
    SET_REG_WORD(SDIOH1_CMD_S1, load_data);
    // Wait until the start_cmd flag is lowered or an error flag is raised.
    // Lowering the start_cmd bit in the CMD register indicates that the command was
    // sent to the card
    if(timeOut){
        while((GET_REG_WORD_VAL(SDIOH1_CMD_S1) & (1 << DRVSDIO_CMD_START_CMD)) && (!rints) && --timeOut) {
            // Get the value of the RINTSTS register to check for error flags (bits)
            if(!timeOut)
                rints = DRVSDIO_TIMEOUT;
            else
                rints = (GET_REG_WORD_VAL(SDIOH1_RINTSTS_S1) & (1 << DRVSDIO_RAWINTSTATUS_HLE));
        }
    }
    else {
        while((GET_REG_WORD_VAL(SDIOH1_CMD_S1) & (1 << DRVSDIO_CMD_START_CMD)) && (!rints)) {
            // Get the value of the RINTSTS register to check for error flags (bits)
            rints = (GET_REG_WORD_VAL(SDIOH1_RINTSTS_S1) & (1 << DRVSDIO_RAWINTSTATUS_HLE));
        }
    }
    DPRINTF1("\nDone Loading 0x%lx\n", rints);
    // Returns whether HLE has ocurred or not
    return rints;
}

/// This function convert the memory to uncached
///
/// @index[in-out] memOut The modified address
/// @param[in] memIn The original address
/// @return none
inline static void* drvSdioCnvToUncachedMem(void* memIn)
{
    // Write to bypassed addresses
   if (((u32) memIn) & DRV_DDRMASK)
       return (void*) (((u32) memIn) | DRV_DDRBYPASSMASK);
   else
       return (void*) (((u32) memIn) | DRV_CMXBYPASSMASK);
}

/// This function loads a command having an argument
///
/// After command is loaded we wait for the command to
/// complete (handling the the Command DONE flag in the RINTSTS).
///
/// @param[in] command The command that needs to be loaded
/// @param[in] arg The argument that should be used with this command
/// @param[in] checkbusy Indicates whether to wait for the card or not
/// @param[in] card This is the card number
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @param[in] checkDmaBusy Indicates whether to wait for the DMA or not
/// @return flags The value of the RINTSTS (Raw Interrupt Status Register)
static u32 drvSdioLoadAndWait(u32 command, u32 arg, u32 checkbusy, u32 card, u32 timeout, u32 checkDmaBusy)
{
    u32 flags = 0;
    // Place the argument into the argument register
    SET_REG_WORD(SDIOH1_CMDARG_S1, arg);
    // Load the command
    flags = drvSdioLoadCommand(command, checkbusy, card, timeout, checkDmaBusy);
    // Not HLE error
    if (flags == 0) {
        // Waits the command to complete or the specified error flags to be raised
        flags |= DrvSdioWaitForFlags((1 << DRVSDIO_RAWINTSTATUS_CDONE) | DRVSDIO_COMMANDDONE_ERROR_FLAGS, timeout);
    }
    DPRINTF1("\ndrvSdioLoadAndWait Command 0x%lx Arg 0x%lx Check Busy %ld Card Error %ld Flags 0x%lx\n", command, arg, checkbusy, card, flags);
    return flags;
}

/// This function loads a command with an argument
///
/// It returns the response from the card (the value of RESPONSE register - 0)
///
/// @param[in] command The command that needs to be loaded
/// @param[in] arg The argument that should be used with this command
/// @param[in] checkbusy Indicates whether to wait for the card or not
/// @param[in] card This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @param[in] checkDmaBusy Indicates whether to wait for the DMA or not
/// @return resp The value of RESPONSE register 0
static u32 drvSdioLoadAndWait_ShortResponse(u32 command, u32 arg, u32 checkbusy, u32 card, u32 *errorflags, u32 timeout, u32 checkDmaBusy)
{
    u32 resp = 0, flags = 0;
    // Load Arg
    SET_REG_WORD(SDIOH1_CMDARG_S1, arg);
    // Load Command
    flags = drvSdioLoadCommand(command, checkbusy, card, timeout, checkDmaBusy);
    // Not HLE error
    if (flags == 0) {
        // Waits the command to complete or the specified error flags to be raised
        flags |= DrvSdioWaitForFlags((1 << DRVSDIO_RAWINTSTATUS_CDONE) | DRVSDIO_COMMANDDONE_ERROR_FLAGS, timeout);
        if ((flags & DRVSDIO_COMMANDDONE_ERROR_FLAGS) == 0) {
            // No errors, get short response
            resp = GET_REG_WORD_VAL(SDIOH1_RESP0_S1);
        }
    }
    *errorflags = flags;
    DPRINTF1("\ndrvSdioLoadAndWait_ShortResponse Command 0x%lx Arg 0x%lx Check Busy %ld Card %ld ErrorFlags 0x%lx Resp 0x%lx\n", command, arg, checkbusy, card, flags, resp);
    return resp;
}

/// This function loads a command with an argument
///
/// It returns the response from the card (the value of ALL the RESPONSE registers)
///
/// @param[in] command The command that needs to be loaded
/// @param[in] arg The argument that should be used with this command
/// @param[in] checkbusy Indicates whether to wait for the card or not
/// @param[in] card This is the card number
/// @param[in] resp The values of RESPONSE registers
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @param[in] checkDmaBusy Indicates whether to wait for the DMA or not
/// @return flags The value of the RINTSTS (Raw Interrupt Status Register)
static u32 drvSdioLoadAndWait_LongResponse(u32 command, u32 arg, u32 checkbusy, u32 card, u32 *resp, u32 timeout, u32 checkDmaBusy)
{
    u32 flags = 0;

    // Load Arg
    SET_REG_WORD(SDIOH1_CMDARG_S1, arg);
    // Load Command
    flags = drvSdioLoadCommand(command, checkbusy, card, timeout, checkDmaBusy);
    // Not HLE error
    if (flags == 0) {
        // Command done? Errors ?
        flags |= DrvSdioWaitForFlags((1 << DRVSDIO_RAWINTSTATUS_CDONE) | DRVSDIO_COMMANDDONE_ERROR_FLAGS, timeout);
        if (((flags & DRVSDIO_COMMANDDONE_ERROR_FLAGS) == 0) && (resp)) {
            // No errors, get Long response
            resp[3] = swcLeonSwapU32(GET_REG_WORD_VAL(SDIOH1_RESP0_S1));
            resp[2] = swcLeonSwapU32(GET_REG_WORD_VAL(SDIOH1_RESP1_S1));
            resp[1] = swcLeonSwapU32(GET_REG_WORD_VAL(SDIOH1_RESP2_S1));
            resp[0] = swcLeonSwapU32(GET_REG_WORD_VAL(SDIOH1_RESP3_S1));
        }
    }
    DPRINTF1("\ndrvSdioLoadAndWait_LongResponse Command 0x%lx Arg 0x%lx Check Busy %ld Card ErrorFlags 0x%lx Resp 0x%lx\n", command, arg, checkbusy, card, flags);
    return flags;
}

/// This function allows to select a card.
///
/// Returns response to CMD7 and Flags contains the interrupts to be checked for errors CMD7
///
/// @param[in] relativecardaddress The relative card address
/// @param[in] cardslot This is the card number
/// @param[in] flags The value of the RINTSTS (Raw Interrupt Status Register)
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @return resp0 The value of RESPONSE register 0 or DRVSDIO_TIMEOUT
/// in case of timeout
static u32 drvSdioCardSelect(u32 relativecardaddress, u32 cardslot, u32 *flags, u32 timeout)
{
    u32 resp0 = 0, tmp = 0;
    DPRINTF1("\nSend CMD7 \n");
    // Prepare the command
    tmp = ((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (DRVSDIO_CMD7 << DRVSDIO_CMD_CMD_INDEX));
    // Send command and wait
    resp0 = drvSdioLoadAndWait_ShortResponse(tmp, (relativecardaddress << 16), TRUE, cardslot, flags, timeout, TRUE);
    DPRINTF1("\ndrvSdioCardSelect RCA 0x%lx Card Slot %ld Flags 0x%lx Resp 0x%lx\n", relativecardaddress, cardslot, *flags, resp0);
    // Return response to CMD7
    return resp0;
}

/// This function allows to deselect a card
///
/// It returns the interrupts to be checked for errors CMD7
///
/// @param[in] cardslot This is the card number
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @param[in] checkDmaBusy Indicates whether to wait for the DMA or not
/// @return flags The value of the RINTSTS (Raw Interrupt Status Register) or
/// DRVSDIO_TIMEOUT in case of timeout
static u32 drvSdioCardDeSelect(u32 cardslot, u32 timeout, u32 checkDmaBusy)
{
    // In any case wait till card is not busy. If not when erasing we may have problems. We could use polling though TBD
    u32 flags = drvSdioLoadAndWait(((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD7 << DRVSDIO_CMD_CMD_INDEX)), 1, TRUE, cardslot, timeout, checkDmaBusy);
    DPRINTF1("\ndrvSdioCardDeSelect Flags 0x%lx\n", flags);
    return flags;
}

/// This function width of bus only for SD/SDIO in bits ACMD6
///
/// @param[in] width The bus width
/// @param[in] relativecardaddress The relative card address
/// @param[in] cardslot This is the card number
/// @param[in] flags The value of the RINTSTS (Raw Interrupt Status Register)
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @return resp0 The value of RESPONSE register 0 or DRVSDIO_TIMEOUT
/// in case of timeout
static u32 drvSdioSelectBusWidth(u32 width, u32 relativecardaddress, u32 cardslot, u32 *flags, u32 timeout)
{
    u32 resp = 0, tmp = 0, iflags = 0;
    UNUSED(relativecardaddress);
    // Prepare CMD55
    // This command is executed prior CMD6 to force send ACMD6 and not CMD6
    tmp = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD55 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC));
    DPRINTF1("\nSend CMD55 \n");
    // Load Command
    iflags = drvSdioLoadAndWait(tmp, (drvsdioCards[cardslot].relativeCardAddress << DRVSDIO_R6_RCA), TRUE, cardslot, timeout, TRUE);
    if ((iflags & (DRVSDIO_COMMANDDONE_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_HLE))) == 0) {
        DPRINTF1("\nCMD6\n");
        // Prepare CMD6
        tmp = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_ACMD6 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC));
        // Load Command
        resp = drvSdioLoadAndWait_ShortResponse(tmp, width == 1? 0x00:0x2, TRUE, cardslot, &iflags, timeout, TRUE);
    }
    *flags = iflags;
    DPRINTF1("\ndrvSdioSelectBusWidth Width %ld RCA 0x%lx ErrorFlags 0x%lx Resp 0x%lx\n", width, relativecardaddress, iflags, resp);
    return resp;
}

/// This function Set Block length by SDIO registers and CMD16
///
/// @param[in] blocklength The block length
/// @param[in] bytecount The bytecount that will be used
/// @param[in] card This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @param[in] timeout The timeout value, when it is 0 the timeout is disabled
/// @return resp0 The value of RESPONSE register 0 or DRVSDIO_TIMEOUT
/// in case of timeout
static u32 drvSdioSetBlockLength(u32 blocklength, u32 bytecount, u32 card, u32 *errorflags, u32 timeout)
{
    u32 resp0 = 0;
    // Setting block length to register
    SET_REG_WORD(SDIOH1_BLKSIZ_S1, blocklength);
    // Setting byte count to register
    SET_REG_WORD(SDIOH1_BYTCNT_S1, bytecount);
    // Load Command
    resp0 = drvSdioLoadAndWait_ShortResponse(((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (DRVSDIO_CMD16 << DRVSDIO_CMD_CMD_INDEX)), blocklength, TRUE, card, errorflags, timeout, TRUE);
    DPRINTF1("\ndrvSdioSetBlockLength BlockLength 0x%lx Byte Count 0x%lx Card %ld ErrorFlags 0x%lx Resp 0x%lx\n", blocklength, bytecount, card, *errorflags, resp0);
    return resp0;
}

/// This function allows to reset FIFO
///
/// @param[in] none
/// @return none
static void drvSdioResetFIFO(void)
{
    u32 controlreg = 0;
    DPRINTF1("\nPerforming FIFO Reset\n");
    // Get the value of the Contlor register
    controlreg = GET_REG_WORD_VAL(SDIOH1_CTRL_S1);
    // Set the fifo reset bit in the Control register
    controlreg |= (0x1 << DRVSDIO_CTRL_FIFO_RESET);
    // Set back the Control register
    SET_REG_WORD(SDIOH1_CTRL_S1, controlreg);
    // Wait for reset to be performed
    while(GET_REG_WORD_VAL(SDIOH1_CTRL_S1) & (0x1 << DRVSDIO_CTRL_FIFO_RESET));
    DPRINTF1("\nFIFO Reset Done \n");
}

/// This function resets the DMA
///
/// @param[in] none
/// @return none
static void drvSdioResetDma(void)
{
    u32 controlreg = 0;
    DPRINTF1("\nPerforming FIFO Reset\n");
    // Get the value of the Contlor register
    controlreg = GET_REG_WORD_VAL(SDIOH1_CTRL_S1);
    // Set the fifo reset bit in the Control register
    controlreg |= (0x1 << DRVSDIO_CTRL_DMA_RESET);
    // Set back the Control register
    SET_REG_WORD(SDIOH1_CTRL_S1, controlreg);
    // Wait for reset to be performed
    while(GET_REG_WORD_VAL(SDIOH1_CTRL_S1) & (0x1 << DRVSDIO_CTRL_DMA_RESET));
    DPRINTF1("\nFIFO Reset Done \n");
}


/// This function fills the descriptors
///
/// @param[in] count Number of descriptors with given blocksize
/// @param[in] descpointer The address used for descriptor generation
/// @param[in] blksize The size of a block that will be read/write
/// @param[in] addr The address where data will be stored in descriptors
/// @return count The number of descriptors
static u32 drvSdioFillDescriptors(u32 count, DrvSdio_Descriptors *descpointer, u32 blksize, u32 addr)
{
    u32 i = 0;
    DrvSdio_Descriptors *bdescpointer = NULL;

    // Write to bypassed addresses
    bdescpointer = (DrvSdio_Descriptors*) drvSdioCnvToUncachedMem(descpointer);

    // Make sure buffer addresses are not bypassed
    if (addr & DRV_DDRMASK)
        addr &= ~DRV_DDRBYPASSMASK;
    else
        addr &= ~DRV_CMXBYPASSMASK;

    for (i = 0; i < count ; i ++) {
        bdescpointer[i].descriptor0 = ((1 << DRVSDIO_DES0_OWN) | (1 << DRVSDIO_DES0_CH));
        bdescpointer[i].descriptor1 = (blksize << DRVSDIO_DES1_BS1);
        bdescpointer[i].descriptor2 = addr + i * blksize;
        bdescpointer[i].descriptor3 = (u32) &descpointer[i + 1];
        DPRINTF1("\nDes %ld 0: 0x%lx 1: 0x%lx 2: 0x%lx 3: 0x%lx\n", i, descpointer[i].descriptor0, descpointer[i].descriptor1, descpointer[i].descriptor2, descpointer[i].descriptor3);
    }
    DPRINTF1("\nDescriptors %ld Start Addr 0x%lx Transfer Size %ld\n", i, addr, blksize);
    return count;
}

/// This function marks the start and end of the descriptor chain
///
/// @param[in] descpointer Pointer to descriptors in memory
/// @param[in] descriptors number of descriptors
static void drvSdioCompleteChainedDescriptor(DrvSdio_Descriptors *descpointer, u32 descriptors)
{
    DrvSdio_Descriptors *bdescpointer = NULL;
    // Write to bypassed addresses
    bdescpointer = (DrvSdio_Descriptors*) drvSdioCnvToUncachedMem(descpointer);

    if (descriptors) {
        bdescpointer[0].descriptor0 |= (1 << DRVSDIO_DES0_FS);
        bdescpointer[descriptors - 1].descriptor0 |= (1 << DRVSDIO_DES0_LD);
        bdescpointer[descriptors - 1].descriptor3 = (u32) &descpointer[0];
    }
}

/// This function allows to build chained descriptors
///
/// Sizes must be integer multiple of blksize
///
/// @param[in] descriptorbaseaddress The address used for descriptor generation
/// @param[in] transactionList List of buffers and sizes to be used consecutively
/// @param[in] transCount Length of the transaction list
/// @param[in] blksize The size of a block that will be read/write
/// @return total The number of descriptors
static u32 drvSdioBuildChainedDescriptor(u32 descriptorbaseaddress, tyDrvSdio_Transaction *transactionList, u32 transCount, u32 blksize)
{
    UNUSED(blksize); // hush the compiler warning.
    DrvSdio_Descriptors *descpointer = NULL;
    u32 count = 0, dmatransize = DRVSDIO_MAXDMATRANSACTIONSIZE, total = 0, j = 0, size = 0, addr = 0;

    descpointer = (DrvSdio_Descriptors *) descriptorbaseaddress;
    if (transCount == 1) {
         while ((transactionList[0].size < dmatransize) && (transactionList[0].size > DRVSDIO_MINDMATRANSACTIONSIZE))
            dmatransize = (dmatransize >> 1);
    }

    for (j = 0; j < transCount; j ++) {
        // The size of the data buffer for each descriptor must be a multiple of
        // blksize
        assert((transactionList[j].size % blksize) == 0);
        // Must be 32-bit aligned
        transactionList[j].buffer &= 0xFFFFFFFC;
        size = transactionList[j].size;
        addr = transactionList[j].buffer;
        do {
            count = drvSdioFillDescriptors(size / dmatransize, (DrvSdio_Descriptors *)&descpointer[total], dmatransize, addr);
            DPRINTF1("\ncount %ld size %ld dmatransize %ld\n", size / dmatransize, size, dmatransize);
            total += count;
            size -= count * dmatransize;
            addr += count * dmatransize;
            dmatransize = (dmatransize >> 1);
        } while ((dmatransize >= DRVSDIO_MINDMATRANSACTIONSIZE) && (size > 0));
        dmatransize = DRVSDIO_MAXDMATRANSACTIONSIZE;
    }
    // Handle first and last descriptors
    drvSdioCompleteChainedDescriptor(descpointer, total);
    DPRINTF1("\ndrvSdioBuildChainedDescriptor Desc Addr 0x%lx List Length %ld Descriptors %ld\n", (u32)descpointer, transCount, total);
    return total;
}

/// This function allows to work out the value to be used in div x for a desired frequency
///
/// @param[in] cclk_in_khz System clock in KHz
/// @param[in] sdfreq_khz Desired card clock frequency in KHz
/// @return res Desired value for the clock frequency divider
static u32 drvSdioCalculateDivValue(u32 cclk_in_khz, u32 sdfreq_khz)
{
    u32 res = 0;

    // Just bypass the divider if input clock is slower
    if(cclk_in_khz > sdfreq_khz)
    {
        // Calculate the divider
        sdfreq_khz = (sdfreq_khz << 1);
        if(sdfreq_khz != 0) {
          res = (cclk_in_khz/ sdfreq_khz);
          if ((cclk_in_khz % sdfreq_khz) > (u32) ((1 << res) * DRVSDIO_MAXDEVIATIONKHZ))
            res++;
        }
    }
    return res;
}

/// This function allows to change the frequency of operation in any of the 4 possible dividers
///
/// @param[in] cardslot This is the card number
/// @param[in] cclk_in_khz System clock in KHz
/// @param[in] sdfreq_khz Desired card clock frequency in KHz
/// @return none
static u32 drvSdioChangeFrequency(u32 cardslot, u32 div, u32 cclk_in_khz, u32 sdfreq_khz)
{
    u32 tmp = 0;
    u32 res = 0;
    // Work out the value for enumeration process
    // drvSdioCalculateDivValue expects sdfrequency in Hz that's why we are converting
    DPRINTF1("cclk_in_khz %ld sdfreq_khz %ld\n", cclk_in_khz, sdfreq_khz);
    tmp = drvSdioCalculateDivValue(cclk_in_khz, sdfreq_khz);
    // Disable Clock for all Cards
    SET_REG_WORD(SDIOH1_CLKENA_S1, DRVSDIO_CLKENA_DISABLE_ALL);
    // Execute change by sending a command to update clock registers
    res = drvSdioLoadCommand((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_UPDATE_CLK_REG), TRUE, cardslot, DRV_NOTIMEOUT, TRUE);
    if (res & DRVSDIO_COMMANDDONE_ERROR_FLAGS)
    {
        // There is an error updating clocks
        DPRINTF1("\ndrvSdioChangeFrequency failed to update clocks\n");
        return res;
    }
    // Set new value into the right divider
    SET_REG_WORD(SDIOH1_CLKDIV_S1, (tmp << (div << 3)));
    DPRINTF1("\nDIV %ld: %ld sdfreq_khz %ld\n",div , tmp, sdfreq_khz);
    // Cardslot is using divider x
    SET_REG_WORD(SDIOH1_CLKSRC_S1, (div << (cardslot << 1)));
    /* Enable clock for all 16 cards and Non-low-power-mode */
    SET_REG_WORD(SDIOH1_CLKENA_S1, DRVSDIO_CLKENA_ENABLE_ALL);
    // Execute change by sending a command to update clock registers
    res = drvSdioLoadCommand((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_UPDATE_CLK_REG), FALSE, cardslot, DRV_NOTIMEOUT, TRUE);
    if (res & DRVSDIO_COMMANDDONE_ERROR_FLAGS)
    {
        // There is an error updating clocks
        DPRINTF1("\ndrvSdioChangeFrequency failed to update clocks\n");
        return res;
    }
    DPRINTF1("\ndrvSdioChangeFrequency Card %ld Div %ld SD Freq (KHz) %ld\n", cardslot, div, sdfreq_khz);
    drvsdioCards[cardslot].statusFlags |= (1 << DRVSDIO_INTERNALSD_STATUS_FREQ_SWITCHED);
    return 0;
}

/// This function selects the optimal phase to shift on cclk_in_sample clock
///
/// This function receives a vector containing the values one and zero. The ones
/// represent the phase values which pass the fine tuning test, the zeros
/// represent the phase values which didn't pass the fine tuning test.
/// This function select the middle index of the biggest continue sequence of ones
/// Ex. The function will select the value in brackets:
/// Case 1) vect = {0,1,0,0,0,0,1,(1),1,0,1,1,0,1,0,0}
///
/// Case 2) vect = {1,1,0,0,0,0,1,0,1,1,1,1,0,1,1,(1)}
///
/// Case 3) vect = {0,0,1,0,0,0,1,0,1,0,0,0,0,(1),1,0}
///
/// @param[in] vect Vector containing zeros and ones
/// @param[in] sz vect's size
/// @param[out] optimalTuningPhase contains the optimal phase selected
/// @return success or fail
static DRV_RETURN_TYPE drvSdioFindOptimalTuningPhase(u8* vect, u32 sz, u32* optimalTuningPhase)
{
    u32 startingPoint = 0, i = 0, k = 0;
    u32 indexFirstOfSeq = 0;
    u32 maxSequenze = 0;
    u32 sequenzeCounter = 0;

    assert(vect != NULL);
    assert(sz != 0);
    assert(optimalTuningPhase != NULL);

    // Find the starting point, the last 1 preceded by 0
    i = sz;
    while(i > 0){
        if(vect[i - 1] == 1)
            i--;
        else{
            startingPoint = i;
            startingPoint %= sz;
            break;
        }
    }

    // Find the biggest '1' sequence and its first element index
    i = sz;
    k = startingPoint;
    while(i > 0){
        if(vect[k] == 1)
            sequenzeCounter++;
        else if(vect[k] == 0)
            sequenzeCounter = 0;
        // The vector must contain only values 1 and 0
        else
            assert(0);
        // Update maxSequenze and indexFirstOfSeq values
        if(sequenzeCounter > maxSequenze) {
            maxSequenze = sequenzeCounter;
            if(sequenzeCounter > k)
                indexFirstOfSeq = k - sequenzeCounter + sz + 1;
            else
                indexFirstOfSeq = k - sequenzeCounter + 1;
        }
        i--;
        k++;
        k %= sz;
    }

    if(maxSequenze == 0)
        return MYR_DRV_ERROR;

    // Select the optimal tuning phase value, select the central value of
    // the biggest '1' sequence
    *optimalTuningPhase = (indexFirstOfSeq + (maxSequenze >> 1) - 1 + (maxSequenze % 2)) % sz;

    return MYR_DRV_SUCCESS;
}

/// This function sets the cclk_in_sample shift phase
///
/// @param[in] cardslot Slot where the card is located 0-1
/// @return none
static void drvSdioSetPhaseShift(u8 value)
{
    u32 tmp = 0;

    tmp = GET_REG_WORD_VAL(SDIOH1_UHS_REG_EXT_S1);
    tmp &= ~(0xF << DRVSDIO_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL);
    tmp |= (value << DRVSDIO_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL);
    SET_REG_WORD(SDIOH1_UHS_REG_EXT_S1, tmp);
    // This delay value has been tested at a frequency of 200 MHz, higher
    // frequency may required a different value
    drvSdiodelay(DRVSDIO_CLK_SHIFT_PHASE_DELAY);
}

/// This function performs the fine tuning for a specific clock phase value
///
/// @param[in] cardslot Slot where the card is located 0-1
/// @index[in] index of the clock phase under test
/// @return 1 if the clock phase value passes the test, 0 otherwise
static u32 drvSdioTestFineTuningShiftFace(u32 cardslot, u32 index)
{
    u32 command = 0, resp = 0, flags = 0, arg = 0;
    u32 descriptBaseAddr = 0, tuneCount = 0, cardSelected = 0, isTuneOk = 0;
    const u32 count = 1;
    const u32 timeout = DRVSDIO_FINETUNING_TIMEOUT;
    tyDrvSdio_Transaction transactionList[1];
    static u8 tuningRdBuf[DRVSDIO_TUNING_BLOCK_SZ];
    const u32 tuningPattern[] = DRVSDIO_TUNING_PATTERN;
    void* tuningBufPtr = NULL;

    DPRINTF1("\nFine tuning index %ld",index);
    // Set the cclk_in_sample shift phase
    drvSdioSetPhaseShift(index);

    // Convert the buffer to uncached
    tuningBufPtr = drvSdioCnvToUncachedMem(tuningRdBuf);

    do{
        memset(tuningBufPtr, 0x00, DRVSDIO_TUNING_BLOCK_SZ);
        transactionList[0].size = DRVSDIO_TUNING_BLOCK_SZ;
        transactionList[0].buffer = (u32)tuningBufPtr;

        // Select the card
        resp = drvSdioCardSelect(drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, timeout);
        if(resp == DRVSDIO_TIMEOUT)
            goto TIME_OUT_ERROR;
        if (DRVSDIO_TUNING_ISTHERE_COMMAND_DONE_ERROR(flags) != 0){
            DPRINTF1("\nError selecting the card. flags:%lx response:%lx", flags, resp);
            goto ERROR;
        }
        cardSelected = 1;

        // Set bus width
        resp = drvSdioSelectBusWidth(drvsdioCards[cardslot].busWidth, drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, timeout);
        if(resp == DRVSDIO_TIMEOUT)
            goto TIME_OUT_ERROR;
        if (DRVSDIO_TUNING_ISTHERE_COMMAND_DONE_ERROR(flags) != 0){
            DPRINTF1("\nError setting the width. flags:%lx response:%lx", flags, resp);
            goto ERROR;
        }

        // Set block length
        resp = drvSdioSetBlockLength(DRVSDIO_TUNING_BLOCK_TX_SZ, drvSdioGetScatteredTransactionSize(transactionList,count), cardslot, &flags, timeout);
        if(resp == DRVSDIO_TIMEOUT)
            goto TIME_OUT_ERROR;
        if (DRVSDIO_TUNING_ISTHERE_COMMAND_DONE_ERROR(flags) != 0){
            DPRINTF1("\nError setting the block length. flags:%lx response:%lx", flags, resp);
            goto ERROR;
        }

        // Fifo needs to be reset before reading
        drvSdioResetFIFO();
        // Get Descriptors base address
        descriptBaseAddr = GET_REG_WORD_VAL(SDIOH1_DBADDR_S1);
        // Create the descriptors
        resp = drvSdioBuildChainedDescriptor(descriptBaseAddr, transactionList, count, DRVSDIO_TUNING_BLOCK_TX_SZ);
        UNUSED(resp);  // avoid 'unused variable' error from scan build
        // Send the CMD19(fine tuning command)
        command = (1 << DRVSDIO_CMD_START_CMD) |
                (DRVSDIO_CMD19 << DRVSDIO_CMD_CMD_INDEX) |
                (1 << DRVSDIO_CMD_CHECK_RESP_CRC) |
                (1 << DRVSDIO_CMD_RESP_EXPECT) |
                (1 << DRVSDIO_CMD_DATA_EXPECTED) |
                (0 << DRVSDIO_CMD_TRANSFER_MODE) |
                (1 << DRVSDIO_CMD_WAIT_PREVDATA_CMPL) |
                (1 << DRVSDIO_CMD_SEND_AUTO_STOP);
        arg =  0;
        resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, cardslot, &flags, timeout, TRUE);
        if(resp == DRVSDIO_TIMEOUT)
            goto TIME_OUT_ERROR;
        if (DRVSDIO_TUNING_ISTHERE_COMMAND_DONE_ERROR(flags) != 0) {
            DPRINTF1("\nError loading CMD19. flags:%lx response:%lx", flags, resp);
            goto ERROR;
        }
        // Read error check
        if ((resp & ((1 << DRVSDIO_CARD_STATUS_OUT_OF_RANGE) |
                (1 << DRVSDIO_CARD_STATUS_ADDRESS_ERROR) |
                (1 << DRVSDIO_CARD_STATUS_CARD_LOCKED) |
                (1 << DRVSDIO_CARD_STATUS_ILLEGAL_COMMAND))) != 0) {
            DPRINTF1("\nError in reading operation. flags:%lx response:%lx", flags, resp);
            goto ERROR;
        }
        // Wait for data transfer errors or data transfer over
        flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_DTO), timeout);
        if(flags == DRVSDIO_TIMEOUT)
            goto TIME_OUT_ERROR;
        if (!(flags & (1 << DRVSDIO_RAWINTSTATUS_DTO)) || ((flags & DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS) != 0 || flags == DRVSDIO_TIMEOUT)){
            DPRINTF1("\nError in reading operation. flags:%lx response:%lx", flags, resp);
            goto ERROR;
        }
        // Wait for AC Done
        flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_ACD), timeout);
        if(flags == DRVSDIO_TIMEOUT)
            goto TIME_OUT_ERROR;
        if (!(flags & (1 << DRVSDIO_RAWINTSTATUS_ACD)) || ((flags & DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS) != 0 || flags == DRVSDIO_TIMEOUT)){
            DPRINTF1("\nError waiting for ACD flags:%lx response:%lx", flags, resp);
            goto ERROR;
        }
        // Check the received tuning pattern
        if(memcmp(tuningPattern, tuningBufPtr, DRVSDIO_TUNING_BLOCK_TX_SZ)){
            DPRINTF1("\nError, wrong tuning pattern");
            goto ERROR;
        }

        // No previous errors, the fine tuning succeed
        isTuneOk = TRUE;
        DPRINTF1("\nFine tuning results: %lx Response value: %lx", flags, resp);
ERROR:
        // The host controller may generate a response time-out interrupt, it must be cleaned
        // This function is relying on the hack done in the ICB driver to switch
        // between bare-metal and RTEMS. There is no way to workaround it unless
        // we write an Hgl layer that will be done in 2x8x
        DRVSDIO_CLEARFLAGS(DRVSDIO_RAWINTSTATUS_RTO);

        // Send the CMD12 (Switch the SD card in idle state)
        flags = 0;
        command = (1 << DRVSDIO_CMD_START_CMD) |
                  (1 << DRVSDIO_CMD_RESP_EXPECT) |
                (DRVSDIO_CMD12 << DRVSDIO_CMD_CMD_INDEX) |
                (1 << DRVSDIO_CMD_STOP_ABORT_CMD);

        resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, 1, &flags, timeout, FALSE);
        if(resp == DRVSDIO_TIMEOUT)
            goto TIME_OUT_ERROR;

        if(cardSelected){
            resp = drvSdioCardDeSelect(cardslot, timeout, FALSE);
            if(resp == DRVSDIO_TIMEOUT)
                goto TIME_OUT_ERROR;
            DPRINTF1("\nDe-select card. Response: %lx", resp);
            cardSelected = 0;
        }
        // Clear any pending interrupt in case of error
        // This function is relying on the hack done in the ICB driver to switch
        // between bare-metal and RTEMS. There is no way to workaround it unless
        // we write an Hgl layer that will be done in 2x8x
        DRVSDIO_CLEARFLAGS(0xFFFF);

        tuneCount++;

    }while((!isTuneOk) && tuneCount < DRVSDIO_TUNING_MAX_LOOP);

    return isTuneOk;
TIME_OUT_ERROR:
    return DRVSDIO_TIMEOUT;
}

/// This function performs the fine tuning
///
/// This function sets the correct phase to shift on cclk_in_sample clock
///
/// @param[in] cardslot Slot where the card is located 0-1
/// @return success or fail
static DRV_RETURN_TYPE drvSdioFineTuning(u32 cardslot)
{
    u32 resp = 0, index = 0;
    u32 optimalPhase = 0;
    u8 tuningCorrectValues[DRVSDIO_TUNING_DELAY_RANGE];

    // Program FIFO threshold
    drvSdioProgramFIFO(DRVSDIO_TUNING_DMASIZE, DRVSDIO_TUNING_RX_WMARK, DRVSDIO_TUNING_TX_WMARK);

    while(index < DRVSDIO_TUNING_DELAY_RANGE)
    {
        resp = drvSdioTestFineTuningShiftFace(cardslot, index);
        if(resp == 1){
            DPRINTF1("\nDelay %lx ok", index);
            tuningCorrectValues[index] = 1;
        }
        else if(resp == 0)
            tuningCorrectValues[index] =  0;
        else
            goto TIME_OUT_ERROR;
        index++;
    }

    // Program FIFO threshold to default values
    drvSdioProgramFIFO(DRVSDIO_DMASIZE, DRVSDIO_RX_WMARK, DRVSDIO_TX_WMARK);

    // Resets the FIFO
    drvSdioResetFIFO();
    // Reset DMA
    drvSdioResetDma();

    // Find the optimal phase
    resp = drvSdioFindOptimalTuningPhase(tuningCorrectValues, DRVSDIO_TUNING_DELAY_RANGE, &optimalPhase);
    if(resp != DRVSDIO_SUCCESS) {
        //Set the cclk_in_sample shift phase
        drvSdioSetPhaseShift(0);
        return DRVSDIO_ERROR;
    }
#ifdef DRVSDIO_DEBUG
    DPRINTF1("\nFine tune vector:");
    u32 j = 0;
    while(j < 16){
        DPRINTF1("%01x:",tuningCorrectValues[j] );
        j++;
    }
    DPRINTF1("\nOptimal phase: %ld", optimalPhase);
#endif

    // Set the optimal cclk_in_sample shift phase
    drvSdioSetPhaseShift(optimalPhase);

    return DRVSDIO_SUCCESS;

TIME_OUT_ERROR:
    return DRVSDIO_TIMEOUT;
}

static uint32_t drvSdioVoltageSwitch(u32 cardslot, DrvSdioVoltSwitchFunc voltSwitch)
{
    u32 flags, i, ret;
    u32 cmdUpdateClk = 1 << DRVSDIO_CMD_START_CMD
            |          1 << DRVSDIO_CMD_UPDATE_CLK_REG
            ;
    u32 cmdVoltSwitch =   1 << DRVSDIO_CMD_START_CMD
            | DRVSDIO_CMD11 << DRVSDIO_CMD_CMD_INDEX
            |             1 << DRVSDIO_CMD_RESP_EXPECT
            |             1 << DRVSDIO_CMD_CHECK_RESP_CRC
            |             1 << DRVSDIO_CMD_VOLT_SWITCH
            ;
    // Exit from low power mode, for SDCLK to not be stopped during voltage switch
    CLR_REG_BITS_MASK(SDIOH1_CLKENA_S1, 1 << (cardslot + DRVSDIO_CLKENA_LOWPOWER_SDIO0));
    flags = drvSdioLoadCommand(cmdUpdateClk, TRUE, cardslot, DRV_NOTIMEOUT, TRUE);
    if(DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) != 0)
        return flags;

    // Begin voltage switch procedure
    DPRINTF1("\nCMD11\n");
    flags = drvSdioLoadCommand(cmdVoltSwitch, FALSE, cardslot, DRV_NOTIMEOUT, TRUE);
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0)
    {
        // Wait for first volt switch interrupt
        // TODO: A 2ms timeout should be implemented
        DrvSdioWaitForFlags(1 << DRVSDIO_RAWINTSTATUS_VOLTSW, DRV_NOTIMEOUT);

        // Wait for SD host FSM (card clock 400kHz) ???
        for(i = 0; i < 164 * 10; i++) // 164 needed at 600MHz, *10 for coverage
            NOP;

        // Stop supplying the clock. Volt_switch flag is needed for proper state machine operation
        CLR_REG_BITS_MASK(SDIOH1_CLKENA_S1, 1 << cardslot);
        flags = drvSdioLoadCommand(cmdUpdateClk | 1 << DRVSDIO_CMD_VOLT_SWITCH, FALSE, cardslot, DRV_NOTIMEOUT, TRUE);
        if(DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) != 0)
            return flags;

        // Change voltage to 1.8V, using provided function
        ret = voltSwitch(DRVSDIO_1v8);
        if(ret)
        {
            DPRINTF1("\nVoltage switch failed\n");
            return ret;
        }

        // Wait for voltage to stabilize (should be 5ms)
        // TODO: add a proper ms delay function !!!
        drvSdiodelay(DRVSDIO_VOLTAGE_SW_DELAY);
        // Set the regulator for 1v8. We have external voltage control, so not needed
        // SET_REG_BITS_MASK(SDIOH1_UHS_S1, 1 << (cardslot + DRVSDIO_UHS_REG_VOLT_REG));
        // Start again the clock
        SET_REG_BITS_MASK(SDIOH1_CLKENA_S1, 1 << cardslot);
        flags = drvSdioLoadCommand( cmdUpdateClk | 1 << DRVSDIO_CMD_VOLT_SWITCH, FALSE, cardslot, DRV_NOTIMEOUT, TRUE);
        if(DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) != 0)
            return flags;
        // Wait for the second volt switch interrupt and for command done
        // TODO: A 1ms timeout should be implemented
        DrvSdioWaitForFlags(1 << DRVSDIO_RAWINTSTATUS_VOLTSW, DRV_NOTIMEOUT);
        DrvSdioWaitForFlags(1 << DRVSDIO_RAWINTSTATUS_CDONE, DRV_NOTIMEOUT);
    }
    else
    {
        DPRINTF1("\nCMD11 failed\n");
        return flags;
    }
    return 0;
}

/// This function returns the maximum speed mode
///
/// This function returns the maximum sdio speed mode, the value may change after
/// after calling DrvSdioEnumerate
///
/// @param[in] cardslot Slot where the card is located 0-1
/// @return sdio spped mode
tyDrvSdioSpeedMode DrvGetMaxSpeed(u32 cardslot)
{
    return drvsdioCards[cardslot].card_opts.max_speed_mode;
}

DRV_RETURN_TYPE DrvSdioEnumerate(u32 cardslot, u32 *errorflags)
{
    u32 tmp = 0, command = 0, arg = 0, flags = 0, cardTransferSpeed = 0, i = 0;
    u32 __attribute__((unused)) resp = 0;  // avoid 'unused variable' error from scan build
    // Just in case
    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    if(drvsdioCards[cardslot].card_opts.max_speed_mode == SDIO_INVALID_SPEED_MODE)
        return DRVSDIO_ERROR;
    if(drvsdioCards[cardslot].card_opts.max_speed_mode == SDIO_DDR50) // Speed mode not supported
        drvsdioCards[cardslot].card_opts.max_speed_mode = SDIO_HIGHEST_SPEEDMODE_SUPPORTED;

    if ((tyDrvSdio_Driver.status & (1 << DRVSDIO_INTERNALSD_STATUS_FREQ_SWITCHED))) {
        // Use Hold register by default
        drvsdioCards[cardslot].useHoldRegister = TRUE;
        DPRINTF1("\nEnumerating Card in Slot %ld \n", cardslot);
        // Clear any pending interrupts
        SET_REG_WORD(SDIOH1_RINTSTS_S1, DRVSDIO_RAWINTSTATUS_ALL);

        DPRINTF1("\nCMD0\n");
        // Prepare command
        command = (1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD0 << DRVSDIO_CMD_CMD_INDEX);
        // No args for CMD0
        arg = 0;
        // Load command
        tmp = drvSdioLoadAndWait(command, arg, FALSE, cardslot, DRV_NOTIMEOUT, TRUE);

        // Check for errors
        if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(tmp) == 0)
        {
            // CMD8 may fail sometimes when fast cards are used. That's why we need to repeated again
            for (i = 0; i < 4; i++)
            {
                // Checks if the card supports high Capacity SD2.0
                DPRINTF1("\nCMD8\n");
                // Prepare command
                command = (1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (DRVSDIO_CMD8 << DRVSDIO_CMD_CMD_INDEX);
                // Pass voltage window as parameter to CMD8
                arg = ((DRVSDIO_CMD8_ARG_DEFAULT_PATTERN << DRVSDIO_CMD8_ARG_PATTERN) | (DRVSDIO_CMD8_ARG_V_2_7__3_6 << DRVSDIO_CMD8_ARG_VOLTAGE));
                // Load command
                resp = drvSdioLoadAndWait_ShortResponse(command, arg, FALSE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
                // If succeed, CMD8 should echo the voltage window passed as argument to it
                DPRINTF1("\nCMD8 flags 0x%lx   arg 0x%lx  error 0x%lx\n", flags, arg, DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags));
                if ((DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) && (resp == arg))
                    break;
            }
            // Check for Errors
            if ((DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) && (resp == arg)) {
                resp = 0;

                DPRINTF1("\nCMD55 11\n");
                // Prepare command. Need to be performed prior ACMD41
                command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD55 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC));
                // No args for CMD55
                arg = 0;
                // Load command
                resp = drvSdioLoadAndWait_ShortResponse(command, arg, FALSE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
                // Check for Errors
                if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
                    DPRINTF1("\nACMD41  11\n");
                    // Prepare command
                    // DO NOT USE CRC BECAUSE IT WILL ALWAYS FAIL !!!!!!!!!
                    command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD41 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT));
                    // Arg should be 0
                    arg = 0x0000;
                    // Load command
                    resp = drvSdioLoadAndWait_ShortResponse(command, arg, FALSE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
                }

                // Need a proper response to ACMD41 as well a no errors
                while(((resp & (1 << DRVSDIO_ACMD41_ARG_BUSY)) == 0) && (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0)) {
                    DPRINTF1("\nCMD55\n");
                    // Prepare command. Need to be performed prior CMD41
                    command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD55 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC));
                    // No args for CMD55
                    arg = 0;
                    // Load command
                    resp = drvSdioLoadAndWait_ShortResponse(command, arg, FALSE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
                    // Check for Errors
                    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
                        DPRINTF1("\nACMD41\n");
                        // DO NOT USE CRC BECAUSE IT WILL ALWAYS FAIL !!!!!!!!!
                        // Prepare command.
                        command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD41 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT));
                        // Set flags for High Speed, Voltage switch to check if those options are supported by the card
                        arg = ((1 << DRVSDIO_ACMD41_ARG_HCS)  | (1 << DRVSDIO_ACMD41_ARG_OCR_3_2__3_3) | (1 << DRVSDIO_ACMD41_ARG_XPC) | (1 << DRVSDIO_ACMD41_ARG_S18R) | (DRVSDIO_HOST_VOLT_RANGE));
                        // Load command
                        resp = drvSdioLoadAndWait_ShortResponse(command, arg, FALSE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
                    }
                }
                // Check for Errors
                if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
                // Use 4-bit width by default
                    drvsdioCards[cardslot].busWidth = 4;
                    if (resp & (1 << DRVSDIO_R3_CCS)) {
                        DPRINTF1("\nThe CCS bit set in response for ACMD 41 shows that card is a High Density SD-MEM card %ld  resp: 0x%lx\n", cardslot, resp);
                        // If bit S18A is set then the card supports voltage switch
                        if (resp & (1 << DRVSDIO_R3_S18A)) {
                            DPRINTF1("\nCard supports voltage switching to 1.8V \n");
                            if (drvsdioCards[cardslot].card_opts.max_speed_mode > SDIO_HIGH_SPEED     && // Excludes default speed and high speed 3.3V
                                    drvsdioCards[cardslot].card_opts.callback) // If user provided a voltage switch function
                            {
                                tmp = drvSdioVoltageSwitch(cardslot, drvsdioCards[cardslot].card_opts.callback);
                                if (tmp) {
                                    DPRINTF1("\nVoltage switch error !\n");
                                    *errorflags = tmp;
                                    return DRVSDIO_ERROR;
                                }
                            }
                            else {
                                // In this case the card support UHS, the maximum speed is an UHS,
                                // but there is not voltage switch call back function to switch to 1.8V
                                if(drvsdioCards[cardslot].card_opts.max_speed_mode > SDIO_HIGH_SPEED &&
                                        drvsdioCards[cardslot].card_opts.callback == NULL)
                                    drvsdioCards[cardslot].card_opts.max_speed_mode = SDIO_HIGH_SPEED;

                                DPRINTF1("\nVoltage switch callback function not provided \nMoving on.\n");
                            }
                        }
                        else {
                            DPRINTF1("\nCard does not support voltage switch to 1.8V or was previously switched\n");
                            // In this case the card doesn't support UHS, but the maximum speed is an UHS
                            if(drvsdioCards[cardslot].card_opts.max_speed_mode > SDIO_HIGH_SPEED)
                                drvsdioCards[cardslot].card_opts.max_speed_mode = SDIO_HIGH_SPEED;
                        }
                        drvsdioCards[cardslot].sdhc = TRUE;
                    }
                    else {
                        if (drvsdioCards[cardslot].card_opts.max_speed_mode > SDIO_HIGH_SPEED)
                            drvsdioCards[cardslot].card_opts.max_speed_mode = SDIO_HIGH_SPEED; //The card is not High Density SD card, it doesn't support UHS modes
                        drvsdioCards[cardslot].sdhc = FALSE;
                        DPRINTF1("\nThe CCS bit set to 0 in response for ACMD 41 shows that card is a Low Density SD-MEM card %ld\n", cardslot);
                    }
                    /* Asks the Card ID */
                    DPRINTF1("\nCMD2\n");
                    // Prepare command
                    command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD2 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (1 << DRVSDIO_CMD_RESP_LENGTH) | (1 << DRVSDIO_CMD_RESP_EXPECT));
                    // No args for CMD2
                    arg = 0;
                    // Load command
                    flags = drvSdioLoadAndWait_LongResponse(command, arg, FALSE, cardslot, drvsdioCards[cardslot].cid, DRV_NOTIMEOUT, TRUE);
                    // Check for errors
                    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
                        // We should do something with this information TBD
                        while ((drvsdioCards[cardslot].relativeCardAddress == 0) && (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0)) {
                            /* Assign a relative address to the card */
                            DPRINTF1("\nCMD3\n");
                            // Prepare command
                            command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD3 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC));
                            // No args for CMD3
                            arg = 0;
                            // Load command
                            resp = drvSdioLoadAndWait_ShortResponse(command, arg, FALSE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
                            if ((DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) && (resp >> DRVSDIO_R6_RCA))
                                drvsdioCards[cardslot].relativeCardAddress = (resp >> DRVSDIO_R6_RCA);
                        }
                        if (drvsdioCards[cardslot].relativeCardAddress > 0) {
                            // Change the speed mode
                            if(drvsdioCards[cardslot].card_opts.max_speed_mode != SDIO_DEFAULT_SPEED) {
                                drvSdioSwitchToHighSpeedMode(cardslot, drvsdioCards[cardslot].card_opts.max_speed_mode, &flags);
                                if ((DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) != 0)) {
                                    DPRINTF1("\nCMD6 failed to switch to High-Speed mode\n");
                                }
                            }
                        }

                        if ((drvsdioCards[cardslot].relativeCardAddress > 0) && (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0)) {
                            // Requires the card specific data containing all the card information
                            DPRINTF1("\nCMD9\n");
                            // Prepare command
                            command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD9 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (1 << DRVSDIO_CMD_RESP_LENGTH) | (1 << DRVSDIO_CMD_RESP_EXPECT));
                            // Relative address as arg
                            arg = (drvsdioCards[cardslot].relativeCardAddress << DRVSDIO_R6_RCA);
                            // Load command
                            flags = drvSdioLoadAndWait_LongResponse(command, arg, FALSE, cardslot, drvsdioCards[cardslot].csd, DRV_NOTIMEOUT, TRUE);
                            // Force it here TBD
                            drvsdioCards[cardslot].blockSize = DRVSDIO_DEFAULTBLOCKSIZE;
                            // The block size must be 512, 1024, 4096
                            if(drvsdioCards[cardslot].blockSize != 512 &&
                                    drvsdioCards[cardslot].blockSize != 1024 &&
                                    drvsdioCards[cardslot].blockSize != 4096) {
                                DPRINTF1("\nError, blksize must be 512, 1024 or 4096");
                                return DRVSDIO_ERROR;
                            }
                            // The card is now initialized and enumerated
                            // Switch to working clock frequency. The working speed is got from the CSD card register.
                            tmp = DrvSdioGetCardTransferSpeed(cardslot, &cardTransferSpeed);
                            if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(tmp) == 0)
                            {
                                tmp = drvSdioChangeFrequency(cardslot, 0, tyDrvSdio_Driver.cclk_in_khz, cardTransferSpeed);
                                if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(tmp) != 0)
                                {
                                    DPRINTF1("\nError changing frequency\n");
                                    *errorflags = flags;
                                    return DRVSDIO_ERROR;
                                }
                            }
                        }
                        else {
                            DPRINTF1("\nError in CMD3 !  relCardAddr 0x%x\n", drvsdioCards[cardslot].relativeCardAddress);
                            *errorflags = flags;
                            return DRVSDIO_ERROR;
                        }
                    }
                    else {
                        DPRINTF1("\nError in CMD2 !\n");
                        *errorflags = flags;
                        return DRVSDIO_ERROR;
                    }
                }
                else {
                    DPRINTF1("\nError in CMD41 !\n");
                    *errorflags = flags;
                    return DRVSDIO_ERROR;
                }
            }
            else {
                DPRINTF1("\nNO ANSWER TO CMD8 !!!!!!\n");
                *errorflags = flags;
                return DRVSDIO_ERROR;
            }
        }
        else {
            DPRINTF1("\nError in CMD0 !\n");
            *errorflags = flags;
            return DRVSDIO_ERROR;
        }
        *errorflags = flags;
        DPRINTF1("\nDrvSdioEnumerate Slot %ld RCA 0x%x Flags 0x%lx resp 0x%lx\n", cardslot, drvsdioCards[cardslot].relativeCardAddress, flags, resp);

        // Execute fine tuning
        if(drvsdioCards[cardslot].card_opts.max_speed_mode == SDIO_SDR50 ||
                    drvsdioCards[cardslot].card_opts.max_speed_mode == SDIO_SDR104 ||
                    drvsdioCards[cardslot].card_opts.max_speed_mode == SDIO_DDR50) {
            resp = drvSdioFineTuning(cardslot);
            if(resp != DRVSDIO_SUCCESS)
                return resp;
        }
        return DRVSDIO_SUCCESS;
    }

    return DRVSDIO_ERROR;
}

/// Start a Read transaction on a card which has been previously configured
///
/// @param[in] blocknumber Number of blocks to read
/// @param[in] cardslot This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @return resp The value of RESPONSE register 0
static u32 drvSdioStartSingleBlockRead(u32 blocknumber, u32 cardslot, u32 *errorflags)
{
    u32 command = 0, arg = 0, flags = 0, resp = 0;
    DPRINTF1("\nCMD17\n");
    // Prepare command CMD17 for a single block read
    command = ((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (1 << DRVSDIO_CMD_DATA_EXPECTED) | (DRVSDIO_CMD17 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_WAIT_PREVDATA_CMPL) | (1 << DRVSDIO_CMD_READ_CEATA_DEVICE));
    // Set the number of blocks to read as argument to the read command
    arg = blocknumber;
    // Load command
    resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);

    // Check for standard errors
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        // Wait for Data Transfer errors or data transfer over only if response was ok, otherwise we may wait for ever
        // TBD If we need to check for more errors
        if ((resp & ((1 << DRVSDIO_CARD_STATUS_OUT_OF_RANGE) | (1 << DRVSDIO_CARD_STATUS_ADDRESS_ERROR) | (1 << DRVSDIO_CARD_STATUS_CARD_LOCKED) | (1 << DRVSDIO_CARD_STATUS_ILLEGAL_COMMAND))) == 0)
            flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_DTO), DRV_NOTIMEOUT);
        else
            flags |= (1 << DRVSDIO_R1_ERROR_FLAG);
    }
    DPRINTF1("\ndrvSdioStartSingleBlockRead Block 0x%lx ErrorFlags 0x%lx Res 0x%lx\n", blocknumber, flags, resp);
    *errorflags = flags;
    return resp;
}

/// Start a Single Write transaction on a card which has been previously configured
///
/// @param[in] blocknumber Number of blocks to write
/// @param[in] cardslot This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @return resp The value of RESPONSE register 0
static u32 drvSdioStartSingleBlockWrite(u32 blocknumber, u32 cardslot, u32 *errorflags)
{
    u32 command = 0, arg = 0, flags = 0, resp = 0;
    DPRINTF1("\nCMD24\n");
    // Prepare command CMD24 for a single block write
    command = ((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_WAIT_PREVDATA_CMPL) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (1 << DRVSDIO_CMD_DATA_EXPECTED) | (1 << DRVSDIO_CMD_READ_WRITE) | (DRVSDIO_CMD24 << DRVSDIO_CMD_CMD_INDEX));
    // Set the number of blocks to read as argument to the read command
    arg = blocknumber;
    // Load command
    resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
    // Check for standard errors
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        // Wait for Data Transfer errors or data transfer over only if response was ok, otherwise we may wait for ever
        // TBD If we need to check for more errors
        if ((resp & ((1 << DRVSDIO_CARD_STATUS_OUT_OF_RANGE) | (1 << DRVSDIO_CARD_STATUS_ADDRESS_ERROR) | (1 << DRVSDIO_CARD_STATUS_CARD_LOCKED) | (1 << DRVSDIO_CARD_STATUS_ILLEGAL_COMMAND))) == 0) {
            flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_DTO), DRV_NOTIMEOUT);
            // Need to wait for Busy Clear Interrupt after DTO
            if ((flags & (1 << DRVSDIO_RAWINTSTATUS_DTO)) && ((flags & DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS) == 0))
                flags |= DrvSdioWaitForFlags((1 << DRVSDIO_RAWINTSTATUS_BCI), DRV_NOTIMEOUT);
        }
        else
            flags |= ( 1 << DRVSDIO_R1_ERROR_FLAG);
    }
    DPRINTF1("\ndrvSdioStartSingleBlockWrite Block 0x%lx ErrorFlags 0x%lx Res 0x%lx\n", blocknumber, flags, resp);
    *errorflags = flags;
    return resp;
}

/// Start a Multiple Write transaction on a card which has been previously configured
///
/// @param[in] startblock Offset in blocks where the writing will start on the card
/// @param[in] blockcount Number of blocks to write
/// @param[in] cardslot This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @return resp The value of RESPONSE register 0
static u32 drvSdioStartMultipleBlockWrite(u32 startblock, u32 blockcount, u32 cardslot, u32 *errorflags)
{
    u32 command = 0, flags = 0, resp = 0;
    u32 __attribute__((unused)) arg;
    arg = 0;
    // Performing CMD55 prior CMD23 makes CMD23 -> ACMD23
    DPRINTF1("\nSend CMD55 \n");
    // Prepare command
    command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD55 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC));
    // No arg for CMD55
    arg = 0;
    // Load command
    flags = drvSdioLoadAndWait(command, (drvsdioCards[cardslot].relativeCardAddress << DRVSDIO_R6_RCA), FALSE, cardslot, DRV_NOTIMEOUT, TRUE);
    // Check for Errors
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        DPRINTF1("\nACMD23\n");
        // Prepare command. ACMD23 will pre-erase blocks before writing. This should speed-up the writing procedure for multiple blocks
        command = ((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (DRVSDIO_CMD23 << DRVSDIO_CMD_CMD_INDEX));
        // Set number of blocks that will be pre-erased
        arg = blockcount;
        // Load command
        resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
    }
//FIXME: shouldn't this be an elif
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        DPRINTF1("\nCMD25\n");
        // Prepare command for multi block writing
        command = ((1 << DRVSDIO_CMD_SEND_AUTO_STOP) | (1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_WAIT_PREVDATA_CMPL) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (1 << DRVSDIO_CMD_DATA_EXPECTED) | (1 << DRVSDIO_CMD_READ_WRITE) | (DRVSDIO_CMD25 << DRVSDIO_CMD_CMD_INDEX));
        // Set the start block as argument
        arg = startblock;
        // Load command
        resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
        if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
            // Wait for Data Transfer errors or data transfer over only if response is OK, otherwise we may wait for ever
            // TBD If we need to check for more errors
            if ((resp & ((1 << DRVSDIO_CARD_STATUS_OUT_OF_RANGE) | (1 << DRVSDIO_CARD_STATUS_ADDRESS_ERROR) | (1 << DRVSDIO_CARD_STATUS_CARD_LOCKED) | (1 << DRVSDIO_CARD_STATUS_ILLEGAL_COMMAND))) == 0) {
                flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_DTO) | (1 << DRVSDIO_RAWINTSTATUS_HTO), DRV_NOTIMEOUT);
                // Need to wait for Busy Clear Interrupt after DTO
                if (flags & (DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_DTO))) {
                    flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_BCI), DRV_NOTIMEOUT);
                    if ((flags & (1 << DRVSDIO_RAWINTSTATUS_BCI)) && ((flags & DRVSDIO_DATATRANSFER_WR_ERROR_FLAGS) == 0))
                        flags |= DrvSdioWaitForFlags((1 << DRVSDIO_RAWINTSTATUS_ACD), DRV_NOTIMEOUT);
                }
            }
            else
                flags |= (1 << DRVSDIO_R1_ERROR_FLAG);
        }
    }

    *errorflags = flags;
    DPRINTF1("\ndrvSdioStartMultipleBlockWrite Start %ld Count %ld Card %ld Error Flags 0x%lx Resp 0x%lx\n", startblock, blockcount, cardslot, flags, resp);
    return resp;
}

///  Start a Multiple Read transaction on a card which has been previously configured
///
/// @param[in] startblock Offset in blocks where the writing will start on the card
/// @param[in] blockcount Number of blocks to write
/// @param[in] cardslot This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @return resp The value of RESPONSE register 0
static u32 drvSdioStartMultipleBlockRead(u32 startblock, u32 blockcount, u32 cardslot, u32 *errorflags)
{
    UNUSED(blockcount);
    u32 command = 0, arg = 0, flags = 0, resp = 0;
    DPRINTF1("\nCMD18\n");
    // Prepare command
    command = ((1 << DRVSDIO_CMD_SEND_AUTO_STOP) | (1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (1 << DRVSDIO_CMD_DATA_EXPECTED) | (1 << DRVSDIO_CMD_WAIT_PREVDATA_CMPL) | (DRVSDIO_CMD18 << DRVSDIO_CMD_CMD_INDEX));
    // Set the start block as argument
    arg = startblock;
    // Load command
    resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        // TBD If we need to check for more errors
        if ((resp & ((1 << DRVSDIO_CARD_STATUS_OUT_OF_RANGE) | (1 << DRVSDIO_CARD_STATUS_ADDRESS_ERROR) | (1 << DRVSDIO_CARD_STATUS_CARD_LOCKED) | (1 << DRVSDIO_CARD_STATUS_ILLEGAL_COMMAND))) == 0) {
            // Wait for Data Transfer errors or data transfer over
            flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_DTO), DRV_NOTIMEOUT);
            if ((flags & (1 << DRVSDIO_RAWINTSTATUS_DTO)) && ((flags & DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS) == 0)) {
                // Wait for AC Done
                flags |= DrvSdioWaitForFlags(DRVSDIO_DATATRANSFER_RD_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_ACD), DRV_NOTIMEOUT);
            }
        }
        else
            flags |= (1 << DRVSDIO_R1_ERROR_FLAG);
    }
    *errorflags = flags;
    DPRINTF1("\ndrvSdioStartMultipleBlockRead Start %ld Count %ld Card %ld Error Flags 0x%lx Resp 0x%lx\n", startblock, blockcount, cardslot, flags, resp);
    return resp;
}

/// Allows to erase blocks from the card
///
/// Card must be selected (transfer mode )!!!! CMD 32 CMD33 and CMD38 to execute operations.
/// Resp contains the sd card status (R1/R1b)
///
/// @param[in] startblock Offset in blocks where the writing will start on the card
/// @param[in] blockcount Number of blocks to write
/// @param[in] cardslot This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @return resp Contains the sd card status (R1/R1b)
static u32 drvSdioEraseBlocks(u32 firstblock, u32 lastblock, u32 card, u32 *errorflags)
{
    u32 iflags = 0, tmp = 0, resp = 0;
    DPRINTF1("\nCMD32\n");
    // Support for SD cards
    if (drvsdioCards[card].sdhc == 0) {
        firstblock *= drvsdioCards[card].blockSize;
        lastblock *= drvsdioCards[card].blockSize;
    }
    // Prepare and Load command
    tmp = ((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (DRVSDIO_CMD32 << DRVSDIO_CMD_CMD_INDEX));
    resp = drvSdioLoadAndWait_ShortResponse(tmp, firstblock, TRUE, card, &iflags, DRV_NOTIMEOUT, TRUE);
    // Simple check for errors
    if (((iflags & (DRVSDIO_COMMANDDONE_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_HLE))) == 0) && ((resp & DRVSDIO_CARD_STATUS_ERASESEQ_FLAGS_MASK) == 0)) {
        DPRINTF1("\nCMD33\n");
        // Prepare and Load command
        tmp = ((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (DRVSDIO_CMD33 << DRVSDIO_CMD_CMD_INDEX));
        resp = drvSdioLoadAndWait_ShortResponse(tmp, lastblock, TRUE, card, &iflags, DRV_NOTIMEOUT, TRUE);
        // Simple check for errors
        if (((iflags & (DRVSDIO_COMMANDDONE_ERROR_FLAGS | (1 << DRVSDIO_RAWINTSTATUS_HLE))) == 0) && ((resp & DRVSDIO_CARD_STATUS_ERASESEQ_FLAGS_MASK) == 0)) {
            DPRINTF1("\nCMD38\n");
            // Prepare and Load command
            tmp = ((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (DRVSDIO_CMD38 << DRVSDIO_CMD_CMD_INDEX));
            resp = drvSdioLoadAndWait_ShortResponse(tmp, 0, TRUE, card, &iflags, DRV_NOTIMEOUT, TRUE);
        }
    }
    *errorflags = iflags;
    return resp;
}

/// Allows to work out the total size of a scattered transfer
///
/// @param[in] transactionList Transaction list
/// @param[in] count Length of the transaction list
/// @return Size of the transfer
static u32 drvSdioGetScatteredTransactionSize(tyDrvSdio_Transaction *transactionList, u32 count)
{
    u32 i, size = 0;
    for (i = 0; i < count ; i ++)
        size += transactionList[i].size;
    return size;
}

DRV_RETURN_TYPE DrvSdioWriteDataBlockList(u32 sdblocknumber, tyDrvSdio_Transaction *transactionList, u32 count, u32 cardslot, u32 *errorflags)
{
    u32 flags = 0, descriptors_base_address = 0;
    u32 __attribute__((unused)) resp;  // avoid 'dead assignment' error from scan build
    resp = 0;

    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    // Select a card for writing
    resp = drvSdioCardSelect(drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, DRV_NOTIMEOUT);
    // Simple error flags check
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        // Set bus width
        resp = drvSdioSelectBusWidth(drvsdioCards[cardslot].busWidth, drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, DRV_NOTIMEOUT);
        // Simple error flags check
        if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
            // Set block length
            resp = drvSdioSetBlockLength(drvsdioCards[cardslot].blockSize, drvSdioGetScatteredTransactionSize(transactionList,count), cardslot, &flags, DRV_NOTIMEOUT);
            // Simple error flags check
            if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
                // Fifo needs to be reset before writing
                drvSdioResetFIFO();
                // Get Descriptors base address
                descriptors_base_address = GET_REG_WORD_VAL(SDIOH1_DBADDR_S1);
                // Create the necessary number of descriptors (depending on data size)
                resp = drvSdioBuildChainedDescriptor(descriptors_base_address, transactionList, count, drvsdioCards[cardslot].blockSize);
                // Support for SD cards
                if (drvsdioCards[cardslot].sdhc == 0) {
                    sdblocknumber *= drvsdioCards[cardslot].blockSize;
                }
                // Work out the number of blocks
                resp = drvSdioGetScatteredTransactionSize(transactionList,count) / drvsdioCards[cardslot].blockSize;
                if (resp == 0x01)
                    // If descriptor is only one then perform Single block write
                    resp = drvSdioStartSingleBlockWrite(sdblocknumber, cardslot,&flags);
                else {
                    // If descriptors are more than one then perform Multiple block write
                    resp = drvSdioStartMultipleBlockWrite(sdblocknumber, resp, cardslot, &flags);
                }
            }
        }
        // Deselect the card
        resp = drvSdioCardDeSelect(cardslot, DRV_NOTIMEOUT, TRUE);
    }
    *errorflags = flags;
    if (DRVSDIO_VERIFY_WR_BLOCK_RESULT(flags) != TRUE)
        resp = DRVSDIO_ERROR;
    else
        resp  = DRVSDIO_SUCCESS;
    DPRINTF1("\nDrvSdioWriteDataBlockList Block 0x%lx BlockSize 0x%x Data Size 0x%lx ErrorFlags 0x%lx Res 0x%lx descriptors_base_address 0x%lx\n", sdblocknumber, drvsdioCards[cardslot].blockSize, drvSdioGetScatteredTransactionSize(transactionList,count), flags, resp, descriptors_base_address);

    return resp;
}

DRV_RETURN_TYPE DrvSdioReadDataBlockList(u32 sdblocknumber, tyDrvSdio_Transaction *transactionList, u32 count, u32 cardslot, u32 *errorflags)
{
    u32 flags = 0, descriptors_base_address = 0;
    u32 __attribute__((unused)) resp = 0; // avoid 'unused variable' error from scan build

    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    // Select a card for reading
    resp = drvSdioCardSelect(drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, DRV_NOTIMEOUT);
    // Simple error flags check
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        // Set bus width
        resp = drvSdioSelectBusWidth(drvsdioCards[cardslot].busWidth, drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, DRV_NOTIMEOUT);
        // Simple error flags check
        if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
            // Set block length
            resp = drvSdioSetBlockLength(drvsdioCards[cardslot].blockSize, drvSdioGetScatteredTransactionSize(transactionList,count), cardslot, &flags, DRV_NOTIMEOUT);
            // Simple error flags check
            if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
                // Fifo needs to be reset before reading
                drvSdioResetFIFO();
                // Get Descriptors base address
                descriptors_base_address = GET_REG_WORD_VAL(SDIOH1_DBADDR_S1);
                // Create the necessary number of descriptors (depending on data size)
                resp = drvSdioBuildChainedDescriptor(descriptors_base_address, transactionList, count, drvsdioCards[cardslot].blockSize);
                // Support for SD cards
                if (drvsdioCards[cardslot].sdhc == 0)
                    sdblocknumber *= drvsdioCards[cardslot].blockSize;
                // Work out the number of blocks
                resp = drvSdioGetScatteredTransactionSize(transactionList,count) / drvsdioCards[cardslot].blockSize;
                if (resp == 0x01){
                    // If descriptor is only one then perform Single block read
                    resp = drvSdioStartSingleBlockRead(sdblocknumber, cardslot,&flags);
                }
                else{
                    // If descriptors are more than one then perform Multiple block read
                    resp = drvSdioStartMultipleBlockRead(sdblocknumber, resp, cardslot, &flags);
                }
            }
        }
        // Deselect the card
        resp = drvSdioCardDeSelect(cardslot, DRV_NOTIMEOUT, TRUE);
    }
    *errorflags = flags;
    if (DRVSDIO_VERIFY_RD_BLOCK_RESULT(resp) != TRUE)
        resp = DRVSDIO_ERROR;
    else
        resp = DRVSDIO_SUCCESS;

    DPRINTF1("\nDrvSdioReadDataBlockList List Length %ld Block 0x%lx Block Size 0x%x DataSize 0x%lx ErrorFlags 0x%lx Resp 0x%lx descriptors_base_address 0x%lx\n", count, sdblocknumber, drvsdioCards[cardslot].blockSize, drvSdioGetScatteredTransactionSize(transactionList,count), flags, resp, descriptors_base_address);
    return resp;
}

DRV_RETURN_TYPE DrvSdioWriteDataBlock(u32 sdblocknumber, u32 sourceaddress, u32 datasize, u32 cardslot, u32 *errorflags)
{
    tyDrvSdio_Transaction transactionList[1];
    transactionList[0].size = datasize;
    transactionList[0].buffer = sourceaddress;
    return DrvSdioWriteDataBlockList(sdblocknumber, &transactionList[0], 1, cardslot, errorflags);
}

DRV_RETURN_TYPE DrvSdioReadDataBlock(u32 destaddress, u32 sdblocknumber, u32 datasize, u32 cardslot, u32 *errorflags)
{
    tyDrvSdio_Transaction transactionList[1];
    transactionList[0].size = datasize;
    transactionList[0].buffer = destaddress;
    return DrvSdioReadDataBlockList(sdblocknumber, &transactionList[0], 1, cardslot, errorflags);
}

DRV_RETURN_TYPE DrvSdioEraseDataBlock(u32 firstblock, u32 lastblock, u32 cardslot, u32 *errorflags)
{
    u32 flags = 0;
    u32 __attribute__((unused)) resp = 0; // avoid 'unused variable' error from scan build
    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    // Select a card
    resp = drvSdioCardSelect(drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, DRV_NOTIMEOUT);
    // Check for errors
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        // Perform Block erase
        resp = drvSdioEraseBlocks(firstblock, lastblock, cardslot, &flags);
        if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
            flags |= drvSdioCardDeSelect(cardslot, DRV_NOTIMEOUT, TRUE);
        }
    }
    *errorflags = flags;
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) != 0) {
        return DRVSDIO_ERROR;
    }
    DPRINTF1("\nDrvSdioEraseDataBlock Start Block 0x%lx Last Block 0x%lx CardSlot %ld Error Flags 0x%lx Resp 0x%lx\n", firstblock, lastblock, cardslot, flags, resp);
    return DRVSDIO_SUCCESS;
}

/// Program FIFO
///
/// @param[in] dma_Msize Burst size of multiple transaction
/// @param[in] rx_wmark FIFO threshold watermark level when receiving data to card
/// @param[in] tx_wmark FIFO threshold watermark level when transmitting data to card
/// @return none
static void drvSdioProgramFIFO(u32 dma_Msize, u32 rx_wmark, u32 tx_wmark)
{
   u32 write_val;

   write_val = (((dma_Msize & DRVSDIO_FIFOTH_DMA_MULTIPLETRAN_MASK) << DRVSDIO_FIFOTH_DMA_MULTIPLETRAN) |
                ((rx_wmark & DRVSDIO_FIFOTH_RX_WATERMARK_MASK) << DRVSDIO_FIFOTH_RX_WATERMARK) |
                ((tx_wmark << DRVSDIO_FIFOTH_TX_WATERMARK) & DRVSDIO_FIFOTH_TX_WATERMARK_MASK));
   SET_REG_WORD(SDIOH1_FIFOTH_S1, write_val);
   DPRINTF1("\ndrvSdioProgramFIFO DMA Size 0x%lx RXWM 0x%lx TXWM 0x%lx FIFOTH 0x%lx\n", dma_Msize, rx_wmark, tx_wmark, write_val);
}

DRV_RETURN_TYPE DrvSdioGetCardBlockNumber(u32 cardslot, u64 *cardblocknum)
{
    u64 capacity = 0;
    u32 res = 0;

    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    if (drvsdioCards[cardslot].relativeCardAddress) {
        // Get the capacity from the CSD card register
        res = DrvSdioGetCardCapacity(cardslot, &capacity);
        capacity = capacity >> 9;
    }
    else {
        DPRINTF1("\nNo relative card address assigned. Card is probably not initialized\n");
        res = DRVSDIO_ERROR;
    }
    DPRINTF1("\nDrvSdioGetCardBlockNumber CardSlot %ld Res %llu\n", cardslot, capacity);
    *cardblocknum = capacity;
    return res;
}

DRV_RETURN_TYPE DrvSdioGetCardCapacity(u32 cardslot, u64 *cardcapacity)
{
    u64 cSize, csdType;
    u8 *pointer = NULL;

    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    if (drvsdioCards[cardslot].relativeCardAddress) {
        // Get the capacity from the CSD card register
        pointer = (u8 *) drvsdioCards[cardslot].csd;
        csdType = (u32)DRVSDIO_CSD_GET_CSD_STRUCTURE(pointer) + 1;
        if (csdType == 2)
            cSize = (DRVSDIO_CSD_GET_C_SIZE(pointer) + 1) * 512 * 1024;
        else {
            DPRINTF1("\nSC card type not supported \n");
            return DRVSDIO_CARD_TYPE_NOT_SUPPORTED;
        }
    }
    else {
        DPRINTF1("\nNo relative card address assigned. Card is probably not initialized\n");
        return DRVSDIO_ERROR;
    }
    DPRINTF1("\nDrvSdioGetCardCapacity Cardslot %ld Capacity %llu\n", cardslot, cSize);
    *cardcapacity = cSize;
    return DRVSDIO_SUCCESS;
}

DRV_RETURN_TYPE DrvSdioGetCardTransferSpeed(u32 cardslot, u32 *cardtransferspeed)
{
    u32 s = 0, e = 0, m = 0;
    u8 *pointer = NULL;

    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    if (drvsdioCards[cardslot].relativeCardAddress) {
        // Get the value of the CSD card register
        pointer = (u8 *) drvsdioCards[cardslot].csd;
        // Parse the Card transfer speed
        s = DRVSDIO_CSD_GET_TRAN_SPEED(pointer);
        e = s & 0x7;
        m = s >> 3;
        switch (e) {
            case 0: s = 10000; break;
            case 1: s = 100000; break;
            case 2: s = 1000000; break;
            case 3: s = 10000000; break;
            default: s = 0; break;
        }
        switch (m) {
            case 1: s *= 11; break;
            case 2: s *= 12; break;
            case 3: s *= 13; break;
            case 4: s *= 15; break;
            case 5: s *= 20; break;
            case 6: s *= 25; break;
            case 7: s *= 30; break;
            case 8: s *= 35; break;
            case 9: s *= 40; break;
            case 10: s *= 45; break;
            case 11: s *= 50; break;
            case 12: s *= 55; break;
            case 13: s *= 60; break;
            case 14: s *= 70; break;
            case 15: s *= 80; break;
            default: s *= 0; break;
        }
    }
    else {
        DPRINTF1("\nNo relative card address assigned. Card is probably not initialized\n");
        return DRVSDIO_NOT_INITIALIZED;
    }
    s = s / 1000;
    DPRINTF1("\nDrvSdioGetCardTransferSpeed CardSlot %ld Res %ld Khz \n", cardslot, s);

    *cardtransferspeed = s;
    return DRVSDIO_SUCCESS;
}

DRV_RETURN_TYPE DrvSdioGetCardAccessTime(u32 cardslot, u32 *cardaccesstime)
{
    u32 ac = 0, e = 0, m = 0;
    u8 *pointer = NULL;

    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    if (drvsdioCards[cardslot].relativeCardAddress){
        // Get the value of the CSD card register
        pointer = (u8 *) drvsdioCards[cardslot].csd;
        // Parse the Card access time
        ac = DRVSDIO_CSD_GET_TAAC(pointer);
        e = ac & 0x7;
        m = ac >> 3;
        switch (e) {
            case 0: ac = 1; break;
            case 1: ac = 10; break;
            case 2: ac = 100; break;
            case 3: ac = 1000; break;
            case 4: ac = 10000; break;
            case 5: ac = 100000; break;
            case 6: ac = 1000000; break;
            case 7: ac = 10000000; break;
            default: ac = 0; break;
        }
        switch (m) {
            case 1: ac *= 10; break;
            case 2: ac *= 12; break;
            case 3: ac *= 13; break;
            case 4: ac *= 15; break;
            case 5: ac *= 20; break;
            case 6: ac *= 25; break;
            case 7: ac *= 30; break;
            case 8: ac *= 35; break;
            case 9: ac *= 40; break;
            case 10: ac *= 45; break;
            case 11: ac *= 50; break;
            case 12: ac *= 55; break;
            case 13: ac *= 60; break;
            case 14: ac *= 70; break;
            case 15: ac *= 80; break;
            default: ac *= 0; break;
        }
        ac = ac/10;
    }
    else {
        DPRINTF1("\nNo relative card address assigned. Card is probably not initialized\n");
        return DRVSDIO_NOT_INITIALIZED;
    }
    DPRINTF1("\nDrvSdioGetCardAccessTime CardSlot %ld Res %ld \n", cardslot, ac);
    *cardaccesstime = ac;
    return DRVSDIO_SUCCESS;
}

DRV_RETURN_TYPE DrvSdioGetCardMaxAccessTime(u32 cardslot, u32 transfer_speed, u32 *cardmaxaccesstime)
{
    u64 ac = 0;
    u32 n = 0, res = 0, maxacctime = 0;
    u8 *pointer = NULL;
    u32 ret = 0;

    assert(cardslot < DRVSDIO_MAXDRIVERCARDS);
    if (drvsdioCards[cardslot].relativeCardAddress){
        // Get the value of the CSD card register
        pointer = (u8 *) drvsdioCards[cardslot].csd;
        // Parse the Card access time
        ret = DrvSdioGetCardAccessTime(cardslot, &maxacctime);
        if (ret == DRVSDIO_SUCCESS)
        {
            ac = maxacctime;
            n = DRVSDIO_CSD_GET_NSAC(pointer);
            ac = (ac * transfer_speed) / 8000000000ULL;
            res = n + (u32) ac;
        }
    }
    else {
        DPRINTF1("\nNo relative card address assigned. Card is probably not initialized\n");
        ret = DRVSDIO_NOT_INITIALIZED;
    }
    DPRINTF1("\nDrvSdioGetCardAccessTime Cardslot %ld Transfer Speed %ld Res %ld \n", cardslot, transfer_speed, res);
    *cardmaxaccesstime = res;
    return ret;
}

// ISR in charge of handling SD interrupts and Internal DMA
static void drvSdioIrq_Handler(u32 source)
{
    UNUSED(source);
    u32 tmp = GET_REG_WORD_VAL(SDIOH1_RINTSTS_S1);
    DrvSdio_RInterrupts |= tmp;
    SET_REG_WORD(SDIOH1_RINTSTS_S1, tmp);
    GET_REG_WORD_VAL(SDIOH1_RINTSTS_S1); // make sure that the clearing of the interrupt has completed before we clear the ICB level-sensitive interrupt bit
    // Clear the interrupt
    DrvIcbIrqClear(IRQ_SDIO);
}

DRV_RETURN_TYPE DrvSdioSetupIrq(u32 interruptPriority)
{
    /* Install interrupt handler */
    DrvIcbSetIrqHandler(IRQ_SDIO, drvSdioIrq_Handler);
    DrvIcbConfigureIrq( IRQ_SDIO, interruptPriority, POS_LEVEL_INT);
    DrvIcbEnableIrq(IRQ_SDIO);
    return DRVSDIO_SUCCESS;
}

void DrvSdioReset(u32 hardware)
{
    if (hardware) {
        // Set back the Control register
        SET_REG_WORD(SDIOH1_CTRL_S1, (1 << DRVSDIO_CTRL_FIFO_RESET) | (1 << DRVSDIO_CTRL_DMA_RESET) | (1 << DRVSDIO_CTRL_CONTROLLER_RESET));
        // Wait for reset to be performed
        while(GET_REG_WORD_VAL(SDIOH1_CTRL_S1) & ((1 << DRVSDIO_CTRL_FIFO_RESET) | (1 << DRVSDIO_CTRL_DMA_RESET) | (1 << DRVSDIO_CTRL_CONTROLLER_RESET)));
        // Delay just in case
        drvSdiodelay(200000);
    }
    // Reset all control structures
    memset(&drvsdioCards, 0x00, sizeof(drvsdioCards));
    memset(&tyDrvSdio_Driver, 0x00, sizeof(tyDrvSdio_Driver));
    DrvSdio_RInterrupts = 0;
    DPRINTF1("\nDrvSdioReset\n");
}

void DrvSdioDeinitialise(void)
{
    u32 tmp = 0;

    /* Disable the sdio clock */
    tmp = GET_REG_WORD_VAL(SDIOH1_CLKENA_S1);
    tmp &=  (!DRVSDIO_MAXNUMCARDS_MASK);
    SET_REG_WORD(SDIOH1_CLKENA_S1, tmp);
    drvSdioLoadCommand((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_UPDATE_CLK_REG), FALSE, 0, DRVSDIO_TIMEOUT_VALUE, TRUE);

    /* Disable power to cards */
    tmp = GET_REG_WORD_VAL(SDIOH1_PWREN_S1);
    tmp &=  (!DRVSDIO_MAXNUMCARDS_MASK);
    SET_REG_WORD(SDIOH1_PWREN_S1, tmp);

    DrvSdioReset(TRUE);
}

DRV_RETURN_TYPE DrvSdioInitialise(u32 cclk_in, u32 descriptors_base_address, DrvSdioOpts *opts)
{
    u32 tmp = 0;

    // Initialize sd card configuration struct
    for (int slot = 0; slot < DRVSDIO_MAXDRIVERCARDS; ++slot)
    {
        // Reset the internal control struct first
        memset(&drvsdioCards[slot], 0x00, sizeof(tyDrvSdioCardConfig));

        if(opts == NULL) {
            drvsdioCards[slot].card_opts.max_speed_mode = SDIO_HIGH_SPEED;
            // Set a voltage switching callback for every card slot
            drvsdioCards[slot].card_opts.callback   = NULL;
        }
        // Check if the passed speed mode is supported
        else if(opts[slot].max_speed_mode <= SDIO_HIGHEST_SPEEDMODE_SUPPORTED) {
            drvsdioCards[slot].card_opts.max_speed_mode = opts[slot].max_speed_mode;
            // Set a voltage switching callback for every card slot
            drvsdioCards[slot].card_opts.callback   = opts[slot].callback;
        }
        else {
            drvsdioCards[slot].card_opts.max_speed_mode = SDIO_HIGHEST_SPEEDMODE_SUPPORTED;
            // Set a voltage switching callback for every card slot
            drvsdioCards[slot].card_opts.callback   = opts[slot].callback;
        }
    }

    if ((cclk_in == 0) ||
            (descriptors_base_address == 0))
    {
        DPRINTF1("\nDrvSdioInitialise FAIL. \nBad clock frequency or descriptors address\n");
        return DRVSDIO_ERROR;
    }
    // Make sure descriptors base address is not passed bypassed
    if (descriptors_base_address & DRV_DDRMASK)
        descriptors_base_address &= ~DRV_DDRBYPASSMASK;
    else
        descriptors_base_address &= ~DRV_CMXBYPASSMASK;
    tyDrvSdio_Driver.cclk_in_khz = cclk_in;
    DPRINTF1("Configuring cclk_in %ld\n", cclk_in);
    /* Enable power to cards, PWREN reg  - enable power to all cards */
    SET_REG_WORD(SDIOH1_PWREN_S1, DRVSDIO_MAXNUMCARDS_MASK);
    /* Access raw interrupt register to clear any pending interrupt */
    SET_REG_WORD(SDIOH1_RINTSTS_S1, DRVSDIO_RAWINTSTATUS_ALL);
    /* Set global interrupt enable bit 0x00000010 */
    SET_REG_WORD(SDIOH1_CTRL_S1, (1 << DRVSDIO_CTRL_INT_ENABLE));
    // Work out the value for enumeration process
    tmp = DRVSDIO_GET_DIV_VALUE(cclk_in, DRVSDIO_STARTFREQ);
    DPRINTF1("Div:%ld\n",tmp);
    /* Reduce operating frequency to near 400Khz for Div 0, the only one being used  */
    SET_REG_WORD(SDIOH1_CLKDIV_S1, (tmp << DRVSDIO_CLKSOURCE_DIV0));
    /* Clock source dedicated to each card. 00 Div 0, 01 Div 1, 10 Div 2 and 11 Div 3. MMC-Ver3.3-only controller, only Div 0 is enabled.
    Select Div 0 for all cards */
    SET_REG_WORD(SDIOH1_CLKSRC_S1, 0x00000000);
    /* Enable clock for all 16 cards and Non-low-power-mode */
    SET_REG_WORD(SDIOH1_CLKENA_S1, DRVSDIO_CLKENA_ENABLE_ALL);
    /* For SDIOH1_CLKDIV_S1, SDIOH1_CLKDIV_S1 and SDIOH1_CLKENA_S1 to be loaded bits CMD_START_CMD and CMD_UPDATE_CLK_REG must
    be set in CMD register 0x80200000*/
    drvSdioLoadCommand((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_UPDATE_CLK_REG), FALSE, 0, DRV_NOTIMEOUT, TRUE);
    /* Program Data timeout and response timeout Using default values 0xFFFFFF, 0x40 . 0xFFFFFF40*/
    SET_REG_WORD(SDIOH1_TMOUT_S1, (DRVSDIO_TMOUT_DEFAULT_DATATIMEOUT << DRVSDIO_TMOUT_DATATIMEOUT) | (DRVSDIO_TMOUT_DEFAULT_RESPONSETIMEOUT << DRVSDIO_TMOUT_RESPONSETIMEOUT));
    // By default 4 bit bus for all cards
    SET_REG_WORD(SDIOH1_CTYPE_S1, DRVSDIO_CTYPE_SD0_SD15_4BITS);
    /* Program FIFO threshold to default values and DMA_SIZE burst size for multiple transactions to 1 (The same as DW-DMA controller multiple-transaction-size) */
    drvSdioProgramFIFO(DRVSDIO_DMASIZE, DRVSDIO_RX_WMARK, DRVSDIO_TX_WMARK);
    /* Set CTRL_INT_ENABLE and CTRL_USE_IDMAC in CTRL, 0x02000010 */
    SET_REG_WORD(SDIOH1_CTRL_S1, (1 << DRVSDIO_CTRL_USE_IDMAC) | (1 << DRVSDIO_CTRL_INT_ENABLE));
    /* Program IDMAC */
    /* Set PBL with same value as FIFO DMA Transaction Size DRVSDIO_DMASIZE */
    tmp = ((1 << DRVSDIO_BMOD_FB) | (1 << DRVSDIO_BMOD_DE) | (DRVSDIO_DMASKIP << DRVSDIO_BMOD_DSL) | (DRVSDIO_DMASIZE << DRVSDIO_BMOD_PBL));
    /* 0 * HWORD/WORD/DWORD to be skipped between 2 unchained descriptors == dmac_skip_length, IDMAC enable and Fixed bursts */
    SET_REG_WORD(SDIOH1_BMOD_S1, tmp);
    /* Enable all interrupts except Receive/Transmit FIFO data request, 0x0000ffcf */
    tmp = (DRVSDIO_INTMASK_EVENTALL & ~((1 << DRVSDIO_INTMASK_RXDR) | (1 << DRVSDIO_INTMASK_TXDR)));
    SET_REG_WORD(SDIOH1_INTMASK_S1, tmp);
    /* Program descriptor base address aligned to 32 - bit */
    SET_REG_WORD(SDIOH1_DBADDR_S1, descriptors_base_address & 0xFFFFFFFC);
    /* Disable all interrupts for the Internal DMAC */
    SET_REG_WORD(SDIOH1_IDINTEN_S1, 0x00);
    /* Enable Busy Interrupt !!! */
    /* Enable Card Read Threshold and set its value */
    SET_REG_WORD(SDIOH1_CARDTHRCTL_S1,((1 << DRVSDIO_CARDTHRCTL_BSYCLRINTEN) | (1 << DRVSDIO_CARDTHRCTL_CARDRDTHREN) | (DRVSDIO_CARDTHRCTL_CARDRDTHRESHOLD_MASK << DRVSDIO_CARDTHRCTL_CARDRDTHRESHOLD)));
    /* Clock Shift */
    tmp = GET_REG_WORD_VAL(SDIOH1_UHS_REG_EXT_S1);
    tmp |= ((DRVSDIO_DEFAULT_CLK_DRV_PHASE_CTRL << DRVSDIO_UHS_REG_EXT_CLK_DRV_PHASE_CTRL) | (DRVSDIO_DEFAULT_CLK_SMPL_PHASE_CTRL << DRVSDIO_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL));
    SET_REG_WORD(SDIOH1_UHS_REG_EXT_S1, tmp);
    /* DELAY TO BE REPLACED BY A PROPER ROUTINE */
    drvSdiodelay(100*cclk_in);

    /* Reset all cards */
    SET_REG_WORD(SDIOH1_RST_S1, DRVSDIO_RST_N_RESETCARDALL);
    /* DELAY TO BE REPLACED BY A PROPER ROUTINE */
    drvSdiodelay(40*cclk_in);
    /* Activate all the cards   */
    SET_REG_WORD(SDIOH1_RST_S1, DRVSDIO_RST_N_ACTIVECARDALL);
    tyDrvSdio_Driver.status |= (1 << DRVSDIO_INTERNALSD_STATUS_FREQ_SWITCHED);
    DPRINTF1("\nDrvSdioInitialise Done\n");

    return DRVSDIO_SUCCESS;
}

DRV_RETURN_TYPE DrvSdioInit(u32 cclk_in, u32 descriptors_base_address)
{
    u32 tmp = 0;

    if ((tyDrvSdio_Driver.status & (1 << DRVSDIO_INTERNALSD_STATUS_FREQ_SWITCHED)) == 0) {
        if ((cclk_in == 0) ||
                (descriptors_base_address == 0))
        {
            DPRINTF1("\nDrvSdioInit FAIL. \nBad clock frequency or descriptors address %ld\n", cclk_in);
            return DRVSDIO_ERROR;
        }
        // Make sure descriptors base address is not passed bypassed
        if (descriptors_base_address & DRV_DDRMASK)
            descriptors_base_address &= ~DRV_DDRBYPASSMASK;
        else
            descriptors_base_address &= ~DRV_CMXBYPASSMASK;
        tyDrvSdio_Driver.cclk_in_khz = cclk_in;
        DPRINTF1("Configuring cclk_in %ld\n", cclk_in);
        /* Enable power to cards, PWREN reg  - enable power to all cards */
        SET_REG_WORD(SDIOH1_PWREN_S1, DRVSDIO_MAXNUMCARDS_MASK);
        /* Access raw interrupt register to clear any pending interrupt */
        SET_REG_WORD(SDIOH1_RINTSTS_S1, DRVSDIO_RAWINTSTATUS_ALL);
        /* Set global interrupt enable bit 0x00000010 */
        SET_REG_WORD(SDIOH1_CTRL_S1, (1 << DRVSDIO_CTRL_INT_ENABLE));
        // Work out the value for enumeration process
        tmp = DRVSDIO_GET_DIV_VALUE(cclk_in, DRVSDIO_STARTFREQ);
        DPRINTF1("Div:%ld\n",tmp);
        /* Reduce operating frequency to near 400Khz for Div 0, the only one being used  */
        SET_REG_WORD(SDIOH1_CLKDIV_S1, (tmp << DRVSDIO_CLKSOURCE_DIV0));
        /* Clock source dedicated to each card. 00 Div 0, 01 Div 1, 10 Div 2 and 11 Div 3. MMC-Ver3.3-only controller, only Div 0 is enabled.
        Select Div 0 for all cards */
        SET_REG_WORD(SDIOH1_CLKSRC_S1, 0x00000000);
        /* Enable clock for all 16 cards and Non-low-power-mode */
        SET_REG_WORD(SDIOH1_CLKENA_S1, DRVSDIO_CLKENA_ENABLE_ALL);
        /* For SDIOH1_CLKDIV_S1, SDIOH1_CLKDIV_S1 and SDIOH1_CLKENA_S1 to be loaded bits CMD_START_CMD and CMD_UPDATE_CLK_REG must
        be set in CMD register 0x80200000*/
        drvSdioLoadCommand((1 << DRVSDIO_CMD_START_CMD) | (1 << DRVSDIO_CMD_UPDATE_CLK_REG), FALSE, 0, DRV_NOTIMEOUT, TRUE);
        /* Program Data timeout and response timeout Using default values 0xFFFFFF, 0x40 . 0xFFFFFF40*/
        SET_REG_WORD(SDIOH1_TMOUT_S1, (DRVSDIO_TMOUT_DEFAULT_DATATIMEOUT << DRVSDIO_TMOUT_DATATIMEOUT) | (DRVSDIO_TMOUT_DEFAULT_RESPONSETIMEOUT << DRVSDIO_TMOUT_RESPONSETIMEOUT));
        // By default 4 bit bus for all cards
        SET_REG_WORD(SDIOH1_CTYPE_S1, DRVSDIO_CTYPE_SD0_SD15_4BITS);
        /* Program FIFO threshold to default values and DMA_SIZE burst size for multiple transactions to 1 (The same as DW-DMA controller multiple-transaction-size) */
        drvSdioProgramFIFO(DRVSDIO_DMASIZE, DRVSDIO_RX_WMARK, DRVSDIO_TX_WMARK);
        /* Set CTRL_INT_ENABLE and CTRL_USE_IDMAC in CTRL, 0x02000010 */
        SET_REG_WORD(SDIOH1_CTRL_S1, (1 << DRVSDIO_CTRL_USE_IDMAC) | (1 << DRVSDIO_CTRL_INT_ENABLE));
        /* Program IDMAC */
        /* Set PBL with same value as FIFO DMA Transaction Size DRVSDIO_DMASIZE */
        tmp = ((1 << DRVSDIO_BMOD_FB) | (1 << DRVSDIO_BMOD_DE) | (DRVSDIO_DMASKIP << DRVSDIO_BMOD_DSL) | (DRVSDIO_DMASIZE << DRVSDIO_BMOD_PBL));
        /* 0 * HWORD/WORD/DWORD to be skipped between 2 unchained descriptors == dmac_skip_length, IDMAC enable and Fixed bursts */
        SET_REG_WORD(SDIOH1_BMOD_S1, tmp);
        /* Enable all interrupts except Receive/Transmit FIFO data request, 0x0000ffcf */
        tmp = (DRVSDIO_INTMASK_EVENTALL & ~((1 << DRVSDIO_INTMASK_RXDR) | (1 << DRVSDIO_INTMASK_TXDR)));
        SET_REG_WORD(SDIOH1_INTMASK_S1, tmp);
        /* Program descriptor base address aligned to 32 - bit */
        SET_REG_WORD(SDIOH1_DBADDR_S1, descriptors_base_address & 0xFFFFFFFC);
        /* Disable all interrupts for the Internal DMAC */
        SET_REG_WORD(SDIOH1_IDINTEN_S1, 0x00);
        /* Enable Busy Interrupt !!! */
        /* Enable Card Read Threshold and set its value */
        SET_REG_WORD(SDIOH1_CARDTHRCTL_S1,((1 << DRVSDIO_CARDTHRCTL_BSYCLRINTEN) | (1 << DRVSDIO_CARDTHRCTL_CARDRDTHREN) | (DRVSDIO_CARDTHRCTL_CARDRDTHRESHOLD_MASK << DRVSDIO_CARDTHRCTL_CARDRDTHRESHOLD)));
        /* Clock Shift */
        tmp = GET_REG_WORD_VAL(SDIOH1_UHS_REG_EXT_S1);
        tmp |= ((DRVSDIO_DEFAULT_CLK_DRV_PHASE_CTRL << DRVSDIO_UHS_REG_EXT_CLK_DRV_PHASE_CTRL) | (DRVSDIO_DEFAULT_CLK_SMPL_PHASE_CTRL << DRVSDIO_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL));
        SET_REG_WORD(SDIOH1_UHS_REG_EXT_S1, tmp);
        /* DELAY TO BE REPLACED BY A PROPER ROUTINE */
        drvSdiodelay(100*cclk_in);
        /* Reset all cards */
        SET_REG_WORD(SDIOH1_RST_S1, DRVSDIO_RST_N_RESETCARDALL);
        /* DELAY TO BE REPLACED BY A PROPER ROUTINE */
        drvSdiodelay(40*cclk_in);
        /* Activate all the cards */
        SET_REG_WORD(SDIOH1_RST_S1, DRVSDIO_RST_N_ACTIVECARDALL);
        tyDrvSdio_Driver.status |= (1 << DRVSDIO_INTERNALSD_STATUS_FREQ_SWITCHED);
        DPRINTF1("\nDrvSdioInit Done\n");
    }
    else {
        DPRINTF1("\nDrvSdioInit FAIL\n");
        return DRVSDIO_ERROR;
    }
    return DRVSDIO_SUCCESS;
}

/// Switch to High-Speed mode
///
/// First check if the card is spec version compatible.
/// After that check if the card supports switch to High-Speed function and if available
/// perform the switch.
///
/// @param[in] cardslot This is the card number
/// @param[in] errorflags The value of the RINTSTS (Raw Interrupt Status Register)
/// @return none
static void drvSdioSwitchToHighSpeedMode(u32 cardslot, tyDrvSdioSpeedMode mode, u32 *errorflags)
{
    u32 command = 0, arg = 0, flags = 0;
    u32 __attribute__((unused)) resp; // avoid 'unused variable' error from scan build
    resp = 0;
    resp = drvSdioCardSelect(drvsdioCards[cardslot].relativeCardAddress, cardslot, &flags, DRV_NOTIMEOUT);

    // Switches function mode
    //  Mode 1 Operation - Set Function
    DPRINTF1("\nCMD6\n");
    // Prepare CMD6
    command = ((1 << DRVSDIO_CMD_START_CMD) | (DRVSDIO_CMD6 << DRVSDIO_CMD_CMD_INDEX) | (1 << DRVSDIO_CMD_RESP_EXPECT) | (1 << DRVSDIO_CMD_CHECK_RESP_CRC) | (1 << DRVSDIO_CMD_WAIT_PREVDATA_CMPL));
    // Set args for CMD6
    // Set bit 31 to 1 to perform Set function; set 0 to the reserved bits; set F to the not changed groupd; set 1 for bit 1 to enable High-Speed mode
    if (mode >= SDIO_UHS_I)
        arg = 0x80FFFFF0 | (mode - 2); // Access modes  0: SDR12, 1: SDR25, 2: SDR50, 3: SDR104, 4: DDR50
    else
        arg = 0x80FFFFF1; //Access mode1 also means HS
    // Load Command
    resp = drvSdioLoadAndWait_ShortResponse(command, arg, TRUE, cardslot, &flags, DRV_NOTIMEOUT, TRUE);
    // Wait for a while after performing the command
    drvSdiodelay(20 * SYSTEMFREQ_CONFIG);
    if (DRVSDIO_ISTHERE_COMMAND_DONE_ERROR(flags) == 0) {
        if (mode >= SDIO_UHS_I) {
            if (mode == SDIO_DDR50) { // DDR50 not yet supported
                // Set also the DDR mode in the controller
                SET_REG_BITS_MASK(SDIOH1_UHS_S1, 1 << (cardslot + DRVSDIO_UHS_REG_DDR_REG));
                DPRINTF1("Switched to DDR50\n");
            }
        }
        else {
            drvsdioCards[cardslot].card_opts.max_speed_mode = SDIO_HIGH_SPEED;
        }
    }
    else {
        DPRINTF1("\nCMD6 Fail to switch to High-Speed mode\n");
    }
    DPRINTF1("\nCMD6 2 flags 0x%lx   resp 0x%lx\n", flags, resp);
    // Deselect the card (return to previous state)
    resp = drvSdioCardDeSelect(cardslot, DRV_NOTIMEOUT, TRUE);
    *errorflags = flags;
}

///@}
