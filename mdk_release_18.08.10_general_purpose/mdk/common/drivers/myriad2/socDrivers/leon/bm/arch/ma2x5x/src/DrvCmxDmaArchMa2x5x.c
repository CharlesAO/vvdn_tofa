///
/// @file DrvCmxDmaArchMa2x5x.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvCmxDmaArchMa2x5x
/// @{
///


#include "DrvCmxDma.h"

#ifndef USE_CMX_DMA_NEW_DRIVER

int DrvCmxDmaSetup(u32 interruptOffset, u32 nrOfAgents, u32 startingFrom)
{
    intOffset = interruptOffset;
    usedAgentsNr = nrOfAgents;
    startFrom = startingFrom;

    if(cmxDmaStatus.status == DRV_CDMA_INITIALIZED)
    {
        CmxDmaInitTail(tail);
        //interrupt bits are mapped directly to used link agents, no need to shift 'startingFrom' positions
        u32 leonIntMask = (0xF >> (NR_OF_LINK_AGENTS - usedAgentsNr)) << intOffset;

        if(swcWhoAmI() == PROCESS_LEON_RT)
        {
        	u32 leonRTIntRedirectMask = (0xF >> (NR_OF_LINK_AGENTS - usedAgentsNr)) << intOffset;
        	SET_REG_WORD(CDMA_SET_IMASK_ADR, leonRTIntRedirectMask);
        }
        //Map each channel to a single Link Agent.
        SET_REG_WORD(CDMA_CHTOAGN_ADR, 0X3210);
        //Enable the interrupt for our Leon
        SET_REG_WORD(CDMA_SET_INTEN_ADR, leonIntMask);
        //Enable DMA
        SET_REG_WORD(CDMA_CTRL_ADR, DMA_ENABLE_BIT);

        return CMX_DMA_DRV_SUCCESS;
    }
    else return CMX_DMA_DRV_NOT_INITIALIZED;
}

#endif // USE_CMX_DMA_NEW_DRIVER

///@}

