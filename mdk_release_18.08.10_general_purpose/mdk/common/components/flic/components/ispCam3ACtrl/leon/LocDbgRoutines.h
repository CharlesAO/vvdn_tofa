///
/// @file      LocDbgRoutines.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Header for LocDbgRoutines.
///

/*
 * LocDbgRoutines.h
 *
 *  Created on: Apr 12, 2017
 */

#ifndef COMPONENTS_FLIC_COMPONENTS_ISPINTERFACE_LEON_LOCDBGROUTINES_H_
#define COMPONENTS_FLIC_COMPONENTS_ISPINTERFACE_LEON_LOCDBGROUTINES_H_

#include "stdio.h"

#ifdef APP_SPECIFC_INTERFACE

// user can create on application level what he need
 // this is an overwrite over some debugs call from ic_main.c

#else

// this is some default interface, stand alone
#include "IspCam3ACtrl.h"

#define APP_SPEC_DEFS01
#define APP_SPEC_DEFS02

#define INSTANCES_COUNT_MAX     6

static inline void doNothing(int a, int b, int c) {
    UNUSED(a);UNUSED(b);UNUSED(c);
}
static inline void doPrint(int a, int b, int c) {
    printf("lg: %d, %d, %d \n", a, b, c);
    //UNUSED(a);UNUSED(b);UNUSED(c);
}
#define _PROFILE_ADD(ID, V1, V2) doNothing((int)ID, (int)V1, (int)V2)
//#define PROFILE_ADD(ID, V1, V2) doPrint((int)ID, (int)V1, (int)V2)

#define APP_DBG_ERROR(X, Y, ...)


#define DL_LAYER(x)    (1 << (x))

#define DL_FATAL       DL_LAYER(0)
#define DL_ERROR       DL_LAYER(1)
#define DL_WARNING     DL_LAYER(2)
#define DL_FUNC        DL_LAYER(3)
#define DL_MESSAGE     DL_LAYER(4)
#define DL_PRINT       DL_LAYER(5)

enum {
    PROFILE_ID_SKIPPED,
    PROFILE_ID_APPLICATION_START = 0x0100,
    PROFILE_ID_SENSOR = 0x0200,
    PROFILE_ID_ALGOS = 0x0300,
    PROFILE_ID_LRT = 0x0400,
    PROFILE_ID_EXT_COMMAND = 0x0500,
    PROFILE_ID_LOS = 0x0600,
    PROFILE_ID_DTP = 0x0700,
    PROFILE_ID_SYS = 0x0800,
};

enum {
    PROFILE_ID_GUZZI_CAMERA3_CAP_REQ = PROFILE_ID_APPLICATION_START,

    PROFILE_ID_SENSOR_EXP_GAIN = PROFILE_ID_SENSOR,
    PROFILE_ID_SENSOR_WRITE_SETTINGS,
    PROFILE_ID_SENSOR_READ_FRAME,
    PROFILE_ID_SENSOR_LIGHT,
    PROFILE_ID_SENSOR_LENS,
    PROFILE_ID_I2C_TRANSACTION_WR,
    PROFILE_ID_I2C_TRANSACTION_RD,
    PROFILE_ID_CAMERA_TIME,
    PROFILE_ID_SENSOR_TIME,
    PROFILE_ID_SENSOR_STOP,
    PROFILE_ID_SENSOR_STREAM_OFF,

    PROFILE_ID_ALGOS_CL_EXP_GAIN = PROFILE_ID_ALGOS,
    PROFILE_ID_ALGOS_ACA_EXP_GAIN,
    PROFILE_ID_ALGOS_AE_RUN,
    PROFILE_ID_ALGOS_AF_RUN,
    PROFILE_ID_ALGOS_AFD_RUN,
    PROFILE_ID_ALGOS_AWB_RUN,
    PROFILE_ID_ALGOS_FMV_RUN,
    PROFILE_ID_ALGOS_AF_INPUT,
    PROFILE_ID_ALGOS_AE_RUN_INPUT,

    PROFILE_ID_LRT_STATS_READY = PROFILE_ID_LRT,
    PROFILE_ID_LRT_END_FRAME,
    PROFILE_ID_LRT_START_FRAME,
    PROFILE_ID_LRT_LINE_REACHED,
    PROFILE_ID_LRT_ISP_START,
    PROFILE_ID_LRT_ISP_END,
    PROFILE_ID_LRT_ISP_CFG_ACEPT,
    PROFILE_ID_LRT_ZSL_LOCKED,
    PROFILE_ID_LRT_SEND_DATA,
    PROFILE_ID_LRT_DATA_SENT,
    PROFILE_ID_LRT_ERROR,

    PROFILE_ID_EXT_START_CMD= PROFILE_ID_EXT_COMMAND,
    PROFILE_ID_EXT_CAPTURE_CMD,
    PROFILE_ID_EXT_STOP_CMD,
    PROFILE_ID_EXT_LENS_MOVE,

    PROFILE_ID_LOS_TRIGGER_CAPTURE = PROFILE_ID_LOS,
    PROFILE_ID_LOS_LOCK_ZSL,
    PROFILE_ID_LOS_START_SRC,
    PROFILE_ID_LOS_CONFIG_ISP,
    PROFILE_ID_LOS_START,
    PROFILE_ID_LOS_STOP,
    PROFILE_ID_LOS_GOB_CONFIG,
    PROFILE_ID_LOS_SRC_CONFIG,
    PROFILE_ID_LOS_SRC_STOP,
    PROFILE_ID_LOS_SRC_SART,

    PROFILE_ID_DTP_VPIPE = PROFILE_ID_DTP,
    PROFILE_ID_DTP_VPIPE_LSC,
    PROFILE_ID_DTP_VPIPE_RGB2RGB,
    PROFILE_ID_DTP_VPIPE_GAMMA,
    PROFILE_ID_DTP_IPIPE_CONVERT,

    PROFILE_ID_SYS_CL_PROCESS = PROFILE_ID_SYS,
    PROFILE_ID_SYS_CAM_PROCESS,
    PROFILE_ID_SYS_ACA_PROCESS,
    PROFILE_ID_SYS_FR_INFO_PROCESS,

};

static inline int getSrcLimits(uint32_t srcId, icSourceSetup* srcSet) {
    icSourceSetup* srcLimits = inc_getSourceLimits(srcId);
    if(srcLimits) {
        memcpy(srcSet, srcLimits, sizeof(icSourceSetup));
        return 0;
    }
    return -1;
}

#define UPDATE_SRC_LIMITS(SRC_ID,RETERN_VAL) getSrcLimits(SRC_ID,RETERN_VAL)

#endif

#endif /* COMPONENTS_FLIC_COMPONENTS_ISPINTERFACE_LEON_LOCDBGROUTINES_H_ */
