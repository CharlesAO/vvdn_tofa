///
/// @file DrvIcb.c
/// 
/// 
/// @ingroup DrvIcb
/// @{
/// @brief ICB Driver Implementation
///

/*===========================================================================*/
/*                            Include Headers                                */
/*===========================================================================*/
#include <DrvRegUtils.h>
#include "registersMyriad.h"
#include "mv_types.h"
#include "DrvIcb.h"
#include "DrvIcbDefines.h"
#include "swcLeonUtils.h"
#include "assert.h"
#ifdef __RTEMS__
#include "bsp.h"
#endif

#define IRQ_SRC_ENABLE_BIT_OFFSET   (7)    // Note: Bit 6 is reserved see ICB_C2C_INT_CFG0

#ifdef __RTEMS__
rtems_interrupt_handler gBSPIntsources[MV_TOTAL_LEON_IRQS] = {0};
#endif

// Irq vector use driver functions to modify it's content
extern irq_handler __irq_table[MV_TOTAL_LEON_IRQS];

// Disable/Restore Traps when dealing with ICB settings
#define DRVICB_CRITICAL_TRAPS
// Type of protection while handling ICB settings
#ifdef DRVICB_CRITICAL_TRAPS
// Start of critical section by disabling Traps (Allow changing PIL in interrupt context)
#define DRVICB_START_CRITICAL_SECTION()         swcLeonDisableTraps()
// End of critical section by restoring Traps. Allow changing PIL in interrupt context
#define DRVICB_END_CRITICAL_SECTION(_x)         if (_x) swcLeonEnableTraps()
#else
// Start of critical section by masking all interrupts. 
#define DRVICB_START_CRITICAL_SECTION()         swcLeonSetPIL(15)
// End of critical section by restoring PIL
#define DRVICB_END_CRITICAL_SECTION(_x)         swcLeonSetPIL(_x)
#endif

// Calculate the ICB byte offset we want to access:
// source >> 5 is the n-th word in the bit array
#define DRVICB_GET_SOURCE_OFFSET(source)           (((source) >> 5) << 2 )

/*===========================================================================*/
/*                                 Functions                                 */
/*===========================================================================*/
static inline u32 getIcbBase(void)
{
    if (swcLeonGetProcessorConfig() & LEON_PROCESSOR_INDEX_MASK)
        return ICB1_BASE_ADR; // LeonRT ICB base Address
    else
        return ICB0_BASE_ADR; // LeonOS ICB base Address 
}


void DrvIcbSetupIrq(u32 irqSrc, u32 priority, u32 type,irq_handler function)
{    
  // Check  Parameters 
  assert((irqSrc < MV_TOTAL_LEON_IRQS) && (priority <= 14) && (priority > 0) && (type <=3) && ((u32) function > 0));
#ifndef __RTEMS__
  u32 icbbase = getIcbBase(); 
  u32 t = DRVICB_GET_SOURCE_OFFSET(irqSrc);
  // Before accessing the ICB settings Start Critical Section
  u32 old_value = DRVICB_START_CRITICAL_SECTION();
  // Disable IRQ
  SET_REG_WORD(icbbase +  ICB_ENABLE_0_OFFSET  + t, GET_REG_WORD_VAL( icbbase +  ICB_ENABLE_0_OFFSET + t ) & ~( 1 << (irqSrc - t*8) ));
  // Clear IRQ
  SET_REG_WORD(icbbase + ICB_CLEAR_0_OFFSET +  t, 1 << (irqSrc - t*8) );
  // Setup Handler 
  if(irqSrc < MV_TOTAL_LEON_IRQS )
    __irq_table[irqSrc] = function;
  // Configure Interrupt and priority
  SET_REG_WORD( icbbase + ( irqSrc << 2 ), ( priority << 2 )| type );
  // Enable IRQ
  SET_REG_WORD(icbbase +  ICB_ENABLE_0_OFFSET + t, GET_REG_WORD_VAL( icbbase +  ICB_ENABLE_0_OFFSET + t) |( 1 << (irqSrc - t*8) ));
  // End of critical section
  DRVICB_END_CRITICAL_SECTION(old_value);
  // Enable Traps 
  swcLeonEnableTraps();
#else  
  // Disable interrupt if enabled
  BSP_Mask_interrupt(irqSrc);
  // Clear interrupt if already triggered 
  BSP_Clear_interrupt(irqSrc);
     // Install new Handler 
  BSP_interrupt_register(irqSrc, NULL, (rtems_interrupt_handler) function, (void *)(irqSrc));
  gBSPIntsources[irqSrc]=(rtems_interrupt_handler)function;
  // Set Type and priority 
  BSP_Set_interrupt_type_priority(irqSrc, type, priority);
  // Enable Interrupt 
  BSP_Unmask_interrupt(irqSrc);
#endif
}


