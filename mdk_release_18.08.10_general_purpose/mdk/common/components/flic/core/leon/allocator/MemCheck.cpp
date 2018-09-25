///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dynamic memory allocation checks
///

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

//==========================
#if defined(ALLOC_DEBUG)
//==========================

#include "FlicAtomic.h"

typedef struct{
    void       *sys;
    void       *addr;
    uint32_t    size;
    const char *file;
    int         line;
}AllocEntry;

//max number of allocs (concurrent)
#ifndef ALLOC_MAX
#define ALLOC_MAX 128
#endif

//default alignment target
#ifndef NEW_ALIGN
#define NEW_ALIGN 16
#endif

//print all new/delete ops
#ifndef ALLOC_PRINT
#define ALLOC_PRINT 0
#endif

#define CANARY_SZ 4 //[bytes]
const uint8_t canary[] = {'F','L','I','C'};

AllocEntry allocBuf[ALLOC_MAX];

//################################################################
void AllocInit(){
   memset(allocBuf, 0, sizeof(allocBuf));
}
//################################################################
static int _findEmpty()
{
   for(int i=0; i<ALLOC_MAX; i++)
     if(allocBuf[i].addr == 0)
       return i;
  //can't record new allocs, meaning we got more than ALLOC_MAX allocs
   assert(0);
}
//################################################################
static int _markEmpty(void *addr)
{
   for(int i=0; i<ALLOC_MAX; i++)
     if(allocBuf[i].addr == addr){
        allocBuf[i].addr = 0;
        return i;
     }
  //invalid release
   assert(0);
   return -1;
}
//################################################################
static void * _align(void *in, uint32_t target)
{
    target--;
    uint32_t ret = ((uint32_t)in + target) & ~(target);
    return (void*)ret;
}
//################################################################
static void _canaryWr(void *ptr, uint32_t sz)
{
    uint8_t *c = (uint8_t*)ptr + sz;
    c[0] = canary[0];  c[1] = canary[1];
    c[2] = canary[2];  c[3] = canary[3];
}
//################################################################
static int _canaryRd(void *ptr, uint32_t sz)
{
    uint8_t *c = (uint8_t*)ptr + sz;

    return ((c[0] == canary[0]) && (c[1] == canary[1]) &&
            (c[2] == canary[2]) && (c[3] == canary[3]))  ? 0 : 1;
}

//################################################################
static void * _my_new(size_t size, const char *file, int line)
{
    Atomic a;

  //must Find & Mark (i.e. set .addr) an available slot
  //as a single atomic operation
    a.Enter();
    int   pos = _findEmpty();
    allocBuf[pos].sys  = malloc(size + NEW_ALIGN + CANARY_SZ);
    allocBuf[pos].addr = _align(allocBuf[pos].sys, NEW_ALIGN);
    a.Leave();

    allocBuf[pos].size = size;
    allocBuf[pos].file = file;
    allocBuf[pos].line = line;
   _canaryWr(allocBuf[pos].addr, allocBuf[pos].size);

    if(ALLOC_PRINT)
      printf("new : addr = 0x%lx sz = %d, %s:%d\n", (uint32_t)allocBuf[pos].addr, size, file, line);

    return allocBuf[pos].addr;
}
void * operator new  (size_t size, const char *file, int line){return _my_new(size, file, line);}
void * operator new[](size_t size, const char *file, int line){return _my_new(size, file, line);}

//for allocs from files that don't include Flic.h (MemCheck.h actually)
void * operator new  (size_t size)                            {return _my_new(size, "N/A",   0);}
void * operator new[](size_t size)                            {return _my_new(size, "N/A",   0);}


//################################################################
void _my_delete(void *p){
    int pos = _markEmpty(p);

    if(ALLOC_PRINT)
     printf("delete %lx\n", (uint32_t)p);

    free(allocBuf[pos].sys);
}
void operator delete  (void *p) {_my_delete(p);}
void operator delete[](void *p) {_my_delete(p);} //??? same thing ???

//################################################################
int CheckOverflow(int echo)
{
  uint32_t err = 0;
  for(int i=0; i<ALLOC_MAX; i++)
    if(allocBuf[i].addr){
       int e = _canaryRd(allocBuf[i].addr, allocBuf[i].size);
       if(echo && e)
         printf("OVERFLOW alloc no %d\n", i);
       err += e;
    }
  return err;
};

//################################################################
//All allocs that still exist are treated as MEM-LEAKS
int CheckMemLeaks(int echo)
{
  int err = 0;
  for(int i=0; i<ALLOC_MAX; i++)
    if(allocBuf[i].addr){
      if(echo)
         printf("LEAKED addr = 0x%08lx size = %ld, %s, line %d\n",
                   (uint32_t)allocBuf[i].addr, allocBuf[i].size,
                             allocBuf[i].file, allocBuf[i].line);
      err++;
    }
  return err;
}

//################################################################
//Debug
void ShowAllocs()
{
    printf("Allocs_begin:\n");
    for(int i=0; i<ALLOC_MAX; i++)
    {
      if(allocBuf[i].addr)
        printf("[%3d] addr = 0x%08lx size = %ld, %s, line %d\n", i,
                (uint32_t)allocBuf[i].addr, allocBuf[i].size,
                          allocBuf[i].file, allocBuf[i].line);
    }
    printf("Allocs_end.\n");
}

#endif //ALLOC_DEBUG
