///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MV0212 Board setup implementation
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvGpio.h"
#include "DrvCDCEL.h"
#include "DrvWm8325.h"
#include "DrvRegUtilsDefines.h"
#include "MV0212.h"
#include "brdMv0212GpioDefaults.h"
#include <string.h>

// #define BOARD_DEBUG  //Debug Messages
#ifdef BOARD_DEBUG
#include <stdio.h>
#define DPRINTF_BOARD(...) printf(__VA_ARGS__)
#else
#define DPRINTF_BOARD(...)
#endif

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define IRQ_SRC_0   0
#define IRQ_SRC_1   1
#define IRQ_SRC_2   2
#define IRQ_SRC_3   3

#define NUM_I2C_DEVICES 3

#define ONE_BYTE_SIZE                    0x01
#define TWO_BYTES_SIZE                   0x02

#define PLL_STATUS_OK   0
#define I2C_STATUS_OK   0

#define EEPROM_MEMORY_SIZE               0x23
#define EEPROM_I2C_SLAVE_ADDRESS         0x50
#define EEPROM_PART_1_SIZE                 32 // only 32 bytes can be read or written at a time from eeprom
#define EEPROM_PART_2_SIZE                  3
#define EEPROM_PART_1_OFFSET             0x00
#define EEPROM_PART_2_OFFSET             0x20
#define PCB_REVISION_OFFSET              0x0D
#define BOARD_NAME_OFFSET                0x07

#ifndef WM8325_SLAVE_ADDRESS
#define WM8325_SLAVE_ADDRESS       0x36
#endif
#define WM8325_GPIOS_LEVEL_LOW   0XF000
#define GPIO_DEFAULT_CFG_VALUE   0xA400  //gpio's default value

#define BOARD_212_NAME                          "MV0212"

static uint32_t boardI2CErrorHandler(I2CM_StatusType error, uint32_t param1, uint32_t param2);
static uint8_t protocolReadSample2[] = I2C_PROTO_READ_16BA;
static uint8_t protocolWriteSample2[] = I2C_PROTO_WRITE_16BA;

MV0212GpioCfg BoardMV0212GPIOCFGDefault = { .version = 0,
                                            .gpioConfig = brdMV0212GpioCfgDefault};

static tyI2cConfig i2c0DefaultConfiguration =
    {
     .device                = IIC1_BASE_ADR,
     .sclPin                = MV0212_I2C0_SCL_PIN,
     .sdaPin                = MV0212_I2C0_SDA_PIN,
     .speedKhz              = MV0212_I2C0_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0212_I2C0_ADDR_SIZE_DEFAULT,
     .errorHandler          = &boardI2CErrorHandler,
    };

static tyI2cConfig i2c1DefaultConfiguration =
    {
     .device                = IIC2_BASE_ADR,
     .sclPin                = MV0212_I2C1_SCL_PIN,
     .sdaPin                = MV0212_I2C1_SDA_PIN,
     .speedKhz              = MV0212_I2C1_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0212_I2C1_ADDR_SIZE_DEFAULT,
     .errorHandler          = &boardI2CErrorHandler,
    };

static tyI2cConfig i2c2DefaultConfiguration =
    {
     .device                = IIC3_BASE_ADR,
     .sclPin                = MV0212_I2C2_SCL_PIN,
     .sdaPin                = MV0212_I2C2_SDA_PIN,
     .speedKhz              = MV0212_I2C2_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0212_I2C2_ADDR_SIZE_DEFAULT,
     .errorHandler          = &boardI2CErrorHandler,
    };

static tyI2cConfig i2c2RevisionDetectConfiguration =
    {
     .device                = IIC3_BASE_ADR,
     .sclPin                = MV0212_I2C2_SCL_PIN,
     .sdaPin                = MV0212_I2C2_SDA_PIN,
     .speedKhz              = MV0212_I2C2_SPEED_KHZ_DEFAULT,
     .addressSize           = MV0212_I2C2_ADDR_SIZE_DEFAULT,
    };

static BoardI2CConfig *defaultCfg[] = { &i2c0DefaultConfiguration,
                                        &i2c1DefaultConfiguration,
                                        &i2c2DefaultConfiguration,
                                        NULL                       };

MV0212I2CCfg BoardMV0212I2CCFGDefault =
    { .version = 0,
      .i2cCfg = defaultCfg
    };


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-const-variable"
static const BoardGPIOConfig brdMV0212RevDetectConfig[] =
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
            D_GPIO_PAD_PULL_DOWN     | // Enable weak pullups so that we can detect revision
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

static I2CM_Device i2cDevHandle[NUM_I2C_DEVICES];

