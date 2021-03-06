///
/// @file DrvI2cMaster.c
/// @copyright All code copyright Movidius Srl 2012, Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @date   07-Feb-2012
/// @ingroup DrvI2cMaster
/// @{
///
/// This Module provides an general purpose API which can be used to drive I2C modules.
///
///  @todo {
///  Proper clock stretching support with error flagging in BB case
///  Detect loss of arbitration in BB case
///  GetByte NACK handling?
///  Fixup Doxygen API Documentation
///  More regression test cases
///  Async api??
///  Investigate the strange 0x09 seen on the bus when > 400Khz (HW mode)
///  Regression tests for low clock speeds
///  Verify consistency of bus timing vs requested for both BB and hardware
/// }


/******************************************************************************
 1: Included types first then APIs from other modules
******************************************************************************/
#include <mv_types.h>
#include "registersMyriad.h"
#include <stdlib.h>
#include <stdio.h>
#include <DrvRegUtils.h>
#include "DrvI2cMaster.h"
#include "DrvI2c.h"
#include "DrvGpio.h"
#include "DrvCpr.h"
#include "DrvTimer.h"


#ifndef MIN
#   define MIN(a,b) (a>b?b:a)
#endif

#ifndef MAX
#   define MAX(a,b) (a>b?a:b)
#endif

#define    PS_IN_ONE_MS (1000 * 1000 * 1000)
#define NS_IN_ONE_MS (1000 * 1000)

#define IIC_SDA_HOLD (0x7C)

#define IIC_CON_TX_EMPTY_CTRL       (1 << 8) 
#define IIC_CON_MASTER              ( (1 << 6) | 1)
#define IIC_CON_RESTART_ENABLE      ( (1 << 5)    )
#define IIC_CON_VALUE               (IIC_CON_MASTER | IIC_CON_RESTART_ENABLE )

#define RECOVERY_TRIES             (100     ) // Try 100 SCL clocks to recover SDA stuck low
#define NUM_CLOCKS_I2C_RESET       (16      ) // Try at least 16 clocks to recover SDA stuck low
#define DEFAULT_I2C_TIMEOUT_US     (100000  ) // Default to 100ms  
#define TX_FIFO_THRESHOLD_VALUE    (12      )
#define RX_FIFO_THRESHOLD_VALUE    (12      )

#define BITBASH_SW_OVERHEAD_CLOCKS (79      ) // Measured emperically, It isn't really an exact science 
// #define DRV_I2CM_DEBUG1  // Level 1: Debug Messages (Highest priority, e.g. Errors)
// #define DRV_I2CM_DEBUG2  // Level 2: Debug Messages (Mid priority)
// #define DRV_I2CM_DEBUG3  // Level 3: Debug Messages (Low priority, general debug)
// #define DRV_I2CM_DEBUG4  // Level 4: Debug Messages (Exceptional debug, may effect program flow signficantly)
// #define DRV_I2CM_DEBUG5  // Level 4: Debug Messages (Exceptional debug, may effect program flow signficantly)


#ifdef DRV_I2CM_DEBUG1
#define DPRINTF1(...) printf(__VA_ARGS__)
#else
#define DPRINTF1(...)
#endif

#ifdef DRV_I2CM_DEBUG2
#define DPRINTF2(...) printf(__VA_ARGS__)
#else
#define DPRINTF2(...)
#endif

#ifdef DRV_I2CM_DEBUG3
#define DPRINTF3(...) printf(__VA_ARGS__)
#else
#define DPRINTF3(...)
#endif

#ifdef DRV_I2CM_DEBUG4
#define DPRINTF4(...) printf(__VA_ARGS__)
#else
#define DPRINTF4(...)
#endif

#ifdef DRV_I2CM_DEBUG5
#define DPRINTF5(...) printf(__VA_ARGS__)
#else
#define DPRINTF5(...)
#endif

/******************************************************************************
 2:  Source File Specific #defines
******************************************************************************/

/******************************************************************************
 3:  Local variables
******************************************************************************/
static u8 simpleWriteProto[] = I2C_PROTO_WRITE_8BA;
static u8 simpleReadProto[]  = I2C_PROTO_READ_8BA;

typedef enum
{
    BITS_SET,
    BITS_CLEAR
} I2CMCompareType;

typedef enum
{
    WRITE_OP = 0,
    READ_OP  = 1
} I2CMOperationType;


const u32 IIC_FIFO_PROBLEM =  IIC_IRQ_TX_OVER | IIC_IRQ_RX_OVER | IIC_IRQ_RX_UNDER;

static int i2cmDeviceConfigure(I2CM_Device * dev,u32 speedKhz);
static int i2cmWaitForStatus(I2CM_Device * dev,u32 flags, I2CMCompareType compare);
static int i2cmSendSlaveAddress(I2CM_Device * dev, u32 slaveAddr, I2CMOperationType op);
static int i2cmSendByte(I2CM_Device * dev, u32 data, u32 flags);
static int i2cmGetByte(I2CM_Device * dev, u8 * buf, u32 flags,u32 * bytesStillToBePopped,u32 verify,u32 bytesLeftToRead);
static I2CM_StatusType i2cmCheckForErrors(I2CM_Device *dev);
static int i2cmWaitForComplete(I2CM_Device * dev);
// Bit Bash Support functions
static inline void waitTicks(u32 ticks);
static inline void setSclHi(I2CM_Device * dev);
static inline void setSdaHi(I2CM_Device * dev);
static inline void setSclLo(I2CM_Device * dev);
static inline void setSdaLo(I2CM_Device * dev);
static inline void setSdaValue(I2CM_Device * dev,u32 value);
static inline int getSdaVal(I2CM_Device * dev);
static inline int getSclVal(I2CM_Device * dev);
static int i2cmBbReadWriteBit(I2CM_Device * dev,u32 bit);
int i2cmBbWrite(I2CM_Device * dev, u32 byte); 
int i2cmBbRead(I2CM_Device * dev, u32 nack); 

