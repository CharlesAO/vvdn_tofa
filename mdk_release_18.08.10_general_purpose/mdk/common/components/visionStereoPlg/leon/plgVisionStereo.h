
#ifndef MIG_PLG_STEREO_H__
#define MIG_PLG_STEREO_H_
#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
#include "messages.h"
#include <BehQueueOvr.h>
#include "visionBasePlg.h"
#include "Stereo.h"

class plgVisionStereo : public visionBasePlg{
    public:

        // Constructor
        plgVisionStereo(stereo_cfg cfg, stereo_resources res,customProcCtx _ctx) : m_cfg(cfg), m_res(res)
        {
            ctx = _ctx;
        }

        void
        Init(void)
            { }
    private:
        void* pluginSpecificInit(void);
        void* pluginSpecificProcess();
        stereo_cfg m_cfg;
        stereo_resources m_res;

        Stereo *Stereo_obj;
};



#endif
