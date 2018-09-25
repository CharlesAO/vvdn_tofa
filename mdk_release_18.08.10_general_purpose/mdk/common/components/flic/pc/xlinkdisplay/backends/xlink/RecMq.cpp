/// =====================================================================================
///
///        @file:      be_xlink.cpp
///        @brief:     flicpc Xlink Backend
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include "RecMq.h"

#include <stdio.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <mqueue.h>

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------3

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

 void *RecMq::xLinkRead(void *This)
{
    ((RecMq*)This)->xlinkReadThread();
}

void  *RecMq::xlinkReadThread()
{
    streamPacketDesc_t * packet;

    mqd_t mqdes = mq_open(qName, O_RDWR );
    if(mqdes == (mqd_t) - 1)
    {
        perror("mq_open ");
        assert(0);
    }

    int iters = 0;
    XLinkError_t status;
    while(status != X_LINK_COMMUNICATION_NOT_OPEN)
    {
        status = XLinkReadData(streamId, &packet);

        if (status != X_LINK_SUCCESS) {
            printf("myriad get data failed: %x\n", status);
        }

        printf ( "StreamId #%d | Name %10s | Iter %6d | Packet size: %8u |\n",
                streamId, (const char*)"NULL", iters++, packet->length );

        uint8_t *buff = (uint8_t*)malloc(packet->length);
        memcpy(buff, packet->data, packet->length);
        int status =  mq_send(mqdes, (char *) &buff, sizeof(uint8_t*), 0);
        if(status)
        {
            perror("mq_send");
            assert(0);
        }

        status = XLinkReleaseData(streamId);
        if (status)
        {
            printf("release data failed: %x\n", status);
        }
    }
}

void RecMq::Start()
{
    int ret = pthread_create(&thread, NULL, xLinkRead, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void RecMq::Stop()
{
    pthread_join(thread, NULL);
}
