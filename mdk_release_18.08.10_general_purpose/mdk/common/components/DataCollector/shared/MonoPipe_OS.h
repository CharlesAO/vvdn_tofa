///
/// @file      MonoPipe_OS.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
/// @author:   MT
/// @brief     Contains a sub-pipe for 1 mono isp pipe
///

#ifndef _MonoPipe_OS_H_
#define _MonoPipe_OS_H_

/**************************************************************************************************
 ~~~ Includes
 **************************************************************************************************/
#include "GrupsTypes.h"
#include "MonoPipe_RT.h"

#include <Flic.h>
#include <FlicRmt.h>
#include <MemAllocator.h>
#include <Pool.h>
#include <ImgFrame.h>

#include "PlgSrcCtrl.h"
#include "PlgEventsRec.h"
#include "MonoPipeCommon.h"
#include <messages.h>

/**************************************************************************************************
 ~~~  Configuration Type
 **************************************************************************************************/


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/
class MonoPipe_RT;

class MonoPipe_OS : public PlgGroup {
private:
    RmtI<CmdGenMsg>   *rtRiSrcCtrl[N_MONO_CAMS]; //on LRT
    RmtO<CmdGenMsg>   *rtSeSrcCtrl[N_MONO_CAMS]; //on LRT
    RmtO<CmdGenMsg>   *rtSeEv     [N_MONO_CAMS]; //on LRT
    RmtO<StereoImgMsg> *rtSeIspOut; //on LRT
    RmtO<ImgFramePtr> *rtSeDispOut; //on LRT

    PlgSrcCtrl         *plgSrcCtrl [N_MONO_CAMS];
    PlgEventsRec       plgEventsRec[N_MONO_CAMS];

public:
    MonoPipeConfig    cfg;

    RmtO<CmdGenMsg>   osSeSrcCtrl[N_MONO_CAMS];
    RmtI<CmdGenMsg>   osRiSrcCtrl[N_MONO_CAMS];
    RmtI<CmdGenMsg>   osRiEv     [N_MONO_CAMS];
    RmtI<StereoImgMsg> osRiIspOut ;
    RmtI<ImgFramePtr> osRiDispOut;


    //public IOs
    //MSender  <ImgFramePtr> *out[N_MONO_CAMS];
    MSender  <ImgFramePtr> *dispOut;

    void Create(MonoPipeConfig  *ispMonoGrpConfig, MonoPipe_RT  *MonoPipeParams_RT);
    void AddTo(Pipeline *p);
};

#endif //_GrpIspMonoVdo2L_OS_H_
