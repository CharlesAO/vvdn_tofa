///
/// @file DrvLeon.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvLeon
/// @{
/// @brief     Driver to control the startup of LeonRT processor
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvCpr.h>
#include <registersMyriad.h>
#include <assert.h>
#include <DrvRegUtils.h>
#include <DrvLeon.h>
#include <DrvLeonDefines.h>
#include <swcLeonUtils.h>
#include <stdio.h>
#include <dbgTracerApi.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define LEON_STARTUP_ALIGNMENT_MASK   ( 0xFFF)  // bottom 12 bits of target address must be 0 for 4KB alignment

#define LRT_DSU_DEBUG_MODE (1 << 6)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
volatile drvLeonStates_enum_t __attribute__((section(".cmx.data.LeonRTState"))) LeonRTState=LEON_NO_STATE;
extern volatile drvLeonStates_enum_t lrt_LeonRTState;
extern u32 lrt_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void DrvLeonRTStartup(u32 leonStartupAddress)
{
    u32 __attribute__((unused)) activateRead;  // avoid 'unused variable' error from scan build
    u32 lrtDsuCtrlValue;
    u32 lrtVectorValue;

    // The LeonRT application start address must be aligned to a 4K boundary
    // as described in the hardware documentation
    // This is enforced in the default linker script but checked here.
    assert((leonStartupAddress & LEON_STARTUP_ALIGNMENT_MASK) == 0);

    lrtDsuCtrlValue = GET_REG_WORD_VAL(DSU_LEON_RT_CTRL_ADR);
    lrtVectorValue = GET_REG_WORD_VAL(CPR_LEON_RT_VECTOR_ADR);

#ifdef POWER_AWARE
     if (!DrvCprGetUpaClk(POWER_AWARE_LRT_CORE_ID))
        DrvCprEnableUpa(POWER_AWARE_LRT_CORE_ID);
#endif

    // DM(Debug Mode - bit 6) bit of the LRT_DSU_CTRL register is now used to determine
    // if we are running under a debugger or we had a cold boot.
    // The previous condition, DSU_Leon_OS_Debug_Enable (bit 17) of the CPR_GEN_CTRL
    // register is not valid anymore because it is set by the boot code regardless of
    // the boot mode.

    if ((lrtVectorValue & LEON_IRQI_RUN) && (lrtDsuCtrlValue & LRT_DSU_DEBUG_MODE))
    {
        // This signifies that the LeonRT Debug Support Unit is enabled which
        // means that we are running under a debugger. In this case it is not
        // possible to start the LRT using the LeonRT cold boot sequence.
        // Instead it is necessary to use the DSU functionality to start the LeonRT
        // This must be done in a way that interferes with debugger operation as little
        // as possible

        // Get the original DSU Control value to preserve debugger settings
        lrtDsuCtrlValue = GET_REG_WORD_VAL(DSU_LEON_RT_CTRL_ADR);

        lrtDsuCtrlValue &= ~DSU_CTRL_PROC_ERROR_BIT; // Writing 1 into the error mode bit
        // resets the error mode, and other stuff, so we will only want to explicitly set it

        // Now set a couple of BREAK ON bits.
        lrtDsuCtrlValue |= DSU_CTRL_BREAK_ON_ERROR_BIT | DSU_CTRL_BREAK_ON_SW_WATCHPOINT_BIT;

        // Apply processor Halt bit to hold in halt until ready to execute, and clear error mode
        SET_REG_WORD(DSU_LEON_RT_CTRL_ADR,lrtDsuCtrlValue | DSU_CTRL_PROC_HALT_BIT | DSU_CTRL_PROC_ERROR_BIT);

        // Now that we have cleared error mode, we are allowed to write to the PC registers:
        // setup the DSU PC/NPC from which to start execution
        SET_REG_WORD(DSU_LEON_RT_PC_REG_ADR ,leonStartupAddress    );
        SET_REG_WORD(DSU_LEON_RT_NPC_REG_ADR,leonStartupAddress + 4);

        // Clear the BreakNow and SingleStep bits, just in case they were set
        SET_REG_WORD(DSU_LEON_RT_BREAK_SINGLE_STEP_ADR,0);

        // Finally start execution by clearing the Processor Halt Bit
        lrtDsuCtrlValue &= ~DSU_CTRL_PROC_HALT_BIT;
        SET_REG_WORD(DSU_LEON_RT_CTRL_ADR,lrtDsuCtrlValue);
    }
    else
    {
        // This sequence performs a cold boot start of the LeonRT
        // (i.e. This is used with the LRT is not under the control the Debugger DSU)
        leonStartupAddress &= ~LEON_STARTUP_ALIGNMENT_MASK; // Force alignment

        // Setup the new target address and the RUN bit to request start of execution
        SET_REG_WORD(CPR_LEON_RT_VECTOR_ADR,((u32)leonStartupAddress) | LEON_IRQI_RUN );

        // Then toggle the system reset to the LEON_RT to get it to latch the new address
      	DrvCprSysDeviceAction(MSS_DOMAIN,ASSERT_RESET,   DEV_MSS_LRT | DEV_MSS_LRT_DSU );
        DrvCprSysDeviceAction(MSS_DOMAIN,DEASSERT_RESET, DEV_MSS_LRT                   );

        // The following read is needed as a barrier to ensure that the previous writes
        // have taken effect
        activateRead = GET_REG_WORD_VAL(MSS_RSTN_CTRL_ADR);

        // Finally pulse the IRQI.RST input to the LeonRT to actually start it running
        SET_REG_WORD(CPR_LEON_RT_VECTOR_ADR,((u32)leonStartupAddress) | LEON_IRQI_RUN | LEON_IRQI_RESET );
        SET_REG_WORD(CPR_LEON_RT_VECTOR_ADR,((u32)leonStartupAddress) | LEON_IRQI_RUN );
    }


    //Set the state to running
    lrt_LeonRTState=LEON_RUNNING;

    // Add the tracer functionality to visualize the leon rt run:start
    dbgLogEvent(LOG_EVENT_LRT_RUN, 1, DEBUG_LOG_LEVEL_HIGH);

    return;
}

