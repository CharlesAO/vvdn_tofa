///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     I2C Slave Functions.
///

/// System Includes
/// ----------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#ifdef MYRIAD1
#include "isaac_registers.h"
#else
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#endif
#include "mv_types.h"
#include "DrvI2c.h"
#include "DrvIcb.h"
#include "DrvGpio.h"
#include "DrvCpr.h"

/// Application Includes
/// ----------------------------------------------------------------------------
#include "I2CSlaveApi.h"

/// 2:  Source Specific #defines and types  (typedef,enum,struct)
/// ----------------------------------------------------------------------------
#ifndef I2C_SLAVE_DATA_SECTION
#define I2C_SLAVE_DATA_SECTION(x) x
#endif

#ifndef I2C_SLAVE_CODE_SECTION
#define I2C_SLAVE_CODE_SECTION(x) x
#endif

#ifdef I2CSLAVEDBGUART
#define i2cSlaveLog(x) (*(volatile u32*) 0x80000000 =(x))
#elif I2CSLAVEARRAYDEBUG

#define I2CLOGSIZE 128
u8 i2cIrqLog[I2CLOGSIZE];
u32 i2cIrqLog_i;

void i2cSlaveLog(u8 x)
{
    i2cIrqLog[i2cIrqLog_i++] = x;
    if(i2cIrqLog_i == I2CLOGSIZE)
    i2cIrqLog_i = 0;
}
#else
#define i2cSlaveLog(x) ((void)(x))
#endif

#define I2C_DEVICE_NO 3

typedef enum {
    LEONRT,
    LEONOS
} current_leon_t;

/// 3: Global Data (Only if absolutely necessary)
/// ----------------------------------------------------------------------------

/// 4: Static Local Data
/// ----------------------------------------------------------------------------
static i2cStat_t i2cSlaveStatus[I2C_DEVICE_NO];
static i2cSlaveHandle_t *i2cSlaveHandl[I2C_DEVICE_NO];

/// 5: Static Function Prototypes
/// ----------------------------------------------------------------------------
void i2cSlaveHandler(u32 source);
static inline current_leon_t getCurrentLeon(void);

/// 6: Functions Implementation
/// ----------------------------------------------------------------------------

static inline current_leon_t getCurrentLeon(void)
{
#ifdef MYRIAD2
    if (swcLeonGetProcessorConfig() & LEON_PROCESSOR_INDEX_MASK)
        return LEONRT;
    else
        return LEONOS;
#else
    return LEONOS;
#endif
}

void I2C_SLAVE_CODE_SECTION(I2CSlaveSetupCallbacks)(
                i2cSlaveHandle_t *hndl,
                i2cReadAction* cbReadAction,
                i2cWriteAction* cbWriteAction)
{
    hndl->cbReadAction = cbReadAction;
    hndl->cbWriteAction = cbWriteAction;
}

/// Init the I2C Slave
s32 I2C_SLAVE_CODE_SECTION(I2CSlaveInit)(i2cSlaveHandle_t *hndl,
                                        i2cSlaveAddrCfg_t *config)
{
    u32 blockNumber, speed;

    switch (hndl->i2cConfig.device)
    {
    case IIC1_BASE_ADR:
        blockNumber = I2C_BLOCK_1;
        break;
    case IIC2_BASE_ADR:
    	blockNumber = I2C_BLOCK_2;
    	break;
    case IIC3_BASE_ADR:
    	blockNumber = I2C_BLOCK_3;
        break;
    default:
        return -1;
    }

    i2cSlaveHandl[blockNumber] = hndl;

    /// Init variables
    i2cSlaveStatus[blockNumber].regSpaceMemory = (u8*) (config->regMemAddr);
    i2cSlaveStatus[blockNumber].regMemByteSize = config->regMemByteSize;
    i2cSlaveStatus[blockNumber].addrSize = config->dataByteNo;
    i2cSlaveStatus[blockNumber].dataSize = config->dataByteNo;
    i2cSlaveStatus[blockNumber].receivedBytes = 0;
    i2cSlaveStatus[blockNumber].i2cState = I2CSLAVE_STOPPED;

    if (hndl->i2cConfig.speedKhz <= I2C_SPEED_SLOW_KHZ)
    {
        speed = 1;
    }
    if ((hndl->i2cConfig.speedKhz > I2C_SPEED_SLOW_KHZ) &&
        (hndl->i2cConfig.speedKhz <= I2C_SPEED_FAST_KHZ))
    {
        speed = 2;
    }
    if ((hndl->i2cConfig.speedKhz > I2C_SPEED_FAST_KHZ) &&
        (hndl->i2cConfig.speedKhz <= I2C_SPEED_HIGH_KHZ))
    {
        speed = 3;
    }

    /// FIFO threshold level is set to the byte number of the transaction
    /// This gives us a IRQ on every address or register write, which must be handled
    /// speed - 1 max 100k, 2 max 400k, 3 max 3.4M
    DrvIicSlaveInit(hndl->i2cConfig.device, config->slvAddr,
                    config->dataByteNo - 1, speed);

    /// Set the IRQ source
    switch(getCurrentLeon())
    {
        default:
        case LEONOS:
            hndl->irqSource = IRQ_IIC1 + blockNumber;
            break;
        /// TODO: please remove hardcoding from here
        case LEONRT: 
            hndl->irqSource = IRQ_DYNAMIC_9;
            break;
    };
    
    i2cSlaveStatus[blockNumber].i2cHndl = hndl;

    /// Clear all interrupts
    u32 irqStat = GET_REG_WORD_VAL(
                    i2cSlaveHandl[blockNumber]->i2cConfig.device + IIC_CLR_INTR);
    UNUSED(irqStat);

    /// Configure the interrupt mask
    /// We must set interrupt flags for:  RxFIFOFull, RxReadRequest, StopDetect
    SET_REG_WORD(i2cSlaveHandl[blockNumber]->i2cConfig.device + IIC_INTR_MASK,
                 IIC_IRQ_RX_FULL | IIC_IRQ_RD_REQ | IIC_IRQ_STOP_DET | IIC_IRQ_RX_DONE | IIC_IRQ_TX_ABRT);

    /// Set the interrupt properties in the entire system
    /// A "level" interrupt is essential to avoid loosing new interrupts
    /// that happen during the interrupt handler operation
    /// In this case we use I2C peripheral IRQ latch and avoid manually clearing the IRQ
    DrvIcbSetupIrq(hndl->irqSource, I2C_SLAVE_INT_LEVEL,
                   POS_LEVEL_INT, i2cSlaveHandler);

    return 0;
}

