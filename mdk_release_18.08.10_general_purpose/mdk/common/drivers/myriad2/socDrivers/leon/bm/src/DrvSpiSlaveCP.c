///
/// @file DrvSpiSlaveCP.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSpiSlaveCP
/// @{
/// @brief     SPI Slave Functions.
///


// System Includes
// ----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>

// Application Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "registersMyriad.h"
#include "DrvSpiSlaveCP.h"
#include <DrvGpio.h>
#include "DrvIcb.h"
#include "DrvTimer.h"
#include "DrvRegUtils.h"


// Source Specific #defines and types (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define min(a,b)                          \
    ({ __typeof__ (a) _a = (a);           \
       __typeof__ (b) _b = (b);           \
       _a < _b ? _a : _b; })
#define max(a,b)                          \
    ({ __typeof__ (a) _a = (a);           \
       __typeof__ (b) _b = (b);           \
       _a > _b ? _a : _b; })

// Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------


#if SPISLAVEARRAYDEBUG

#define SPILOGSIZE 128
u8 spiIrqLog[SPILOGSIZE];
u32 spiIrqLog_i;

void spiSlaveLog(u8 x)
{
    spiIrqLog[spiIrqLog_i++] = x;
    if(spiIrqLog_i == SPILOGSIZE)
    spiIrqLog_i = 0;
}

#else

#define spiSlaveLog(x) ((void)(x))

#endif


// Static Local Data
// ----------------------------------------------------------------------------
static spiHandler_t* spiBareMetalHandlerArray[SPI_DEVICE_NO];

static void spiSlaveBmIrqHandler(u32 source);

static void spiSlaveCPRegisterTxPacket(spiHandler_t *handler, u8 channel, u8 flags, s32 size, void *buff);

static inline void assertMessageGpio(u32 gpio)
{
    DrvGpioSetPinLo(gpio);
}

static inline void deassertMessageGpio(u32 gpio)
{
    DrvGpioSetPinHi(gpio);
}

/// returns the base address of a SPI device given the device number
///@param[in]  device SPI device number
static inline u32 getSpiBaseAddr(int device)
{
    return ((device == SPI1) ? SPI1_BASE_ADR :
            (device == SPI2) ? SPI2_BASE_ADR :
            (device == SPI3) ? SPI3_BASE_ADR : 0);
}


static void txDisableInterrupt(spiHandler_t *handler)
{
    u32 val;

    val = GET_REG_WORD_VAL(handler->baseRegAddr + SPI_IMR_OFFSET);
    val &= ~SPI_IMR_TXEIM;
    SET_REG_WORD(handler->baseRegAddr + SPI_IMR_OFFSET, val);
}

static void txEnableInterrupt(spiHandler_t *handler)
{
    u32 val;

    val = GET_REG_WORD_VAL(handler->baseRegAddr + SPI_IMR_OFFSET);
    val |= SPI_IMR_TXEIM;
    SET_REG_WORD(handler->baseRegAddr + SPI_IMR_OFFSET, val);
}

static inline u32 getRxFifoData(spiHandler_t *handler)
{
    u32 word;
    word = GET_REG_HALF_VAL(handler->baseRegAddr + SPI_DR_OFFSET);
    return word;
}

static inline void setTxFifoData(spiHandler_t *handler, u32 d)
{
    SET_REG_HALF(handler->baseRegAddr + SPI_DR_OFFSET, d);
}

static s32 setFifo(spiHandler_t *handler, u32 offset, s32 requestedByteLength)
{
    s32 fifoLevel;

    // set the tx FIFO level for the next transfer
    fifoLevel = requestedByteLength >> (handler->bytePerWord - 1);

    // make sure it's positive
    fifoLevel = max(fifoLevel, 0);

    if (fifoLevel > FIFO_FULL_LEVEL)
    {
        fifoLevel = FIFO_FULL_LEVEL;
    }

    SET_REG_WORD(handler->baseRegAddr + offset, fifoLevel - 1);

    return fifoLevel;
}

