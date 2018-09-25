/// System Includes
/// -------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <mqueue.h>

/// Application Includes
/// -------------------------------------------------------------------------------------
#include "be_xlink.h"
#include "fe_hevc.h"
#include "RecMq.h"
#include "stereoNode.h"
#include "helper.h"
/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
#define DISPLAY_QU_NAME "/cv_display"
#define MAXNAMESIZE 32
#define  NR_LINK_BUFS     10
#define  FRAME_WIDTH    1280
#define  FRAME_HEIGHT    720
#define  DATA_LENGTH (NR_LINK_BUFS * FRAME_WIDTH * FRAME_HEIGHT)
/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------
//static const char* fifo_bitstream = "/tmp/bitstreamfifo";

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------

/// Functions Implementation
/// -------------------------------------------------------------------------------------

int stereoCapture()
{
    int streamId;
    char name[MAXNAMESIZE];
    XLinkError_t rc = XLinkGetDeviceName(0, name, sizeof(name));
    if (rc != X_LINK_SUCCESS)
        printf("Could not get device name: %d \n\n", rc);

    XLinkGlobalHandler_t glHandler = { .loglevel = 0, .profEnable = 0, };
    XLinkInitialize(&glHandler);

    XLinkHandler_t handler = { .devicePath = name };
    rc = X_LINK_COMMUNICATION_UNKNOWN_ERROR;
    while (rc != X_LINK_SUCCESS) {
        rc = XLinkConnect(&handler);
    }

    while((streamId = XLinkOpenStream(handler.linkId, "stereoChn",DATA_LENGTH)) == 0xDEADDEAD);//readonly
    createMsgQueue(DISPLAY_QU_NAME, 10, sizeof(uint16_t*));

    RecMq stereoRec(streamId, DISPLAY_QU_NAME);
    stereoRec.Start();
    
    stereoNode stereo(DISPLAY_QU_NAME);
    stereo.Start();

    stereoRec.Stop();
    stereo.Stop();
    return 0;
}
