#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include "pthread.h"

#include "XLink.h"
#include "usb_boot.h"
#include "commonFuncs.h"
#define DATA_LENGTH (3*1024*1024)
#define MAXNAMESIZE 28
int main(void)
{
    char name[MAXNAMESIZE];
    char name2[MAXNAMESIZE];
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
    rc = XLinkGetDeviceName(1, name2, sizeof(name2));
    if (!rc)
    {
        printf("Device Found name %s \n", name2);
    }
    else
    {
        printf("ERROR couldn't find devices rc %d\n", rc);
        exit(1);
    }
    XLinkGlobalHandler_t ghandler = {
        .protocol = USB_VSC
    };
    XLinkHandler_t handler[2];

#ifdef USE_USB_VSC
    handler[0].devicePath = name;
    handler[1].devicePath = name2;
#else
    handler[0].devicePath = "/dev/ttyACM0";
    handler[0].devicePath2 = "/dev/ttyACM1";
    handler[1].devicePath = "/dev/ttyACM0";
    handler[1].devicePath2 = "/dev/ttyACM1";
#endif


 // boot Myriad
    if(XLinkBootRemote(name, MVCMD_NAME) != X_LINK_SUCCESS){
        XLinkResetAll();
        exit(1);
    }
    printf("Myriad  %s was booted\n", name);
    if(XLinkBootRemote(name2, MVCMD_NAME) != X_LINK_SUCCESS){
        XLinkResetAll();
        exit(1);
    }
    printf("Myriad  %s was booted\n", name2);

    while(XLinkInitialize(&ghandler) != X_LINK_SUCCESS);
    printf("Initialize done\n");

    int sc;
    int streamId[2];
    pthread_attr_t attr;
    pthread_t tid[2];
    bool pass[2];
    if(pthread_attr_init(&attr) != 0) {
        printf("pthread_attr_init error\n");
    }
    XLinkError_t ret;
    int i;
    for (i = 0; i < 2; i++)
    {
        ret = XLinkConnect(&handler[i]);
        while (ret != X_LINK_SUCCESS && ret != X_LINK_ERROR)
            ret = XLinkConnect(&handler[i]);
        if (ret == X_LINK_ERROR)
            break;
        //while(XLinkConnect(&handler[i]) != X_LINK_SUCCESS);
        printf("XLinkConnect done - link Id %d\n", handler[i].linkId);
    }
    if (ret == X_LINK_ERROR)
    {
        XLinkResetAll();
        return -1;
    }

    for (i = 0; i < 2; i++)
    {
        while((streamId[i] = XLinkOpenStream(handler[i].linkId, "firstStream",DATA_LENGTH)) == 0xDEADDEAD);//readonly

        // New thread for each link to send and receive messages to (&from) the Myriad
        sc = pthread_create(&tid[i], &attr, &sendAndReceiveMessages, (void*)&streamId[i] );
        if (sc) {
            printf("Control Thread creation failed\n");
        }
    }
    // Wait for threads to complete
    for(int i = 0; i < 2; i++) {
        pthread_join(tid[i], (void**)&pass[i]);
    }

    for (i = 0; i < 2; i++)
    {
        XLinkError_t status = XLinkCloseStream(streamId[i]);
        if (status != X_LINK_SUCCESS)
        {
            printf("close stream failed: %x %x\n", streamId[i], status);
        }
        printf("close stream success: %x\n", streamId[i]);
    }

    for(int i = 0; i < 2; i++) {
        XLinkError_t status = XLinkResetRemote(handler[i].linkId);
        if (status != X_LINK_SUCCESS)
        {
            printf("reset failed: %x\n", status);
        }
        if(pass[i]) {
            printf("TEST PASSED! on %d :)\n", handler[i].linkId);
        } else {
            printf("TEST FAILED!  on %d :(\n", handler[i].linkId);
        }
    }
}

