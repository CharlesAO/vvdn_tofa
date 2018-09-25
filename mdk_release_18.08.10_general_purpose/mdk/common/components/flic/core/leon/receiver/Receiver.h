///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Master/Slave Receiver
///

#ifndef __RECEIVER_H__
#define __RECEIVER_H__

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>

template<typename T> class SSender;

//Note: one plugin migh send a notification to another plugin from an IRQ
//      (e.g. Isp::LineReached), so "Notify" must be runnable from IRQ as well
class SemSync : public ISync{
  private: sem_t sem;
  public :
          void Create() {assert(OK==sem_init(&sem,0,0));}
           int Notify() {return sem_post(&sem);}
           int Wait  () {return sem_wait(&sem);}
          void Destroy(){assert(OK== sem_destroy(&sem));}
};

//####################################################################
// Slave-Receiver interface
//####################################################################
template<typename T> class ISReceiver : public Io
{
  public:

    virtual void Create(uint32_t nMsg, IBuffCmd<T> *beh, ISync *s) = 0;
            void Create(uint32_t nMsg, IBuffCmd<T> *beh) { Create(nMsg, beh , NULL); }
            void Create(uint32_t nMsg, ISync *s)         { Create(nMsg, NULL,    s); }
            void Create(uint32_t nMsg          )         { Create(nMsg, NULL, NULL); }

    virtual void Delete () = 0;
    virtual void Destroy() = 0;

  //Receiver view:
    virtual int  Receive   (T *msg, IBuffCmd<T> *c = NULL) = 0;
    virtual int  TryReceive(T *msg, IBuffCmd<T> *c = NULL) = 0;

  //Sender view (i.e. called by sender)
    virtual int  Post      (Io *who, T *msg, TimeSpec* ts, IBuffCmd<T> *c = NULL) = 0;
};

//####################################################################
// Slave-Receiver default implementation
//####################################################################
template<typename T> class SReceiver : public ISReceiver<T>
{
   public:
         MsgBuf<T>   mq;  //Slave must buffer incoming messages

         ISync   *sync;   //for grouped inputs

         using ISReceiver<T>::Create; //Thanks Razvan :)
         void Create(uint32_t nMsg, IBuffCmd<T> *beh, ISync *s)
         {
            mq.Create(nMsg, beh);
            sync = s;
         }
         void Delete () { mq.Delete (); };
         void Destroy() { mq.Destroy(); };

  //Receiver view:
    virtual int FLIC_WU Receive(T *m, TimeSpec* ts, TsType tt, IBuffCmd<T> *c)
    {
        TimeSpec absTs = GetAbsTs(ts, tt);
        return mq.Pop(this, m, &absTs, c);
    }

    virtual int FLIC_WU Receive   (T *m, TimeSpec* ts, TsType tt = REL){return Receive(m,  ts,      tt, NULL);}
    virtual int FLIC_WU Receive   (T *m, IBuffCmd<T> *c = NULL)        {return Receive(m, &TS_INF, ABS,    c);}
    virtual int FLIC_WU TryReceive(T *m, IBuffCmd<T> *c = NULL)        {return Receive(m, &TS_TRY, ABS,    c);}

  //Sender view (called by sender)
    virtual int  Post    (Io *who, T *msg, TimeSpec *ts, IBuffCmd<T> *c = NULL)
    {
        int rc = mq.Push(who, msg, ts, c);
        if((rc==0) && (sync))
            sync->Notify();
        return rc;
    }
    virtual ~SReceiver<T>(){};
};

//###############################################################
//Master-Receiver
//###############################################################
template <typename T> class MReceiver: public Io
{
  public:
   //Master behaviour: 'Receive' invokes a 'Pull' from associated SSender
    virtual int FLIC_WU Receive(T *m, TimeSpec* ts, TsType tt, IBuffCmd<T> *c)
    {
        TimeSpec absTs = GetAbsTs(ts, tt);
        return snd->Pull(this, m, &absTs, c);
    }
    virtual int FLIC_WU Receive   (T *m, TimeSpec* ts, TsType tt = REL){return Receive(m,      ts,  tt, NULL);}
    virtual int FLIC_WU Receive   (T *m, IBuffCmd<T> *c = NULL)        {return Receive(m, &TS_INF, ABS,    c);}
    virtual int FLIC_WU TryReceive(T *m, IBuffCmd<T> *c = NULL)        {return Receive(m, &TS_TRY, ABS,    c);}

    SSender<T>  *snd;
    virtual ~MReceiver(){};
};


#endif