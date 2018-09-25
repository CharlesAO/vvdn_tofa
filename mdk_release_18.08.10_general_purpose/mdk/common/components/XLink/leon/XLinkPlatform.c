/*
* Copyright 2018 Intel Corporation.
* The source code, information and material ("Material") contained herein is
* owned by Intel Corporation or its suppliers or licensors, and title to such
* Material remains with Intel Corporation or its suppliers or licensors.
* The Material contains proprietary information of Intel or its suppliers and
* licensors. The Material is protected by worldwide copyright laws and treaty
* provisions.
* No part of the Material may be used, copied, reproduced, modified, published,
* uploaded, posted, transmitted, distributed or disclosed in any way without
* Intel's prior express written permission. No license under any patent,
* copyright or other intellectual property rights in the Material is granted to
* or conferred upon you, either expressly, by implication, inducement, estoppel
* or otherwise.
* Any license under such intellectual property rights must be express and
* approved by Intel in writing.
*/ 
///
/// @file
///
/// @brief     Application configuration Leon header
///
#include "XLinkPlatform.h"

#include "stdio.h"
#include <assert.h>
#include <stdlib.h>
#include <rtems.h>
#include <rtems/libio.h>

#include <pthread.h>
#include <semaphore.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <registersMyriad.h>
#include <DrvRegUtilsDefines.h>
#include "mvMacros.h"

#include "OsDrvUsbPhy.h"
#include "usbpumpdebug.h"
#include "usbpump_application_rtems_api.h"


#ifndef DISABLE_LEON_DCACHE
#define USBPUMP_MDK_CACHE_ENABLE        1
#else
#define USBPUMP_MDK_CACHE_ENABLE        0
#endif  /*DISABLE_LEON_DCACHE*/

//memory pool
#ifndef USB_XLINK_MEM_POOL_SIZE
#define USB_XLINK_MEM_POOL_SIZE (10*1024*1024)
#endif  /*USB_XLINK_MEM_POOL_SIZE*/

#define USB_CHUNK_SIZE (5*1024*1024)

#define MEM_ALIGNMENT 64
#define MAX_SYMBOLS 50

/* USB_VSC*/  
#ifdef USE_USB_VSC
#include "usb_vsc.h"
void* gl_AdrTbl = &VSCCompAdrTbl;
char gl_deviceNameWr[MAX_SYMBOLS] = USB_VSC_DEVNAME;
char gl_deviceNameRd[MAX_SYMBOLS] = USB_VSC_DEVNAME;
size_t gl_packetLength = PACKET_LENGTH;
#endif

/* USB_CDC*/  
#ifdef USE_USB_CDC
#include "usb_uart.h"
void* gl_AdrTbl = &UsbUartAdrTbl;
char gl_deviceNameWr[MAX_SYMBOLS] = "/dev/usb0";
char gl_deviceNameRd[MAX_SYMBOLS] = "/dev/usb1";
size_t gl_packetLength = PACKET_LENGTH;
#endif

/* PCIE_EP*/
#if defined(USE_PCIE) && defined(MA2480)
#include <OsDrvPcieEpStream.h>
void* gl_AdrTbl = &OsDrvPcieEpStreamTable;
char gl_deviceNameWr[MAX_SYMBOLS] = OS_DRV_PCIE_EP_STREAM_DEVNAME;
char gl_deviceNameRd[MAX_SYMBOLS] = OS_DRV_PCIE_EP_STREAM_DEVNAME;
size_t gl_packetLength = PCIE_MAX_BUFFER_SIZE;
#endif

uint32_t usedMemory;
extern sem_t pingSem;

int gl_fdWrite, gl_fdRead;
rtems_device_major_number gl_devMajor = 0;
int gl_protocol = -1;

/*#################################################################################
###################################### INTERNAL ###################################
##################################################################################*/
/*Acknowledge functions need to reside inside each protocols IOdevice.
This is a temporary solution below: (these need to be moved to USB_CDC component)*/
int acknowledgeDataWrite()
{    
#ifdef USE_USB_CDC  
    int nread = 0;
    uint8_t acknowledge;
    while(nread < (int)sizeof(acknowledge))
    {
        int count = read(gl_fdRead, &acknowledge, sizeof(acknowledge));
        if (count > 0)
            nread += count;
    }
    if(nread != sizeof(acknowledge) || acknowledge != 0xEF){
        printf("No acknowledge received %d %d\n", nread, acknowledge);
        return -1;      //error
    }   
#endif
    return 0;
}

