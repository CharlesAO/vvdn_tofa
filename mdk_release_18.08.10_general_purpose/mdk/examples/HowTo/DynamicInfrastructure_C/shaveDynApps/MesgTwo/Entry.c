/*
 * Entry.c
 *
 *  Created on: Mar 12, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <svuCommonShave.h>

unsigned int __attribute__((section(".bss"))) myArray[1024];

typedef void (*printfunc_t)(const char *format, ...);
extern unsigned int someArray[200];

printfunc_t theMDKprintf;

void CMXCODE( CMXFunction)(void)
{
    scMutexRequest(4);
    (theMDKprintf)("Setting the other memory to something else running from a CMX function.\n");
    scMutexRelease(4);
    myArray[512]=0xC0FEBABE;
}

void DDRFunction(void) {
    scMutexRequest(4);
    (theMDKprintf)("Setting the memory to something else running from a DDR function.\n");
    scMutexRelease(4);
    myArray[512] = 0xC0FEBEEF;
}

int Entry(void) {
    scMutexRequest(4);
    printf("Other memory from MesgTwo app is: 0x%X\n", myArray[512]);
    scMutexRelease(4);
    myArray[512] = 0xC0FEC0FE;
    scMutexRequest(4);
    printf("Same memory for MesgTwo is now: 0x%X\n", myArray[512]);
    scMutexRelease(4);

    theMDKprintf = (printfunc_t) printf;
    DDRFunction();

    CMXFunction();
    scMutexRequest(4);
    printf("I've got another of these messages after jumping out of a CMX function! It says that memory became: 0x%X\n",myArray[512]);
    scMutexRelease(4);
    scMutexRequest(4);
    printf("Some shared memory from this shave at location 0x%X from a LeonOS declared variable is 0x%X.\n",
            (unsigned int) &someArray[100], someArray[100]);
    scMutexRelease(4);

    return 0;
}
