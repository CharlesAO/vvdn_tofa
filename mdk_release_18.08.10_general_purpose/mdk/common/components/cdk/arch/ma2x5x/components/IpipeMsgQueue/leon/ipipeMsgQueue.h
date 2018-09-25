#ifndef _IPIPE_MSG_QUEUE_
#define _IPIPE_MSG_QUEUE_

icEvent *ipipeQueueGetNextOutputSlot(icEventQueue *q);
int32_t  ipipeQueueGetEvent         (icEventQueue *q, icEvent *ev);

#endif