int acknowledgeDataRead(int param)
{
#ifdef USE_USB_CDC 
    uint8_t ack = 0xEF;
    int nwrite = write(gl_fdWrite, &ack, sizeof(ack));
    if(nwrite != sizeof(ack))
    {
        printf("Failed to write data %d != %d\n", nwrite, param);
    }
#endif
    return param;
}
/*#################################################################################
###################################### EXTERNAL ###################################
##################################################################################*/
int XLinkWrite(void*fd, void* data, int size, unsigned int timeout)
{
    UNUSED(fd);
    UNUSED(timeout);
    int byteCount = 0, toWrite = 0, rc = 0;

#if defined(USE_USB_CDC) || defined(USE_USB_VSC)
    UNUSED(rc);
    while (byteCount < size){
        toWrite = (gl_packetLength && (size_t)(size - byteCount) > gl_packetLength) \
                    ? gl_packetLength : (size_t)(size - byteCount);
        byteCount += write(gl_fdRead, &((char*)data)[byteCount], toWrite);
        acknowledgeDataWrite();      //temporary
    }
#elif defined(USE_PCIE)
    while (byteCount < size){
        toWrite = (gl_packetLength && (size_t)(size - byteCount) > gl_packetLength) \
                    ? gl_packetLength : (size_t)(size - byteCount);
        rc = write(gl_fdRead, &((char*)data)[byteCount], toWrite);

        if (byteCount == -1) {
            rtems_fatal_error_occurred(byteCount);
        }

        byteCount += rc;
    }
#endif
    return byteCount;
}

int XLinkRead(void*fd, void* data, int size, unsigned int timeout)
{
    UNUSED(fd);
    UNUSED(timeout);    
    int byteCount = 0, toRead = 0, rc = 0;

 #if defined(USE_USB_CDC) || defined(USE_USB_VSC)
    UNUSED(rc);
    while(byteCount < size) { 
        toRead = (gl_packetLength && (size_t)(size - byteCount) > gl_packetLength) \
                    ? gl_packetLength : (size_t)(size - byteCount);       
        byteCount += read(gl_fdWrite, &((char*)data)[byteCount], toRead);
        acknowledgeDataRead(size);   //temporary
    }
 #elif defined(USE_PCIE)
    while(byteCount < size) { 
        toRead = (gl_packetLength && (size_t)(size - byteCount) > gl_packetLength) \
                    ? gl_packetLength : (size_t)(size - byteCount);       
        rc = read(gl_fdWrite, &((char*)data)[byteCount], toRead);

        if (byteCount == -1) {
           printf("EP Driver read failed \n");
           rtems_fatal_error_occurred(byteCount);
        }

        byteCount += rc;
    }
 #endif
    return byteCount;
}

int XLinkPlatformGetDeviceName(int index, char* name, int nameSize)
{
    UNUSED(index);
    UNUSED(name);
    UNUSED(nameSize);
    return -1;
}

int XLinkPlatformResetRemote(void* fd)
{
    UNUSED(fd);
    close(gl_fdWrite);
    if(gl_fdRead != gl_fdWrite)
        close(gl_fdRead);

    // Reset Myriad device if using USB
 #if defined(USE_USB_CDC) || defined(USE_USB_VSC)     /*USB CLASSES*/
    SET_REG_WORD(CPR_MAS_RESET_ADR, 0);
#endif

    return 0;
}

int XLinkPlatformConnect(const char* devPathRead,
                           const char* devPathWrite,
                           void** fd)
{
    /*Nothing to do here on Myriad*/
    UNUSED(devPathRead);
    UNUSED(devPathWrite);
    UNUSED(fd);
    return 0;
}

int XLinkPlatformInit(protocol_t protocol, int loglevel)
{
    UNUSED(loglevel);
    gl_protocol = protocol;     

    rtems_status_code sc = rtems_io_register_driver(0, gl_AdrTbl, &gl_devMajor);
    if (sc != RTEMS_SUCCESSFUL) {
        printf("Driver Register failed !!!!\n");
        rtems_fatal_error_occurred(sc);
    }

    gl_fdWrite = open(gl_deviceNameWr, O_RDWR);

 #if defined(USE_USB_CDC) || defined(USE_USB_VSC)     /*USB CLASSES*/
    gl_fdRead = open(gl_deviceNameRd, O_RDWR);
    if((gl_fdWrite < 0 || gl_fdRead < 0)) {
        printf("No device opened !!!! descriptors: %d %d\n", gl_fdWrite, gl_fdRead);
    }
 #elif defined(USE_PCIE)
    gl_fdRead = gl_fdWrite;
 #endif
    return 0;
}

void deallocateData(void* ptr, uint32_t size, uint32_t alignment)
{
    UNUSED(alignment);
    if (!ptr)
        return;
    free(ptr);
    usedMemory -= size;
}

void* allocateData(uint32_t size, uint32_t alignment)
{
    assert(MEM_ALIGNMENT % alignment == 0);
    void* ret = NULL;
    if (usedMemory + size > USB_XLINK_MEM_POOL_SIZE) {
        printf("Not enough memory %ld %ld %d\n",
            usedMemory, size, USB_XLINK_MEM_POOL_SIZE);
        return ret;
    }
    posix_memalign(&ret, alignment, size);
    usedMemory += size;
    return ret;
}

int XLinkPlatformBootRemote(const char* deviceName, const char* binaryPath)
{
    UNUSED(deviceName);
    UNUSED(binaryPath);
    return -1; // would be nice to boot the PC, but let's not do that.
}

/* end of file */
