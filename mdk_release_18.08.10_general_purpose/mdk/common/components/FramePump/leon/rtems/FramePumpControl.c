///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief FramePump control functions for typical I2C sensors
///

#include <FramePumpControl.h>
#include <stdio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <FramePumpDefines.h>

#ifdef MA2X8X
#include "FramePumpGenericI2CDriver.h"
#else
#include "OsDrvI2cMyr2.h"
#endif

#define MVLOG_UNIT_NAME FramePumpControl
#include <mvLog.h>

rtems_status_code framePumpSensorOpen (struct FramePumpVideoContext *ctx)
{
    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    if (ctx->ctlName) {
        ctx->ctlFd = open(ctx->ctlName, O_RDWR);
        mvLog(MVLOG_DEBUG, "%s: %d\n", ctx->ctlName , ctx->ctlFd);
        if (ctx->ctlFd < 0) {
            return ctx->ctlFd;
        }
    } else {
        ctx->ctlFd = 0;
    }

    if (ctx->ctlNameShared) {
        ctx->ctlFdShared = open(ctx->ctlNameShared, O_RDWR);
        mvLog(MVLOG_DEBUG, "%s: %d\n", ctx->ctlNameShared , ctx->ctlFdShared);
        if (ctx->ctlFdShared < 0) {
            return ctx->ctlFdShared;
        }
    } else {
        ctx->ctlFdShared = 0;
    }

    if (!ctx->ctlFdShared && !ctx->ctlFd) {
        return RTEMS_INVALID_NUMBER;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code framePumpSensorClose (struct FramePumpVideoContext *ctx)
{
    rtems_status_code sc = RTEMS_SUCCESSFUL;

    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    if (ctx->ctlFd) {
        sc = close(ctx->ctlFd);
        if (sc != RTEMS_SUCCESSFUL) {
            return sc;
        }
    }

    if (ctx->ctlFdShared) {
        sc = close(ctx->ctlFdShared);
        if (sc != RTEMS_SUCCESSFUL) {
            return sc;
        }
    }

    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    return RTEMS_SUCCESSFUL;
}

static rtems_status_code burnI2cConfigurationToCamera (
    int fd,
    const struct FramePumpSensorDriver *spec,
    const FramePumpI2CConfigDescriptor *i2cConfig)
{
    uint32_t i = 0;
    int rval = 0;
    struct i2cStruct_t i2cCtl = {0};

    if (i2cConfig == NULL) {
        return RTEMS_INVALID_ADDRESS;
    }

    (void)spec;

    for (i = 0; i < i2cConfig->numberOfRegs; i++) {
        i2cCtl.regAddress = i2cConfig->regValues[i][0];
        i2cCtl.regValue = (void*)&i2cConfig->regValues[i][1];
        i2cCtl.wordCount = 1;

        rval = ioctl(fd, CTL_WRITE_REGISTER, &i2cCtl);
        if (rval != i2cCtl.wordCount) {
            mvLog(MVLOG_WARN,
                  "I2C sensor configuration failed at address 0x%lx \n"
                  " with errcode %d \n", (uint32_t)i2cCtl.regAddress, rval);
            i-=1;
            continue;
        } else {
            mvLog(MVLOG_DEBUG, "I2C sensor %d wrote address 0x%lx with 0x%x \n",
                  fd, (uint32_t)i2cCtl.regAddress, *(uint8_t*)i2cCtl.regValue);
        }
    }

    if (i2cConfig->delayMs) {
        usleep(i2cConfig->delayMs * 1000);
    }

    return RTEMS_SUCCESSFUL;
}


static rtems_status_code searchAndBurnI2cConfigsByType (
    int fd,
    const struct FramePumpSensorDriver* spec,
    enum FramePumpi2cConfigType_t configType)
{
    int32_t rval = 0;
    uint32_t i = 0;

    for (i = 0 ; i < spec->nbOfI2CConfigSteps ; i++) {
        if (spec->i2cConfigSteps[i].configType == configType) {
            rval = burnI2cConfigurationToCamera(fd, spec,
                                                &spec->i2cConfigSteps[i]);
            if (rval) {
                return rval;
            }
        }
    }

    return rval;
}

rtems_status_code framePumpSensorConfig (
    struct FramePumpVideoContext *ctx,
    struct FramePumpControlConfig *control)
{
    int fd = 0;

    if (FP_IS_SHARED_CTL(control->cfgType) && ctx->ctlFdShared != 0) {
        fd = ctx->ctlFdShared;
    } else {
        fd = ctx->ctlFd;
    }

    switch (FP_STRIP_CTL(control->cfgType)) {
    case FRAME_PUMP_CONTROL_START:
        mvLog(MVLOG_DEBUG, "START %s: %s:%d", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(fd, ctx->spec, FRAME_PUMP_START_STREAMING);
        break;
    case FRAME_PUMP_CONTROL_STOP:
        mvLog(MVLOG_DEBUG, "STOP %s: %s:%d", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(fd, ctx->spec, FRAME_PUMP_STOP_STREAMING);
        break;
    case FRAME_PUMP_CONFIG_SEND_CONFIG:
        mvLog(MVLOG_DEBUG, "CONFIG %s: %s:%d", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(fd, ctx->spec, FRAME_PUMP_FULL_CONFIG);
        break;
    case FRAME_PUMP_CONFIG_TESTPATTERN:
        mvLog(MVLOG_DEBUG, "TESTPATTERN %s: %s:%d", __FILE__ , __func__, __LINE__);
        searchAndBurnI2cConfigsByType(ctx->ctlFd, ctx->spec, FRAME_PUMP_COLORBAR);
        break;
    case FRAME_PUMP_CONFIG_ENABLE_METADATA:
        mvLog(MVLOG_INFO, "METADATA %s: %s:%d", __FILE__ , __func__, __LINE__);
        if (ctx->spec->metaROI.lines != 0 || ctx->spec->metaROI.columns != 0) {
            searchAndBurnI2cConfigsByType(ctx->ctlFd, ctx->spec, FRAME_PUMP_ENABLE_METADATA);
            ctx->outSpec.hasMetadata = 1;
        }
        break;
    default:
        return RTEMS_INVALID_NUMBER;
    }

    return RTEMS_SUCCESSFUL;
}

rtems_status_code framePumpSensorPowerDown (struct FramePumpVideoContext *ctx)
{
    (void)ctx;
    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);
    return RTEMS_SUCCESSFUL;
}

rtems_status_code framePumpSensorPowerUp (struct FramePumpVideoContext *ctx)
{
    (void)ctx;
    mvLog(MVLOG_DEBUG, "%s: %s:%d\n", __FILE__ , __func__, __LINE__);
    return RTEMS_SUCCESSFUL;
}


struct FramePumpControlFunctions framePumpDefaultControl =
{
    .open = framePumpSensorOpen,
    .close = framePumpSensorClose,
    .config = framePumpSensorConfig,
    .power_up = framePumpSensorPowerUp,
    .power_down = framePumpSensorPowerDown,
};