static inline void setTxFifoTreshold(spiHandler_t *handler, s32 requestedByteLength)
{
    handler->pe.txFifoLevel = setFifo(handler, SPI_TXFTRL_OFFSET, requestedByteLength);
}

static inline void setRxFifoThreshold(spiHandler_t *handler, s32 requestedByteLength)
{
    handler->pe.rxFifoLevel = setFifo(handler, SPI_RXFTLR_OFFSET, requestedByteLength);
}

int DrvSpiSlaveCPInitBlock(spiHandler_t* handler,
                           wordSizeBytes_t bytePerWord,
                           dmaUsed_t useDma,
                           spiSlaveBlock_t device,
                           u32 cpol,
                           u32 cpha,
                           u32 hostIrqGpio,
                           spiTxStartCallback_t* txStartCb,
                           spiTxDoneCallback_t*  txDoneCb,
                           spiRxStartCallback_t* rxStartCb,
                           spiRxDoneCallback_t*  rxDoneCb,
                           spiPeDoneCallback_t*  peOverCb)
{
    UNUSED(useDma); // hush the compiler warning.
    u32 frameSize;
    u32 regVal;

    assert(device >= 0 && device < 3);

    handler->baseRegAddr = getSpiBaseAddr(device);
    handler->device = device;
    handler->irqSource = IRQ_SPI1 + device;

    handler->pe.spiRxStatus = SPI_IDLE;
    handler->pe.spiTxStatus = SPI_IDLE;
    handler->bytePerWord = bytePerWord;

    handler->hostIrqGpio = hostIrqGpio;

    //configure SPI block as Slave
    regVal = GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR);
    regVal = regVal & ~(1 << (handler->device + 4));
    SET_REG_WORD(CPR_GEN_CTRL_ADR, regVal);

    if (!rxStartCb || !peOverCb || !rxDoneCb)
    {
        //return with error if the read or write callbacks are not defined
        return -1;
    }

    handler->txStartCb = txStartCb;
    handler->txDoneCb = txDoneCb;
    handler->rxStartCb = rxStartCb;
    handler->rxDoneCb = rxDoneCb;
    handler->peOverCb = peOverCb;

    //disable SPI block
    SET_REG_WORD(handler->baseRegAddr + SPI_SSIENR_OFFSET, 0);

    frameSize = (bytePerWord == 1) ? SPI_ONE_BYTE_FRAME_SIZE : SPI_TWO_BYTES_FRAME_SIZE;
    //clear all interrupts
    GET_REG_WORD_VAL(handler->baseRegAddr + SPI_ICR_OFFSET);

    SET_REG_WORD(handler->baseRegAddr + SPI_CTRLR0_OFFSET,
                 SPI_CTRLR0_SRL_NORMAL  |
                 SPI_CTRLR0_SLV_OE_ENA  | // enable txd for slave
                 SPI_CTRLR0_TMOD_TX_RX  | // set mode as both transmit and receive
                 ((cpol & (u32)0x1) << SPI_CTRLR0_SCPOL_BIT)  |
                 ((cpha & (u32)0x1) << SPI_CTRLR0_SCPH_BIT)   |
                 SPI_CTRLR0_FRF_MOT_SPI   | // data frame format: Motorola SPI
                 frameSize );


    setRxFifoThreshold(handler, sizeof(spiPacketHeader));
    setTxFifoTreshold(handler, 16);

    //configure interrupts
    SET_REG_WORD(handler->baseRegAddr + SPI_IMR_OFFSET,
                 SPI_IMR_RXFIM | SPI_IMR_RXOIM | SPI_IMR_RXUIM |
                 SPI_IMR_TXOIM);

    //enable SPI block
    SET_REG_WORD(handler->baseRegAddr + SPI_SSIENR_OFFSET, 1);

    return 0;
}

