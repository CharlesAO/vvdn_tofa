///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MsgBuffer : default push/pop queue behavior
///

#ifndef __BEH_QUEUE_H__
#define __BEH_QUEUE_H__

#include "rtems.h"
#include <errno.h>
#include <swcLeonUtilsDefines.h> //for NOP

//##############################################################
template <typename T> class QueueCmd : public IBuffCmd<T>
{
  public:
    virtual int doPush   (MsgBuf<T>*q, T *m, TimeSpec *ts);
    virtual int doPop    (MsgBuf<T>*q, T *m, TimeSpec *ts);

    static IBuffCmd<T> * Inst()
    {
        static QueueCmd<T> inst;
        return &inst;
    }
};

//##############################################################
template<typename T> int _push(MsgBuf<T> *b, T *elem)
{
   int wIdx = b->GetFreeSlot();
     //*b->mssg[wIdx] = *elem; //invoke operator= !!!
   new(b->mssg[wIdx]) T(*elem);//invoke copy-constructor
   return b->MarkPush(wIdx);
}
//=====================================================
template<typename T> int _pop(MsgBuf<T> *b, int rIdx, T *elem)
{
   assert(rIdx != INVALID_IDX);
   *elem = *b->mssg[rIdx];      //invoke T operator=
            b->mssg[rIdx]->~T();//free element from queue
   return b->MarkPop(rIdx);
}
//##############################################################
//Return oldest message in queue
template<typename T> int _nxt(MsgBuf<T> *b, T *elem)
{
   int rIdx = b->GetNext();
   return _pop(b, rIdx, elem);
}

//##############################################################
template<typename T> int QueueCmd<T>::doPush(MsgBuf<T> *b, T *m, TimeSpec *ts)
{
    Atomic a;
    int rc = sem_timedwait(&b->semFree, ts);

    a.Enter();
    if(rc == OK) rc = _push(b, m);
    else         rc = errno;
    a.Leave();

    return rc;
}
//##############################################################
template<typename T> int QueueCmd<T>::doPop(MsgBuf<T> *b, T *m, TimeSpec *ts)
{
    Atomic a;
  //ts = now + ts;
    int rc = sem_timedwait(&b->semUsed, ts);

    a.Enter();
    if(rc == OK) {
       assert(b->semUsedV>0); b->semUsedV--;
       rc = _nxt(b, m);
    }
    else rc = errno;
    a.Leave();

    return rc;
}

#endif
