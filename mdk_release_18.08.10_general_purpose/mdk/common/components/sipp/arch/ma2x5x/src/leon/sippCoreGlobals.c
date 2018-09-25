///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

//####################################################################################
#if defined(MYRIAD2) || defined(SIPP_PC)
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)

SippGlobals gSipp =
{
  {
     /*SIPP_SIGMA_ID    */ { sippInitSigma   , sippLoadSigma   ,  SZ(SigmaParam)   , sippSetOBufLevelsSigma,      I_FC(SIPP_SIGMA_ID   ), sippGetIBufCtxSigma,    sippGetIBufIdsSigma,     sippGetOBufIdsSigma,     sippCheckOPipeConnectionSigma     , sippSetBufLatenciesSigma,      0 },
     /*SIPP_LSC_ID      */ { sippInitLsc     , sippLoadLsc     ,  SZ(LscParam)     , sippSetOBufLevelsLsc,        I_FC(SIPP_LSC_ID     ), sippGetIBufCtxLsc,      sippGetIBufIdsLsc,       sippGetOBufIdsLsc,       sippCheckOPipeConnectionLsc       , sippSetBufLatenciesLsc,        0 },
     /*SIPP_RAW_ID      */ { sippInitRaw     , sippLoadRaw     ,  SZ(RawParam)     , sippSetOBufLevelsRaw,        I_FC(SIPP_RAW_ID     ), sippGetIBufCtxRaw,      sippGetIBufIdsRaw,       sippGetOBufIdsRaw,       sippCheckOPipeConnectionRaw       , sippSetBufLatenciesRaw,        0 },
     /*SIPP_DBYR_ID     */ { sippInitDbyr    , sippLoadDbyr    ,  SZ(DbyrParam)    , sippSetOBufLevelsDbyr,       I_FC(SIPP_DBYR_ID    ), sippGetIBufCtxDbyr,     sippGetIBufIdsDbyr,      sippGetOBufIdsDbyr,      sippCheckOPipeConnectionDbyr      , sippSetBufLatenciesDbyr,       0 },
     /*SIPP_DOGL_ID     */ { sippInitDogLtm  , sippLoadDogLtm  ,  SZ(DogLtmParam)  , sippSetOBufLevelsDoGLTM,     I_FC(SIPP_DOGL_ID    ), sippGetIBufCtxDoGLTM,   sippGetIBufIdsDoGLTM,    sippGetOBufIdsDoGLTM,    sippCheckOPipeConnectionDoGLTM    , sippSetBufLatenciesDoGLTM,     sippSetupUpdateDoGLTM },
     /*SIPP_LUMA_ID     */ { sippInitLumaDns , sippLoadLumaDns ,  SZ(YDnsParam)    , sippSetOBufLevelsLuma,       I_FC(SIPP_LUMA_ID    ), sippGetIBufCtxLumDns,   sippGetIBufIdsLumDns,    sippGetOBufIdsLumDns,    sippCheckOPipeConnectionLuma      , sippSetBufLatenciesLuma,       0 },
     /*SIPP_SHARPEN_ID  */ { sippInitSharpen , sippLoadSharpen ,  SZ(UsmParam)     , sippSetOBufLevelsSharpen,    I_FC(SIPP_SHARPEN_ID ), sippGetIBufCtxSharpen,  sippGetIBufIdsSharpen,   sippGetOBufIdsSharpen,   sippCheckOPipeConnectionSharpen   , sippSetBufLatenciesSharpen,    0 },
     /*SIPP_CGEN_ID     */ { sippInitGenChr  , sippLoadGenChr  ,  SZ(GenChrParam)  , sippSetOBufLevelsGenChroma,  I_FC(SIPP_CGEN_ID    ), sippGetIBufCtxGenChroma,sippGetIBufIdsGenChroma, sippGetOBufIdsGenChroma, sippCheckOPipeConnectionGenChroma , sippSetBufLatenciesGenChroma,  0 },
     /*SIPP_MED_ID      */ { sippInitMed     , sippLoadMed     ,  SZ(MedParam)     , sippSetOBufLevelsMedian,     I_FC(SIPP_MED_ID     ), sippGetIBufCtxMed,      sippGetIBufIdsMed,       sippGetOBufIdsMed,       sippCheckOPipeConnectionMedian    , sippSetBufLatenciesMedian,     sippSetupUpdateMedian},
     /*SIPP_CHROMA_ID   */ { sippInitChrDns  , sippLoadChrDns  ,  SZ(ChrDnsParam)  , sippSetOBufLevelsChrDns,     I_FC(SIPP_CHROMA_ID  ), sippGetIBufCtxChrDns,   sippGetIBufIdsChrDns,    sippGetOBufIdsChrDns,    sippCheckOPipeConnectionChrDns    , sippSetBufLatenciesChrDns,     0 },
     /*SIPP_CC_ID       */ { sippInitColComb , sippLoadColComb ,  SZ(ColCombParam) , sippSetOBufLevelsColourComb, I_FC(SIPP_CC_ID      ), sippGetIBufCtxColComb,  sippGetIBufIdsColComb,   sippGetOBufIdsColComb,   sippCheckOPipeConnectionColourComb, sippSetBufLatenciesColourComb, 0 }, //WARNING: has another input as well !!!
     /*SIPP_LUT_ID      */ { sippInitLut     , sippLoadLut     ,  SZ(LutParam)     , sippSetOBufLevelsLut,        I_FC(SIPP_LUT_ID     ), sippGetIBufCtxLut,      sippGetIBufIdsLut,       sippGetOBufIdsLut,       sippCheckOPipeConnectionLut       , sippSetBufLatenciesLut,        0 },
     /*SIPP_EDGE_OP_ID  */ { sippInitEdgeOp  , sippLoadEdgeOp  ,  SZ(EdgeParam)    , sippSetOBufLevelsEdgeOp,     I_FC(SIPP_EDGE_OP_ID ), sippGetIBufCtxEdgeOp,   sippGetIBufIdsEdgeOp,    sippGetOBufIdsEdgeOp,    sippCheckOPipeConnectionDefault   , sippSetBufLatenciesEdgeOp,     0 },
     /*SIPP_CONV_ID     */ { sippInitConv    , sippLoadConv    ,  SZ(ConvParam)    , sippSetOBufLevelsConv,       I_FC(SIPP_CONV_ID    ), sippGetIBufCtxConv,     sippGetIBufIdsConv,      sippGetOBufIdsConv,      sippCheckOPipeConnectionDefault   , sippSetBufLatenciesConv,       0 },
     /*SIPP_HARRIS_ID   */ { sippInitHarris  , sippLoadHarris  ,  SZ(HarrisParam)  , sippSetOBufLevelsHarris,     I_FC(SIPP_HARRIS_ID  ), sippGetIBufCtxHarris,   sippGetIBufIdsHarris,    sippGetOBufIdsHarris,    sippCheckOPipeConnectionDefault   , sippSetBufLatenciesHarris,     0 },
     /*SIPP_UPFIRDN0_ID */ { sippInitPolyFir , sippLoadPolyFir0,  SZ(PolyFirParam) , sippSetOBufLevelsPoly0,      I_FC(SIPP_UPFIRDN0_ID), sippGetIBufCtxPolyFir0, sippGetIBufIdsPolyFir0,  sippGetOBufIdsPolyFir0,  sippCheckOPipeConnectionPoly      , sippSetBufLatenciesPoly,       0 },
     /*SIPP_UPFIRDN1_ID */ { sippInitPolyFir , sippLoadPolyFir1,  SZ(PolyFirParam) , sippSetOBufLevelsPoly1,      I_FC(SIPP_UPFIRDN1_ID), sippGetIBufCtxPolyFir1, sippGetIBufIdsPolyFir1,  sippGetOBufIdsPolyFir1,  sippCheckOPipeConnectionPoly      , sippSetBufLatenciesPoly,       0 },
     /*SIPP_UPFIRDN2_ID */ { sippInitPolyFir , sippLoadPolyFir2,  SZ(PolyFirParam) , sippSetOBufLevelsPoly2,      I_FC(SIPP_UPFIRDN2_ID), sippGetIBufCtxPolyFir2, sippGetIBufIdsPolyFir2,  sippGetOBufIdsPolyFir2,  sippCheckOPipeConnectionPoly      , sippSetBufLatenciesPoly,       0 },
     /*SIPP_MIPI_TX0_ID */ { sippInitMipiTx  , sippLoadMipiTx  ,  SZ(MipiTxParam)  , sippSetOBufLevelsMipiTx0,    0                     , 0                     , 0                      , sippGetOBufIdsMipiTx0,   0                                 , sippSetBufLatenciesMipiTx,     0 },
     /*SIPP_MIPI_TX1_ID */ { sippInitMipiTx  , sippLoadMipiTx  ,  SZ(MipiTxParam)  , sippSetOBufLevelsMipiTx1,    0                     , 0                     , 0                      , sippGetOBufIdsMipiTx1,   0                                 , sippSetBufLatenciesMipiTx,     0 },
     /*SIPP_MIPI_RX0_ID */ { sippInitMipiRx  , sippLoadMipiRx  ,  SZ(MipiRxParam)  , sippSetOBufLevelsMipiRx0,    0                     , 0                     , 0                      , sippGetOBufIdsMipiRx0,   0                                 , sippSetBufLatenciesMipiRx,     0 },
     /*SIPP_MIPI_RX1_ID */ { sippInitMipiRx  , sippLoadMipiRx  ,  SZ(MipiRxParam)  , sippSetOBufLevelsMipiRx1,    0                     , 0                     , 0                      , sippGetOBufIdsMipiRx1,   0                                 , sippSetBufLatenciesMipiRx,     0 },
     /*SIPP_MIPI_RX2_ID */ { sippInitMipiRx  , sippLoadMipiRx  ,  SZ(MipiRxParam)  , sippSetOBufLevelsMipiRx2,    0                     , 0                     , 0                      , sippGetOBufIdsMipiRx2,   0                                 , sippSetBufLatenciesMipiRx,     0 },
     /*SIPP_MIPI_RX3_ID */ { sippInitMipiRx  , sippLoadMipiRx  ,  SZ(MipiRxParam)  , sippSetOBufLevelsMipiRx3,    0                     , 0                     , 0                      , sippGetOBufIdsMipiRx3,   0                                 , sippSetBufLatenciesMipiRx,     0 },
     /*SIPP_AF_STATS_ID */ { 0               ,              0  ,  0                , 0                          , 0                     , 0                     , 0                      , 0                      , 0                                 , 0                            , 0 },
     /*SIPP_LUMA_HIST_ID*/ { 0               ,              0  ,  0                , 0                          , 0                     , 0                     , 0                      , 0                      , 0                                 , 0                            , 0 },
     /*SIPP_RGB_HIST_ID */ { 0               ,              0  ,  0                , 0                          , 0                     , 0                     , 0                      , 0                      , 0                                 , 0                            , 0 },
     /*SIPP_DMA_ID      */ { sippInitDma     ,              0  ,  SZ(DmaParam)     , 0                          , 0                     , 0                     , 0                      , 0                      , 0                                 , 0                            , 0 },
     /*SIPP_SVU_ID      */ { 0               ,              0  ,  0                , 0                          , 0                     , 0                     , 0                      , 0                      , 0                                 , 0                            , 0 },
  },

    //Note: Remaining data members will be initialized dynamically...
};

#endif
#endif
