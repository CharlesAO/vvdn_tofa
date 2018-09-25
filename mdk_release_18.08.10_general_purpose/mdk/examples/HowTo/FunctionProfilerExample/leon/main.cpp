///
/// @file

/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///
// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <DrvCpr.h>
#include <DrvLeon.h>

#include <pthread.h>

#include <dbgTracerApi.h>

#include <UnitTestApi.h>
//extern "C" {
//#include "rtems_config.h"
//}

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 lrt_start;
extern char lrt_msgbuff[128];

// 4: Static Local Data
// ----------------------------------------------------------------------------
static int rc1, rc2, rc3;
static pthread_t thread1, thread2, thread3;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
void* functionC(void *ptr);
void* functionD(void *ptr);
void* functionE(void *ptr);

#if no_instrument_works
template <int howManyTimes>
//__attribute__((no_instrument_function))
inline __attribute__((always_inline))
void _nop(void) {
	asm volatile ("nop");
	_nop<howManyTimes-1>();
}
template<>
//__attribute__((no_instrument_function))
inline __attribute__((always_inline))
void _nop<0>(void) { }
#else
template <int n>
inline __attribute__((always_inline))
void _nop() {
	for(int i=0; i<n; ++i) asm volatile ("nop");
}
#endif

static void someStaticFunction() {
	_nop<15>();
}

/**
 * Hidden visibility indicates that the entity declared will have a new form
 * of linkage, which we'll call “hidden linkage”. Two declarations of an object
 * with hidden linkage refer to the same object if they are in the same shared object.
 */
void __attribute__ ((visibility ("hidden"))) hiddenFunction() {

}

/**
 * Internal visibility is like hidden visibility, but with additional
 * processor specific semantics. Unless otherwise specified by the psABI,
 * GCC defines internal visibility to mean that a function is never
 * called from another module. Compare this with hidden functions which,
 * while they cannot be referenced directly by other modules, can be
 * referenced indirectly via function pointers. By indicating that a
 * function cannot be called from outside the module, GCC may for instance
 * omit the load of a PIC register since it is known that the calling
 * function loaded the correct value.
 */
void __attribute__ ((visibility ("internal"))) internalFunction() {

}

/**
 * Protected visibility is like default visibility except that it
 * indicates that references within the defining module will bind
 * to the definition in that module. That is, the declared entity
 * cannot be overridden by another module.
 */
void __attribute__ ((visibility ("protected"))) protectedFunction() {

}

// this is template function in order to increase the chance to optimize nops
template <int nbNopsBefore, int nbNopsAfter>
void recFunc(unsigned int level) {

	_nop<nbNopsBefore>();

	if (level > 1) {
		recFunc<nbNopsBefore, nbNopsAfter>(level - 1);
	}

	_nop<nbNopsAfter>();

}


extern "C" void initProfilerTimers();

// 6: Functions Implementation
// ----------------------------------------------------------------------------
extern "C"
void *POSIX_Init (void * /* args */)
{
    pthread_attr_t attr;
    s32 sc;

//    sc = initClocksAndMemory();
//    if(sc)
//        exit(sc);

    // TODO: configure timers
    initProfilerTimers();

		unitTestInit ();
    //printf ("\n");
    printf ("RTEMS POSIX Started\n");  /* initialise variables */

    sc = pthread_attr_init(&attr);
    if(sc) {
        printf("pthread_attr_init error");
        exit(sc);
    }
    sc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if(sc) {
        printf("pthread_attr_setinheritsched error");
        exit(sc);
    }
    sc = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if(sc) {
        printf("pthread_attr_setschedpolicy error");
        exit(sc);
    }

    if ((rc1=pthread_create( &thread1, &attr, functionC, &lrt_start))) {
        printf("Thread 1 creation failed: %d\n", rc1);
        exit(rc1);
    }
    else {
        printf("Thread 1 created\n");
    }
    //pthread_set_name_np(rc1, "thr1");

    if ((rc2=pthread_create( &thread2, &attr, functionD, &lrt_start))) {
        printf("Thread 2 creation failed: %d\n", rc1);
        exit(rc1);
    }
    else {
        printf("Thread 2 created\n");
    }

    if ((rc3=pthread_create( &thread3, &attr, functionE, &lrt_start))) {
        printf("Thread 3 creation failed: %d\n", rc1);
        exit(rc1);
    }
    else {
        printf("Thread 3 created\n");
    }

    if(pthread_join( thread1, NULL) || pthread_join( thread2, NULL) || pthread_join( thread3, NULL) ) {
        printf("pthread_join error!\n");
    }

    //
		unitTestFinalReport ();

    exit(0);
}

void* functionC(void *ptr){

    printf("LeonRT Start Addr: 0x%lX\n",(uint32_t)ptr);

    DrvLeonRTStartup((u32)ptr);
    DrvLeonRTWaitExecution();

    printf("%s", lrt_msgbuff);
    printf("LeonRT Stop.\n");

//    pthread_exit(0);
    return nullptr;
}

void* functionD(void * /*ptr*/){
	// thread "D"
	printf("thread D start\n");
	for (int i=0; i<1000; i++) {
		recFunc<400, 800>(4);
		recFunc<200, 300>(2);
	}
	someStaticFunction();
	//rtems_task_wake_after(1); // funny method doing what?
	printf("thread D ending\n");
//	pthread_exit(0);
	return nullptr;
}

void* functionE(void * /*ptr*/){
	// thread "E"
	printf("thread E start\n");
	recFunc<200, 300>(2);
	hiddenFunction();
	internalFunction();
	protectedFunction();
	printf("thread E ending\n");
//	pthread_exit(0);
    return nullptr;
}
