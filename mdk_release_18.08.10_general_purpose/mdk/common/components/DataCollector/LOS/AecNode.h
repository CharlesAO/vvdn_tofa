#ifndef AEC_NODE_H__
#define AEC_NODE_H__
#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

typedef struct aecContext
{
    int camCnt;
}aecContext;
int aecInit(void);


class PlgAec : public ThreadedPlugin{
  public:
    aecContext aecCtx;
    MSender<ImgFramePtr> expOut;
    SReceiver<ImgFramePtr> frameIn;
    void Create(){
        frameIn.Create(1);
        Add(&expOut, ".expOut");
        Add(&frameIn, ".frameIn");
        aecInit();
    }
    void *threadFunc(void *);
    void *aecThreadFunc(aecContext * context);
};

#endif
