///
/// @file CmxDma.c
/// 
/// 
/// @ingroup CmxDma
/// @{
///

#include <CmxDma.h>

#ifndef USE_CMX_DMA_NEW_DRIVER

u32 CMXDMA_SECTION *___globalTail[4] = {NULL, NULL, NULL, NULL};

void CmxDmaInitTail(u32 **tail[])
{
    if ((tail[0] == NULL) &&
        (tail[1] == NULL) &&
        (tail[2] == NULL) &&
        (tail[3] == NULL))
    {
        tail[0] = &___globalTail[0];
        tail[1] = &___globalTail[1];
        tail[2] = &___globalTail[2];
        tail[3] = &___globalTail[3];
    }
}
#endif

///@}
