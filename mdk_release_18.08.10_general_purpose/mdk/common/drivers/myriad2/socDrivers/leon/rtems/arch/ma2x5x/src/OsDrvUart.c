///
/// @file OsDrvUart.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvUart
/// @{
///

#include <OsDrvUart.h>

#include <rtems.h>
#include <rtems/libio.h>
#include <bsp.h>
#include <bsp/irq-generic.h>

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <mvMacros.h>

#include <swcFifo.h>
#include <swcLeonUtils.h>
#include <DrvLeonL2C.h>
#include <OsDrvCpr.h>

//#define OS_DRV_UART

#ifdef OS_DRV_UART
    #include <rtems/bspIo.h>
    #define OS_DPRINTK1(...)            printk(__VA_ARGS__)
#else
    #define OS_DPRINTK1(...)
#endif

typedef struct osUartHandler_t osUartHandler;

typedef void              (*HNDL) (osUartHandler *handler);
typedef rtems_status_code (*WRITE)(rtems_libio_rw_args_t *rw_args, osUartHandler *handler);
typedef rtems_status_code (*READ) (rtems_libio_rw_args_t *rw_args, osUartHandler *handler);

struct osUartHandler_t{
    OS_MYRIAD_DRIVER_STATUS status;

    rtems_vector_number irqVector;

    u32 ioTimeoutTicks;
    rtems_id threadWrId;
    rtems_id threadRdId;

    struct swcFifo_t rxFifo;
    struct swcFifo_t txFifo;

    struct osUartAFCSettings_t dataIn;
    struct osUartAFCSettings_t dataOut;

    HNDL  hndl;
    WRITE write;
    READ  read;
};

RTEMS_INTERRUPT_LOCK_MEMBER(TxLock)
RTEMS_INTERRUPT_LOCK_MEMBER(RxLock)

static struct osUartHandler_t *uartBlockHandlers;

static bool pushDataToBuffer(struct osUartAFCSettings_t * hndl, u8 data )
{
    if(hndl->index >= hndl->transferSize)
        return 0;

    hndl->location[hndl->index] = data;
    hndl->index++;

    return 1;
}

static bool popDataFromBuffer(struct osUartAFCSettings_t * hndl, u8 *data )
{
    if(hndl->index <= 0)
        return 0;

    *data = hndl->location[hndl->transferSize - hndl->index];
    hndl->index--;

    return 1;
}

static void AutoFlowControlHandler(struct osUartHandler_t * handler)
{
    u8 writeRx, readTx;
    s32 flagTx = 0, flagRx = 0;

    OS_DPRINTK1("ReceiveFifoLvl: %d\n", GET_REG_WORD_VAL(UART_RFL_ADR));

    while (GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_RFNE)
    {
        if(handler->dataOut.transferSize > handler->dataOut.index)
        {
            writeRx = GET_REG_WORD_VAL(UART_RBR_ADR);
            if(pushDataToBuffer(&handler->dataOut, writeRx))
            {
                OS_DPRINTK1("handler->dataOut: 0x%x\n", writeRx);
                flagRx = 1;
            }
            else
                break;
        }
        else
            break;
    }

    if(handler->dataOut.transferSize <= handler->dataOut.index && flagRx == 1)
        rtems_event_send(handler->threadRdId, OSCOMMON_EVENT_UART);

    while((GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_TFNF))
    {
        if(popDataFromBuffer(&handler->dataIn, &readTx))
        {
            SET_REG_WORD(UART_THR_ADR, readTx);
            OS_DPRINTK1("handler->dataIn: 0x%x\n", readTx);
            flagTx = 1;
        }
        else
            break;
    }

    OS_DPRINTK1("TransmitFifoLvl: %d\n", GET_REG_WORD_VAL(UART_TFL_ADR));

    if(handler->dataIn.index <= 0 && flagTx == 1)
    {
        rtems_event_send(handler->threadWrId, OSCOMMON_EVENT_UART);
        CLR_REG_BITS_MASK(UART_IER_ADR, (D_UART_IER_PTIME | D_UART_IER_ETBEI));
    }
}

