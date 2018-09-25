#ifndef ROTATION_MATRIX_NODE_H__
#define ROTATION_MATRIX_NODE_H__

#include <pthread.h>
#include <OsImuHAL.h>

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
#include <rotationMatMsg.h>

#include <math.h>


#define ONE_FRAME_DELAY_NS 33250000

class PlgRotationMat : public ThreadedPlugin{
  public:
    MSender<RotMatMsg> out;
    SReceiver<ImgFramePtr> timeStampIn;
    SReceiver<ImgFramePtr> inIMU;

    int freqDivider = 1; //in case the frame comes with higher frequency than required, we can use this divider
    void Create(){
        isPrevValid = 0;
        isCurrentValid = 0;
        previousTs = 0;
        timeStampIn.Create(2);
        inIMU.Create(128);
        Add(&out, ".out");
        Add(&timeStampIn, ".timeStampIn");
        Add(&inIMU, ".inIMU");
        dbgTotalRotMat.rotationMatrix[0] = 0;
        dbgTotalRotMat.rotationMatrix[1] = 0;
        dbgTotalRotMat.rotationMatrix[2] = 0;

    }
    void *threadFunc(void *);
  private:
    int processImuSample(ImgFrame* msg);
    int integrateImuSample(mvImuData_t* imuCurr, mvImuData_t* imuPrev);

    mvImuData_t currentSample;
    mvImuData_t previousSample;
    u8 isPrevValid;
    u8 isCurrentValid;
    u64 previousTs;
    u64 currentTs;
    RotMatMsg rotMat;
    RotMatMsg dbgTotalRotMat;

    static const float  c_nsPerSecond;
    static const int BIAS_ESTIMATE_COUNT;
    static const float DEG_PER_RADIAN;

    int biasEstimate = 0;
    float bias_x = 0;
    float bias_y = 0;
    float bias_z = 0;
};

#endif