/******************************************************************************
 4: Functions Implementation
******************************************************************************/
static inline void waitTicks(u32 ticks)
{
    u32 t0;
    u32 t1;
    u32 elapsed;

    t0 = GET_REG_WORD_VAL(TIM0_FREE_LOWER_RAW_ADR);
    do
    {
        t1 = GET_REG_WORD_VAL(TIM0_FREE_LOWER_RAW_ADR);
        if (t1 < t0)
            elapsed = 0xFFFFFFFF - t0 + t1;
        else
            elapsed = t1 - t0;
    } while ( elapsed < ticks);
}
static inline void setSclHi(I2CM_Device * dev)
{
    *(volatile u32 *) dev->sclModeRegAddr = D_GPIO_MODE_7 | D_GPIO_DIR_IN;
    return;
}

static inline void setSdaHi(I2CM_Device * dev)
{
    *(volatile u32 *) dev->sdaModeRegAddr = D_GPIO_MODE_7 | D_GPIO_DIR_IN;
    return;
}

static inline void setSclLo(I2CM_Device * dev)
{
    *(volatile u32 *) dev->sclModeRegAddr = D_GPIO_MODE_7;
    return;
}

static inline void setSdaLo(I2CM_Device * dev)
{
    *(volatile u32 *) dev->sdaModeRegAddr = D_GPIO_MODE_7;
    return;
}

static inline void setSdaValue(I2CM_Device * dev,u32 value)
{
    *(volatile u32 *) dev->sdaModeRegAddr = D_GPIO_MODE_7 | (value << D_GPIO_BIT_SHIFT_DIR_IN);
    return;
}

static inline int getSdaVal(I2CM_Device * dev)
{
    int value;

    value = *(volatile int *) dev->sdaRawRegAddr;
    value = (value >> dev->sdaRawOffset) & 0x1;

    return value;
}

static inline int getSclVal(I2CM_Device * dev)
{
    int value;

    value = *(volatile int *) dev->sclRawRegAddr;
    value = (value >> dev->sclRawOffset) & 0x1;

    return value;
}

static inline void i2cmBbStart(I2CM_Device * dev)
{
    // TODO: Review restart timing and also the length of delays required for it
    if (dev->stateProtoStart == STARTED) // Already started so we do a restart
    {
        setSdaHi(dev);
        waitTicks(dev->clksPerQuarterCycle); 

        // Wait for the clock to go high
        // Warning TODO: This needs a timeout ASAP!!
        // TODO: How do we error handle if this times out!
        setSclHi(dev);
        while (getSclVal(dev) == 0);
    }
    // TODO: Arbitration check for SDA low!!

    waitTicks(dev->clksPerQuarterCycle); 

    setSdaLo(dev);

    waitTicks(dev->clksPerHalfCycle); 

    setSclLo(dev);

    waitTicks(dev->clksPerQuarterCycle); 

    return;
}

static inline void i2cmBbStop(I2CM_Device * dev)
{
    setSdaLo(dev);
    waitTicks(dev->clksPerQuarterCycle);

    // Wait for the clock to go high
    // Warning TODO: This needs a timeout ASAP!!
    // TODO: How do we error handle if this times out!
    setSclHi(dev);
    while (getSclVal(dev) == 0);

    waitTicks(dev->clksPerHalfCycle);

    // TODO: Handle loss of arbitration here
    setSdaHi(dev);

    waitTicks(dev->clksPerQuarterCycle);
    return;
}

// TODO: This function still needs to have support added for error detection:
// A) Clock stretch timeout
// B) Loss of arbutration (write only)
static int i2cmBbReadWriteBit(I2CM_Device * dev,u32 bit) 
{
    u32 timeoutClockStretch = dev->clksPerI2cCycle << 8; // 256 Character lengths TODO: Review timeout 
                                                            
    setSdaValue(dev,bit); // When reading the parameter bit must be set to 1 so that we release the SDA pin

    waitTicks(dev->clksPerQuarterCycle); 

    // Wait for the clock to go high
    // Warning TODO: This needs a timeout ASAP!!
    // TODO: How do we error handle if this times out!
    setSclHi(dev);  
    while (getSclVal(dev) == 0)
        if (bit++ > timeoutClockStretch)
            break;

    waitTicks(dev->clksPerHalfCycle); 

    // Read the bit  [ Read case: We are interested in the value; Write Case: A 0 means arbitration lost ]
    bit = getSdaVal(dev); 
    
    setSclLo(dev);
    
    waitTicks(dev->clksPerQuarterCycle); 

    return bit;
}



int i2cmBbWrite(I2CM_Device * dev, u32 byte) 
{
  int i;
  u32 nack;

  for(i=0; i<8; i++ ) 
      {
      i2cmBbReadWriteBit(dev,(byte & 0x80) >> 7);
      byte <<= 1;
      }

  nack =  i2cmBbReadWriteBit(dev,1); 
  return nack;
}