static rtems_status_code AutoFlowControlWrite(rtems_libio_rw_args_t *rw_args, struct osUartHandler_t * handler)
{
    rtems_status_code sc;
    rtems_event_set eventOut;

    handler->dataIn.location     = (u8*)rw_args->buffer;
    handler->dataIn.transferSize = rw_args->count;
    handler->dataIn.index        = rw_args->count;

    SET_REG_BITS_MASK(UART_IER_ADR, (D_UART_IER_PTIME | D_UART_IER_ETBEI));

    sc = rtems_event_receive(
            OSCOMMON_EVENT_UART,
            RTEMS_WAIT | RTEMS_EVENT_ALL,
            handler->ioTimeoutTicks,
            &eventOut);

    rw_args->bytes_moved = handler->dataIn.index;
    if(sc != RTEMS_SUCCESSFUL)
        return sc;

    return sc;
}

static rtems_status_code AutoFlowControlRead(rtems_libio_rw_args_t *rw_args, struct osUartHandler_t * handler)
{
    rtems_status_code sc;
    rtems_event_set eventOut;

    handler->dataOut.location     = (u8*)rw_args->buffer;
    handler->dataOut.transferSize = rw_args->count;
    handler->dataOut.index        = 0;

    sc = rtems_event_receive(
            OSCOMMON_EVENT_UART,
            RTEMS_WAIT | RTEMS_EVENT_ALL,
            handler->ioTimeoutTicks,
            &eventOut);

    rw_args->bytes_moved = handler->dataOut.index;
    if(sc != RTEMS_SUCCESSFUL)
        return sc;

    return sc;
}

static void SwFifoHandler(struct osUartHandler_t * handler)
{
    u8 writeRx, readTx;
    s32 flagTx = 0, flagRx = 0;

    OS_DPRINTK1("ReceiveFifoLvl: %d\n", GET_REG_WORD_VAL(UART_RFL_ADR));

    while (GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_RFNE)
    {
        if(swcFifoAvailable(&handler->rxFifo))
        {
            writeRx = GET_REG_WORD_VAL(UART_RBR_ADR);
            if(!swcFifoPush8Bit(&handler->rxFifo, writeRx))
            {
                flagRx = 1;
                OS_DPRINTK1("read: %d\n", writeRx);
            }
        }
        else
            break;
    }

    if(flagRx == 1)
        rtems_event_send(handler->threadRdId, OSCOMMON_EVENT_UART);

    while((GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_TFNF))
    {
        if(!swcFifoPop8Bit(&handler->txFifo, &readTx))
        {
            SET_REG_WORD(UART_THR_ADR, readTx);
            flagTx = 1;
            OS_DPRINTK1("write: %d\n", readTx);
        }
        else
            break;
    }

    OS_DPRINTK1("TransmitFifoLvl: %d\n", GET_REG_WORD_VAL(UART_TFL_ADR));

    if(!swcFifoLength(&handler->txFifo) && flagTx == 1)
    {
        rtems_event_send(handler->threadWrId, OSCOMMON_EVENT_UART);
        CLR_REG_BITS_MASK(UART_IER_ADR, (D_UART_IER_PTIME | D_UART_IER_ETBEI));
    }
}

