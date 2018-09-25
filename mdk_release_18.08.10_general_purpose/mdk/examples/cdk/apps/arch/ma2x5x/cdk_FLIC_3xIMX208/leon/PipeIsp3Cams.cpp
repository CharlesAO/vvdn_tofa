#include <Opipe.h>

#include <Flic.h>
#include <MemAllocator.h>

#include "PlgSource.h"
#include "PlgIspCtrl.h"
#include "PlgIspProc.h"
#include "PlgIspMono.h"
#include "PlgOutItf.h"

#include "PipeIsp3Cams.h"
#include "sched.h"

PlgSource         plgSrc    [APP_NR_OF_CAMS] SECTION(".cmx_direct.data"); //Source
PlgIspCtrl       *plgIspCtrl                 SECTION(".cmx_direct.data"); //Guzzi wrapper

PlgIspProc        plgIspC    [APP_NR_OF_CAMS_COLOR] SECTION(".cmx_direct.data"); //ISP
PlgIspMono        plgIspM    [APP_NR_OF_CAMS_MONO] SECTION(".cmx_direct.data"); //ISP
//PlgIspProc        plgIspM    [APP_NR_OF_CAMS_MONO] SECTION(".cmx_direct.data"); //ISP

PlgPool<ImgFrame> plgPoolSrc[APP_NR_OF_CAMS] SECTION(".cmx_direct.data"); //source out pool
PlgPool<ImgFrame> plgPoolIspO[APP_NR_OF_CAMS] SECTION(".cmx_direct.data"); //isp    out pool


PlgOutItf         plgOut                     SECTION(".cmx_direct.data"); //out

PlgIspProc        plgIspStl    [APP_NR_OF_CAMS_COLOR] SECTION(".cmx_direct.data"); //ISP
PlgPool<ImgFrame> plgPoolIspStl[APP_NR_OF_CAMS_COLOR] SECTION(".cmx_direct.data"); //isp    out pool

Pipeline          SECTION(".cmx_direct.data") p(20);


