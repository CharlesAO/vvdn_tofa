// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Software test library
///
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <DrvSvuDefines.h>
#include <registersMyriad.h>
#include <moviVectorUtils.h>
#include <swcWhoAmI.h>
#include <svuCommonShave.h>
#include <mv_types.h>
#include <stdio.h>
#include "testUtilsApi.h"
#include "testUtilsPrivate.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void shaveProfileInitDefault(performanceStruct *perfStruct)
{
  perfStruct->perfCounterStall = -1;
  perfStruct->perfCounterExec  = -1;
  perfStruct->perfCounterClkCycles  = -1;
  perfStruct->perfCounterBranch = -1;
}

void shaveProfileInit(performanceStruct *perfStruct, u32 stallsTypes)
{
  //   First thing first, in order to count the stalls, the user must specify the stalls types.
  perfStruct->stallsTypes = stallsTypes;
  shaveProfileInitDefault(perfStruct);
}

void shaveProfileReset(void)
{
  //Reset counters to trigger counting properly
  shaveSetLocalReg( PERF_COUNTER1_INSTR, 0 );
  shaveSetLocalReg( PERF_COUNTER2_STALLS, 0 );
  shaveSetLocalReg( PERF_COUNTER3_CLK_CYCLES, 0 );
  shaveSetLocalReg( PERF_COUNTER4_BRANCHES, 0 );
}

void shaveProfileStart(performanceStruct *perfStruct)
{
  //Enable history
  u32 bpCtrl = shaveGetLocalReg(OPERATION_CTRL_REG);
  bpCtrl |= OCR_HIST_REG;
  shaveSetLocalReg( OPERATION_CTRL_REG, bpCtrl );

  //Enable debugging
  u32 dcrValue = shaveGetLocalReg(DEBUG_CTRL_REG);
  dcrValue |= DCR_DBGENABLE;
  shaveSetLocalReg( DEBUG_CTRL_REG, dcrValue );

  //Enable exclusive access for the 4 counters/ counter controls
  scMutexRequest( swcWhoAmI() );

  //Enable the counters for all 4 control registers
  shaveSetLocalReg( PERF_COUNTER_CTRL_INSTR, PC_EX_IN_EN );
  shaveSetLocalReg( PERF_COUNTER_CTRL_STALLS, perfStruct->stallsTypes );
  shaveSetLocalReg( PERF_COUNTER_CTRL_CLK_CYCLES, PC_CLK_CYC_EN );
  shaveSetLocalReg( PERF_COUNTER_CTRL_BRANCHES, PC_BR_TAKEN_EN );

  shaveProfileReset();
}

void shaveProfileStop()
{
  shaveProfileReset();
  //Disable exclusive access for the 4 counters/ counter cuontrols
  scMutexRelease( swcWhoAmI() );
}
volatile u8 TestValDebug;
u32 shaveRetrieveLocalRegister( u32 localRegister )
{
  u32 shaveProcessAdr;
  shaveProcessAdr = swcWhoAmI();

  //Get current shave offset
  shaveProcessAdr = SHAVE_0_BASE_ADR + ((shaveProcessAdr - PROCESS_SHAVE0)<<16);
  shaveProcessAdr = shaveProcessAdr + localRegister;

  return shaveProcessAdr;
}

void shaveSetLocalReg( u32 localRegister, u32 value )
{
  u32 shaveAddress = shaveRetrieveLocalRegister( localRegister );

  SET_REG_WORD( shaveAddress, value );
}

u32 shaveGetLocalReg(u32 localRegister)
{
  u32 shaveAddress = shaveRetrieveLocalRegister( localRegister );

  return GET_REG_WORD_VAL( shaveAddress );
}

u32 shaveGetFieldValue(performanceStruct *perfStruct, \
    performanceCounterDef counterType)
{
  u32 fieldValue = 0;
  switch(counterType){
    case INSTRUCT_COUNT:
      fieldValue = shaveGetLocalReg( PERF_COUNTER1_INSTR );
      if( perfStruct )
        perfStruct->perfCounterExec = fieldValue;
      break;
    case STALL_COUNT:
      fieldValue = shaveGetLocalReg( PERF_COUNTER2_STALLS );
      if( perfStruct )
        perfStruct->perfCounterStall = fieldValue;
      break;
    case CLK_CYCLE_COUNT:
      fieldValue = shaveGetLocalReg( PERF_COUNTER3_CLK_CYCLES );
      if( perfStruct )
        perfStruct->perfCounterClkCycles = fieldValue;
      break;
    case BRANCH_COUNT:
      fieldValue = shaveGetLocalReg( PERF_COUNTER4_BRANCHES );
      if( perfStruct )
        perfStruct->perfCounterBranch = fieldValue;
      break;
    default:
      printf("Wrong field name - please specify the correct name\n");
  }
  return fieldValue;
}

void shaveRetrieveFieldValues( performanceStruct *perfStruct )
{
  perfStruct->perfCounterExec = shaveGetLocalReg( PERF_COUNTER1_INSTR );
  perfStruct->perfCounterStall = shaveGetLocalReg( PERF_COUNTER2_STALLS );
  perfStruct->perfCounterClkCycles = shaveGetLocalReg( PERF_COUNTER3_CLK_CYCLES );
  perfStruct->perfCounterBranch = shaveGetLocalReg( PERF_COUNTER4_BRANCHES );
}

void shaveProfilePrint( performanceStruct *perfStruct )
{
  printf("\nShave executed clock cycle count: %d\n", \
      perfStruct->perfCounterClkCycles);
  printf("\nNumber of branches taken: %d\n", \
      perfStruct->perfCounterBranch);
  printf("\nShave measured stall cycle count: %d\n", \
      perfStruct->perfCounterStall);
  printf("\nShave measured instruction count: %d\n", \
      perfStruct->perfCounterExec);
}

void shaveShowCounterValues( performanceStruct *perfStruct )
{
  shaveRetrieveFieldValues( perfStruct );
  shaveProfilePrint( perfStruct );
}

tyHwPlatform shaveGetProcessorType(void)
{
  tyHwPlatform platformType = -1; //unknown platform

  if (IS_PLATFORM_SILICON)
  {
    platformType = PLATFORM_SILICON;
  }
  else if (IS_PLATFORM_VCS)
  {
    platformType = PLATFORM_VCS;
  }
  else if (IS_PLATFORM_FPGA)
  {
    platformType = PLATFORM_FPGA;
  }
  else if (IS_PLATFORM_MOVISIM)
  {
    platformType = PLATFORM_MOVISIM;
  }
  else
  {
    printf("ERROR: no platform detected; check that the server is started\n");
  }
  return platformType;
}
