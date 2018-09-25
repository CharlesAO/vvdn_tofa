///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MsgBuffer : Overwrite Oldest PUSH policy
///

#ifndef __BEH_QUEUE_OVR_H__
#define __BEH_QUEUE_OVR_H__

#include "rtems.h"
#include "BehQueue.h"

//##############################################################
template <typename T> class QueueCmdOvr : public QueueCmd<T>
{
  public:
    int doPush(MsgBuf<T>*q, T *m, TimeSpec *ts);

    static IBuffCmd<T> * Inst()
    {
        static QueueCmdOvr<T> inst;
        return &inst;
    }
};


//##############################################################
template<typename T> int QueueCmdOvr<T>::doPush(MsgBuf<T> *b, T *m, TimeSpec *ts)
{
    UNUSED(ts);

    Atomic a;
    T tmp;
    a.Enter();
    //if buff full, free a slot:
    if(b->Count() == (int)b->nElem) {
        QueueCmd<T>::doPop (b, &tmp, ts);
    }
    //now can safely insert new element:
    QueueCmd<T>::doPush(b, m, ts);
    a.Leave();

    return OK;
}

#endif
