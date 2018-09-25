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
#include <stdlib.h>
#include <string.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <LeonIPCApi.h>
#include <DrvLeon.h>
#include "rtems_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define MSG_SIZE       1
#define MSG_QUEUE_SIZE 5

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Leon RT start address
extern u32 lrt_start;
// Leon RT start address
extern u32 lrt_start;
leonIPCChannel_t __attribute__((section(".cmx_direct.data"))) lrt_LOStoLRTChannel;
uint32_t __attribute__((section(".cmx_direct.data"))) messagePool[MSG_QUEUE_SIZE * MSG_SIZE];
uint32_t __attribute__((section(".cmx_direct.data"))) someVar;
volatile uint32_t __attribute__((section(".cmx_direct.data"))) LrtSendMessage;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static volatile uint32_t MessagesSent;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void *POSIX_Init (void *args)
{
    UNUSED(args);

    int localMessageCnt;
    int status;

    initClocksAndMemory();
    printf("LeonOS Started.\n");
    printf("LeonRT Start Addr: 0x%lX\n",(u32)&lrt_start);

    status = LeonIPCTxInit(&lrt_LOStoLRTChannel, messagePool, MSG_QUEUE_SIZE, MSG_SIZE);

    // Start the LeonRT application
    DrvLeonRTStartup((u32)&lrt_start);
    DrvLeonRTWaitForBoot();

    LrtSendMessage=0;
    localMessageCnt=0;

    while (MessagesSent < 5)
    {
        if (LrtSendMessage == 1)
        {
            someVar = 0xCAFE0000 + (++localMessageCnt);
            status = LeonIPCSendMessage(&lrt_LOStoLRTChannel, &someVar);
            if (status != IPC_SUCCESS)
            {
                printf("Message sending failed with code 0x%X:\n", status);
                switch (status)
                {
                case IPC_RX_NOTINITIALIZED:
                    printf("LRT did not initialize Rx\n");
                    break;
                case MSG_QUEUE_OVERFLOW:
                    printf("No space left in the queue for another message\n");
                    break;
                default:
                    printf("Unknown failure\n");
                    break;
                }
            }
            else
            {
                MessagesSent++;
            }
            LrtSendMessage = 0;
        }
    }

    DrvLeonRTWaitExecution();

    printf("LeonRT Stop.\n");

    exit(0);
}


