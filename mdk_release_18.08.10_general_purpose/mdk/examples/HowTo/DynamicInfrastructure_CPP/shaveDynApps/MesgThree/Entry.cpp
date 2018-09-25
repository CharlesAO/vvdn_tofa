/*
 * Entry.c
 *
 *  Created on: Mar 12, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <svuCommonShave.h>
#include <iostream>
using namespace std;

typedef void (*printfunc_t)(const char *format, ...);
extern unsigned int lrt_someRTArray[20];

printfunc_t theMDKprintf;

class Test  // local to fun
{
private:
    char *str;
public:
    // Fine as the method is defined inside the local class
    int method(int a, int b) {
        int c;
        c = a + b;
        str = new char[25];
        if (nullptr == str)
            printf("Memory could not be allocated on the shave\n");
        else
            printf("Some heap allocated memory at 0x%x\n", (unsigned int) str);
        return c;
    }
};

void __attribute__((noinline)) DDRFunction(void) {
    scMutexRequest(4);
    (theMDKprintf)("Calling printf from a DDR function.\n");
    scMutexRelease(4);
}

void __attribute__((noinline)) funABC(int p1, int p2) {
    int __attribute__((unused)) m; // avoid 'unused variable' error from scan build

    Test t;
    m = t.method(p1, p2);
}

extern "C" int Entry(void) {
    funABC(2, 8);

    theMDKprintf = (printfunc_t) printf;
    DDRFunction();

    scMutexRequest(4);
    printf("Some shared memory from this shave at location 0x%X from a LeonRT declared variable is 0x%X.\n",
            (unsigned int) &lrt_someRTArray[10], lrt_someRTArray[10]);
    scMutexRelease(4);

    return 0;
}
