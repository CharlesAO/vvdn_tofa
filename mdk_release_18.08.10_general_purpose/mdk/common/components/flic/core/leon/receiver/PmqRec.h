///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Posix MQ-based receiver (for POD messages only !)
///

#ifndef __PMQ_REC_H__
#define __PMQ_REC_H__

#include <Flic.h>
#include <Receiver.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mqueue.h>

void getNextName(char *name);

//###############################################################
//A [Slave-Receiver] based on POSIX message-queue
// IMPORTANT: suitable for POD-like messages
//  DO NOT USE for messages that contain PoPtr members !!!
//###############################################################

template<typename T> class PmqRec: public ISReceiver<T>
{
  public:
      using ISReceiver<T>::Create; //Thanks Razvan :)

      void Create(uint32_t nMsg, IBuffCmd<T> *beh, ISync *s);
      int FLIC_WU Receive   (T *m, IBuffCmd<T> *c = NULL);
      int FLIC_WU TryReceive(T *m, IBuffCmd<T> *c = NULL);
      void Destroy   ();
      void Delete    (){};

   //Slave behaviour (Sender is allowed to Post any time)
      int  Post   (Io *w, T *m, TimeSpec *ts, IBuffCmd<T> *c = NULL);
      void SetLog (IBuffLog<T> *);

      struct timespec tvPush;
      struct timespec tvPop;

  private:
      mqd_t        mq;     //Slave must buffer incoming messages
      char         name[8];//mq name
      ISync       *sync;   //for grouped inputs
      IBuffLog<T> *log;
};


//###############################################################
//Implementation:
//###############################################################
template<typename T> void PmqRec<T>::Create(uint32_t nMsg, IBuffCmd<T> *beh, ISync *s)
{
    UNUSED(beh);
    struct mq_attr Qattr;
    sync = s;

   //initialize the queue attributes
    Qattr.mq_maxmsg  = nMsg;
    Qattr.mq_msgsize = sizeof(T);
    Qattr.mq_curmsgs = 0;

  //create the message queue
    getNextName (name);
    mq = mq_open(name, O_CREAT | O_RDWR, S_IREAD | S_IWRITE, &Qattr);
    assert(mq != (mqd_t) - 1);
  //printf("MQ id = 0x%lx\n", (uint32_t)mq);

    tvPush.tv_sec = tvPush.tv_nsec = 0;
    tvPop .tv_sec = tvPop .tv_nsec = 0;
    log = NULL;
}
//###############################################################
template<typename T> int PmqRec<T>::Post(Io *who, T *m, TimeSpec *ts, IBuffCmd<T> *c)
{
    assert(c == NULL); //can't customize
    if(FLIC_OP_BEGIN_LOG && log) log->LogPushBegin(who, m);
    int rc = mq_timedsend(mq, (const char*)m, sizeof(T), 0, ts);
    if((rc == OK) && (sync)) sync->Notify();
    if(FLIC_OP_END_LOG   && log) log->LogPushEnd  (who, m, rc);
    return rc;
}
//###############################################################
template<typename T> int PmqRec<T>::Receive(T *m, IBuffCmd<T> *c)
{
    assert(c == NULL); //can't customize
    int rc = mq_receive(mq, (char*)m, sizeof(T), NULL);
    if(FLIC_OP_BEGIN_LOG && log) log->LogPopBegin(this);
    if(rc != -1) rc = OK;
    if(FLIC_OP_END_LOG   && log) log->LogPopEnd  (this, m, rc);
  //else printf("PmqRec::Receive rc = %d\n", rc);
    return rc;
}
//###############################################################
template<typename T> int PmqRec<T>::TryReceive(T *m, IBuffCmd<T> *c)
{
    assert(c == NULL); //can't customize
    if(FLIC_OP_BEGIN_LOG && log) log->LogPopBegin(this);
    int rc = mq_timedreceive(mq, (char*)m, sizeof(T), NULL, &tvPop);
    if(rc != -1) rc = OK;
    if(FLIC_OP_END_LOG   && log) log->LogPopEnd  (this, m, rc);
    return rc;
}
//###############################################################
template<typename T> void PmqRec<T>::SetLog(IBuffLog<T> *_log)
{
  log = _log;
}
//###############################################################
template<typename T> void PmqRec<T>::Destroy()
{
    assert(OK == mq_unlink(name));
    assert(OK == mq_close(mq));
}

#endif