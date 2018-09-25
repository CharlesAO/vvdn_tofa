///
/// @file OsDrvI2cBus.c
/// 
/// 
/// @ingroup OsDrvI2cBus
/// @{
///

#include "OsDrvI2cBus.h"
#include <bsp/irq-generic.h>
#include "OsDrvCpr.h"
#include "OsCommon.h"
#include <rtems/status-checks.h>
#include "OsCross.h"
#include <rtems.h>


#define MAX(a, b) ((a) > (b))?(a):(b)
#define MIN(a, b) ((a) < (b))?(a):(b)

i2cConfigModes i2cConfig[I2C_SPEED_MODES] = {
        {        //I2C_SS_CONFIG
                .hcnt = I2C_SS_HCNT,
                .lcnt = I2C_SS_LCNT,
                .mode = I2C_SS_MOD
        },
        {        //I2C_FS_CONFIG
                .hcnt = I2C_FS_HCNT,
                .lcnt = I2C_FS_LCNT,
                .mode = I2C_FS_MOD
        },
        {       //I2C_FSP_CONFIG
                .hcnt = I2C_FSP_HCNT,
                .lcnt = I2C_FSP_LCNT,
                .mode = I2C_FS_MOD
        },
        {       //I2C_HS_CONFIG
                .hcnt = I2C_HS_HCNT,
                .lcnt = I2C_HS_LCNT,
                .mode = I2C_HS_MOD
        }
};

// SCL_H_TIME = (HCNT + SPKLEN + 7 )*IC_CLK + SCL_FallTime;
static int getI2cHCNTcc(u32 timeNs,
                                                 u32 *sclHigh)
{
    u32 rv = OS_MYR_DRV_SUCCESS, spklen = 0, sysClk;
    // Get current sys clk:
    if (OsDrvCprGetSysClockKhz(&sysClk) != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("Error: OsDrvCprGetSysClockKhz FAILED! Unable to get sys clk in order to calculate I2C SCL Low and High timing\n");
        return OS_MYR_DRV_I2C_INVALID_CLOCK;
    }

    // icClk in ns. I2C it's on the slow AHB, so we get half of the sysClk
    float icClk = 2.0 * 1000000.0 / sysClk;
    // SPKLEN is different in HS mode than in SS, FS and FSP
    if (timeNs == i2cConfig[I2C_SPEED_HS].hcnt)
        spklen = I2C_SPKLEN_HS;
    else
        spklen = I2C_SPKLEN_SS_FS_FSP;

    *sclHigh = (int) ((float) (((float) (timeNs - I2C_SCL_FALL_TIME_NS)) / icClk + 0.5)
                      - 7 - spklen);

    if (*sclHigh <= 0){
        OS_I2C_DPRINTF1("getI2cHCNTcc() FAILED to calculate SCL HCNT: Increase the system clock frequency\n");
        return OS_MYR_DRV_I2C_INVALID_CLOCK;
    }
    return rv;
}

// SCL_L_TINME = (LCNT + 1)*IC_CLK - SCL_FallTime + SCL_RiseTime
static int getI2cLCNTcc(u32 timeNs,
                                                 u32 *sclLow)
{
    u32 rv = OS_MYR_DRV_SUCCESS, sysClk;
    // Get current sys clk:
    if ((rv = OsDrvCprGetSysClockKhz(&sysClk)) != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("Error: Unable to get sys clk in order to calculate:\n"
                        "    I2C SCL Low and High timing\n RetVal %d\n",
                        rv);
        return OS_MYR_DRV_I2C_INVALID_CLOCK;
    }

    // icClk in ns. I2C it's on the slow AHB, so we get half of the sysClk
    float icClk = 2.0 * 1000000.0 / sysClk; // icClk in ns

    *sclLow = (int) ((float) (((float) (timeNs + I2C_SCL_FALL_TIME_NS - I2C_SCL_RISE_TIME_NS)) / icClk
                              + 0.5) - 1);
    if (*sclLow <= 0){
        OS_I2C_DPRINTF1("getI2cLCNTcc() FAILED to calculate SCL HCNT: \n"
                        "    Increase the system clock frequency\n");
        return OS_MYR_DRV_I2C_INVALID_CLOCK;
    }
    return rv;
}

