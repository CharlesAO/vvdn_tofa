///
/// @file HglSpi.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglSpi HglSpi Driver
/// @{
/// @brief     Header of HGL for SPI
///


#ifndef HGLSPI_H__
#define HGLSPI_H__

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
#define SPI_CTRLR0_SCPOL_BIT        (1<<7)
#define SPI_CTRLR0_SCPH_BIT         (1<<6)
#define SPI_CTRLR0_DFS_MASK         (0x000F)

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

#define SPI_TX_FIFO_DEPTH        64
#define SPI_RX_FIFO_DEPTH        64

#define SPI_TX_FIFO_DEPTH_ISR    (32)
#define SPI_RX_FIFO_DEPTH_ISR    (SPI_RX_FIFO_DEPTH - 32)

#define SPI_TX_OVERFLOW  0x02
#define SPI_RX_OVERFLOW  0x08
#define SPI_RX_UNDERFLOW 0x04

// SPI packet structure
typedef enum // block number
{
    SPI1 = 0,
    SPI2 = 1,
    SPI3 = 2,
    SPI_DEVICE_NO,
} spiSlaveBlock_t;

typedef enum
{
    HGL_SPI_SLAVE,
    HGL_SPI_MASTER,
} spiType_t;

typedef enum
{
    SPI_MODE_RX_TX   = 0x0000,
    SPI_MODE_TX_ONLY = 0x0100,
    SPI_MODE_RX_ONLY = 0x0200,
} spiModeRxTx_t;

typedef enum {
    WORD_1_BYTE = 1,
    WORD_2_BYTE = 2,
} wordSizeBytes_t; ///< word size in bytes

typedef enum {
    DMA_DISABLED = 0,
    DMA_ENABLED = 1,
} dmaUsed_t; ///< specifies whether DMA is used for transfers or not

typedef enum {
    SPI_TX_IRQ_ENABLE = 0,
    SPI_TX_IRQ_DISABLE,
    SPI_RX_IRQ_ENABLE,
    SPI_RX_IRQ_DISABLE,
} spiIrqOperation_t;

struct myr2SpiRegs
{
    volatile u32 CTRLR0;
    volatile u32 CTRLR1;
    volatile u32 SSIENR;
    volatile u32 MWCR;
    volatile u32 SER;
    volatile u32 BAUDR;
    volatile u32 TXFTRL;
    volatile u32 RXFTLR;
    volatile u32 TXFLR;
    volatile u32 RXFLR;
    volatile u32 SR;
    volatile u32 IMR;
    volatile u32 ISR;
    volatile u32 RISR;
    volatile u32 TXOICR;
    volatile u32 RXOICR;
    volatile u32 RXUICR;
    volatile u32 MSTICR;
    volatile u32 ICR;
    volatile u32 DMACR;
    volatile u32 DMATDLR;
    volatile u32 DMARDLR;
    volatile u32 IDR;
    volatile u32 SSI_COMP_VERSION;
    volatile u16 DR[72];
    volatile u32 RX_SAMPLE_DLY;
};

struct spiTask_t
{
    void *originPtr;
    void *crtPtr;
    u32 totalSize;
    u32 remainingSize;
};

struct spiHandler_t;

typedef void (*HglSpiRxCallback)(struct spiHandler_t *handler);
typedef void (*HglSpiTxCallback)(struct spiHandler_t *handler);

typedef struct spiHandler_t {
    spiSlaveBlock_t device;  ///< SPI device number: 0 to 2
    volatile struct myr2SpiRegs *regs;

    u8 bytesPerWord; ///< word size in bytes
    dmaUsed_t useDma; ///< specifies whether DMA is used for transfers or not

    struct spiTask_t rx;
    struct spiTask_t tx;

    HglSpiRxCallback rxCb;
    HglSpiTxCallback txCb;

} spiHandler_t;

int HGLSpiInit(spiHandler_t* handler,
               spiType_t st,
               wordSizeBytes_t bytesPerWord,
               spiSlaveBlock_t device,
               u32 cpol,
               u32 cpha,
               u32 baudRate);

void HGLSpiIrqOperation(spiHandler_t * handler, spiIrqOperation_t operation);

int HGLSpiUpdateConfiguration(spiHandler_t* handler,
                              u32 bytesPerWord,
                              u32 cpol,
                              u32 cpha,
                              u32 baudRate);

void HGLSpiUpdateTrfMode(spiHandler_t* handler,
                         spiModeRxTx_t mode);

int HGLSpiSetSlave(spiHandler_t* handler, u32 slave);

int HglSpiRxTx(spiHandler_t * handler, void *rxBuff, const void *txBuff, u32 size);
int HglSpiRx(spiHandler_t * handler, void *rxBuff, u32 size);
int HglSpiTx(spiHandler_t * handler, const void *txBuff, u32 size);

int HglSpiRxHandler(spiHandler_t * handler, u32 * remainingSize);
int HglSpiTxHandler(spiHandler_t * handler, u32 * remainingSize);

void HglSpiDmaEnableRx(spiHandler_t *handler, u32 dmaThreshold);
void HglSpiDmaEnableTx(spiHandler_t *handler, u32 dmaThreshold);

void HglSpiDmaDisableRx(spiHandler_t *handler);
void HglSpiDmaDisableTx(spiHandler_t *handler);

int HglSpiCheckError(spiHandler_t *handler);

int HglSpiGetBusy(spiHandler_t *handler);

void HglSpiFillSpiTask(struct spiTask_t *st, void *origin, u32 size);
void HglSpiRequestData(struct spiTask_t *st, void **data, u32 *size);
void HglSpiRequestDataAligned(struct spiTask_t *st,
                              void **data, u32 *size, u32 align);
void HglSpiClearSpiTask(struct spiTask_t *st);

#ifdef __cplusplus
}
#endif

#endif

///@}
