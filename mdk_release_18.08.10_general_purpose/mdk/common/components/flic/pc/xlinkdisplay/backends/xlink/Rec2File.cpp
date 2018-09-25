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
#include "Rec2File.h"

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
#include <ctime>

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

 void *Rec2File::xLinkRead(void *This)
{
    ((Rec2File*)This)->xlinkReadThread();
}

void  *Rec2File::xlinkReadThread()
{
    streamPacketDesc_t * packet;

    int dir_err = mkdir("Captures", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        printf("Error creating directory!n");
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

          char strtime[32];
          auto t = std::time(nullptr);
          auto tm = *std::localtime(&t);
          strftime(strtime, sizeof(strtime), "%Y%m%d_%H%M%S", &tm);
          char fullname[64];

          sprintf(fullname, "%s",  dirname);
          sprintf(fullname + strlen(dirname), "/%s.jpg", strtime);
          FILE *pf = fopen(fullname, "wb");
          if(pf){
              fwrite (packet->data , sizeof(char), packet->length, pf);
              fclose(pf);
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
}

void Rec2File::Start()
{
    int ret = pthread_create(&thread, NULL, xLinkRead, this);
    if (ret) {
        fprintf(stderr, "Error - pthread_create() return code: %d\n", ret);
        exit (EXIT_FAILURE);
    } else {
        printf("thread created\n");
    }
}

void Rec2File::Stop()
{
    pthread_join(thread, NULL);
}