void i2cSlaveProcessWrReq(u32 i2cSel)
{
    u16 address = 0;
    u32 IIC_DATA_reg = 0;

    i2cSlaveLog('W');
    i2cSlaveLog('0' + GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_RXFLR));

    while (GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_RXFLR) > 0)
    {
        switch (i2cSlaveStatus[i2cSel].i2cState)
        {
        case I2CSLAVE_STOPPED:
        case I2CSLAVE_ADDRESS:
        case I2CSLAVE_READ:
            /// First comes the address:
	    IIC_DATA_reg = GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_DATA_CMD) & 0xFF;

            address = (address << 8) | IIC_DATA_reg;	    

            i2cSlaveStatus[i2cSel].receivedBytes++;
            if (i2cSlaveStatus[i2cSel].receivedBytes == i2cSlaveStatus[i2cSel].addrSize)
            {
                i2cSlaveStatus[i2cSel].receivedBytes = 0;
                i2cSlaveStatus[i2cSel].i2cState = I2CSLAVE_WRITE;
                i2cSlaveStatus[i2cSel].callbackAddressReg =
                                i2cSlaveStatus[i2cSel].addrReg = address;
            }
            break;

        case I2CSLAVE_WRITE:
        {
            u32 regMemIndex = i2cSlaveStatus[i2cSel].addrReg
                              * i2cSlaveStatus[i2cSel].dataSize
                              + i2cSlaveStatus[i2cSel].receivedBytes++;	    

            i2cSlaveLog('0' + regMemIndex);

            IIC_DATA_reg = GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_DATA_CMD); 

            u8 aux = (u8) IIC_DATA_reg; 

            if (regMemIndex < i2cSlaveStatus[i2cSel].regMemByteSize)
            {
                i2cSlaveStatus[i2cSel].regSpaceMemory[regMemIndex] = aux;
            }
        }
        break;
        default:
            return;
        }
    }
}

void i2cSlaveProcessRdReq(u32 i2cSel)
{
    u32 i, bytesToSend;
    i2cSlaveLog('R');

    i2cSlaveStatus[i2cSel].i2cState = I2CSLAVE_READ;

    /// standard transaction size is data size
    bytesToSend = i2cSlaveStatus[i2cSel].dataSize;

    /// Send data to Master
    if (i2cSlaveStatus[i2cSel].i2cHndl->cbReadAction)
    {
        u32 regMemIndex = i2cSlaveStatus[i2cSel].callbackAddressReg
                        * i2cSlaveStatus[i2cSel].dataSize;
        u32 reg = i2cSlaveStatus[i2cSel].callbackAddressReg;	

        if (regMemIndex < i2cSlaveStatus[i2cSel].regMemByteSize)
        {
            void *addrPtr = &i2cSlaveStatus[i2cSel].regSpaceMemory[regMemIndex];

            /// Call the read callback that can:
            ///    - write the register map of the specific register
            ///    - override register size for bulk transfers
            i2cSlaveStatus[i2cSel].i2cHndl->cbReadAction(reg, addrPtr, &bytesToSend);
        }
    }

    for (i = 0; i < bytesToSend; i++)
    {
        u32 regMemIndex = i2cSlaveStatus[i2cSel].addrReg * i2cSlaveStatus[i2cSel].dataSize + i;
	
        i2cSlaveLog('0' + regMemIndex);

        if (regMemIndex < i2cSlaveStatus[i2cSel].regMemByteSize)
        {
            SET_REG_WORD(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_DATA_CMD,
                         (i2cSlaveStatus[i2cSel].regSpaceMemory[regMemIndex] & 0xff));
        }
        else
        {
            SET_REG_WORD(i2cSlaveHandl[i2cSel]->i2cConfig.device+IIC_DATA_CMD,
                         0xFF);
        }
    }

    /// Prepare for next possible transaction
    i2cSlaveStatus[i2cSel].addrReg++;
}

