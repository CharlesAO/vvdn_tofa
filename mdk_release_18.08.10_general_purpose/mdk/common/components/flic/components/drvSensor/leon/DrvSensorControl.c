///
/// @file      DrvSensorControl.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     DrvSensor control functions for typical I2C sensors
///            Platform(s) supported : ma2x8x
///

#include "leon/DrvSensorControl.h"

#include <stdio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "OsDrvI2cMyr2.h"
#include "leon/DrvSensorDefines.h"

//#define DRV_SENSOR_DEBUG 1
#if DRV_SENSOR_DEBUG
#define FPPRINTF(...) printf(__VA_ARGS__)
#else
#define FPPRINTF(...)
#endif

int drvSensorSensorOpen (struct DrvSensorVideoContext *ctx)
{
    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    if(ctx->ctlName)
    {
        ctx->ctlFd = open(ctx->ctlName, O_RDWR);
        FPPRINTF("%s: %d\n", ctx->ctlName , ctx->ctlFd);
        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);
        if(ctx->ctlFd < 0)
            return ctx->ctlFd;
    }
    else
    {
        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);
        ctx->ctlFd = 0;
    }
    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);
    if(!ctx->ctlFd)
    {
        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);
        return -2;
    }
    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);
    return 0;
}

int drvSensorSensorClose (struct DrvSensorVideoContext *ctx)
{
    int sc;

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    if(ctx->ctlFd)
    {
        sc = close(ctx->ctlFd);
        if(sc)
            return sc;
    }

    if(ctx->ctlFdShared)
    {
        sc = close(ctx->ctlFdShared);
        if(sc)
            return sc;
    }

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    return 0;
}

static int burnI2cConfigurationToCamera(int fd,
                                        const struct DrvSensorCamSpec *spec,
                                        const DrvSensorI2CConfigDescriptor *i2cConfig)
{
    u32 i;
    int rval;

    if (i2cConfig == NULL)
        return -1;

    (void)spec;

    for (i = 0; i < i2cConfig->numberOfRegs; i++)
    {
        struct i2cStruct_t i2cCtl;

        i2cCtl.regAddress = i2cConfig->regValues[i][0];
        i2cCtl.regValue = (void*)&i2cConfig->regValues[i][1];
        i2cCtl.wordCount = 1;

        rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
        if(rval != i2cCtl.wordCount)
        {
            FPPRINTF("I2C sensor configuration failed at address 0x%lx \n"
                            "        with errcode %d \n", (u32)i2cCtl.regAddress, rval);
            i-=1;
            continue;
        }
        else
        {
            FPPRINTF("I2C sensor wrote address 0x%lx with 0x%x \n", (u32)i2cCtl.regAddress, *(u8*)i2cCtl.regValue);
        }
    }

    if  (i2cConfig->delayMs)
        usleep(i2cConfig->delayMs * 1000);

    return 0;
}


static int searchAndBurnI2cConfigsByType(int fd,
                                  const struct DrvSensorCamSpec* spec,
                                  enum DrvSensori2cConfigType_t configType)
{
    s32 rval = 0;
    u32 i;

    for(i = 0 ; i < spec->nbOfI2CConfigSteps ; i++)
    {
        if(spec->i2cConfigSteps[i].configType == configType)
        {
            rval = burnI2cConfigurationToCamera(fd, spec,
                                                &spec->i2cConfigSteps[i]);
            if(rval)
                return rval;
        }
    }

    return rval;
}

int drvSensorSensorConfig (struct DrvSensorVideoContext *ctx,
                           struct DrvSensorCotrolConfig *control)
{
    int fd;

    if(FP_IS_SHARED_CTL(control->cfgType))
    {
        fd = ctx->ctlFdShared;
    }
    else
    {
        fd = ctx->ctlFd;
    }

    switch (FP_STRIP_CTL(control->cfgType))
    {
    case DRV_SENSOR_START:
        FPPRINTF("START %s: %s:%d\n", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(fd, ctx->spec, DRV_SENSOR_START_STREAMING);
        break;
    case DRV_SENSOR_STOP:
        FPPRINTF("STOP %s: %s:%d\n", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(fd, ctx->spec, DRV_SENSOR_STOP_STREAMING);
        break;
    case DRV_SENSOR_CONFIG_SEND_CONFIG:
        FPPRINTF("CONFIG %s: %s:%d\n", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(fd, ctx->spec, DRV_SENSOR_FULL_CONFIG);
        break;
    case DRV_SENSOR_CONFIG_TESTPATTERN:
        FPPRINTF("TESTPATTERN %s: %s:%d\n", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(ctx->ctlFd, ctx->spec, DRV_SENSOR_COLORBAR);
        break;
    default:
        return RTEMS_INVALID_NUMBER;
    }

    return 0;
}

int drvSensorSensorPowerDown (struct DrvSensorVideoContext *ctx)
{
    (void)ctx;
    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);
    return 0;
}

int drvSensorSensorPowerUp (struct DrvSensorVideoContext *ctx)
{
    (void)ctx;
    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);
    return 0;
}


struct DrvSensorControlFunctions drvSensorDefaultControl =
{
    .open = drvSensorSensorOpen,
    .close = drvSensorSensorClose,
    .config = drvSensorSensorConfig,
    .power_up = drvSensorSensorPowerUp,
    .power_down = drvSensorSensorPowerDown,
};
