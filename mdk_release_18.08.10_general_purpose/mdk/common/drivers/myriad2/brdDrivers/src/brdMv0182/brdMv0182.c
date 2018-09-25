#include <brdMv0182.h>
#include <DrvGpio.h>
#include <assert.h>
#include "mv_types.h"
#include <stdio.h>
#include <stdlib.h>
#include "DrvCDCEL.h"
#include "DrvWm8325.h"
#include <DrvTimer.h>
#include "DrvRegUtilsDefines.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define NUM_I2C_DEVICES                   (3)
#define BMX_OR_TLV_SLAVE_ADDRESS         0x18
#define REGISTER_0X00                    0x00
#define BMX_ACCELEROMETER_ID             0xFA
#define ONE_BYTE_SIZE                    0x01

#define EEPROM_MEMORY_SIZE               0x23
#define EEPROM_I2C_SLAVE_ADDRESS         0x50
#define EEPROM_PART_1_SIZE                 32    //only 32 bytes can be read or write at a time from eeprom
#define EEPROM_PART_2_SIZE                  3
#define EEPROM_PART_1_OFFSET             0x00
#define EEPROM_PART_2_OFFSET             0x20

#define PCB_REVISION_OFFSET              0x0D
#define R4_EEPROM_VALUE                  0x04
#define R5_EEPROM_VALUE                  0x05

// Bit used to select 1.8 V in SDIO interface
#define SDIO_1_8V_INTERFACE_SELECT         23

// #define DRV_BOARD_DEBUG  //Debug Messages
#ifdef DRV_BOARD_DEBUG
#define DPRINTF_BOARD(...) printf(__VA_ARGS__)
#else
#define DPRINTF_BOARD(...)
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
// This function declaration is needed by the data initialised below
static u32 commonI2CErrorHandler(I2CM_StatusType error, u32 param1, u32 param2);
static u8 protocolReadSample[] = I2C_PROTO_READ_8BA;
static u8 protocolReadSample2[] = I2C_PROTO_READ_16BA;

static I2CM_Device i2cDevHandle[NUM_I2C_DEVICES];

static tyI2cConfig i2c0DefaultConfiguration =
    {
     .device                = IIC1_BASE_ADR,
     .sclPin                = MV0182_I2C0_SCL_PIN,
     .sdaPin                = MV0182_I2C0_SDA_PIN,
     .speedKhz              = MV0182_I2C0_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0182_I2C0_ADDR_SIZE_DEFAULT,
     .errorHandler          = &commonI2CErrorHandler,
    };

static tyI2cConfig i2c1DefaultConfiguration =
    {
     .device                = IIC2_BASE_ADR,
     .sclPin                = MV0182_I2C1_SCL_PIN,
     .sdaPin                = MV0182_I2C1_SDA_PIN,
     .speedKhz              = MV0182_I2C1_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0182_I2C1_ADDR_SIZE_DEFAULT,
     .errorHandler          = &commonI2CErrorHandler,
    };

static tyI2cConfig i2c2DefaultConfiguration =
    {
     .device                = IIC3_BASE_ADR,
     .sclPin                = MV0182_I2C2_SCL_PIN,
     .sdaPin                = MV0182_I2C2_SDA_PIN,
     .speedKhz              = MV0182_I2C2_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0182_I2C2_ADDR_SIZE_DEFAULT,
     .errorHandler          = &commonI2CErrorHandler,
    };

static tyI2cConfig i2c2RevisionDetectConfiguration =
    {
     .device                = IIC3_BASE_ADR,
     .sclPin                = MV0182_I2C2_SCL_PIN,
     .sdaPin                = MV0182_I2C2_SDA_PIN,
     .speedKhz              = MV0182_I2C2_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0182_I2C2_ADDR_SIZE_DEFAULT,
    };

