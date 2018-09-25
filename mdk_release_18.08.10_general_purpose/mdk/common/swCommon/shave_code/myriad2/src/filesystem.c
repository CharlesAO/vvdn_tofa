#include <stdint.h>
#include <reent.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <_syslist.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>


void __attribute__((weak, no_instrument_function)) __svu_open(const char* name, int flags, int mode) {
    (void) name;
    (void) flags;
    (void) mode;
}
// TODO: check if should be _open or open
void __attribute__((weak, alias("__svu_open"))) _open(const char* name, int flags, int mode);

#if 1
void __attribute__((weak, no_instrument_function)) __svu_write(int file, const void* p, size_t len) {
    (void) file;
    (void) p;
    (void) len;
}
void __attribute__((weak, alias("__svu_write"))) _svu_write(int file, const void* p, size_t len);
#else
void __svu_write(int file, const void* p, size_t len);
#define _svu_write __svu_write
#endif

void _put_output_char(int c);

int
__attribute__((no_instrument_function))
write(int file, const void* p, size_t len)
{
    if (file < 1) {
        errno = EBADF;
        return -1;
    }

    if (file == 1 || file == 2) {
        unsigned i=0;
        const char* msg = (const char*) p;
        while (i<len)
            _put_output_char(msg[i++]);
        return i;
    }

    _svu_write(file, p, len);

    return len;
}
