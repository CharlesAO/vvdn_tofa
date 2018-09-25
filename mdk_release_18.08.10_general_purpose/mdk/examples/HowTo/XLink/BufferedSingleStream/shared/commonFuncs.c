#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include "XLink.h"
#include "commonFuncs.h"

#define BUFF_LEN (3*1024*1024)
#define is_aligned(POINTER, BYTE_COUNT) \
        (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

static uint8_t buffer1[BUFF_LEN];
unsigned int msgIdx[NUM_MESSAGES+1]; // start and end points of messages


bool sendAndReceiveMessages(int streamId) {
    //creating data to send
    srand(time(NULL));
    for(int i = 0 ; i< BUFF_LEN; i++){
        buffer1[i]=rand();
    }
    uint32_t startIdx[10] = {0 , 43, 80, 224, 271, 512, 768, 944, 1025, 2048};
    assert(NUM_MESSAGES % 10 == 0);
    assert((startIdx[9] * (1+ (NUM_MESSAGES / 10)) + 2048) < BUFF_LEN);
    for(int i = 0; i < NUM_MESSAGES; i++) {
        msgIdx[i] = startIdx[i%10] + (startIdx[9] * (i/10));
    }
    msgIdx[NUM_MESSAGES] = msgIdx[NUM_MESSAGES-1] + 2048;

    bool testPassed = true;
    streamPacketDesc_t* packet; // ptr to received data

    for(int i = 0; i<NUM_MESSAGES; i++) {
        //SEND
        printf("Sending some data... ");
        fflush(stdout);
        XLinkError_t status = XLinkWriteData(streamId, &buffer1[msgIdx[i]], msgIdx[i+1]-msgIdx[i]);
        if (status == X_LINK_SUCCESS)
        {
            printf("Data sent\n");
        }
        else
        {
            printf("set data failed: %x\n", status);
            testPassed = false;
        }

        //RECEIVE
        printf("Reading back data... ");
        fflush(stdout);
        status = XLinkReadData(streamId, &packet);
        if (status == X_LINK_SUCCESS)
        {
            printf("Data read\n");
        }
        else
        {
            printf("get data failed: %x\n", status);
            testPassed = false;
        }
        //CHECK
        if (packet->length != msgIdx[i+1]-msgIdx[i]){
            printf("Packet length wrong\n");
            testPassed = false;
        }
        if (!is_aligned(packet->data, 64)) {
            printf("Data not 64 byte aligned");
            testPassed = false;
        }
        for(unsigned int k = 0; k < packet->length ; k++)
        {
            if (packet->data[k] != buffer1[msgIdx[i]+k])
            {
                printf("Value at index %u is wrong: %x\n", k, packet->data[k]);
                testPassed = false;
                break;
            }
        }

        //RELEASE
        status = XLinkReleaseData(streamId);
        if (status != 0)
        {
            printf("release data failed: %x\n", status);
            testPassed = false;
        }

    }
    return testPassed;
}

void receiveAndReturnMessages(int streamId){
    printf("Reading Data... ");
    fflush(stdout);
    streamPacketDesc_t* packet;
    XLinkError_t status = XLinkReadData(streamId, &packet);
    if (status != X_LINK_SUCCESS)
    {
        printf("Get data failed: %x\n", status);
    } else {
        printf("Data read\n");
    }
    printf("Writing back data... ");
    fflush(stdout);
    status = XLinkWriteData(streamId, &packet->data[0], packet->length);
    if (status != X_LINK_SUCCESS)
    {
        printf("set data failed: %x\n", status);
    } else {
        printf("Data sent\n");
    }

    status = XLinkReleaseData(streamId);
    if (status != X_LINK_SUCCESS)
    {
        printf("release data failed: %x\n", status);
    }
}

/* End of File */