int DrvSpiSlaveCPInit(spiHandler_t* handler,
                      wordSizeBytes_t bytePerWord,
                      dmaUsed_t useDma,
                      spiSlaveBlock_t device,
                      u32 cpol,
                      u32 cpha,
                      u32 interruptLevel,
                      u32 hostIrqGpio,
                      spiTxStartCallback_t* txStartCb,
                      spiTxDoneCallback_t*  txDoneCb,
                      spiRxStartCallback_t* rxStartCb,
                      spiRxDoneCallback_t*  rxDoneCb,
                      spiPeDoneCallback_t*  peOverCb)
{
    int pil, rc;

    rc = DrvSpiSlaveCPInitBlock(handler, bytePerWord, useDma, device, cpol, cpha, hostIrqGpio,
                                txStartCb, txDoneCb, rxStartCb, rxDoneCb, peOverCb);
    if(rc)
        return rc;

    spiBareMetalHandlerArray[device] = handler;

    /// Disable all Interrupts
    pil = swcLeonSetPIL(15);

    // Disable ICB (Interrupt Control Block) while setting new irq_sr
    DrvIcbDisableIrq(handler->irqSource);
    // Clear any existing interrupts
    DrvIcbIrqClear(handler->irqSource);
    // Configure irq_sr handlers
    DrvIcbSetIrqHandler(handler->irqSource, spiSlaveBmIrqHandler);
    // Configure interrupts
    DrvIcbConfigureIrq(handler->irqSource, interruptLevel, POS_EDGE_INT);
    // Trigger the interrupts
    DrvIcbEnableIrq(handler->irqSource);
    // Can enable the irq_sr now
    swcLeonSetPIL(pil);

    return 0;
}

static inline s32 spiSlaveIsRxFull(spiHandler_t *handler)
{
    return (GET_REG_WORD_VAL(handler->baseRegAddr + SPI_ISR_OFFSET) & SPI_ISR_RXFIS);
}

static inline s32 spiSlaveIsTxEmpty(spiHandler_t *handler)
{
    return (GET_REG_WORD_VAL(handler->baseRegAddr + SPI_ISR_OFFSET) & SPI_ISR_TXEIS);
}



static void txFifoFiller(spiHandler_t *handler)
{
    if(spiSlaveIsTxEmpty(handler))
    {
        switch(handler->pe.spiTxStatus)
        {
        case SPI_IDLE:
            return; // just do nothing and return early if we did not change the state
        case SPI_HEADER:
            if (handler->bytePerWord == 1)
            {
                setTxFifoData(handler, handler->pe.txPacket.ph.flags);
                setTxFifoData(handler, handler->pe.txPacket.ph.channelId);
                setTxFifoData(handler, (handler->pe.txPacket.ph.payloadLength >> 8) & 0xFF);
                setTxFifoData(handler, (handler->pe.txPacket.ph.payloadLength >> 0) & 0xFF);
            }
            else
            {
                setTxFifoData(handler,
                              ((handler->pe.txPacket.ph.flags << 8) & 0xFF00) |
                              (handler->pe.txPacket.ph.channelId & 0x00FF));
                setTxFifoData(handler, handler->pe.txPacket.ph.payloadLength);
            }

            handler->pe.txMetaAddr = (void*)handler->pe.txPacket.buff;
            handler->pe.txRemainingBytes = handler->pe.txPacket.ph.payloadLength;
            handler->pe.spiTxStatus = SPI_PAYLOAD;
            // no break ; continue to fill TX FIFO
        case SPI_PAYLOAD:
            while ( (GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET) & SPI_SR_TFNF) &&
                    (handler->pe.txRemainingBytes > 0) )
            {
                if (handler->bytePerWord == 1)
                {
                    setTxFifoData(handler, *(u8*)(handler->pe.txMetaAddr));
                }
                else
                {
                    setTxFifoData(handler, *(u16*)(handler->pe.txMetaAddr));
                }

                handler->pe.txRemainingBytes -= handler->bytePerWord;
                handler->pe.txMetaAddr += handler->bytePerWord;
            }

            if (handler->pe.txRemainingBytes <= 0)
            {
                handler->pe.spiTxStatus = SPI_FOOTER;
            }
            else
            {
                break;
            }
        case SPI_FOOTER:
            // if there is enough space in TX FIFO fill last word with 0xFFFF
            if(GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET) & SPI_SR_TFNF)
            {
                setTxFifoData(handler, 0);

                txDisableInterrupt(handler);

                handler->pe.spiTxStatus = SPI_IDLE;
            }
            break;
        default:
            assert(0);
            break;
        }
    }
}




