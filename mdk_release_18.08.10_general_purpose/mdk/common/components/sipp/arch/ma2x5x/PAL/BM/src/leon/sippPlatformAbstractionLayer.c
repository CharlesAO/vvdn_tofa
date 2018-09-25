#include <sippPalTypes.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#ifdef __RTEMS__
#include <rtems.h>
#endif


#ifndef SIPP_NO_PRINTF
#include <VcsHooksApi.h>
#endif
#include <DrvIcb.h>

#ifndef SIPP_NO_PRINTF
#define SIPP_PRT_BUFF_SIZE 1024
static char achspfbuff[SIPP_PRT_BUFF_SIZE];
static u32 guTraceLevel;
#endif

///////////////////////////////////////////////////
// Trace / Error / Message log functions
//

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalTraceInit (was trace_init in app code)                //
//                                                                            //
//  DESCRIPTION: Use global platform config file to set up trace levels       //
//                                                                            //
//  INPUTS:      None                                                         //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void sippPalTraceInit ( void )
{
    #ifndef SIPP_NO_PRINTF
    guTraceLevel = SIPP_TRACE_LEVEL_5;
    #endif /* !SIPP_NO_PRINTF */

}


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalTrace                                                 //
//                                                                            //
//  DESCRIPTION: This function sends a trace message depending upon the flags //
//               provided.                                                    //
//                                                                            //
//  INPUTS:      uFlags - Value indicating the source of this trace message   //
//                        and its priority level.                             //
//               string - Print formating string (as for printf).             //
//               ...    - Additional parameters depending upon the contents   //
//                        of string.                                          //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       None.                                                        //
//                                                                            //
//  CONTEXT:     This function must be called from non-interrupt context.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef SIPP_NO_PRINTF

