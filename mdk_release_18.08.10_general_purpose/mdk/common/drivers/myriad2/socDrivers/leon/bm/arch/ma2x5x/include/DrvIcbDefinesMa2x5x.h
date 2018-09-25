///
/// @file DrvIcbDefinesMa2x5x.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvIcbMa2x5x
/// @{
/// @brief     Definitions and types needed by Interrupt Controller Block
///


#ifndef DRV_ICB_DEF_MA2X5X_H
#define DRV_ICB_DEF_MA2X5X_H 

// There are a total of 64 interrupt Slots on each ICB (LeonOS and LeonRT)
// Each Interrupt source is assigned a number bellow
// Some sources are valid only for LeonRT other only for LeonOS and a small subset are available on both
// e.g. each Leon has its own timer 

//                                           
#define IRQ_WATCHDOG          (  0  )           // LOS & LRT 
#define IRQ_UART              (  1  )           // LOS Only
#define IRQ_REAL_TIME_CLOCK   (  2  )           // LOS Only 

#define IRQ_TIMER             (IRQ_TIMER_1 - 1) // Used so we can add timer number to index
#define IRQ_TIMER_1           (  3  )           // LOS & LRT
#define IRQ_TIMER_2           (  4  )           // LOS & LRT
#define IRQ_TIMER_3           (  5  )           // LOS & LRT
#define IRQ_TIMER_4           (  6  )           // LOS & LRT
#define IRQ_TIMER_5           (  7  )           // LOS & LRT
#define IRQ_TIMER_6           (  8  )           // LOS & LRT
#define IRQ_TIMER_7           (  9  )           // LOS & LRT
#define IRQ_TIMER_8           ( 10  )           // LOS & LRT

#define IRQ_IIC1              ( 11  )           // LOS Only 
#define IRQ_IIC2              ( 12  )           // LOS Only 
#define IRQ_IIC3              ( 13  )           // LOS Only 
                                
#define IRQ_SPI1              ( 14  )           // LOS Only
#define IRQ_SPI2              ( 15  )           // LOS Only
#define IRQ_SPI3              ( 16  )           // LOS Only
                                
#define IRQ_I2S_1             ( 17  )           // LOS Only
#define IRQ_I2S_2             ( 18  )           // LOS Only
#define IRQ_I2S_3             ( 19  )           // LOS Only
                                
#define IRQ_ETH               ( 20  )           // LOS Only
#define IRQ_SDIO              ( 21  )           // LOS Only
#define IRQ_USB               ( 22  )           // LOS Only
#define IRQ_AHB_DMA           ( 23  )           // LOS Only
#define IRQ_LEON4_L2C         ( 24  )           // LOS & LRT
#define IRQ_AHB_DMA_LRT       ( 25  )           // LOS Only
#define IRQ_AON               ( 26  )           // LOS Only
#define IRQ_USB_CTRL          ( 30  )           // LOS Only
#define IRQ_ROIC              ( 31  )           // LOS Only

#define IRQ_CIF0              ( 25  )           // LRT Only
#define IRQ_LCD               ( 26  )           // LRT Only
#define IRQ_NAL               ( 27  )           // LRT Only
#define IRQ_CIF1              ( 28  )           // LRT Only
#define IRQ_AXIM              ( 29  )           // LRT Only

/////////////
// UPA IRQs
/////////////
#define IRQ_SVE_0             (  32 )           // LOS & LRT
#define IRQ_SVE_1             (  33 )           // LOS & LRT
#define IRQ_SVE_2             (  34 )           // LOS & LRT
#define IRQ_SVE_3             (  35 )           // LOS & LRT
#define IRQ_SVE_4             (  36 )           // LOS & LRT
#define IRQ_SVE_5             (  37 )           // LOS & LRT
#define IRQ_SVE_6             (  38 )           // LOS & LRT
#define IRQ_SVE_7             (  39 )           // LOS & LRT
                                
#define IRQ_SVE_8             (  40 )           // LOS & LRT
#define IRQ_SVE_9             (  41 )           // LOS & LRT
#define IRQ_SVE_10            (  42 )           // LOS & LRT
#define IRQ_SVE_11            (  43 )           // LOS & LRT
#define IRQ_CMX               (  44 )           // LOS & LRT
#define IRQ_BICUBIC           (  45 )           // LOS & LRT
#define IRQ_CMXDMA            (  46 )           // LOS & LRT

// IRQ 47 unused                                

#define IRQ_GPIO_0            (  48 )           // LOS Only
#define IRQ_GPIO_1            (  49 )           // LOS Only
#define IRQ_GPIO_2            (  50 )           // LOS Only
#define IRQ_GPIO_3            (  51 )           // LOS Only

#define IRQ_SIPP_0            (  48 )           // LRT Only
#define IRQ_SIPP_1            (  49 )           // LRT Only
#define IRQ_SIPP_2            (  50 )           // LRT Only
#define IRQ_MIPI              (  51 )           // LRT Only

#endif

///@}
