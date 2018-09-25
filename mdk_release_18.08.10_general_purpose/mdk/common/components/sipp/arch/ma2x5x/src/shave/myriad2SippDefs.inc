//--------------------------------------------------------------
//automatically generated from PC app (sippDbgDumpAsmOffsets)
//--------------------------------------------------------------

.ifndef SIPP_SHAVE_ASM_DEFS
.set    SIPP_SHAVE_ASM_DEFS

//offsets within SippFilter struct
.set F_O_SVU_RUN         112 // fptr->funcSvuRun
.set F_O_PAR_INFO         76 // fptr->parInfo
.set F_O_LINES_I         188 // fptr->dbLinesIn[0]
.set F_O_LINES_O         744 // fptr->dbLineOut[0]
.set F_O_EXENO             0 // fptr->exeNo
.set F_O_H_PADDING       720 // fptr->hPadding
.set F_O_COMM_INFO       160 // fptr->gi
.set F_O_OUT_LPTR       1280 // fptr->outLinePtr
.set F_O_FIRST_O_SLC     148 // fptr->firstOutSlc
.set F_O_LN_TO_PAD      1000 // fptr->lnToPad
.set F_O_OUT_OFFS        736 // fptr->outOff
.set F_O_LINES_USED      128 // fptr->nLinesUsed
.set F_O_PIXEL_POS      1636 // fptr->ptrChunkPos
.set F_O_PL_STRIDE      1264 // fptr->planeStride
.set F_O_N_LINES        1468 // fptr->nLines
.set F_O_PARENTS          44 // fptr->parents
.set F_O_PAR_OBUF_IDX   1476 // fptr->parentOBufIdx
.set F_O_BYTESPP        1644 // fptr->bytesPerPix
.set F_O_LNS_PER_ITER     20 // fptr->linesPerIter
.set F_O_LNS_THIS_ITER   712 // fptr->linesThisIter
.set F_O_BPP             728 // fptr->bpp
.set F_O_SLICE_WIDTH     704 // fptr->sliceWidth
.set F_O_OUTPUT_WIDTH    700 // fptr->outputW
.set F_O_OUTPUT_HEIGHT    16 // fptr->outputH

//offsets within CommInfo struct
.set G_O_FIRST_S          0 // gi->sliceFirst
.set G_O_LAST_S           4 // gi->sliceLast
.set G_O_SLC_SZ           8 // gi->sliceSize
.set G_O_CUR_F           12 // gi->curFrame

//offsets within SippPipeline struct
.set P_O_SVU_CMD          68 // pl->svuCmd
.set P_O_SVU_SYNC_MTX     72 // pl->svuSyncMtx
.set P_O_SVU_SYNC_SEM     80 // pl->svuSyncSem
.set P_O_SVU_LIST        992 // pl->filtersSvu
.set P_O_N_PAD_FILTS      36 // pl->nPadFilters
.set P_O_N_SVU_FILTS    1120 // pl->nFiltersSvu
.set P_O_ITER           1656 // pl->iteration
.set P_O_SCHED_NFO      1628 // pl->schedInfo
.set P_O_GLOBAL_NFO        0 // pl->gi
.set P_O_PAD_LIST        220 // pl->padList

//offsets within SchedInfo struct
.set S_O_SVU_MASK         4 // schedInfo[0].shaveMask
.set SZ_SCH_INFO         24 // sizeof(pl->schedInfo[0])

.endif
