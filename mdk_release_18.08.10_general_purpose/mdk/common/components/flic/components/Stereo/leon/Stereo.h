#ifndef STEREO_H

#include <ImgFrame.h>
#include "stereo_cfg.h"
#include "SippWarp.h"
#include <string>
#include "logger.h"
#include "OsDrvSvu.h"
#include "stereoUtils.h"


void StereoInit(stereo_cfg &cfg, stereo_resources &res);


class Stereo
{
public:

    uint32_t  id;
    Stereo(stereo_cfg &cfg,stereo_resources &res);
    ~Stereo();
    void CreateSipp(void);
    void Process(frameBuffer fLeft, frameBuffer fRight, frameBuffer fDisparity, frameBuffer  fDepth);
    void Stop(void);
    void buildStereoPipe (StereoPipe * pPl);
    void buildStereoPipeSubpixel (StereoPipe * pPl);
    void configStereoPipe (StereoPipe * pPl);
    void UnRegPipeFromMap (void);
    rtems_id wsgbm_cb_sem = 0;
    void setHomography( float* right_homog, bool init);
    void setConfidence( const uint8_t& new_conf_val);

private:
    uint32_t lg_group;
    uint32_t lg_ster_proc;
    uint32_t lg_sipp;
    uint32_t lg_shave;
    stereo_cfg cfg_;
    stereo_resources res_;
    uint8_t in_bitsize;
    uint8_t in_bytesize;
    uint8_t stereo_bitcfg;
    static Logger &log;
    StereoPipe stereoPipe;
    uint8_t ker_x;
    uint8_t ker_y;
    uint8_t numDisp;
    bool customStereoConfig_ = false;
    uint8_t left_id;
    uint8_t right_id;
    void SetMaskSize(OsDrvStereoCTKernelSize ct_ker);
    uint64_t ConfigMask(mask_type maskType);

    std::string unique_str;


    uint64_t instance_name;
    char *name_str;

    void RunSIPP(frameBuffer &Left, frameBuffer &Right, frameBuffer &Disparity, frameBuffer &Depth);
    void SetDefaultStereoParam();
    void SetDefaultStereoTuning720p();
    void SetDefaultStereoTuningVga();
    void SetSubpixelStereoTuningVga();

};

typedef struct objpipemap
{
    Stereo *obj;
    SippPipeline *pl_wsgbm;
    //SippPipeline *pl_disp2depth;
} objpipemap;


#define STEREO_H
#endif
