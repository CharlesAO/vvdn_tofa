///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Message buffer base class
///

#include "Flic.h"
#include <rtems/timespec.h>
#include <mv_types.h>

TimeSpec TS_INF = {.tv_sec =  TIME_MAX, .tv_nsec = 0}; //"infinite" wait
TimeSpec TS_TRY = {.tv_sec =         0, .tv_nsec = 0}; //no wait

//##############################################################
void * operator new (size_t size, void *storage) _GLIBCXX_USE_NOEXCEPT
{
    UNUSED(size);
    return storage;
}

//##############################################################
void BufferBase::Init(size_t numElem)
{
    nElem = numElem;

    order  = new uint32_t[numElem];
    ts     = new uint64_t[numElem];

    for(uint32_t i=0; i< nElem; i++)
       order[i] = EMPTY_SLOT;

    pushIdx = EMPTY_SLOT + 1;

  //Shared between processes, initial value
    assert(0 == sem_init(&semFree, 0, nElem));
    assert(0 == sem_init(&semUsed, 0,     0)); semUsedV = 0;
}

//##############################################################
void BufferBase::Free()
{
    delete[] order;
    delete[] ts;
}
//##############################################################
void BufferBase::Destroy()
{
   assert(0 == sem_destroy(&semFree));
   assert(0 == sem_destroy(&semUsed));
}

//##############################################################
int BufferBase::Count()
{
    //int nUsed;
    //sem_getvalue(&semUsed, &nUsed);
    //return nUsed;
    return semUsedV;
}

//##############################################################
//returns oldest valid message in queue
//based on "push-order" value
//(smallest order => oldest message)
int BufferBase::GetNext()
{
         int oldest    = INVALID_IDX;
    uint32_t oldestAge = 0xFFFFFFFF;

   //Search for oldest message (min order wins)
    for(uint32_t i=0; i< nElem; i++)
     if(order[i] != EMPTY_SLOT) //skip blanks
      if(order[i] < oldestAge){
          oldest    = i;
          oldestAge = order[i];
      }
    return oldest;
}

//##############################################################
//Note: called in atomic section if we're sure this will
//      succeed (see failing asserts at the bottom)
int BufferBase::GetFreeSlot()
{
    for(uint32_t i=0; i< nElem; i++)
     if(order[i] == EMPTY_SLOT)
      return i;
  //else
    assert(0);
    return -1;
}

//##############################################################
//rIdx = position from where Pop is done (data is read and copied to user space)
int BufferBase::MarkPop(int rIdx)
{
 //this message got removed from queue, mark this position
 //as FREE to be filled by future PUSH commands
   order[rIdx] = EMPTY_SLOT;
   int rc = sem_post(&semFree);
   return rc;
}

//##############################################################
//wIdx = position where push was done
int BufferBase::MarkPush(int wIdx)
{
   order[wIdx] = pushIdx++;
   int rc = sem_post(&semUsed);
   if(rc == OK) semUsedV++;
   return rc;
}

//##############################################################
//find message with timestamp closest to target ts (tts)
int BufferBase::ClosestTs(uint64_t tts)
{
    int      minPos   = INVALID_IDX;
    uint64_t minDelta = 0xFFFFFFFFFFFFFFFFLL; //TBD: use the u64_max
    for(uint32_t i=0; i< nElem; i++)
    {
      uint64_t mTs = ts[i];

      if(order[i] != EMPTY_SLOT){ //skip blanks
        uint64_t delta = (mTs > tts) ? (mTs - tts) : (tts - mTs);
        if(delta < minDelta){
            minDelta = delta;
            minPos   = i;
        }
      }
    }
    return minPos;
}
