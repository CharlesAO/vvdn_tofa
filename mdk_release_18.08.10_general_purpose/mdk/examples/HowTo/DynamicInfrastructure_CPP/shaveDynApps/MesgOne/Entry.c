/*
 * Entry.c
 *
 *  Created on: Mar 12, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <swcCdma.h>
#include <svuCommonShave.h>
#include "theDynContext.h"

unsigned int __attribute__((section(".bss"))) myArray[10];

typedef void (*printfunc_t)(const char *format, ...);
extern unsigned int someArray[20];

printfunc_t theMDKprintf;

dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) task1;
dmaTransactionList_t __attribute__((section(".cmx.cdmaDescriptors"))) *ref1;

void CMXCODE( CMXFunction)(void)
{
    scMutexRequest(4);
    (theMDKprintf)("Setting the memory to something else running from a CMX function!\n");
    scMutexRelease(4);
    myArray[5]=0xC0FEBABE;
}

void __attribute__((noinline)) DDRFunction(void) {
    scMutexRequest(4);
    (theMDKprintf)("Setting the memory to something else running from a DDR function.\n");
    scMutexRelease(4);
    myArray[5] = 0xC0FEBEEF;
}

int Entry(void) {
    float someVar;
    u32* inAddress;
    u32* outAddress;

    scMutexRequest(4);
    int i;
    someVar = 7.0;
    printf("Printing a float value: %0.2f\n", someVar);
    myArray[5] = 513;
    printf("Some local memory is: 0x%X\n", myArray[5]);
    printf("Some leon shared memory is: 0x%X\n", someArray[15]);
    scMutexRelease(4);

    //Performing one DDR transfer
    // Initialize tasks handle
    u32 id1 = dmaInitRequester(1);

    inAddress = (u32*) ((u32) 0x80000000);
    outAddress = (u32*) (0x70000000 + 1024 * 128 * 6);

    //Transfer some memory to the 6th, 7th and 8th CMX slice (320 KB)
    ref1 = dmaCreateTransaction(id1, &task1, (u8*) inAddress, (u8*) outAddress,
            320 * 1024);
    dmaStartListTask(ref1);
    dmaWaitTask(ref1);
    scMutexRequest(4);
    printf("---------------------------------------------\n");
    printf("Some of the original DDR values:\n");
    for (i = 0; i < 3; i++) {
        printf("0x%X=0x%X|", (unsigned int) &inAddress[i], inAddress[i]);
        if ((i & 0x3) == 0)
            printf("\n");
    }
    printf("\n");
    printf("The same in CMX:\n");
    for (i = 0; i < 3; i++) {
        printf("0x%X=0x%X|", (unsigned int) &outAddress[i], outAddress[i]);
        if ((i & 0x3) == 0)
            printf("\n");
    }
    printf("---------------------------------------------\n");
    printf("\n");
    scMutexRelease(4);
    return 0;
}