int i2cmBbRead(I2CM_Device * dev, u32 nack) 
{
  int i;
  u32 byte = 0;

  for(i=0; i<8; i++ ) 
      {
      byte = (byte << 1) | i2cmBbReadWriteBit(dev,1);
      }

  i2cmBbReadWriteBit(dev,nack);
  return byte;
}

I2CM_StatusType DrvI2cMInitFromConfig(I2CM_Device *dev, const tyI2cConfig *config) {
    I2CM_StatusType retval;
    retval = DrvI2cMInit(dev, config->device, config->sclPin, config->sdaPin, config->speedKhz, config->addressSize);
    if (config->errorHandler)
        DrvI2cMSetErrorHandler(dev, config->errorHandler);
    return retval;
}



I2CM_StatusType DrvI2cMInit(I2CM_Device * dev,I2CMModuleOption module, u32 sclPin,u32 sdaPin,u32 speedKhz,I2CMAddrType addrType)
{
    I2CM_StatusType status = I2CM_STAT_OK;
    u32 sclPinMode = 0;
    u32 sdaPinMode = 0;
    u64 enableClocks = 0;
    int i;
    u32 sysFreqKhz=0;
    u32 sysClkPeriodPS = 0; // Clock period in picoseconds

    dev->sclPin   = sclPin;
    dev->sdaPin   = sdaPin;
    dev->addrSize = addrType;
    dev->timeoutUs = 5000;
    dev->maxAckPoll = 20; // Try 20 times for an ACK before giving up
    dev->speedKhz  = speedKhz; // We keep a backup of the configured speed in case someone wants to reconfigure us

    enableClocks = DEV_CSS_LOS_ICB  |
                   DEV_CSS_GPIO |
                   DEV_CSS_LOS_TIM  |
                   DEV_CSS_APB1_CTRL |
                   DEV_CSS_APB3_CTRL | 
                   DEV_CSS_SAHB_CTRL | 
                   DEV_CSS_APB4_CTRL |
                   DEV_CSS_LAHB_CTRL;

    dev->errorFunc = NULL;

    dev->stateProtoStart = STOPPED;
    dev->stateError = 0;
 
    switch (module)
    {
    case IIC1_DEVICE:
        DPRINTF3("\nIIC0 Configure\n");
        DrvCprSysDeviceAction(CSS_DOMAIN,PULSE_RESET, DEV_CSS_I2C0);

        dev->i2cDeviceAddr = IIC1_BASE_ADR;

        enableClocks |= DEV_CSS_I2C0;
 
        // Setup SCL PIN
        
        if (dev->sclPin == 20) sclPinMode = D_GPIO_MODE_4;
        else if (dev->sclPin == 60) sclPinMode = D_GPIO_MODE_0;
             else if (dev->sclPin == 10) sclPinMode = D_GPIO_MODE_1; 
                  else 
                  { 
                        status = I2CM_STAT_INVALID_PIN;
                        goto i2cm_init_error;
                  }

        // Setup SDA PIN
        if (dev->sdaPin == 21) sdaPinMode = D_GPIO_MODE_4;
        else if (dev->sdaPin == 61) sdaPinMode = D_GPIO_MODE_0;
             else if (dev->sdaPin == 11) sdaPinMode = D_GPIO_MODE_1;        
                  else
                  {
                        status = I2CM_STAT_INVALID_PIN;
                        goto i2cm_init_error;
                  }
 
        break;    
    case IIC2_DEVICE:
        DPRINTF3("\nIIC1 Configure\n");
        DrvCprSysDeviceAction(CSS_DOMAIN,PULSE_RESET, DEV_CSS_I2C1);

        dev->i2cDeviceAddr = IIC2_BASE_ADR;
        enableClocks |= DEV_CSS_I2C1;
        // Setup SCL PIN
        if (dev->sclPin == 12) sclPinMode = D_GPIO_MODE_2;
        else if (dev->sclPin == 49) sclPinMode = D_GPIO_MODE_4;
             else if (dev->sclPin == 62) sclPinMode = D_GPIO_MODE_0; 
                  else if (dev->sclPin == 77) sclPinMode = D_GPIO_MODE_4;              
                       else 
                       { 
                              status = I2CM_STAT_INVALID_PIN;
                              goto i2cm_init_error;
                       }

        // Setup SDA PIN
        if (dev->sdaPin == 13) sdaPinMode = D_GPIO_MODE_2;
        else if (dev->sdaPin == 50) sdaPinMode = D_GPIO_MODE_4;
             else if ( (dev->sdaPin == 63) || (dev->sdaPin == 69) )sdaPinMode = D_GPIO_MODE_0;
                  else if (dev->sdaPin == 78) sdaPinMode = D_GPIO_MODE_4;        
                       else
                       {
                             status = I2CM_STAT_INVALID_PIN; 
                             goto i2cm_init_error;
                       }
        
        break;
    case IIC3_DEVICE:
        DPRINTF3("\nIIC2 Configure\n");
        DrvCprSysDeviceAction(CSS_DOMAIN,PULSE_RESET,DEV_CSS_I2C2);

        dev->i2cDeviceAddr = IIC3_BASE_ADR;
        enableClocks |= DEV_CSS_I2C2;

        // Setup SCL PIN        
        if (dev->sclPin == 79) sclPinMode = D_GPIO_MODE_2;
            else if (dev->sclPin == 47) sclPinMode = D_GPIO_MODE_4;
                  else 
                  { 
                         status = I2CM_STAT_INVALID_PIN;
                         goto i2cm_init_error;
                  }


        // Setup SDA PIN
        if (dev->sdaPin == 80) sdaPinMode = D_GPIO_MODE_2;
        else if (dev->sdaPin == 48) sdaPinMode = D_GPIO_MODE_4;
             else
             {
                   status = I2CM_STAT_INVALID_PIN; 
                   goto i2cm_init_error;
             }
        
       
        break;
    case GPIO_BITBASH:
        DPRINTF3("\nBITBASH Configure (scl:%d sda:%d)\n",dev->sclPin,dev->sdaPin);
        dev->i2cDeviceAddr = GPIO_BASE_ADR;
        // Set GPIOS to High-Z Bit bash will pull them low as needed
        sdaPinMode = D_GPIO_DIR_IN | D_GPIO_MODE_7;
        sclPinMode = D_GPIO_DIR_IN | D_GPIO_MODE_7;
        DrvGpioSetPinLo(dev->sclPin); // Default the pins to driven low
        DrvGpioSetPinLo(dev->sdaPin); // And use change of direction to simulate Open Drain pin
        break;
    default:
        DPRINTF1("\nDefault\n");
        status = I2CM_STAT_INVALID_MODULE;
        goto i2cm_init_error;
    }
//    DRV_CPR_enableClocks(enableClocks);
 
    DrvCprSysDeviceAction(CSS_DOMAIN,ENABLE_CLKS,enableClocks);   // commented for testing in VCS
 
    DrvGpioSetMode(dev->sclPin, sclPinMode );
    DrvGpioSetMode(dev->sdaPin, sdaPinMode );
 
    // Allow 1 ms for the pins to float high
    // TODO: Isn't this a bit too long?
    //SleepMs(1);  // vcs
 
    if (DrvGpioGetRaw(dev->sdaPin) == 0)
    {
     
      DPRINTF2("\nSDA_PIN %d stuck low, attempting recovery\n",dev->sdaPin);
      DrvGpioSetMode(dev->sclPin, D_GPIO_MODE_7 );  // Drive SCL pin direct
      for (i=0;i<RECOVERY_TRIES;i++)
      {
        DrvGpioSetPinHi(dev->sclPin);
        //SleepMs(1);  //vcs
        DrvGpioSetPinLo(dev->sclPin);
        if ((DrvGpioGetRaw(dev->sdaPin) != 0) && (i > NUM_CLOCKS_I2C_RESET))
          break;
      }
      if (i == RECOVERY_TRIES)
      {
        DPRINTF1("\nRecovery Failed: SDA_PIN %d still stuck low (%d)\n",dev->sdaPin,i);
        status = I2CM_STAT_SDA_STUCK_LOW;
        goto i2cm_init_error;
      }
      else
      {
        DPRINTF2("\nRecovery Success: SDA_PIN %d is now High (%d)\n",dev->sdaPin,i);
        DrvGpioSetMode(dev->sclPin, sclPinMode );
        DrvGpioSetMode(dev->sdaPin, sdaPinMode );
      }
    }
 
    // First get system frequency
    sysFreqKhz = DrvCprGetClockFreqKhz(SYS_CLK,NULL);
    sysClkPeriodPS =  PS_IN_ONE_MS / sysFreqKhz;
    dev->clksPerI2cCycle = PS_IN_ONE_MS / (speedKhz * sysClkPeriodPS);
 
    if (dev->i2cDeviceAddr != GPIO_BASE_ADR)
        i2cmDeviceConfigure(dev,speedKhz);
    else
    {
        u32 regNumber;
         
        dev->clksPerI2cCycle = MIN(dev->clksPerI2cCycle, dev->clksPerI2cCycle - BITBASH_SW_OVERHEAD_CLOCKS);
        dev->clksPerHalfCycle    = dev->clksPerI2cCycle >> 1;
        dev->clksPerQuarterCycle = dev->clksPerI2cCycle >> 2;

        regNumber = dev->sclPin / 32;
        dev->sclRawRegAddr = GPIO_DATA_IN_RAW0_ADR + (4 * regNumber);
        dev->sclRawOffset  = dev->sclPin - (regNumber * 32);

        regNumber = dev->sdaPin / 32;
        dev->sdaRawRegAddr = GPIO_DATA_IN_RAW0_ADR + (4 * regNumber);
        dev->sdaRawOffset  = dev->sdaPin - (regNumber * 32);

        // Special speed optimisation hack to allow for fastest possible BitBash speed
        // We cache the address of the GPIO Mode register which will be used to change pin direction.
        // This way the function does not have to calculate it on the fly.
        dev->sclModeRegAddr =  GPIO_MODE0_ADR + (4 * dev->sclPin);
        dev->sdaModeRegAddr =  GPIO_MODE0_ADR + (4 * dev->sdaPin);
    }

     
   // DPRINTF1("\nfreq %d Khz ; sysClkPerPs %d ; clksPerI2cCycle %d ; 1/2: %d ; 1/4: %d\n",sysFreqKhz,sysClkPeriodPS,dev->clksPerI2cCycle,dev->clksPerHalfCycle,dev->clksPerQuarterCycle);

    // If we get to here we mark the device as initilaised
    dev->initialisedMagic = INITIALISED_MAGIC_VALUE;
 
i2cm_init_error:
    return status;
}