static const drvGpioInitArrayType brdMv0182RevDetectConfig =
{
    // -----------------------------------------------------------------------
    // PCB Revision detect, set weak pullups on the necessary pins
    // -----------------------------------------------------------------------
    {9,  9  , ACTION_UPDATE_PAD          // Only updating the PAD configuration
            ,
            PIN_LEVEL_LOW              // Don't Care, not updated
            ,
            D_GPIO_MODE_0            // Don't Care, not updated
            ,
            D_GPIO_PAD_PULL_DOWN       | // Enable weak pullups so that we can detect revision
            D_GPIO_PAD_DRIVE_2mA     |
            D_GPIO_PAD_SLEW_SLOW     |
            D_GPIO_PAD_SCHMITT_OFF   |
            D_GPIO_PAD_RECEIVER_ON   |
            D_GPIO_PAD_LOCALCTRL_OFF |
            D_GPIO_PAD_LOCALDATA_LO  |
            D_GPIO_PAD_LOCAL_PIN_IN
     , NULL

    },
    // -----------------------------------------------------------------------
    // Finally we terminate the Array
    // -----------------------------------------------------------------------
    {0,0    , ACTION_TERMINATE_ARRAY      // Do nothing but simply termintate the Array
            ,
              PIN_LEVEL_LOW               // Won't actually be updated
            ,
              D_GPIO_MODE_0               // Won't actually be updated
            ,
              D_GPIO_PAD_DEFAULTS         // Won't actually be updated
     , NULL
    }
};
/******************************************************************************
 5: Functions Implementation
******************************************************************************/
static void cleanBuffer(u8 *src, u32 size)
{
    u32 index;
    for(index = 0; index < size; index++)
    {
        *(src+index)=0;
    }
}

static u8 verifyCheckSum(u8 *src,u8 size)
{
    u32 sum = 0;
    u32    index;

    for(index = 0;index < size;index++)
    {
        sum = sum + *(src+index);
    }

    sum = sum & 0xFF;

    if(sum == 0)
    {
        return MYR_DRV_SUCCESS; //checksum is ok
    }
    else
    {
        return MYR_DRV_ERROR; //checksum is not ok
    }
}


s32 brd182InitialiseI2C(tyI2cConfig * i2c0Cfg, tyI2cConfig * i2c1Cfg,tyI2cConfig * i2c2Cfg,I2CM_Device ** i2c0Dev,I2CM_Device ** i2c1Dev,I2CM_Device ** i2c2Dev)
{
    s32 ret;

    // Unless the user specifies otherwise we use the default configuration
    if (i2c0Cfg == NULL)
        i2c0Cfg = &i2c0DefaultConfiguration;

    if (i2c1Cfg == NULL)
        i2c1Cfg = &i2c1DefaultConfiguration;

    if (i2c2Cfg == NULL)
        i2c2Cfg = &i2c2DefaultConfiguration;

    // Initialise the I2C device to use the I2C0 Hardware block
    ret = DrvI2cMInitFromConfig(&i2cDevHandle[0], i2c0Cfg);
    if (ret != I2CM_STAT_OK)
    {
        return -1;
    }
    *i2c0Dev = &i2cDevHandle[0]; // Return the handle

    // Initialise the I2C device to use the I2C1 Hardware block
    ret = DrvI2cMInitFromConfig(&i2cDevHandle[1], i2c1Cfg);
    if (ret != I2CM_STAT_OK)
    {
        return -2;
    }
    *i2c1Dev = &i2cDevHandle[1]; // Return the handle

    // Initialise the I2C device to use the I2C1 Hardware block
    ret = DrvI2cMInitFromConfig(&i2cDevHandle[2], i2c2Cfg);
    if (ret != I2CM_STAT_OK)
    {
        return -3;
    }
    *i2c2Dev = &i2cDevHandle[2]; // Return the handle

    // Also setup a common error handler for I2C
    if (i2c0Cfg->errorHandler)
        DrvI2cMSetErrorHandler(&i2cDevHandle[0], i2c0Cfg->errorHandler);

    if (i2c1Cfg->errorHandler)
        DrvI2cMSetErrorHandler(&i2cDevHandle[1], i2c1Cfg->errorHandler);

    if (i2c2Cfg->errorHandler)
        DrvI2cMSetErrorHandler(&i2cDevHandle[2], i2c2Cfg->errorHandler);

    return 0;
}

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
// This is patch for MV0182 R4 with MA2x5x as level shifter is always ON
static void brd182PatchSDIOInterface(void)
{
    // Fix for R4 and MA2x5x.
    SET_REG_BITS_MASK(AON_RETENTION0_ADR, 1 << SDIO_1_8V_INTERFACE_SELECT);
}
#endif