void DrvIcbConfigureIrq( u32 source, u32 priority, u32 type ) {
  // Check parameters
  assert((source < MV_TOTAL_LEON_IRQS) && (priority <= 14) && (priority > 0) && (type <=3) );
#ifndef __RTEMS__ 
  u32 icbbase = getIcbBase();    
  // Before accessing the ICB settings Start Critical Section
  u32 old_value = DRVICB_START_CRITICAL_SECTION();
  // config trap trigger
  SET_REG_WORD( icbbase +( source << 2 ), ( priority << 2 )| type );
  // End of critical section
  DRVICB_END_CRITICAL_SECTION(old_value);
#else
  BSP_Set_interrupt_type_priority(source, type, priority);
#endif
}


void DrvIcbEnableIrq( u32 source ) {
  // Check parameters
  assert(source < MV_TOTAL_LEON_IRQS);
#ifndef __RTEMS__
  u32 t = DRVICB_GET_SOURCE_OFFSET(source);
  u32 icbbase = getIcbBase(); 
  // Before accessing the ICB settings Start Critical Section
  u32 old_value = DRVICB_START_CRITICAL_SECTION();
  SET_REG_WORD(icbbase +  ICB_ENABLE_0_OFFSET + t, GET_REG_WORD_VAL( icbbase +  ICB_ENABLE_0_OFFSET + t) |( 1 << (source - t*8) ));
  // End of critical section
  DRVICB_END_CRITICAL_SECTION(old_value);
#else
  BSP_Unmask_interrupt(source);
#endif
}


void DrvIcbDisableIrq(u32 source) {
  // Check parameters
  assert(source < MV_TOTAL_LEON_IRQS);
#ifndef __RTEMS__
  u32 t = DRVICB_GET_SOURCE_OFFSET(source);
  u32 icbbase = getIcbBase();
  // Before accessing the ICB settings Start Critical Section
  u32 old_value = DRVICB_START_CRITICAL_SECTION();
  SET_REG_WORD(icbbase +  ICB_ENABLE_0_OFFSET  + t, GET_REG_WORD_VAL( icbbase +  ICB_ENABLE_0_OFFSET + t ) & ~( 1 << (source - t*8) ));
  // End of critical section
  DRVICB_END_CRITICAL_SECTION(old_value);
#else
  BSP_Mask_interrupt(source);
#endif
}

void DrvIcbSetIrqHandler( u32 source, irq_handler function ) {
  // Check parameters
  assert((source < MV_TOTAL_LEON_IRQS) && ((u32) function > 0));  
#ifndef __RTEMS__  
  // Before accessing the ICB settings Start Critical Section
  u32 old_value = DRVICB_START_CRITICAL_SECTION();
  if( source < MV_TOTAL_LEON_IRQS ) {
      __irq_table[ source ] = function;
  }
  // End of critical section
  DRVICB_END_CRITICAL_SECTION(old_value);
#else
  BSP_interrupt_register(source, NULL, (rtems_interrupt_handler) function, (void *) source);
  gBSPIntsources[source]=(rtems_interrupt_handler)function;
#endif
}


int DrvIcbIrqPending( u32 source ) {
  // Check parameters
  assert(source < MV_TOTAL_LEON_IRQS);
#ifndef __RTEMS__
  u32 t = DRVICB_GET_SOURCE_OFFSET(source);
  return GET_REG_WORD_VAL( getIcbBase() + ICB_PEND_0_OFFSET + t )&( 1 << (source - t*8) );
#else
  return BSP_Is_interrupt_pending(source);
#endif 
}


