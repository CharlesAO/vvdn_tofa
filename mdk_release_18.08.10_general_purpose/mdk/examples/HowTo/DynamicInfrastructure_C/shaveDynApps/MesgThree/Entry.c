/*
 * Entry.c
 *
 *  Created on: Mar 12, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <svuCommonShave.h>

typedef void (*printfunc_t)(const char *format, ...);

unsigned int __attribute__((section(".bss"))) myArray[1024];
extern unsigned int lrt_someRTArray[20];

printfunc_t theMDKprintf;

void CMXCODE( CMXFunction)(void)
{
    scMutexRequest(4);
    (theMDKprintf)("I'm setting the other memory to something else running from a CMX function!\n");
    scMutexRelease(4);
    myArray[512]=0xC0FEBABE;
}

void DDRFunction(void) {
    scMutexRequest(4);
    (theMDKprintf)("Setting the memory to something else running from a DDR function\n");
    scMutexRelease(4);
    myArray[512] = 0xC0FEBEEF;
}

int Entry(void) {
    scMutexRequest(4);
    printf("Other local memory to MesgThree is: 0x%X\n", myArray[512]);
    scMutexRelease(4);
    myArray[512] = 0xC0FEC0FE;
    scMutexRequest(4);
    printf("Same memory from MesgThree is now: 0x%X\n", myArray[512]);
    scMutexRelease(4);

    theMDKprintf = (printfunc_t) printf;
    DDRFunction();

    CMXFunction();
    scMutexRequest(4);
    printf("After jumping out of a CMX function, that memory became: 0x%X\n", myArray[512]);
    scMutexRelease(4);
    scMutexRequest(4);
    printf("Shared memory from this shave at location 0x%X from a LeonOS declared variable is 0x%X.\n",
            (unsigned int) &lrt_someRTArray[10], lrt_someRTArray[10]);
    scMutexRelease(4);
    return 0;
}
