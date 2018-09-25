///
/// @file DrvSpiSlaveDefinesCP.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSpiSlaveCP
/// @{
/// @brief     SPI type and constant definitions
///


#ifndef _DRV_SPI_SLAVE_DEFINES_CP_H_
#define _DRV_SPI_SLAVE_DEFINES_CP_H_

#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Control Register 0
#define SPI_CTRLR0_SRL              (0x0800)
#define SPI_CTRLR0_SRL_NORMAL       (0x0000)
#define SPI_CTRLR0_SRL_TEST         (0x0800)
#define SPI_CTRLR0_SLV_OE           (0x0400)
#define SPI_CTRLR0_SLV_OE_ENA       (0x0000)
#define SPI_CTRLR0_SLV_OE_DIS       (0x0400)
#define SPI_CTRLR0_TMOD_TX_RX       (0x0000)
#define SPI_CTRLR0_TMOD_TX_ONLY     (0x0100)
#define SPI_CTRLR0_TMOD_RX_ONLY     (0x0200)
#define SPI_CTRLR0_TMOD_EEPORM_RD   (0x0300)
#define SPI_CTRLR0_SCPOL            (0x0080)
#define SPI_CTRLR0_SCPOL_LOW        (0x0000)
#define SPI_CTRLR0_SCPOL_HI         (0x0080)
#define SPI_CTRLR0_SCPH             (0x0040)
#define SPI_CTRLR0_FRF_MOT_SPI      (0x0000)
#define SPI_CTRLR0_FRF_TI_SSP       (0x0010)
#define SPI_CTRLR0_FRF_NSM          (0x0020)
#define SPI_CTRLR0_SCPOL_BIT        (7)
#define SPI_CTRLR0_SCPH_BIT         (6)

// Status Register
#define SPI_SR_BUSY                 (0x01)
#define SPI_SR_TFNF                 (0x02)
#define SPI_SR_TFE                  (0x04)
#define SPI_SR_RFNE                 (0x08)
#define SPI_SR_RFF                  (0x10)
#define SPI_SR_TXE                  (0x20)
#define SPI_SR_DCOL                 (0x40)

// Interrupt Mask Register
#define SPI_IMR_TXEIM               (0x01)
#define SPI_IMR_TXOIM               (0x02)
#define SPI_IMR_RXUIM               (0x04)
#define SPI_IMR_RXOIM               (0x08)
#define SPI_IMR_RXFIM               (0x10)
#define SPI_IMR_MSTIM               (0x20)

// Interrupt Status Register
#define SPI_ISR_TXEIS               (0x01)
#define SPI_ISR_TXOIS               (0x02)
#define SPI_ISR_RXUIS               (0x04)
#define SPI_ISR_RXOIS               (0x08)
#define SPI_ISR_RXFIS               (0x10)
#define SPI_ISR_MSTIS               (0x20)

// Raw Interrupt Status Register
#define SPI_RISR_TXEIR               (0x01)
#define SPI_RISR_TXOIR               (0x02)
#define SPI_RISR_RXUIR               (0x04)
#define SPI_RISR_RXOIR               (0x08)
#define SPI_RISR_RXFIR               (0x10)
#define SPI_RISR_MSTIR               (0x20)

// DMA Control Register
#define SPI_DMACR_RDMAE              (0x1)
#define SPI_DMACR_TDMAE              (0x2)

#define SPI_FIFO_WIDTH_BYTES      2
#define SPI_TX_FIFO_DEPTH        64
#define SPI_RX_FIFO_DEPTH        64
#define FIFO_FULL_LEVEL (SPI_RX_FIFO_DEPTH >> 1)
#define TX_EMPTY_LEVEL (2)

#define SPI_DISABLE_TIMEOUT_US  100
#define SPI_MAX_DISABLE_COUNT     2

// error codes
#define ERR_SPI_BUSY  (1)

//SPI packet flags
#define TXN_START        (0x01)
#define TXN_CONTINUE     (0x02)
#define TXN_COMPLETE     (0x04)
#define TXN_ABORT        (0x08)
#define TXN_MST_READ     (0x10)
#define TXN_MST_WRITE    (0x20)
#define TXN_SLV_REQ      (0x40)