// Calculate the number of I2C_In_clk(which is sysClock/2) needed
// to set the IIC_SDA_HOLD register to, in order to meet the standard
// requiremet of a SDA_Hold_Time >= 300 ns.
static int getI2cSdaHoldTime(void)
{
    u32 rv = OS_MYR_DRV_SUCCESS;
    u32 sysClk;
    fp32 sysClkNs;

    // Get current sys clk:
    if ((rv = OsDrvCprGetSysClockKhz(&sysClk)) != OS_MYR_DRV_SUCCESS){
        OS_I2C_DPRINTF1("Error: Unable to get sys clk in order to calculate:"
                        "    I2C SDA Hold RetVal %d\n",
                        rv);
        return OS_MYR_DRV_I2C_INVALID_CLOCK;
    }

    sysClkNs = ((fp32)(1e6)) / ((fp32)sysClk);

    return 300/sysClkNs/2;
}

//-------------------------------------------------------------------------------------
// ISR Handling
//-------------------------------------------------------------------------------------
// Handle TX in ISR
static u32 myr2I2cTxFifoEmptyHandler(myr2I2cRegs_t *i2cregs,
                                          myr2I2cBusEntry_t *e)
{
    u32 slots = 0, count = 0, cmd = 0;

    if (e->data <= e->end){
        // Work out empty slots
        slots = IIC_TX_FIFO_DEPTH - i2cregs->txLevel;
        while ((e->data <= e->end) && (count < slots)){
            // get new word from buffer
            cmd = *(e->data++);
            // send start bit if necessary
            if (e->sendStart)
            {
                cmd |= (1 << IIC_DATA_CMD_RESTART);
                e->sendStart = 0;
            }
            // send stop bit if necessary
            if ((e->data > e->end) && (e->sendStop))
            {
                cmd |= (1 << IIC_DATA_CMD_STOP);
                e->sendStop = 0;
            }
            // write FIFO register
            i2cregs->cmdData = cmd;
            count++;
        }

        // Adapt threshold to maintain flow of data
        if (e->end == e->data)
            i2cregs->txThresh = 0;
        else
            i2cregs->txThresh = (i2cregs->txLevel >> 1) - 1;
    }
    else{
        // End transaction
        e->data = e->end = NULL;

        // Transmission finished, nothing left to send
        i2cregs->intrMask &= ~IIC_IRQ_TX_EMPTY;

        // Nothing to do
        e->interruptStatus = I2C_MASTER_IDLE;

        // Can disable now, once we sent all bytes
        bsp_interrupt_vector_disable(e->vector);

        // Notify Task
        rtems_event_send(e->taskId, OSCOMMON_EVENT_I2C_MTX_END);
    }
    return count;
}

// Handle RX in ISR
static u32 myr2I2cRxFifoFullHandler(volatile myr2I2cRegs_t *i2cregs,
                                         myr2I2cBusEntry_t *e)
{
    u32 slots = 0, count = 0, cmd = 0;
    if (e->data <= e->end)
    {
        // Read available data
        cmd = i2cregs->rxLevel;
        for (count = 0; count < cmd; count++)
        {
            *(e->data++) = (u8) i2cregs->cmdData;
        }

        // Work out slots
        slots = MIN(e->rxReq, IIC_TX_FIFO_DEPTH - i2cregs->txLevel);

        for (count = 0; count < slots; count++)
        {
            e->rxReq--;

            // Request pending data
            cmd = (1 << IIC_DATA_CMD_READ);

            // Send Start only if required
            if (e->sendStart){
                cmd |= (1 << IIC_DATA_CMD_RESTART);
                e->sendStart = 0;
            }
            if ((e->rxReq == 0) && (e->sendStop))
            {
                cmd |= (1 << IIC_DATA_CMD_STOP);
                e->sendStop = 0;
            }
            i2cregs->cmdData = cmd;
        }

        // Update Threshold
        // TODO: negative fix
        i2cregs->rxThresh = MIN((IIC_RX_FIFO_DEPTH >> 1), e->end - e->data + 1) - 1;
    }
    if (e->data > e->end)
    {
        e->data = e->end = NULL;

            // Notify Task
            rtems_event_send(e->taskId, OSCOMMON_EVENT_I2C_MRX_END);

            // Nothing to do
            e->interruptStatus = I2C_MASTER_IDLE;

            // Disable interrupt
            bsp_interrupt_vector_disable(e->vector);
        }
    return slots;
}

