#ifndef _PROFILE_H_
#define _PROFILE_H_

#define MV_PROF_VERSION         (4u)

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

#include <profilerAPI.h>

void __profilerInit();

typedef struct __attribute__((packed)) ProfileBufferEntry_t {
    void* buffer;
    void* end;
    void* __restrict__ head;
} ProfileBufferEntry_t;

// master record
typedef struct __attribute__((packed)) ProfileBufferMasterRecord_t {
    const uint32_t magic; // always 0x666f7270 (prof)
    const uint32_t version; // > 1
    volatile ProfileBufferEntry_t* fProf;
    volatile ProfileBufferEntry_t* sProf;
    volatile ProfileBufferEntry_t* tProf;
    volatile ProfileBufferEntry_t* pProf;
    uint64_t startTime;
    uint8_t coreId; // not const because shave core is retrieved at runtime during init
    const char* myriadChipVersion;
    uint32_t overhead; // time spent in hooks
    int32_t clockSpeed;
} ProfileBufferMasterRecord_t;

extern volatile ProfileBufferMasterRecord_t __profileBufferMasterRecord__;

// when the debugger starts the application sets a breakpoint on this particular function. Whenever
// runw is halted due to this breakpoint, the buffer is collected and interpreted. The execution is resumed
// afterwards
void collectProfileData(struct ProfileBufferEntry_t* p);

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

#define BUFFER_FULL(ProfileEntry_t, entry) (entry).head = (struct ProfileEntry_t*) (entry).buffer

#ifndef profilePushEntry
#define profilePushEntry(ProfileEntry_t, entry, ...) { \
    struct ProfileEntry_t* pe = (struct ProfileEntry_t*) (entry).head; \
    *pe = (struct ProfileEntry_t) {__VA_ARGS__}; \
    (entry).head = ((struct ProfileEntry_t*)((entry).head))+1; \
    /* *(struct ProfileEntry_t*)((entry).head)++ = (struct ProfileEntry_t) {__VA_ARGS__}; */ \
    if (unlikely((entry).head == (entry).end)) { \
        (entry).head = (entry).buffer; \
    } \
}
#endif

#ifdef __cplusplus
} // extern "C"
#endif

// TODO: move to some utils.h
#define UNCACHED(addr) ((void*)(((int)addr) | 0x40000000))

// !!! WARNING !!! never ever break or return in the middle of a CRTITICAL_SECTION_XXX()
#if defined(__shave__)
#include <ShDrvMutex.h>
#elif defined(__RTEMS__)
#include <rtems.h>
#else // BM
#include <swcLeonUtils.h>
#endif

#endif
