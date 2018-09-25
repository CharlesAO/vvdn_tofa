#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>

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
        //exit(1);
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

    int streamId;

    // boot Myriad
#ifndef BOOT_DISABLE
    if( XLinkBootRemote(name, MVCMD_NAME) != X_LINK_SUCCESS){
        //XLinkResetRemote();
        exit(1);
    }   
#endif
    printf("Myriad was booted\n");

    // try to open communication. Myriad is not yet booted, so this will fail
    while( XLinkInitialize(&ghandler) != X_LINK_SUCCESS);
    printf("Initialize done\n");

    while(XLinkConnect(&handler) != X_LINK_SUCCESS);
    printf("XLinkConnect done - link Id %d\n", handler.linkId);

    while((streamId = XLinkOpenStream(handler.linkId, "firstStream",DATA_LENGTH)) == 0xDEADDEAD);//readonly

    bool pass = sendAndReceiveMessages(streamId);

    XLinkError_t status = XLinkCloseStream(streamId);
    if (status != X_LINK_SUCCESS)
    {
        printf("close stream failed: %x\n", status);
    }

    status = XLinkResetRemote(handler.linkId);
    if (status != X_LINK_SUCCESS)
    {
        printf("reset failed: %x\n", status);
    }

    if(pass) {
        printf("TEST PASSED! :)\n");
    } else {
        printf("TEST FAILED! :(\n");
    }
}

