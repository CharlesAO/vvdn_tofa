
#include "ipipeDbg.h"
/*
 * Internal debug code, disabled by default.
 */

#ifdef IPIPE2_ENABLE_DBG_LOCAL
volatile ipipeDbfMsgBufT ipipe2DbgMsgBuf[IPIPE2_DBG_MAX_MESSAGE];
volatile uint32_t ipipe2DbgMsgIdx = 0;
#endif