//function return board revision: R1/R0, R2  R3 or R4
s32 brd182GetPcbRevison(tyMv0182PcbRevision *brdRev)
{
    static tyMv0182PcbRevision detectedPcbRevision = MV0182_REV_NOT_INIT; // Default to unknown
    u8 data;
    s32 pinVal;
    u8 eepromData[EEPROM_MEMORY_SIZE];
    I2CM_Device i2c2Dev;
    I2CM_StatusType status;
    tyI2cConfig * i2c2Cfg;
    i2c2Cfg = &i2c2RevisionDetectConfiguration;
    // i2c communication is used to make difference between R2, R3 and R4
    // on R2 boards, 0x18 i2c address is used for BMX slave
    // on R3 boards, 0x18 i2c address is used for TLV slave
    // first register (address 0x00) is read from the slave who have 0x18 address
    // if value is 0xFA (device id for BMX,always should be 0xFA)means that BMX
    //respond and revision is R2. If other value is received, means that TLV slave
    //respond (TLV never should have value 0xFA at address 0x00) and revision is R3 or R4.
    //on R4 board, exist an EEPROM memory that store revision details.
    //on R4 board slave from address 0x50 (EEPROM) respond.
    //on R3 board slave from address 0x50 does not respond.

    // Only read the revision for first time when this function is called. Afterwards we just return the value
    if (detectedPcbRevision == MV0182_REV_NOT_INIT)
    {
        // Setup the necessary GPIOS to enable weak pullups.
        // we do this so that this function can be called before performing the primary GPIO configurations
        DrvGpioInitialiseRange(brdMv0182RevDetectConfig);
        //GPIO 9 (MV0182_REV_DETECT) is set to low
        //if pull up (specific for R2, R3 and R4 revision) is mounted,
        //the GPIO should be high, otherwise should be low (revision R1/R0)
        pinVal  = (DrvGpioGetPin(MV0182_REV_DETECT) & 0x01);

        if (pinVal == PIN_LEVEL_LOW)
        {
             //pin low -> revision is R0/R1
            detectedPcbRevision = MV0182_R0R1;
            DPRINTF_BOARD("Revision detected: R0-R1\n");
        }
        else
        {
            //pin high -> revision is R2 or R3

            // Initialise the I2C device to read the slave from address 0x18
            status = DrvI2cMInitFromConfig(&i2c2Dev, i2c2Cfg);
            if (status != I2CM_STAT_OK)
            {
                DPRINTF_BOARD("I2C initialization error\n");
                return (s32)MYR_DRV_ERROR;
            }

            // read register 0x00 from slave 0x18
            status = DrvI2cMTransaction(&i2c2Dev, BMX_OR_TLV_SLAVE_ADDRESS,
                    REGISTER_0X00, protocolReadSample, &data,
                    ONE_BYTE_SIZE);

            if (status != I2CM_STAT_OK)
            {
                DPRINTF_BOARD("I2C error\n");
                return (s32)MYR_DRV_ERROR;
            }

            //Check if at address 0x18 BXM slave respond (data should be BMX accelerometer chip ID) or TLV
            if (data == BMX_ACCELEROMETER_ID)
            {
                //if BMX is slave respond with BMX_ACCELEROMETER_ID at slave address 0x18, means that revision is R2
                detectedPcbRevision = MV0182_R2;
                DPRINTF_BOARD("Revision detected: R2\n");
            }
            else
            {
                //if data is not BMX_ACCELEROMETER_ID, means that TLV respond and revision is R3, R4 or R5
                //check EEPROM the detect revision:
                // if I2CM_STAT_ABRT_7B_ADDR_NOACK (-10) is received, EEPROM doesn't exist on board -> revision R3,
                // else read revision details from eeprom for revision R4 or R5
                cleanBuffer(&eepromData[0],EEPROM_MEMORY_SIZE);
                status = DrvI2cMTransaction(&i2c2Dev, EEPROM_I2C_SLAVE_ADDRESS,
                                EEPROM_PART_1_OFFSET,
                                protocolReadSample2,
                                (u8 *) &eepromData[EEPROM_PART_1_OFFSET],
                                EEPROM_PART_1_SIZE);
                if(status == I2CM_STAT_ABRT_7B_ADDR_NOACK)
                {
                    //eeprom does not respond -> revision is R3;
                    detectedPcbRevision = MV0182_R3;
                    DPRINTF_BOARD("Revision detected: R3\n");
                }
                else
                {
                    if(status != I2CM_STAT_OK)
                    {
                        //check other I2C error
                        DPRINTF_BOARD("I2C error\n");
                        return (s32)MYR_DRV_ERROR;
                    }
                    else
                    {
                        //read rest of EEPROM
                        status = DrvI2cMTransaction(&i2c2Dev, EEPROM_I2C_SLAVE_ADDRESS,
                                EEPROM_PART_2_OFFSET,
                                protocolReadSample2,
                                (u8 *) &eepromData[EEPROM_PART_2_OFFSET],
                                EEPROM_PART_2_SIZE);
                        if(status != I2CM_STAT_OK)
                        {
                            DPRINTF_BOARD("I2C error\n");
                            return (s32)MYR_DRV_ERROR;
                        }

                        if (verifyCheckSum(&eepromData[0], EEPROM_MEMORY_SIZE) == MYR_DRV_SUCCESS)
                        {
                            switch (eepromData[PCB_REVISION_OFFSET])
                            {
                            case R4_EEPROM_VALUE:
                                detectedPcbRevision = MV0182_R4;
                                DPRINTF_BOARD("Revision detected: R4\n");

                                //apply patch only for R4E2 - ma2150
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
                                brd182PatchSDIOInterface();
#endif
                                break;
                            case R5_EEPROM_VALUE:
                                detectedPcbRevision = MV0182_R5;
                                DPRINTF_BOARD("Revision detected: R5\n");
                                break;
                            default:
                                DPRINTF_BOARD("Error: invalid revision in EEPROM\n");
                                return (s32) MYR_DRV_ERROR;
                                break;
                            }
                        }
                        else
                        {
                            DPRINTF_BOARD("Error: checksum from EEPROM is invalid\n");
                            return (s32)MYR_DRV_ERROR;
                        }
                    }
                }
            }
        }
    }
    *brdRev = detectedPcbRevision;
    return (s32)MYR_DRV_SUCCESS;
}

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// Static Function Implementations
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------

