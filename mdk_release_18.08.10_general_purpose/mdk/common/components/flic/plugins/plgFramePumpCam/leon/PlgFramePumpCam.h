#ifndef __PLG_FRAME_PUMP_CAM__
#define __PLG_FRAME_PUMP_CAM__

#include <pthread.h>

typedef struct cameraNodeContext
{
    char *camName;
    int camFrameRate;
    int camCnt;
    pthread_barrier_t *barrier;
}cameraNodeContext;

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

class PlgFramePumpCam : public ThreadedPlugin{
  public:
    int isMaster;
    cameraNodeContext camCtx;
    MSender<ImgFramePtr> out;
    MReceiver<ImgFramePtr> emptyIn;
    uint32_t cnt;
    void Create(){
        previousCnt = 0;
        writeIdx = 0;
        readIdx = 0;
        Add(&out, ".out");
        Add(&emptyIn, ".emptyIn");

        camDesc = open(camCtx.camName, O_RDWR);
        if(camDesc < 0)
        {
            printf( "Failed to open camera\n");
        }
    }
    void *threadFunc(void *);
    void *vgaThreadFunc(cameraNodeContext * context);
    void populateFrameSpec(frameSpec* fs);
  private:
    static const int MAX_MESSAGES= 12;
    int camDesc;
    ImgFramePtr frameMsgList[MAX_MESSAGES];
    uint32_t writeIdx;
    uint32_t readIdx;
    uint32_t previousCnt;
};

#endif