// Handle TX Abort in ISR
static void myr2I2cTxAbortHandler(myr2I2cRegs_t *i2cregs, myr2I2cBusEntry_t *e)
{
    if (e->interruptStatus == I2C_MASTER_TRANSMISSION){
        // Nothing to do
        e->interruptStatus = I2C_MASTER_IDLE;

        // Disable TX interrupt
        i2cregs->intrMask &= ~IIC_IRQ_TX_EMPTY;

        // Notify Task
        rtems_event_send(e->taskId, OSCOMMON_EVENT_I2C_MTX_NACK);
    }
    else if (e->interruptStatus == I2C_MASTER_RECEPTION){
        // Nothing to do
        e->interruptStatus = I2C_MASTER_IDLE;

        // Notify Task
        rtems_event_send(e->taskId, OSCOMMON_EVENT_I2C_MRX_NACK);
    }
}

static void myr2I2cHandler(void *arg)
{
    myr2I2cBusEntry_t *e = arg;
    myr2I2cRegs_t *i2cregs = (myr2I2cRegs_t *) e->regs;
    u32 interrupts = 0, tmp = 0; UNUSED(tmp);
    interrupts = i2cregs->intrStat ;

    // TX Abort, may be brought about by tx or rx
    if (interrupts & IIC_IRQ_TX_ABRT)
    {
        //save abort source
        e->txAbrtSource = e->regs->txAbortSource;

        // Clear Int
        tmp = i2cregs->clearTxAbrtIntr;

        // Take actions
        myr2I2cTxAbortHandler(i2cregs, e);
    }

    // TX Buffer below threshold
    if (interrupts & IIC_IRQ_TX_EMPTY)
    {
        if (e->interruptStatus == I2C_MASTER_TRANSMISSION)
            myr2I2cTxFifoEmptyHandler(i2cregs, e);
        else
            i2cregs->intrMask &= ~IIC_IRQ_TX_EMPTY;
    }

    // RX Buffer above threshold
    if ((interrupts & IIC_IRQ_RX_FULL) && (e->interruptStatus == I2C_MASTER_RECEPTION))
    {
        myr2I2cRxFifoFullHandler(i2cregs, e);
    }

    // We need to clear the interrupt at the end of the handler
    BSP_Clear_interrupt(IRQ_IIC1 + e->index);
}

/// Allows to flush the RX FIFO without having to disable the I2C module
static u32 myr2I2cRxFifoFlush(myr2I2cBusEntry_t *bus)
{
    u32 count = 0, tmp = 0; UNUSED(tmp);
    myr2I2cRegs_t *iic = (myr2I2cRegs_t *) bus->regs;
    while (iic->rxLevel > 0){
        count++;
        tmp = iic->cmdData;
    }
    return count;
}

static rtems_event_set myr2I2cWaitWithEvent(myr2I2cBusEntry_t *e,
                                              s32 transfer_type)
{
    bsp_interrupt_vector_enable(e->vector);
    rtems_event_set ev_s;
    rtems_event_receive(transfer_type,
                        RTEMS_WAIT | RTEMS_EVENT_ANY,
                        I2C_MASTER_MAX_OP_TIMEOUT,
                        &ev_s);
    OS_I2C_DPRINTF2("myr2I2cWait() event out %d\n", ev_s);
    return ev_s;
}

static s32 excuteLastReadTransactionIfNeeded(rtems_libi2c_bus_t *bus)
{
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;
    volatile myr2I2cRegs_t *regs = e->regs;
    rtems_event_set ev;

    if(e->interruptStatus == I2C_MASTER_RECEPTION)
    {
        /// Set threshold
        regs->rxThresh = 0;

        myr2I2cRxFifoFullHandler(regs, e);

        // Wait for hardware to finish transaction (handled in ISR)
        ev = myr2I2cWaitWithEvent(e,
                                  OSCOMMON_EVENT_I2C_MRX_END |
                                  OSCOMMON_EVENT_I2C_MRX_NACK);
        if(ev & OSCOMMON_EVENT_I2C_MRX_END)
        {
            // return number of bytes sent
            return RTEMS_SUCCESSFUL;
        }
        else if (ev & OSCOMMON_EVENT_I2C_MRX_NACK)
        {
            // peripheral did not respond (NACK)
            return -RTEMS_IO_ERROR;
        }
        else
        {
            // TODO: reset peripheral to resume normality
            return -RTEMS_TIMEOUT;
        }
    }
    return RTEMS_SUCCESSFUL;
}