I2CM_StatusType DrvI2cMSetErrorHandler(I2CM_Device * dev,I2CErrorHandler handler)
{
    if (dev->initialisedMagic != INITIALISED_MAGIC_VALUE)
        return I2CM_STAT_NOT_INIT; // All we can do is return an error code as we can't call common error handler. Just hope the caller checks his errors

    dev->errorFunc = handler;
    return I2CM_STAT_OK;
}

void DrvI2cMWriteByte(I2CM_Device * dev,u32 slaveAddr,u32 regAddr, u8 value)
{
    (void)DrvI2cMTransaction(dev,slaveAddr, regAddr ,simpleWriteProto, &value , 1);
    return;
}

u8   DrvI2cMReadByte(I2CM_Device * dev,u32 slaveAddr,int regAddr)
{
    int ret;
    u8 value;
    ret=DrvI2cMTransaction(dev,slaveAddr, regAddr ,simpleReadProto, &value , 1);
    if (ret != 0)
    {
        //printf("\nRet: %d",ret);
    }
    return value;
}

void DrvI2cMGetCurrentConfig(I2CM_Device * dev,tyI2cConfig * currentConfig)
{
    currentConfig->device       = dev->i2cDeviceAddr;
    currentConfig->speedKhz     = dev->speedKhz;
    currentConfig->addressSize  = dev->addrSize;
    currentConfig->errorHandler = dev->errorFunc;
    currentConfig->sclPin       = dev->sclPin;
    currentConfig->sdaPin       = dev->sdaPin;
    return;
}