// Pipe for 1 color camera and 2 mono camera conected.
// id 0 for colr
// in 1, 2 for mono
// id 3 for still. Still available just for color camera
void pipe3CamsCreate(GetSrcSzLimits getSrcSzLimits)
{
    uint32_t c = 0;

    OpipeReset();
    // create all plugins
    RgnAlloc.Create(RgnBuff, DEF_POOL_SZ);
    plgIspCtrl = PlgIspCtrl::instance();
    plgIspCtrl->Create();
    plgIspCtrl->schParam.sched_priority = CUSTOM_FLIC_PRIORITY;
    plgOut    .Create();
    plgOut.schParam.sched_priority = CUSTOM_FLIC_PRIORITY;

    // COLOR SIDE
    for(c = (uint32_t)IC_SOURCE_0; c < APP_NR_OF_CAMS_COLOR; c++) {
        plgSrc[c]    .Create((icSourceInstance)c);
        plgSrc[c].schParam.sched_priority = CUSTOM_FLIC_PRIORITY;
        plgSrc[c]    .outFmt   = SIPP_FMT_16BIT;
        icSourceSetup srcSet;
        assert((0 == getSrcSzLimits(c, &srcSet)) && "Camera not connected, App expect 3 cams");
        icSourceSetup* srcLimits = &srcSet;
        srcLimits->maxBpp = 16;
        plgPoolSrc[c].Create(&RgnAlloc, N_POOL_FRMS, ((srcLimits->maxPixels * srcLimits->maxBpp)) >> 3); //RAW
        plgPoolIspO[c].Create(&RgnAlloc, N_POOL_FRMS, (srcLimits->maxPixels * 3)>>1); //YUV420
        plgPoolIspStl[c].Create(&RgnAlloc, N_POOL_FRMS_STL, (srcLimits->maxPixels * 3)>>1); //YUV420
        //srcSet.appSpecificInfo incorporate information about 1.2 downscale preview will be applay or not on this image
        plgIspC[c].Create(c, srcSet.appSpecificInfo);
        plgIspC[c].schParam.sched_priority = CUSTOM_FLIC_PRIORITY;
        plgIspStl[c].Create(c+APP_NR_OF_CAMS, 0);
        plgIspStl[c].schParam.sched_priority = CUSTOM_FLIC_PRIORITY;
    }

    // MONO SIDE
    for(c = (uint32_t)APP_NR_OF_CAMS_COLOR; c < (APP_NR_OF_CAMS_COLOR+APP_NR_OF_CAMS_MONO); c++) {
        plgSrc[c]    .Create((icSourceInstance)c);
        plgSrc[c].schParam.sched_priority = CUSTOM_FLIC_PRIORITY;
        plgSrc[c]    .outFmt   = SIPP_FMT_16BIT;
        icSourceSetup srcSet;
        assert((0 == getSrcSzLimits(c, &srcSet)) && "Camera not connected, App expect 3 cams");
        icSourceSetup* srcLimits = &srcSet;
        srcLimits->maxBpp = 16;
        plgPoolSrc[c].Create(&RgnAlloc, N_POOL_FRMS, ((srcLimits->maxPixels * srcLimits->maxBpp)) >> 3); //RAW
        plgPoolIspO[c].Create(&RgnAlloc, N_POOL_FRMS, (srcLimits->maxPixels * 3)>>1); //RAW8 out
        plgIspM[c-APP_NR_OF_CAMS_COLOR].Create(c, 0);
        plgIspM[c-APP_NR_OF_CAMS_COLOR].schParam.sched_priority = CUSTOM_FLIC_PRIORITY;
    }

    // add to pipeline
    p.Add(plgIspCtrl);
    p.Add(&plgOut);


    //COLOR SIDE
    for(c = (uint32_t)IC_SOURCE_0; c < APP_NR_OF_CAMS_COLOR; c++) {
        p.Add(&plgSrc       [c]);
        p.Add(&plgIspC      [c]);
        p.Add(&plgIspStl    [c]);
        p.Add(&plgPoolSrc   [c]);
        p.Add(&plgPoolIspO  [c]);
        p.Add(&plgPoolIspStl[c]);
    }

    // MONO SIDE
    for(c = (uint32_t)APP_NR_OF_CAMS_COLOR; c < (APP_NR_OF_CAMS_COLOR+APP_NR_OF_CAMS_MONO); c++) {
        p.Add(&plgSrc       [c]);
        p.Add(&plgIspM      [c-APP_NR_OF_CAMS_COLOR]);
        p.Add(&plgPoolSrc   [c]);
        p.Add(&plgPoolIspO  [c]);
    }

    // link plugins
    for(c = 0; c < APP_NR_OF_CAMS_COLOR; c++) {
        plgPoolSrc[c].out             .Link(&plgSrc[c].inO);
        plgSrc[c]    .outCommand      .Link(&plgIspCtrl->inCtrlResponse);
        plgIspCtrl -> outSrcCommand[c].Link(&plgSrc[c].inCommand);
        plgPoolIspO[c].out             .Link(&plgIspC[c].inO);
        plgSrc[c]    .out             .Link(&plgIspC[c].inI);
        plgIspC[c]    .outF            .Link(&plgOut.in );
        plgIspC[c]    .outE            .Link(&plgIspCtrl->inCtrlResponse);

        plgPoolIspStl[c].out          .Link(&plgIspStl[c].inO);
        plgSrc[c]       .outStl       .Link(&plgIspStl[c].inI);
        plgIspStl[c]    .outF         .Link(&plgOut.in );
        plgIspStl[c]    .outE         .Link(&plgIspCtrl->inCtrlResponse);
    }


    // link plugins
    for(c = (uint32_t)APP_NR_OF_CAMS_COLOR; c < (APP_NR_OF_CAMS_COLOR+APP_NR_OF_CAMS_MONO); c++) {
        plgPoolSrc[c].out             .Link(&plgSrc[c].inO);
        plgSrc[c]    .outCommand      .Link(&plgIspCtrl->inCtrlResponse);
        plgIspCtrl -> outSrcCommand[c].Link(&plgSrc[c].inCommand);
        plgPoolIspO[c].out             .Link(&plgIspM[c-APP_NR_OF_CAMS_COLOR].inO);
        plgSrc[c]    .out             .Link(&plgIspM[c-APP_NR_OF_CAMS_COLOR].inI);
        plgIspM[c-APP_NR_OF_CAMS_COLOR]    .outF            .Link(&plgOut.in );
        plgIspM[c-APP_NR_OF_CAMS_COLOR]    .outE            .Link(&plgIspCtrl->inCtrlResponse);
    }

  plgOut.outCmd.Link(&plgIspCtrl->inCtrlResponse);
  plgIspCtrl->outOutCmd.Link(&plgOut.inCmd);

    p.Start();
}

void pipe3CamsDestroy(void) {
    p.Stop();
    p.Wait();
    p.Delete();
    RgnAlloc.Delete();
}
