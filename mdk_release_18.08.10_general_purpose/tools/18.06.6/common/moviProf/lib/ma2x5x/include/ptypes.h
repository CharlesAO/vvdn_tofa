#ifndef __P_TYPES_H__
#define __P_TYPES_H__

#ifdef __cplusplus
 #include <cstdint>
 extern "C" {
#else
 #include <stdint.h>
#endif

// TODO: union with the other profilers
typedef struct __attribute__ ((packed)) ProfileEntry_t {
    uint32_t functionAddress;
    uint32_t context;
    uint64_t timestamp;
} ProfileEntry_t;

typedef struct __attribute__((packed)) ProfileBufferEntry_t {
    uint32_t headerSize;
    uint32_t overhead; // time spent in hooks
    //void *end;
    ProfileEntry_t * __restrict__ head;
    ProfileEntry_t * end;
    uint64_t startTime;
    uint8_t coreId; // not const because shave core is retrieved at runtime during init
    uint8_t bufferType;
    uint8_t _unused[6]; // only for alignment. can be removed
} ProfileBufferEntry_t;

// master record
typedef struct __attribute__((packed)) ProfileBufferMasterRecord_t {
    uint32_t magic; // always 0x666f7270 (prof)
    uint32_t version; // > 1
    const char *myriadChipVersion;
    uint32_t entryCnt;
    const char* toolsVersion;
    uint32_t sysClockMhz;
    uint32_t _unused[2];
} ProfileBufferMasterRecord_t;

typedef enum FunctionProfileEntryType {
    HOOK_FUNC_ENTRY = 0x1, /*function entry hook*/
    HOOK_FUNC_EXIT,         /*function exit hook*/
    HOOK_TASK_SWITCH,        /*task entry hook*/
    HOOK_ISR_ENTRY,         /*ISR entry hook*/
    HOOK_ISR_EXIT,           /*ISR exit  hook*/
    HOOK_MEM_VALUE          /*used to log 64 bit value variables, registers, etc*/
} FunctionProfileEntryType;

#define UNCACHED(addr) ((void*)(((int)(addr)) | 0x40000000))

#ifndef FPROFILER_DATA_SECTION
#define FPROFILER_DATA_SECTION __attribute__((section(".ddr_direct.bss")))
#endif

#define FPROFILER_CHUNK_SIZE (sizeof(ProfileBufferEntry_t) + (1024 * sizeof(ProfileEntry_t)))

#ifdef PROFILE_DEBUG
extern volatile int _profile_unhalt;
#define p_assert(cond) if (!(cond)) {\
        printf("assert failed %s: %d\n", __FILE__, __LINE__); \
        while (_profile_unhalt == 0); \
        asm volatile("" : : : "memory"); \
        _profile_unhalt = 0; /* set for the next potential assert */ \
    }
#else
#define p_assert(cond) (void)(cond)
#endif

#define noopt() asm volatile("" : : : "memory")

#ifdef __cplusplus
 } // extern "C"
#endif

#endif