I2CM_StatusType DrvI2cMTransaction(I2CM_Device * dev, u32 slaveAddr,u32 regAddr,u8 * proto, u8 * dataBuffer, u32 dataBufferSize)
{
    u32 pindex=0;
    u32 bytesDone=0;
    u32 retVal=I2CM_STAT_OK;
    u32 bytesToBePopped=0;
    u32 verifyFlag = 0;
    u8  byte;

    DPRINTF2("\nIIC()-> slaveAddr %02X, regAddr %04X, size: %08X",slaveAddr,regAddr,dataBufferSize);
    DPRINTF3("\nIIC()-> dev: %08X, proto %08X, buffer: %08X",(u32)dev,(u32)proto,(u32)dataBuffer);
    
    u32 loops =0;

    if (dev->initialisedMagic != INITIALISED_MAGIC_VALUE)
        return I2CM_STAT_NOT_INIT; // All we can do is return an error code as we can't call common error handler. Just hope the caller checks his errors
    
    do
    {
        loops++;
        DPRINTF4("\nP:%02X",proto[pindex]);
        switch (proto[pindex])
        {
        case S_ADDR_WR:
            retVal = i2cmSendSlaveAddress(dev,slaveAddr, WRITE_OP);
                
            if (retVal != I2CM_STAT_OK)
                goto  error_handler;
            break;
        case S_ADDR_RD:
            
            i2cmSendSlaveAddress(dev,slaveAddr, READ_OP);
            break;
        case R_ADDR_HH:
            
            retVal = i2cmSendByte(dev,(regAddr >> 24) & 0xFF,0);
            if (retVal != I2CM_STAT_OK)
                goto  error_handler;
            break;
        case R_ADDR_HL:
            
            retVal = i2cmSendByte(dev,(regAddr >> 16) & 0xFF,0);
            if (retVal != I2CM_STAT_OK)
                goto  error_handler;
            break;
        case R_ADDR_H:
            
            retVal = i2cmSendByte(dev,(regAddr >> 8) & 0xFF,0);
            if (retVal != I2CM_STAT_OK)
                goto  error_handler;
            break;
        case R_ADDR_L:
            
            retVal = i2cmSendByte(dev,regAddr & 0xFF,0);
            if (retVal != I2CM_STAT_OK)
                goto  error_handler;
            break;
        case DATAW:
            
            if (dataBufferSize - bytesDone)
            {
                if ((dataBufferSize - bytesDone) == 1) // send stop for the last byte 
                     retVal = i2cmSendByte(dev,dataBuffer[bytesDone],IIC_DATA_CMD_STOP);                
                else
                     retVal = i2cmSendByte(dev,dataBuffer[bytesDone],0);
                     
                if (retVal != I2CM_STAT_OK)
                    goto  error_handler;
                bytesDone++;
            }
            else
            {
                retVal = I2CM_STAT_WRITE_UNDERFLOW;
                goto error_handler;
            }
            break;
        case DATAV:
            
            verifyFlag = 1;
        case DATAR:
            
            if (dataBufferSize - bytesDone)
            {
                
                if ((dataBufferSize - bytesDone) == 1) // send stop after the last byte
                           retVal = i2cmGetByte(dev,&dataBuffer[bytesDone-bytesToBePopped],IIC_DATA_CMD_STOP,&bytesToBePopped,verifyFlag,(dataBufferSize - bytesDone) );
                else           
                           retVal = i2cmGetByte(dev,&dataBuffer[bytesDone-bytesToBePopped],0,&bytesToBePopped,verifyFlag,(dataBufferSize - bytesDone) );                
                           
                
                if (retVal != I2CM_STAT_OK)
                    goto  error_handler;
                bytesDone++;
            }
            else
            {
                retVal = I2CM_STAT_READ_UNDERFLOW;
                goto error_handler;
            }
            break;
        case LOOP_MINUS_1:
            
            pindex -=2; // We have to subtract 2 as this run around the loop cost us 1
        default:
            break;
        }
        pindex++;
        //printf("\n %d %d %d %d %d",pindex,bytesDone, out_bytes, bytesDone, in_bytes);
    } while (bytesDone < dataBufferSize );

    
    if (dev->i2cDeviceAddr == GPIO_BITBASH)
    {
        i2cmBbStop(dev);
        dev->stateProtoStart = STOPPED;
    }
    else
    {
        
        // Wait until all the bytes stored in the RX fifo have been recovered
        while (bytesToBePopped)
        {
            retVal = i2cmWaitForStatus(dev,IIC_STATUS_RX_FIFO_NOT_EMPTY,BITS_SET);
            if (retVal !=  I2CM_STAT_OK)
            {
                DPRINTF1("\nTIMEOUT 7\n");
                goto error_handler;
            }
            byte = (u8)GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_DATA_CMD);
            if (verifyFlag)
            {
                if (byte != dataBuffer[bytesDone-bytesToBePopped])
                    return I2CM_STAT_VERIFY_FAIL;
            }
            else
                dataBuffer[bytesDone-bytesToBePopped] = byte;
            bytesToBePopped--;
        }
            

        // We must wait until transmission has been completed before we exit
        // as otherwise we cannot tell if there has been any errors
        retVal = i2cmWaitForComplete(dev);
        if (retVal != I2CM_STAT_OK)
            goto  error_handler;
    
        retVal = i2cmCheckForErrors(dev);
        if (retVal != I2CM_STAT_OK)
            goto  error_handler;
        else
            return I2CM_STAT_OK;
    }
    
    return I2CM_STAT_OK;

