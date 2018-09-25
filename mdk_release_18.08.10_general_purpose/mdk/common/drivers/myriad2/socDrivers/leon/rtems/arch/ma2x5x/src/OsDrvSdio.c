///
/// @file OsDrvSdio.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvSdio
/// @{
/// @brief     RTEMS Driver to control Clocks, Power and Reset
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <rtems/blkdev.h>
#include <rtems.h>
#include <bsp.h>
#include <stdio.h>
#include <DrvSdio.h>
#include <DrvSdioDefines.h>
#include <registersMyriad.h>
#include <OsDrvCpr.h>
#include <DrvTimer.h>
#include "swcLeonUtils.h"
#include "OsDrvSdio.h"
#include "OsCommon.h"
#include <rtems/bdbuf.h>
#include <stdlib.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

//#define OS_DRV_SDIO_L1

#ifdef OS_DRV_SDIO_L1
#define OS_DPRINTF1(...) printf(__VA_ARGS__)
#else
#define OS_DPRINTF1(...)
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// We keep the info passed by the application (during Init time) here
static osDrvSdioEntries_t osDrvSdioInfo;
// Gain access to this so that we can work out the maximum descriptors needed
extern const rtems_bdbuf_config rtems_bdbuf_configuration; 
// Internal struct keeping the ID of the calling task in order to perform proper read/write operations
static struct
{
    rtems_id task_id;
    rtems_id controlMutex;
    u32 sdioClock;
    OsDrvSdio_Descriptors *descriptors;
    tyDrvSdio_Transaction *txList;
    tyDrvSdio_Transaction *rxList;
} osSdioControl;

// 4: Static Local Data 
// ----------------------------------------------------------------------------
static osCommonIrqShareInfo_t osCommonIrqSharedInfo;

static rtems_device_driver osDrvSdioSDInitialize(rtems_device_major_number major, rtems_device_minor_number minor, void *arg);

