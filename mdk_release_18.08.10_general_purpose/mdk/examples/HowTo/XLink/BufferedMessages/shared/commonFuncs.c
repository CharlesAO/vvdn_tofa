#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include <pthread.h>
#include "XLink.h"
#include "commonFuncs.h"

#define BUFF_LEN (3*1024*1024)
#define is_aligned(POINTER, BYTE_COUNT) \
        (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)
#define NUM_THREADS 6
static uint8_t buffer1[BUFF_LEN];
static unsigned int msgIdx[NUM_MESSAGES+1]; // start and end points of messages
static __attribute((section(".ddr.bss"))) uint8_t messageReceivedBack[NUM_MESSAGES];

typedef struct sendingMsg {
    int streamId;
    int message_no;
}sendingMsg;

void* sendMessages(void* s) {
    int streamId = ((sendingMsg*)s)->streamId;
    int message_no = ((sendingMsg*)s)->message_no;
    for(int j = message_no; j<NUM_MESSAGES; j+=NUM_THREADS) {
        usleep((int) (((float)rand()/RAND_MAX) * 50000));
        //SEND
        buffer1[msgIdx[j]] = j; //so that the receiver knows where to look for this data
        XLinkError_t status = XLinkWriteData(streamId, &buffer1[msgIdx[j]], msgIdx[j+1]-msgIdx[j]);
        if (status != X_LINK_SUCCESS)
        {
            printf("set data failed: %x\n", status);
        }
        printf("sendMessages: sleeping for 2 seconds to allow stream to clear a bit....\n");
        sleep(2);
        printf("sendMessages: sleep done\n");
    }
    return NULL;
}

void* receiveMessages(void* sId) {
    int streamId = *((int*)sId);
    streamPacketDesc_t* packet; // ptr to received data
    bool allMessagesRecd = false, testPassed = true;
    while(testPassed && !allMessagesRecd) {
        printf("Receive Messages sleeping for 1s......\n");
        sleep(1); //1 second sleep to allow messages to build up
        printf("Recieve Messages sleep done\n");
        //RECEIVE
        XLinkError_t status = XLinkReadData(streamId, &packet);
        if (status != X_LINK_SUCCESS)
        {
            printf("get data failed: %x\n", status);
            testPassed = false;
        }
        int i;
        //CHECK
        if(packet->length>0) {
            i = packet->data[0];
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
            messageReceivedBack[i] = 1;
        }
        else {
            printf("This test can't pass if there are messages of size 0\n");
            testPassed = false;
            i = -1;
        }

        //RELEASE
        status = XLinkReleaseData(streamId);
        if (status != 0)
        {
            printf("release data failed: %x\n", status);
            testPassed = false;
        }
        //check if all messages have been received
        int a =0;
        for(int j =0; j<NUM_MESSAGES; j++){
            a+=messageReceivedBack[j];
        }
        if(a==NUM_MESSAGES){
            allMessagesRecd = true;
        }
    }
    return (testPassed && allMessagesRecd) ? (void*)testPassed : NULL;
}

bool sendAndReceiveMessages(int streamId) {
    //creating data to send
    srand(time(NULL));
    for(int i = 0 ; i< BUFF_LEN; i++){
        buffer1[i]=rand();
    }
    uint32_t startIdx[10] = {1 , 43, 80, 224, 271, 512, 768, 944, 1025, 2048};

    assert(NUM_MESSAGES < 256); //first byte is used to signal message_no
    assert(NUM_MESSAGES % 10 == 0); //because the start index buffer is defined in 10s
    //ensure all indices of the messages fall within BUFF_LEN
    assert((startIdx[9] * (1+ (NUM_MESSAGES / 10)) + 2048) < BUFF_LEN);
    for(int i = 0; i < NUM_MESSAGES; i++) {
        msgIdx[i] = startIdx[i%10] + (startIdx[9] * (i/10));
    }
    msgIdx[NUM_MESSAGES] = msgIdx[NUM_MESSAGES-1] + 2048;

    pthread_attr_t attr;
    int sc;

    //start thread for receiving
    pthread_t receiver;

    if(pthread_attr_init(&attr) !=0) {
        printf("pthread_attr_init error\n");
    }
    sc=pthread_create(&receiver, &attr, &receiveMessages, (void*)&streamId);
    if (sc) {
        printf("Control Thread creation failed\n");
    }

    //NUM_THREADS threads to send messages
    pthread_t senders[NUM_THREADS];
    int returnVal;
    sendingMsg a[NUM_THREADS];
    for(int i = 0; i<NUM_THREADS; i++) {

        a[i].streamId = streamId;
        a[i].message_no = i;

        if(pthread_attr_init(&attr) !=0) {
            printf("pthread_attr_init error\n");
        }
        sc=pthread_create(&senders[i], &attr, &sendMessages, &a[i]);
        if (sc) {
            printf("Control Thread creation failed\n");
        }
    }
    sleep(1);
    for(int i = 0; i< NUM_THREADS; i++) {
        pthread_join(senders[i], NULL);
    }
    pthread_join(receiver, (void**)&returnVal);

    return (bool)returnVal;
}

void receiveAndReturnMessages(int streamId){
    streamPacketDesc_t* packet;
    XLinkError_t status = XLinkReadData(streamId, &packet);
    if (status != X_LINK_SUCCESS)
    {
        printf("Get data failed: %x\n", status);
    }
    status = XLinkWriteData(streamId, &packet->data[0], packet->length);
    if (status != X_LINK_SUCCESS)
    {
        printf("set data failed: %x\n", status);
    }
    status = XLinkReleaseData(streamId);
    if (status != X_LINK_SUCCESS)
    {
        printf("release data failed: %x\n", status);
    }
}

/* End of File */