//transfer type
#define SPI_SLAVE_TRANSMIT 0
#define SPI_SLAVE_RECEIVE  1
#define SPI_SLAVE_IDLE     2

//supported frame sizes
#define SPI_ONE_BYTE_FRAME_SIZE  0x07
#define SPI_TWO_BYTES_FRAME_SIZE 0x0F

#define SPI_DEVICE_NO 3

#define DUMMY 0xFFFF

// SPI packet structure
typedef enum // block number
{
    SPI1 = 0,
    SPI2 = 1,
    SPI3 = 2,
} spiSlaveBlock_t;

typedef enum {
    WORD_1_BYTE = 1,
    WORD_2_BYTE = 2,
} wordSizeBytes_t; ///< word size in bytes

typedef enum {
    DMA_DISABLED = 0,
    DMA_ENABLED = 1,
} dmaUsed_t; ///< specifies whether DMA is used for transfers or not

typedef enum
{
    SPI_IDLE = 0,
    SPI_HEADER, 
    SPI_PAYLOAD,
    SPI_RX_TX_OVERHEAD,
    SPI_FOOTER,
} spiStatus_t;

typedef struct {
    uint8_t flags;
    uint8_t channelId;
    uint16_t payloadLength;
} spiPacketHeader;

typedef struct {
    spiPacketHeader ph;
    const void *buff;
} spiPacket;

typedef enum {
    INVALID = 0,
    RX_ONLY = 1,
    TX_ONLY = 2,
    RX_TX   = 3,
} transactionType_t;

typedef struct {
    spiStatus_t spiRxStatus;
    spiStatus_t spiTxStatus;

    transactionType_t tt;

    spiPacket rxPacket;
    spiPacket txPacket;

    s32 rxFifoLevel;
    s32 txFifoLevel;

    void * rxMetaAddr;
    void * txMetaAddr;

    s32 remainingRx;
    s32 txRemainingBytes;

    s32 remainingPayload;

} spiPacketExchange_t;


#define HEADER_SIZE  sizeof(spiPacketHeader)

///function called whenever a write request happens
/// @param[in]  length size in bytes of the received buffer
/// @param[in]  channel the channel on which the buffer is received
/// @param[in]  flags transfer parameters
typedef void* (spiTxStartCallback_t)(u16 length, u8 channel, u8 flags);

///function called whenever a read request happens
/// @param[in]  length size in bytes of the buffer to be sent
/// @param[in]  channel the channel on which the buffer will be sent
/// @param[in]  flags transfer parameters
typedef void* (spiRxStartCallback_t)(u16 length, u8 channel, u8 flags);

///function called when a write is finished
/// @param[in]  length size in bytes of the received buffer
/// @param[in]  channel the channel on which the buffer was received
/// @param[in]  flags transfer parameters
typedef s32 (spiTxDoneCallback_t)(u16 length, u8 channel, u8 flags, void* buffer);

///function called when a read is finished
/// @param[in]  length size in bytes of the sent buffer
/// @param[in]  channel the channel on which the buffer was be sent
/// @param[in]  flags transfer parameters
typedef s32 (spiRxDoneCallback_t)(u16 length, u8 channel, u8 flags, void* buffer);

///function called when packet exchange is finished
/// @param[in]  length size in bytes of the sent buffer
/// @param[in]  channel the channel on which the buffer was be sent
/// @param[in]  flags transfer parameters
typedef s32 (spiPeDoneCallback_t)(s32 *channel, void **buffer, s32 *size);

typedef struct {
    spiSlaveBlock_t device;  ///< SPI device number: 0 to 2
    spiPacketExchange_t pe;
    u32 baseRegAddr;
    u32 irqSource;
    u8 bytePerWord; ///< word size in bytes
    u32 hostIrqGpio;
    spiTxStartCallback_t* txStartCb;
    spiTxDoneCallback_t*  txDoneCb;
    spiRxStartCallback_t* rxStartCb;
    spiRxDoneCallback_t*  rxDoneCb;
    spiPeDoneCallback_t*  peOverCb;
} spiHandler_t;

void DrvSpiSendPacket(void *context, u8 channel, u8 flags, s32 size, void * buff);

int DrvSpiSlaveWaitUntilNotBusy(void);

#ifdef __cplusplus
}
#endif

#endif

///@}