static transactionType_t spiSlaveReadHeader(spiHandler_t *handler)
{
    transactionType_t transactionType = INVALID;
    u32 sr;

    // get headerMeta data
    if (handler->bytePerWord == 1)
    {
        handler->pe.rxPacket.ph.flags = getRxFifoData(handler);
        handler->pe.rxPacket.ph.channelId = getRxFifoData(handler);
        handler->pe.rxPacket.ph.payloadLength = getRxFifoData(handler) << 8;
        handler->pe.rxPacket.ph.payloadLength |= getRxFifoData(handler) << 0;
    }
    else
    {
        u32 headerMeta;
        headerMeta = getRxFifoData(handler);
        handler->pe.rxPacket.ph.flags = (u8)((headerMeta >> 8) & 0xFF);
        handler->pe.rxPacket.ph.channelId = (u8)((headerMeta >> 0) & 0xFF);
        handler->pe.rxPacket.ph.payloadLength = getRxFifoData(handler);
    }

    // mark the first bit (RX packet valid)
    transactionType |= RX_ONLY & (handler->pe.rxPacket.ph.flags != 0xFF &&
                                  handler->pe.rxPacket.ph.flags != 0x00);
    // mark the second bit (TX packet valid)
    sr = GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET);
    transactionType |= TX_ONLY & (((sr & SPI_SR_TFE)  == 0) << 1);

    return transactionType;
}


static inline s32 spiSlavePeripheralBusy(spiHandler_t *handler)
{
    int busy = 0;
    u32 sr = GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET);

    // check the state machine of the driver
    busy = busy || ((sr & SPI_SR_BUSY) != 0);
    busy = busy || ((sr & SPI_SR_TFE)  == 0);
    busy = busy || ((sr & SPI_SR_RFNE) != 0);

    return busy;
}


static inline void spiSlaveSetIdle(spiHandler_t *handler)
{
    handler->pe.spiRxStatus = SPI_IDLE;
    setRxFifoThreshold(handler, sizeof(spiPacketHeader));
}

static inline void spiSlaveStartPacketExchange(spiHandler_t *handler)
{
    if (spiSlaveIsRxFull(handler))
    {
        handler->pe.spiRxStatus = SPI_HEADER;
    }
    else if (spiSlaveIsTxEmpty(handler)) // if we could send and we did not receve anything yet
    {
        if ( !spiSlavePeripheralBusy(handler) ) // and if the block is not currently reciving
        {
            s32 size_next;
            void *buffer_next;
            s32 channel_next;

            if(handler->peOverCb(&channel_next, &buffer_next, &size_next)) // and if we have data to send
            {
                spiSlaveCPRegisterTxPacket(handler, channel_next, 0, size_next, buffer_next);
                txFifoFiller(handler); // start to fill header ASAP

                // expect a header in this case
                handler->pe.spiRxStatus = SPI_HEADER;
                setRxFifoThreshold(handler, sizeof(spiPacketHeader));
            }
            else
            {
                deassertMessageGpio(handler->hostIrqGpio);
                txDisableInterrupt(handler);
            }
        }
    }
}

static inline void spiSlaveConfigureReceive(spiHandler_t *handler)
{
    handler->pe.rxPacket.buff = (void*)(handler->rxStartCb(
                                        handler->pe.rxPacket.ph.payloadLength,
                                        handler->pe.rxPacket.ph.channelId,
                                        handler->pe.rxPacket.ph.flags));
    handler->pe.rxMetaAddr = (void*)handler->pe.rxPacket.buff;
    handler->pe.remainingRx = handler->pe.rxPacket.ph.payloadLength;
}


