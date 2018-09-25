///
/// @file      IspCam3ACtrl.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Header for Isp Camera 3A Control
///

#ifndef __ISP_CAM_3A_CTRL_H__
#define __ISP_CAM_3A_CTRL_H__



//#include <Flic.h>
//#include <Receiver.h>
//#include <Sender.h>
#include "ic_main.h"

#ifdef __cplusplus
extern "C" {
#endif

void     CreateIspCam3ACtrl(const char **camsL, uint32_t nrOfCams);


void inc_cam_frame_start( void *p_prv, uint32_t sourceInstance, uint32_t seqNo, uint64_t ts, void *userData);
void inc_cam_frame_line_reached(void *p_prv, uint32_t sourceInstance, uint32_t seqNo,uint64_t ts, void *userData);
void inc_cam_frame_end(void *p_prv, uint32_t sourceInstance, uint32_t seqNo,uint64_t ts, void *userData);
void inc_cam_terminate_fr( void *p_prv, uint32_t sourceInstance, void *userData);

void inc_cam_stats_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv);
void inc_cam_ipipe_cfg_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv);
// TODO: replace this, is not useful for 3a control to know when the frame is processed
void incCamIspStart(int32_t ispInstance, int32_t seqNo, uint32_t userData);
void incCamIspEnd(int32_t ispInstance, int32_t seqNo, uint32_t userData);


void inc_cam_ipipe_buff_locked(void *p_prv, void *userData, unsigned int sourceInstance,void *buffZsl, uint64_t ts, unsigned int seqNo);
void inc_cam_capture_ready(void *p_prv, unsigned int seqNo, void *p_cfg_prv);


//void inc_cam_terminate_fr( void *p_prv, uint32_t sourceInstance, void *userData);


icSourceSetup* inc_getSourceLimits(uint32_t srtId);

// cam controll comands from the user interface, can be extended on user application side, by adding
// new command, create another switch case, with will have on default branch this callback
#define    ISP_CMD__NOP     0
#define    ISP_CMD_START    1
#define    ISP_CMD_STOP     2
#define    ISP_CMD_CAPTURE  3

typedef struct {
    uint32_t cmdId;
    uint32_t camId;
} IspCommandT;

void IspCam3ACtrlSendCmd(IspCommandT cmd);

void platform_cam_led1_toggle (void);

#ifdef __cplusplus
}
#endif


#endif //__ISP_CAM_3A_CTRL_H__