static u32 commonI2CErrorHandler(I2CM_StatusType i2cCommsError, u32 slaveAddr, u32 regAddr)
{
    UNUSED(slaveAddr); // hush the compiler warning.
    UNUSED(regAddr);

    if(i2cCommsError != I2CM_STAT_OK)
    {
        DPRINTF_BOARD("\nI2C Error (%d) Slave (%02X) Reg (%02X)",i2cCommsError,slaveAddr,regAddr);
        assert(i2cCommsError == 0);
    }
    return i2cCommsError; // Because we haven't really handled the error, pass it back to the caller
}

// ---------------------------------------------------------------------------------
// External PLL
// ---------------------------------------------------------------------------------
s32 brd182ExternalPllConfigure(u32 configIndex)
{
    I2CM_Device *   pllI2cHandle;
    int             retVal;

    // Get handle for the I2C
    pllI2cHandle = &i2cDevHandle[2];

    DrvGpioSetPin(58, 1); //enable the CDCEL chip
   // Configure the PLL
    if ((retVal = CDCE925Configure(pllI2cHandle,configIndex)))
        return retVal;

    return 0; // Success
}

s32 brd182SetLed(I2CM_Device * i2cDevice,tyBrdLedId ledNum,tyLedState ledState)
{
    const u32 LEN_TWO_BYTES=2;
    const u32 MSB_LED_OFF  =0x00;
    const u32 MSB_LED_ON   =0x40;
    const u32 LSB_LED      =0x00;
    int retVal;
    u8 protocolWriteSample2[] = I2C_PROTO_WRITE_16BA;

    u16 regAddr;
    u8 controlValue[2];

    if (ledNum == BRD_LED1)
        regAddr = WM8325_STATUS_LED_1;
    else if (ledNum == BRD_LED2)
        regAddr = WM8325_STATUS_LED_2;
    else
    {
        #ifdef NDEBUG
        exit(-1);
        #else
        assert(FALSE);
        #endif
    }

    if (ledState == LED_ON)
        controlValue[0] = MSB_LED_ON;
    else
        controlValue[0] = MSB_LED_OFF;

    controlValue[1] = LSB_LED;
    retVal = DrvI2cMTransaction(i2cDevice, MV0182_WM8325_I2C_ADDR_7BIT,
                                regAddr,
                                protocolWriteSample2,
                                (u8 *) &controlValue[0],
                                LEN_TWO_BYTES);

    return retVal;
}
