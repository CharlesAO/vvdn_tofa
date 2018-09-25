///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Setting thread priority @ start
///

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <UnitTestApi.h>

#define CUSTOM_PRIO 70
int expectedPriority;

//##############################################################
class Plg : public ThreadedPlugin
{
  public:
    void *threadFunc(void *)
    {
        int policy;
        struct sched_param schParam;
        pthread_getschedparam(pthread_self(), &policy, &schParam);
        unitTestAssert(schParam.sched_priority == expectedPriority);
        printf("schParam.sched_priority = %d\n", schParam.sched_priority);
        return NULL;
    }
};

Plg    plg;
Pipeline p;

//##############################################################
extern "C" void *POSIX_Init (void *)
{
    unitTestInit();

  //============================================================
  //1) Default RTEMS POSIX (no custom setting)
    extern pthread_attr_t _POSIX_Threads_Default_attributes;
    expectedPriority = _POSIX_Threads_Default_attributes.schedparam.sched_priority;
    p.Add(&plg);
    p.Start();
    p.Wait();
    p.Stop();
    p.Delete();

  //============================================================
  //2) Custom thread priority
    plg.schParam.sched_priority = expectedPriority = CUSTOM_PRIO;
    p.Add(&plg);
    p.Start();
    p.Wait();
    p.Stop();
    p.Delete();

    unitTestFinalReport();
    exit(0);
}