error_handler:
    
    if (dev->i2cDeviceAddr == GPIO_BITBASH)
    {
        i2cmBbStop(dev);
        dev->stateProtoStart = STOPPED;
    }
    if (dev->errorFunc != NULL) retVal = dev->errorFunc(retVal,slaveAddr,regAddr);
    return retVal;
}

static int i2cmWaitForComplete(I2CM_Device * dev)
{
    int retVal;
    
    retVal = i2cmWaitForStatus(dev,IIC_STATUS_TX_FIFO_EMPTY,BITS_SET);
        
    if ( retVal !=  I2CM_STAT_OK)
    {
        DPRINTF1("\nTIMEOUT 5\n");
        return retVal;
    }
    // Even after the FIFO is empty we need to wait for the I2C device to stop transmitting the last byte
    retVal = i2cmWaitForStatus(dev,IIC_STATUS_ACTIVITY,BITS_CLEAR);
        
    if (retVal!=  I2CM_STAT_OK)
    {
        DPRINTF1("\nTIMEOUT 6\n");
        return retVal;
    }

    return I2CM_STAT_OK;
}

static int i2cmDeviceConfigure(I2CM_Device * dev,u32 speedKhz)
{
    u32 offsetHighCount=0;
    u32 offsetLowCount=0;
    //u32 clksPerI2CHi=0;
    //u32 clksPerI2CLo=0;
    int scl_hcnt, scl_lcnt;

    // These are needed for calculating the number of 
    // system clocks needed by SDA Hold Time to meet the 
    // standard requirement that SDA Hold Time should be
    // at least 300 ns.
    u32   SDA_Hold_sysFreqKhz;
    float SDA_Hold_sysClkPeriodNS;
    float SDA_Hold_NoOfSysClcksNeeded;

    I2CM_StatusType status = I2CM_STAT_OK;


    if (speedKhz <= 100)
    {
        dev->speedMode = I2CM_SPEED_SLOW;
        offsetHighCount = IIC_SS_HCNT;
        offsetLowCount = IIC_SS_LCNT;
        scl_hcnt = getIicHCNTcc(I2C_SS_HCNT);
        scl_lcnt = getIicLCNTcc(I2C_SS_LCNT);
        if ((scl_hcnt <= 0) || (scl_lcnt <= 0))
        {
            status = I2CM_STAT_INVALID_SPEED;
            goto i2cm_dev_cfg_err;
        }
    }
    else if (speedKhz <= 400)
    {
        dev->speedMode = I2CM_SPEED_FAST;
        offsetHighCount = IIC_FS_HCNT;
        offsetLowCount = IIC_FS_LCNT;
        scl_hcnt = getIicHCNTcc(I2C_FS_HCNT);
        scl_lcnt = getIicLCNTcc(I2C_FS_LCNT);
        if ((scl_hcnt <= 0) || (scl_lcnt <= 0))
        {
            status = I2CM_STAT_INVALID_SPEED;
            goto i2cm_dev_cfg_err;
        }
    }
    else if (speedKhz <= 3400)
    {
        dev->speedMode = I2CM_SPEED_HIGH;
        offsetHighCount = IIC_HS_HCNT;
        offsetLowCount = IIC_HS_LCNT;
        scl_hcnt = getIicHCNTcc(I2C_HS_HCNT);
        scl_lcnt = getIicLCNTcc(I2C_HS_LCNT);
        if ((scl_hcnt <= 0) || (scl_lcnt <= 0))
        {
            status = I2CM_STAT_INVALID_SPEED;
            goto i2cm_dev_cfg_err;
        }
    }
    else
    {
        status = I2CM_STAT_INVALID_SPEED;
        goto i2cm_dev_cfg_err;
    }    
    
    // Calculate the number of I2C_In_clk(which is sysClock/2) needed
    // to set the IIC_SDA_HOLD register to, in order to meet the standard 
    // requiremet of a SDA_Hold_Time >= 300 ns.
    SDA_Hold_sysFreqKhz = DrvCprGetClockFreqKhz(SYS_CLK,NULL);
    SDA_Hold_sysClkPeriodNS = ((float)NS_IN_ONE_MS) / ((float)SDA_Hold_sysFreqKhz);  
    SDA_Hold_NoOfSysClcksNeeded = 300/SDA_Hold_sysClkPeriodNS/2;    

    SET_REG_WORD(dev->i2cDeviceAddr + IIC_ENABLE, 0); // Disable the module to allow config update

    SET_REG_WORD(dev->i2cDeviceAddr + IIC_SDA_HOLD, SDA_Hold_NoOfSysClcksNeeded);  

    // configure as master, do not use 10 bit adr, set speed, restart enable 
    SET_REG_WORD(dev->i2cDeviceAddr + IIC_CON, IIC_CON_VALUE | ((dev->speedMode & 0x3) << 1));

    // Calculate HiCount and Low count
    //clksPerI2CHi = (dev->clksPerI2cCycle * 30) / 100;
    //clksPerI2CLo = dev->clksPerI2cCycle - clksPerI2CHi;
    DPRINTF2("\nSCL Hi: %d, Lo: %d", scl_hcnt,scl_lcnt);

    SET_REG_WORD(dev->i2cDeviceAddr + offsetHighCount, scl_hcnt);
    SET_REG_WORD(dev->i2cDeviceAddr + offsetLowCount,  scl_lcnt);
    SET_REG_WORD(dev->i2cDeviceAddr + IIC_TX_TL, TX_FIFO_THRESHOLD_VALUE);
    SET_REG_WORD(dev->i2cDeviceAddr + IIC_RX_TL, RX_FIFO_THRESHOLD_VALUE);

    // Set default Target Address of 0
    SET_REG_WORD(dev->i2cDeviceAddr + IIC_TAR, 0x0 | (dev->addrSize==ADDR_10BIT?(1<<12):0));

    SET_REG_WORD(dev->i2cDeviceAddr + IIC_ENABLE, 1); // Re-Enable the module

i2cm_dev_cfg_err:
    return status;
}

