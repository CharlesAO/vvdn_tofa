///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <swcLeonUtils.h>
#include <DrvLeon.h>
#include <DrvIcb.h>
#include <DrvTimer.h>
#include <DrvCpr.h>
#include <LeonIPCApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define MSG_SIZE       1
#define MSG_QUEUE_SIZE 5

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
#define MESSAGE_POOL_BYTE_SIZE (1024)

extern leonIPCChannel_t LOStoLRTChannel;
uint32_t __attribute__((section(".ddr.data"))) received[MSG_QUEUE_SIZE * MSG_SIZE];
uint32_t __attribute__((section(".ddr.data"))) someVar;

static uint32_t messagePool[MSG_QUEUE_SIZE * MSG_SIZE];
static volatile uint32_t __attribute__((section(".ddr.data"))) MessagesReceived;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

#define MSS_LRT_RESETS_TO_DEASSERT        ( \
        ( DEV_MSS_LRT_DSU      ) | \
        ( DEV_MSS_LRT_L2C      ) | \
        ( DEV_MSS_LRT_ICB      ) | \
        ( DEV_MSS_TIM          ) | \
        ( DEV_MSS_RTAHB_CTRL   ) | \
        ( DEV_MSS_LRT          ) | \
        ( DEV_MSS_RTBRIDGE     ) | \
        ( DEV_MSS_APB2_CTRL    ) | \
        ( DEV_MSS_APB_SLV      ) )

static volatile int localCnt=0;

void LRTSignal(struct leonIPCChannel_t *channel)
{
    uint32_t msgNo, i;

    LeonIPCNumberOfPendingMessages(channel, &msgNo);
    for (i = 0; i < msgNo; i++)
    {
        if (LeonIPCReadMessage(channel, &received[MessagesReceived]) == IPC_SUCCESS)
        {
            printf("LRT: Cofee received from LOS: 0x%lX\n", received[MessagesReceived]);
            MessagesReceived++;
        }
    }
}

void LeonIPCMessagingISR(uint32_t irqSrc);

int main(void)
{
    UNUSED(messagePool);

    //Make sure the cache is flushed out

    //Deassert MSS clocks as per the datasheet specification in chapter 7.9.17 -> MSS RSTN bits are not automatically deasserted
    DrvCprSysDeviceAction(MSS_DOMAIN,DEASSERT_RESET,MSS_LRT_RESETS_TO_DEASSERT);

    swcLeonSetPIL(0);

    LeonIPCRxInit(&LOStoLRTChannel, &LRTSignal, IRQ_DYNAMIC_5, 5);    DrvLeonRTSignalBootCompleted();

    //Just wait for 5 messages which are sent by LeonOS
    while (MessagesReceived < 5)
        ;

    DrvLeonRTSignalStop();

    return 0;
}
