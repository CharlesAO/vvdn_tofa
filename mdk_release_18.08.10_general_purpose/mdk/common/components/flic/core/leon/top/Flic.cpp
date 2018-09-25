///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     FLIC top
///

#include <Flic.h>
#include <stdio.h>
#include <DrvRegUtils.h>

//#####################################################################
//Log Named IDs (usually for constants)
NamedId *ids[MAX_IDS];
uint32_t nId = 0;

//Place-holder for when a name is not specified
FlicName NoName = Flic::Name(".xxxxxxx");

//#####################################################################
uint32_t Flic::GetNextId()
{
    static uint32_t msgID = 1;
    Atomic a;

    a.Enter();
    uint32_t ret = msgID++;
    a.Leave();

    return ret;
}
//=====================================================================
NamedId::NamedId(const char *des)
{
    s = des;
    v = Flic::GetNextId();
    if(nId<MAX_IDS)
       ids[nId++] = this;
}
//=====================================================================
void PrintFlicIds()
{
    printf("-------------------------------------\n");
    printf("Named IDs\n");
    printf("Value: Description\n");
    printf("-------------------------------------\n");
    for(uint32_t i=0; i<nId; i++)
      printf(" %3ld : %s\n", ids[i]->v, ids[i]->s);
    printf("-------------------------------------\n");
}
//=====================================================================
FlicName Flic::Name(const char *s)
{
  FlicName ret = 0LL;
  uint32_t i;

 //copy up to 8 chars
  for(i=0; i<8; i++){
    if(s[i] == 0)
      break;
    ret |= ((uint64_t)s[i])<<(i*8LL);
  }

 //fill remaining chars with spaces
  for(; i<8; i++)
    ret |= ((uint64_t)'_')<<(i*8LL);

  return ret;
}
//#####################################################################
Pipeline::Pipeline(int maxPlugs)
{
  plugins  = new IPlugin* [maxPlugs];
  nPlugins = 0;
  mPlugins = maxPlugs;
  log      = NULL;
}

Pipeline::~Pipeline()
{
  delete[] plugins;
}

void Pipeline::Add(IPlugin *plg, const char *plgName)
{
    assert(nPlugins < mPlugins);
    plugins[nPlugins] = plg;
    plg->pl = this;
    plg->name = plgName ? Flic::Name(plgName) : NoName;
    if(FLIC_PLOG && log)
      log->Print("Pipe [%.8s] ADD_PLG [%.8s]\n", (char*)&name, plgName);
    nPlugins++;
}

void Pipeline::Start()
{
   this->alive = 1;
   if(FLIC_PLOG && log) log->Print("Pipe [%.8s] START\n", (char*)&name);
   for(size_t i=0; i<nPlugins; i++)
      plugins[i]->Start();
}
void Pipeline::Stop()
{
   this->alive = 0;
   if(FLIC_PLOG && log) log->Print("Pipe [%.8s] STOP\n", (char*)&name);
   for(size_t i=0; i<nPlugins; i++)
       plugins[i]->Stop();
}

void Pipeline::Delete()
{
   assert(alive == 0);
   if(FLIC_PLOG && log) log->Print("Pipe [%.8s] DELETE\n", (char*)&name);
   for(size_t i=0; i<nPlugins; i++)
       plugins[i]->Delete();
   nPlugins = 0;
}

void Pipeline::Wait()
{
   if(FLIC_PLOG && log) log->Print("Pipe [%.8s] WAIT\n", (char*)&name);
   for(size_t i=0; i<nPlugins; i++)
      plugins[i]->Join();
   this->alive = 0; //needed here ?
   if(FLIC_PLOG && log) log->Print("Pipe [%.8s] SOTPPED.\n", (char*)&name);
}

//return 1 if Pipeline contains plg, 0 otherwise
int Pipeline::Has(IPlugin *plg)
{
   for(size_t i=0; i<nPlugins; i++)
    if(plugins[i] == plg)
     return 1;
  //else:
   return 0;
}

//#####################################################################
IPlugin::IPlugin(int maxIOs)
{
   pl  = NULL;
   pIO = new Io* [maxIOs];
   nIO = 0;
   mIO = maxIOs;
   
   instId.v = Flic::GetNextId();
 //name to be set by app !!
}

