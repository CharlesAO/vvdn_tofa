///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

#if (defined(SIPP_PC) || defined(MYRIAD2))

//#################################################################################
// Used because memcpy can not be used on PC model for set registers

static INLINE void memcpySetU32Registers(u32* dst, u32* src, u32 length)
{
    u32 dstAdr  = (u32)dst;
    u32 *srcAdr = (u32*)src;
    u32 x  = 0;
    for (x = 0; x < length; x++)
    {
        SET_REG_WORD(dstAdr, *srcAdr);
        dstAdr +=4;
        srcAdr++;
    }
}

//#######################################################################################
//This does NOT touch HW so can safely be called from a pipeline session not active on HW

u32 sippInitPolyFir (SippFilter *fptr)
{
    PolyFirParam * param = (PolyFirParam*)fptr->params;
    u32            retVal = 0;

    retVal  = sippIbufSetup (fptr, 0);
    retVal += sippObufSetup (fptr, 0);

    // Fix up input buffer cfg depending on plane mode
    if (param->planeMode == POLY_PLANE_Y)
    {
        // Force num planes to 1
        fptr->iBuf[0]->cfg &= (~(0xF << SIPP_NP_OFFSET));
    }
    else if (param->planeMode == POLY_PLANE_U)
    {
        // Fix the buffer base address to point to 2nd plane
        fptr->iBuf[0]->base += fptr->iBuf[0]->ps;
        // Force num planes to 1
        fptr->iBuf[0]->cfg  &= (~(0xF << SIPP_NP_OFFSET));
    }
    else if (param->planeMode == POLY_PLANE_V)
    {
        // Fix the buffer base address to point to 3nd plane
        fptr->iBuf[0]->base += (fptr->iBuf[0]->ps << 0x1);
        // Force num planes to 1
        fptr->iBuf[0]->cfg &= (~(0xF << SIPP_NP_OFFSET));
    }
    else if (param->planeMode == POLY_PLANE_UV)
    {
        // Fix the buffer base address to point to 2nd plane
        fptr->iBuf[0]->base += fptr->iBuf[0]->ps;
        // Force num planes to 2
        fptr->iBuf[0]->cfg &= (~(0xF << SIPP_NP_OFFSET));
        fptr->iBuf[0]->cfg |= (0x1 << SIPP_NP_OFFSET);
    }

    // Pack Input/Output resolutions
    param->frmDimPar = (fptr->parents[0]->outputH << SIPP_IMGDIM_SIZE)|fptr->parents[0]->outputW;
    param->frmDimFlt = (fptr->outputH             << SIPP_IMGDIM_SIZE)|fptr->outputW;

    //Pack some internals
    param->kerSz  = fptr->nLinesUsed[0];
    param->cfgReg = ((param->kerSz & 0x07) <<  0) |
                    ((param->clamp & 0x01) <<  3) |
                    ((param->horzD & 0x3F) <<  4) |
                    ((param->horzN & 0x1F) << 10) |
                    ((param->vertD & 0x3F) << 16) |
                    ((param->vertN & 0x1F) << 22) ;

    return retVal;
}

//#################################################################################
// Clearly this is a HW session only function

void sippLoadPolyFir (SippFilter *fptr, u32 unitID)
{
    SippHwBuf    *targetI;
    SippHwBuf    *targetO;
    u32           paramSet;
    SippHwBuf    *srcI  = fptr->iBuf[0];
    SippHwBuf    *srcO  = fptr->oBuf[0];
    PolyFirParam *param = (PolyFirParam*)fptr->params;

    if (unitID == SIPP_UPFIRDN0_ID)
    {
        targetI  = (SippHwBuf*)I_BASE(SIPP_UPFIRDN0_ID);
        targetO  = (SippHwBuf*)O_BASE(SIPP_UPFIRDN0_ID);
        paramSet = SIPP_UPFIRDN_BASE_ADR;
    }
    else
    {
    	if (unitID == SIPP_UPFIRDN1_ID)
    	{
            targetI  = (SippHwBuf*)I_BASE(SIPP_UPFIRDN1_ID);
            targetO  = (SippHwBuf*)O_BASE(SIPP_UPFIRDN1_ID);
    	}
    	else
    	{
    		targetI  = (SippHwBuf*)I_BASE(SIPP_UPFIRDN2_ID);
    		targetO  = (SippHwBuf*)O_BASE(SIPP_UPFIRDN2_ID);
    	}
        // Units 1 & 2 share a register set
        paramSet = SIPP_UPFIRDN12_BASE_ADR;
    }

    // Program I & O buffers
    if (srcI)
    {
        PROG_IO_BUFF(targetI, srcI);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SippFilter * par     = fptr->parents[0];
        s32          oBufIdx = fptr->parentOBufIdx[0];

        SET_REG_WORD((u32)&targetI->cfg, ((par->nPlanes[oBufIdx]-1) << SIPP_NP_OFFSET));
    }

    if (srcO)
    {
        PROG_IO_BUFF(targetO, srcO);
    }
    else
    {
        // Still need to set up planes for an oPipe setup
        SET_REG_WORD((u32)&targetO->cfg, ((fptr->nPlanes[0]-1) << SIPP_NP_OFFSET));
    }

    // PolyFIR specific params
    SET_REG_WORD(paramSet + 0x00,     param->frmDimPar);     //SIPP_UPFIRDN0_FRM_IN_DIM_ADR
    SET_REG_WORD(paramSet + 0x04,     param->frmDimFlt);     //SIPP_UPFIRDN0_FRM_OUT_DIM_ADR
    SET_REG_WORD(paramSet + 0x08,     param->cfgReg);        //SIPP_UPFIRDN0_CFG_ADR

    //Set coefficients (the number of coefs is variable !)
    memcpySetU32Registers((u32*)(paramSet + 0x0C), //SIPP_UPFIRDNX_VP0_3210_ADR
                          (u32*)param->vertCoefs,
                          (param->vertN * 8 * 1)>>2);

    memcpySetU32Registers((u32*)(paramSet + 0x8C), //SIPP_UPFIRDNX_HP0_3210_ADR
                          (u32*)param->horzCoefs,
                          (param->horzN * 8 * 1)>>2);

    #ifdef SIPP_PC
    // (In model only) enabled bit is only set when buffer initialisation is
    // complete, poll this register until then.
    while (GET_REG_WORD_VAL(SIPP_CONTROL_ADR) & (1 << unitID));
    #endif

}

