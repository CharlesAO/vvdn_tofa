#ifndef RBIMU_H__
#define RBIMU_H__

#include <semaphore.h>
#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

int imuInit(void);


typedef struct imuContext {
    const char * name;
    sem_t* outputSemaphore;
    int sampleRate;
    int range;
    int bmxIrqGpio;
    int bmxIrqPriority;
    MReceiver<ImgFramePtr>* in;
    MSender<ImgFramePtr>* out;
} imuContext;



class PlgImu : public ThreadedPlugin{
  public:
    MSender<ImgFramePtr> out;
    MReceiver<ImgFramePtr> emptyIn;
    void Create(){
        Add(&out, ".out");
        Add(&emptyIn, ".emptyIn");
        imuInit();
    }

    void *threadFunc(void *);
    void* syncThread(void* context);
  private:
    imuContext imuCtxtBmi;
    imuContext imuCtxtAcc;
    imuContext imuCtxtMag;
    imuContext imuCtxtGyro;
    pthread_t imuThreadAcc;
    pthread_t imuThreadGyro;
    pthread_t imuThreadMag;
    pthread_t imuThreadBmi;
};

#endif
