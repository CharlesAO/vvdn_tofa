///
/// @file      MonoPipe_RT.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
/// @author:   MT
/// @brief     Contains a sub-pipe for 1 isp mono pipe
///
///

#ifndef _MonoPipe_RT_H_
#define _MonoPipe_RT_H_

/**************************************************************************************************
 ~~~ Includes
 **************************************************************************************************/
#include "GrupsTypes.h"
#include "MonoPipe_OS.h"

#include <Flic.h>
#include <FlicRmt.h>
#include <MemAllocator.h>
#include <Pool.h>
#include <ImgFrame.h>

#include "PlgSrcMipi.h"
#include "PlgEventsRec.h"
#include "PlgMonoIsp.h"
#include "PlgIspCtrl.h"
//#include "PlgStereoSync.h"
#include "PlgCamSync.h"
#include "PlgStereoProc.h"
#include "MonoPipeCommon.h"

/**************************************************************************************************
 ~~~  Configuration Type
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
class MonoPipe_OS;

class MonoPipe_RT : public PlgGroup {
private:
    // pointers to linked part
    RmtO<CmdGenMsg>   *osSeSrcCtrl[N_MONO_CAMS];
    RmtI<CmdGenMsg>   *osRiSrcCtrl[N_MONO_CAMS];
    RmtI<CmdGenMsg>   *osRiEv     [N_MONO_CAMS];
    RmtI<StereoImgMsg> *osRiIspOut;
    RmtI<ImgFramePtr> *osRiDispOut;

    // plugins
    PlgSrcMipi         plgSrc    [N_MONO_CAMS];
    PlgMonoIsp         plgIsp    [N_MONO_CAMS];
    PlgPool<ImgFrame>  plgPoolSrc[N_MONO_CAMS];
    PlgPool<ImgFrame>  plgPoolIsp[N_MONO_CAMS];
    PlgIspCtrl         plgIspCtrl[N_MONO_CAMS];


    MonoPipeConfig    cfg;

public:
    //PlgStereoSync      plgStereoSync;
    PlgCamSync plgSync;

    RmtI<CmdGenMsg>   rtRiSrcCtrl [N_MONO_CAMS];
    RmtO<CmdGenMsg>   rtSeSrcCtrl [N_MONO_CAMS];
    RmtO<CmdGenMsg>   rtSeEv      [N_MONO_CAMS];
    RmtO<StereoImgMsg> rtSeIspOut;
    RmtO<ImgFramePtr> rtSeDispOut;

    void Create(MonoPipe_OS  *MonoPipeParams_OS, IAllocator *rgnAlloc);
    void AddTo(Pipeline *p);
};

#endif //_GrpIspMonoVdo2L_RT_H_
