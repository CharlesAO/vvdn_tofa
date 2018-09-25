/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     rtems driver for board mv0182
///
#include <OsBrdMv0182.h>
#include <OsDrvGpio.h>
#include <OsDrvI2cBus.h>
#include <OsDrvCpr.h>
#include <brdGpioCfgs/brdMv0182GpioDefaults.h>
#include <rtems.h>
#include <OsDrvI2cBus.h>
#include <OsDrvI2cMyr2.h>
#include <OsBrdMv0182Defines.h>
#include <OsDrvGpio.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <rtems.h>
#include <rtems/libio.h>

// Driver dependencies
#include <OsBmx055.h>
#include <OsWm8325.h>
#include <OsCDCEL.h>
#include <OsADV7513.h>
#include <OsOv7750.h>
#include <OsImx208.h>
#include <OsImx214.h>
#include <OsEEPROM.h>
#include <OsDrvBMI160.h>

#if 0
#define MV182PRINTF(...) printf(__VA_ARGS__)
#else
#define MV182PRINTF(...)
#endif
// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Clock enable pin
#define GPIO_58_CLK_ENABLE                      (58)

// PCB Revision Detection
#define GPIO_9_REV_DETECT                       (9)
#define PIN_LOW                                 (0)

//used to recover i2c2 bus sda pin
#define I2C2_SDA_GPIO                           (80)
#define I2C2_SCL_GPIO                           (79)
#define RECOVERY_TRIES                          (100) // Try 100 SCL clocks to recover SDA stuck low
#define NUM_CLOCKS_I2C_RESET                    (16) // Try at least 16 clocks to recover SDA stuck low

#define BMX_ACCELEROMETER_ID                    (0xFA)
#define BMX_OR_TLV_SLAVE_ADDRESS                (0x18)
#define ONE_BYTE_SIZE                           (1)
#define TWO_BYTES_SIZE                          (2)

// Bit used to select 1.8 V in SDIO interface
#define SDIO_1_8V_INTERFACE_SELECT              23

// Read size for EEPROM
#define EEPROM_READ_SIZE                        0x23
// EEPROM offset to read the PCB revision
#define EEPROM_PCB_REVISION_OFFSET              0x0D
// EEPROM Value matching R4 revision
#define EEPROM_R4_VALUE                         0x04
// EEPROM Value matching R5 revision
#define EEPROM_R5_VALUE                         0x05
// I2C address
#define EEPROM_SLAVE_ADDRESS                    0x50
// EEPROM device name
#define EEPROM_NAME                             "24AA32A"
//GPIO 57 is used to enable control of the SDCard voltage
#define GPIO_57_SDCARD_VOLTAGE  57
#define GPIO_HIGH               1
#define GPIO_LOW                0
// Default BMX055 I2C addresses
#define BMX055ACCELEROMETER_DEFAULTI2CADDR1      0x18
#define BMX055ACCELEROMETER_DEFAULTI2CADDR2      0x19

// Default I2C Bus name for LibI2C
#define DEFAULT_I2C_BUSNAME                      "/dev/i2c"

// Maximum number of buses to describe devices
#define MAX_I2C_BUSES_PER_DEV                   (3)
#define MAX_I2C_ADDRESSES_PER_DEV               (8)

// Retrieve I2C Address from LibI2C minor
#define OSBRDMV0182_LIBI2C_GET_I2CADDRESS(x)    (x & 0x3FF)


typedef enum
{
    DRV_OS_BRD_182_UNINITIALIZED  = 0,         // Driver status not initialized
    DRV_OS_BRD_182_INITIALIZED,                // Driver initialized
    DRV_OS_BRD_182_INITIALIZATION_IN_PROGRESS, // Driver opened, ready to be used
} tyOsBrd182StatusCode;

typedef struct
{
    const char* name;
    const rtems_libi2c_drv_t* protocolDesc;
    int32_t busIds[MAX_I2C_BUSES_PER_DEV];
    uint16_t addresses[MAX_I2C_ADDRESSES_PER_DEV];
}tyOsBrd182DevList;

typedef int tyOsI2cbusHandlers;
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------


// 4: Static Local Data
// ----------------------------------------------------------------------------
tyOsMv0182PcbRevision pcbRevision;
#pragma weak i2cbusHandlers
tyOsI2cbusHandlers i2cbusHandlers[3];