void sippPalTrace ( u32 uFlags, const char *psz_format, ...)
{
    va_list arg;
    u8  bEnabled = (uFlags > guTraceLevel) ? 0x1 : 0x0;

    if ( bEnabled )
    {
        va_start( arg, psz_format);
        vsnprintf ( &achspfbuff[0], SIPP_PRT_BUFF_SIZE, ( const char *)psz_format, arg );
        va_end(arg);

        vcsFastPuts (achspfbuff);

    }
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalPrintInt                                              //
//                                                                            //
//  DESCRIPTION: This function provides optimised version of sippPalTrace     //
//               for printing only one value                                  //
//                                                                            //
//  INPUTS:      uFlags - Value indicating the source of this trace message   //
//                        and its priority level.                             //
//               string - Print formating string (as for printf).             //
//               ...    - Additional parameters depending upon the contents   //
//                        of string.                                          //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       None.                                                        //
//                                                                            //
//  CONTEXT:     This function must be called from non-interrupt context.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void sippPalPrintInt (u32 uVal)
{
    u8  bEnabled = (guTraceLevel < SIPP_TRACE_LEVEL_4) ? 0x1 : 0x0;

    if (bEnabled)
    {
        printInt (uVal);
    }
}

#endif /* SIPP_NO_PRINTF */

///////////////////////////////////////////////////
// Critical section functions
//

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalCriticalSectionBegin                                      //
//                                                                                //
//  DESCRIPTION: This function begins a critical section                          //
//                                                                                //
//  INPUTS:      pState - Storage for returned state value which must be          //
//                        passed on the matching call to crit_sec_exit            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippPalCriticalSectionBegin (SIPP_PAL_CRIT_STATE * pState)
{
    #ifdef __RTEMS__
    rtems_interrupt_level level;
    rtems_interrupt_disable(level);
    *pState = level;
    #else
    *pState = swcLeonSetPIL(15);
    #endif
} /* sippPalCriticalSectionBegin */

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalCriticalSectionEnd                                    //
//                                                                            //
//  DESCRIPTION: This function ends a critical section                        //
//                                                                            //
//  INPUTS:      PreviousState - The execution state returned by the matching //
//                               call to sippPalCriticalSectionBegin          //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       None.                                                        //
//                                                                            //
//  CONTEXT:     This function must be called from any context.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void sippPalCriticalSectionEnd (SIPP_PAL_CRIT_STATE PreviousState)
{
    #ifdef __RTEMS__
    rtems_interrupt_level level = (rtems_interrupt_level)PreviousState;
    rtems_interrupt_enable(level);
    #else
    u32 interruptLevel = (u32)PreviousState;
    swcLeonSetPIL(interruptLevel);
    #endif
}



///////////////////////////////////////////////////
// C Runtime Wrappers
//


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalMemcpy                                                //
//                                                                            //
//  DESCRIPTION:                                                              //
//     Copy a block of bytes from one location in memory to another           //
//                                                                            //
//  INPUT PARAMETERS:                                                         //
//     pDest      - Pointer to the buffer into which bytes will be copied.    //
//     pSrc       - Pointer to the first byte to be copied                    //
//     uSize      - Number of bytes to be copied                              //
//                                                                            //
//  OUTPUT PARAMETERS:                                                        //
//                                                                            //
//  RETURN VALUES:                                                            //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
//  CONTEXT:                                                                  //
//     This function may be called from any context                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void * sippPalMemcpy (void *       pDest,
                      const void * pSrc,
                      u32          uSize)
{
    if ( pDest == NULL || pSrc == NULL )
    {
       return (void *)NULL;
    }

    return memcpy ( pDest, pSrc, uSize );
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalMemset                                                //
//                                                                            //
//  DESCRIPTION:                                                              //
//     Set a block of memory to a given value                                 //
//                                                                            //
//  INPUT PARAMETERS:                                                         //
//     pDest      - Pointer to the first byte to be set                       //
//     nChar      - The value to be used as the set to value                  //
//     uSize      - Number of bytes to be set                                 //
//                                                                            //
//  OUTPUT PARAMETERS:                                                        //
//                                                                            //
//  RETURN VALUES:                                                            //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
//  CONTEXT:                                                                  //
//     This function may be called from any context                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void sippPalMemset (void *pDest, s32 nChar, u32 uCount)
{
    memset (pDest, nChar, uCount);
    //vcsHookFastMemSet (pDest, nChar, uCount);
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    pal_memcompare                                               //
//                                                                            //
//  DESCRIPTION:                                                              //
//     Compare two memory areas ( word aligned and in integer size in terms   //
//     of words )                                                             //
//                                                                            //
//  INPUT PARAMETERS:                                                         //
//                                                                            //
//  OUTPUT PARAMETERS:                                                        //
//                                                                            //
//  RETURN VALUES:                                                            //
//     True - if there is a difference, False if 2 areas have same contents   //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
//  CONTEXT:                                                                  //
//     This function may be called from any context                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u8 sippPalMemCompare (void *pArea1, void *pArea2, u32 uSizeInWords)
{

  u32 i;
  u32 *ptr0, *ptr1;
  u32 uChange;

  ptr0 = ( u32 * ) pArea1;
  ptr1 = ( u32 * ) pArea2;

  for ( i = 0, uChange = 0; ( i < uSizeInWords ) && ( uChange == 0 ); i++ )
  {
    if ( ptr0[i] != ptr1[i] )
    {
      uChange = 1;
    }
  }

  return ( uChange ) ? 0x1 : 0x0;
}



///////////////////////////////////////////////////
// Miscellaneous functions
//

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalFindHighestBit                                        //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:                                                                  //
//                                                                            //
//  RETURNS:                                                                  //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
//  CONTEXT:     ??                                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 sippPalFindHighestBit (u32 uValue)
{
    register u32 uResult;
    register u32 uShift;

    /***************************************************/
    /* Get the highest bit set in the uValue parameter */
    /***************************************************/
    if (uValue != 0)
    {
        uResult = 0;
        uShift = ( ( uValue & 0xFFFF0000 ) != 0 ) << 4; uValue >>= uShift; uResult |= uShift;
        uShift = ( ( uValue & 0xFF00     ) != 0 ) << 3; uValue >>= uShift; uResult |= uShift;
        uShift = ( ( uValue & 0xF0       ) != 0 ) << 2; uValue >>= uShift; uResult |= uShift;
        uShift = ( ( uValue & 0xC        ) != 0 ) << 1; uValue >>= uShift; uResult |= uShift;
        uShift = ( ( uValue & 0x2        ) != 0 ) << 0; uValue >>= uShift; uResult |= uShift;
    }
    else
    {
       return(0xFFFFFFFF);
    }
    UNUSED(uValue); // avoid 'unused variable' error from scan build

    return uResult;
}

