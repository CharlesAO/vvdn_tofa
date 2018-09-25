// -----------------------------------------------------------------------------
// Copyright (C) 2015 Movidius Ltd. All rights reserved
//
// Filename         : sippPal.h
// Company          : Movidius
// Description      : Header file for the SIPP Platform Abstraction Layer
//
// -----------------------------------------------------------------------------

#ifndef _SIPP_PAL_H_
#define _SIPP_PAL_H_

/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <sippTypes.h>
#include <sippPalTypes.h>

/* For va_list */
#include <stdarg.h>

/////////////////////////////////////////////////////////////////////////////////
//  Function prototypes
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////
// Critical section functions
//

void sippPalCriticalSectionBegin ( SIPP_PAL_CRIT_STATE *pState );

void sippPalCriticalSectionEnd ( SIPP_PAL_CRIT_STATE PreviousState );

///////////////////////////////////////////////////
// Interrupt Functions
//

///////////////////////////////////////////////////
// Processor Cache Control Functions
//

///////////////////////////////////////////////////
// C Runtime Wrappers
//

void * sippPalMemcpy ( void *pDest,
                       const void *pSrc,
                       u32 uSize );

//
void sippPalMemset ( void *pDest, s32 nChar, u32 uCount );

//
u8 sippPalMemCompare ( void *pArea1, void *pArea2, u32 uSizeInWords );

///////////////////////////////////////////////////
// Hardware Timer Service APIs
//

///////////////////////////////////////////////////
// Trace / Error / Message log functions
//

void sippPalTraceInit ( void );

#ifdef SIPP_NO_PRINTF
/* Declare sippPalTrace as an empty statement and cast to void to avoid a "no-effect" warning. */
/* This soaks up the trailing semi-colon and avoids leaving them dangling.                     */
#define sippPalTrace(...) (void)(0)
#define sippPalPrintInt(...) (void)(0)
#else
void sippPalTrace ( u32 uFlags, const char *psz_format, ...);
void sippPalPrintInt (u32 uVal);
#endif

///////////////////////////////////////////////////
// Miscellaneous functions
//

u32 sippPalFindHighestBit ( u32 uValue );

///////////////////////////////////////////////////
// Memory management abstraction functions
//

#if SIPP_RTOS != SIPP_NO_RTOS
#ifndef __MOVICOMPILE__

eSIPP_STATUS sippPalQuCreate (uint32_t       maxElements,
                              const char *   pszName,
                              SIPP_PAL_QU_ID quId);

eSIPP_STATUS sippPalQuDestroy (SIPP_PAL_QU_ID quId);

eSIPP_STATUS sippPalQuAttach (const char *   pszName,
                              SIPP_PAL_QU_ID pQu);

eSIPP_STATUS sippPalQuPost (SIPP_PAL_QU_ID quId,
                            void *         message);

eSIPP_STATUS sippPalQuReceive (SIPP_PAL_QU_ID quId,
                               SIPP_PAL_QU_MSG * message);

eSIPP_STATUS sippPalThreadCreate (SIPP_PAL_PFNTHREAD pfnEntryPoint,
                                  int                nArgC,
                                  void **            ppArgV,
                                  uint32_t           stackSize,
                                  uint8_t            priority,
                                  const char *       pszName,
                                  SIPP_PAL_THREAD_ID pThread);

void sippPalThreadTerminate (SIPP_PAL_THREAD_ID pThread);

#endif
#endif

#endif /* _SIPP_PAL_H_ */

/* End of File */