IPlugin::~IPlugin()
{
    delete[] pIO;
}

uint32_t IPlugin::Alive(){
  //assert(pl!= NULL); //TBD: should make it NULL in constructor
    return pl->alive;
}

//Invoked by Senders/Receivers for record
void IPlugin::Add(Io *s, const char *n)
{
    assert(nIO < mIO);
    s->par = this;
    s->name = n ? Flic::Name(n) : NoName;
    pIO[nIO++] = s;
}

ThreadedPlugin::ThreadedPlugin()
{
  schParam.sched_priority = 0;//reserved
  policy = SCHED_RR;
}

void * ThreadedPlugin::InternalThreadEntryFunc(void * This)
{
    ((ThreadedPlugin *)This)->threadFunc(This);
    return NULL;
}

void ThreadedPlugin::Start()
{
   pthread_attr_t attr;

   assert(OK == pthread_attr_init           (&attr));
   assert(OK == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
   assert(OK == pthread_attr_setschedpolicy (&attr, policy));

 if(schParam.sched_priority)
   assert(OK == pthread_attr_setschedparam  (&attr, &schParam));

   assert(OK == pthread_create(&thread, &attr, ThreadedPlugin::InternalThreadEntryFunc, this));
   assert(OK == pthread_attr_destroy   (&attr));
};

void ThreadedPlugin::Join() {
    pthread_join(thread, NULL);
}
void ThreadedPlugin::SetThreadName(const char *name)
{
    if(instId.s){
        pthread_setname_np(RTEMS_SELF, instId.s);
    }
    else {
        if(name){
            pthread_setname_np(RTEMS_SELF, name);
        }
        else{
            char nm[_Thread_Maximum_name_size];
            snprintf (nm, _Thread_Maximum_name_size, "Plg%ld", instId.v);
            pthread_setname_np(RTEMS_SELF, nm);
        }
    }
}

void IPlugin::Stop()
{
    uint32_t i;
    for(i=0; i<nIO; i++) pIO[i]->Destroy();
}

void IPlugin::Delete()
{
    uint32_t i;
    for(i=0; i<nIO; i++) pIO[i]->Delete();
    nIO = 0;
}

//Check that current IO was added to Plugin container via an "Add" method
void CheckParent(Io *io)
{
  #if defined(CANCEL_IO_PARENT_CHECK)
    UNUSED(io);
  #else
    IPlugin *par = io->par;
  //a) check it has a parent
    assert(par);
  //b) check parent contains the "io"
    int found = INVALID_IDX;
    for(uint32_t i=0; i<par->nIO; i++)
     if(par->pIO[i] == io){
        found = i;
        break;
     }
    assert(found != INVALID_IDX);
  //c) check plugin belongs to pipe:
    Pipeline *p = par->pl;
    assert(p);
    assert(p->Has(par));
  #endif
}

uint32_t NonCachedAddr(void *vAddr)
{
    uint32_t addr = (uint32_t)vAddr;
    if((addr >> 28) == 0x7) return addr | 0x08000000; //CMX addr
    if((addr >> 30) == 0x2) return addr | 0x40000000; //DDR addr
    return addr; //else ?
}

//to detect CMX or DDR non-cached addr
uint32_t isNonCachedAddr(void *vAddr)
{
    uint32_t addr = (uint32_t)vAddr;
    if((addr >> 24) == 0x78) return 1; //CMX addr
    if((addr >> 30) ==  0x3) return 1; //DDR addr
    return 0;
}

swcProcessorType ThisCpu()
{
  static swcProcessorType curCpu = swcWhoAmI();
  return curCpu;
}

TimeSpec GetAbsTs(const TimeSpec *ts, TsType tt)
{
    TimeSpec absTs;
    absTs = *ts;

    if(tt == REL){
       TimeSpec now;
       clock_gettime(CLOCK_REALTIME, &now);
       rtems_timespec_add_to(&absTs, &now);
    }
   //else, ts is already ABS, nothing to do...

    return absTs;
}
