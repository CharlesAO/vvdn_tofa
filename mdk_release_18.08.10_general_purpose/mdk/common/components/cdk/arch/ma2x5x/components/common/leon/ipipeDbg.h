#ifndef __IPIPE_DBG_H__
#define __IPIPE_DBG_H__

#include "IcTypes.h"
#include <assert.h>


/*
 * Internal debug code, disabled by default.
 */
//#define IPIPE2_ENABLE_DBG_LOCAL

#ifdef IPIPE2_ENABLE_DBG_LOCAL
#define IPIPE2_DBG_MAX_MESSAGE 8096
typedef struct {
    uint32_t id;
    uint32_t mes;
}ipipeDbfMsgBufT;
extern volatile ipipeDbfMsgBufT ipipe2DbgMsgBuf[IPIPE2_DBG_MAX_MESSAGE];
extern volatile uint32_t ipipe2DbgMsgIdx;
static inline void ipipe2DbgAddMes(uint32_t id, uint32_t message) {
    ipipe2DbgMsgBuf[ipipe2DbgMsgIdx].id = id;
    ipipe2DbgMsgBuf[ipipe2DbgMsgIdx].mes = message;
    ipipe2DbgMsgIdx++;
    //assert(ipipe2DbgMsgIdx < IPIPE2_DBG_MAX_MESSAGE);
    if (ipipe2DbgMsgIdx == IPIPE2_DBG_MAX_MESSAGE ) {
        ipipe2DbgMsgIdx = 0;
    }
}
#else
#define ipipe2DbgAddMes(X,Y)
#endif

#endif //__IPIPE_DBG_H__