static inline void spiSlaveConfigureDataExchange(spiHandler_t *handler)
{
    switch(handler->pe.tt)
    {
    case INVALID:
        // both headers invalid ... probably an unrequested IRQ
        // waiting for a new transaction
        spiSlaveSetIdle(handler);
        break;
    case TX_ONLY:
        // we are just sending a packet
        handler->pe.spiRxStatus = SPI_RX_TX_OVERHEAD;
        handler->pe.remainingPayload = handler->pe.txPacket.ph.payloadLength;
        break;
    case RX_ONLY:
        // we are just receiving a packet
        handler->pe.spiRxStatus = SPI_PAYLOAD;
        spiSlaveConfigureReceive(handler);
        handler->pe.remainingPayload = handler->pe.rxPacket.ph.payloadLength;
        break;
    case RX_TX:
        // duplex packet
        spiSlaveConfigureReceive(handler);
        handler->pe.spiRxStatus = SPI_PAYLOAD;
        // payload exchange has the maximum size of RX and TX
        // (read or write overhead for different lengths)
        handler->pe.remainingPayload = max(handler->pe.rxPacket.ph.payloadLength,
                                                 handler->pe.txPacket.ph.payloadLength);
        break;
    default:
        assert(0);
        break;
    }

    setRxFifoThreshold(handler, handler->pe.remainingPayload);
}

static inline void spiSlaveReadData(spiHandler_t *handler)
{
    while ((handler->pe.remainingRx > 0) &&
           (GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET) & SPI_SR_RFNE))
    {
        u32 rxFifoData = getRxFifoData(handler);

        if ( (handler->bytePerWord == 1) || // word == 1 byte
             (handler->bytePerWord == 2 && handler->pe.remainingRx == 1)) // last byte in 2byte/word transaction
        {
            *(u8*)(handler->pe.rxMetaAddr) = (u8)rxFifoData;
        }
        else
        {
            *(u16*)(handler->pe.rxMetaAddr) = (u16)getRxFifoData(handler);
        }

        handler->pe.remainingRx -= handler->bytePerWord;
        handler->pe.remainingPayload -= handler->bytePerWord;

        handler->pe.rxMetaAddr += handler->bytePerWord;
    }
}

static inline void spiSlaveDumpRxData(spiHandler_t *handler)
{
    while ((handler->pe.remainingPayload > 0) &&
           (GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET) & SPI_SR_RFNE))
    {
        getRxFifoData(handler);
        handler->pe.remainingPayload -= handler->bytePerWord;
    }
}

static inline void spiSlaveDecideNextState(spiHandler_t *handler)
{
    if (handler->pe.remainingPayload > 0)
    {
        if(handler->pe.remainingRx > 0)
        {
            // stay in payload
            handler->pe.spiRxStatus = SPI_PAYLOAD;
            // set the rx FIFO level for the next transfer
            setRxFifoThreshold(handler, handler->pe.remainingPayload);
        }
        else
        {
            // go to RX overhead
            handler->pe.spiRxStatus = SPI_RX_TX_OVERHEAD;
            // set the rx FIFO level for the next transfer
            setRxFifoThreshold(handler, handler->pe.remainingPayload);
        }
    }
    else
    {
        handler->pe.spiRxStatus = SPI_FOOTER;
        setRxFifoThreshold(handler, handler->bytePerWord);
    }
}

static inline int spiSlaveReadFooter(spiHandler_t *handler)
{
    if(GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET) & SPI_SR_RFNE)
    {
        getRxFifoData(handler);
        return 1;
    }
    return 0;
}

static inline void spiSlaveEndPacketExchange(spiHandler_t *handler)
{

    if (handler->rxDoneCb)
    {
        handler->rxDoneCb(handler->pe.rxPacket.ph.payloadLength,
                          handler->pe.rxPacket.ph.channelId,
                          handler->pe.rxPacket.ph.flags,
                          (void*)handler->pe.rxPacket.buff);
    }

    handler->pe.spiRxStatus = SPI_IDLE;
    setRxFifoThreshold(handler, sizeof(spiPacketHeader));

    // check if we don't have a back-to-back tx transaction and
    // deassert IRQ if not
    txEnableInterrupt(handler);
}


