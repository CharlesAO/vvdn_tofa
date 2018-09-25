/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     rtems driver for board mv0212
///

#include <OsDrvGpio.h>
#include <OsDrvI2cBus.h>
#include <OsDrvCpr.h>
#include <brdMv0212GpioDefaults.h>
#include <rtems.h>
#include <OsDrvI2cBus.h>
#include <OsDrvI2cMyr2.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <rtems.h>
#include <rtems/libio.h>

// Driver dependencies
#include <OsWm8325.h>
#include <OsCDCEL.h>
#include <OsADV7513.h>
#include <OsEEPROM.h>
#include <OsOv7750.h>
#include <OsImx208.h>
#include <OsImx214.h>
#include <OsDrvBMI160.h>
#include "OsMV0212.h"

#if 0
#define BOARD_PRINTF(...) printf(__VA_ARGS__)
#else
#define BOARD_PRINTF(...)
#endif
// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Read size for EEPROM
#define EEPROM_READ_SIZE                        0x23
// EEPROM offset to read the board name
#define BOARD_NAME_OFFSET                       0x07
// EEPROM offset to read the PCB revision
#define EEPROM_PCB_REVISION_OFFSET              0x0D
// I2C address
#define EEPROM_SLAVE_ADDRESS                    0x50
// EEPROM device name
#define EEPROM_NAME                             "24AA32A"

#define BOARD_212_NAME                          "MV0212"

// Default I2C Bus name for LibI2C
#define DEFAULT_I2C_BUSNAME                     "/dev/i2c"

// Maximum number of buses to describe devices
#define MAX_I2C_BUSES_PER_DEV                   (3)
#define MAX_I2C_ADDRESSES_PER_DEV               (8)
#define RECOVERY_TRIES                          (100) // Try 100 SCL clocks to recover SDA stuck low
#define NUM_CLOCKS_I2C_RESET                    (16) // Try at least 16 clocks to recover SDA stuck low

typedef struct
{
    const char* name;
    const rtems_libi2c_drv_t* protocolDesc;
    int32_t busIds[MAX_I2C_BUSES_PER_DEV];
    uint16_t addresses[MAX_I2C_ADDRESSES_PER_DEV];
}BoardDevList;

static struct
{
    BoardStatusCode initialised;
    uint32_t boardRev;
}boardControl = { .initialised = BRD_UNINITIALIZED };

typedef int32_t I2CBusHandlers;

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
MV0212GpioCfg BoardMV0212GPIOCFGDefault = { .version = 0,
                                            .gpioConfig = brdMV0212GpioCfgDefault };


// 4: Static Local Data
// ----------------------------------------------------------------------------
#pragma weak i2cbusHandlers
I2CBusHandlers i2cbusHandlers[3];

/// check DECLARE_I2C_BUS description before calling
DECLARE_I2C_BUS(myr2_brd_mv212_i2c_0, 1, I2C_SPEED_FS, 0, 8);
DECLARE_I2C_BUS(myr2_brd_mv212_i2c_1, 2, I2C_SPEED_FS, 0, 8);
DECLARE_I2C_BUS(myr2_brd_mv212_i2c_2, 3, I2C_SPEED_FS, 0, 8);

