///
/// @file DrvIcbDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvIcb
/// @{
/// @brief     Definitions and types needed by Interrupt Controller Block
///


#ifndef DRV_ICB_DEF_H
#define DRV_ICB_DEF_H 

#include <registersMyriad.h>
#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
 #include "DrvIcbDefinesMa2x5x.h"
#endif

// 1: Defines
// ----------------------------------------------------------------------------

#define NUM_INT_LEVELS              (16)
#define MV_TOTAL_LEON_IRQS          (ICB_INT_NO)

// Interrupt Type Definitions                      
#define POS_EDGE_INT  0
#define NEG_EDGE_INT  1
#define POS_LEVEL_INT 2
#define NEG_LEVEL_INT 3

////////////////////////////////////////////////////////////////
// Using the following 12 IRQ sources it is possible to dynamically
// route IRQs between LeonOS and LeonRT or vise versa. 
// The configuration of this is handled by the ICB driver
// For example it is possible to route the IRQ_ETH from LOS to LRT
// and have it appear on IRQ_DYNAMIC_0
// or route IRQ_MIPI from LRT to LOS on IRQ_DYNAMIC_1
////////////////////////////////////////////////////////////////
// 
#define IRQ_DYNAMIC_0            (  52 )           // LOS & LRT       
#define IRQ_DYNAMIC_1            (  53 )           // LOS & LRT       
#define IRQ_DYNAMIC_2            (  54 )           // LOS & LRT       
#define IRQ_DYNAMIC_3            (  55 )           // LOS & LRT       
#define IRQ_DYNAMIC_4            (  56 )           // LOS & LRT       
#define IRQ_DYNAMIC_5            (  57 )           // LOS & LRT       
#define IRQ_DYNAMIC_6            (  58 )           // LOS & LRT       
#define IRQ_DYNAMIC_7            (  59 )           // LOS & LRT       
#define IRQ_DYNAMIC_8            (  60 )           // LOS & LRT       
#define IRQ_DYNAMIC_9            (  61 )           // LOS & LRT       
#define IRQ_DYNAMIC_10           (  62 )           // LOS & LRT       
#define IRQ_DYNAMIC_11           (  63 )           // LOS & LRT       

#endif // DRV_ICB_DEF_H

///@}
