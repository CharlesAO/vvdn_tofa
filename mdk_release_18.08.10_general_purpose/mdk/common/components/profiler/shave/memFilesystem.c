#ifdef FS_ENABLE

#include <stdint.h>
#include <errno.h>

//#include <stdio.h> // TODO: for test only

typedef char ProfileEntry_t;
// TODO: rework this oop hack using dependency inversion in c++
#define PROFILE_ENTRY_DEFINED
#include "profile.h"

#ifndef FS_TOTAL_SIZE
#define FS_TOTAL_SIZE (1024*10*5)
#endif

ProfileEntry_t __attribute__ ((aligned (32)))
   __fileSystemBuffer[FS_TOTAL_SIZE/sizeof(ProfileEntry_t)]
   __attribute__((section(".ddr_direct.data")))
   ;


//volatile int __attribute__((section(".ddr.data"))) fpDummy = 0;

//__attribute__((section(".ddr.data")))
__attribute__((section(".ddr_direct.data")))
static ProfileBufferEntry_t pProfile = {
   (struct ProfileEntry_t*) __fileSystemBuffer,
   &__fileSystemBuffer[FS_TOTAL_SIZE/sizeof(ProfileEntry_t)],
   (struct ProfileEntry_t*)__fileSystemBuffer
};

// FIXME: when close should be implemented, rework this
static int lastOpenedFile = 2; // stderr

// FIXME: ugly code. rework if close will be implemented
#ifndef MAX_OPEN_FILES
#define MAX_OPEN_FILES 10
#endif

/**
* The buffer holds all the operations made on filesystem.
* Each operation is 32 bit aligned
*
* open()
* | 32bit filename size | (uint32_t)-1 | filename | (uint32_t) flags | (uint32_t) mode | (uint32_t) created fd |
*
* write()
* | 32bit data size | (uint32_t)fd | data |
*
*/

unsigned int initDone = 0;
__attribute__((constructor))
__attribute__((no_instrument_function))
void initFS() {
    if (initDone) return;
    initDone ++;
//    fpDummy = 99;
    pProfile.buffer = UNCACHED(pProfile.buffer);
    pProfile.head = UNCACHED(pProfile.head);
    pProfile.end  = UNCACHED(pProfile.end);
    __profileBufferMasterRecord__.pProf = UNCACHED(&pProfile);
    __builtin_memset(__fileSystemBuffer, 0, sizeof(__fileSystemBuffer)/sizeof(__fileSystemBuffer[0]));
    // initDone+=(int)__profileBufferMasterRecord__.pProf;
    __profilerInit();
    //printf("mem fs initialized: %x %d\n", (unsigned int)&__profileBufferMasterRecord__, __profileBufferMasterRecord__.coreId);
}

__attribute__((no_instrument_function))
static inline void put_byte(char byte) {
	*((char*)pProfile.head) = byte;
    pProfile.head = (struct ProfileEntry_t*)(((char*)pProfile.head) + 1);
}
__attribute__((no_instrument_function))
static inline void put_word(uint32_t word) {
    put_byte((word      ) & 0xFF);
    put_byte((word >>  8) & 0xFF);
    put_byte((word >> 16) & 0xFF);
    put_byte((word >> 24) & 0xFF);
}
__attribute__((no_instrument_function))
static inline void copy(const void* p, size_t len) {
    char* bytes = (char*) p;
    for (size_t i=0; i<len; i++) {
        char what = *bytes++;
        put_byte(what);
    }
}


// syscalls alias implementation

// | 32bit filename size | (uint32_t)-1 | (uint32_t) flags | (uint32_t) mode | (uint32_t) created fd | filename |
int __attribute__((no_instrument_function)) open(const char* name, int flags, int mode) {
    if (!initDone) initFS();
    if (lastOpenedFile == MAX_OPEN_FILES) {
        errno = ENFILE; // The system-wide limit on the total number of open files has been reached.
        return -1;
    }

    uint32_t nameLen = __builtin_strlen(name) + 1;

    // start here
    put_word(nameLen);
    put_word(-1);
    put_word(flags);
    put_word(mode);
    put_word(++lastOpenedFile);
    copy(name, nameLen);
    return lastOpenedFile;
}

int __attribute__((no_instrument_function)) __svu_write(int file, const void* p, size_t len) {
    if (len + 16 > (size_t)(pProfile.end - (void*)pProfile.head) ) {
        errno = ENOSPC;
        return -1;
    }

    // start here
    put_word(len);
    put_word(file);
    copy(p, len);
    return len;
}

#if ctor_replacement
static const void *_keep_[] __attribute__((used, section(".preinit.profiler"))) = {
    &initFS
};
#endif

#endif // FS_ENABLE
