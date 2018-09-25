///
/// @file      PlgXlinkConfig.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Plugin for ISP Pipelines - Xlink channel Output
///            Platform(s) supported : ma2x8x
///

#include <stdio.h>
#include <Flic.h>
#include "PlgXlinkConfig.h"
#include <ImgFrame.h>
#include <sipp.h>
#include "../shared/pcCfgMsg.h"
#include "MsgBuf.h"
#include "BehQueueOvr.h"
#include "XLink.h"
#include "mvLog.h"

static void fullRead(streamId_t configStream, PCMessage* buffer, uint8_t cnt)
{
    streamPacketDesc_t * packet;
    XLinkError_t rc = XLinkReadData(configStream, &packet);
    if (packet == NULL || rc != X_LINK_SUCCESS)
    {
        printf("can't read data\n");
    }
    assert(cnt == packet->length);
    memcpy(buffer, packet->data, packet->length);
    rc = XLinkReleaseData(configStream);
    assert(rc == X_LINK_SUCCESS);
}

//#############################################
void PlgXlinkConfig::Create(uint32_t maxSz, const char* channame)
{
    // Create Sender
    maxPktSize = maxSz;
	Add(&out, "out");

    uint32_t strLenght = strlen(channame) + 1; /// incl termination
    if(strLenght > PLGXLINK_MAX_CHAN_NAME)
    {
        channelName[0] = 0;
        printf ( "Invalid channel name!\n" );
    }
    else
    {
        memcpy(channelName, channame, strLenght);
    }

    if(channelName[0] == 0)
    {
        printf ( "Invalid channel name!\n" );
        return;
    }

    streamId = XLinkOpenStream(0, channelName, maxPktSize);

    printf ( "Opened xlink stream named %s\n", channelName );
}

//#############################################
void * PlgXlinkConfig::threadFunc(void *)
{
    rtems_object_set_name(RTEMS_SELF, "PlgXlinkConfig");
    ConfigMessage pc_message;
    while(Alive())
    {
        PCMessage data;
        fullRead(streamId, &data, sizeof(PCMessage));
        assert(sizeof(PCMessage) == sizeof(ConfigMessage));
        pc_message.seq_num = data.seq_num;
        pc_message.type = (configMessageType)data.type;
        memcpy(pc_message.data, data.data, sizeof(data.data));
        int err = out.Send(&pc_message);
        if(err!=0){
            printf("Error sending packet data!\n");
        }
    }

    return NULL;
}
