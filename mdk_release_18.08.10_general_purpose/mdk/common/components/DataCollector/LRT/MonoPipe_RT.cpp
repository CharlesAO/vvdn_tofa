///
/// @file      MonoPipe_RT.cpp
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

void MonoPipe_RT::Create(MonoPipe_OS  *MonoPipeParams_OS, IAllocator *rgnAlloc) {
    cfg = MonoPipeParams_OS->cfg;
    cfg.rgnAlloc = rgnAlloc;
    osRiIspOut  = &MonoPipeParams_OS->osRiIspOut;
    rtSeIspOut.Create(osRiIspOut);
    //plgSync.camCtx.camCnt = N_MONO_CAMS;
    plgSync.Create();
    for(int cam = 0; cam < N_MONO_CAMS; ++cam)
    {
        if(((int)cfg.ipcThreadPriority[cam] < 0)||(cfg.ipcThreadPriority[cam] > 254))
            cfg.ipcThreadPriority[cam] = DEFAULT_IPC_FLIC_LEVEL;

        osSeSrcCtrl[cam] = &MonoPipeParams_OS->osSeSrcCtrl[cam];
        osRiSrcCtrl[cam] = &MonoPipeParams_OS->osRiSrcCtrl[cam];
        osRiEv[cam]      = &MonoPipeParams_OS->osRiEv[cam];

        plgSrc[cam]       .Create((icSourceInstance)cfg.camId[cam]);
        plgIspCtrl[cam]   .Create();
        plgIsp[cam]       .Create(cfg.camId[cam], cfg.spIspVdoCfg[cam].firstCmxSliceUsed,
                                  cfg.spIspVdoCfg[cam].nrOfCmxSliceUsed,
                                  cfg.spIspVdoCfg[cam].monoIrqRatePow);
        plgPoolSrc[cam]   .Create(cfg.rgnAlloc, cfg.spSrcCfg[cam].nrFrmsSrc,
                                  cfg.spSrcCfg[cam].maxImgSz);
        plgPoolIsp[cam]   .Create(cfg.rgnAlloc, cfg.spIspVdoCfg[cam].nrFrmsIsp,
                                  cfg.spIspVdoCfg[cam].maxImgSz, true);
        rtRiSrcCtrl[cam].Create(osSeSrcCtrl[cam]);
        rtRiSrcCtrl[cam].schParam.sched_priority = cfg.ipcThreadPriority[cam];
        rtSeSrcCtrl[cam].Create(osRiSrcCtrl[cam]);
        rtSeSrcCtrl[cam].schParam.sched_priority = cfg.ipcThreadPriority[cam];
        rtSeEv[cam].Create(osRiEv[cam]);
        rtSeEv[cam].schParam.sched_priority = cfg.ipcThreadPriority[cam];
        rtSeIspOut.schParam.sched_priority = cfg.ipcThreadPriority[cam];
            
    }


}

void MonoPipe_RT::AddTo(Pipeline *p) {

    p->Add(&rtSeIspOut);
    for(int cam = 0; cam < N_MONO_CAMS; ++cam)
    {
        p->Add(&plgSrc[cam]);
        p->Add(&plgPoolSrc[cam]);
        p->Add(&plgIspCtrl[cam]);
        p->Add(&plgIsp[cam]);
        p->Add(&plgPoolIsp[cam]);
        p->Add(&rtRiSrcCtrl[cam]);
        p->Add(&rtSeSrcCtrl[cam]);
        p->Add(&rtSeEv[cam]);

        rtRiSrcCtrl[cam] .out       .Link(&plgSrc[cam].iCtrl);
        plgSrc[cam]      .oCtrl     .Link(&rtSeSrcCtrl[cam].in);
        plgSrc[cam]      .oSoF      .Link(&rtSeEv[cam].in);
        plgSrc[cam]      .oEoF      .Link(&rtSeEv[cam].in);
        plgSrc[cam]      .oLineHit  .Link(&rtSeEv[cam].in);
        plgSrc[cam]      .oErr      .Link(&rtSeEv[cam].in);

        plgPoolSrc[cam]  .out       .Link(&plgSrc[cam].iFrm);
        plgPoolIsp[cam]  .out       .Link(&plgIsp[cam].inO);
        plgSrc[cam]      .oFrm      .Link(&plgIspCtrl[cam].inO);
        plgIspCtrl[cam]  .out       .Link(&plgIsp[cam].inI);
        //plgIsp[cam]      .outF      .Link(&rtSeIspOut[cam].in);
        plgIsp[cam]      .outE      .Link(&rtSeEv[cam].in);
    }

    p->Add(&plgSync);
    plgSync.outStereo.Link(&rtSeIspOut.in);

    plgIsp[0].outF      .Link(&plgSync.inO[0]);
    plgIsp[1].outF      .Link(&plgSync.inO[1]);

}