static BoardDevList boardi2cDeviceListFirstPhase[] =
{
    {
        .name = WM8325_NAME,
        .protocolDesc = &wm8325ProtocolDrvTbl,
        .busIds = {2, -1, -1},
        .addresses = {WM8325_SLAVE_ADDR, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};

static BoardDevList i2cDeviceListImx214Cam[] =
{
    {
        .name = "imx214_video",
        .protocolDesc = &imx214ProtocolDrvTbl,
        .busIds = {1, -1, -1},
        .addresses = {IMX214_I2C_ADDR, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};

static BoardDevList i2cDeviceListOv7750CamPair[] =
{
    {
        .name = "ov7750_tracking_left",
        .protocolDesc = &ov7750ProtocolDrvTbl,
        .busIds = {1, 0, -1},
        .addresses = {0x61, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "ov7750_tracking_right",
        .protocolDesc = &ov7750ProtocolDrvTbl,
        .busIds = {1, 0, -1},
        .addresses = {0x62, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "ov7750_tracking",
        .protocolDesc = &ov7750BroadcastProtocolDrvTbl,
        .busIds = {1, 0, -1},
        .addresses = {0x71, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};

static BoardDevList i2cDeviceListImx208CamPair[] =
{
    {
        .name = "imx208_left",
        .protocolDesc = &imx208ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {(0x6c >> 1), 0, 0, 0, 0, 0, 0},
    },
    {
        .name = "imx208_right",
        .protocolDesc = &imx208ProtocolDrvTbl,
        .busIds = {0, 1, -1},
        .addresses = {(0x6e >> 1), 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};

static BoardDevList boardi2cDeviceListSecondPhase[] =
{
    {
        .name = "BMI160",
        .protocolDesc = &OsBMI160DriverDescriptor,
        .busIds = {2, -1, -1},
        .addresses = {0x68, 0x69, 0, 0, 0, 0, 0},
    },
    {
        .name = CDCEL_NAME,
        .protocolDesc = &cdcelProtocolDrvTbl,
        .busIds = {2, -1, -1},
        .addresses = {CDCEL913_SLAVE_ADDR, CDCEL925_SLAVE_ADDR, 0, 0, 0, 0, 0},
    },
    {
        .name = ADV7513_NAME,
        .protocolDesc = &adv7513ProtocolDrvTbl,
        .busIds = {2, -1, -1},
        .addresses = {ADV7513_SLAVE_ADDRESS, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = EEPROM_NAME,
        .protocolDesc = &eepromDrvTbl,
        .busIds = {2, -1, -1},
        .addresses = {EEPROM_SLAVE_ADDRESS, 0, 0, 0, 0, 0, 0},
    },
    {
        .name = NULL,
        .protocolDesc = NULL,
        .busIds = {-1, -1, -1},
        .addresses = {0, 0, 0, 0, 0, 0, 0, 0},
    }
};
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

static int32_t rtemsGetMajorMinor(const char * name,
                              rtems_device_major_number *major,
                              rtems_device_minor_number *minor)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    struct stat st;
    sc = stat(name, &st);
    if (sc)
    {
        return -sc;
    }

    if(S_ISCHR( st.st_mode ))
    {
        if (major)
            *major = rtems_filesystem_dev_major_t( st.st_rdev );
        if (minor)
            *minor = rtems_filesystem_dev_minor_t( st.st_rdev );
        return sc;
    }
    else
    {
        return RTEMS_UNSATISFIED;
    }
}

// EEPROM CRC Checking function
static u32 eepromCRCCheck(u8 *src, u32 size)
{
    u32 sum = 0, index  = 0;

    for(index = 0;index < size;index++)
        sum = sum + *(src+index);
    sum = sum & 0xFF;
    return sum;
}

// Read all EEPROM data and verify CRC
static s32 readAllEEPROMData(const char *dev_name, u8 *data, u32 size)
{
    int status = 0, fd = 0;
    fd = open(dev_name, O_RDONLY,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
    if (fd < 0)
        return fd;
    // Set offsset at the beginning
    status = lseek(fd, 0, SEEK_SET);
    if (status < 0)
        goto error;
    // Read
    status = read(fd, data, size);
    if (status < 0)
        goto error;
    status = close(fd);
    if (status < 0)
        return status;
    // Check CRC
    if (eepromCRCCheck(data, size) != 0)
        return -1;
    else
        return 0;
error:
    close(fd);
    return status;
}

static int32_t boardGetBrdInfo(BoardInfo *info)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    u8 *buff = (u8 *) malloc(EEPROM_READ_SIZE);

    if (!buff)
        return RTEMS_NO_MEMORY;

    memset(buff, 0, EEPROM_READ_SIZE);

    // Read and verify
    sc = readAllEEPROMData(DEFAULT_I2C_BUSNAME "." EEPROM_NAME, buff, EEPROM_READ_SIZE);
    if (sc == 0)
    {
        info->revision = buff[EEPROM_PCB_REVISION_OFFSET];
        memcpy(info->name, &buff[BOARD_NAME_OFFSET], NUM_CHARS_BRD_NAME + 1);
    }
    else
        return sc;

    // deallocate memory
    free(buff);
    return sc;
}

int32_t BoardGetPCBRevision(uint32_t *brd_rev)
{
    // Get board revision
    *brd_rev = boardControl.boardRev;

    return BRDCONFIG_SUCCESS;
}

static BoardErrorCode boardRegisterI2CDevices(BoardDevList* i2c_device_list)
{
    BOARD_PRINTF("\nRegistering a list of i2c devices:\n");
    uint32_t dev_ind = 0;
    int32_t i2c_drv_minor;
    BoardDevList* device = &i2c_device_list[dev_ind];
    BoardErrorCode rc = BRDCONFIG_SUCCESS;

    while(device->name != NULL)
    {
        BOARD_PRINTF("Registering i2c device %s\n", device->name);
        int32_t i = 0, j = 0;
        while (i < MAX_I2C_BUSES_PER_DEV && device->busIds[i] != -1)
        {
            j = 0;
            while (j < MAX_I2C_ADDRESSES_PER_DEV && device->addresses[j] != 0)
            {
                BOARD_PRINTF("Polling address 0x%x on bus %ld\n", device->addresses[j], device->busIds[i]);
                i2c_drv_minor = rtems_libi2c_register_drv(device->name,
                                                        (rtems_libi2c_drv_t*) device->protocolDesc,
                                                        i2cbusHandlers[device->busIds[i]],
                                                        device->addresses[j]);
                if (i2c_drv_minor >= 0)
                {
                    BOARD_PRINTF("i2c device %s registered at bus %ld with address 0x%x\n",
                           device->name, device->busIds[i], device->addresses[j]);
                    break;
                }
                j++;
            }

            if (j < MAX_I2C_ADDRESSES_PER_DEV && device->addresses[j] != 0)
            {
                // The device was found. No need to poll it on the other buses
                break;
            }
            i++;
        }
        if (i >= MAX_I2C_BUSES_PER_DEV || device->busIds[i] == -1 )
        {
            // This isn't an error, we can use this feature to autodetect devices (i.e  what camera is connected to the board)
            BOARD_PRINTF("Device not found\n");
            rc = BRDCONFIG_ERROR;
        }
        BOARD_PRINTF("----------------------------------------\n");
        device = &i2c_device_list[++dev_ind];
    }

    return rc;
}

void registerCamADevices(void)
{
    boardRegisterI2CDevices(i2cDeviceListImx214Cam);

    return;
}

void registerCamBDevices(void)
{
    BoardErrorCode retCode;

    // Try to find connected cameras; when found, stop searching
    retCode = boardRegisterI2CDevices(i2cDeviceListImx208CamPair);
    if (retCode == BRDCONFIG_SUCCESS)
        return;

    boardRegisterI2CDevices(i2cDeviceListOv7750CamPair);

    return;
}

static int checkAndRecoverI2cSdaLines(void)
{
    char sclName[32];
    char sdaName[32];
    int j, sc;

    for( j = 0 ; j < 3 ; j++)
    {
        sprintf(sclName, "/dev/gpio.i2c%d_scl", j);
        sprintf(sdaName, "/dev/gpio.i2c%d_sda", j);

        int sclFd = open(sclName, O_RDWR);
        int sdaFd = open(sdaName, O_RDWR);

        BOARD_PRINTF("%s: %d ; %s: %d\n", sclName, sclFd, sdaName, sdaFd);

        int rawVal = 0;
        sc = ioctl(sdaFd, GPIO_GET_RAW, &rawVal);
        if (sc == 0 && rawVal == 0)
        {
            int i;
            int sclPinMode;
            int sdaPinMode;
            int sclPinModeFake = D_GPIO_MODE_7;

            BOARD_PRINTF("\n%s stuck low, attempting recovery\n",sdaName);

            ioctl(sdaFd, GPIO_GET_MODE, &sdaPinMode);
            ioctl(sclFd, GPIO_GET_MODE, &sclPinMode);

            ioctl(sclFd, GPIO_SET_MODE, &sclPinModeFake);

            for (i=0 ; i < RECOVERY_TRIES ; i++)
            {
                int gpioVal;

                gpioVal = GPIO_HIGH;
                write(sclFd, &gpioVal, sizeof(gpioVal));
                usleep(1000);
                gpioVal = GPIO_LOW;
                write(sclFd, &gpioVal, sizeof(gpioVal));
                usleep(1000);

                sc = ioctl(sdaFd, GPIO_GET_RAW, &rawVal);
                if ((rawVal != 0) && (i > NUM_CLOCKS_I2C_RESET))
                    break;
            }
            if (i == RECOVERY_TRIES)
            {
                OS_I2C_DPRINTF2("\nRecovery Failed: %s still stuck low (%d)\n",sdaName,i);
                close(sclFd);
                close(sdaFd);
                return RTEMS_IO_ERROR;
            }
            else
            {
                OS_I2C_DPRINTF3("\nRecovery Success: %s is now High (%d)\n",sdaName,i);
            }

            ioctl(sdaFd, GPIO_SET_MODE, &sdaPinMode);
            ioctl(sclFd, GPIO_SET_MODE, &sclPinMode);
        }

        close(sclFd);
        close(sdaFd);
    }
    return 0;
}

//TODO change the return type to BoardErrorCode, here and downstream (=> API "change")
static int32_t boardInitialiseI2C(BoardI2CDevConfiguration* config)
{
    int32_t ret_code;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    // init I2C library
    if ((ret_code = rtems_libi2c_initialize()) != RTEMS_SUCCESSFUL)
    {
        return BRDCONFIG_I2C_SLAVE_ERROR;
    }

    int32_t conf_ind = 0;
    while ((config[conf_ind].type != MV212_END) && (config[conf_ind].type != MV212_I2C0_STATE))
        conf_ind++;
    // if the user didn't specify any config for i2c0, by default it's master.
    if (((config[conf_ind].type == MV212_I2C0_STATE) && (config[conf_ind].value == MV212_I2C_MASTER)) || config[conf_ind].type == MV212_END)
    {
        //register I2C0 bus as master
        ret_code = rtems_libi2c_register_bus(DEFAULT_I2C_BUSNAME,(rtems_libi2c_bus_t *) &myr2_brd_mv212_i2c_0);
        if (ret_code < 0)
        {
            return BRDCONFIG_I2C_SLAVE_ERROR;
        }
        else
        {
            i2cbusHandlers[0] = ret_code;
        }
    }
    else
    {
        // TODO: Configure i2c0 as slave. Right now the user will have to take care of it.
        BOARD_PRINTF("WARNING: i2c0 slave mode configuration is not implemented\n");
    }
    ret_code = rtems_libi2c_register_bus(DEFAULT_I2C_BUSNAME,(rtems_libi2c_bus_t *) &myr2_brd_mv212_i2c_1);
    if (ret_code < 0)
    {
        return BRDCONFIG_I2C_SLAVE_ERROR;
    }
    else
    {
        i2cbusHandlers[1] = ret_code;
    }

    //register I2C2 bus
    ret_code = rtems_libi2c_register_bus(DEFAULT_I2C_BUSNAME,(rtems_libi2c_bus_t *) &myr2_brd_mv212_i2c_2);

    if (ret_code < 0)
    {
        return BRDCONFIG_I2C_SLAVE_ERROR;
    }
    else
    {
        i2cbusHandlers[2] = ret_code;
    }

    ret_code = checkAndRecoverI2cSdaLines();
    if (ret_code < 0)
    {
        return BRDCONFIG_I2C_SLAVE_ERROR;
    }

    // Register I2C devices first phase
    ret_code = boardRegisterI2CDevices(boardi2cDeviceListFirstPhase);
    if (ret_code < 0)
    {
        return ret_code;
    }
    conf_ind = 0;
    while ((config[conf_ind].type != MV212_END) && config[conf_ind].type != MV212_CAM_B_I2C_BUS)
            conf_ind++;
    MV0212CamBBus wm_pin_val = MV212_I2C0;

    if (config[conf_ind].type == MV212_CAM_B_I2C_BUS)
    {
        // if the config was found, set the value requested by user
        wm_pin_val = config[conf_ind].value;
    }
    else
    {
        // if the config was not found, set the default value
        wm_pin_val = MV212_I2C0;
    }
    //Set the I2C bus for CAMB. To do this we need to configure WM8325_I2C_SELECT_PIN_NO pin of wm
    rtems_device_major_number wm_major = 0;
    rtems_device_minor_number wm_minor = 0;
    rtemsGetMajorMinor("/dev/i2c."WM8325_NAME, &wm_major, &wm_minor);
    sc = rtems_io_open(wm_major, wm_minor, NULL);
    if(sc)
        return BRDCONFIG_ERROR;
    wm8325GpioStruct_t gpio_conf;
    rtems_libio_ioctl_args_t ctl;
    ctl.command = WM8325_MODE_SET_GPIO;
    ctl.buffer = (void*) &gpio_conf;
    gpio_conf.gpioMode = WM8325_OUTPUT;
    gpio_conf.gpioNr = WM8325_I2C_SELECT_PIN_NO;
    gpio_conf.gpioVal = wm_pin_val;

    sc = rtems_io_control(wm_major, wm_minor, &ctl);
    if(sc)
        return BRDCONFIG_ERROR;

    ctl.command = WM8325_SET_GPIO;

    sc = rtems_io_control(wm_major, wm_minor, &ctl);
    if(sc)
        return BRDCONFIG_ERROR;

    sc = rtems_io_close(wm_major, wm_minor, NULL);
    if(sc)
        return BRDCONFIG_ERROR;

    // Register I2C devices second phase (optional devices)
    boardRegisterI2CDevices(boardi2cDeviceListSecondPhase);

    // register optional sensors on CAMA connector
    registerCamADevices();

    // register optional sensors on CAMB connector
    registerCamBDevices();

    return BRDCONFIG_SUCCESS;
}

static int32_t boardInitialiseGpioDriver(const BoardGPIOConfig *gpio_cfg)
{
    int32_t ret_code;
    uint32_t gpio_major;

    ret_code = rtems_io_register_driver( 0, &osDrvGpioTblName, &gpio_major );
    if (ret_code != RTEMS_SUCCESSFUL)
    {
        BOARD_PRINTF("gpio register rc = %d\n", ret_code);
        return BRDCONFIG_ERROR;
    }

    ret_code = rtems_io_initialize(gpio_major, 0, (void*)gpio_cfg);
    if (ret_code != RTEMS_SUCCESSFUL)
    {
        BOARD_PRINTF("gpio initialize %d\n", ret_code);
        return BRDCONFIG_ERROR;
    }
    return BRDCONFIG_SUCCESS;
}

int32_t BoardInit(BoardConfigDesc *init_cfg)
{
    int32_t return_code = BRDCONFIG_ERROR;
    int32_t conf_idx = 0;

    BOARD_PRINTF("Starting mv0212 board initialize\n");

    if(boardControl.initialised == BRD_UNINITIALIZED)
    {

        boardControl.initialised = BRD_INITIALIZATION_IN_PROGRESS;

        if (!init_cfg) // Use Default I2C and GPIO configuration
        {
            return_code = boardInitialiseGpioDriver(BoardMV0212GPIOCFGDefault.gpioConfig);
            if(return_code != BRDCONFIG_SUCCESS)
            {
                boardControl.initialised = BRD_UNINITIALIZED;
                return_code = BRD_UNINITIALIZED;
                return return_code;
            }
            BoardI2CDevConfiguration gen_config[] = {{ MV212_END, 0 }}; // default config
            return_code = boardInitialiseI2C(gen_config);
            if(return_code != BRDCONFIG_SUCCESS)
            {
                boardControl.initialised = BRD_UNINITIALIZED;
                return_code = BRD_UNINITIALIZED;
                return return_code;
            }
        }
        else
        {
            while (init_cfg[conf_idx].type != BRDCONFIG_GPIO && init_cfg[conf_idx].type != BRDCONFIG_END)
                conf_idx++;

            if(init_cfg[conf_idx].type == BRDCONFIG_GPIO)
            {
                if (init_cfg[conf_idx].config == NULL)
                return BRDCONFIG_ERROR;

                BoardGPIOConfig* boardCustomCfg = (BoardGPIOConfig*)init_cfg[conf_idx].config;
                return_code = boardInitialiseGpioDriver(boardCustomCfg);
                if(return_code != BRDCONFIG_SUCCESS)
                {
                    boardControl.initialised = BRD_UNINITIALIZED;
                    return_code = BRD_UNINITIALIZED;
                    return return_code;
                }
            }
            else // use the default gpio config
            {
                return_code = boardInitialiseGpioDriver(BoardMV0212GPIOCFGDefault.gpioConfig);
                if(return_code != BRDCONFIG_SUCCESS)
                {
                    boardControl.initialised = BRD_UNINITIALIZED;
                    return_code = BRD_UNINITIALIZED;
                    return return_code;
                }
            }

            conf_idx = 0;
            while (init_cfg[conf_idx].type != BRDCONFIG_I2CDEVLIST && init_cfg[conf_idx].type != BRDCONFIG_END)
                conf_idx++;

            if(init_cfg[conf_idx].type == BRDCONFIG_I2CDEVLIST)
            {
                if (init_cfg[conf_idx].config == NULL)
                return BRDCONFIG_ERROR;

                return_code = boardInitialiseI2C((BoardI2CDevConfiguration *)init_cfg[conf_idx].config);
                if(return_code != BRDCONFIG_SUCCESS)
                {
                    boardControl.initialised = BRD_UNINITIALIZED;
                    return_code = BRD_UNINITIALIZED;
                    return return_code;
                }
            }
            else // use the default i2c config
            {
                BoardI2CDevConfiguration gen_config[] = {{ MV212_END, 0 }}; // default config
                return_code = boardInitialiseI2C(gen_config);
                if(return_code != BRDCONFIG_SUCCESS)
                {
                    boardControl.initialised = BRD_UNINITIALIZED;
                    return_code = BRD_UNINITIALIZED;
                    return return_code;
                }
            }
        }
        // Check if we use the correct board
        static BoardInfo brd_info;
        return_code = boardGetBrdInfo(&brd_info);
        if (strncmp(brd_info.name, BOARD_212_NAME, NUM_CHARS_BRD_NAME) != 0 || return_code != 0)
        {
            boardControl.initialised = BRD_UNINITIALIZED;
            return BRDCONFIG_INVALIDBRD;
        }
        boardControl.boardRev = brd_info.revision;

        boardControl.initialised = BRD_INITIALIZED;
        BOARD_PRINTF("mv0212 board initialized successfully\n");
    }
    else
    {
        return_code = BRD_ALREADY_INITIALIZED;
        BOARD_PRINTF("mv0212 board already initialized \n");
    }

    return return_code;
}
