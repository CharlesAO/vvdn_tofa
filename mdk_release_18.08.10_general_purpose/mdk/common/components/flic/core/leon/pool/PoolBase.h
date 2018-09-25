///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Pool / Shared-pointer basics
///

#ifndef __SH_BASE_H__
#define __SH_BASE_H__

#include <stdio.h>
#include <Flic.h>
#include <Sender.h>
#include <MsgBuf.h>
#include <FlicIpc.h>

//Remote methods defs
#define RMT_MET_INC_REF 1
#define RMT_MET_DEC_REF 2
#ifndef RMT_DEFAULT_POOL_PRIORITY
#define RMT_DEFAULT_POOL_PRIORITY FLIC_IPC_DEFAULT_THREAD_PRIORITY
#endif

//####################################################################
//Pool Sender : P should be a PoPtr<F>, where F is the user frame descriptor
template <typename P> class PoolSender : public SSender<P>
{
   private:
    MsgBuf<P> *freeFrm;

   public:
    void Init   (MsgBuf<P> *freeF) { freeFrm = freeF;  }
    int  Pull   (Io *who, P *msg, TimeSpec *ts, IBuffCmd<P> *c){
        return freeFrm->Pop(who, msg, ts, c);
    }

    void Destroy() { }
    void Delete () { }
};

//####################################################################
class RefCnt
{
  friend class RefBase;
//private:
  public:
    int  cnt; // ref count

  public:
    RefCnt() {cnt = 0;}

    int IncRef()
    {
      //printf("_inc A=0x%lx V=%d\n", (uint32_t)&cnt, cnt);
       rtems_interrupt_level   level;
       rtems_interrupt_disable(level);
       cnt++;
       int ret = cnt;
       rtems_interrupt_enable(level);
       return ret;
    }

    int DecRef()
    {
       assert(cnt > 0);
       rtems_interrupt_level   level;
       rtems_interrupt_disable(level);
       cnt--;
       int ret = cnt;
       rtems_interrupt_enable(level);
      //printf("_dec A=0x%lx V=%d\n", (uint32_t)&cnt, cnt);
       return ret;
    }
};
//####################################################################
class RefBase
{
  public:
    RefCnt    *ref; // Reference cnt (LOCAL)
    RefCnt    *ref2;// Reference cnt (REMOTE)
    RefBase() {ref = ref2 = NULL;}
    const char *name; //debug
};

template <typename T> class PoPtr;

//####################################################################
template <typename T> class PoolBase : public IPlugin
{
   public:
     swcProcessorType cpu;
     sem_t          rmtOp; //created only if [shared]
     virtual void Free(PoPtr<T> *obj) = 0;
     static void * rmtEventsFunc(void *This);
};

//####################################################################
//Pool-Object-Ptr (a sort of smart pointer)
template <typename T> class PoPtr : public RefBase
{
public:
    T           *ptr;
    PoolBase<T> *pool; //need to know who I belong to
    uint32_t     poolCpu;

public:
    PoPtr(                  ) {pool = NULL; ptr = NULL; name = NULL  ; poolCpu = 0;}
    PoPtr(const char *szName) {pool = NULL; ptr = NULL; name = szName; poolCpu = 0;}

  //Cool stuff:
    T& operator*  () {return *ptr;}
    T* operator-> () {return  ptr;}

    void  RmtIncRef(); //only for Shared pools
    void  RmtDecRef();

private:
    //=================================================================
    void CpuCheck()
    {
      //assert(poolCpu == pool->cpu);
      if(poolCpu != pool->cpu){
         printf("poolCpu=%ld pool->cpu%d\n", poolCpu, pool->cpu);
         assert(0);
      }

      if ((poolCpu != PROCESS_LEON_OS) &&
          (poolCpu != PROCESS_LEON_RT))
      {
          printf("ERROR: invalid CPU : %d (pool @ 0x%lx)\n", pool->cpu, (uint32_t)pool);
          assert(0);
      }
    }
    //=================================================================
    void UnRefOld()
    {
     //NOTE: in case of super-frame other.ptr/ref/pool could be NULL
      if(ref && pool){
        CpuCheck();
        if(poolCpu == ThisCpu())
        {
          if(ref->DecRef() == 0)
            pool->Free(this);
        }
        else{
         if(!USE_OPT_POPTR){
           RmtDecRef();//RMI
         }else{
           if(ref2->DecRef() == 0)
            RmtDecRef();//RMI
         }
        }
      }
    }
    //=================================================================
    void RefNew()
    {
      if(ref && pool){
       CpuCheck();
       if(poolCpu == ThisCpu())
          ref->IncRef();
       else{
         if(!USE_OPT_POPTR){
           RmtIncRef();//RMI
         }else{
           assert(isNonCachedAddr(pool) && "Original pool is not shared !");
           if(ref2->IncRef() == 1) //was 0, now 1
            RmtIncRef();
         }
       }
      }
    }

public:
    //=================================================================
    PoPtr<T>(const PoPtr<T>& other)
    {
        //DPRINTF("COPY_CONSTRUCTOR\n");
        ptr  = other.ptr;
        ref  = other.ref;
        ref2 = other.ref2;
        pool = other.pool;
        poolCpu = pool ? pool->cpu : 0;
        RefNew();
    }

    //=================================================================
    PoPtr<T>& operator = (const PoPtr<T>& other) // Assignment operator
    {
        if(this != &other) // Avoid self assignment
        {
          //Ownership changes here:
          //1) release what we referenced so far
            UnRefOld();

          //2) and from now on, we'll refer what 'other' refers
            ptr  = other.ptr;
            ref  = other.ref;
            ref2 = other.ref2;
            pool = other.pool;
            poolCpu = pool ? pool->cpu : 0;
            RefNew();
        }
        return *this;
    }
    //=================================================================
    void Reset() {
        UnRefOld();
    }
    //=================================================================
    ~PoPtr<T>()
    {
        Reset();
    }
};

