///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter Leon comms - irqs/ipc
///

#if defined(FLIC_2LEONS)

#include <FlicIpc.h>
#include <assert.h>
#include <bsp.h>
#if defined(MA2X8X)
#include <OsDrvDynIrq.h>
#endif

//invalid IRQ number
#define INV 0xFFFFFFFF

#ifndef DYN_IRQ_ASSIGNMENT
#define DYN_IRQ_ASSIGNMENT 0 //0 for static alloc
#endif

//#######################################################################
//  Defininig the call struct on LRT, as it's visible on LOS
//as well this way (via lrt_ prefix)
//  Once ACK-ed, don't rely on inpCall/outCall content aymore !!!
//as another pending transaction might launch, if you need something,
//buffer locally before ACK-ing

#if defined(LEON_RT)  // MUST_BE_NON_CACHED !!!
           RmiCtx   rmiCall_0 ALIGNED(128);
           RmiCtx   rmiCall_1 ALIGNED(128);
  volatile RmiCtx * inpCall = (RmiCtx*)NonCachedAddr(&rmiCall_1);
           RmiCtx * outCall = (RmiCtx*)NonCachedAddr(&rmiCall_0);
#else
  extern   RmiCtx  lrt_rmiCall_0;
  extern   RmiCtx  lrt_rmiCall_1;
  volatile RmiCtx * inpCall = (RmiCtx*)NonCachedAddr(&lrt_rmiCall_0);
           RmiCtx * outCall = (RmiCtx*)NonCachedAddr(&lrt_rmiCall_1);
#endif


//#######################################################################
//FLIC requires 2 IRQs per leon, which may not concide, depending on how
//resources get allocated
#define REQ 0
#define ACK 1

#if defined(LEON_RT)
 uint32_t flicIrq_0[2] SECTION(".cmx_direct.data") ALIGNED(128) = {INV,INV};
 uint32_t flicIrq_1[2] SECTION(".cmx_direct.data") ALIGNED(128) = {INV,INV};
 uint32_t *locIrq = flicIrq_0;
 uint32_t *rmtIrq = flicIrq_1;
#else
 extern uint32_t lrt_flicIrq_0[2];
 extern uint32_t lrt_flicIrq_1[2];
        uint32_t *locIrq = lrt_flicIrq_1;
        uint32_t *rmtIrq = lrt_flicIrq_0;
#endif

sem_t           FlicRmt::callAck;
pthread_mutex_t FlicRmt::callMtx;

//#######################################################################
//Incomming call IRQ
void FlicRmt::rmiIncCallHndl(void *)
{
  //wakeup target waiting thread:
    sem_post(inpCall->semToWake);
    BSP_Clear_interrupt(locIrq[REQ]);
  //the thread is then responsible to ACK the call
}

//#######################################################################
//Remote confirmed the call
void FlicRmt::rmiCallAckHndl(void *)
{
    sem_post(&callAck);
    BSP_Clear_interrupt(locIrq[ACK]);
}

//#######################################################################
void FlicRmt::Init()
{
   assert(OK == pthread_mutex_init(&callMtx, NULL));
   assert(OK == sem_init          (&callAck, 0, 0));
   assert((sizeof(RmiCtx) & 63) == 0);

  //IRQ handlers
  #if (defined(MA2X8X) && DYN_IRQ_ASSIGNMENT)
    assert(OK == OsDrvDynIrqRequest(&locIrq[REQ]));
    assert(OK == OsDrvDynIrqRequest(&locIrq[ACK]));
  //printf("DYN req=%ld, ack=%ld\n",locIrq[REQ], locIrq[ACK]);
  #else
    locIrq[REQ] = FLIC_RMI_REQ_IRQ;
    locIrq[ACK] = FLIC_RMI_ACK_IRQ;
  #endif

   BSP_Clear_interrupt(locIrq[REQ]);
   BSP_Set_interrupt_type_priority  (locIrq[REQ], POS_LEVEL_INT, FLIC_RMI_IRQ_PRI);
   assert(OK==BSP_interrupt_register(locIrq[REQ], NULL, rmiIncCallHndl, NULL));

   BSP_Clear_interrupt(locIrq[ACK]);
   BSP_Set_interrupt_type_priority  (locIrq[ACK], POS_LEVEL_INT, FLIC_RMI_IRQ_PRI);
   assert(OK==BSP_interrupt_register(locIrq[ACK], NULL, rmiCallAckHndl, NULL));

   if(FLIC_RMI_EN_BARR) {void BarInit(); BarInit();}
}

//#######################################################################
void FlicRmt::Destroy()
{
  assert(OK == pthread_mutex_destroy(&callMtx));
  assert(OK ==           sem_destroy(&callAck));

  #if (defined(MA2X8X) && DYN_IRQ_ASSIGNMENT)
   assert(OK == OsDrvDynIrqRelease(locIrq[REQ]));
   assert(OK == OsDrvDynIrqRelease(locIrq[ACK]));
  #endif

  assert(OK == BSP_interrupt_unregister(locIrq[REQ], rmiIncCallHndl, NULL));
  assert(OK == BSP_interrupt_unregister(locIrq[ACK], rmiCallAckHndl, NULL));

  locIrq[REQ] = INV;
  locIrq[ACK] = INV;

  if(FLIC_RMI_EN_BARR) {void BarKill(); BarKill();}
}

//#######################################################################
void FlicRmt::RmiCall(RmiCtx *ctx) //towards remote
{
  assert(rmtIrq[REQ] != INV);
  assert(!rtems_interrupt_is_in_progress());
  pthread_mutex_lock  (&callMtx);
  //1) write to NC addr space
  *outCall = *ctx;  //who to deblock
  //2) readback to make sure write is done
  if(FLIC_READBACK){
    RmiCtx *rdBack = outCall;
    while(!(*rdBack == *ctx))
    {NOP;NOP;NOP;NOP;NOP;NOP;}
  }
  BSP_Force_remote_interrupt(rmtIrq[REQ]);
  //Wait for remote to confirm the call
  sem_wait(&callAck);
  pthread_mutex_unlock(&callMtx);
  //Call line is free now :)
}

//#######################################################################
//Incomming call/resp ack
void FlicRmt::RmiAck()
{
   assert(rmtIrq[ACK] != INV);
   BSP_Force_remote_interrupt(rmtIrq[ACK]);
}
#endif
