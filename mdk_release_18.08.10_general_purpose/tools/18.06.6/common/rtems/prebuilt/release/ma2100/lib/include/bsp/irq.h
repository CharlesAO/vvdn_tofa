/**
 * MYRIAD generic shared IRQ setup
 *
 * Based on libbsp/shared/include/irq.h.
 *
 * Modified for MYRIAD2 BSP.
 * COPYRIGHT (c) 2013-2016.
 * Movidius.
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef LIBBSP_MYRIAD_IRQ_CONFIG_H
#define LIBBSP_MYRIAD_IRQ_CONFIG_H

#include <bsp.h>

// Incremented number of vector interrupts so that the last one is used to handle irq source 0x7F
#define BSP_INTERRUPT_VECTOR_MAX_STD MYRIAD_NUM_INT_LEVELS      /* Standard IRQ controller */
#define BSP_INTERRUPT_VECTOR_MAX_EXT (MYRIAD_IRQ_MAX_INDEX + 1) /* Extended IRQ controller */

#define BSP_INTERRUPT_VECTOR_MIN 0
#define BSP_INTERRUPT_VECTOR_MAX BSP_INTERRUPT_VECTOR_MAX_EXT


#endif /* LIBBSP_MYRIAD_IRQ_CONFIG_H */
