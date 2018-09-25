///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Pool
///

#ifndef __POOL_H__
#define __POOL_H__

#include <stdio.h>
#include <Flic.h>
#include <Sender.h>
#include <MsgBuf.h>
#include <PoolBase.h>

//####################################################################
//not cool, but want to save additional dummy members
template <typename T> class Pool : public PoolBase<T>
{
  private:
    IAllocator *alloc;    //PoolBase
    uint32_t    nFrames;  //PoolBase
    uint32_t    frmSize;  //PoolBase
    T          *bufDesA;  //allocation
    T          *bufDes;   //use
    RefCnt     *refCnt;   //PoolBase

    void       *refCnt2A; //Allocated
    RefCnt     *refCnt2;  //for Remote side to update

  public:
    bool          shared;
    pthread_t     thread; //created only if [shared]

    uint32_t  *bases;
    MsgBuf    <PoPtr<T>> freeFrm;
    PoolSender<PoPtr<T>> out;

  public:
    void Create(IAllocator *a, uint32_t nFrm, uint32_t fSize, bool sh = false)
    {
    //printf("Pool Create\n");
      shared  = sh;
      this->cpu = swcWhoAmI();
      alloc   = a;
      nFrames = nFrm;
      bufDesA = new T       [nFrm];
      refCnt  = new RefCnt  [nFrm];
      bases   = new uint32_t[nFrm];
      frmSize = fSize;
      freeFrm.Create(nFrm/*, poolBeh*/);

      if(shared){
         assert(OK == posix_memalign(&refCnt2A, 64, DCALGN(sizeof(RefCnt)*nFrm)));
         refCnt2 = (RefCnt*)NonCachedAddr(refCnt2A);
         assert(isNonCachedAddr(this));
      }

     //For shared pools, 'bufDes' needs to be visible on remote side.
     //Using separate BufDesA (A=Allocation so that delete matches new)
      if(shared) bufDes = (T*)NonCachedAddr(bufDesA);
      else       bufDes = bufDesA;

      for(uint32_t i=0; i<nFrm; i++)
      {
        PoPtr<T> pImg;        //block scope
        refCnt[i].cnt = 1;    //initial owner: the Pool
        pImg.ptr   = &bufDes[i];
        pImg.ref   = &refCnt[i];

        if(shared){
          new(&refCnt2[i]) RefCnt();//invoke constructor on the NC storage !
          pImg.ref2  = &refCnt2[i];
        }else{
          pImg.ref2  = NULL;
        }

        pImg->base = a->Alloc(frmSize);
        pImg->size = frmSize;
        bases[i]   = (uint32_t)pImg->base;
        pImg.pool  = this;
        pImg.poolCpu = this->cpu;
        freeFrm.Push(NULL, &pImg);
        assert(pImg->base); //check alloc
      //"pImg" gets destructed here !!!
      }

      out.Init(&freeFrm);
      this->Add(&out, "out");
    }

    void Delete()
    {
       for(uint32_t i=0; i<nFrames; i++){
          alloc->Free((void*)bases[i]);
       }
       freeFrm.Delete();

       delete[] bufDesA;
       delete[] refCnt;
       if(shared) free(refCnt2A);
       delete[] bases;
       this->nIO = 0;
    }

    void Free(PoPtr<T> *ptr){
      freeFrm.Push(NULL, ptr);
    }

    void Start(){
      if(shared)
      {
         assert(OK == sem_init(&this->rmtOp, 0, 0));
         pthread_attr_t attr;
         struct sched_param schParam;
         assert(OK == pthread_attr_init           (&attr));
         assert(OK == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
         assert(OK == pthread_attr_setschedpolicy (&attr, SCHED_RR));
         assert(OK == pthread_attr_getschedparam (&attr, &schParam));
         schParam.sched_priority = RMT_DEFAULT_POOL_PRIORITY;
         assert(OK == pthread_attr_setschedparam  (&attr, &schParam));
         assert(OK == pthread_create(&thread, &attr, Pool<T>::rmtEventsFunc, this));
         assert(OK == pthread_attr_destroy   (&attr));
      }
    }

    void Stop(){
        if(shared){
           assert(0 == sem_destroy(&this->rmtOp));
        }
        freeFrm.Destroy();
    }

    void ShowRefCounts() //debug
    {
      printf("############# Ref Counts #############\n");
      for(uint32_t i=0; i<nFrames; i++)
        printf("refCnt[%ld] = %x\n", i, refCnt[i].cnt);
      printf("\n");
    }

    int FreeCnt(){ //debug
        return freeFrm.Count();
    }
};

//====================================================================
//the "typedef" for templates:
template <typename T> using PlgPool = Pool<T>;

#endif