/// check DECLARE_I2C_BUS description before calling
DECLARE_I2C_BUS(myr2_i2c_0, 1, I2C_SPEED_FS, 0, 8);
DECLARE_I2C_BUS(myr2_i2c_1, 2, I2C_SPEED_FS, 0, 8);
DECLARE_I2C_BUS(myr2_i2c_2, 3, I2C_SPEED_FS, 0, 8);

static tyOsBrd182DevList i2cDeviceListFirstPhase[] =
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

static tyOsBrd182DevList i2cDeviceListImx214Cam[] =
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

static tyOsBrd182DevList i2cDeviceListOv7750CamPair[] =
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

static tyOsBrd182DevList i2cDeviceListImx208CamPair[] =
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

static tyOsBrd182DevList i2cDeviceListSecondPhase[] =
{
    {
        .name = "BMI160",
        .protocolDesc = &OsBMI160DriverDescriptor,
        .busIds = {2, -1, -1},
        .addresses = {0x68, 0x69, 0, 0, 0, 0, 0},
    },
    {
        .name = BMX055_ACCEL_NAME,
        .protocolDesc = &bmxAccelProtocolDrvTbl,
        .busIds = {2, -1, -1},
        .addresses = {BMX055ACCELEROMETER_DEFAULTI2CADDR2, BMX055ACCELEROMETER_DEFAULTI2CADDR1, 0, 0, 0, 0, 0},
    },
    {
        .name = BMX055_GYRO_NAME,
        .protocolDesc = &bmxGyroProtocolDrvTbl,
        .busIds = {2, -1, -1},
        .addresses = {0x69, 0x68, 0, 0, 0, 0, 0},
    },
    {
        .name = BMX055_MAG_NAME,
        .protocolDesc = &bmxMagProtocolDrvTbl,
        .busIds = {2, -1, -1},
        .addresses = {0x10, 0x11, 0x12, 0x13, 0, 0, 0},
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
static tyOsBrd182ErrorCode osBrdMv0182InitialiseI2C(tyOsBoard0182Configuration* config);
static tyOsBrd182ErrorCode osBrdMv0182InitialiseGpioDriver(void);
static tyOsBrd182ErrorCode checkAndRecoverI2cSdaLines(void);

// 6: Functions Implementation
// ----------------------------------------------------------------------------

static int rtemsGetMajorMinor(const char * name,
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
static s32 readAllEEPROMData(const char *devName, u8 *data, u32 size)
{
    int status = 0, fd = 0;
    fd = open(devName, O_RDONLY,
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

// Read revision from GPIO
static rtems_status_code readPCBRevisionFromGPIO(const char *devName, tyOsMv0182PcbRevision *rev)
{
    rtems_status_code status = RTEMS_IO_ERROR;
    rtems_device_major_number major;
    rtems_device_minor_number minor;
    gpioVal_t revisionPinValue;
    rtems_libio_rw_args_t rw_args = {0};

    if (rtemsGetMajorMinor(devName, &major, &minor) != 0)
        return RTEMS_IO_ERROR;
    // Setup the necessary GPIOS to enable weak pullups was done when gpio's
    // (osBrdMv0182RevDetectConfig) was configured.
    // gpio 9 (MV0182_REV_DETECT) is set to low
    // if pull up (specific for R2 and R3 revision) is mounted,
    // the gpio should be high, otherwise should be low (revision R1/R0)
    status = rtems_io_open(major, GPIO_9_REV_DETECT, NULL);
    if (status)
        return status;
    //read
    rw_args.buffer = (void*)&revisionPinValue;
    status = rtems_io_read(major, GPIO_9_REV_DETECT, &rw_args);
    if (status)
        return status;
    //Release
    status = rtems_io_close(major, GPIO_9_REV_DETECT, NULL);
    if (status)
        return status;
    // pin low -> revision is R0/R1
    if (revisionPinValue == PIN_LOW)
        *rev = OS_MV0182_R0R1;
    else
        *rev = OS_MV0182_REV_NOT_DETECTED;
    return RTEMS_SUCCESSFUL;
}

// Retrieve PCB Revision
static rtems_status_code readPCBRevisionFromEEPROM(char *devName, u32 readSize, u32 revOffset, tyOsMv0182PcbRevision *rev)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
    u8 *buff = (u8 *) malloc(readSize);

    if (!buff)
        return RTEMS_NO_MEMORY;
    memset(buff, 0, readSize);
    // By default not detected
    *rev = OS_MV0182_REV_NOT_DETECTED;
    // Read and verify
    if (readAllEEPROMData(devName, buff, readSize) == 0) {
        switch(buff[revOffset]) {
            case EEPROM_R4_VALUE:
                *rev = OS_MV0182_R4;
                break;
            case EEPROM_R5_VALUE:
                *rev = OS_MV0182_R5;
                break;
            default:
                // If memory is present but no PCB revision retrieved IO ERROR !
                sc = RTEMS_IO_ERROR;
                break;
        }
    }
    // deallocate memory
    free(buff);
    return sc;
}

// Read revision based on I2C address BMX055
static rtems_status_code readPCBRevisionfromBMX055AccelAddr(const char *devName, tyOsMv0182PcbRevision *pcbRev)
{
    rtems_device_minor_number minor;
    rtems_device_major_number major;
    u32 i2cAddr = 0;
    rtems_status_code sc = RTEMS_IO_ERROR;

    // Get current major and minor to deduct I2C address
    if ((sc = rtemsGetMajorMinor(devName, &major, &minor)) == 0) {
        // Obtain address from LibI2C minor
        i2cAddr = OSBRDMV0182_LIBI2C_GET_I2CADDRESS(minor);
        if (i2cAddr == BMX055ACCELEROMETER_DEFAULTI2CADDR1)
            *pcbRev = OS_MV0182_R2;
        else if (i2cAddr == BMX055ACCELEROMETER_DEFAULTI2CADDR2)
            *pcbRev = OS_MV0182_R3;
        else
            *pcbRev = OS_MV0182_REV_NOT_DETECTED;
    }
    return sc;
}

// General operation to detect
static rtems_status_code osBrdDetectPCBRevision(tyOsMv0182PcbRevision *pcbRev)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;
#ifdef MA2450
    *pcbRev = OS_MV0212;
    return RTEMS_SUCCESSFUL;
#endif
    tyOsMv0182PcbRevision rev = OS_MV0182_REV_NOT_DETECTED;

    // 1. Check if we can detect revision from GPIO
    sc = readPCBRevisionFromGPIO(OSDRVGPIO_DEVNAME, &rev);
    // Do we already have proper revision number? or an error ?
    if ((sc) || ((sc == 0) && (rev != OS_MV0182_REV_NOT_DETECTED))) {
        *pcbRev = rev;
        return sc;
    }
    // 2. Check if revision is present in EEPROM
    sc = readPCBRevisionFromEEPROM(DEFAULT_I2C_BUSNAME "." EEPROM_NAME, EEPROM_READ_SIZE,
                                    EEPROM_PCB_REVISION_OFFSET, &rev);
    // Do we already have proper revision number? or an error ?
    if ((sc) || ((sc == 0) && (rev != OS_MV0182_REV_NOT_DETECTED))) {
        *pcbRev = rev;
        return sc;
    }
    // 3. Check if revision can be retrieved from BMX055 Accelerometer I2C Address
    sc = readPCBRevisionfromBMX055AccelAddr(DEFAULT_I2C_BUSNAME "." BMX055_ACCEL_NAME, &rev);

    // Do we already have proper revision number? or an error ?
    if ((sc) || ((sc == 0) && (rev != OS_MV0182_REV_NOT_DETECTED))) {
        *pcbRev = rev;
        return sc;
    }
    // No valid means of retrieving revision number
    return RTEMS_IO_ERROR;
}

static tyOsBrd182ErrorCode osBrdMv0182RegisterI2CDevices(tyOsBrd182DevList* i2cDeviceList)
{
    MV182PRINTF("\nRegistering a list of i2c devices:\n");
    uint32_t devInd = 0;
    int32_t i2cDrvMinor;
    tyOsBrd182DevList* device = &i2cDeviceList[devInd];
    tyOsBrd182ErrorCode rc = DRV_OS_BRD_182_DRV_SUCCESS;

    while(device->name != NULL)
    {
        MV182PRINTF("Registering i2c device %s\n", device->name);
        int32_t i = 0, j = 0;
        while (i < MAX_I2C_BUSES_PER_DEV && device->busIds[i] != -1)
        {
            j = 0;
            while (j < MAX_I2C_ADDRESSES_PER_DEV && device->addresses[j] != 0)
            {
                MV182PRINTF("Polling address 0x%x on bus %ld\n", device->addresses[j], device->busIds[i]);
                i2cDrvMinor = rtems_libi2c_register_drv(device->name,
                                                        (rtems_libi2c_drv_t*) device->protocolDesc,
                                                        i2cbusHandlers[device->busIds[i]],
                                                        device->addresses[j]);
                if (i2cDrvMinor >= 0)
                {
                    MV182PRINTF("i2c device %s registered at bus %ld with address 0x%x\n",
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
            MV182PRINTF("Device not found\n");
            rc = DRV_OS_BRD_182_DRV_ERROR;
        }
        MV182PRINTF("----------------------------------------\n");
        device = &i2cDeviceList[++devInd];
    }
    return rc;
}

void registerCamA(void)
{
    osBrdMv0182RegisterI2CDevices(i2cDeviceListImx214Cam);

    return;
}

void registerCamB(void)
{
    tyOsBrd182ErrorCode retCode;

    // Try to find connected cameras; when found, stop searching
    retCode = osBrdMv0182RegisterI2CDevices(i2cDeviceListImx208CamPair);
    if (retCode == DRV_OS_BRD_182_DRV_SUCCESS)
        return;

    osBrdMv0182RegisterI2CDevices(i2cDeviceListOv7750CamPair);

    return;
}

static tyOsBrd182ErrorCode osBrdMv0182InitialiseI2C(tyOsBoard0182Configuration* config)
{
    int32_t retCode;
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    // init I2C library
    if ((retCode = OsDrvLibI2CInitialize()) != RTEMS_SUCCESSFUL)
    {
        return DRV_OS_BRD_182_I2C_DRIVER_ERROR;
    }

    int32_t confInd = 0;
    while ((config[confInd].type != MV182_END) && (config[confInd].type != MV182_I2C0_STATE))
        confInd++;
    // if the user didn't specify any config for i2c0, by default it's master.
    if (((config[confInd].type == MV182_I2C0_STATE) && (config[confInd].value == MV182_I2C_MASTER)) || config[confInd].type == MV182_END)
    {
        //register I2C0 bus as master
        retCode = rtems_libi2c_register_bus(DEFAULT_I2C_BUSNAME,(rtems_libi2c_bus_t *) &myr2_i2c_0);
        if (retCode < 0)
        {
            return DRV_OS_BRD_182_I2C_DRIVER_ERROR;
        }
        else
        {
            i2cbusHandlers[0] = retCode;
        }
    }
    else
    {
        // TODO: Configure i2c0 as slave. Right now the user will have to take care of it.
        MV182PRINTF("WARNING: i2c0 slave mode configuration is not implemented\n");
    }
    retCode = OsDrvLibI2CRegisterBus(DEFAULT_I2C_BUSNAME,(rtems_libi2c_bus_t *) &myr2_i2c_1);
    if (retCode < 0)
    {
        return DRV_OS_BRD_182_I2C_DRIVER_ERROR;
    }
    else
    {
        i2cbusHandlers[1] = retCode;
    }

    //register I2C2 bus
    retCode = OsDrvLibI2CRegisterBus(DEFAULT_I2C_BUSNAME,(rtems_libi2c_bus_t *) &myr2_i2c_2);

    if (retCode < 0)
    {
        return DRV_OS_BRD_182_I2C_DRIVER_ERROR;
    }
    else
    {
        i2cbusHandlers[2] = retCode;
    }

    retCode = checkAndRecoverI2cSdaLines();
    if (retCode < 0)
    {
        return DRV_OS_BRD_182_I2C_DRIVER_ERROR;
    }

    // Register I2C devices first phase
    retCode = osBrdMv0182RegisterI2CDevices(i2cDeviceListFirstPhase);
    if (retCode != DRV_OS_BRD_182_DRV_SUCCESS)
    {
        return retCode;
    }

    confInd = 0;
    while ((config[confInd].type != MV182_END) && config[confInd].type != MV182_CAM_B_I2C_BUS)
            confInd++;
    tyOsMv0182CamBBus wmPinVal = MV182_I2C0;

    if (config[confInd].type == MV182_CAM_B_I2C_BUS)
    {
        // if the config was found, set the value requested by user
        wmPinVal = config[confInd].value;
    }
    else
    {
        // if the config was not found, set the default value
        wmPinVal = MV182_I2C0;
    }
    //Set the I2C bus for CAMB. To do this we need to configure WM8325_I2C_SELECT_PIN_NO pin of wm
    rtems_device_major_number wmMajor = 0;
    rtems_device_minor_number wmMinor = 0;
    rtemsGetMajorMinor("/dev/i2c."WM8325_NAME, &wmMajor, &wmMinor);
    sc = rtems_io_open(wmMajor, wmMinor, NULL);
    if(sc)
        return DRV_OS_BRD_182_DRV_ERROR;
    wm8325GpioStruct_t gpioConf;
    rtems_libio_ioctl_args_t ctl;
    ctl.command = WM8325_MODE_SET_GPIO;
    ctl.buffer = (void*) &gpioConf;
    gpioConf.gpioMode = WM8325_OUTPUT;
    gpioConf.gpioNr = WM8325_I2C_SELECT_PIN_NO;
    gpioConf.gpioVal = wmPinVal;
    sc = rtems_io_control(wmMajor, wmMinor, &ctl);
    if(sc)
        return DRV_OS_BRD_182_DRV_ERROR;
    ctl.command = WM8325_SET_GPIO;
    sc = rtems_io_control(wmMajor, wmMinor, &ctl);
    if(sc)
        return DRV_OS_BRD_182_DRV_ERROR;
    sc = rtems_io_close(wmMajor, wmMinor, NULL);
    if(sc)
        return DRV_OS_BRD_182_DRV_ERROR;

    // Register I2C devices second phase which may be missing or not
    osBrdMv0182RegisterI2CDevices(i2cDeviceListSecondPhase);

    // register optional sensors on CAMA connector
    registerCamA();

    // register optional sensors on CAMB connector
    registerCamB();

    return DRV_OS_BRD_182_DRV_SUCCESS;
}

/*
 * After one run, MCP3424 (ADC) can remain in a state that
 * hold down the sda line - when suddenly stopped.
 *  When is tried a second run, the i2c2 bus (sda line)
 *  remain low (adc wait for clock to terminate unfinished transaction) .
 *  This function check and and unlock sda line.
 */
static tyOsBrd182ErrorCode checkAndRecoverI2cSdaLines(void)
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

        MV182PRINTF("%s: %d ; %s: %d\n", sclName, sclFd, sdaName, sdaFd);

        int rawVal = 0;
        sc = ioctl(sdaFd, GPIO_GET_RAW, &rawVal);
        if (sc == 0 && rawVal == 0)
        {
            int i;
            int sclPinMode;
            int sdaPinMode;
            int sclPinModeFake = D_GPIO_MODE_7;

            MV182PRINTF("\n%s stuck low, attempting recovery\n",sdaName);

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
    return DRV_OS_BRD_182_DRV_SUCCESS;
}

static tyOsBrd182ErrorCode osBrdMv0182InitialiseGpioDriver(void)
{
    OS_MYRIAD_DRIVER_STATUS_CODE retCode;
    uint32_t gpioMajor;

    retCode = rtems_io_register_driver( 0, &osDrvGpioTblName, &gpioMajor );
    if (retCode != OS_MYR_DRV_SUCCESS)
    {
        MV182PRINTF("gpio register rc = %d\n", retCode);
        return DRV_OS_BRD_182_GPIO_DRIVER_ERROR;
    }

    retCode = rtems_io_initialize(gpioMajor, 0,
                                  (void*)brdMV0182R2R3GpioCfgDefault);
    if (retCode != OS_MYR_DRV_SUCCESS)
    {
        MV182PRINTF("gpio initialzie %d\n", retCode);
        return DRV_OS_BRD_182_GPIO_DRIVER_ERROR;
    }
    return DRV_OS_BRD_182_DRV_SUCCESS;
}


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
// This is patch for MV0182 R4 with MA2x5x as level shifter is always ON
static void osBrdPatchSDIOInterface(void)
{
    // Fix for R4 and MA2x5x.
    SET_REG_BITS_MASK(AON_RETENTION0_ADR, 1 << SDIO_1_8V_INTERFACE_SELECT);
}
#endif

// Extra actions to be taken based on board revision
// TODO Once we have a better mechanism such as arch folder for board driver this has to be moved
static rtems_status_code osBrdPCBRevisionPostActions(tyOsMv0182PcbRevision pcbrev)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    switch(pcbrev) {
        case OS_MV0182_R0R1:
            printf("This MDK SW package does not support Revision 0 (R0) nor Revision 1 (R1) MDK boards.\n");
            break;
        case OS_MV0182_R4:
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
            osBrdPatchSDIOInterface();
#endif
            break;
        // ! GPIO 57 is ON by default
        // case OS_MV0182_R5:
        //     //--------------------------------------------------------------------------------------
        //     if (rtemsGetMajorMinor(OSDRVGPIO_DEVNAME, &major, &minor) != 0)
        //         return RTEMS_IO_ERROR;

        //     sc = rtems_io_open(major,GPIO_57_SDCARD_VOLTAGE , NULL);
        //     if (sc)
        //         return sc;
        //     //set GPIO 57 low, to enable SD Card voltage
        //     gpioValue = GPIO_LOW;
        //     sc = rtems_io_write(major,GPIO_57_SDCARD_VOLTAGE , &gpioValue);
        //     if (sc)
        //         return sc;
        //     //Release
        //     sc = rtems_io_close(major,GPIO_57_SDCARD_VOLTAGE , NULL);
        //     if (sc)
        //         return sc;
        //     break;
            //--------------------------------------------------------------------------------------
        case OS_MV0182_REV_NOT_DETECTED:
            // If board revision cannot be detected
            sc = RTEMS_IO_ERROR;
            break;
        default:
            break;
    }
    return sc;
}

// TODO Make Board driver IO driver to be loaded during the OS start-up & add
//      CTL functions to read PCB version and MAC address
tyOsBrd182ErrorCode osBoard0182Initialise(tyOsBoard0182Configuration* config)
{
    int32_t rv;
    tyOsBrd182ErrorCode returnCode;
    MV182PRINTF("Starting 182 board initialize\n");
    static volatile tyOsBrd182StatusCode boardStatus = DRV_OS_BRD_182_UNINITIALIZED;

    if(boardStatus == DRV_OS_BRD_182_UNINITIALIZED)
    {
        boardStatus = DRV_OS_BRD_182_INITIALIZATION_IN_PROGRESS;

        // Init CPR Driver in order to be able to get sys clk
        rv = OsDrvCprInit();
        switch (rv)
        {
        case OS_MYR_DRV_SUCCESS:
        case OS_MYR_DRV_ALREADY_INITIALIZED:
        case OS_MYR_DRV_ALREADY_OPENED:
            break;
        default:
            boardStatus = DRV_OS_BRD_182_UNINITIALIZED;
            return DRV_OS_BRD_182_CPR_INIT_ERROR;
        }

        // Open CPR Driver in order to be able to get sys clk
        rv = OsDrvCprOpen();
        switch (rv)
        {
        case OS_MYR_DRV_SUCCESS:
        case OS_MYR_DRV_ALREADY_INITIALIZED:
        case OS_MYR_DRV_ALREADY_OPENED:
            break;
        default:
            boardStatus = DRV_OS_BRD_182_UNINITIALIZED;
            return DRV_OS_BRD_182_CPR_OPEN_ERORR;
        }

        // Initialize and start GPIO driver
        // Check if revision is unsupported R1
        returnCode = osBrdMv0182InitialiseGpioDriver();
        if(returnCode)
        {
            boardStatus = DRV_OS_BRD_182_UNINITIALIZED;
            return returnCode;
        }

        // Initialize I2C busses
        returnCode = osBrdMv0182InitialiseI2C(config);
        if(returnCode != DRV_OS_BRD_182_DRV_SUCCESS){
            boardStatus = DRV_OS_BRD_182_UNINITIALIZED;
            return returnCode;
        }
        // Get board revision
        osBrdDetectPCBRevision(&pcbRevision);

        // Check revision code and post actions
        if ((returnCode = osBrdPCBRevisionPostActions(pcbRevision))) {
            boardStatus = DRV_OS_BRD_182_UNINITIALIZED;
            MV182PRINTF("Problem in revision detection\n");
            return returnCode;
        }
        boardStatus = DRV_OS_BRD_182_INITIALIZED;
        MV182PRINTF("182 board initialized successfully\n");
    }
    else
    {
        returnCode = DRV_OS_BRD_182_ALREADY_INITIALIZED;
        MV182PRINTF("182 board already initialized \n");
    }

    return returnCode;
}
