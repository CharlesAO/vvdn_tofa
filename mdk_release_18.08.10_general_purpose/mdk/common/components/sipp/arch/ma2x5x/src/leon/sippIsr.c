/// @file      sippIsr.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API - asynchronous runtime API
///            Platform(s) supported : ma2x5x
///

/////////////////////////////////////////////////////////////////////////////////
//  Header Files
/////////////////////////////////////////////////////////////////////////////////
#include <sipp.h>
#include <sippInternal.h>
#ifdef MYRIAD2
#include <DrvIcbDefines.h>
#include <DrvIcb.h>
#endif
#ifdef SIPP_PC
#include "wrapperSem.h"
#endif

/////////////////////////////////////////////////////////////////////////////////
//  Global Variables
/////////////////////////////////////////////////////////////////////////////////
extern pSIPP_HW_SESSION pgSippHW;
#ifdef SIPP_PC
extern Semaphore *      serialiseSem;
#endif
extern tSippFramework   gSippFramework;
extern u32              sippGlobalOBFLIncStatus;

/////////////////////////////////////////////////////////////////////////////////
//  Global Macros
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//  Code
/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippIsrSetup                                                     //
//                                                                                //
//  DESCRIPTION: Program ICB driver with sipp requirements                        //
//                                                                                //
//  INPUTS:      None.                                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippIsrSetup ( )
{
    #if defined(__sparc)
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    if (swcWhoAmI() == PROCESS_LEON_OS)
    {
        #ifdef SIPP_SCRT_ENABLE_OPIPE
        DrvIcbIrqClear (gSippFramework.dynIrqSipp0);
        DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_0, gSippFramework.dynIrqSipp0, 0x1);
        DrvIcbSetupIrq (gSippFramework.dynIrqSipp0, 10, POS_LEVEL_INT, sippIbflDecHandler);
        #endif

        DrvIcbIrqClear (gSippFramework.dynIrqSipp1);
        DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_1, gSippFramework.dynIrqSipp1, 0x1);
        DrvIcbSetupIrq (gSippFramework.dynIrqSipp1, 10, POS_LEVEL_INT, sippObflIncHandler);

        DrvIcbIrqClear (gSippFramework.dynIrqSipp2);
        DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_2, gSippFramework.dynIrqSipp2, 0x1);
        DrvIcbSetupIrq (gSippFramework.dynIrqSipp2, 10, POS_LEVEL_INT, sippFrameDoneIrqHandler);
    }
    else
    {
        #ifndef SIPP_DISABLE_INTERRUPT_SETUP
        #ifdef SIPP_SCRT_ENABLE_OPIPE
        DrvIcbSetupIrq (IRQ_SIPP_0, 10, POS_LEVEL_INT, sippIbflDecHandler);
        #endif
        DrvIcbSetupIrq (IRQ_SIPP_1, 10, POS_LEVEL_INT, sippObflIncHandler);
        DrvIcbSetupIrq (IRQ_SIPP_2, 10, POS_LEVEL_INT, sippFrameDoneIrqHandler);
        #endif
    }
    #endif
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippIntBarrierSetup                                              //
//                                                                                //
//  DESCRIPTION: HW filter interrupt service routine                              //
//                                                                                //
//  INPUTS:      useIntBar - Set to force use interrupt barriers                  //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippIntBarrierSetup (u32 useIntBar)
{
    // Output buffer fill level increment interrupt
    // This assumes the SIPP framework is the only processing paradigm for the SIPP HW
    if (useIntBar)
    {
        #ifndef NO_INTERRUPT_BARRIERS_MODE
        sippGlobalOBFLIncStatus = 0;
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, 0xFFFFFFFF);

        SET_REG_WORD(SIPP_INT1_BARRIER0_ADR, 0xFFFFFFFF);
        SET_REG_WORD(SIPP_INT1_BARRIER1_ADR, 0xFFFFFFFF);
        SET_REG_WORD(SIPP_INT1_BARRIER2_ADR, 0xFFFFFFFF);
        SET_REG_WORD(SIPP_INT1_BARRIER3_ADR, 0xFFFFFFFF);
        #endif

        #ifndef SIPP_NO_IRQ_BARRIER2
        SET_REG_WORD(SIPP_INT2_CLEAR_ADR, 0xFFFFFFFF);
        SET_REG_WORD(SIPP_INT2_BARRIER0_ADR, 0xFFFFFFFF);
        SET_REG_WORD(SIPP_INT2_BARRIER1_ADR, 0xFFFFFFFF);
        SET_REG_WORD(SIPP_INT2_BARRIER2_ADR, 0xFFFFFFFF);
        SET_REG_WORD(SIPP_INT2_BARRIER3_ADR, 0xFFFFFFFF);
        #endif

        sippPalTrace(SIPP_RUNTIME_TL_INFO, "Interrupt barriers enabled\n");
    }
    else
    {
        // Clear the interrupt barrier
        SET_REG_WORD(SIPP_INT0_BARRIER0_ADR, 0);
        SET_REG_WORD(SIPP_INT0_BARRIER1_ADR, 0);
        SET_REG_WORD(SIPP_INT0_BARRIER2_ADR, 0);
        SET_REG_WORD(SIPP_INT0_BARRIER3_ADR, 0);

        SET_REG_WORD(SIPP_INT1_BARRIER0_ADR, 0);
        SET_REG_WORD(SIPP_INT1_BARRIER1_ADR, 0);
        SET_REG_WORD(SIPP_INT1_BARRIER2_ADR, 0);
        SET_REG_WORD(SIPP_INT1_BARRIER3_ADR, 0);

        SET_REG_WORD(SIPP_INT2_BARRIER0_ADR, 0);
        SET_REG_WORD(SIPP_INT2_BARRIER1_ADR, 0);
        SET_REG_WORD(SIPP_INT2_BARRIER2_ADR, 0);
        SET_REG_WORD(SIPP_INT2_BARRIER3_ADR, 0);

        sippPalTrace(SIPP_RUNTIME_TL_INFO, "Interrupt barriers disabled\n");
    }

    pgSippHW->isrSetup = useIntBar;
}
