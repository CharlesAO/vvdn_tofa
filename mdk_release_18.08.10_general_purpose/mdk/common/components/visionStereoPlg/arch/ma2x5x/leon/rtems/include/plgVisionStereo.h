
#ifndef MIG_PLG_STEREO_H__
#define MIG_PLG_STEREO_H_
#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
#include "messages.h"
#include <BehQueueOvr.h>
#include "visionBasePlg.h"

class plgVisionStereo : public visionBasePlg{
    public:

        // Constructor
        plgVisionStereo(customProcCtx _ctx)
        {
            ctx = _ctx;
        }

        void
        Init(void)
            { }
    private:
        void* pluginSpecificInit(void);
        void* pluginSpecificProcess();
};



#endif
