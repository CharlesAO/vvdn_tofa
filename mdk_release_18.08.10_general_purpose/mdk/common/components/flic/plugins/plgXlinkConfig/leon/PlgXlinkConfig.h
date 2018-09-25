///
/// @file      PlgXlink.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgXlink header.
///


#ifndef __PLG_XLINK_CONFIG_H__
#define __PLG_XLINK_CONFIG_H__

#ifndef PLGXLINK_MAX_CHAN_NAME
#define PLGXLINK_MAX_CHAN_NAME  (50)
#endif

#include <Flic.h>
#include <Sender.h>
#include <pcCfgMsg.h>
#include <ConfigMessage.h>

class PlgXlinkConfig : public ThreadedPlugin
{
public:
    MSender<ConfigMessage> out;

    void  *threadFunc(void *);
    void   Create(uint32_t maxSz, const char* channame);

    uint32_t maxPktSize;
    int      streamId;
    char  channelName[PLGXLINK_MAX_CHAN_NAME];
};

#endif //__PLG_XLINK_CONFIG_H__
