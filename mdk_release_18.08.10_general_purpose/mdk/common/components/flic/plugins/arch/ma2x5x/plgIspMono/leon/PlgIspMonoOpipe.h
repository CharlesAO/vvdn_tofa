///
/// @file      PlgIspMonoOpipe.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Opipe Interface for the Isp flic plugin.
///

#ifndef COMPONENTS_FLIC_PLUGINS_PLGISPMONO_LEON_PLGISPMONOOPIPE_H_
#define COMPONENTS_FLIC_PLUGINS_PLGISPMONO_LEON_PLGISPMONOOPIPE_H_

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

class PlgIspMonoOpipe {
public:
#ifndef PLG_ISP_MAX_GAMMA_SIZE
#define PLG_ISP_MAX_GAMMA_SIZE (512)
#endif
#ifndef PLG_ISP_GAMMA_SECT
//#define PLG_ISP_GAMMA_SECT ".cmx.bss"
#define PLG_ISP_GAMMA_SECT
#endif

    uint32_t nFrmDone;
    void IspStart(OpipeBayerMono *opF, ImgFrame *fInp, ImgFrame *fOut, icIspConfig *ispCfg);
    void IspWait();
    void IspBuildPipe(OpipeBayerMono *opF);
    void IspDestroy();
    OpipeBayerMono     opF;
    uint32_t    down2xOn;
    PolyFirUtils polyUtils;
    IspCommonOpipe opipeCommon;

private:
    static uint16_t locGammaMono[PLG_ISP_MAX_GAMMA_SIZE];
    static LutCfg   locLutMonoCfg;
    void fetchIcIspConfig(Opipe *p, icIspConfig *ic);
    void plgIspMonoBayerSetParams(Opipe *p, icIspConfig *ic);
};


#endif /* COMPONENTS_FLIC_PLUGINS_PLGISPMONO_LEON_PLGISPMONOOPIPE_H_ */
