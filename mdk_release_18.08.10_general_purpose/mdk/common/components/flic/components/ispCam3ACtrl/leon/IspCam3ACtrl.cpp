///
/// @file      IspCam3ACtrl.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Implementation for Isp Camera 3A Control
///            Platform(s) supported : ma2x8x
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

#include "IspCam3ACtrl.h"
#include "ic_main.h"
#include <ImgFrame.h>
#include "IspCommon.h"

#include "DrvSensor.h"

#include "IspCfgGenerate.h"
#include "IspCamCtrl.h"


IspCommandT dbgUserCmd = {ISP_CMD__NOP, 0 };


typedef struct {
    const char* name;
    uint32_t id;
    IspCfgGenerate *ispCfgGenerate;
    IspCamCtrl     *ispCamCtrl;
}CameraT;

typedef struct {
    CameraT cam[IPIPE_MAX_SOURCES_ALLOWED];
    const char **cameraNameList;
    uint32_t nrOfCamsInApp;
    pthread_mutex_t cmdMtx;
}IspCam3ACtrl;
IspCam3ACtrl ispCam3ACtrl;


void     CreateIspCam3ACtrl(const char **camsL, uint32_t nrOfCams) {
    memset((void*)&ispCam3ACtrl, 0, sizeof(ispCam3ACtrl));
    ispCam3ACtrl.nrOfCamsInApp = nrOfCams;
    ispCam3ACtrl.cameraNameList = camsL;
    for (uint32_t i = 0; i < nrOfCams; i++) {
        ispCam3ACtrl.cam[i].name = camsL[i];
        ispCam3ACtrl.cam[i].id   = i;
        ispCam3ACtrl.cam[i].ispCamCtrl  = new IspCamCtrl(camsL[i]);
        ispCam3ACtrl.cam[i].ispCfgGenerate   = new IspCfgGenerate();

        //delete[] qBuff;
        //qBuff = NULL;
    }
    assert(0 == pthread_mutex_init(&ispCam3ACtrl.cmdMtx, NULL));
}


void inc_cam_frame_start( void *p_prv, uint32_t sourceInstance, uint32_t seqNo, uint64_t ts, void *userData) {
    UNUSED(p_prv);
    UNUSED(ts);
    UNUSED(seqNo);
    UNUSED(ts);
    UNUSED(userData);
    los_configIsp(ispCam3ACtrl.cam[sourceInstance].ispCfgGenerate->reqCfg(), sourceInstance);
}
void inc_cam_frame_line_reached(void *p_prv, uint32_t sourceInstance, uint32_t seqNo,uint64_t ts, void *userData) {
    UNUSED(p_prv);
    UNUSED(sourceInstance);
    UNUSED(seqNo);
    UNUSED(ts);
    UNUSED(userData);
}

void inc_cam_frame_end(void *p_prv, uint32_t sourceInstance, uint32_t seqNo,uint64_t ts, void *userData) {
    UNUSED(p_prv);
    UNUSED(sourceInstance);
    UNUSED(seqNo);
    UNUSED(ts);
    UNUSED(userData);
}
void inc_cam_terminate_fr( void *p_prv, uint32_t sourceInstance, void *userData) {
    UNUSED(p_prv);
    UNUSED(sourceInstance);
    UNUSED(userData);
}

void incCamIspStart(int32_t ispInstance, int32_t seqNo, uint32_t userData) {
    UNUSED(ispInstance);
    UNUSED(seqNo);
    UNUSED(userData);
}
void inc_cam_ipipe_cfg_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv) {
    UNUSED(p_prv);
    UNUSED(seqNo);
    UNUSED(p_cfg_prv);
}
void incCamIspEnd(int32_t ispInstance, int32_t seqNo, uint32_t userData) {
    UNUSED(ispInstance);
    ispCam3ACtrl.cam[ispInstance].ispCfgGenerate->configConsumed((void*)userData);
    UNUSED(seqNo);
    UNUSED(userData);
}
void inc_cam_stats_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv) {
    UNUSED(p_prv);
    UNUSED(seqNo);
    UNUSED(p_cfg_prv);
}
void inc_cam_ipipe_buff_locked(void *p_prv, void *userData, unsigned int sourceInstance, void *buffZsl, uint64_t ts, unsigned int seqNo) {
    UNUSED(p_prv);
    UNUSED(userData);
    UNUSED(sourceInstance);
    UNUSED(buffZsl);
    UNUSED(ts);
    UNUSED(seqNo);
}
void inc_cam_capture_ready( void *p_prv, unsigned int seqNo, void *p_cfg_prv) {
    UNUSED(p_prv);
    UNUSED(seqNo);
    UNUSED(p_cfg_prv);
}

icSourceSetup* inc_getSourceLimits(uint32_t srtId) {
    if((srtId<ispCam3ACtrl.nrOfCamsInApp)&&(IPIPE_SRC_AVAILABLE==ispCam3ACtrl.cam[srtId].ispCamCtrl->src.sourceStatus)) {
        return &ispCam3ACtrl.cam[srtId].ispCamCtrl->src.sourceSetup;
    }
    else {
        return (icSourceSetup*)NULL;
    }
}

void IspCtrlCmdCalbacks(IspCommandT *command) {
    UNUSED(command);
}


//#############################################
//NOTES:
//1) Guzzi manages config frame pool => no need for a config pool @ wrapper level
//2) Config gets released by Guzzi when it receives IC_EVENT_TYPE_ISP_END
//   => IcIspConfig doesn't need to inherit Message base class ???

// this is interface Guzzi - Ipipe thread.
// In order to allow develop stand alone functionality, another thread will be internally created and will control
// sensors and will generate configs.


void ispCamStart(uint32_t camId) {
    los_start(NULL);
    los_configIsp(ispCam3ACtrl.cam[camId].ispCfgGenerate->reqCfg(), camId);
    los_ConfigureSource(camId, &ispCam3ACtrl.cam[camId].ispCamCtrl->src.sourceConfig, 0);
    los_startSource (camId);
    ispCam3ACtrl.cam[camId].ispCamCtrl->startCam();
}
void ispCamStop(uint32_t camId) {
    los_stopSource(camId);
    los_stop();
    ispCam3ACtrl.cam[camId].ispCamCtrl->stopCam();
}

void IspCam3ACtrlSendCmd(IspCommandT cmd) {
    pthread_mutex_lock(&ispCam3ACtrl.cmdMtx);
    switch (cmd.cmdId) {
    case ISP_CMD_START:
        ispCamStart(cmd.camId);
        break;
    case ISP_CMD_STOP:
        ispCamStop(cmd.camId);
        break;
    default:
        assert(0);
    }
    pthread_mutex_unlock(&ispCam3ACtrl.cmdMtx);
}


void platform_cam_led1_toggle (void) {

}
