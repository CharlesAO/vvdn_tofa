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
#include "XLink.h"

static __attribute((section(".ddr.bss"))) uint8_t buffer1[3*1024*1024];

void *POSIX_Init (void *args)
{
    UNUSED(args);
    if (initClocksAndMemory()) {
        printf("Failed to initialize clocks.\n");
    }

    XLinkGlobalHandler_t ghandler = {
        .protocol = USB_VSC,
    };
    XLinkError_t status = X_LINK_COMMUNICATION_NOT_OPEN;

    status = XLinkInitialize(&ghandler);
    if (status != X_LINK_SUCCESS)
        exit(1);
    printf("Initialized\n");

    for(int i = 0; i < 1024*1024; i++)
    {
        buffer1[i] = i/4096;
    }
    int streamId = XLinkOpenStream(0, "firstStream", 5*1024*1024);
    streamPacketDesc_t * packet;

    int bootTimes = 100;
    unsigned int k;
    XLinkProfStart();
    while(bootTimes-- && (status != X_LINK_COMMUNICATION_NOT_OPEN))
    {
        printf("BOOT TIMES: %d\n", bootTimes);

        status = XLinkReadData(streamId, &packet);

        if (status != X_LINK_SUCCESS)
        {
            printf("myriad get data failed: %x\n", status);
        }
        printf("Data read\n");
        if(packet->length != 3*1024*1024) {abort();}
        for(k = 0; k < packet->length ; k++)
        {
            if (packet->data[k] != (uint8_t)(k/512))
            {
                printf("we were not able to read back data at index %u data: %x %x\n", k, packet->data[k],(uint8_t)(k+bootTimes));
                break;
            }
        }
        printf("Data checked\n");

        printf("Writing back data from myriad... ");
        fflush(stdout);
        status = XLinkWriteData(streamId, &packet->data[0], packet->length);
        if (status != X_LINK_SUCCESS)
        {
            printf("myriad set data failed: %x\n", status);
        }
        status = XLinkReleaseData(streamId);
        if (status != X_LINK_SUCCESS)
        {
            printf("release data failed: %x\n", status);
        }
        printf("Data sent\n");
    }
    XLinkProfStop();
    XLinkProfPrint();

    while(1)
        sleep(1);
    while(1)
        sleep(100);
    exit(0);
}
/* End of File */