static int i2cmWaitForStatus(I2CM_Device * dev,u32 flags, I2CMCompareType compare)
{
    u32 retVal;
    u32 timeoutReached;
    tyTimeStamp statusTimeout;
    
    DrvTimerInitTimeStamp(&statusTimeout,dev->timeoutUs);
    
    DrvTimerCheckTimeStamp(&statusTimeout, &timeoutReached);
    while(!timeoutReached)
    {
        
        retVal = i2cmCheckForErrors(dev);
        if (retVal != I2CM_STAT_OK)
            return retVal;
    
        if (compare == BITS_SET)
        {
            
            if (GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_STATUS) & flags) // Break when flag bit set
                 return I2CM_STAT_OK;
        }
        else
        {
            
            if (! (GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_STATUS) & flags)) // Break when flag bit clear
                 return I2CM_STAT_OK;
        }
        DrvTimerCheckTimeStamp(&statusTimeout, &timeoutReached);
    }

    return I2CM_STAT_TIMEOUT;
}

static int i2cmSendSlaveAddress(I2CM_Device * dev, u32 slaveAddr, I2CMOperationType readNotWrite)
{
    int retVal;
    enum
    {
        START   = 0,
        RESTART = 1
    };

    if (dev->i2cDeviceAddr == GPIO_BITBASH)
    {
        i2cmBbStart(dev);
        dev->stateProtoStart = STARTED;
        retVal =  i2cmBbWrite(dev,(slaveAddr << 1) | readNotWrite);
        if (retVal)
            dev->stateError=I2CM_STAT_ABRT_7B_ADDR_NOACK;
    }
    else
    {
        if (dev->addrSize == ADDR_10BIT)
            slaveAddr = (slaveAddr & 0x3FF) | (1<<12);
        else
            slaveAddr =  slaveAddr & 0x7F;

        // When performing a new write operation, we must wait for the FIFO to be empty
        // as otherwise the start condition will not be emitted by the I2C hardware
        if (readNotWrite == WRITE_OP)
        {
            // Step 1: Wait for FIFO empty
            retVal = i2cmWaitForStatus(dev,IIC_STATUS_TX_FIFO_EMPTY,BITS_SET);
            if ( retVal !=  I2CM_STAT_OK)
            {
                DPRINTF1("\nTIMEOUT 1\n");
                return retVal;
            }

            // Step 2: Wait for Master Idle
            retVal = i2cmWaitForStatus(dev,IIC_STATUS_ACTIVITY_MASTER,BITS_CLEAR);
            if (retVal !=  I2CM_STAT_OK)
            {
                DPRINTF1("\nTIMEOUT 2: %08X\n",GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_STATUS));
                return retVal;
            }

            // Step 3: Set Slave Target Address
            SET_REG_WORD(dev->i2cDeviceAddr + IIC_TAR, slaveAddr);
        }
        else
        {
            // Even for read operations we should set the device address
            SET_REG_WORD(dev->i2cDeviceAddr + IIC_TAR, slaveAddr);
        }
    }

    retVal = i2cmCheckForErrors(dev);
    return retVal;
}