static struct
{
    uint32_t boardRev;
}boardCtrl;

static uint32_t boardI2CErrorHandler(I2CM_StatusType i2c_comms_error, uint32_t slave_addr, uint32_t reg_addr)
{
    UNUSED(slave_addr); // hush the compiler warning.
    UNUSED(reg_addr);

    if(i2c_comms_error != I2CM_STAT_OK)
    {
        DPRINTF_BOARD("\nI2C Error (%d) Slave (%02X) Reg (%02X)", i2c_comms_error, slave_addr, reg_addr);
    }
    return i2c_comms_error; // Because we haven't really handled the error, pass it back to the caller
}


static void cleanBuffer(uint8_t *src, uint32_t size)
{
    uint32_t index;
    for(index = 0; index < size; index++)
    {
        *(src+index)=0;
    }
}

static uint8_t verifyCheckSum(uint8_t *src, uint8_t size)
{
    uint32_t sum = 0;
    uint32_t index;

    for(index = 0;index < size;index++)
    {
        sum = sum + *(src+index);
    }

    sum = sum & 0xFF;

    if(sum == 0)
    {
        return BRDCONFIG_SUCCESS; //checksum is ok
    }
    else
    {

        return BRDCONFIG_ERROR; //checksum is not ok
    }
}

static void boardInitGpio(const BoardGPIOConfig *gpio_cfg)
{
    // TODO: Remove these four lines
    DrvGpioIrqSrcDisable(IRQ_SRC_0);
    DrvGpioIrqSrcDisable(IRQ_SRC_1);
    DrvGpioIrqSrcDisable(IRQ_SRC_2);
    DrvGpioIrqSrcDisable(IRQ_SRC_3);

    DrvGpioInitialiseRange(gpio_cfg);
}

static int32_t cfgWM8325GpiosDefault(I2CM_Device* dev)
{
    int32_t result;
    uint8_t control_value[2];

    // Set all PMIC GPIOs to default state
    control_value[0] = ((GPIO_DEFAULT_CFG_VALUE & 0xFF00) >> 8);
    control_value[1] = (GPIO_DEFAULT_CFG_VALUE & 0x00FF);
    for (uint32_t i = 0; i < 12; ++i)
    {
        result = DrvI2cMTransaction(dev, WM8325_SLAVE_ADDRESS,
                                    (WM8325_GPIO1_CONTROL + i),
                                    protocolWriteSample2,
                                    (uint8_t *) &control_value[0],
                                    TWO_BYTES_SIZE);

            if (result != I2CM_STAT_OK)
            {
                DPRINTF_BOARD("Transaction write failed reg 0x%x \n",  (WM8325_GPIO1_CONTROL + i));
                return (int32_t)BRDCONFIG_I2C_SLAVE_ERROR;
            }
    }

    return (int32_t)BRDCONFIG_SUCCESS;
}

int32_t BoardInitExtPll(uint32_t clock_config)
{
    I2CM_Device *pll_i2c_handle;
    int          ret_val;

    // Get handle for the I2C
    pll_i2c_handle = &i2cDevHandle[2];

    DrvGpioSetPin(58, 1); //enable the CDCEL chip

   // Configure the PLL
    if ((ret_val = CDCE925Configure(pll_i2c_handle, clock_config)))
        return ret_val;

    return (int32_t)BRDCONFIG_SUCCESS; // Success
}