// Hide from user
static rtems_driver_address_table osDrvSdioDriverTable = {
  initialization_entry: osDrvSdioSDInitialize,
  open_entry:           rtems_blkdev_generic_open,
  close_entry:          rtems_blkdev_generic_close,
  read_entry:           rtems_blkdev_generic_read,
  write_entry:          rtems_blkdev_generic_write,
  control_entry:        rtems_blkdev_generic_ioctl
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
/// Wait for events
/// 
/// @param[in] flag: The flags we are waiting for
/// @return Events captured 
static u32 osDrvWaitforFlags(u32 flag)
{       
    rtems_event_set event_out;
    OsCommonEventReceive(flag, TRUE, TRUE, RTEMS_NO_TIMEOUT, &event_out, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_SDIO);
    return (u32) event_out;
}

/// Trick to reuse bare-metal driver
/// 
/// The bare-metal function has weak attribute
/// @param[in] flag: The flags we are waiting for
u32 DrvSdioWaitForFlags(u32 flag, u32 timeOut)
{
    u32 events = osDrvWaitforFlags(flag);
    UNUSED(timeOut);
    return events;
}

// ISR in charge of handling SD interrupts and Internal DMA 
static void osDrvSdioIrqHandler(u32 source)
{
    UNUSED(source);// hush the compiler warning.

    u32 sdioints;
    sdioints = GET_REG_WORD_VAL(SDIOH1_RINTSTS_S1);
    SET_REG_WORD(SDIOH1_RINTSTS_S1, sdioints);
    OsDrvCommonInterruptSendEvent(osSdioControl.task_id, sdioints, &osCommonIrqSharedInfo.irqShared, OSCOMMON_EVENT_SDIO, FALSE);
    BSP_Clear_interrupt(IRQ_SDIO);
}

/// Rearrange data in memory in order to use the correct cache data
/// 
/// @param[in] transactions:  transaction list
/// @param[in] length: Length of the transaction list
/// @return none
static inline void osDrvSdioFixCacheRead(tyDrvSdio_Transaction *transactions, uint32_t length)
{
    uint32_t i = 0;
    for (i = 0; i < length; i ++) {
        rtems_cache_invalidate_data_range((void *) transactions[i].buffer, transactions[i].size);
    }
}

/// Rearange data in memory in order to use the correct cache data
/// 
/// @param[in] transactions:  transaction list
/// @param[in] length: Length of the transaction list
/// @return none
static inline void osDrvSdioFixCacheWrite(tyDrvSdio_Transaction *transactions, uint32_t length)
{
    uint32_t i = 0;
    for (i = 0; i < length; i ++) {
        rtems_cache_flush_range_l2((void *) transactions[i].buffer, transactions[i].size);
    }
}

/// Rearrange data in memory in order to use the correct cache data
/// 
/// @param[in] buffs:  OS transaction list
/// @param[in] length: Length of the OS transaction list
/// @param[in] transactions: Bare-metal transaction list
/// @return Number of OS transaction processed
static uint32_t osDrvSdioFillTransactions(rtems_blkdev_sg_buffer *buffs, uint32_t length, tyDrvSdio_Transaction *transactions)
{
    uint32_t i = 0, j = 1, totalsize = 0;

    transactions[i].buffer = (uint32_t) buffs[0].buffer;
    transactions[i].size = buffs[0].length;
    totalsize = buffs[0].length;

    while ((j < length) && (buffs[j].block == buffs[0].block + (totalsize / OSDRVSDIO_DEFAULTBLOCKSIZE))) {
        i ++;
        transactions[i].buffer = (uint32_t) buffs[j].buffer;
        transactions[i].size = buffs[j].length;
        totalsize += buffs[j].length;
        j ++;
    }
    return j;
}

/// Performs a reading operation by using the Bare-Metal function
/// 
/// @param[in] r: Pointer to buffer, length and start block that will be read
/// @param[in] dd: Disk device. Needed to obtain minor number
/// @return RTEMS_SUCCESSFUL on success or RTEMS_IO_ERROR on error
static int osDrvSdioReadData(rtems_blkdev_request *r, rtems_disk_device *dd)
{
    int res;
    u32 flags = 0 , processed = 0, length = 0, startBlock = 0;

    rtems_device_minor_number minor = rtems_disk_get_minor_number(dd);
    // Get the current task id so the Irq handler can send requests to the waitforflags in the correct thread
    osSdioControl.task_id = r->io_task;
    do {
        // Save start block
        startBlock = r->bufs[processed].block;
        // Fill transaction list
        length = osDrvSdioFillTransactions(&r->bufs[processed],r->bufnum - processed, osSdioControl.rxList);
        // Update processed
        processed += length;
        // Call bare-metal function
        res = DrvSdioReadDataBlockList(startBlock, osSdioControl.rxList, length, osDrvSdioInfo.slotInfo[minor].slot, &flags);
        // Cache issues
        osDrvSdioFixCacheRead(osSdioControl.rxList, length);
    }while ((processed < r->bufnum) && (res == DRVSDIO_SUCCESS));
    if (res == DRVSDIO_SUCCESS)
        res = RTEMS_SUCCESSFUL;
    else
        res = RTEMS_IO_ERROR;
    return res;
}

static int osDrvSdioWriteData(rtems_blkdev_request *r, rtems_disk_device *dd)
{
    int res;
    u32 flags = 0 , processed = 0, length = 0, startBlock = 0;

    rtems_device_minor_number minor = rtems_disk_get_minor_number(dd);
    // Get the current task id so the Irq handler can send requests to the waitforflags in the correct thread
    osSdioControl.task_id = r->io_task;
    do {
        // Save start block
        startBlock = r->bufs[processed].block;
        // Fill transaction list
        length = osDrvSdioFillTransactions(&r->bufs[processed],r->bufnum - processed, osSdioControl.txList);
        // Update processed
        processed += length;
        // Cache issues
        osDrvSdioFixCacheWrite(osSdioControl.txList, length);
        // Call bare-metal function
        res = DrvSdioWriteDataBlockList(startBlock, osSdioControl.txList, length, osDrvSdioInfo.slotInfo[minor].slot, &flags);
    }while ((processed < r->bufnum) && (res == DRVSDIO_SUCCESS));
    if (res == DRVSDIO_SUCCESS)
        res = RTEMS_SUCCESSFUL;
    else
        res = RTEMS_IO_ERROR;
    return res;
}

/// This function enumerates the card
/// 
/// @param[in] slot: This is the card slot number
/// @return RTEMS_SUCCESSFUL on success or RTEMS_IO_ERROR on error
static int osDrvSdioCardEnumeration(int slot)
{
    int res;
    u32 flags = 0;
    osSdioControl.task_id = rtems_task_self(); 
    // Call the bare-metal Enumerate function
    res = DrvSdioEnumerate(slot, &flags);
    if (res == MYR_DRV_SUCCESS)
       return RTEMS_SUCCESSFUL;
    else
       return RTEMS_IO_ERROR;
}

/// This function calls the Bare-Metal Read or Write functions
///
/// @param[in] dd: Disk device
/// @param[in] req: IOCTL request code
/// @param[in] arg: IOCTL argument
/// @return RTEMS_SUCCESSFUL on success or RTEMS_IO_ERROR on error
static int osDrvSdioIOCtl(rtems_disk_device *dd, uint32_t req, void *arg)
{
    rtems_status_code res = RTEMS_SUCCESSFUL;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    rtems_blkdev_request *reqArg = arg;

    // rtems_blkdev_request may contain thread id
    if (req == RTEMS_BLKIO_REQUEST) {
        switch (reqArg->req)
        {
            // Read from SDcard
            case RTEMS_BLKDEV_REQ_READ:
                sc = rtems_semaphore_obtain(osSdioControl.controlMutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
                if (sc != RTEMS_SUCCESSFUL) {
                    res = RTEMS_IO_ERROR;
                    break;
                }
                res = osDrvSdioReadData(reqArg, dd);
                sc = rtems_semaphore_release(osSdioControl.controlMutex);
                if (sc != RTEMS_SUCCESSFUL) {
                    res = RTEMS_IO_ERROR;
                }
            break;

            // Write to SDcard
            case RTEMS_BLKDEV_REQ_WRITE:
                   sc = rtems_semaphore_obtain(osSdioControl.controlMutex, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
                if (sc != RTEMS_SUCCESSFUL) {
                    res = RTEMS_IO_ERROR;
                    break;
                }
                res = osDrvSdioWriteData(reqArg, dd);
                  sc = rtems_semaphore_release(osSdioControl.controlMutex);
                if (sc != RTEMS_SUCCESSFUL) {
                    res = RTEMS_IO_ERROR;
                }
            break;

            // TBD Sync
            case RTEMS_BLKDEV_REQ_SYNC:
                OS_DPRINTF1 ("Sync request\n\n");
            break;

            default:
                OS_DPRINTF1("Bad request\n\n");
        }
        // We need to finish the request with the request done function
        rtems_blkdev_request_done(reqArg, res);
    }
    else if (req == RTEMS_BLKIO_CAPABILITIES) 
    {
        *(uint32_t *) arg = RTEMS_BLKDEV_CAP_MULTISECTOR_CONT;
        res = RTEMS_SUCCESSFUL;
    }
    else {
        // Call a default ioctl to handle such a request
        res = rtems_blkdev_ioctl(dd, req, arg);
    }
    return res;
}

/// SDcard driver initialization
/// 
/// @param[in] major: Disk major device number
/// @param[in] minor: Minor device number, not applicable
/// @param[in] arg: Minor device number, not applicable
/// @return RTEMS_SUCCESSFUL on success or RTEMS_IO_ERROR on error
static rtems_device_driver osDrvSdioSDInitialize(rtems_device_major_number major, rtems_device_minor_number minor, void *arg)
{
    UNUSED(arg);// hush the compiler warning.

    rtems_status_code res = RTEMS_SUCCESSFUL;
    dev_t dev;
    u64 blocks = 0;
    u32 size = 0, i = 0;
    DrvSdioOpts opts[DRVSDIO_MAXDRIVERCARDS];

    // Register IRQ handler
    BSP_Set_interrupt_type_priority(IRQ_SDIO, POS_LEVEL_INT, osDrvSdioInfo.interruptPriority);
    BSP_interrupt_register(IRQ_SDIO, NULL, (rtems_interrupt_handler) osDrvSdioIrqHandler, NULL);

    // Init Disk IO
    res = rtems_disk_io_initialize();
    if (res != RTEMS_SUCCESSFUL)
    {
        return RTEMS_IO_ERROR;
    }
    // Need to allocate space for descriptors
    blocks = rtems_bdbuf_configuration.max_write_blocks > rtems_bdbuf_configuration.max_read_ahead_blocks ? rtems_bdbuf_configuration.max_write_blocks : rtems_bdbuf_configuration.max_read_ahead_blocks;
    size = ((rtems_bdbuf_configuration.buffer_max * blocks * sizeof(OsDrvSdio_Descriptors)) / DRVSDIO_MAXDMATRANSACTIONSIZE) + 3 * sizeof(OsDrvSdio_Descriptors);
    osSdioControl.descriptors = (OsDrvSdio_Descriptors *) malloc(size);
    OS_DPRINTF1("Descriptors: 0x%x Size %d End 0x%x\n", osSdioControl.descriptors, size, osSdioControl.descriptors + size);

    osSdioControl.txList = (tyDrvSdio_Transaction *) malloc (blocks * sizeof(tyDrvSdio_Transaction));
    OS_DPRINTF1("TX List: 0x%x Size %d\n", osSdioControl.txList, blocks * sizeof(tyDrvSdio_Transaction));

    osSdioControl.rxList = (tyDrvSdio_Transaction *) malloc (blocks * sizeof(tyDrvSdio_Transaction));
    OS_DPRINTF1("RX List: 0x%x Size %d\n", osSdioControl.rxList, blocks * sizeof(tyDrvSdio_Transaction));

    if ((osSdioControl.descriptors == NULL) || (osSdioControl.txList == NULL) || (osSdioControl.rxList == NULL)) {
        OS_DPRINTF1("Error while trying to allocate memory for descriptors/tx/rx list!\n");
        if (osSdioControl.descriptors != NULL)
            free(osSdioControl.descriptors);
        if (osSdioControl.txList != NULL)
            free(osSdioControl.txList);
        if (osSdioControl.rxList != NULL)
            free(osSdioControl.rxList);
        return RTEMS_NO_MEMORY;
    }

    // Set default parameters in SDIO initialization struct
    for(i = 0; i < DRVSDIO_MAXDRIVERCARDS; ++i)
    {
        opts[i].callback = NULL;
        opts[i].max_speed_mode = SDIO_INVALID_SPEED_MODE;
    }

    // Set application specific parameters in SDIO initialization struct
    for(i = 0; i < osDrvSdioInfo.slots; ++i)
    {
        u8 slot = osDrvSdioInfo.slotInfo[i].slot;

        opts[slot].callback = osDrvSdioInfo.slotInfo[i].volSwCallback;
        opts[slot].max_speed_mode = osDrvSdioInfo.slotInfo[i].maxSpeed;
    }

    res = DrvSdioInitialise(osSdioControl.sdioClock, (u32) osSdioControl.descriptors, opts);
    if (res != RTEMS_SUCCESSFUL) {
        return RTEMS_IO_ERROR;
    }
    for (minor = 0; minor < osDrvSdioInfo.slots; ++minor)
    {
        /* Initialize disk IO */
        dev = rtems_filesystem_make_dev_t(major, minor);
        // Init the card
        res = osDrvSdioCardEnumeration(osDrvSdioInfo.slotInfo[minor].slot);
        if (res != RTEMS_SUCCESSFUL) {
            return RTEMS_IO_ERROR;
        }
        if (res == RTEMS_SUCCESSFUL) {
           /* Create disk device */
           res = DrvSdioGetCardBlockNumber(osDrvSdioInfo.slotInfo[minor].slot, &blocks);
           if (res == RTEMS_SUCCESSFUL)  // Block size How to obtain it ? TBD
                res = rtems_disk_create_phys (dev, OSDRVSDIO_DEFAULTBLOCKSIZE, blocks, osDrvSdioIOCtl, NULL, osDrvSdioInfo.slotInfo[minor].devName);
           OS_DPRINTF1("rtems_disk_create_phys() res %s Blocks %d\n", rtems_status_text(res), blocks);
           if (res != RTEMS_SUCCESSFUL)
                res = RTEMS_IO_ERROR;
        } else
            res = RTEMS_IO_ERROR;
    }

    return res;
}

int OsDrvSdioInit(osDrvSdioEntries_t *info)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    rtems_device_major_number major;
    // Check input info
    if (info == NULL) {
        OS_DPRINTF1("\nError in info struct\n");
        return OS_DRV_SDIO_ERROR;
    }
    // Get the Sdio Clock
    sc = OsDrvCprGetClockFreqKhz(AUX_CLK_SDIO,NULL, (s32 *) &osSdioControl.sdioClock);

    if (sc != RTEMS_SUCCESSFUL) {
        return OS_DRV_SDIO_GET_CLOCKS_ERROR;
    }
    osDrvSdioInfo = *info;
    sc = rtems_semaphore_create( rtems_build_name('S', 'D', '0', '1'), 1, RTEMS_SIMPLE_BINARY_SEMAPHORE, 1, &osSdioControl.controlMutex);
    OS_DPRINTF1("\nrtems_semaphore_create %d\n", sc);
    if (sc == RTEMS_SUCCESSFUL) {
        // Register the sdio driver 
        sc = rtems_io_register_driver (0, &osDrvSdioDriverTable, &major);
        if (sc == RTEMS_SUCCESSFUL) {
            sc = OS_DRV_SDIO_SUCCESS;
        }
    }
    return sc;
}


///@}
