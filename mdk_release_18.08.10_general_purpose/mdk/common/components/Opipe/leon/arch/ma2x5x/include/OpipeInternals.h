///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - internals
///

#ifndef __O_PIPE_INTERNALS_H__
#define __O_PIPE_INTERNALS_H__


#ifdef __cplusplus
extern "C" {
#endif

 #include <registersMyriad.h>     //various registers used under the hood by some functions
 #include <swcLeonUtilsDefines.h> //for NOP
 #include <DrvRegUtils.h>
 #include <UnitTestApi.h>         //debug in VCS
 #include <VcsHooksApi.h>

//#####################################################################################
//OPIPE error codes
#define OPIPE_SUCCESS              0
#define OPIPE_ERR_OUT_OF_MEM       1 //memory pool size exceeded, increase OPIPE_MPOOL_SZ !
#define OPIPE_ERR_TOO_MANY_SRC     2 //MAX_SOURCES exceeded
#define OPIPE_ERR_TOO_MANY_SNK     3 //MAX_SINKS   exceeded
#define OPIPE_ERR_MISSING_CFG      4 //null cfg ptr for filter that is enabled (thus needs valid cfg)
#define OPIPE_ERR_UNALIGNED_CBUF   5 //circular buffer unaligned, please align buffer to 8Byte boundary
#define OPIPE_ERR_UNALIGNED_FBUF   6 //look for LSC gain map, RAW stats buffers etc...
#define OPIPE_ERR_UNALIGNED_GM_S   7 //unaligned LSC GainMap stride, must 8B multiple

//#####################################################################################
//Magic numbers
#define OP_UNINITIALIZED 0xDEADBEEF

//#####################################################################################
//Nice macros to access addresses based on HW resources IDs
#define CONCAT3(A, B, C) A ## B ## C

#define I_BASE(X) CONCAT3(SIPP_IBUF, X, _BASE_ADR)
#define I_CFG(X)  CONCAT3(SIPP_IBUF, X, _CFG_ADR)
#define I_LS(X)   CONCAT3(SIPP_IBUF, X, _LS_ADR)
#define I_PS(X)   CONCAT3(SIPP_IBUF, X, _PS_ADR)
#define I_FC(X)   CONCAT3(SIPP_IBUF, X, _FC_ADR)
#define I_IR(X)   CONCAT3(SIPP_IBUF, X, _IR_ADR)
#define O_BASE(X) CONCAT3(SIPP_OBUF, X, _BASE_ADR)
#define O_CFG(X)  CONCAT3(SIPP_OBUF, X, _CFG_ADR)
#define O_LS(X)   CONCAT3(SIPP_OBUF, X, _LS_ADR)
#define O_FC(X)   CONCAT3(SIPP_OBUF, X, _FC_ADR)
#define O_IR(X)   CONCAT3(SIPP_OBUF, X, _IR_ADR)

#define I_CTX(X)  CONCAT3(SIPP_ICTX, X, _ADR)
#define O_CTX(X)  CONCAT3(SIPP_OCTX, X, _ADR)

#if defined(OPIPE_RUNTIME_CHECKS)
#define COND(IX,OX) IX,OX
#else
#define COND(IX,OX)
#endif

//#####################################################################################
struct OpipeS;
typedef struct OpipeS Opipe;

typedef struct {
   void     (*cfgFn)(Opipe *p); //config routine
   uint32_t iBase;
   uint32_t oBase;
   uint32_t iFc;
   uint32_t iIR;
   uint32_t oIR;
 #if defined(OPIPE_RUNTIME_CHECKS)
   uint32_t iCtx;
   uint32_t oCtx;
 #endif
 }SippInfo; // __attribute__((aligned(256)));

extern const SippInfo gSippNfo[];

void     opipeError(uint32_t errCode);
void     opipeInitMemPool();
uint32_t opipeAllocBuff(uint32_t size);

void progCmxBuff(uint32_t buffBase, uint32_t bufAddr, uint32_t width, uint32_t height, uint32_t nPlanes, uint32_t format);

void cfgSigma    (Opipe *pipe);
void cfgLsc      (Opipe *pipe);
void cfgRaw      (Opipe *pipe);
void cfgDbyr     (Opipe *pipe);
void cfgDogLtm   (Opipe *pipe);
void cfgLumaDns  (Opipe *pipe);
void cfgSharpen  (Opipe *pipe);
void cfgGenChr   (Opipe *pipe);
void cfgMedian   (Opipe *pipe);
void cfgChromaDns(Opipe *pipe);
void cfgColComb  (Opipe *pipe);
void cfgLut      (Opipe *pipe);
void cfgConv     (Opipe *pipe);
void cfgUpfirdn0 (Opipe *pipe);
void cfgUpfirdn1 (Opipe *pipe);
void cfgUpfirdn2 (Opipe *pipe);
void cfgMipiRx0  (Opipe *pipe);
void cfgMipiRx1  (Opipe *pipe);
void cfgMipiRx2  (Opipe *pipe);
void cfgMipiRx3  (Opipe *pipe);
void cfgHarris   (Opipe *pipe);

void SetSippInterrupts();
void cfgSources  (Opipe *p);
void cfgSinks    (Opipe *p);
void cfgSwLinks  (Opipe *p);
void startSources(Opipe *p);

//Default IRQ handlers
void oPipeSippObflIncIsr(uint32_t irqSrc);
void oPipeSippIbflDecIsr(uint32_t irqSrc);
void oPipeSippEofIsr    (uint32_t irqSrc);

void OpipeDetCfg (Opipe *p);
void OpipeHwReset(uint32_t mask);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