//########################################################################
//  _____  ______ __  __  ____ _______ ______       _          __  __
// |  __ \|  ____|  \/  |/ __ \__   __|  ____|     | |        / _|/ _|
// | |__) | |__  | \  / | |  | | | |  | |__     ___| |_ _   _| |_| |_
// |  _  /|  __| | |\/| | |  | | | |  |  __|   / __| __| | | |  _|  _|
// | | \ \| |____| |  | | |__| | | |  | |____  \__ \ |_| |_| | | | |
// |_|  \_\______|_|  |_|\____/  |_|  |______| |___/\__|\__,_|_| |_|
//
//########################################################################

//Handle remote PoPtr IncRef/DecRef, this thread only gets started
//for SHARED pools
template <typename T> void * PoolBase<T>::rmtEventsFunc(void *This)
{
  #if defined(FLIC_2LEONS)
   pthread_setname_np(RTEMS_SELF, "rmtPool");
   PoolBase<T> *me = (PoolBase<T>*)This;
   while(me->Alive())
   {
      sem_wait(&me->rmtOp);
      switch(inpCall->methodId)
      {
       case RMT_MET_INC_REF:
       {
            RefCnt *ref = (RefCnt*)inpCall->params[0];
            ref->IncRef();
            FlicRmt::RmiAck();
            break;
       }
       case RMT_MET_DEC_REF:             //Remote part is done with this obj
       {
            T           *ptr = (T          *)inpCall->params[0];
            RefCnt      *ref = (RefCnt     *)inpCall->params[1];
            PoolBase<T> *pool= (PoolBase<T>*)inpCall->params[2];
            RefCnt      *ref2= (RefCnt     *)inpCall->params[3];
            assert(pool == me); //paranoia check

            //TBD: name

            if(ref->DecRef() == 0)
            {
              //Local obj to be pushed
               PoPtr<T> p;
               p.pool = me;
               p.poolCpu = me->cpu;
               p.ref  = ref;
               p.ref2 = ref2;
               p.ptr  = ptr;
               me->Free(&p);

              //Cancel local obj destruction, as point here is to do a Push,
              //not a Push followed by a Pop (thus nothing)
               p.pool = NULL;
               p.ref  = NULL;
            }
            FlicRmt::RmiAck();
            break;
       }
       default:
       {
            assert(0);
            break;
       }
      }//switch
   }//while

  #else
   UNUSED(This);
  #endif

   return NULL;
}

//####################################################################
//Remote communication is the same, no matter what the Pool looks like
//But the final release is customizable !!!

template <typename T> void PoPtr<T>::RmtIncRef()
{
 #if defined(FLIC_2LEONS)
 //Note: "pool" is remotely defined/instantiated
 //assert(this->pool->shared == true);
   RmiCtx ctx;
   ctx.semToWake = &pool->rmtOp;
   ctx.methodId  = RMT_MET_INC_REF;
   ctx.params[0] = (uint32_t)this->ref;
   FlicRmt::RmiCall(&ctx);
 #endif
}

template <typename T> void PoPtr<T>::RmtDecRef()
{
  #if defined(FLIC_2LEONS)
 //Note: "pool" is remotely defined/instantiated
 //assert(this->pool->shared == true);
   RmiCtx ctx;
   ctx.semToWake = &pool->rmtOp;
   ctx.methodId  = RMT_MET_DEC_REF;
   ctx.params[0] = (uint32_t)ptr;
   ctx.params[1] = (uint32_t)ref;
   ctx.params[2] = (uint32_t)pool;
   ctx.params[3] = (uint32_t)ref2;
   FlicRmt::RmiCall(&ctx);
  #endif
}

#endif
