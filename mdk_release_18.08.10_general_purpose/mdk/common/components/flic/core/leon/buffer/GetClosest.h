///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MsgBuffer : Closest-Timestamp POP
///

#ifndef _GET_CLOSEST_H__
#define _GET_CLOSEST_H__

#include "Flic.h"

template <typename T> class ClosestTsCmd : public IBuffCmd<T>
{
  public:
    uint64_t targetTs;
    virtual int doPop  (MsgBuf<T>*q, T *m, TimeSpec *ts);

  private: //only exposing Pop functionality above
    virtual int doPush (MsgBuf<T>*, T *, TimeSpec *){return 0;};
};

//##############################################################
//Return ts from given type
//Users need to specify concrete implementations
template<typename T> uint64_t ClosestTsGet(T *m); // { return m->ts; }

template<typename T> int _closestTs(MsgBuf<T> *b, uint64_t tts)
{
   //get all TSs in an array
    for(uint32_t i=0; i< b->nElem; i++)
        b->ts[i] = ClosestTsGet(b->mssg[i]);
   //scan and return the index of element to be popped
    return b->ClosestTs(tts);
}

//Return message with closest TS
template<typename T> int _cts(MsgBuf<T> *b, uint64_t ts, T *elem){
   int rIdx = _closestTs(b, ts);
   return _pop(b, rIdx, elem);
}

template<typename T> int ClosestTsCmd<T>::doPop(MsgBuf<T> *b, T *m, TimeSpec *ts)
{
    UNUSED(ts);
    int rc;
    Atomic a;

    rc = sem_wait(&b->semUsed);
    a.Enter();
    if(rc == OK){
      assert(b->semUsedV>0); b->semUsedV--;
      rc = _cts(b, targetTs, m);
    }
    a.Leave();
    return rc;
}

#endif