void DrvLeonRTStartupStart(void)
{
    DrvLeonRTStartup((u32)&lrt_start);
}

void DrvLeonRTSignalStop(void)
{
    u32 pointerAddress;
    pointerAddress  = (u32)&LeonRTState;
	if(!(pointerAddress & 0x80000000))
        pointerAddress |= 0x08000000;             // Use uncached pointer for CMX
    else
        pointerAddress |= 0x40000000;             // Use uncached pointer for DDR

	//Signal that we have stopped
	SET_REG_WORD(pointerAddress,LEON_FINISHED);
    // Add the tracer functionality to visualize the leon rt run:complete
    dbgLogEvent(LOG_EVENT_LRT_RUN, 0, DEBUG_LOG_LEVEL_HIGH);
    return;
}

void DrvLeonRTSignalBootCompleted(void)
{
    u32 pointerAddress;
    pointerAddress  = (u32)&LeonRTState;
	if(!(pointerAddress & 0x80000000))
	    pointerAddress |= 0x08000000;             // Use uncached pointer for CMX
	else
	    pointerAddress |= 0x40000000;             // Use uncached pointer for DDR

    //Signal that we have stopped
    SET_REG_WORD(pointerAddress,LEON_BOOTED);
    return;
}

int DrvLeonRTHasStopped(void)
{
    u32 ReadValue;
    u32 pointerAddress;
    pointerAddress  = (u32)&lrt_LeonRTState;
	if(!(pointerAddress & 0x80000000))
	    pointerAddress |= 0x08000000;             // Use uncached pointer for CMX
	else
	    pointerAddress |= 0x40000000;             // Use uncached pointer for DDR

    ReadValue=swcLeonReadNoCacheU32(pointerAddress);
    if (ReadValue == LEON_FINISHED) {
        return 1;
    }else{
        return 0;
    }
}

int DrvLeonRTHasBooted(void)
{
    u32 ReadValue;
    u32 pointerAddress;
    pointerAddress  = (u32)&lrt_LeonRTState;
	if(!(pointerAddress & 0x80000000))
	    pointerAddress |= 0x08000000;             // Use uncached pointer for CMX
	else
	    pointerAddress |= 0x40000000;             // Use uncached pointer for DDR

    ReadValue=swcLeonReadNoCacheU32(pointerAddress);
    if (ReadValue == LEON_BOOTED) {
        return 1;
    } else {
        return 0;
    }
}

void DrvLeonRTWaitExecution(void)
{
	// Add the tracer functionality to visualize the leon rt wait:start
	dbgLogEvent(LOG_EVENT_WAIT_FOR_LRT, 1, DEBUG_LOG_LEVEL_HIGH);
    volatile u32 ReadValue;
    u32 pointerAddress;
    pointerAddress  = (u32)&lrt_LeonRTState;
	if(!(pointerAddress & 0x80000000))
	    pointerAddress |= 0x08000000;             // Use uncached pointer for CMX
	else
	    pointerAddress |= 0x40000000;             // Use uncached pointer for DDR

    ReadValue=swcLeonReadNoCacheU32(pointerAddress);
    //Wait blocking for LeonRT to signal it has finished
    while (ReadValue != LEON_FINISHED){
        ReadValue=swcLeonReadNoCacheU32(pointerAddress);
    }

#ifdef POWER_AWARE
    if (DrvCprGetUpaClk(POWER_AWARE_LRT_CORE_ID))
        DrvCprClearUpa(POWER_AWARE_LRT_CORE_ID);
#endif

    // Add the tracer functionality to visualize the leon rt wait:complete
    dbgLogEvent(LOG_EVENT_WAIT_FOR_LRT, 0, DEBUG_LOG_LEVEL_HIGH);
    return;
}

void DrvLeonRTWaitForBoot(void)
{
    volatile u32 ReadValue;
    u32 pointerAddress;
    pointerAddress  = (u32)&lrt_LeonRTState;
	if(!(pointerAddress & 0x80000000))
	    pointerAddress |= 0x08000000;             // Use uncached pointer for CMX
	else
	    pointerAddress |= 0x40000000;             // Use uncached pointer for DDR

    ReadValue=swcLeonReadNoCacheU32(pointerAddress);
    //Wait blocking for LeonRT to signal it has finished
    while (ReadValue != LEON_BOOTED){
        ReadValue=swcLeonReadNoCacheU32(pointerAddress);
    }
    return;
}

///@}
