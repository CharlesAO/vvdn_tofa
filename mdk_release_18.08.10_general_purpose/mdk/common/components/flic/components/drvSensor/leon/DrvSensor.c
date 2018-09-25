///
/// @file      DrvSensor.c
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     DrvSensor main API implementation
///            Platform(s) supported : ma2x8x
///

#include "leon/DrvSensor.h"

#include <malloc.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#include <bsp.h>
#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/libi2c.h>

#include <DrvLeonL2C.h>
#include <swcLeonUtils.h>

#ifndef MAX_DRV_SENSOR_CAMS
#define MAX_DRV_SENSOR_CAMS 8
#endif

#ifndef FP_QUEUED_FRAMES
#define FP_QUEUED_FRAMES 100
#endif

#define __CACHE_LINE_SIZE 64

//#define DRV_SENSOR_DEBUG 1
#if DRV_SENSOR_DEBUG
#define FPPRINTF(...) printf(__VA_ARGS__)
#else
#define FPPRINTF(...)
#endif

static struct DrvSensorVideoContext *fpContext[MAX_DRV_SENSOR_CAMS];

static int drvSensorGetFrameSpec (frameSpec* pFrameSpec, struct DrvSensorVideoContext *ctx);
static int drvSensorGetCamSpec (struct DrvSensorCamSpec* pCamSpec, struct DrvSensorVideoContext *ctx);
static int drvSensorGetCamHwConectionSpec (struct DrvSensorHwContext* pCamSpec, struct DrvSensorVideoContext *ctx);


rtems_device_driver DrvSensorInitialize(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    (void)minor;

    if(e == NULL) {
        return RTEMS_SUCCESSFUL;
    }
    else {
        rtems_status_code sc;
        int minor_cam;
        static int lastDevice = 0;
        struct DrvSensorVideoDeviceSensor * fpvds = (struct DrvSensorVideoDeviceSensor*)e;
        struct DrvSensorVideoDevice * fpvd = &fpvds->vd;

        struct DrvSensorVideoContext * ctx = (struct DrvSensorVideoContext *)
            malloc(sizeof(struct DrvSensorVideoContext));
        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

        if(fpvd->ctlFnc == NULL) {
            sc = RTEMS_INVALID_ADDRESS;
            printf("Could not register %s peripheral Invalid ctlFnc !!\n", fpvd->name);
            goto cleanUp;
        }
        ctx->ctlFnc = fpvd->ctlFnc;
        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

        minor_cam = rtems_libi2c_register_drv(fpvd->i2cControlDeviceName,
                                          fpvd->i2cProtocolAdrTbl,
                                          *fpvd->i2cBusControlId,
                                          fpvd->i2cAddress);

        if(minor_cam < 0) {
            sc = RTEMS_INVALID_ADDRESS;
            printf("Could not register %s peripheral !!\n", fpvd->i2cControlDeviceName);
            goto cleanUp;
        }


        if(fpvd->controlDevice == NULL) {
            sc = RTEMS_INVALID_ADDRESS;
            printf("Could not register %s peripheral Invalid controlDevice !!\n", fpvd->name);
            goto cleanUp;
        }
        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

        // Check if control devices are registered (probing for cameras)
        FPPRINTF("DrvSensor %s stat = %d \n", fpvd->controlDevice, 0);
        //printf("DrvSensor %s stat = %d \n", fpvd->controlDevice, 0);

        if((fpvd->controlDevice && stat(fpvd->controlDevice, NULL)) ) {
            sc = RTEMS_NOT_DEFINED;
            printf("Could not register %s peripheral Invalid stat !!\n", fpvd->name);
            goto cleanUp;
        }
        ctx->ctlName = fpvd->controlDevice;
        ctx->ctlNameShared = fpvd->sharedControlDevice;

        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

        if(NULL == fpvds->spec) {
            sc = RTEMS_INVALID_ADDRESS;
            printf("Could not register %s peripheral Invalid spec !!\n", fpvd->name);
            goto cleanUp;
        }
        ctx->spec = fpvds->spec;
        FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

        // hw contex update
        ctx->hwCtx.ctrlNo = fpvds->ctrlNo;
        ctx->hwCtx.ctrlRecNo = fpvds->ctrlRecNo;

        FPPRINTF("%s: %d:%d\n", fpvd->name, (int)major, (int)lastDevice);
        //register with the minor number from virtual channel
        sc = rtems_io_register_name(fpvd->name, major, lastDevice);
        if (sc)
            return sc;
        fpContext[lastDevice++] = ctx;

        return 0;

    cleanUp:
        free(ctx);
        return sc;
    }
}

