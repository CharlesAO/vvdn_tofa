///
/// @file DrvSpiDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSpi
/// @{
/// @brief     SPI type and constant definitions
///


#ifndef _DRV_SPI_SLAVE_DEFINES_H_
#define _DRV_SPI_SLAVE_DEFINES_H_

#include "mv_types.h"
#include "DrvAhbDma.h"

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

#define HEADER_SIZE  4

// number of microsecons to wait for SPI to be disabled
#define SPI_DISABLE_TIMEOUT_US       (100)
#define SPI_MAX_DISABLE_COUNT        (2)

#define SPI_FIFO_WIDTH_BYTES      2
#define SPI_TX_FIFO_DEPTH        64
#define SPI_RX_FIFO_DEPTH        64
//#define RX_FULL_LEVEL  (SPI_RX_FIFO_DEPTH >> 1)
//#define TX_FULL_LEVEL  (SPI_RX_FIFO_DEPTH >> 1)

#define SPI_DMA_RX_FULL_LEVEL  (8)
#define SPI_DMA_TX_EMPTY_LEVEL (16)

#define SPI_DMA_HW_INT_TX_0    (10)
#define SPI_DMA_HW_INT_RX_0    (11)
#define SPI_DMA_HW_INT_TX_1    (12)
#define SPI_DMA_HW_INT_RX_1    (13)
#define SPI_DMA_HW_INT_TX_2    (14)
#define SPI_DMA_HW_INT_RX_2    (15)
#define SPI_DMA_INT_LEVEL      (10)
#define SPI_DMA_CH_PRIO         (5)
#define SPI_DMA_TX_DEFAULT_CHANNEL  (2)
#define SPI_DMA_RX_DEFAULT_CHANNEL  (3)

#ifndef RX_FULL_LEVEL
#define RX_FULL_LEVEL  (32)
#endif
#ifndef TX_FULL_LEVEL
#define TX_FULL_LEVEL  (32)
#endif
#ifndef TX_EMPTY_LEVEL
#define TX_EMPTY_LEVEL (8)
#endif

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

#define DUMMY 0xFFFF

// SPI packet structure
typedef enum // block number
{
    SPI1 = 0,
    SPI2 = 1,
    SPI3 = 2,
    SPI_DEVICE_NO,
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
    READ_HEADER,
    WRITE_HEADER,
    READ_DATA,
    WRITE_DATA,
    WRITE_DUMMY,
    READ_DUMMY,
    RX_IDLE,
    TX_IDLE
} spiTransferState_t;

///function called whenever a write request happens
/// @param[in]  length size in bytes of the received buffer
/// @param[in]  channel the channel on which the buffer is received
/// @param[in]  flags transfer parameters
typedef void* (spiWriteRequestCallback_t)(u16 length, u8 channel, u8 flags);

///function called whenever a read request happens
/// @param[in]  length size in bytes of the buffer to be sent
/// @param[in]  channel the channel on which the buffer will be sent
/// @param[in]  flags transfer parameters
typedef void* (spiReadRequestCallback_t)(u16 length, u8 channel, u8 flags);

///function called when a write is finished
/// @param[in]  length size in bytes of the received buffer
/// @param[in]  channel the channel on which the buffer was received
/// @param[in]  flags transfer parameters
typedef s32 (spiWriteDoneCallback_t)(u16 length, u8 channel, u8 flags, void* buffer);

///function called when a read is finished
/// @param[in]  length size in bytes of the sent buffer
/// @param[in]  channel the channel on which the buffer was be sent
/// @param[in]  flags transfer parameters
typedef s32 (spiReadDoneCallback_t)(u16 length, u8 channel, u8 flags, void* buffer);

typedef struct {
    spiSlaveBlock_t device;  ///< SPI device number: 0 to 2
    u8 rxFlags; ///< receive data flags
    u8 txFlags; ///< transmit data flags
    u8 txChannel; ///< transmit channel
    u8 rxChannel; ///< receive channel
    u8 wordSizeBytes; ///< word size in bytes
    dmaUsed_t useDma; ///< specifies whether DMA is used for transfers or not
    void* rxBuffer;  ///< pointer to receive buffer
    void* txBuffer;  ///< pointer to transmit buffer
    u32 rxSize; ///< size of receive data
    u32 txSize; ///< size of transmit data
    ahbDmaHandler_t* txDmaHandler; ///< AHB DMA handler for transmission
    ahbDmaHandler_t* rxDmaHandler; ///< AHB DMA handler for reception
    spiTransferState_t rxState; ///< receive state
    spiTransferState_t txState; ///< transmit state
    spiWriteRequestCallback_t* writeCallback;
    spiWriteDoneCallback_t* writeDoneCallback;
    spiReadRequestCallback_t* readCallback;
    spiReadDoneCallback_t* readDoneCallback;
} spiHandler_t;

#endif

///@}
