#ifndef __POOL_OBJ_H__
#define __POOL_OBJ_H__

#include <stdint.h>

//Each buffer descriptor (T) "must"(not really) inherit PoBuf
//as for each buffer, we need to know (as a starting poing) base + size
struct PoBuf {
    void     *base;
    uint32_t  size;
};

#endif