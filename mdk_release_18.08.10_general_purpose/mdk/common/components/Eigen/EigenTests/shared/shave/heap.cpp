#include <stdlib.h>
#include <stdio.h>
#include <sys/shave_system.h>

#define HEAP_SIZE (2048 * 1024)

/* This is the working heap. Can be placed anywhere.
 */
char heap[HEAP_SIZE];

/* The heap must be set before anything else happens on the Shave.
 */
void initHeap()
{
	__setheap((void*)heap, HEAP_SIZE);
}

/* All the function pointers defined in the .preinit section are called by crt0
 * when __crtinit is launched from the Leon. This must happen before any other
 * Shave code is launched. Similarily, __crtfini must be called after the Shave
 * application has finished to ensure proper resource deallocation (think about
 * freeing memory, mutexes, write-backs etc).
 */
__attribute__((section(".preinit")))
void (*preInitHeapPtr)(void) = &initHeap;
