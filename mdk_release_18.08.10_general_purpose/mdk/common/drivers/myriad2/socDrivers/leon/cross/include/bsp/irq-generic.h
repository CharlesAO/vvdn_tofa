///
/// @file irq-generic.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup irq-generic irq-generic Driver
/// @{
/// @brief     Cross Interrupts
///

#ifndef __IRQ_GENERIC_H__
#define __IRQ_GENERIC_H__

#define rtems_interrupt_disable(status_)														do { \
																									status_ = swcLeonGetPIL(); \
																									swcLeonSetPIL(15); \
																								} while(0);
																								
#define rtems_interrupt_enable(status_)															swcLeonSetPIL(status_);
																																																	
#define BSP_interrupt_register(irq_, info_, isr_, arg_)											OsCrossInterruptRegister(irq_, (irq_handler) isr_)

#define rtems_interrupt_handler_install(irq_, info_, type_, isr_, arg_)							OsCrossInterruptRegister(irq_, (irq_handler) isr_)

#define BSP_interrupt_unregister(irq_, isr_, arg_)												OsCrossInterruptUnregister(irq_)
																								
#define rtems_interrupt_handler_remove(irq_, isr_, arg_)									 	OsCrossInterruptUnregister(irq_)
#define BSP_shared_interrupt_register(irq_, info_, isr_, arg_)									OsCrossInterruptRegister(irq_, (irq_handler) isr_)
#define BSP_shared_interrupt_unregister(irq_, isr_, arg_)										OsCrossInterruptUnregister(irq_)
#define BSP_shared_interrupt_clear(irq_)														DrvIcbIrqClear(irq_)
#define BSP_shared_interrupt_unmask(irq_)														DrvIcbEnableIrq(irq_)
#define BSP_shared_interrupt_mask(irq_)															DrvIcbDisableIrq(irq_)
#define BSP_Clear_interrupt(irq_)																DrvIcbIrqClear(irq_)
#define BSP_Force_interrupt(irq_)																DrvIcbIrqTrigger(irq_)
#define BSP_Is_interrupt_pending(irq_)															DrvIcbIrqPending(irq_)
#define BSP_Unmask_interrupt(irq_)																DrvIcbEnableIrq(irq_)
#define BSP_Mask_interrupt(irq_)																DrvIcbDisableIrq(irq_)
#define BSP_Set_interrupt_type_priority(irq_, type_, priority_)								    DrvIcbConfigureIrq(irq_, priority_, type_)
#define BSP_Dynamic_irq_reroute(source_irq_, target_irq_, enable_)								DrvIcbDynamicIrqConfig(TO_OTHER_LEON, source_irq_ ,target_irq_, enable_)
#define BSP_Force_remote_interrupt(target_irq_)													DrvIcbIrqRemoteTrigger(target_irq_)
#define bsp_interrupt_vector_enable(irq_)														DrvIcbEnableIrq(irq_)
#define bsp_interrupt_vector_disable(irq_)														DrvIcbDisableIrq(irq_)
#endif

///@}
