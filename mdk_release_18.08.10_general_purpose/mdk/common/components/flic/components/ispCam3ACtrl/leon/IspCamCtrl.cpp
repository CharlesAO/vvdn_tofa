///
/// @file      IspCamCtrl.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Implementation for Isp Camera Control
///            Platform(s) supported : ma2x8x
/// @ author   truicam
///

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <bsp.h>
#include <rtems.h>
#include <rtems/libio.h>
#include <assert.h>
#include "IspCamCtrl.h"

IspCamCtrl::IspCamCtrl(const char *cam) {
    // TODO Auto-generated constructor stub
    name = cam;
    initSourceConfig();
}

IspCamCtrl::~IspCamCtrl() {
    // TODO Auto-generated destructor stub
}


void IspCamCtrl::startCam(void) {
    int sc;
    sc = ioctl(fileDesc, DRV_SENSOR_START);
    if(-1 == sc) {
        perror("failed to set start sensor !!!");
    }
}
void IspCamCtrl::stopCam(void) {
    int sc;
    sc = ioctl(fileDesc, DRV_SENSOR_STOP);
    if(-1 == sc) {
        perror("failed to set start sensor !!!");
    }
}

//###################################################################################
//This would come from Guzzi via IC_EVENT_TYPE_CONFIG_SOURCE message
void IspCamCtrl::initSourceConfig()
{
    int fd, sc;
    struct DrvSensorCamSpec drvSensorCamSpec;
    struct DrvSensorHwContext drvSensorHwContext;

    printf("Openning camera %s.... \n", name);
    fd = open(name, O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    printf("Camera opened .... \n");
    if(fd < 0) {
        printf("Failed to open camera %s : %s \n", name, strerror(errno));
        assert(0);
    }
    fileDesc = fd;
    sc = ioctl(fileDesc, DRV_SENSOR_CONFIG_SEND_CONFIG);
    if(sc){ perror("failed to set testpattern !!!");}

    // get config from camera
    sc = ioctl(fileDesc, DRV_SENSOR_GET_CAM_SPEC, &drvSensorCamSpec);
    if(sc){ perror("failed to get config !!!");}
    // get board information about mipi controler and receiver id
    sc = ioctl(fileDesc, DRV_SENSOR_GET_CAM_HW_CONEC_SPEC, &drvSensorHwContext);
    if(sc){ perror("failed to get connection config !!!");}

    // construct PlgSourceConfig
    src.sourceConfig.bayerFormat             = (icBayerFormat)drvSensorCamSpec.internalPixelFormat;
    src.sourceConfig.bitsPerPixel            = drvSensorCamSpec.bitsPerPixel;
    src.sourceConfig.cameraOutputSize.w      = drvSensorCamSpec.outSize.w;
    src.sourceConfig.cameraOutputSize.h      = drvSensorCamSpec.outSize.h;
    src.sourceConfig.cropWindow.x1           = drvSensorCamSpec.outFOV.x1;
    src.sourceConfig.cropWindow.x2           = drvSensorCamSpec.outFOV.x2;
    src.sourceConfig.cropWindow.y1           = drvSensorCamSpec.outFOV.y1;
    src.sourceConfig.cropWindow.y2           = drvSensorCamSpec.outFOV.y2;
    src.sourceConfig.mipiRxData.controllerNo = (IcMipiRxCtrlNoT)drvSensorHwContext.ctrlNo;
    src.sourceConfig.mipiRxData.dataMode     = drvSensorCamSpec.mipiCfg->dataMode;
    src.sourceConfig.mipiRxData.dataType     = (IcMipiRxDataTypeT)drvSensorCamSpec.mipiCfg->pixelFormat;
    src.sourceConfig.mipiRxData.laneRateMbps = drvSensorCamSpec.mipiCfg->dataRateMbps;
    src.sourceConfig.mipiRxData.noLanes      = drvSensorCamSpec.mipiCfg->nbOflanes;
    src.sourceConfig.mipiRxData.recNrl       = (IcMipiRxCtrlRecNoT)drvSensorHwContext.ctrlRecNo;

    src.sourceSetup.maxWidth  = drvSensorCamSpec.outSize.w;
    src.sourceSetup.maxHeight = drvSensorCamSpec.outSize.h;
    src.sourceSetup.maxPixels = drvSensorCamSpec.outSize.w * drvSensorCamSpec.outSize.h;
    src.sourceSetup.maxHorizD = 1;
    src.sourceSetup.maxHorizN = 1;
    src.sourceSetup.maxVertD  = 1;
    src.sourceSetup.maxVertN  = 1;
    src.sourceSetup.maxBpp    = 16;

    src.sourceStatus = IPIPE_SRC_AVAILABLE;
    //DrvSensorHwContext
}
