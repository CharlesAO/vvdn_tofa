///
/// @file DrvIcb.h
/// 
/// 
/// @defgroup DrvIcb Icb Driver
/// @{
/// @brief Functionality of the Icb Driver.
///

#ifndef DRV_ICB_H
#define DRV_ICB_H

/**
 * @file DrvIcb.h
 *                MOVIDIA Trap Managing Drivers\n
 *                Copyright 2008, 2009 MOVIDIA Ltd.\n
 *                Header file with description to all functions
 * @brief Public ICB Driver Functions
*/

/*===========================================================================*/
/*                                     Include Headers                       */
/*===========================================================================*/
#include "mv_types.h"
#include "registersMyriad.h"
#include "DrvIcbDefines.h"
#include "swcLeonUtils.h"

/*===========================================================================*/
/*                            Type Declarations                              */
/*===========================================================================*/
/**
 * @typedef *irq_handler
 *        Function type\n
 *        When treating an interrupt one will declare such a function\n
 *        and use DrvIcbSetIrqHandler( u32 source, irq_handler function )\n
 *        to assign it to a specific source
 */

typedef void (*irq_handler)( u32 source ); // function in the rom

typedef enum
{
    LOS_TO_LRT,
    LRT_TO_LOS,
    TO_OTHER_LEON
} tyDynamicIrqDirection;

#ifdef __cplusplus
extern "C" {
#endif


/*===========================================================================*/
/*                                 Functions                                 */
/*===========================================================================*/


/// Setup an IRQ handler
///
/// Performs the steps necessary to enable a Myriad IRQ
/// @param[in] irqSrc (see DrvIcbDefines.h) e.g. IRQ_UART
/// @param[in] priority priority in the range of 1-14 (higher has greater priority)
/// @param[in] type of either POS_EDGE_INT, NEG_EDGE_INT, POS_LEVEL_INT, NEG_LEVEL_INT
/// @param[in] function pointer to irq hanlder (type void funct(u32 src); )
/// @return    void  (asserts on error)
void DrvIcbSetupIrq(u32 irqSrc,u32 priority, u32 type,irq_handler function);

/// Unsetup an IRQ handler
///
/// Removes the previously assigned interrupt handler
/// @param[in] source (see DrvIcbDefines.h) e.g. IRQ_UART
/// @return    void  (asserts on error)
void DrvIcbIrqUnsetup(u32 source);

/**
 * @fn void DrvIcbConfigureIrq( u32 source, u32 priority, u32 type )
 *     Configures the way an irq is generated( eg. edge/level positive/negative )
 * @param source - trap number in the docs Table1\n
 *                 defines for each source are available in DrvIcbDefines.h
 * @param priority - irq priority from 0 to 15( 15 = NMI )
 * @param type     - level or edge see DrvIcbDefines.h
 * @return  - void
 */
void DrvIcbConfigureIrq( u32 source, u32 priority, u32 type );

/**
 * @fn void DrvIcbSetIrqHandler( u32 source, irq_handler function );
 *     Assigns a handler function to a specific hardware irq source
 * @param source - trap number  ( see DrvIcbDefines.h defines )
 * @param function - user declared function following\n
 *                   the irq_handler function pattern\n
 *                   see tc_icb_0 for an example funtion\n
 * Note. User must clear PIL and Interrupt_pending before returning from trap handler.
 */
 void DrvIcbSetIrqHandler( u32 source, irq_handler function );

/**
 * @{
 * @name Enable/Disable Traps
 *  Enable/Disable Traps for a specific Source
 * @param source - trap number ( see DrvIcbDefines.h defines )
 */
void DrvIcbEnableIrq(  u32 source );
void DrvIcbDisableIrq( u32 source );
/**@}
 * @fn void DrvIcbIrqClear( u32 source )
 *     Clears the Pending Flag for the Source IRQ\n
 *     This should be done at the end of every *irq_handler function\n
 *     But only after calling drvIcbClrPsrPil
 * @param source - irq source to get cleared
 */
void DrvIcbIrqClear( u32 source );

/**
 * @fn int DrvIcbIrqPending( u32 source );
 *     Gives the Pending Flag for the Source IRQ\n
 * @param source - irq source
 * @return 1 if the source irq is pending\n
 *         and 0 if it's not
 */
int DrvIcbIrqPending( u32 source );

/**
 * @fn u32 DrvIcbIrqSrc( void );
 *     Gives the Current Interupt Source
 * @return A SEVEN bit value which is the IRQ Source\n
 */
u32 DrvIcbIrqSrc( void );


/**
 * @fn void DrvIcbIrqTrigger( u32 source )
 *     Forces an interrupt to trigger for the Source IRQ\n
 * @param source - irq source to be trigger
 */
void DrvIcbIrqTrigger( u32 source );

/**
 * @fn void DrvIcbIrqRemoteTrigger( u32 source )
 *     Forces an interrupt to trigger for the Source IRQ
 *     on the other Leons ICB block.
 *     If called from LOS it triggers an LRT IRQ
 *     If called from LRT it triggers an LOS IRQ
 *     This function can be used to trigger a software
 *     interrupt on the other leon. For example
 *     the other leon may have configured one of the
 *     dynamic IRQ slots (53-63) for a specific
 *     software interrupt handler. This handler
 *     can then be triggered using this function
 *     Note: This function is safe because the trigger
 *     mechanism is atomic at a hardware level and as such
 *     this does not require mutex protection. Specifically
 *     only a single register write is needed and the hardware performs
 *     the necessary read-modify-write in an atomic manner.
 * @param source - irq source id to trigger
 */
void DrvIcbIrqRemoteTrigger( u32 source );

/// Dynamically route IRQ from one leon to the other
///
/// Each Leon processor has 64 IRQ slots
/// Some irq sources are only directly available on the local Leon
/// e.g. SIPP IRQs go to LeonRT whereas Ethernet IRQs go to LeonOS
/// However there is a provision for a Leon to route a given IRQ to the other Leon
/// IRQ slots 52-63 are reserved for this purpose
/// For example it is possible to route ethernet IRQ to LeonRT slot 52 using the call:
/// DrvIcbDynamicIrqConfig(LOS_TO_LRT,IRQ_ETH,IRQ_DYNAMIC_0,1,0);
///
/// @param[in] tyDynamicIrqDirection - Direction of routed IRQ  {LOS_TO_LRT,LRT_TO_LOS,TO_OTHER_LEON}
/// @param[in] sourceIrq - Source IRQ number
/// @param[in] targetIrq - Source IRQ number (IRQ_DYNAMIC_0-11)
/// @param[in] bEnableBit - Enable Bit for IRQ
/// @return    0 on success, non-zero otherwise
void DrvIcbDynamicIrqConfig(tyDynamicIrqDirection direction, u32 sourceIrq ,u32 targetIrq, u32 bEnableBit);


static inline void drvIcbEnableTraps( void ) __attribute__((deprecated));
static inline void drvIcbEnableTraps( void ) {
    swcLeonEnableTraps();
}

/**
 * @fn static inline void drvIcbSetPil( u32 pil );
 *
 * DO NOT USE: deprecated: use swcLeonSetPIL(pil) instead!
 *
 * Set IRQ Level in the Leon's PSR
 */
static inline void drvIcbSetPil( u32 pil ) __attribute__((deprecated));
static inline void drvIcbSetPil( u32 pil ) {
    swcLeonSetPIL(pil);
}

#ifdef __cplusplus
}
#endif

#endif // DRV_ICB_H

///@}