rtems_device_driver DrvSensorOpen(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    rtems_status_code sc;
    struct DrvSensorCotrolConfig cfg;
    struct DrvSensorVideoContext * ctx = fpContext[(int)minor];
    if(ctx == NULL)
        return RTEMS_NOT_DEFINED;
    (void)major;
    (void)e;

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    sc = ctx->ctlFnc->open(ctx);
    if(sc)
        return sc;

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);


    cfg.cfgType = DRV_SENSOR_STOP;
    ctx->ctlFnc->config(ctx, &cfg);

    cfg.cfgType = DRV_SENSOR_CONFIG_SEND_CONFIG;
    ctx->ctlFnc->config(ctx, &cfg);

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    return 0;
}

rtems_device_driver DrvSensorClose(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    int sc;
    struct DrvSensorCotrolConfig cfg;
    struct DrvSensorVideoContext * ctx = fpContext[(int)minor];
    if(ctx == NULL)
        return RTEMS_NOT_DEFINED;
    (void)major;
    (void)e;

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    cfg.cfgType = DRV_SENSOR_STOP;
    ctx->ctlFnc->config(ctx, &cfg);

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    sc = ctx->ctlFnc->close(ctx);
    if(sc)
        return sc;

    FPPRINTF("%s: %s:%d\n", __FILE__ , __func__, __LINE__);

    return 0;
}


rtems_device_driver DrvSensorRead(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);
    return 0;
}

rtems_device_driver DrvSensorWrite(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    UNUSED(major);
    UNUSED(minor);
    UNUSED(e);
    return 0;
}

rtems_device_driver DrvSensorControl(
    rtems_device_major_number major,
    rtems_device_minor_number minor,
    void *e)
{
    int sc = RTEMS_SUCCESSFUL;
    struct DrvSensorCotrolConfig cfg;
    rtems_libio_ioctl_args_t * ctl = (rtems_libio_ioctl_args_t *) e;
    struct DrvSensorVideoContext * ctx = fpContext[(int)minor];

    if(ctx == NULL)
        return RTEMS_NOT_DEFINED;
    if(ctl == NULL)
        return RTEMS_INVALID_ADDRESS;

    (void)major;

    FPPRINTF("CONTROL %d %x!!!!\n", (int)ctl->command , (unsigned int)ctl->buffer);

    // Try to check if it's a sensor driver control command
    cfg.cfgType = ctl->command;
    cfg.buffer = ctl->buffer;
    sc = ctx->ctlFnc->config(ctx, &cfg);
    if(sc != RTEMS_INVALID_NUMBER)
    {
        ctl->ioctl_return = 0;
        return sc;
    }

    switch (FP_STRIP_CTL(ctl->command)) {
    case DRV_SENSOR_GET_FRAME_SPEC:
        sc = drvSensorGetFrameSpec(ctl->buffer, ctx);
        break;
    case DRV_SENSOR_GET_CAM_SPEC:
        sc = drvSensorGetCamSpec (ctl->buffer, ctx);
        break;
    case DRV_SENSOR_GET_CAM_HW_CONEC_SPEC:
        sc = drvSensorGetCamHwConectionSpec(ctl->buffer, ctx);
        break;
    default:
        sc = RTEMS_INVALID_NUMBER;
        break;
    }

    ctl->ioctl_return = (int)sc;
    return sc;
}

static int drvSensorGetFrameSpec (frameSpec* pFrameSpec, struct DrvSensorVideoContext *ctx)
{
    if (!pFrameSpec) {
        return RTEMS_INVALID_ADDRESS;
    }

    pFrameSpec->width   = ctx->spec->outSize.w;
    pFrameSpec->height  = ctx->spec->outSize.h;
    pFrameSpec->stride  = ctx->spec->outSize.w * ctx->spec->bytesPerPixel;
    pFrameSpec->bytesPP = ctx->spec->bytesPerPixel;
    pFrameSpec->type    = ctx->spec->internalPixelFormat;
    return 0;
}
//DrvSensorCamSpec
static int drvSensorGetCamSpec (struct DrvSensorCamSpec* pCamSpec, struct DrvSensorVideoContext *ctx)
{
    if (!pCamSpec) {
        return RTEMS_INVALID_ADDRESS;
    }
    memcpy(pCamSpec, ctx->spec, sizeof(struct DrvSensorCamSpec));
    return 0;
}

//DrvSensorCamSpec
static int drvSensorGetCamHwConectionSpec (struct DrvSensorHwContext* pCamSpec, struct DrvSensorVideoContext *ctx)
{
    if (!pCamSpec) {
        return RTEMS_INVALID_ADDRESS;
    }
    memcpy(pCamSpec, &ctx->hwCtx, sizeof(struct DrvSensorHwContext));
    return 0;
}


rtems_driver_address_table DrvSensorDrvTblName = {
    .initialization_entry = DrvSensorInitialize,
    .open_entry           = DrvSensorOpen,
    .close_entry          = DrvSensorClose,
    .read_entry           = DrvSensorRead,
    .write_entry          = DrvSensorWrite,
    .control_entry        = DrvSensorControl
};
