///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

//These get allocated PER SHAVE
extern SippPipeline *sipp_pl;     //The global Pipeline structure shared by Shaves and Leon.

#if defined(__MOVICOMPILE__)
#include <svuCommonShave.h> //scGetShaveNumber(); asm inline version
#elif defined(SIPP_PC)
#endif

#ifdef __MOVICOMPILE__
#else //PC land
extern u32 dbg_svu_no;
extern int scGetShaveNumber();
#endif

//########################################################################################
//Padding operates on previous generated line !
// - the last line of a buffer gets padded when the bottom replication starts
// - this is when last line of that buffer will get used due to the DELAY required by HW
//Shave K will pad segment K; due to shifted starts, segment K might be in different slice
//########################################################################################

//==========================================================================
// same functionality as getInPtr, just that works on output buffer, and ADDR
// should consider firstOutSlc of current buffer!

// We may have to expand this to take in an output buffer number IF
// and of the SW filters would like to have more than one output buffer in the future

u32 getOutPtr (SippFilter * fptr,
               u32          iterNo,
               u32          planeNo)
{
    u32 curSvu = scGetShaveNumber();

    // Initial proposal, as HW WinRegs would:
    u32 runNo = fptr->exeNo&1;
    s32 addr  = (s32)fptr->dbLineOut[0][runNo][iterNo];

    // Add "magic" precomputed constant
    addr += fptr->outOff[0][curSvu * fptr->nPlanes[0] + planeNo];

    return (u32)addr;
}

//#################################################################
//For a given filter "fptr", return the absolute line address(lineNo) from parent filter
//indicated by index "parent"

u32 getInPtr (SippFilter * fptr,
              u32          parNo,
              u32          iterNo,
              u32          lineNo,
              u32          planeNo)
{
    u32          curSvu     = scGetShaveNumber();
    SippFilter * parent     = fptr->parents[parNo];
    s32          parObufIdx = fptr->parentOBufIdx[parNo];

    // Initial proposal, as HW WinRegs would:
    u32 runNo = fptr->exeNo&1;
    s32 addr  = fptr->dbLinesIn[parNo][runNo][iterNo][lineNo];

    // Add "magic" precomputed constant
    addr += parent->outOff[parObufIdx][curSvu * parent->nPlanes[parObufIdx] + planeNo];

    return (u32)addr;
}


//#################################################################
//Compute 3 input plane pointers for a given line
//This is useful for accessing RGB/Chroma data
void getIn3PlanePtr (SippFilter * fptr,
                     u32          parNo,
                     u32          iterNo,
                     u32          lineNo,
                     void *       out3Ptr)
{
    u32          curSvu     = scGetShaveNumber();
    SippFilter * parent     = fptr->parents[parNo];
    s32          parObufIdx = fptr->parentOBufIdx[parNo];

    // Initial proposal, as HW WinRegs would:
    u32 runNo = fptr->exeNo&1;
    s32 addr  = fptr->dbLinesIn[parNo][runNo][iterNo][lineNo];

    // Add "magic" precomputed constants
    ((u32*)out3Ptr)[0] = addr + parent->outOff[parObufIdx][curSvu * parent->nPlanes[parObufIdx] + 0];
    ((u32*)out3Ptr)[1] = addr + parent->outOff[parObufIdx][curSvu * parent->nPlanes[parObufIdx] + 1];
    ((u32*)out3Ptr)[2] = addr + parent->outOff[parObufIdx][curSvu * parent->nPlanes[parObufIdx] + 2];
}

//#################################################################
//For a given filter "fptr", return the absolute line address(lineNo) from parent filter
//indicated by index "parent"
u32 getInPtrAbs (SippFilter * fptr,
                 u32          parNo,
                 u32          iterNo,
                 u32          lineNo,
                 u32          planeNo,
                 u32          targetSlice)
{
    u32          curSvu     = targetSlice;
    SippFilter * parent     = fptr->parents[parNo];
    s32          parObufIdx = fptr->parentOBufIdx[parNo];

    //Initial proposal, as HW WinRegs would:
    u32 runNo = fptr->exeNo&1;
    s32 addr  = fptr->dbLinesIn[parNo][runNo][iterNo][lineNo];

    //Add "magic" precomputed constant
    addr += parent->outOff[parObufIdx][curSvu * parent->nPlanes[parObufIdx] + planeNo];

    return (u32)addr;
}

//########################################################################################
//For a given filter "fptr", return the absolute line address(lineNo) from parent filter
//indicated by index "parent"

u32 getPlaneIoPtrs (SippFilter * fptr,
                    u32          parNo,
                    u32          iterNo,
                    u32          planeNo,
                    void *       inPtrs)
{
    u32          curSvu     = scGetShaveNumber();
    SippFilter * parent     = fptr->parents[parNo];
    u32          nInLines   = fptr->nLinesUsed[parNo];
    u32          runNo      = fptr->exeNo & 1;
    s32          parObufIdx = fptr->parentOBufIdx[parNo];
    u32          i, inCt, outAddr;

    // Plane-magic-offset for current shave (is to be added to all
    // input pointers !)
    inCt = parent->outOff[parObufIdx][curSvu * parent->nPlanes[parObufIdx] + planeNo];

    // Input lines:
    for (i = 0; i < nInLines; i++)
    {
        ((u32*)inPtrs)[i]  = fptr->dbLinesIn[parNo][runNo][iterNo][i] + inCt;
    }

    // Output line:
    outAddr  = (s32)fptr->dbLineOut[0][runNo][iterNo];
    outAddr += fptr->outOff[0][curSvu * fptr->nPlanes[0] + planeNo];

    return outAddr;
}
