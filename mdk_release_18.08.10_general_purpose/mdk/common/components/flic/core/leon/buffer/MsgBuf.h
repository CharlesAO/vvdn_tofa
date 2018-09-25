///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Message Buffer
///

#include "Flic.h"

#ifndef _BUFFER_H__
#define _BUFFER_H__

#include <rtems.h>
#include <BehQueue.h>

//#####################################################################
template<typename T> void MsgBuf<T>::Create(size_t numElem, IBuffCmd<T> *beh)
{
    Init(numElem); //non-T

   //using worst case alignment requirement (i.e. 8 Bytes)
    storage = new uint64_t [(sizeof(T) * numElem + 7)/8];
    mssg    = new T*[numElem];
    for(uint32_t i=0; i< numElem; i++) // T
       mssg[i] = &(((T*)storage)[i]);  // T

    log  = NULL;                       // T
    if(beh == NULL)
       beh = QueueCmd<T>::Inst();
    exec = beh;
}

//#####################################################################
template<typename T> void MsgBuf<T>::Delete()
{
    delete[] storage;
    delete[] mssg;
    storage = NULL;   //T
    Free();
}

//#####################################################################
template<typename T> void MsgBuf<T>::SetBehavior(IBuffCmd<T> *beh)
{
    exec = beh;
}

//#####################################################################
//Helpers
template<typename T> int MsgBuf<T>::ThePush(Io *who, T *m, IBuffCmd<T> *req, TimeSpec *ts)
{
    assert(storage != NULL);
    IBuffCmd<T> *doer = req ? req : exec;
    if(FLIC_OP_BEGIN_LOG && log)  log->LogPushBegin(who, m);
    int rc = doer->doPush(this, m, ts);
    if(FLIC_OP_END_LOG   && log)  log->LogPushEnd  (who,  m, rc);
    return rc;
}

template<typename T> int MsgBuf<T>::ThePop(Io *who, T *m, IBuffCmd<T> *req, TimeSpec *ts)
{
    assert(storage != NULL);
    IBuffCmd<T> *doer = req ? req : exec;
    if(FLIC_OP_BEGIN_LOG && log)  log->LogPopBegin(who);
    int rc = doer->doPop(this, m, ts);
    if(FLIC_OP_END_LOG   && log)  log->LogPopEnd  (who,  m, rc);
    return rc;
}

//check we're not in IRQ context
#ifndef IC
#define IC assert(!rtems_interrupt_is_in_progress())
#endif

//#####################################################################
//Top wrappers
template<typename T> int MsgBuf<T>::Push   (Io *who, T *m, IBuffCmd<T> *req) {    return ThePush(who, m, req, &TS_INF);}
template<typename T> int MsgBuf<T>::TryPush(Io *who, T *m, IBuffCmd<T> *req) {IC; return ThePush(who, m, req, &TS_TRY);}
template<typename T> int MsgBuf<T>::Pop    (Io *who, T *m, IBuffCmd<T> *req) {    return ThePop (who, m, req, &TS_INF);}
template<typename T> int MsgBuf<T>::TryPop (Io *who, T *m, IBuffCmd<T> *req) {IC; return ThePop (who, m, req, &TS_TRY);}

//Timed
template<typename T> int MsgBuf<T>::Push(Io *who, T *m, TimeSpec *ts, IBuffCmd<T> *req) {return ThePush(who, m, req, ts);}
template<typename T> int MsgBuf<T>::Pop (Io *who, T *m, TimeSpec *ts, IBuffCmd<T> *req) {return ThePop (who, m, req, ts);}
#endif