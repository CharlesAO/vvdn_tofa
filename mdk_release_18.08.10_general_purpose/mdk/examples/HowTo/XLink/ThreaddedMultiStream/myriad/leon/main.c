#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <rtems.h>
#include <rtems/bspIo.h>
#include "app_config.h"
#include "rtems_config.h"

#include "mv_types.h"
#include "../../shared/commonFuncs.h"
#include "XLink.h"


void* Func(void* a) {
    int streamId = *((int*)a);
    for(int i = 0; i<NUM_MESSAGES; i++) {
        receiveAndReturnMessages(*((int*)a));
    }
    XLinkError_t status = XLinkCloseStream(streamId);
    if (status != X_LINK_SUCCESS)
    {
        printf("close stream failed: %x\n", status);
    }
    return NULL;
}

void *POSIX_Init (void *args)
{
    UNUSED(args);
    // Init and configure system clocks
    if (initClocksAndMemory()) {
        printf("Failed to initialize clocks.\n");
    }

    // handler is required to contain profiling data (and usb interface data in CDC mode)
    // handler is required to remain in scope as long as UsbLink communication is open
    XLinkGlobalHandler_t ghandler = {
        .protocol = USB_VSC
    };
    XLinkError_t status = X_LINK_COMMUNICATION_NOT_OPEN;

    // Call initialise until it succeeds. It could fail before the myriad has started
    while(status != X_LINK_SUCCESS) {
        status = XLinkInitialize(&ghandler);
    }

    // Communication streams (maximum USB_LINK_MAX_STREAMS - defined in UsbLinkPublicDefines.h)
    streamId_t streamId[NUM_STREAMS];
    while((streamId[0] = XLinkOpenStream(0, "firstStream", DATA_LENGTH)) == 0xDEADDEAD);
    while((streamId[1] = XLinkOpenStream(0, "secondStream", DATA_LENGTH)) == 0xDEADDEAD);

    // Create a thread for each stream to receive messages and return them to PC
    pthread_t tid[NUM_STREAMS];
    pthread_attr_t attr;
    int sc;

    for(int i =0; i<NUM_STREAMS; i++) {
        if(pthread_attr_init(&attr) !=0) {
            printf("pthread_attr_init error\n");
        }
        if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) {
            printf("pthread_attr_setinheritsched error\n");
        }
        if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
            printf("pthread_attr_setschedpolicy error\n");
        }
        sc=pthread_create(&tid[i], &attr, &Func, &streamId[i]);
        if (sc) {
            printf("Control Thread creation failed\n");
        }
        struct sched_param schedparam;
        schedparam.sched_priority = 200; //default
        if(pthread_setschedparam(tid[i], SCHED_RR, &schedparam) != 0) {
            printf("pthread_attr_setschedpolicy error\n");
        }
    }
    while(1)
        sleep(1);
    while(1)
        sleep(100);
    exit(0);
}

/* End of File */