u32 sippGetIBufCtxPolyFir0 (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX15_ADR;
}

u32 sippGetIBufCtxPolyFir1 (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX16_ADR;
}

u32 sippGetIBufCtxPolyFir2 (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (iBufIdx); //hush the compiler warning
    UNUSED (fptr);    //hush the compiler warning

    return SIPP_ICTX17_ADR;
}

void sippLoadPolyFir0 (SippFilter *fptr)
{
    sippLoadPolyFir (fptr, SIPP_UPFIRDN0_ID);
}
void sippLoadPolyFir1 (SippFilter *fptr)
{
    sippLoadPolyFir (fptr, SIPP_UPFIRDN1_ID);
}
void sippLoadPolyFir2 (SippFilter *fptr)
{
    sippLoadPolyFir (fptr, SIPP_UPFIRDN2_ID);
}

u32 sippGetIBufIdsPolyFir0 (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a polyphase scaler 0 filter input buffer increment
    return (0x1 << SIPP_UPFIRDN0_ID);
}

u32 sippGetIBufIdsPolyFir1 (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a polyphase scaler 1 filter input buffer increment
    return (0x1 << SIPP_UPFIRDN1_ID);
}

u32 sippGetIBufIdsPolyFir2 (SippFilter *fptr, u32 iBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (iBufIdx); //hush the compiler warning

    // Set the bit pos of a polyphase scaler 2 filter input buffer increment
    return (0x1 << SIPP_UPFIRDN2_ID);
}

u32 sippGetOBufIdsPolyFir0 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a polyphase scaler 0 filter output buffer increment
    return (0x1 << SIPP_UPFIRDN0_ID);
}

u32 sippGetOBufIdsPolyFir1 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a polyphase scaler 1 filter output buffer increment
    return (0x1 << SIPP_UPFIRDN1_ID);
}

u32 sippGetOBufIdsPolyFir2 (SippFilter *fptr, u32 oBufIdx)
{
    UNUSED (fptr);    //hush the compiler warning
    UNUSED (oBufIdx); //hush the compiler warning

    // Set the bit pos of a polyphase scaler 2 filter output buffer increment
    return (0x1 << SIPP_UPFIRDN2_ID);
}

void sippSetBufLatenciesPoly (SippFilter *fptr)
{
    // Fill in the latencies of each input and output buffer
    if (fptr->iBufCtx[0]) fptr->iBufCtx[0]->consumptionLatency = 0;
    fptr->OBufProdLatency[0]                                   = 0;
}

void sippSetOBufLevelsPoly0 (SippFilter *fptr, eSippObufControl eOBufSetting)
{
    u32 level = 0;
    u32 numLinesIter = fptr->linesPerIter;

    if (fptr->nLines[0])
    {
        switch (eOBufSetting)
        {
            case eSIPP_SET_OBUF_SPACE_FULL  : level = fptr->nLines[0]; break;
            case eSIPP_SET_OBUF_SPACE_EMPTY : level = 0; break;
            case eSIPP_SET_OBUF_SPACE_ITER  : level = fptr->nLines[0] - numLinesIter; break;
        }

        SET_REG_WORD(O_FC(SIPP_UPFIRDN0_ID), (1<<31) | level);
    }
}

void sippSetOBufLevelsPoly1 (SippFilter *fptr, eSippObufControl eOBufSetting)
{
    u32 level = 0;
    u32 numLinesIter = fptr->linesPerIter;

    if (fptr->nLines[0])
    {
        switch (eOBufSetting)
        {
            case eSIPP_SET_OBUF_SPACE_FULL  : level = fptr->nLines[0]; break;
            case eSIPP_SET_OBUF_SPACE_EMPTY : level = 0; break;
            case eSIPP_SET_OBUF_SPACE_ITER  : level = fptr->nLines[0] - numLinesIter; break;
        }

        SET_REG_WORD(O_FC(SIPP_UPFIRDN1_ID), (1<<31) | level);
    }
}

void sippSetOBufLevelsPoly2 (SippFilter *fptr, eSippObufControl eOBufSetting)
{
    u32 level = 0;
    u32 numLinesIter = fptr->linesPerIter;

    if (fptr->nLines[0])
    {
        switch (eOBufSetting)
        {
            case eSIPP_SET_OBUF_SPACE_FULL  : level = fptr->nLines[0]; break;
            case eSIPP_SET_OBUF_SPACE_EMPTY : level = 0; break;
            case eSIPP_SET_OBUF_SPACE_ITER  : level = fptr->nLines[0] - numLinesIter; break;
        }

        SET_REG_WORD(O_FC(SIPP_UPFIRDN2_ID), (1<<31) | level);
    }
}


#endif
