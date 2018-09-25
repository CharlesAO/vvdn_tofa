///
/// @file      PlgIspProcOpipe.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Opipe Interface for the Isp flic plugin.
///

#ifndef COMPONENTS_FLIC_PLUGINS_PLGISPPROC_LEON_PLGISPPROCOPIPE_H_
#define COMPONENTS_FLIC_PLUGINS_PLGISPPROC_LEON_PLGISPPROCOPIPE_H_

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include "IspCommon.h"
#include "IspCommonOpipe.h"
#include "PolyFirUtils.h"
#include "ImgFrame.h"

#ifndef PLG_ISP_MAX_W
#define PLG_ISP_MAX_W (2144)
#endif

class PlgIspProcOpipe {
public:
    uint32_t nFrmDone;
    void IspStart(OpipeMF *opF, ImgFrame *fInp, ImgFrame *fOut, icIspConfig *ispCfg);
    void IspWait();
    void IspBuildPipe(OpipeMF *opF);
    void IspDestroy();
    OpipeMF     opF;
    UpfirdnCfg  updnCfg0;
    UpfirdnCfg  updnCfg12;
    uint32_t    down2xOn;
    PolyFirUtils polyUtils;
    IspCommonOpipe opipeCommon;

private:
    void fetchIcIspConfig(Opipe *p, icIspConfig *ic);
};


#endif /* COMPONENTS_FLIC_PLUGINS_PLGISPPROC_LEON_PLGISPPROCOPIPE_H_ */
