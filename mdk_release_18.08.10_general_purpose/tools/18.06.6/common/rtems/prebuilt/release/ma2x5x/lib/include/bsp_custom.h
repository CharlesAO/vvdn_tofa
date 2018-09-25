
/*  bspCustom.h
 *
 *  MYRIAD2 BSP.
 *  COPYRIGHT (c) 2015.
 *  Movidius.
 */

#ifndef _BSPCUSTOM_H
#define _BSPCUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif
// Default system freq for ma2x5x
#define DEFAULT_SYSTEM_FREQ             600000

//Default reference clock in kHz. This is the default for the Movidius development board.
#define DEFAULT_REFCLOCK                12000

//Map clock definitions
#define MSS_APB_SLV                     MSS_CP_APB_SLV
#define MSS_APB2_CTRL                   MSS_CP_APB2_CTRL
#define MSS_RTBRIDGE                    MSS_CP_RTBRIDGE
#define MSS_RTAHB_CTRL                  MSS_CP_RTAHB_CTRL
#define MSS_LRT                         MSS_CP_LRT
#define MSS_LRT_DSU                     MSS_CP_LRT_DSU
#define MSS_LRT_L2C                     MSS_CP_LRT_L2C
#define MSS_LRT_ICB                     MSS_CP_LRT_ICB
#define MSS_AXI_BRIDGE                  MSS_CP_AXI_BRIDGE
#define MSS_MXI_CTRL                    MSS_CP_MXI_CTRL
#define MSS_MXI_DEFSLV                  MSS_CP_MXI_DEFSLV
#define MSS_AXI_MON                     MSS_CP_AXI_MON
#define MSS_NAL                         MSS_CP_NAL
#define MSS_CIF0                        MSS_CP_CIF0
#define MSS_CIF1                        MSS_CP_CIF1
#define MSS_LCD                         MSS_CP_LCD
#define MSS_TIM                         MSS_CP_TIM
#define MSS_AMC                         MSS_CP_AMC
#define MSS_SPMP                        MSS_CP_SPMP

// CSS Standard Clocks to run RTEMS on LEON OS
#define DEFAULT_RTEMS_CSS_LOS_CLOCKS   (( 1ULL << CSS_LOS               ) | \
                                        ( 1ULL << CSS_LAHB_CTRL         ) | \
                                        ( 1ULL << CSS_APB4_CTRL         ) | \
                                        ( 1ULL << CSS_CPR               ) | \
                                        ( 1ULL << CSS_ROM               ) | \
                                        ( 1ULL << CSS_LOS_L2C           ) | \
                                        ( 1ULL << CSS_MAHB_CTRL         ) | \
                                        ( 1ULL << CSS_LOS_ICB           ) | \
                                        ( 1ULL << CSS_LOS_DSU           ) | \
                                        ( 1ULL << CSS_LOS_TIM           ) | \
                                        ( 1ULL << CSS_JTAG              ) | \
                                        ( 1ULL << CSS_APB1_CTRL         ) | \
                                        ( 1ULL << CSS_APB3_CTRL         ) | \
                                        ( 1ULL << CSS_UART              ) | \
                                        ( 1ULL << CSS_SAHB_CTRL         ) | \
                                        ( 1ULL << CSS_MSS_MAS           ) | \
                                        ( 1ULL << CSS_UPA_MAS           ) | \
                                        ( 1ULL << CSS_DSS_BUS           ) | \
                                        ( 1ULL << CSS_DSS_BUS_MAHB      ) | \
                                        ( 1ULL << CSS_LAHB2SHB          ) | \
                                        ( 1ULL << CSS_SAHB2MAHB         ) )

// MSS Standard Clocks to run RTEMS on LEON RT
#define DEFAULT_RTEMS_MSS_LRT_CLOCKS   ((  1UL << MSS_LRT_DSU      ) | \
                                        (  1UL << MSS_LRT_L2C      ) | \
                                        (  1UL << MSS_LRT_ICB      ) | \
                                        (  1UL << MSS_TIM          ) | \
                                        (  1UL << MSS_RTAHB_CTRL   ) | \
                                        (  1UL << MSS_LRT          ) | \
                                        (  1UL << MSS_RTBRIDGE     ) | \
                                        (  1UL << MSS_APB2_CTRL    ) | \
                                        (  1UL << MSS_APB_SLV      ))

#define L1_MAX_BYTES_INVALIDATION                           (1024)
#define MYRIAD2_LEON_OS_L1_DATA_CACHE_SIZE                  (32)
#define MYRIAD2_LEON_OS_L1_INST_CACHE_SIZE                  (32)

#define MYRIAD2_LEON_OS_L1_DATA_LINE_SIZE                   (32)
#define MYRIAD2_LEON_OS_L1_INSTRUCTION_LINE_SIZE            (32)

#define MYRIAD2_LEON_RT_L1_DATA_CACHE_SIZE                  (4)
#define MYRIAD2_LEON_RT_L1_INST_CACHE_SIZE                  (4)

#define MYRIAD2_LEON_RT_L1_DATA_LINE_SIZE                   (32)
#define MYRIAD2_LEON_RT_L1_INSTRUCTION_LINE_SIZE            (32)

#ifdef __cplusplus
}
#endif

#endif

