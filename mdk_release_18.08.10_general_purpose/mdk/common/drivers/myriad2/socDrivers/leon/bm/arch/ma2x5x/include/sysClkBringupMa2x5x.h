///
/// @file sysClkBringupMa2x5x.h
/// 
/// 
/// @defgroup sysClkBringupMa2x5x sysClkBringupMa2x5x Driver
/// @{
/// @brief API for the sysClkBringup Driver.
///

#ifndef SYS_CLK_BRINGUP_ARCH_H

#define MSS_CLOCKS_BASIC (DEV_MSS_APB_SLV     |     \
                          DEV_MSS_APB2_CTRL   |     \
                          DEV_MSS_RTBRIDGE    |     \
                          DEV_MSS_RTAHB_CTRL  |     \
                          DEV_MSS_LRT         |     \
                          DEV_MSS_LRT_DSU     |     \
                          DEV_MSS_LRT_L2C     |     \
                          DEV_MSS_LRT_ICB     |     \
                          DEV_MSS_AXI_BRIDGE  |     \
                          DEV_MSS_MXI_CTRL    |     \
                          DEV_MSS_MXI_DEFSLV  |     \
                          DEV_MSS_AXI_MON     |     \
                          DEV_MSS_AMC         |     \
                          DEV_MSS_SIPP        |     \
                          DEV_MSS_TIM         ) 

#define BRINGUP_CORE_CSS_DSS_CLOCKS (                   \
                          DEFAULT_CORE_CSS_DSS_CLOCKS | \
                          DEV_CSS_USB                 | \
                          DEV_CSS_USB_APBSLV          )

#endif // SYS_CLK_BRINGUP_ARCH_H

///@}
