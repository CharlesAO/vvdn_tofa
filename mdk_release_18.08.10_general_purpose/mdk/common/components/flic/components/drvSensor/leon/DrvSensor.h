///
/// @file      DrvSensor.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     DrvSensor header.
///

#ifndef DRVSENSOR_H__
#define DRVSENSOR_H__

#include <rtems.h>
#include <OsDrvI2cMyr2.h>
#include <mv_types.h>
#include <swcFrameTypes.h>
#include <DrvMipi.h>
#include "DrvSensorDefines.h"

#define FP_SHARED_CTL_ID      0x100

#define FP_SHARED_CTL(x)    ((x) | FP_SHARED_CTL_ID)
#define FP_IS_SHARED_CTL(x) ((x) & FP_SHARED_CTL_ID)
#define FP_STRIP_CTL(x)     ((x) & 0xFF)

#define DRV_SENSOR_START            2
#define DRV_SENSOR_STOP             3

#define DRV_SENSOR_CONFIG_SEND_CONFIG   1

#define DRV_SENSOR_CONFIG_SEND_EXPOSURE 4
#define DRV_SENSOR_CONFIG_SEND_GAIN     5
#define DRV_SENSOR_CONFIG_TESTPATTERN   6
#define DRV_SENSOR_GET_TESTPATERN_CRC   7
#define DRV_SENSOR_GET_FRAME_SPEC       9
#define DRV_SENSOR_SET_FRAME_SPEC       10 // NOT IMPLEMENTED YET
#define DRV_SENSOR_GET_FRAME_RATE       11
#define DRV_SENSOR_SET_FRAME_RATE       12

// TODO: REWORK
#define DRV_SENSOR_GET_CAM_SPEC          18
#define DRV_SENSOR_GET_CAM_HW_CONEC_SPEC 19

#define DRV_SENSOR_QUEUE_FRAMEBUFFER   10
#define DRV_SENSOR_DEQUEUE_FRAMEBUFFER 11

typedef enum mipiRxCtrlRecNoT{
    SIPP_DEVICE0 = 0,
    SIPP_DEVICE1 = 1,
    SIPP_DEVICE2 = 2,
    SIPP_DEVICE3 = 3,
    CIF0_DEVICE4 = 4,
    CIF1_DEVICE5 = 5
}eMipiRxCtrlRecNoT;

struct DrvSensorHwContext
{
    u32 irqVector;
    u32 clk;
    enum drvMipiCtrlNo ctrlNo;
    enum mipiRxCtrlRecNoT ctrlRecNo;
    u32 frameCount;
};

struct DrvSensorCotrolConfig {
    u32 cfgType;
    void *buffer;
};

struct DrvSensorGpioDescriptor
{
    char * name;
    u8 gpioNumber;
    enum DrvSensorgpioConfigType_t configType;
    u8 activeLevel;
    u32 delayUs;
};


struct DrvSensorVideoContext
{
    const char *ctlName;
    int ctlFd;

    const char *ctlNameShared;
    int ctlFdShared;

    struct DrvSensorControlFunctions *ctlFnc;

    struct DrvSensorGpioDescriptor *gpios;

    struct DrvSensorHwContext hwCtx;

    const struct DrvSensorCamSpec *spec;

};

struct DrvSensorVideoDevice {
    char *name;

    char *controlDevice;
// TODO: Review this side, i2c initialization and sensor identification
    int  *i2cBusControlId;
    char *i2cBusName;
    char *i2cControlDeviceName;
    unsigned i2cAddress;
    rtems_libi2c_drv_t *i2cProtocolAdrTbl;

    char *sharedControlDevice;
    int  i2cBusSharedControl;
    struct DrvSensorControlFunctions *ctlFnc;

    const frameSpec spec;

    u32 interruptLevel;
};

struct DrvSensorVideoDeviceSensor {
    struct DrvSensorVideoDevice vd;

    enum drvMipiCtrlNo ctrlNo;
    enum mipiRxCtrlRecNoT ctrlRecNo;

    struct DrvSensorGpioDescriptor *gpioa;
    const struct DrvSensorCamSpec *spec;
};

struct DrvSensorControlFunctions {
    int (*open) (struct DrvSensorVideoContext *);
    int (*close) (struct DrvSensorVideoContext *);
    int (*config) (struct DrvSensorVideoContext *,
                    struct DrvSensorCotrolConfig *);
    int (*power_up) (struct DrvSensorVideoContext *);
    int (*power_down) (struct DrvSensorVideoContext *);
};


extern rtems_driver_address_table DrvSensorDrvTblName;

#endif //DRVSENSOR_H__