static int32_t boardGetBrdInfo(BoardInfo *info)
{
    uint8_t eeprom_data[EEPROM_MEMORY_SIZE];
    I2CM_Device i2c2_dev;
    I2CM_StatusType status;
    tyI2cConfig *i2c2_cfg;
    i2c2_cfg = &i2c2RevisionDetectConfiguration;

    status = DrvI2cMInitFromConfig(&i2c2_dev, i2c2_cfg);
    if (status != I2CM_STAT_OK)
    {
        DPRINTF_BOARD("I2C initialization error\n");
        return (int32_t)BRDCONFIG_I2C_DRIVER_ERROR;
    }

    // read revision details from eeprom
    cleanBuffer(&eeprom_data[0],EEPROM_MEMORY_SIZE);
    status = DrvI2cMTransaction(&i2c2_dev, EEPROM_I2C_SLAVE_ADDRESS,
                    EEPROM_PART_1_OFFSET,
                    protocolReadSample2,
                    (uint8_t *) &eeprom_data[EEPROM_PART_1_OFFSET],
                    EEPROM_PART_1_SIZE);

    if(status != I2CM_STAT_OK)
    {
        //check other I2C error
        DPRINTF_BOARD("I2C error\n");
        return (int32_t)BRDCONFIG_I2C_SLAVE_ERROR;
    }
    else
    {
        //read rest of EEPROM
        status = DrvI2cMTransaction(&i2c2_dev, EEPROM_I2C_SLAVE_ADDRESS,
                EEPROM_PART_2_OFFSET,
                protocolReadSample2,
                (uint8_t *) &eeprom_data[EEPROM_PART_2_OFFSET],
                EEPROM_PART_2_SIZE);

        if(status != I2CM_STAT_OK)
        {
            DPRINTF_BOARD("I2C error\n");
            return (int32_t)BRDCONFIG_I2C_SLAVE_ERROR;
        }

        if (verifyCheckSum(&eeprom_data[0], EEPROM_MEMORY_SIZE) == BRDCONFIG_SUCCESS)
        {

            info->revision = eeprom_data[PCB_REVISION_OFFSET];
            memcpy(info->name, &eeprom_data[BOARD_NAME_OFFSET], NUM_CHARS_BRD_NAME + 1);
        }
        else
        {
            DPRINTF_BOARD("Error: checksum from EEPROM is invalid\n");
            return (int32_t)BRDCONFIG_ERROR;
        }
    }

    return (int32_t)BRDCONFIG_SUCCESS;
}

int32_t BoardGetPCBRevision(uint32_t *brd_rev)
{
    *brd_rev = boardCtrl.boardRev;

    return BRDCONFIG_SUCCESS;
}

static int32_t boardI2CInit(void)
{
    int32_t ret = BRDCONFIG_ERROR;
    uint32_t i  = 0;

    while (BoardMV0212I2CCFGDefault.i2cCfg[i] != NULL)
    {
        // Initialise the I2C device to use the I2C Hardware block
        ret = DrvI2cMInitFromConfig(&i2cDevHandle[i], BoardMV0212I2CCFGDefault.i2cCfg[i]);
        if (ret != I2CM_STAT_OK)
        {
            return BRDCONFIG_I2C_DRIVER_ERROR;
        }

        // Also setup a common error handler for I2C
        if (BoardMV0212I2CCFGDefault.i2cCfg[i]->errorHandler)
            DrvI2cMSetErrorHandler(&i2cDevHandle[i], BoardMV0212I2CCFGDefault.i2cCfg[i]->errorHandler);
        i++;
    }
    return BRDCONFIG_SUCCESS;
}

int32_t BoardGetI2CInfo(BoardI2CInfo *info, uint32_t interfaces)
{
    if (info == NULL)
        return BRDCONFIG_ERROR;

    for (uint32_t i = 0; i < interfaces; ++i)
    {
        info[i].handler = &i2cDevHandle[i];
    }

    return BRDCONFIG_SUCCESS;
}

int32_t BoardInit(BoardConfigDesc *init_cfg)
{
    int32_t status = BRDCONFIG_SUCCESS;

    // default I2C configuration in both cases for now
    status = boardI2CInit();
    if(status != BRDCONFIG_SUCCESS)
        return BRDCONFIG_I2C_DRIVER_ERROR;

    // Setting all PMIC GPIOs to default configuration
    status = cfgWM8325GpiosDefault(&i2cDevHandle[2]);
    if(status != BRDCONFIG_SUCCESS)
        return BRDCONFIG_I2C_SLAVE_ERROR;

    if (!init_cfg) // Use Default I2C and GPIO configuration
    {
        boardInitGpio(BoardMV0212GPIOCFGDefault.gpioConfig);
    }
    else
    {
        int32_t conf_idx = 0;

        while (init_cfg[conf_idx].type != BRDCONFIG_END)
        {
            conf_idx++;

            if(init_cfg[conf_idx].type == BRDCONFIG_GPIO)
            {
                if (init_cfg[conf_idx].config == NULL)
                    return BRDCONFIG_ERROR;

                BoardGPIOConfig* board_custom_cfg = (BoardGPIOConfig*)init_cfg[conf_idx].config;
                boardInitGpio(board_custom_cfg);
            }
            else
            {
                boardInitGpio(BoardMV0212GPIOCFGDefault.gpioConfig);
            }
        }
    }

    // Check if we use the correct board
    static BoardInfo brd_info;
    status = boardGetBrdInfo(&brd_info);
    if (strncmp(brd_info.name, BOARD_212_NAME, NUM_CHARS_BRD_NAME) != 0 || status != 0)
    {
        return BRDCONFIG_INVALIDBRD;
    }
    boardCtrl.boardRev = brd_info.revision;

    return status;
}
#pragma GCC diagnostic pop
