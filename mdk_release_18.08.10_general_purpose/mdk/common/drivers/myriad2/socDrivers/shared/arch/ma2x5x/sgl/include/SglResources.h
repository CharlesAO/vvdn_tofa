///
/// @file SglResources.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup SglResources SglResources Driver
/// @{
/// @brief     HW and SW resources definition.
///


#ifndef _SGL_RESOURCES_H_
#define _SGL_RESOURCES_H_

// 1: Defines
// ----------------------------------------------------------------------------

// Number of HW resources of each type for the current architecture.
////////////////////////////////////////////////////////////////////////////////
//                                   / \                                      //
//                                  / ! \                                     //
//                                 /_____\                                    //
//                                                                            //
// For all type of resources managed by ResMgr, the corresponding define must //
// exist here! If a resource type is not present on a given architecture, the //
// corresponding number can be zero. If one wants to add a resource type that //
// will be managed by ResMgr, make sure that the corresponding type is also   //
// properly added in ResMgr code.                                             //
////////////////////////////////////////////////////////////////////////////////
#define SGL_SHAVE_NB              12
#define SGL_HW_FILTER_NB          15
#define SGL_AHB_DMA_CHAN_NB        8
#define SGL_CMX_DMA_CHAN_NB        4
#define SGL_LCD_NB                 1
#define SGL_MIPI_TX_NB             2
#define SGL_I2C_NB                 0
#define SGL_I2S_NB                 0
#define SGL_SPI_NB                 0
#define SGL_SDIO_NB                0
#define SGL_UART_NB                0
#define SGL_GPIO_NB               85
#define SGL_GPIO_IRQ_NB            4
#define SGL_SHAVE_L2_CACHE_NB      1
#define SGL_MUTEX_NB              32

// Maximum number of SHAVE L2 cache partitions for the current architecture.
#define SGL_SHAVE_L2C_PART_NB      8

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

#endif // _SGL_RESOURCES_H_

///@}
