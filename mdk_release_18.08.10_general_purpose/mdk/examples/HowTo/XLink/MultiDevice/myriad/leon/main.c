#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <rtems.h>
#include <rtems/bspIo.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "app_config.h"
#include "rtems_config.h"

#include "mv_types.h"
#include "../../shared/commonFuncs.h"
#include "XLink.h"



void* Func(void* a) {
    UNUSED(a);
    XLinkGlobalHandler_t ghandler = {
        .protocol = USB_VSC
    };
    XLinkError_t status = X_LINK_COMMUNICATION_NOT_OPEN;
    while( status != X_LINK_SUCCESS) {
        status = XLinkInitialize(&ghandler);
    }

    int streamId = XLinkOpenStream(0, "firstStream",5*1024*1024);

    for(int i = 0; i<NUM_MESSAGES; i++) {
        receiveAndReturnMessages(streamId);
    }
    return NULL;
}

void *POSIX_Init (void *args)
{
    UNUSED(args);

    if (initClocksAndMemory()) {
        printf("Failed to initialize clocks.\n");
    }

    pthread_attr_t attr;
    int sc;
    pthread_t tid;


    if(pthread_attr_init(&attr) !=0) {
        printf("pthread_attr_init error\n");
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) {
        printf("pthread_attr_setinheritsched error\n");
    }
    if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
        printf("pthread_attr_setschedpolicy error\n");
    }

    sc=pthread_create(&tid, &attr, &Func, NULL);
    if (sc) {
        printf("Control Thread creation failed\n");
    }
    struct sched_param schedparam;
    schedparam.sched_priority = 200; //default
    if(pthread_setschedparam(tid, SCHED_RR, &schedparam) != 0) {
        printf("pthread_attr_setschedpolicy error\n");
    }
    while(1)
        sleep(1);
    while(1)
        sleep(100);
    exit(0);
}
/* End of File */