void DrvSpiSlaveCPIsrHandler(spiHandler_t *handler)
{
    // Make sure we leave with RX empty
    while(spiSlaveIsRxFull(handler) || spiSlaveIsTxEmpty(handler))
    {
        // transaction is Master Driven, so RX_STATUS counts
        switch (handler->pe.spiRxStatus)
        {
        case SPI_IDLE:
        {
            spiSlaveStartPacketExchange(handler);
        }break;
        case SPI_HEADER:
        {
            // decide what type of transaction is, based on both headers
            handler->pe.tt = spiSlaveReadHeader(handler);

            // Configure payload tranzaction based on tranzaction type and header contents
            spiSlaveConfigureDataExchange(handler);
        }break;
        case SPI_PAYLOAD:
        {
            // fill fifo if we have to
            txFifoFiller(handler);

            spiSlaveReadData(handler);
            //spiSlaveWriteData(handler);

            spiSlaveDecideNextState(handler);
        }break;
        case SPI_RX_TX_OVERHEAD:
        {
            // fill fifo if we have to
            txFifoFiller(handler);

            // wait to transmit all the remaining data
            spiSlaveDumpRxData(handler);
            // switch to waiting next packet exchange
            spiSlaveDecideNextState(handler);
        }break;
        case SPI_FOOTER:
        {
            // read the master dummy
            if(spiSlaveReadFooter(handler))
            {
                spiSlaveEndPacketExchange(handler);
            }
        }break;
        default:
            assert(0);
            break;
        }
    }
}


static void spiSlaveBmIrqHandler(u32 source)
{
    spiHandler_t *handler = spiBareMetalHandlerArray[source - IRQ_SPI1];

    DrvSpiSlaveCPIsrHandler(handler);
}

void DrvSpiSlaveCPSendPacketWait(spiHandler_t *handler, u8 channel, u8 flags, s32 size, void *buff)
{
    while (spiSlavePeripheralBusy(handler))
    {
        __asm volatile ("nop\n");
    }

    spiSlaveCPRegisterTxPacket(handler, channel, flags, size, buff);
}

void spiSlaveCPRegisterTxPacket(spiHandler_t *handler, u8 channel, u8 flags, s32 size, void *buff)
{
    UNUSED(flags); // hush the compiler warning.
    handler->pe.txPacket.ph.payloadLength = size;
    handler->pe.txPacket.ph.flags = TXN_SLV_REQ;
    handler->pe.txPacket.ph.channelId = channel;
    handler->pe.txPacket.buff = buff;

    handler->pe.spiTxStatus = SPI_HEADER;
}


int DrvSpiSlaveCPClose(spiHandler_t *handler)
{
    u8 pollCount;

    // if there are still an ongoing transfer wait until it is finished
    pollCount = 0;
    while ((GET_REG_WORD_VAL(handler->baseRegAddr + SPI_SR_OFFSET) & SPI_SR_BUSY) &&
           (pollCount < SPI_MAX_DISABLE_COUNT))
    {
        DrvTimerInit();
        DrvTimerSleepMicro(SPI_DISABLE_TIMEOUT_US);
        pollCount++;
    }
    if (pollCount < SPI_MAX_DISABLE_COUNT)
    {
        // disable SPI block
        SET_REG_WORD(handler->baseRegAddr + SPI_SSIENR_OFFSET, 0);
        return 0;
    }
    //timeout error
    return 1;
}

void DrvSpiSlaveCPNotifyDataToSend(spiHandler_t *handler)
{
    if(! spiSlavePeripheralBusy(handler))
    {
        txEnableInterrupt(handler);
    }
    assertMessageGpio(handler->hostIrqGpio);
}

///@}
