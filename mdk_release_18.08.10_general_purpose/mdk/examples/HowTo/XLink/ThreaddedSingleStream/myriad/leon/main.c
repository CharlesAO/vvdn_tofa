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
    for(int i = 0; i<NUM_MESSAGES; i++) {
        receiveAndReturnMessages(*((int*)a));
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


    XLinkGlobalHandler_t ghandler = {
        .protocol = USB_VSC
    };
    XLinkError_t status = X_LINK_COMMUNICATION_NOT_OPEN;
    while( status != X_LINK_SUCCESS) {
        status = XLinkInitialize(&ghandler);
    }

    int streamId = XLinkOpenStream(0, "firstStream",DATA_LENGTH);


    if(pthread_attr_init(&attr) != 0) {
        printf("pthread_attr_init error\n");
    }
    if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) {
        printf("pthread_attr_setinheritsched error\n");
    }
    if(pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0) {
        printf("pthread_attr_setschedpolicy error\n");
    }

    sc=pthread_create(&tid, &attr, &Func, &streamId);
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


void Fatal_extension(
    Internal_errors_Source  the_source,
    bool                    is_internal,
    Internal_errors_t       the_error
    )
{
    if(the_error)
        printk("%s : %s \n", __func__, rtems_fatal_source_text(the_source));

    switch(the_source)
    {
    case INTERNAL_ERROR_CORE:
        printk(" : %s \n", rtems_internal_error_text(the_error));
        break;
    case RTEMS_FATAL_SOURCE_EXIT:
        if(the_error)
            printk("\nExited with error code %d\n", the_error);
        break; // normal exit
    case RTEMS_FATAL_SOURCE_ASSERT:
        printk("\n%s : %d in %s \n",
               ((rtems_assert_context *)the_error)->file,
               ((rtems_assert_context *)the_error)->line,
               ((rtems_assert_context *)the_error)->function);
        break;
    case RTEMS_FATAL_SOURCE_EXCEPTION:
        rtems_exception_frame_print((const rtems_exception_frame *) the_error);
        break;
    default:
        printk ("\nSource %d Internal %d Error %d  0x%X:\n",
                the_source, is_internal, the_error, the_error);
        break;
    }
}
/* End of File */
