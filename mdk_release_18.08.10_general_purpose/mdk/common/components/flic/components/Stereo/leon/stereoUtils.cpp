
#include <string>
#include "OsDrvCpr.h"
#include <assert.h>
#include "Stereo.h"
#include <DrvTimer.h>


std::string get_unique_str(uint64_t flic_id,const char *threadname)
{
    std::string unique;
    char *flic_name = reinterpret_cast<char *>(&flic_id);
    flic_name[7] = 0;
    unique += threadname;
    unique += "-";
    unique +=  flic_name;
    return(unique);
}

timespec my_get_time()
{
    timespec ts;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&ts);
    return(ts);
}


uint64_t getTimeStamps() 
{
    int intLevel;
    struct timeval tv;
    rtems_interrupt_disable (intLevel);
    gettimeofday(&tv, NULL);
    rtems_interrupt_enable (intLevel);
    return ((uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec);

}

#ifndef DEFAULT_APP_CLOCK_KHZ
#define DEFAULT_APP_CLOCK_KHZ 600000
#endif
double getTimeStampsTicks(void)
{
    u64 localTimer;
    int intLevel;
    double timeMs;
    rtems_interrupt_disable (intLevel);
    DrvTimerGetSystemTicks64 (&localTimer);
    timeMs = swcLongLongToDouble(localTimer);
    //timeMs = timeMs / (DEFAULT_APP_CLOCK_KHZ/1000) / 1000;

//    printf("ticks:%llu",localTimer);
    rtems_interrupt_enable (intLevel);
    return(timeMs);

}