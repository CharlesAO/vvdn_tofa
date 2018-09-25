#ifndef __IPIPE_UTILS_H__
#define __IPIPE_UTILS_H__

#include "ipipe.h"
void     dumpAndVerification(char *refName, void *refBuf, void *outputBuf, unsigned int size);
uint16_t f32_to_f16(float f);
uint16_t int_to_f16(int i);

//Bypass Leon's  L2 caches
static inline uint32_t ipipeGetNoCacheAddr(uint32_t addr) {
    if((addr >> 28) == 0x7) return ((addr | 0x08000000)); //CMX addr
    if((addr >> 28) == 0x8) return ((addr | 0x40000000)); //DDR addr
    return ((addr)); //else ?
}
//Enable Leon's  L2 caches
static inline uint32_t ipipeGetCacheAddr(uint32_t addr) {
    if((addr >> 28) == 0x7) return ((addr & (~0x08000000))); //CMX addr
    if((addr >> 28) == 0x8) return ((addr & (~0x40000000))); //DDR addr
    return ((addr)); //else ?
}
uint32_t lineWidthInBytes(uint32_t widthInPixels, uint32_t bpp);

// use dot tools in order to see the draw
//#define DBG_DUMP_GRAPH

#ifdef DBG_DUMP_GRAPH
void dumpDotFileLrtAppGraph(icCtrl *ctrl);
#define DBG_DUMP_DOT_FILE_APP_GRAPH(X) dumpDotFileLrtAppGraph(X)
#else
#define DBG_DUMP_DOT_FILE_APP_GRAPH(X)
#endif
#endif