static s32 excuteLastWriteTransactionIfNeeded(rtems_libi2c_bus_t *bus)
{
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;
    volatile myr2I2cRegs_t *regs = e->regs;
    rtems_event_set ev;

    if(e->interruptStatus == I2C_MASTER_TRANSMISSION)
    {
        // Set Threshold
        regs->txThresh = 0;
        // Enable FIFO Empty interrupt, will trigger interrupt
        regs->intrMask |= IIC_IRQ_TX_EMPTY;

        // Wait for hardware to finish transaction (handled in ISR)
        ev = myr2I2cWaitWithEvent(e,
                                  OSCOMMON_EVENT_I2C_MTX_END |
                                  OSCOMMON_EVENT_I2C_MTX_NACK);
        if(ev & OSCOMMON_EVENT_I2C_MTX_END)
        {
            // return number of bytes sent
            return RTEMS_SUCCESSFUL;
    }
        else if (ev & OSCOMMON_EVENT_I2C_MTX_NACK)
        {
            // peripheral did not respond (NACK)
            return -RTEMS_IO_ERROR;
        }
        else
        {
            // TODO: reset peripheral to recover properl
            return -RTEMS_TIMEOUT;
        }
    }
    return RTEMS_SUCCESSFUL;
}

static int myr2I2cSendAddr(rtems_libi2c_bus_t *bus,
                                            u32 addr,
                                            s32 rw)
{
    UNUSED(rw); // hush the compiler warning.

    OS_MYRIAD_DRIVER_STATUS_CODE sc = OS_MYR_DRV_SUCCESS;

    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;
    volatile myr2I2cRegs_t *regs = e->regs;

    sc = excuteLastReadTransactionIfNeeded(bus);
    if(sc)
    {
        return sc;
    }
    sc = excuteLastWriteTransactionIfNeeded(bus);
    if(sc)
    {
        return sc;
    }

    // Set target address
    regs->targetAddr = addr;
    if (e->cfg.addr10bit)
        regs->targetAddr |= (1 << IIC_TAR_10BITADDR_MASTER);

    return sc;
}

static s32 myr2I2cRead(rtems_libi2c_bus_t *bus, u8 *in, s32 n)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;

    if(in == NULL)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    e->taskId = rtems_task_self();

    sc = excuteLastReadTransactionIfNeeded(bus);
    if(sc)
    {
        return sc;
    }

    if(e->interruptStatus != I2C_MASTER_IDLE)
    {
        return -RTEMS_UNSATISFIED;
    }
    else
    {
        if (n <= 0)
        {
            return n;
        }

        // Flush RX Buffer
        myr2I2cRxFifoFlush(e);

        /* Setup receive buffer */
        e->data = in;
        e->end = in + n - 1;
        e->rxReq = n;
        e->interruptStatus = I2C_MASTER_RECEPTION;

        return n;
    }
}

static s32 myr2I2cWrite(rtems_libi2c_bus_t *bus, u8 *out, s32 n)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;

    if(out == NULL)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    e->taskId = rtems_task_self();

    sc = excuteLastWriteTransactionIfNeeded(bus);
    if(sc)
    {
        return sc;
    }

    if(e->interruptStatus != I2C_MASTER_IDLE)
    {
        return -RTEMS_UNSATISFIED;
    }
    else
    {
        // paranoia check
        if (n <= 0)
        {
        return n;
    }

    e->interruptStatus = I2C_MASTER_TRANSMISSION;

    /* Setup transmit buffer */
        e->data = out;
        e->end = out + n - 1;

        return n;
    }
}

