
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
// Default system freq for ma2100
#define DEFAULT_SYSTEM_FREQ             480000

//Default reference clock in kHz. This is the default for the Movidius development board.
#define DEFAULT_REFCLOCK                12000

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