void i2cSlaveProcessStopBit(u32 i2cSel)
{
    i2cSlaveLog('P');
    i2cSlaveLog('\n'); /// Flush the UART buffer

    if ((i2cSlaveStatus[i2cSel].i2cState == I2CSLAVE_WRITE) && /// we had a write command
        (i2cSlaveStatus[i2cSel].i2cHndl->cbWriteAction != NULL)) /// we have a write callback set
    {
        u32 regMemIndex = i2cSlaveStatus[i2cSel].callbackAddressReg
                        * i2cSlaveStatus[i2cSel].dataSize;

        if (regMemIndex < i2cSlaveStatus[i2cSel].regMemByteSize)
        {
            u32 reg = i2cSlaveStatus[i2cSel].callbackAddressReg;
            u32 val;
            switch (i2cSlaveStatus[i2cSel].dataSize)
            {
            default:
                case 1:
                val = *((u8*) &i2cSlaveStatus[i2cSel].regSpaceMemory[regMemIndex]);
                break;
            case 2:
                val = *((u16*) &i2cSlaveStatus[i2cSel].regSpaceMemory[regMemIndex]);
                break;
            case 4:
                val = *((u32*) &i2cSlaveStatus[i2cSel].regSpaceMemory[regMemIndex]);
                break;
            }
            void * addrPtr = &i2cSlaveStatus[i2cSel].regSpaceMemory[regMemIndex];
            u32 byteCount = i2cSlaveStatus[i2cSel].receivedBytes;

            i2cSlaveStatus[i2cSel].i2cHndl->cbWriteAction(reg, val, addrPtr, byteCount);
        }
    }

    i2cSlaveStatus[i2cSel].i2cState = I2CSLAVE_STOPPED;
    i2cSlaveStatus[i2cSel].receivedBytes = 0;
}

void i2cSlaveHandler(u32 source)
{
    u32 i2cSel;
    u32 icBit;
    enum
    {
        END, REPEAT,
    } repeatFlag;

    /// get i2c slave device handled
    switch(getCurrentLeon())
    {
        default:
        case LEONOS:
            i2cSel = source - IRQ_IIC1;
            break;
        /// TODO: please remove hardcoding
        case LEONRT:
            i2cSel = 0;
    }
    assert(i2cSel < I2C_DEVICE_NO);

    i2cSlaveLog('D');

    repeatFlag = REPEAT;
    while (repeatFlag == REPEAT)
    {
        repeatFlag = END;
        if (GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_INTR_STAT) & IIC_IRQ_RX_FULL)
        {
            i2cSlaveProcessWrReq(i2cSel);
            repeatFlag = REPEAT;
        }

        /// Check if a read request has been made
        if (GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_INTR_STAT) & IIC_IRQ_RD_REQ)
        {
            GET_REG_WORD(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_CLR_RD_REQ,
                         icBit); /// clear RD_REQ IRQ
            i2cSlaveProcessRdReq(i2cSel);
            repeatFlag = REPEAT;
        }

        if (GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_INTR_STAT) & IIC_IRQ_TX_ABRT)
        {
            GET_REG_WORD(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_CLR_TX_ABRT,
                         icBit);
            i2cSlaveLog('F');
            repeatFlag = REPEAT;
        }

        if ( GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_INTR_STAT) & IIC_IRQ_RX_DONE)
        {
            GET_REG_WORD(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_CLR_RX_DONE,
                         icBit);
            i2cSlaveLog('N');

            i2cSlaveStatus[i2cSel].i2cState = I2CSLAVE_STOPPED;
            i2cSlaveStatus[i2cSel].receivedBytes = 0;

            repeatFlag = REPEAT;
        }

        if (GET_REG_WORD_VAL(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_INTR_STAT) & IIC_IRQ_STOP_DET)
        {
            GET_REG_WORD(i2cSlaveHandl[i2cSel]->i2cConfig.device + IIC_CLR_STOP_DET,
                         icBit);
            i2cSlaveProcessStopBit(i2cSel);
            repeatFlag = REPEAT;
        }
    }
    i2cSlaveLog(' ');

    /// Clear interrupt from this source
    DrvIcbIrqClear(i2cSlaveStatus[i2cSel].i2cHndl->irqSource);
}
