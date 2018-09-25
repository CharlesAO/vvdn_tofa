#ifndef _MYRIAD2CFG_H_
#define _MYRIAD2CFG_H_

#define SHAVE_SECTION(x)                         \
    extern unsigned char __shave##x##_cmx_text_start[], __shave##x##_cmx_text_end[], __shave##x##_cmx_data_start[], __shave##x##_cmx_data_end[];

SHAVE_SECTION(0);SHAVE_SECTION(1);SHAVE_SECTION(2);SHAVE_SECTION(3);
SHAVE_SECTION(4);SHAVE_SECTION(5);SHAVE_SECTION(6);SHAVE_SECTION(7);
SHAVE_SECTION(8);SHAVE_SECTION(9);SHAVE_SECTION(10);SHAVE_SECTION(11);
#ifdef MA2X8X
SHAVE_SECTION(12);SHAVE_SECTION(13);SHAVE_SECTION(14);SHAVE_SECTION(15);
#endif /* MA2x8x */

#include "../../MemoryManager/shared/include/memManagerAreas.h"

// Initialize memory manager structure
void MemMgrInitialize(void);

#endif
