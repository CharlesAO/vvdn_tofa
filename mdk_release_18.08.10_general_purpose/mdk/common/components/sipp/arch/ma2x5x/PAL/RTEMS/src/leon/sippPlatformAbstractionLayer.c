///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

/////////////////////////////////////////////////////////////////////////////////
//  Header files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippPalTypes.h>
#include <sippPal.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <rtems.h>
#include <rtems/libio.h>
#include <rtems/bspIo.h>
//#include <rtems/status-checks.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <pthread.h>
#include <sched.h>
#include <fcntl.h>

#ifndef SIPP_NO_PRINTF
#include <VcsHooksApi.h>
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Local Macros
/////////////////////////////////////////////////////////////////////////////////

#ifndef SIPP_NO_PRINTF
static u32 guTraceLevel;
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Storage Allocation Globals
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Code

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
        va_start ( arg, psz_format);
        vprintk (psz_format, arg );
        va_end(arg);
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
    int intLevel;
    rtems_interrupt_disable (intLevel);
    *pState = intLevel;

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
    rtems_interrupt_enable ((int)PreviousState);
}

///////////////////////////////////////////////////
// RTOS primitive management
//

///////////////////////////////////////////////////
// Queues
//

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalQuCreate                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       This is internal to the SIPP so I will only have one         //
//               read client for any qu so I open it only once                //
//               All queues are blocking - any non-blocking write access is   //
//               handled in abstraction layer (facilitating posting from irq) //
//                                                                            //
//  CONTEXT:     This function must be called from non-interrupt context.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippPalQuCreate (uint32_t       maxElements,
                              const char *   pszName,
                              SIPP_PAL_QU_ID pQu)
{
    struct mq_attr m_attr;
    m_attr.mq_maxmsg  = maxElements;
    m_attr.mq_msgsize = SIPP_PAL_MESSAGE_SIZE;   // Always have messages be a pointer???

    mqd_t msgq_id     = mq_open (pszName,
                                 O_RDWR | O_CREAT,
                                 S_IRWXU | S_IRWXG,
                                 &m_attr);

    if (msgq_id == (mqd_t) - 1)
    {
        perror ("sippPalQuCreate : Queue creation failure");
        return eSIPP_STATUS_RESOURCE_ERROR;
    }

    // Only allowing one qu at the mo so n oneed to go more complex than this...
    pQu->rtemsQuId    = msgq_id;
    pQu->elementsInQ  = 0;
    pQu->maxQElements = maxElements;
    pQu->qFlags       = 0;

    return eSIPP_STATUS_OK;

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalQuDestroy                                             //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       Doesn't really destroy qu - just renders descriptor invalid! //
//                                                                            //
//  CONTEXT:     This function must be called from non-interrupt context.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippPalQuDestroy (SIPP_PAL_QU_ID pQu)
{
    int32_t rc;

    rc = mq_close(pQu->rtemsQuId);

    if (rc == -1)
    {
        perror ("sippPalQuDestroy : Queue closure failure");
        return eSIPP_STATUS_RESOURCE_ERROR;
    }

    return eSIPP_STATUS_OK;

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalQuAttach                                              //
//                                                                            //
//  DESCRIPTION: Attach to an already created message qu                      //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
//  CONTEXT:     This function must be called from non-interrupt context.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippPalQuAttach (const char *   pszName,
                              SIPP_PAL_QU_ID pQu)
{
    mqd_t msgq_id     = mq_open (pszName,
                                 O_RDWR,
                                 S_IRWXU | S_IRWXG,
                                 NULL);

    if (msgq_id == (mqd_t) - 1)
    {
        perror ("sippPalQuAttach : Queue creation failure");
        return eSIPP_STATUS_RESOURCE_ERROR;
    }

    // Only allowing one qu at the mo so n oneed to go more complex than this...
    pQu->rtemsRWQuId    = msgq_id;

    return eSIPP_STATUS_OK;

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalQuPost                                                //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:                                                                    //
//                                                                            //
//  CONTEXT:     This function must be called from any context.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippPalQuPost (SIPP_PAL_QU_ID quId,
                            void *         message)
{
    int32_t             sc;
    SIPP_PAL_CRIT_STATE cs;
    SIPP_PAL_QU_ID      pSippPalQu = quId;
    mqd_t               msgq_id    = (mqd_t)pSippPalQu->rtemsQuId;

    if (pSippPalQu->qFlags & SIPP_QU_FLAG_STRUCT_FULL)
    {
        // Q is full. Cannot send more messages. This is being done at abstraction level
        // since Queues in RTEMS are blocking on write (which I don't want)
        // if they are blocking on read (which I do)
        return eSIPP_STATUS_FULL;
    }

    sc = mq_send (msgq_id,
                  (const char *)message,
                  SIPP_PAL_MESSAGE_SIZE,
                  0);                                      // We could use pririty here when freeing HW resource?

    if (sc == 0)
    {
        sippPalCriticalSectionBegin (&cs);

        pSippPalQu->elementsInQ += 1;
        if (pSippPalQu->elementsInQ == pSippPalQu->maxQElements)
        {
          pSippPalQu->qFlags |= SIPP_QU_FLAG_STRUCT_FULL;
        }

        sippPalCriticalSectionEnd (cs);
    }
    else
    {
        perror("sippPalQuPost: Unable to post");
        return eSIPP_STATUS_FULL;
    }

    return eSIPP_STATUS_OK;

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalQuReceive                                             //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       Doesn't really destroy qu - just renders descriptor invalid! //
//                                                                            //
//  CONTEXT:     This function must be called from any context.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippPalQuReceive (SIPP_PAL_QU_ID    quId,
                               SIPP_PAL_QU_MSG * message)
{
    int32_t             sc;
    SIPP_PAL_CRIT_STATE cs;
    SIPP_PAL_QU_ID      pSippPalQu = quId;
    mqd_t               msgq_id    = (mqd_t)pSippPalQu->rtemsQuId;
    char                msg[SIPP_PAL_MESSAGE_SIZE];

    sc = mq_receive (msgq_id,
                     msg,
                     SIPP_PAL_MESSAGE_SIZE,
                     0);

    if (sc != SIPP_PAL_MESSAGE_SIZE)
    {
        perror("sippPalQuReceive: Unable to receive message");
        return eSIPP_STATUS_INTERNAL_ERROR;
    }
    else
    {
        sippPalCriticalSectionBegin (&cs);
        if (pSippPalQu->elementsInQ)
        {
            pSippPalQu->elementsInQ -= 1;
        }
        pSippPalQu->qFlags &= ~SIPP_QU_FLAG_STRUCT_FULL;

        *message = (SIPP_PAL_QU_MSG)msg;
        sippPalCriticalSectionEnd (cs);
    }

    return eSIPP_STATUS_OK;
}


///////////////////////////////////////////////////
// Threads
//

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalThreadCreate                                          //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:       Doesn't really destroy qu - just renders descriptor invalid! //
//                                                                            //
//  CONTEXT:     This function must be called from any context.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

eSIPP_STATUS sippPalThreadCreate (SIPP_PAL_PFNTHREAD pfnEntryPoint,
                                  int                nArgC,
                                  void **            ppArgV,
                                  uint32_t           stackSize,
                                  uint8_t            priority,
                                  const char *       pszName,
                                  SIPP_PAL_THREAD_ID pThread)
{

    rtems_status_code  sc;
    struct sched_param schParam;
    pthread_attr_t     attr;
    int32_t            policy;

    if (pthread_attr_init(&attr) != 0)
    {
        perror ("sippPalThreadCreate: pthread_attr_init error");
        return eSIPP_STATUS_INTERNAL_ERROR;
    }
    if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0)
    {
        perror ("sippPalThreadCreate: pthread_attr_setinheritsched error");
        return eSIPP_STATUS_INTERNAL_ERROR;
    }
    if (pthread_attr_setschedpolicy(&attr, SCHED_RR) != 0)
    {
        perror ("sippPalThreadCreate: pthread_attr_setschedpolicy error");
        return eSIPP_STATUS_INTERNAL_ERROR;
    }

    pthread_getschedparam(pthread_self(), (int *)&policy, &schParam);
    schParam.sched_priority = priority;

    if (pthread_attr_setschedparam (&attr, &schParam) != 0)
    {
        perror ("sippPalThreadCreate: pthread_attr_setschedparam error");
        return eSIPP_STATUS_INTERNAL_ERROR;
    }

    if (pthread_attr_setstacksize (&attr, (size_t)stackSize) != 0)
    {
        perror ("sippPalThreadCreate: pthread_attr_setstacksize error");
        return eSIPP_STATUS_INTERNAL_ERROR;
    }

    /* Fill in the parameters to be passed */
    pThread->args[0]     = (uint32_t)nArgC;
    pThread->args[1]     = (uint32_t)ppArgV;
    pThread->args[2]     = (uint32_t)pfnEntryPoint;
    pThread->args[3]     = (uint32_t)0x0; // Unused at this point

    pThread->stackSize   = (uint32_t)stackSize;
    pThread->priority    = (uint8_t)priority;

    // Null rtems thread descriptor to obtain a testable param later to see if this worked.
    pThread->rtemsThread = (pthread_t)0x0;

    if ((sc = pthread_create (&pThread->rtemsThread,
                              &attr,
                              (SIPP_RTEMS_PFNTHREAD)pfnEntryPoint,
                              (void *)pThread->args)))
    {
        perror ("sippPalThreadCreate: pthread_create error");
        return eSIPP_STATUS_INTERNAL_ERROR;
    }
    else
    {
        printk("sippPalThreadCreate: Thread %s created\n", pszName);
    }

    return eSIPP_STATUS_OK;

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippPalThreadTerminate                                       //
//                                                                            //
//  DESCRIPTION: Self-destruct function                                       //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None.                                                        //
//                                                                            //
//  RETURNS:     None.                                                        //
//                                                                            //
//  NOTES:        Cannot be used to terminate a different thread other than   //
//                the calling thread                                          //
//                                                                            //
//  CONTEXT:     This function must be called from any context.               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void sippPalThreadTerminate (SIPP_PAL_THREAD_ID pThread)
{
    if (pThread->rtemsThread)
    {
        pthread_exit ((void *)0);
    }
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
//  FUNCTION:    sippPalMemCompare                                            //
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

    return uResult;
}

