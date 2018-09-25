#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "pthread.h"

#include "XLink.h"
#include "usb_boot.h"
#include "commonFuncs.h"
#define MAXNAMESIZE 28

int main(void)
{
    char name[MAXNAMESIZE];
    int rc = XLinkGetDeviceName(0, name, sizeof(name));
    if (!rc)
    {
        printf("Device Found name %s \n", name);
    }
    else
    {
        printf("ERROR couldn't find devices rc %d\n", rc);
        exit(1);
    }
    XLinkGlobalHandler_t ghandler = {
        .protocol = USB_VSC
    };
    XLinkHandler_t handler = {
#ifdef USE_USB_VSC
        .devicePath = name,
#else
        .devicePath = "/dev/ttyACM0",
        .devicePath2 = "/dev/ttyACM1",
#endif
    };

    // boot Myriad
    if(XLinkBootRemote(name, MVCMD_NAME) != X_LINK_SUCCESS){
        //XLinkResetRemote();
        exit(1);
    }
    printf("Myriad was booted\n");

    // try to open communication. Myriad is not yet booted, so this will fail
    while( XLinkInitialize(&ghandler) != X_LINK_SUCCESS);
    printf("Initialize done\n");

    while(XLinkConnect(&handler) != X_LINK_SUCCESS);
    printf("XLinkConnect done - link Id %d\n", handler.linkId);


    // Open 2 communication streams
    streamId_t streamId[NUM_STREAMS];
    while((streamId[0] = XLinkOpenStream(handler.linkId, "firstStream",DATA_LENGTH)) == 0xDEADDEAD);//readonly
    while((streamId[1] = XLinkOpenStream(handler.linkId, "secondStream",DATA_LENGTH)) == 0xDEADDEAD);//readonly
    assert(NUM_STREAMS == 2);


    pthread_attr_t attr;
    int sc;
    pthread_t tid[NUM_STREAMS];
    bool pass[NUM_STREAMS]; // bools to store the status of the tests on the two streams

    // New thread for each stream to send and receive messages to (&from) the Myriad
    for(int i = 0; i < NUM_STREAMS; i++){
        if(pthread_attr_init(&attr) !=0) {
            printf("pthread_attr_init error\n");
        }
        sc=pthread_create(&tid[i], &attr, &sendAndReceiveMessages, (void*)&streamId[i]);
        if (sc) {
            printf("Control Thread creation failed\n");
        }
    }
    // Wait for threads to complete
    for(int i = 0; i< NUM_STREAMS; i++) {
        pthread_join(tid[i], (void**)&pass[i]);
    }

    XLinkError_t status;
    for(int i = 0; i< NUM_STREAMS; i++) {
        status = XLinkCloseStream(streamId[i]);
        if (status != X_LINK_SUCCESS)
        {
            printf("close stream failed: %x\n", status);
        }
    }
    // Deinitialise comms and reset Myriad
    status = XLinkResetRemote(handler.linkId);
    if (status != X_LINK_SUCCESS)
    {
        printf("reset failed: %x\n", status);
    }

    // check that all streams have passed
    bool allPassed = pass[0];
    for(int i =0; i<NUM_STREAMS; i++){
        allPassed = allPassed && pass[i];
    }
    if(allPassed) {
        printf("TEST PASSED! :)\n");
    } else {
        printf("TEST FAILED! :(\n");
    }
}

