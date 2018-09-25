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
#include "RecFifo.h"

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

 void *RecFifo::xLinkRead(void *This)
{
    ((RecFifo*)This)->xlinkReadThread();
}

void  *RecFifo::xlinkReadThread()
{
    streamPacketDesc_t * packet;

      /// Open fifo
      printf ( "Opening fifo\n" );
      int ret_fifowr;
      int fdfifo = open(qName, O_WRONLY);
      if(fdfifo == -1)
      {
          perror("openfifo");
      }

      int iters = 0;
      XLinkError_t status;
      while(status != X_LINK_COMMUNICATION_NOT_OPEN)
      {
          fflush(stdout);
          status = XLinkReadData(streamId, &packet);

          if (status != X_LINK_SUCCESS)
          {
              printf("myriad get data failed: %x\n", status);
          }
          printf ( "StreamId #%d | Name %10s | Iter %6d | Packet size: %8u |\n",
                streamId, (const char*)"NULL", iters++, packet->length );

          ret_fifowr = write(fdfifo, packet->data, packet->length);
          if(ret_fifowr != packet->length)
          {
              printf ( "Failed to write chunk!\n" );
          }

          status = XLinkReleaseData(streamId);
          if (status == 0)
          {
          }
          else
          {
              printf("release data failed: %x\n", status);
          }
      }

      printf("Closing fifo %s\n", qName);
      close(fdfifo);
}

void RecFifo::Start()
{
    int ret = pthread_create(&thread, NULL, xLinkRead, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void RecFifo::Stop()
{
    pthread_join(thread, NULL);
}
