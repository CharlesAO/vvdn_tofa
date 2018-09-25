#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#include "usb_boot.h"
#include "XLink.h"
#define MAXNAMESIZE 28
#define DATA_LENGTH (3*1024*1024)
uint8_t msg[DATA_LENGTH];
uint8_t msg2[DATA_LENGTH];
uint8_t msg3[DATA_LENGTH];

//#define BOOT_DISABLE /*disable usb-boot if you want to use debugger*/

void printHelp(void)
{
    printf("                                    This test no longer requires any args.\n");
    printf("      --help or -h                  - Print this information\n");
}


int main(int argc, char **argv)
{
    int k;
    printf(" XLink pc example application used to communicate to the Myriad USB device.\n");
    for (int nrArg=0; nrArg<argc; nrArg++)
    {
        if (!strcmp(argv[nrArg],"help")||!strcmp(argv[nrArg],"/?")||!strcmp(argv[nrArg],"--help")||!strcmp(argv[nrArg],"-h")||!strcmp(argv[nrArg],"/h"))
        {
            printf("   Command line parameters:\n");
            printHelp();
            exit(0);
        }
    }
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
        .protocol = USB_VSC,
    };
    XLinkHandler_t handler = {
#if defined(USE_USB_VSC)
        .devicePath = name,
#elif defined(USE_USB_CDC)
        .devicePath = "/dev/ttyACM0",
        .devicePath2 = "/dev/ttyACM1",
#elif defined(USE_PCIE)
        .devicePath = "/dev/ma2485_0",
#endif
    };
    int streamId;
#ifndef BOOT_DISABLE
    if( XLinkBootRemote(name, MVCMD_NAME) != X_LINK_SUCCESS){
        //XLinkResetRemote(0); //TODO EMAN - how did this work before, sending event on an openned channel?/
        exit(1);
    }
#endif
    printf("Myriad was booted\n");
    while(XLinkInitialize(&ghandler) != X_LINK_SUCCESS);
    printf("Initialize done\n");

    while(XLinkConnect(&handler) != X_LINK_SUCCESS);
    printf("XLinkConnect done - link Id %d\n", handler.linkId);

    while((streamId = XLinkOpenStream(handler.linkId, "firstStream", DATA_LENGTH)) == 0xDEADDEAD);//readonly
    printf("XLinkOpenStream done - stream Id %d\n", streamId);

    XLinkProfStart();
    // try to open communication. Myriad is not yet booted, so this will fail
    int bootTimes = 100;

    streamPacketDesc_t * packet;


    XLinkError_t status;
    while (bootTimes-- && status != X_LINK_COMMUNICATION_NOT_OPEN)
    {

        printf("*********************************\n");
        printf ("Remaining iterations: %d \n", bootTimes);
        printf("*********************************\n");

        printf("Booting myriad... ");
        fflush(stdout);
        for(k = 0; k < DATA_LENGTH; k++)
        {
            msg[k] = k / 512;
        }
        printf("Sending some data to myriad... ");
        fflush(stdout);

        status = XLinkWriteData(streamId, msg, DATA_LENGTH);
        if (status == X_LINK_SUCCESS)
        {
        }
        else
        {
            printf("myriad set data failed: %x\n", status);
        }
        printf("Data sent\n");
        printf("Reading back data from myriad... ");
        fflush(stdout);

        status = XLinkReadData(streamId, &packet);

        if (status != X_LINK_SUCCESS)
        {
            printf("myriad get data failed: %x\n", status);
        }
        printf("Data read\n");
        assert(packet->length == DATA_LENGTH);
        for(k = 0; k < DATA_LENGTH; k++)
        {
            if (packet->data[k] != (uint8_t)(k /512))
            {
                printf("we were not able to read back data at index %d data: %x %x\n", k, packet->data[k],(uint8_t)(k + bootTimes));
                break;
            }
        }
        status = XLinkReleaseData(streamId);
        if (status == 0)
        {
        }
        else
        {
            printf("release data failed: %x\n", status);
        }
    }
    status = XLinkResetRemote(handler.linkId);
    if (status == 0)
    {
    }
    else
    {
        printf("reset failed: %x\n", status);
    }
    XLinkProfStop();
    XLinkProfPrint();

    exit(0);  // no errors

}

