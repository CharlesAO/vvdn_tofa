#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/startBicubic/startBicubic.h>
#include <sippBicubic.h>

#define COMMAND_SIZEOF      0x40

static void afisFptrFields(SippFilter *fptr)
{
    printf("  .id = %u\n", fptr->id);
    printf("  .exeNo = %u\n", fptr->exeNo);
    printf("  .bpp = %u, .nPlanes = %u, .nLines = %u\n", /*fptr->log2BytesPerPix*/sippFilterGetOutputBpp(fptr,0), /*fptr->nPlanes*/sippFilterGetNumOutPlanes(fptr, 0), fptr->nLines);
    printf("  .outputW = %u, .hPadding = %u\n", fptr->outputW, fptr->hPadding);
    printf("  .outputH = %u\n", fptr->outputH);
    printf("  .outOff = (int*)%p | [0]=%u\n", fptr->outOff, fptr->outOff? *fptr->outOff: 0);
    printf("  .lineStride = %d\n", fptr->lineStride);
    printf("  .planeStride = %d\n", /*fptr->planeStride*/sippFilterGetPlaneStride(fptr, 0));
    printf("  .sliceWidth = %d, .sliceWidthLastSvu = %d\n", fptr->sliceWidth, fptr->sliceWidthLastSvu);
    printf("  .outputBuffer = %p\n", fptr->outputBuffer);
    printf("  .outLinePtr = %p\n", fptr->outLinePtr);

    printf("  .firstOutSlc = %u\n", fptr->firstOutSlc);
    printf("  .nCtxLoads = %u\n", fptr->nCtxLoads);
    printf("  .unit = %u\n", fptr->unit);
    printf("  .gi = (CommInfo*)%p\n", fptr->gi );

    printf("  .iBuf[] ={ (SippHwBuf*)%p, (SippHwBuf*)%p }\n", fptr->iBuf[0], fptr->iBuf[1] );
    printf("  .oBuf = (SippHwBuf*)%p\n", fptr->oBuf);
    printf("  .sch = (SippSched*)%p\n", fptr->sch);
    printf("  .lnToPad[] = { %p, %p } \n", fptr->lnToPad[0], fptr->lnToPad[1]);
    printf("  .nCons = %u\n", fptr->nCons);
    printf("  .schNo = %d\n", fptr->schNo);
    printf("  .outLinePtrPrev = %p\n", fptr->outLinePtrPrev);
    printf("  .flags = %d\n", fptr->flags);
    printf("  .nParents = %d\n", fptr->nParents);
}

void configureBicubicHWblock(StartBicubicParam *p, UInt64 *input )
{
    UInt32 firstShave = p->firstShave;
    UInt32 lastShave = p->lastShave;
    UInt32 shaveCount = p->shaveCount;
    unsigned int shaveId = scGetShaveNumber();

    UInt64 *xy_ptr = input;
    UInt32 width = p->width;
    UInt32 height = p->height;
    UInt8 *in_ptr = p->input;
    UInt8 *out_ptr = p->output;
    UInt32 bpp = p->bpp;
    UInt32 svuCrt = shaveId - firstShave;

    static uint8_t command_list[12*COMMAND_SIZEOF];
    fp16 range;

    tBicubicReg *pCmd = p->cmd + svuCrt;

    if(svuCrt > 0)
    {
        // link the current entry after the previous one
        // nasty hack - only works because command_list is static
        pCmd[-1].BIC_LINKED_LIST_ADDR = command_list + svuCrt*COMMAND_SIZEOF;
    }

    pCmd->BIC_LINKED_LIST_ADDR    = 0;
    pCmd->BIC_ID                  = 0x1a;
    pCmd->BIC_CTRL                = (1<<BICUBIC_RUN_SHIFT)+(0<<BICUBIC_BYPASS_SHIFT)+(0<<BICUBIC_CLAMP_SHIFT)+(BICUBIC_PIX_FORMAT_U8F<<BICUBIC_PIX_FORMAT_SHIFT)+(0<<BICUBIC_BAYER_ENABLE_SHIFT)+(0<<BICUBIC_BORDER_MODE_SHIFT);
    pCmd->BIC_LINE_LENGTH         = width / shaveCount;
    pCmd->BIC_XY_PIXEL_LIST_BASE  = (uint32_t)xy_ptr;
    pCmd->BIC_BORDER_PIXEL        = 0;
    range = 0.0f;
    pCmd->BIC_CLAMP_MIN           = range.getPackedValue();
    range = 1.0f;
    pCmd->BIC_CLAMP_MAX           = range.getPackedValue();
    pCmd->BIC_INPUT_BASE_ADDR     = (uint32_t)p->input;
    pCmd->BIC_INPUT_TOP_ADDR      = (uint32_t)p->input + width * bpp * height;
    pCmd->BIC_INPUT_CHUNK_WIDTH   = width;
    pCmd->BIC_INPUT_CHUNK_STRIDE  = width * bpp;
    pCmd->BIC_INPUT_LINE_WIDTH    = width;
    pCmd->BIC_INPUT_LINE_STRIDE   = width * bpp;
    pCmd->BIC_INPUT_HEIGHT        = height;
    pCmd->BIC_INPUT_OFFSET        = 0;
    pCmd->BIC_OUTPUT_BASE_ADDR    = (uint32_t)p->output;
    pCmd->BIC_OUTPUT_CHUNK_WIDTH  = width / shaveCount;
    pCmd->BIC_OUTPUT_CHUNK_STRIDE = width / shaveCount * bpp;

    // the last SHAVE builds BIC's task list
    if(shaveId == lastShave)
    {
        // "instantiate" hw and reg map
        BicubicFilt bic;
        tBicubicReg reg;
        bic.SetReg(&reg);
        // pack the commands into a single linked list
        // NOTE: the "next" pointers were previously hacked in
        // (knowing theur final destination addressess in command_list)
        int i;
        for(i = 0; i <= svuCrt; i++)
        {
            uint32_t crtCmdPtr = (uint32_t)( command_list + i * COMMAND_SIZEOF );
            bic.CommandPack(p->cmd + i, (uint8_t*)crtCmdPtr);
        }
        bic.SetUpAndRun(command_list);
    }

    p->output += width / shaveCount * bpp;
}



//#######################################################################################
void svuStartBicubic(SippFilter *fptr)
{
    UInt64* input = (UInt64*)getInPtr(fptr, 0, 0, 0);
    StartBicubicParam *param = (StartBicubicParam*)fptr->params;
#ifdef SIPP_USE_MVCV
#else
    configureBicubicHWblock(param, input );
#endif
}
