///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     FLIC main header
///

#ifndef __FLIC_H__
#define __FLIC_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <rtems.h>
#include <rtems/timespec.h>
#include <pthread.h>
#include <semaphore.h>
#include <FlicAtomic.h>
#include "flicDefs.h"
#include <mv_types.h>
#include <swcWhoAmI.h>
#include <swcLeonUtilsDefines.h> //for NOP
extern "C"{
#define _GNU_SOURCE
#include "pthread.h"
int pthread_setname_np(pthread_t thread, const char *name);
}

//Helpers
typedef timespec TimeSpec;
extern  TimeSpec TS_INF; //for blocking waits
extern  TimeSpec TS_TRY; //for try-operations

typedef uint64_t FlicName; //8 chars

#define EMPTY_SLOT 0
#define INVALID_IDX -1 //invalid array pos

typedef union
{
    uint64_t d64;
    uint32_t d32[2];
}uni64;

struct NamedId{
    uint32_t    v;
    const char *s; //TBD: better use FlicName ?
    NamedId(const char *des);
    NamedId(){s = NULL;};
};

class IAllocator{
  public:
    virtual void *Alloc(size_t size) = 0;
    virtual void  Free (void  *ptr)  = 0;
};

class ISync{
  public:
    virtual int  Notify ()=0;
    virtual int  Wait   ()=0;
    virtual void Destroy()=0;
};

//###############################################################################
//early declaration
                      class IPlugin;
template <typename T> class MsgBuf;

class Io { //Receiver/Sender base
  public:
    IPlugin  *par;  //parent (to check all IOs were added to plugin)
    FlicName  name; //current IO name (string)
    Io(){par = NULL;}
    virtual ~Io(){}

    virtual void Destroy() {};
    virtual void Delete () {};
};

class LogBase {
  //for User to add custom messages from Plugin body
  public: virtual void Print  (const char *fmt, ...)  = 0;
};

template <typename T> class IBuffLog : public LogBase
{
  public:
    virtual void LogPushBegin(Io *, T *) {}
    virtual void LogPopBegin (Io *     ) {}
  //automatically called by Flic
    virtual void LogPushEnd(Io *w, T *m, int rc) = 0;
    virtual void LogPopEnd (Io *w, T *m, int rc) = 0;
};

//Command pattern attempt
template <typename T> class IBuffCmd
{
 public:
   virtual int doPush(MsgBuf<T>*q, T *m, TimeSpec *relTs) = 0;
   virtual int doPop (MsgBuf<T>*q, T *m, TimeSpec *relTs) = 0;
};


//###############################################################################
class BufferBase
{
  public:
    uint32_t   nElem;   //number of elements
    uint32_t  *order;
    uint64_t  *ts;
    uint32_t   pushIdx; //increments on each push

  //Push/Pop sync
    sem_t      semFree; //counting free frames
    sem_t      semUsed; //counting used frames
    int        semUsedV;//logical value for semUsed

    void Init   (size_t n);
    void Free();
    void Destroy(); //breaks OS primitives to de-block waiting threads
    int  Count  (); //ret number of valid elements in buff
    int  GetNext();
    int  GetFreeSlot();
    int  MarkPop( int rIdx);
    int  MarkPush(int wIdx);
    int  ClosestTs(uint64_t ts);
    virtual ~BufferBase(){}
};

//Following thin-templates paradigma
template <typename T> class MsgBuf : public BufferBase
{
  private:
    IBuffLog<T> *log;
    IBuffCmd<T> *exec; //allow custom default push/pop behaviour

  public :
    uint64_t *storage; //static storage for messages in msg buff
    T        **mssg;

    MsgBuf() {log = NULL; storage = NULL;}
    virtual ~MsgBuf(){}
    virtual void Create (size_t numElem, IBuffCmd<T> *beh = NULL);
    virtual void Delete (); //free mem
            void SetBehavior(IBuffCmd<T> *beh);
            void SetLog     (IBuffLog<T> *loG) {log = loG;}

  //Top API that including logging ("c" allows customized push/pop)
    int Push   (Io *who, T *m, IBuffCmd<T> *c = NULL); //back
    int Pop    (Io *who, T *m, IBuffCmd<T> *c = NULL); //front
    int TryPush(Io *who, T *m, IBuffCmd<T> *c = NULL);
    int TryPop (Io *who, T *m, IBuffCmd<T> *c = NULL);
  //Timed API:
    int Push   (Io *who, T *m, TimeSpec *ts, IBuffCmd<T> *c = NULL);
    int Pop    (Io *who, T *m, TimeSpec *ts, IBuffCmd<T> *c = NULL);

   private:
    int ThePush(Io *who, T *m, IBuffCmd<T> *c, TimeSpec *ts);
    int ThePop (Io *who, T *m, IBuffCmd<T> *c, TimeSpec *ts);
};

//==========================================================================
//System stuff
class Flic
{
  public:
    static uint32_t GetNextId();
    static FlicName Name(const char *str);
};

class Pipeline
{
 private:
   IPlugin **plugins;
   uint32_t nPlugins;
   uint32_t mPlugins; //max plugins

 public:
            Pipeline(int maxPlugs = MAX_PLUGS_PER_PIPE);
   virtual ~Pipeline();

   virtual void Add(IPlugin  *plg, const char *name = NULL);

   virtual void Delete();
   virtual void Start ();
   virtual void Stop  ();
   virtual void Wait  ();
            int Has   (IPlugin *plg);

   FlicName     name;
   uint32_t     alive;
   LogBase     *log;
};

//Note: a plugin can invoke whole pipe teardown via pl->Stop
class IPlugin
{
  public:
     Pipeline *pl;      //pipe it belongs to
     NamedId   instId;  //instance ID (init in constructor)
     FlicName  name;

   //Track all senders/receivers for Plugin destroy
     Io      **pIO;
     uint32_t  nIO; //num IOs
     uint32_t  mIO; //max IOs

     void      Add(Io *r, const char *n = NULL);

  public:
      IPlugin(int maxIOs = MAX_IOS_PER_PLUG);
     ~IPlugin();

     virtual void Start (){}; //
     virtual void Join  (){}; //N/A for thread-less plug
     virtual void Stop  ();
     virtual void Delete();
     virtual uint32_t Alive();
};

class ThreadedPlugin : public IPlugin
{
  public: //intended private:
    pthread_t thread;
    struct sched_param schParam;
    int policy;
  private:
    static void * InternalThreadEntryFunc(void * This);
  protected:
    virtual void *threadFunc(void *param) = 0; //user to implement
  public:
                 ThreadedPlugin();
    virtual void Start();
    virtual void Join ();
            void SetThreadName(const char *name = NULL);
};

class PlgGroup {
 public: virtual void AddTo(Pipeline *p) = 0;
 public: virtual ~PlgGroup() {};
};


void CheckParent(Io *io);
uint32_t   NonCachedAddr(void *vAddr);
uint32_t isNonCachedAddr(void *vAddr);
swcProcessorType ThisCpu();
TimeSpec GetAbsTs(const TimeSpec *ts, TsType tt);

//new placement
void * operator new (size_t size, void *storage) _GLIBCXX_USE_NOEXCEPT;

#endif
