#include "Stereo.h"
#include "stereoUtils.h"
#define MVLOG_UNIT_NAME Stereo
#include <mvLog.h>
#include <OsDrvShaveL2c.h>
#include <vector>
#include <algorithm>
#include <DrvTimer.h>
//#define DEBUG
#ifndef DEBUG
#define printf(...)
#endif

Logger & Stereo::log = Logger::getInstance();



Stereo::Stereo(stereo_cfg &cfg, stereo_resources &res):
cfg_(cfg),
res_(res)
{

    switch(cfg.stereocore.algoCfg.inMode)
    {
        case EIGHT_BIT:
            in_bitsize = 8;
            in_bytesize = sizeof(uint8_t);
            stereo_bitcfg = 0;
        break;
        case TEN_BIT:
            in_bitsize = 10;
            in_bytesize = sizeof(uint16_t);
            stereo_bitcfg = 1;
        break;
        default:
            in_bitsize = 8;
            in_bytesize = sizeof(uint8_t);
            stereo_bitcfg = 0;

    }

    switch(cfg.stereocore.algoCfg.dispRange)
    {
        case OS_DRV_STEREO_DISP_96:
            numDisp = 96;
            break;
        case OS_DRV_STEREO_DISP_64:
            numDisp = 64;
            break;
        default:
            numDisp = 64;
            break;
    }


    mvLog(MVLOG_DEBUG,"STEREO CONSTRUCTOR\n");

    CreateSipp();

}

void Stereo::SetDefaultStereoParam()
{
    customStereoConfig_ = cfg_.stereocore.customStereoConfig;

    switch (cfg_.calib.width){
    case 1280:
        if (!customStereoConfig_)
        {
            SetDefaultStereoTuning720p();
        }
        break;

    case 640:
        if (!customStereoConfig_)
        {
            SetDefaultStereoTuningVga();
        }
        break;

    default:
        break;
    }
}
void Stereo::SetDefaultStereoTuning720p()
{
    cfg_.stereocore.customStereoConfig = false;
    cfg_.stereocore.algoCfg = {
            .inMode= OS_DRV_STEREO_INPUT_8BIT,
            .opMode = OS_DRV_STEREO_CT_CM_AGG,
            .outMode = OS_DRV_STEREO_OUTPUT_DSP_CD,
            .dispRange = OS_DRV_STEREO_DISP_96,
            .compandEn = false,
            .costDump = 0,
            .subpixelEn = false};
    cfg_.stereocore.ctCfg = {
            .ctFormat = OS_DRV_STEREO_CT_PV8_CT24,
            .ctKernel = OS_DRV_STEREO_CT_7x7,
            .maskEn = true,
            .maskT = MASK_RASTER};
    numDisp = 96;
}

void Stereo::SetDefaultStereoTuningVga()
{
    cfg_.stereocore.customStereoConfig = false;
    cfg_.stereocore.algoCfg = {
            .inMode= OS_DRV_STEREO_INPUT_8BIT,
            .opMode = OS_DRV_STEREO_CT_CM_AGG,
            .outMode = OS_DRV_STEREO_OUTPUT_DSP_CD,
            .dispRange = OS_DRV_STEREO_DISP_96,
            .compandEn = false,
            .costDump = 0,
            .subpixelEn = false
        };
    cfg_.stereocore.ctCfg = {
            .ctFormat = OS_DRV_STEREO_CT_PV8_CT24,
            .ctKernel = OS_DRV_STEREO_CT_5x5,
            .maskEn = false,
            .maskT = MASK_ZERO};
    numDisp = 96;
}

void Stereo::SetSubpixelStereoTuningVga()
{
    cfg_.stereocore.customStereoConfig = false;
    cfg_.stereocore.algoCfg = {
            .inMode= OS_DRV_STEREO_INPUT_8BIT,
            .opMode = OS_DRV_STEREO_CT_CM_AGG,
            .outMode = OS_DRV_STEREO_OUTPUT_DSP_CD,
            .dispRange = OS_DRV_STEREO_DISP_96,
            .compandEn = false,
            .costDump = 0,
            .subpixelEn = true
        };
    cfg_.stereocore.ctCfg = {
            .ctFormat = OS_DRV_STEREO_CT_PV8_CT24,
            .ctKernel = OS_DRV_STEREO_CT_5x5,
            .maskEn = false,
            .maskT = MASK_ZERO};
    res_.wsgbm_cmx_end = res_.wsgbm_cmx_start + 11; //12 CMX slices min for 96VGA subpixel configuration
    numDisp = 96;
}

void  Stereo::Process(frameBuffer fLeft, frameBuffer fRight, frameBuffer fDisparity, frameBuffer  fDepth)
{
    UNUSED(fLeft);
    UNUSED(fRight);
    UNUSED(fDisparity);
    UNUSED(fDepth);
    mvLog(MVLOG_DEBUG,"Running Stereo on L:%lx R:%lx \n",(uint32_t)fLeft.p1,(uint32_t)fRight.p1);

    rtems_object_set_name(RTEMS_SELF, "PlgStereoProc");
    log.AddEvent(lg_group,lg_ster_proc, START_PROC,my_get_time(),getTimeStamps());

    //#define RUN_DUMMY_TIME_TEST
    mvLog(MVLOG_DEBUG,"%s,RUN SIPP\n",unique_str.c_str());
    log.AddEvent(lg_group,lg_sipp, START_PROC,my_get_time(),getTimeStamps());
    RunSIPP(fLeft, fRight, fDisparity,fDepth);
    mvLog(MVLOG_DEBUG,"Finished Stereo on L:%lx R:%lx \n",(uint32_t)fLeft.p1,(uint32_t)fRight.p1);

}

Stereo::~Stereo()
{
    rtems_status_code sc;
    mvLog(MVLOG_DEBUG,"STEREO DETROY\n");
    UnRegPipeFromMap();
    sippDeletePipeline(stereoPipe.pl_wsgbm);
    //sippDeletePipeline(stereoPipe.pl_disp2depth);
    sc = rtems_semaphore_delete(wsgbm_cb_sem);
    assert(sc == RTEMS_SUCCESSFUL);



}