static rtems_status_code SwFifoWrite(rtems_libio_rw_args_t *rw_args, struct osUartHandler_t * handler)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    rtems_event_set eventOut;
    s32 available;
    void * wrPtr;
    void * crtPtr = rw_args->buffer;
    rtems_interrupt_lock_context lcontext;

    handler->threadWrId =rtems_task_self();
    rw_args->bytes_moved = 0;

    while(rw_args->count - rw_args->bytes_moved)
    {
        rtems_interrupt_lock_acquire(&TxLock, &lcontext);
        available = swcFifoGetWritePtr(&handler->txFifo, &wrPtr, rw_args->count - rw_args->bytes_moved);
        rtems_interrupt_lock_release(&TxLock, &lcontext);

        if(available > 0)
        {
            memcpy(wrPtr, crtPtr, available);

            rtems_interrupt_lock_acquire(&TxLock, &lcontext);
            sc = swcFifoMarkWriteDone(&handler->txFifo);
            rtems_interrupt_lock_release(&TxLock, &lcontext);
            if(sc != 0)
                return sc;

            crtPtr += available;
            rw_args->bytes_moved += available;

            SET_REG_BITS_MASK(UART_IER_ADR, (D_UART_IER_PTIME | D_UART_IER_ETBEI));
        }
        else
        {
            sc = rtems_event_receive(
                    OSCOMMON_EVENT_UART,
                    RTEMS_WAIT | RTEMS_EVENT_ALL,
                    handler->ioTimeoutTicks,
                    &eventOut);
            if(sc != RTEMS_SUCCESSFUL)
                return sc;
        }
    }

    sc = rtems_event_receive(
            OSCOMMON_EVENT_UART,
            RTEMS_WAIT | RTEMS_EVENT_ALL,
            handler->ioTimeoutTicks,
            &eventOut);
    if(sc != RTEMS_SUCCESSFUL)
        return sc;

    return sc;
}

static rtems_status_code SwFifoRead(rtems_libio_rw_args_t *rw_args, struct osUartHandler_t * handler)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    rtems_event_set eventOut;
    s32 readyBytes;
    void * rdPtr;
    void * crtPtr = rw_args->buffer;
    rtems_interrupt_lock_context lcontext;

    handler->threadRdId = rtems_task_self();
    rw_args->bytes_moved = 0;

    while(rw_args->bytes_moved < rw_args->count)
    {
        rtems_interrupt_lock_acquire(&RxLock, &lcontext);
        readyBytes = swcFifoGetReadPtr(&handler->rxFifo, &rdPtr, rw_args->count - rw_args->bytes_moved);
        rtems_interrupt_lock_release(&RxLock, &lcontext);

        if(readyBytes > 0)
        {
            memcpy(crtPtr, rdPtr, readyBytes);

            rtems_interrupt_lock_acquire(&RxLock, &lcontext);
            sc = swcFifoMarkReadDone(&handler->rxFifo);
            rtems_interrupt_lock_release(&RxLock, &lcontext);
            if(sc != 0)
                return sc;

            crtPtr += readyBytes;
            rw_args->bytes_moved += readyBytes;
        }
        else
        {
            sc = rtems_event_receive(
                    OSCOMMON_EVENT_UART,
                    RTEMS_WAIT | RTEMS_EVENT_ALL,
                    handler->ioTimeoutTicks,
                    &eventOut);
            if(sc != RTEMS_SUCCESSFUL)
                return sc;
        }
    }

    return sc;
}

static void OsUartIrqHandler(void * context)
{
    struct osUartHandler_t *handler = (struct osUartHandler_t*)context;

    handler->hndl(handler);

    BSP_Clear_interrupt(handler->irqVector);
}

