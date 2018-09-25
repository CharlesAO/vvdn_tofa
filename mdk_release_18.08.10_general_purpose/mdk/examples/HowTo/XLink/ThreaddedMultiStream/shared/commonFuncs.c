#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#include "XLink.h"
#include "commonFuncs.h"

#define is_aligned(POINTER, BYTE_COUNT) \
        (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

static uint8_t buffer1[DATA_LENGTH];
static unsigned int msgIdx[NUM_MESSAGES+1]; // start and end points of messages
static __attribute((section(".ddr.bss"))) uint8_t messageReceivedBack[NUM_STREAMS][NUM_MESSAGES];

typedef struct sendingMsg {
    int streamId;
    int message_no;
}sendingMsg;

void* sendMessages(void* s) {

    int streamId = ((sendingMsg*)s)->streamId;
    int message_no = ((sendingMsg*)s)->message_no;
    for(int j = message_no; j<NUM_MESSAGES; j+=NUM_THREADS_PER_STREAM) {
        // Random sleeps to mess up the order of the messages sent
        usleep((int) (((float)rand()/RAND_MAX) * 50000));
        //SEND
        printf("Sending some data...\n");
        fflush(stdout);
        buffer1[msgIdx[j]] = j; //so that the receiver knows where to look for this data
        XLinkError_t status = XLinkWriteData(streamId, &buffer1[msgIdx[j]], msgIdx[j+1]-msgIdx[j]);
        printf("Stream %d: SENDING %d\n", streamId, j);
        if (status == X_LINK_SUCCESS)
        {
            printf("Data sent\n");
        }
        else
        {
            printf("set data failed: %x\n", status);
        }
    }
    return NULL;
}

void* receiveMessages(void* sId) {
    int streamId = *((int*)sId);
    streamPacketDesc_t* packet; // ptr to received data
    bool allMessagesRecd = false, testPassed = true;
    while(testPassed && !allMessagesRecd) {
        //RECEIVE
        printf("Reading back data... ");
        fflush(stdout);
        XLinkError_t status = XLinkReadData(streamId, &packet);
        if (status == X_LINK_SUCCESS)
        {
            printf("Data read\n");
        }
        else
        {
            printf("get data failed: %x\n", status);
            testPassed = false;
        }
        int i;
        //CHECK
        if(packet->length>0) {
            i = packet->data[0];
            printf("Stream %d: READING msg %d\n", streamId, i);
            if (packet->length != msgIdx[i+1]-msgIdx[i]){
                printf("Error: Stream[%d] Packet length wrong\n", streamId);
                testPassed = false;
            }
            if (!is_aligned(packet->data, 64)) {
                printf("Error: Stream[%d] Data not 64 byte aligned\n", streamId);
                testPassed = false;
            }
            for(unsigned int k = 0; k < packet->length ; k++)
            {
                if (packet->data[k] != buffer1[msgIdx[i]+k])
                {
                    printf("Error: Stream[%d] Value at index %u is wrong: %x\n", streamId, k, packet->data[k]);
                    testPassed = false;
                    break;
                }
            }
            messageReceivedBack[streamId][i] = 1;
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
            printf("Error: Stream[%d] release data failed: %x\n", streamId, status);
            testPassed = false;
        }
        //check if all messages have been received
        int a =0;
        for(int j =0; j<NUM_MESSAGES; j++){
            a+=messageReceivedBack[streamId][j];
        }
        if(a==NUM_MESSAGES){
            allMessagesRecd = true;
        }
    }
    return (testPassed && allMessagesRecd) ? (void*)testPassed : NULL;
}

void* sendAndReceiveMessages(void* sId) {
    int* streamId = (int*)sId;
    //Before sending the data, we prepare the buffer by generating some random data
    srand(time(NULL));
    for(int i = 0 ; i< DATA_LENGTH; i++){
        buffer1[i]=rand();
    }
    uint32_t startIdx[10] = {1 , 43, 80, 224, 271, 512, 768, 944, 1025, 2048};

    assert(NUM_MESSAGES < 256); //first byte is used to signal message_no
    assert(NUM_MESSAGES % 10 == 0); //because the start index buffer is defined in 10s
    //ensure all indices of the messages fall within DATA_LENGTH
    assert((startIdx[9] * (1+ (NUM_MESSAGES / 10)) + 2048) < DATA_LENGTH);

    // Divide the large buffer into multiple small messages of different lengths
    // msgIdx stores the indices of the starts of each messages
    // (last index has the index after the end of the last message too)
    for(int i = 0; i < NUM_MESSAGES; i++) {
        msgIdx[i] = startIdx[i%10] + (startIdx[9] * (i/10));
    }
    msgIdx[NUM_MESSAGES] = msgIdx[NUM_MESSAGES-1] + 2048;

    pthread_attr_t attr;
    int sc;

    //start thread for receiving (One thread for each stream)
    pthread_t receiver;

    if(pthread_attr_init(&attr) !=0) {
        printf("pthread_attr_init error\n");
    }
    sc=pthread_create(&receiver, &attr, &receiveMessages, (void*)streamId);
    if (sc) {
        printf("Control Thread creation failed\n");
    }

    //NUM_THREADS_PER_STREAM threads to send messages
    pthread_t senders[NUM_THREADS_PER_STREAM];
    long returnVal;
    sendingMsg a[NUM_THREADS_PER_STREAM];
    for(int i = 0; i<NUM_THREADS_PER_STREAM; i++) {

        a[i].streamId = *streamId;
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
    for(int i = 0; i< NUM_THREADS_PER_STREAM; i++) {
        pthread_join(senders[i], NULL);
    }
    pthread_join(receiver, (void**)&returnVal);

    return (void*)returnVal;
}

void receiveAndReturnMessages(int streamId){
    printf("Reading Data from ... %d\n", streamId);
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