static I2CM_StatusType i2cmCheckForErrors(I2CM_Device *dev)
{
    u32 irqStat;
    u32 txAbortSrc;
    volatile u32 tmp = 0; UNUSED(tmp);
    int i;
    I2CM_StatusType retVal;

    if (dev->i2cDeviceAddr == GPIO_BITBASH)
    {

        retVal = dev->stateError;
        dev->stateError = I2CM_STAT_OK; // Reset the error state
        return retVal;
    }
    else
    {
        
        irqStat = GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_RAW_INTR_STAT);
               
        if (irqStat & IIC_IRQ_TX_ABRT)
        {
            
            retVal = I2CM_STAT_ABRT_7B_ADDR_NOACK;
            txAbortSrc = GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_TX_ABRT_SOURCE); // Not currently used
            DPRINTF1("\ntxAbortSrc = %08X",txAbortSrc);
            // Find the first bit that is set and index the error table based on its binary offset.
            for (i=0;i<16;i++) 
            {
                if (txAbortSrc & (0x1 << i))
                    break;
            }
            retVal -= i; 
            tmp = GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_CLR_INTR);    // Important as otherwise TXFifo will remain clear
            tmp = GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_CLR_TX_ABRT); // For some reason this was also necessary
            return retVal;
        }

        // Check for errors
        if(irqStat & IIC_FIFO_PROBLEM) 
        {
            
            DPRINTF1("\nFIFO_ERR STAT = %08X",irqStat);
            return I2CM_STAT_FIFO_ERROR;
        }
    }

    return I2CM_STAT_OK;
}

static int i2cmSendByte(I2CM_Device * dev, u32 data, u32 flags)
{
    int retVal;

    // use flags to send stop or restart

    DPRINTF3("\nWD:%02X",data);

    if (dev->i2cDeviceAddr == GPIO_BITBASH)
    {
        retVal =   i2cmBbWrite(dev,data);
        if (retVal)
            dev->stateError=I2CM_STAT_ABRT_7B_ADDR_NOACK;
    }
    else
    {
        // Check that the FIFO isn't full
        retVal = i2cmWaitForStatus(dev,IIC_STATUS_TX_FIFO_NOT_FULL,BITS_SET);
        if (retVal !=  I2CM_STAT_OK)
        {
            DPRINTF1("\nTIMEOUT 3\n");
            return retVal;
        }

        SET_REG_WORD(dev->i2cDeviceAddr + IIC_DATA_CMD, (data & 0xFF) | (flags & (IIC_DATA_CMD_STOP | IIC_DATA_CMD_RESTART)));  // restrict flags to stop or restart conditions 
    }

    retVal = i2cmCheckForErrors(dev);

    return retVal;
}

static int i2cmGetByte(I2CM_Device * dev, u8 * buf, u32 flags,u32 * bytesStillToBePopped,u32 verify,u32 bytesLeftToRead)
{
    int retVal;
    u8 byte;
    u32 needToReadByte;

    //flags = flags; use flags to send stop command 

    if (dev->i2cDeviceAddr == GPIO_BITBASH)
    {
        needToReadByte = TRUE; // In bitbash mode, we have no FIFO so we must always read the byte
        retVal = i2cmBbRead(dev,(bytesLeftToRead == 1?1:0));
        if (retVal >= 0)
            byte = retVal;
        else
        {
            retVal = I2CM_STAT_ERR; // TODO: Fix generic Error code
            goto getByteError;
        }
        // TODO: Send NACK if necessary
    }
    else
    {
        // Check that the FIFO isn't full
        retVal = i2cmWaitForStatus(dev,IIC_STATUS_TX_FIFO_NOT_FULL,BITS_SET);
        if (retVal !=  I2CM_STAT_OK)
        {
            DPRINTF1("\nTIMEOUT 8\n");
            return retVal;
        }

        // Send the read command
        SET_REG_WORD(dev->i2cDeviceAddr + IIC_DATA_CMD, 0x100 | (flags & (IIC_DATA_CMD_STOP | IIC_DATA_CMD_RESTART)) );

        // If Fifo is full (i.e at its programmed threshold) we pop a byte off the FIFO to keep it from overflowing
        needToReadByte = GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_RAW_INTR_STAT) & IIC_IRQ_RX_FULL;
        if (needToReadByte) 
            byte = (u8)GET_REG_WORD_VAL(dev->i2cDeviceAddr + IIC_DATA_CMD);
        else
            *bytesStillToBePopped += 1;
    }

    if (needToReadByte) 
    {
        if (verify)
        {
            if (byte != *buf)
            {
                DPRINTF5("\nVERIFY_FAIL expected %08X got %08X @ bytesLeftToRead : %d",byte,*buf,bytesLeftToRead);
                return I2CM_STAT_VERIFY_FAIL;
            }
        }
        else
            *buf = byte;
    }

    //    printf("\nRD:%02X",*buf);
    retVal = i2cmCheckForErrors(dev);
getByteError:
    return retVal;
}

/// @}

