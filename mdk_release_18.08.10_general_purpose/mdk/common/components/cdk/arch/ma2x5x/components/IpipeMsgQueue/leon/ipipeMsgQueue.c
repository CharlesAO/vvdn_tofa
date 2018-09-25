#include "ipipe.h"
#include "assert.h"
#include "string.h"
#include <swcLeonUtils.h>

#if defined(__RTEMS__)
#include <rtems.h>
#else
#include <swcLeonUtils.h>
#endif

int criticalSectionEnter(void)
{
    int key;
#if  defined(__RTEMS__)
    rtems_interrupt_disable(key);
#else //defined(__RTEMS__)
    key = swcLeonSetPIL(15);
#endif //defined(__RTEMS__)
    return key;
}

void criticalSectionExit(int key)
{
#if  defined(__RTEMS__)
    rtems_interrupt_enable(key);
#else //defined(__RTEMS__)
    swcLeonSetPIL(key);
#endif //defined(__RTEMS__)
}

//#############################################################################
icEvent *ipipeQueueGetNextOutputSlot(icEventQueue *q)
{
    icEvent *ret;
   u32 key;

    //Check that slot is free : if owned, it means consummer didn't process it
    //yet, so we just overflowed the event circular queue: this is a fatal-error
    //we should always check this condition
   key = criticalSectionEnter();
    assert(IC_EVENT_CTRL_OWNED != (q->eventQ[q->writeIdx].ctrl & IC_EVENT_CTRL_OWNED));

 //else return a ptr to that entry
   ret = &q->eventQ[q->writeIdx];

 //Move to next location (for subsequent call)
   if(++q->writeIdx == IC_EVENT_QUEUE_SIZE)
        q->writeIdx = 0; //circular logic

   criticalSectionExit(key);

   return ret;
}


//Return values:
//   0 : no new event
//   1 : got new event and got copied at "*ev"
int32_t ipipeQueueGetEvent(icEventQueue *q, icEvent *ev)
{
  //Get a ref to next message to be read
   volatile icEvent *evPtr = &q->eventQ[q->readIdx];


    //Just check if we own next message
     if((evPtr->ctrl & IC_EVENT_CTRL_OWNED) == 0)
        return 0;//no events


  //We got an input event :)
  memcpy((void*)ev, (void*)evPtr, sizeof (*ev));

  //Release the ring buffer entry back to the producer
  //(this clears ownership bit as well, thus marked as "CONSUMED")
    evPtr->ctrl = 0;

  //Advance the ring queue read ptr
    if (++q->readIdx == IC_EVENT_QUEUE_SIZE) {
          q->readIdx = 0;
    }

    return 1; //return event is valid
}


//#############################################################################
// "*ev" needs to be obtained previously with "ipipeQueueGetNextOutputSlot"
//void ipipeQueueSendEvent(icEvent *ev)
//{
   //ev->ctrl |= IC_EVENT_CTRL_OWNED; //consummer owns this from now on
   //DrvIcbIrqRemoteTrigger(INTER_CPU_INT);
//}


