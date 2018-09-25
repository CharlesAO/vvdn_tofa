#include <sipp.h>
#include <sippInternal.h>

// u32 controlRegVal;

//####################################################################################
// If a HW SIPP exec unit needs to run a filter, load first filter
void sippCoreUnitLoad (SippPipeline * pPipe, u32 uID)
{
    s32        *  firstIdx = pPipe->hwSippFirst;
    SippFilter ** filters  = pPipe->filters;
    SippFilter *  first    = filters[firstIdx[uID]];

    gSipp.uInfo[uID].hwFnLoad(first); //sippLoadXXXXX

    #ifdef SIPP_SCRT_ENABLE_OPIPE
    if (pPipe->pfnSippRuntime != sippOPipeRuntime)
    #endif
    {
        // 0)Initially, iBFL and oBFL are zero, so filter is stalled due to no input data
        // 1)Next block filter due to full-output buffer
        gSipp.uInfo[uID].hwFnSetOBufs(first, eSIPP_SET_OBUF_SPACE_FULL);
    }
}

//####################################################################################
// At setup, program the HW SIPP filters with first filter in the filter list
// associated to each unit.
// In the case of single-filter/HW_unit, this is the gross setup.
// In multi-filter/HW_unit case, it's potentially useless, but no harm is done)

void sippCoreHwInitialLoad (SippPipeline * pPipe)
{
    u32 unitIdx;
    u32 regVal, controlRegVal;

    // Set oPipeCfg created from all the constituent OSEs' OSEFiltCfgs
    regVal = GET_REG_WORD_VAL(SIPP_OPIPE_CFG_ADR);
    SET_REG_WORD(SIPP_OPIPE_CFG_ADR, (regVal | pPipe->oPipeCfg));

    controlRegVal  = GET_REG_WORD_VAL(SIPP_CONTROL_ADR);

    for (unitIdx = 0; unitIdx < EXE_NUM; unitIdx++)
    {
        if (pPipe->hwSippFltCnt[unitIdx] )
        {
            sippCoreUnitLoad (pPipe, unitIdx);
            controlRegVal |= (0x1 << unitIdx);
        }
    }

    SET_REG_WORD(SIPP_CONTROL_ADR, controlRegVal);

}

//####################################################################################
// At setup, program the HW SIPP filters with first filter in the filter list
// associated to each unit.
//    what this function needs to do now is look at the new pipeline and for all
//    filters which feature in it, decide if we need to save its ocntext and also
//    identify the pipeline to which it is assocaited in order to achieve that save

void sippCoreHwInitialSave (SippPipeline *pPipe)
{
    UNUSED (pPipe); //hush the compiler warning
}