static rtems_status_code setBaudrate(struct osUartHandler_t * handler, s32 baudRate)
{
    s32 uartSpeedHz, divider, UART_LCR, parity;
    u32 timeOut;

    if (OsDrvCprGetClockFreqKhz(AUX_CLK_IRDA, NULL, &uartSpeedHz) != OS_MYR_DRV_SUCCESS)
        return RTEMS_INVALID_CLOCK;

    uartSpeedHz *= 1000;

    divider = (uartSpeedHz << 1) / (baudRate * BIT_TIME);
    parity = divider & 1;
    divider = (divider >> 1) + parity;

    while (GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_BUSY)
    {
        for(timeOut = 0; timeOut < handler->ioTimeoutTicks &&
                (GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_BUSY); timeOut++)
            usleep(1);

        if(GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_BUSY)
        {
            return RTEMS_TIMEOUT;
        }
    }

    UART_LCR = GET_REG_WORD_VAL(UART_LCR_ADR);      //save initial value

    SET_REG_WORD(UART_LCR_ADR, D_UART_LCR_DLAB);    //set latch acces bit to 1

    SET_REG_WORD(UART_DLH_ADR, (divider >> 8) & 0xFF);
    SET_REG_WORD(UART_DLL_ADR, divider & 0xFF);

    SET_REG_WORD(UART_LCR_ADR, UART_LCR);           //set latch acces bit to initial value

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code enableSoftwareFIFO(struct osUartHandler_t * handler, struct osUartInitSettings_t * fifoInit)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    SET_REG_WORD(UART_MCR_ADR, GET_REG_WORD_VAL(UART_MCR_ADR) & ~D_UART_MCR_AFCE);
    SET_REG_WORD(UART_MCR_ADR, GET_REG_WORD_VAL(UART_MCR_ADR) & ~D_UART_MCR_RTS);
    SET_REG_WORD(UART_FCR_ADR, GET_REG_WORD_VAL(UART_FCR_ADR) | D_UART_FCR_RCVR_HALF);

    handler->hndl   = (HNDL)SwFifoHandler;
    handler->write  = (WRITE)SwFifoWrite;
    handler->read   = (READ)SwFifoRead;

    if(!(fifoInit->fifo.rxFifoLocation))
        sc = RTEMS_INVALID_ADDRESS;

    swcFifoInit(&handler->rxFifo, fifoInit->fifo.rxFifoLocation,
            fifoInit->fifo.rxFifoSize);

    if(!(fifoInit->fifo.txFifoLocation))
        sc =  RTEMS_INVALID_ADDRESS;

    swcFifoInit(&handler->txFifo, fifoInit->fifo.txFifoLocation,
            fifoInit->fifo.txFifoSize);

    return sc;
}

static rtems_device_driver uart_initialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    if(e == NULL)
        return RTEMS_SUCCESSFUL;
    else
    {
        struct osUartInitSettings_t *settings =
            (struct osUartInitSettings_t *)e;
         char *devname = "/dev/uart";
         s32 uartSpeedHz;

         struct osUartHandler_t * handler =
             (struct osUartHandler_t *)malloc(
                 sizeof(struct osUartHandler_t));
         memset(handler, 0, sizeof(*handler));

         rtems_interrupt_lock_initialize(&TxLock, "FifoTxLock");
         rtems_interrupt_lock_initialize(&RxLock, "FifoRxLock");

        rtems_status_code status;

        uartBlockHandlers = handler;

        handler->irqVector      = IRQ_UART;
        handler->ioTimeoutTicks = RTEMS_NO_TIMEOUT;

        handler->hndl           = (HNDL)AutoFlowControlHandler;
        handler->write          = (WRITE)AutoFlowControlWrite;
        handler->read           = (READ)AutoFlowControlRead;

        if(handler->status == OS_DRV_STAT_UNINITIALIZED)
        {
            handler->status = OS_DRV_STAT_INITIALIZED;

            if (OsDrvCprGetClockFreqKhz(AUX_CLK_IRDA, NULL, &uartSpeedHz) != OS_MYR_DRV_SUCCESS)
                return OS_MYR_DRV_CLOCK_ERROR;

            uartSpeedHz *= 1000;

            DrvUartInit(&settings->uartCfg, (u32)uartSpeedHz);

            SET_REG_WORD(UART_IER_ADR, 0);

            //select Auto Flow Control
            SET_REG_WORD(UART_MCR_ADR, GET_REG_WORD_VAL(UART_MCR_ADR) | D_UART_MCR_AFCE);
            SET_REG_WORD(UART_MCR_ADR, GET_REG_WORD_VAL(UART_MCR_ADR) | D_UART_MCR_RTS);
            SET_REG_WORD(UART_MCR_ADR, GET_REG_WORD_VAL(UART_MCR_ADR) & ~D_UART_MCR_SIRE);
            SET_REG_WORD(UART_FCR_ADR, GET_REG_WORD_VAL(UART_FCR_ADR) | D_UART_FCR_RCVR_2LESS);

            //Enable UART interrupt
            BSP_Clear_interrupt(handler->irqVector);

            BSP_Set_interrupt_type_priority(handler->irqVector,
                                            POS_LEVEL_INT,
                                            settings->interruptLevel);

            BSP_interrupt_register(handler->irqVector,
                                   "UART",
                                   OsUartIrqHandler,
                                   handler);

            status = rtems_io_register_name(devname, major, minor);
            if (status)
                return status;

            return RTEMS_SUCCESSFUL;
        }
        else return OS_MYR_DRV_ALREADY_INITIALIZED;
    }
}

