#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/extStatsSatPixelsU32/extStatsSatPixelsU32.h>
#include <string.h>
#include <stdlib.h>

void svuExtStatsSatPixelsU32(SippFilter *fptr)
{
    UInt16 *iline;
    ExtStatsSatPixelsU32Param* params = (ExtStatsSatPixelsU32Param*)fptr->params;
    iline=(UInt16*)getInPtr(fptr,0,0,0,0);

    Int32 iBaseAdr    = params->in.base;
    Int32 iLs         = params->in.ls;
    Int32 iBufSz      = params->in.cfg & 0x1FF;
    Int32 ibfl        = params->in.fillCtrl & 0x1FF;
    UInt8 *inLineContinue  = (UInt8*)(iBaseAdr + ibfl * iLs);
    UInt32 crtShave = scGetShaveNumber();
    Int32 crtSlice =  crtShave - fptr->gi->sliceFirst;
    UInt32 sliceLineSize = fptr->sliceWidth;
    if (crtShave == fptr->gi->sliceLast) sliceLineSize = fptr->sliceWidthLastSvu;
    // copy line to correct location
    memcpy(inLineContinue + (crtSlice * fptr->sliceWidth * 2), iline, sliceLineSize*2);

    if ( crtShave == fptr->gi->sliceLast) {
#if defined(SIPP_PC)
        (*params->statsSaturatePxl)((ExtStatsSatPixelsU32Param*)params);
#else  // THIS IS FOR M2V1 workaround for generate statistics on other shave, no bloking sipp
        #define OCR_STOP_GO    0x0004
        #define OSR_SWI_HALT   0x00008
        #define IRF_BASE       0x180       // Base address of IRF
        #define SVU_PTR        0x024       // Instruction Pointer Overwrite Register
        #define SVU_OCR        0x000       // Operation Control Register
        #define SVU_IRR        0x010       // Interrupt Clear Register
        #define SVU_OSR        0x004       // Operation Status Register
        #define SET_REG_WORD(a,x)   ((void)(*(volatile u32*)(((unsigned)(a))) = (u32)(x)))
        #define GET_REG_WORD_VAL(a) (*(volatile u32*)(((unsigned)(a))))
        UInt32 shvBaseAdr = 0x20f00000 + params->shaveNr*0x00010000 +  0x1000;
        volatile UInt32 currentShaveOcr = 0;
        volatile UInt32 currentShaveOsr = 0;

        currentShaveOcr = GET_REG_WORD_VAL(shvBaseAdr + SVU_OCR);
        currentShaveOsr = GET_REG_WORD_VAL(shvBaseAdr + SVU_OSR);
        while ((~currentShaveOcr & OCR_STOP_GO) && (~currentShaveOsr & OSR_SWI_HALT)) {
            currentShaveOcr = GET_REG_WORD_VAL(shvBaseAdr + SVU_OCR);
            currentShaveOsr = GET_REG_WORD_VAL(shvBaseAdr + SVU_OSR);
        }
        SET_REG_WORD(shvBaseAdr + IRF_BASE + (19<<2), (u32)params->stackPointer);
        SET_REG_WORD(shvBaseAdr + IRF_BASE + (18<<2), (u32)params);
        //SET_REG_WORD(shvBaseAdr + IRF_BASE + (17<<2), (u32)params->statsSaturatePxl);
        SET_REG_WORD(shvBaseAdr + IRF_BASE + (17<<2), (u32)params->statsSaturatePxl);
        SET_REG_WORD(shvBaseAdr + IRF_BASE + (16<<2), (u32)params->statsSaturatePxl);
        SET_REG_WORD(shvBaseAdr + SVU_PTR           , (u32)params->statsSaturatePxl);
        SET_REG_WORD(shvBaseAdr + SVU_OCR           , 0);
        SET_REG_WORD(shvBaseAdr + SVU_IRR           , 0xFFFFFFFF);
#endif
    }
}