void DrvIcbIrqClear( u32 source ) {
  // Check parameters
  assert(source < MV_TOTAL_LEON_IRQS);
#ifndef __RTEMS__
  u32 t = DRVICB_GET_SOURCE_OFFSET(source);
  u32 icbbase = getIcbBase();
  // Before accessing the ICB settings Start Critical Section
  u32 old_value = DRVICB_START_CRITICAL_SECTION();
  SET_REG_WORD( icbbase + ICB_CLEAR_0_OFFSET +  t, 1 << (source - t*8) );
  // End of critical section
  DRVICB_END_CRITICAL_SECTION(old_value);
#else
  BSP_Clear_interrupt(source);
#endif
}

void DrvIcbIrqUnsetup( u32 source )
{
    // Check parameters
    assert(source < MV_TOTAL_LEON_IRQS);
#ifndef __RTEMS__
    #ifdef NDEBUG
        UNUSED(source);
    #endif
#else
    BSP_interrupt_unregister(source,
                             (rtems_interrupt_handler)gBSPIntsources[source],
                             (void*)source);
#endif

}

void DrvIcbIrqTrigger( u32 source ) {
  // Check parameters
  assert(source < MV_TOTAL_LEON_IRQS);
#ifndef __RTEMS__
  u32 t = DRVICB_GET_SOURCE_OFFSET(source);
  u32 icbbase = getIcbBase();
  // Before accessing the ICB settings Start Critical Section
  u32 old_value = DRVICB_START_CRITICAL_SECTION();
  SET_REG_WORD(icbbase + ICB_SETINT_0_OFFSET + t, 1 << (source - t*8) );
  // End of critical section
  DRVICB_END_CRITICAL_SECTION(old_value);
#else
  BSP_Force_interrupt(source);
#endif
}

void DrvIcbIrqRemoteTrigger( u32 source ) 
{
  // Check parameters 
  assert(source < MV_TOTAL_LEON_IRQS);
  u32 t = DRVICB_GET_SOURCE_OFFSET(source);
  // Same as DrvIcbIrqTrigger except that we trigger using the other ICB block. e.g. if we are LOS then we trigger an LRT IRQ
  // Note: This is a hardware atomic operation
  SET_REG_WORD((getIcbBase()==ICB0_BASE_ADR? ICB1_BASE_ADR: ICB0_BASE_ADR) + ICB_SETINT_0_OFFSET + t, 1 << (source - t*8) );
}

u32 DrvIcbIrqSrc( void ) {
    // irq level reg is 2x32
    return GET_REG_WORD_VAL(getIcbBase() +  ICB_SRC_OFFSET ); //0x7F is an invalid source
}

void DrvIcbDynamicIrqConfig(tyDynamicIrqDirection direction, u32 SourceIrq ,u32 targetIrq, u32 bEnableBit)
{
    u32 c2cInterConfigOffset;
    u32 bitOffset;
    u32 configRegisterAddress;
    u32 configValue;
    u32 icbBase;
    // Check parameters 
    assert((targetIrq < MV_TOTAL_LEON_IRQS) && (SourceIrq < MV_TOTAL_LEON_IRQS));
    // There are 12 slot configurations divided across 3 registers with 4 configs in each
    c2cInterConfigOffset = (((targetIrq - IRQ_DYNAMIC_0 ) >> 2) & 0x3) * 4; // Bits 2,3 define the register WORD offset
    bitOffset            = (((targetIrq - IRQ_DYNAMIC_0 ) >> 0) & 0x3) * 8; // Bottom 2 bits of the offset define byte within the 32 bit word

    switch (direction)
    {
    case LOS_TO_LRT:
        icbBase = ICB0_BASE_ADR; // LeonOS ICB base Address  
        break;
    case LRT_TO_LOS:
        icbBase = ICB1_BASE_ADR; // LeonRT ICB base Address  
        break;
    default: 
        icbBase = getIcbBase();
        break;
    }

    configRegisterAddress =  icbBase + ICB_C2C_INT_CFG0_OFFSET + c2cInterConfigOffset;

    configValue = (bEnableBit << IRQ_SRC_ENABLE_BIT_OFFSET) | SourceIrq;


    DrvRegSetBitField((void*)configRegisterAddress,bitOffset,8,configValue);

    return;
}

#undef DRVICB_START_CRITICAL_SECTION
#undef DRVICB_END_CRITICAL_SECTION
#undef DRVICB_GET_SOURCE_OFFSET

/// @}