static rtems_device_driver uart_open(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);

    struct osUartHandler_t * handler =
            (struct osUartHandler_t *)uartBlockHandlers;

    //because we don't assign any io pins to the RX signal, then RX will be driven zero
    while (GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_RFNE)
        GET_REG_WORD_VAL(UART_RBR_ADR);

    SET_REG_WORD(UART_IER_ADR, D_UART_IER_ERBFI);

    if(handler->status == OS_DRV_STAT_INITIALIZED)
        return RTEMS_SUCCESSFUL;
    else
        return RTEMS_NOT_DEFINED;
}

static rtems_device_driver uart_close(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    // hush the compiler warning.
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);


    return RTEMS_SUCCESSFUL;
}

static rtems_device_driver uart_write(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(minor);// hush the compiler warning.

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
    struct osUartHandler_t * handler =
            (struct osUartHandler_t *)uartBlockHandlers;

    handler->threadWrId =rtems_task_self();

    return handler->write(rw_args, (struct osUartHandler_t *)handler);
}

static rtems_device_driver uart_read(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(minor);// hush the compiler warning.

    rtems_libio_rw_args_t *rw_args = (rtems_libio_rw_args_t*) e;
        struct osUartHandler_t * handler =
            (struct osUartHandler_t *)uartBlockHandlers;

    handler->threadRdId = rtems_task_self();

    return handler->read(rw_args, handler);
}

static rtems_device_driver uart_control(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);// hush the compiler warning.
    UNUSED(minor);// hush the compiler warning.

    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    s32 read; UNUSED(read);

    struct osUartHandler_t * handler =
        (struct osUartHandler_t *)uartBlockHandlers;

    if (!ctl || !ctl->buffer)
    {
        return RTEMS_INVALID_ADDRESS;
    }

    switch (ctl->command)
    {
    case UART_SET_TIMEOUT_MS:
    {
        u32 timeout = (u32)ctl->buffer;

        // TODO: 1tick = 1ms for now
        handler->ioTimeoutTicks = timeout;
        ctl->ioctl_return = 0;
    } break;

    case UART_LOOPBACK_ENABLED:
    {
        SET_REG_WORD(UART_IER_ADR, GET_REG_WORD_VAL(UART_IER_ADR) & ~D_UART_IER_ERBFI);

        SET_REG_WORD(UART_MCR_ADR, GET_REG_WORD_VAL(UART_MCR_ADR) | D_UART_MCR_LB);

        //when we disable and enable again loopback mode, we receive some characters in UART
        while (GET_REG_WORD_VAL(UART_USR_ADR) & D_UART_USR_RFNE)
            read = GET_REG_WORD_VAL(UART_RBR_ADR);

        SET_REG_WORD(UART_IER_ADR, D_UART_IER_ERBFI);

        ctl->ioctl_return = 0;
    } break;

    case UART_LOOPBACK_DISABLED:
    {
        SET_REG_WORD(UART_MCR_ADR, GET_REG_WORD_VAL(UART_MCR_ADR) & ~D_UART_MCR_LB);

        ctl->ioctl_return = 0;
    } break;

    case UART_SET_BAUDRATE:
        sc = setBaudrate(handler, (u32)ctl->buffer);
        break;

    case UART_SW_FIFO_ENABLED:
        sc = enableSoftwareFIFO(handler, (struct osUartInitSettings_t *)ctl->buffer);
        break;

    default:
        return RTEMS_INVALID_NUMBER;
    }

    return sc;
}

rtems_driver_address_table uartDrvTbl = {              \
        uart_initialize,                               \
        uart_open,                                     \
        uart_close,                                    \
        uart_read,                                     \
        uart_write,                                    \
        uart_control                                   \
};

///@}