static int myr2I2cInit(rtems_libi2c_bus_t *bus)
{
    OS_MYRIAD_DRIVER_STATUS_CODE rv = OS_MYR_DRV_SUCCESS;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    u32 sclLow = 0, sclHigh = 0;
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;
    volatile myr2I2cRegs_t *regs = e->regs;

    /* Disable module */
    regs->enable = 0;

    if ((rv = getI2cHCNTcc(i2cConfig[e->cfg.i2cDeviceSpeed].hcnt, &sclHigh)) != OS_MYR_DRV_SUCCESS){
        return OS_MYR_DRV_ERROR;
    }
    if ((rv = getI2cLCNTcc(i2cConfig[e->cfg.i2cDeviceSpeed].lcnt, &sclLow)) != OS_MYR_DRV_SUCCESS){
        return OS_MYR_DRV_ERROR;
    }

    switch(i2cConfig[e->cfg.i2cDeviceSpeed].mode)
    {
    case 1:
        regs->ssSclHcnt = sclHigh;
        regs->ssSclLcnt = sclLow;
        break;
    case 2:
        regs->fsSclHcnt = sclHigh;
        regs->fsSclLcnt = sclLow;
        break;
    case 3:
        regs->hsSclHcnt = sclHigh;
        regs->hsSclLcnt = sclLow;
        break;
    default:
        return OS_MYR_DRV_ERROR;
    }

    regs->sdaHold = getI2cSdaHoldTime();

    // Enable only specific interrupts TX abort and RX Buffer Full
    regs->intrMask = (IIC_IRQ_TX_ABRT | IIC_IRQ_RX_FULL);

    /* Install interrupt handler and disable this vector */
    sc = rtems_interrupt_handler_install(IRQ_IIC1 + e->index,
                                         "I2C",
                                         RTEMS_INTERRUPT_UNIQUE,
                                         myr2I2cHandler,
                                         e);

    RTEMS_CHECK_SC(sc, "install interrupt handler");

    // Set Priority and level
    BSP_Set_interrupt_type_priority(IRQ_IIC1 + e->index,
                                    POS_LEVEL_INT,
                                    e->cfg.interruptPriority);

    bsp_interrupt_vector_disable(e->vector);

    /* Enable module in master mode */
    regs->control = (1 << IIC_CON_ICSLAVEDISABLE) | (1 << IIC_CON_MASTERMODE)
                                    | (1 << IIC_CON_ICRESTARTEN)
                                    | ((i2cConfig[e->cfg.i2cDeviceSpeed].mode & IIC_CON_SPEED_MASK)
                                            << IIC_CON_SPEED);

    /* Enable module */
    regs->enable = 1;

    return OS_MYR_DRV_SUCCESS;
}

static int myr2I2cIOCtl(rtems_libi2c_bus_t *bus, s32 cmd, void *arg)
{
    // hush the compiler warning.
    UNUSED(cmd);
    UNUSED(arg);
    i2cTxAbrtSource *abrtSrc = (i2cTxAbrtSource *) arg;
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;
    s32 i;

    if(cmd == OS_I2CM_GET_LAST_ERROR)
    {
        if(abrtSrc != NULL)
        {
            *abrtSrc = OS_I2CM_STAT_ABRT_NO_ERROR_RECEIVED;
            for (i=I2C_TX_ABORT_ERRORS_NR-1; i>=0; i-- )
                if (e->txAbrtSource & (0x1 << i))
                {
                    *abrtSrc = i + OS_I2CM_STAT_ABRT_ENUM_OFFSET;
                    break;
                }
            e->txAbrtSource = OS_I2CM_STAT_ABRT_NO_ERROR_RECEIVED; //clear the error
            return OS_MYR_DRV_SUCCESS;
        }
        else
        {
            return RTEMS_INVALID_ADDRESS;
        }
    }
    return OS_MYR_DRV_SUCCESS;
}

// Generate a START
static int myr2I2cSendStart(rtems_libi2c_bus_t *bus)
{
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;
    e->sendStart = 1;
    e->sendStop = 0; // in case a previous command failed, we don't want a stop bit unless requested

    return OS_MYR_DRV_SUCCESS;
}

// Generate a STOP
static int myr2I2cSendStop(rtems_libi2c_bus_t *bus)
{
    myr2I2cBusEntry_t *e = (myr2I2cBusEntry_t *) bus;
    s32 sc = OS_MYR_DRV_SUCCESS;

    e->sendStop = 1;

    sc = excuteLastWriteTransactionIfNeeded(bus);
    if(sc)
        return sc;

    sc = excuteLastReadTransactionIfNeeded(bus);
    if(sc)
        return sc;

    return RTEMS_SUCCESSFUL;
}

const rtems_libi2c_bus_ops_t myr2I2cOps = {
  .init = (rtems_status_code (*) (rtems_libi2c_bus_t *))myr2I2cInit,
  .send_start = (rtems_status_code (*) (rtems_libi2c_bus_t * ))myr2I2cSendStart,
  .send_stop = (rtems_status_code (*) (rtems_libi2c_bus_t * ))myr2I2cSendStop,
  .send_addr = (rtems_status_code (*) (rtems_libi2c_bus_t * , u32 , int ))myr2I2cSendAddr,
  .read_bytes = (int (*) (rtems_libi2c_bus_t * , unsigned char *, int ))myr2I2cRead,
  .write_bytes = (int (*) (rtems_libi2c_bus_t * , unsigned char *, int ))myr2I2cWrite,
  .ioctl = (int (*) (rtems_libi2c_bus_t * , int   , void * ))myr2I2cIOCtl
};

///@}
