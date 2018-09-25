#include <sipp.h>
#include <sippInternal.h>

u32 sippFilterGetNumOutPlanes (SippFilter * fptr, u32 outBufferIdx)
{
    return fptr->nPlanes[outBufferIdx];
}

u32 sippFilterGetOutputBpp (SippFilter * fptr)
{
    return fptr->bytesPerPix;
}

u32 sippFilterGetParentInputLines (SippFilter * fptr, u32 parentIdx)
{
    u32 parentOBuf = fptr->parentOBufIdx[parentIdx];
    return (u32)fptr->parents[parentIdx]->nLines[parentOBuf];
}

u32 sippFilterGetParentPlaneStride (SippFilter * fptr, u32 parentIdx)
{
    u32 parentOBuf = fptr->parentOBufIdx[parentIdx];
    return (u32) fptr->parents[parentIdx]->planeStride[parentOBuf];
}

u32 sippFilterGetPlaneStride (SippFilter * fptr, u32 outBufferIdx)
{
    return fptr->planeStride[outBufferIdx];
}

u32 sippFilterGetLinesThisIter (SippFilter * fptr)
{
    return fptr->linesThisIter[fptr->exeNo & 0x1];
}

u32 sippFilterGetLinesPerIter (SippFilter * fptr)
{
    return fptr->linesPerIter;
}

u32 sippFilterGetParentSliceWidth (SippFilter * fptr, u32 parentIdx)
{
    return fptr->parents[parentIdx]->sliceWidth;
}

u32 sippFilterGetParentOutputWidth (SippFilter * fptr, u32 parentIdx)
{
    return fptr->parents[parentIdx]->outputW;
}

u32 sippFilterGetParentOutputHeight (SippFilter * fptr, u32 parentIdx)
{
    return fptr->parents[parentIdx]->outputH;
}
