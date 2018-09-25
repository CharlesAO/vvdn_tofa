///
/// @file      MonoPipe_OS.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
/// @author:   MT
/// @brief     Contains a sub-pipe for 1 mono isp pipe
///
///
#include "MonoPipe_RT.h"
#include "MonoPipe_OS.h"

#ifndef DEFAULT_IPC_FLIC_LEVEL
#define DEFAULT_IPC_FLIC_LEVEL 240
#endif

void MonoPipe_OS::Create(MonoPipeConfig  *ispMonoGrpConfig, MonoPipe_RT  *MonoPipeParams_RT) {
    cfg = *ispMonoGrpConfig;

    rtSeIspOut  = &MonoPipeParams_RT->rtSeIspOut;
    osRiIspOut.Create(rtSeIspOut);
    for(int cam = 0; cam < N_MONO_CAMS; ++cam)
    {
        if(((int)cfg.ipcThreadPriority[cam] < 0)||(cfg.ipcThreadPriority[cam] > 254))
            cfg.ipcThreadPriority[cam] = DEFAULT_IPC_FLIC_LEVEL;

        rtRiSrcCtrl[cam] = &MonoPipeParams_RT->rtRiSrcCtrl[cam];
        rtSeSrcCtrl[cam] = &MonoPipeParams_RT->rtSeSrcCtrl[cam];
        rtSeEv[cam]      = &MonoPipeParams_RT->rtSeEv[cam];

        plgSrcCtrl[cam] = PlgSrcCtrl::instance();

        plgSrcCtrl[cam]  ->Create(cfg.camId[cam]);
        plgEventsRec[cam] .Create(cfg.mesageQueueEventName[cam]);
        plgEventsRec[cam].schParam.sched_priority = cfg.ipcThreadPriority[cam];

        osSeSrcCtrl[cam].Create(rtRiSrcCtrl[cam]);
        osSeSrcCtrl[cam].schParam.sched_priority = cfg.ipcThreadPriority[cam];
        osRiSrcCtrl[cam].Create(rtSeSrcCtrl[cam]);
        osRiSrcCtrl[cam].schParam.sched_priority = cfg.ipcThreadPriority[cam];
        osRiEv[cam].Create(rtSeEv[cam]);
        osRiEv[cam].schParam.sched_priority = cfg.ipcThreadPriority[cam];
        osRiIspOut.schParam.sched_priority = cfg.ipcThreadPriority[cam];
    }
    rtSeDispOut = &MonoPipeParams_RT->rtSeDispOut;
    osRiDispOut.Create(rtSeDispOut);
    osRiDispOut.schParam.sched_priority = cfg.ipcThreadPriority[0];
}

void MonoPipe_OS::AddTo(Pipeline *p) {

    p->Add(&osRiIspOut);
    for(int cam = 0; cam < N_MONO_CAMS; ++cam)
    {
        if(p->Has(plgSrcCtrl[cam])) {
            // the plugin was added by previous call
        }
        else {
            p->Add(plgSrcCtrl[cam]);
        }

        p->Add(&plgEventsRec[cam]);
        p->Add(&osSeSrcCtrl[cam]);
        p->Add(&osRiSrcCtrl[cam]);
        p->Add(&osRiEv[cam]);

        plgSrcCtrl[cam]->oCamCtrl[cfg.camId[cam]].Link(&osSeSrcCtrl[cam].in);
        osRiSrcCtrl[cam] .out       .Link(&plgSrcCtrl[cam]->iCamCtrl[cfg.camId[cam]]);
        osRiEv[cam]      .out       .Link(&plgEventsRec[cam].iEv);
        //Public IOs/pads
        //out[cam] = &osRiIspOut[cam]  .out;
        dispOut = &osRiDispOut.out;
    }

    p->Add(&osRiDispOut);
}
