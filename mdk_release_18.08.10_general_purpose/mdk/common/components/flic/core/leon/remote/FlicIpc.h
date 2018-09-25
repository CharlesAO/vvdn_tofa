///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter Leon comms - irqs/ipc
///

#ifndef __FLIC_IPC_H__
#define __FLIC_IPC_H__

#include <Flic.h>

#ifndef FLIC_RMI_REQ_IRQ //for CALL, RESP
#define FLIC_RMI_REQ_IRQ 53
#endif

#ifndef FLIC_RMI_ACK_IRQ //ack for CALL, RESP
#define FLIC_RMI_ACK_IRQ 54
#endif

#ifndef MAX_RMI_PARAMS
#define MAX_RMI_PARAMS 4
#endif

#ifndef FLIC_RMI_IRQ_PRI
#define FLIC_RMI_IRQ_PRI 12
#endif

//read-back-check after a write towards the other Leon
//(via non-cached space)
#ifndef FLIC_READBACK
#define FLIC_READBACK 1
#endif

#ifndef FLIC_IPC_DEFAULT_THREAD_PRIORITY
#define FLIC_IPC_DEFAULT_THREAD_PRIORITY 240
#endif

//########################################################################
class RmiCtx
{
  public:
   sem_t     *semToWake; //sem created on remote side
   uint32_t   params[4]; //params, return codes, ...
   uint32_t   methodId;
   uint32_t   _fill[10]; //fill so that sizeof(RmiCtx) divs by 64
};

inline bool operator==(const RmiCtx& lhs, const RmiCtx& rhs)
{
   if((lhs.semToWake == rhs.semToWake) &&
      (lhs.params[0] == rhs.params[0]) &&
      (lhs.params[1] == rhs.params[1]) &&
      (lhs.params[2] == rhs.params[2]) &&
      (lhs.params[3] == rhs.params[3]) &&
      (lhs.methodId  == rhs.methodId))
   return      true;
   else return false;
}

extern volatile RmiCtx * inpCall;
extern          RmiCtx * outCall;

class FlicRmt
{
  private:
 //All requests are multiplexed on a single inter-Leon IRQ line,
 //so gets protected by a mutex, also a single deblocking sem is needed
   static sem_t           callAck; //call ack (remote confirms the call)
   static pthread_mutex_t callMtx; //protect the current call

   static void rmiIncCallHndl(void *);
   static void rmiCallAckHndl(void *);

  public:
   static void Init();
   static void Destroy();
   static void RmiCall(RmiCtx *ctx);
   static void RmiAck();
};

#endif
