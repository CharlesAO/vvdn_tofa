///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved 
///            For License Warranty see: common/license.txt              
///
/// @brief     Definitions and types needed by the WM8325 Power Management Driver API
/// 
/// 
/// 
#ifndef __WM8325DEFINES_H__
#define __WM8325DEFINES_H__


#define WM8325_RESET_ID                         (0x00)
#define WM8325_REVISION                         (0x01)
#define WM8325_PARENT_ID                        (0x4000)
#define WM8325_SYSVDD_CONTROL                   (0x4001)
#define WM8325_THERMAL_MONITORING               (0x4002)
#define WM8325_POWER_STATE                      (0x4003)
#define WM8325_WATCHDOG                         (0x4004)
#define WM8325_ON_PIN_CONTROL                   (0x4005)
#define WM8325_RESET_CONTROL                    (0x4006)
#define WM8325_CONTROL_INTERFACE                (0x4007)
#define WM8325_SECURITY_KEY                     (0x4008)
#define WM8325_SOFTWARE_SCRATCH                 (0x4009)
#define WM8325_OTP_CONTROL                      (0x400A)
#define WM8325_GPIO_LEVEL                       (0x400C)
#define WM8325_SYSTEM_STATUS                    (0x400D)
#define WM8325_ON_SOURCE                        (0x400E)
#define WM8325_OFF_SOURCE                       (0x400F)
#define WM8325_SYSTEM_INTERRUPTS                (0x4010)
#define WM8325_INTERRUPT_STATUS_1               (0x4011)
#define WM8325_INTERRUPT_STATUS_2               (0x4012)
#define WM8325_INTERRUPT_STATUS_3               (0x4013)
#define WM8325_INTERRUPT_STATUS_4               (0x4014)
#define WM8325_INTERRUPT_STATUS_5               (0x4015)
#define WM8325_IRQ_CONFIG                       (0x4017)
#define WM8325_SYSTEM_INTERRUPTS_MASK           (0x4018)
#define WM8325_INTERRUPT_STATUS_1_MASK          (0x4019)
#define WM8325_INTERRUPT_STATUS_2_MASK          (0x401A)
#define WM8325_INTERRUPT_STATUS_3_MASK          (0x401B)
#define WM8325_INTERRUPT_STATUS_4_MASK          (0x401C)
#define WM8325_INTERRUPT_STATUS_5_MASK          (0x401D)
#define WM8325_RTC_WRITE_COUNTER                (0x4020)
#define WM8325_RTC_TIME_1                       (0x4021)
#define WM8325_RTC_TIME_2                       (0x4022)
#define WM8325_RTC_ALARM_1                      (0x4023)
#define WM8325_RTC_ALARM_2                      (0x4024)
#define WM8325_RTC_CONTROL                      (0x4025)
#define WM8325_RTC_TRIM                         (0x4026)
#define WM8325_AUXADC_DATA                      (0x402D)
#define WM8325_AUXADC_CONTROL                   (0x402E)
#define WM8325_AUXADC_SOURCE                    (0x402F)
#define WM8325_COMPARATOR_CONTROL               (0x4030)
#define WM8325_COMPARATOR_1                     (0x4031)
#define WM8325_COMPARATOR_2                     (0x4032)
#define WM8325_COMPARATOR_3                     (0x4033)
#define WM8325_COMPARATOR_4                     (0x4034)
#define WM8325_GPIO1_CONTROL                    (0x4038)
#define WM8325_GPIO2_CONTROL                    (0x4039)
#define WM8325_GPIO3_CONTROL                    (0x403A)
#define WM8325_GPIO4_CONTROL                    (0x403B)
#define WM8325_GPIO5_CONTROL                    (0x403C)
#define WM8325_GPIO6_CONTROL                    (0x403D)
#define WM8325_GPIO7_CONTROL                    (0x403E)
#define WM8325_GPIO8_CONTROL                    (0x403F)
#define WM8325_GPIO9_CONTROL                    (0x4040)
#define WM8325_GPIO10_CONTROL                   (0x4041)
#define WM8325_GPIO11_CONTROL                   (0x4042)
#define WM8325_GPIO12_CONTROL                   (0x4043)
#define WM8325_BACKUP_CHARGER_CONTROL           (0x404B)
#define WM8325_STATUS_LED_1                     (0x404C)
#define WM8325_STATUS_LED_2                     (0x404D)
#define WM8325_DCDC_ENABLE                      (0x4050)
#define WM8325_LDO_ENABLE                       (0x4051)
#define WM8325_DCDC_STATUS                      (0x4052)
#define WM8325_LDO_STATUS                       (0x4053)
#define WM8325_DCDC_UV_STATUS                   (0x4054)
#define WM8325_LDO_UV_STATUS                    (0x4055)
#define WM8325_DC1_CONTROL_1                    (0x4056)
#define WM8325_DC1_CONTROL_2                    (0x4057)
#define WM8325_DC1_ON_CONFIG                    (0x4058)
#define WM8325_DC1_SLEEP_CONTROL                (0x4059)
#define WM8325_DC1_DVS_CONTROL                  (0x405A)
#define WM8325_DC2_CONTROL_1                    (0x405B)
#define WM8325_DC2_CONTROL_2                    (0x405C)
#define WM8325_DC2_ON_CONFIG                    (0x405D)
#define WM8325_DC2_SLEEP_CONTROL                (0x405E)
#define WM8325_DC2_DVS_CONTROL                  (0x405F)
#define WM8325_DC3_CONTROL_1                    (0x4060)
#define WM8325_DC3_CONTROL_2                    (0x4061)
#define WM8325_DC3_ON_CONFIG                    (0x4062)
#define WM8325_DC3_SLEEP_CONTROL                (0x4063)
#define WM8325_DC4_CONTROL_1                    (0x4064)
#define WM8325_DC4_CONTROL_2                    (0x4065)
#define WM8325_DC4_ON_CONFIG                    (0x4066)
#define WM8325_DC4_SLEEP_CONTROL                (0x4067)
#define WM8325_LDO1_CONTROL                     (0x4068)
#define WM8325_LDO1_ON_CONTROL                  (0x4069)
#define WM8325_LDO1_SLEEP_CONTROL               (0x406A)
#define WM8325_LDO2_CONTROL                     (0x406B)
#define WM8325_LDO2_ON_CONTROL                  (0x406C)
#define WM8325_LDO2_SLEEP_CONTROL               (0x406D)
#define WM8325_LDO3_CONTROL                     (0x406E)
#define WM8325_LDO3_ON_CONTROL                  (0x406F)
#define WM8325_LDO3_SLEEP_CONTROL               (0x4070)
#define WM8325_LDO4_CONTROL                     (0x4071)
#define WM8325_LDO4_ON_CONTROL                  (0x4072)
#define WM8325_LDO4_SLEEP_CONTROL               (0x4073)
#define WM8325_LDO5_CONTROL                     (0x4074)
#define WM8325_LDO5_ON_CONTROL                  (0x4075)
#define WM8325_LDO5_SLEEP_CONTROL               (0x4076)
#define WM8325_LDO6_CONTROL                     (0x4077)
#define WM8325_LDO6_ON_CONTROL                  (0x4078)
#define WM8325_LDO6_SLEEP_CONTROL               (0x4079)
#define WM8325_LDO7_CONTROL                     (0x407A)
#define WM8325_LDO7_ON_CONTROL                  (0x407B)
#define WM8325_LDO7_SLEEP_CONTROL               (0x407C)
#define WM8325_LDO8_CONTROL                     (0x407D)
#define WM8325_LDO8_ON_CONTROL                  (0x407E)
#define WM8325_LDO8_SLEEP_CONTROL               (0x407F)
#define WM8325_LDO9_CONTROL                     (0x4080)
#define WM8325_LDO9_ON_CONTROL                  (0x4081)
#define WM8325_LDO9_SLEEP_CONTROL               (0x4082)
#define WM8325_LDO10_CONTROL                    (0x4083)
#define WM8325_LDO10_ON_CONTROL                 (0x4084)
#define WM8325_LDO10_SLEEP_CONTROL              (0x4085)
#define WM8325_LDO11_ON_CONTROL                 (0x4087)
#define WM8325_LDO11_SLEEP_CONTROL              (0x4088)
#define WM8325_EPE1_CONTROL                     (0x4089)
#define WM8325_EPE2_CONTROL                     (0x408A)
#define WM8325_POWER_GOOD_SOURCE_1              (0x408E)
#define WM8325_POWER_GOOD_SOURCE_2              (0x408F)
#define WM8325_CLOCK_CONTROL_1                  (0x4090)
#define WM8325_CLOCK_CONTROL_2                  (0x4091)
#define WM8325_UNIQUE_ID_1                      (0x7800)
#define WM8325_UNIQUE_ID_2                      (0x7801)
#define WM8325_UNIQUE_ID_3                      (0x7802)
#define WM8325_UNIQUE_ID_4                      (0x7803)
#define WM8325_UNIQUE_ID_5                      (0x7804)
#define WM8325_UNIQUE_ID_6                      (0x7805)
#define WM8325_UNIQUE_ID_7                      (0x7806)
#define WM8325_UNIQUE_ID_8                      (0x7807)
#define WM8325_CUSTOMER_OTP_ID                  (0x7810)
#define WM8325_DC1_OTP_CONTROL                  (0x7811)
#define WM8325_DC2_OTP_CONTROL                  (0x7812)
#define WM8325_DC3_OTP_CONTROL                  (0x7813)
#define WM8325_LDO1_2_OTP_CONTROL               (0x7814)
#define WM8325_LDO3_4_OTP_CONTROL               (0x7815)
#define WM8325_LDO5_6_OTP_CONTROL               (0x7816)
#define WM8325_LDO7_8_OTP_CONTROL               (0x7817)
#define WM8325_LDO9_10_OTP_CONTROL              (0x7818)
#define WM8325_LDO11_EPE_CONTROL                (0x7819)
#define WM8325_GPIO1_OTP_CONTROL                (0x781A)
#define WM8325_GPIO2_OTP_CONTROL                (0x781B)
#define WM8325_GPIO3_OTP_CONTROL                (0x781C)
#define WM8325_GPIO4_OTP_CONTROL                (0x781D)
#define WM8325_GPIO5_OTP_CONTROL                (0x781E)
#define WM8325_GPIO6_OTP_CONTROL                (0x781F)
#define WM8325_ICE_CHECK_DATA                   (0x7827)
                                                
#define WM8325_REGISTER_COUNT                      (147)
#define WM8325_MAX_REGISTER                     (0x7827)
                                              
                                              
/*                                                                                                        
 * Field Definitions.                                                                               
 */                                            
                                              
/*                                                                                                        
 * R0 (0x00) - Reset ID                                                                         
 */                                            
#define WM8325_CHIP_ID_MASK                     (0xFFFF)  /* CHIP_ID - [15:0] */
#define WM8325_CHIP_ID_SHIFT                    	 (0)  /* CHIP_ID - [15:0] */
#define WM8325_CHIP_ID_WIDTH                    	(16)  /* CHIP_ID - [15:0] */
                                             
/*                                                                                                      
 * R1 (0x01) - Revision                                                                        
 */                                           
#define WM8325_PARENT_REV_MASK                  (0xFF00)  /* PARENT_REV - [15:8] */
#define WM8325_PARENT_REV_SHIFT                 	(8)  /* PARENT_REV - [15:8] */
#define WM8325_PARENT_REV_WIDTH                 	(8)  /* PARENT_REV - [15:8] */
#define WM8325_CHILD_REV_MASK                   (0x00FF)  /* CHILD_REV - [7:0] */
#define WM8325_CHILD_REV_SHIFT                  	(0)  /* CHILD_REV - [7:0] */
#define WM8325_CHILD_REV_WIDTH                  	(8)  /* CHILD_REV - [7:0] */
                                               
/*                                                                                                          
 * R16384 (0x4000) - Parent ID                                                              
 */                                             
#define WM8325_PARENT_ID_MASK                   (0xFFFF)  /* PARENT_ID - [15:0] */
#define WM8325_PARENT_ID_SHIFT                  	(0)  /* PARENT_ID - [15:0] */
#define WM8325_PARENT_ID_WIDTH                  	(16)  /* PARENT_ID - [15:0] */
                                                
/*                                                                                                            
 * R16385 (0x4001) - SYSVDD Control                                                       
 */                                              
#define WM8325_SYSLO_ERR_ACT_MASK               (0xC000)  /* SYSLO_ERR_ACT - [15:14] */
#define WM8325_SYSLO_ERR_ACT_SHIFT              (    14)  /* SYSLO_ERR_ACT - [15:14] */
#define WM8325_SYSLO_ERR_ACT_WIDTH              (     2)  /* SYSLO_ERR_ACT - [15:14] */
#define WM8325_SYSLO_STS                        (0x0800)  /* SYSLO_STS */
#define WM8325_SYSLO_STS_MASK                   (0x0800)  /* SYSLO_STS */
#define WM8325_SYSLO_STS_SHIFT                  	(11)  /* SYSLO_STS */
#define WM8325_SYSLO_STS_WIDTH                  	(1)  /* SYSLO_STS */
#define WM8325_SYSLO_THR_MASK                   (0x0070)  /* SYSLO_THR - [6:4] */
#define WM8325_SYSLO_THR_SHIFT                  	(4)  /* SYSLO_THR - [6:4] */
#define WM8325_SYSLO_THR_WIDTH                  	(3)  /* SYSLO_THR - [6:4] */
#define WM8325_SYSOK_THR_MASK                   (0x0007)  /* SYSOK_THR - [2:0] */
#define WM8325_SYSOK_THR_SHIFT                  	(0)  /* SYSOK_THR - [2:0] */
#define WM8325_SYSOK_THR_WIDTH                  	(3)  /* SYSOK_THR - [2:0] */
                                               
/*                                                                                                          
 * R16386 (0x4002) - Thermal Monitoring                                                 
 */                                             
#define WM8325_THW_HYST                         (0x0008)  /* THW_HYST */
#define WM8325_THW_HYST_MASK                    (0x0008)  /* THW_HYST */
#define WM8325_THW_HYST_SHIFT                   	(3)  /* THW_HYST */
#define WM8325_THW_HYST_WIDTH                   	(1)  /* THW_HYST */
#define WM8325_THW_TEMP_MASK                    (0x0003)  /* THW_TEMP - [1:0] */
#define WM8325_THW_TEMP_SHIFT                   	(0)  /* THW_TEMP - [1:0] */
#define WM8325_THW_TEMP_WIDTH                   	(2)  /* THW_TEMP - [1:0] */
                                               
/*                                                                                                          
 * R16387 (0x4003) - Power State                                                           
 */                                             
#define WM8325_CHIP_ON                          (0x8000)  /* CHIP_ON */
#define WM8325_CHIP_ON_MASK                     (0x8000)  /* CHIP_ON */
#define WM8325_CHIP_ON_SHIFT                    (    15)  /* CHIP_ON */
#define WM8325_CHIP_ON_WIDTH                    (     1)  /* CHIP_ON */
#define WM8325_CHIP_SLP                         (0x4000)  /* CHIP_SLP */
#define WM8325_CHIP_SLP_MASK                    (0x4000)  /* CHIP_SLP */
#define WM8325_CHIP_SLP_SHIFT                   (    14)  /* CHIP_SLP */
#define WM8325_CHIP_SLP_WIDTH                   (     1)  /* CHIP_SLP */
#define WM8325_REF_LP                           (0x1000)  /* REF_LP */
#define WM8325_REF_LP_MASK                      (0x1000)  /* REF_LP */
#define WM8325_REF_LP_SHIFT                     (    12)  /* REF_LP */
#define WM8325_REF_LP_WIDTH                     (     1)  /* REF_LP */
#define WM8325_PWRSTATE_DLY_MASK                (0x0C00)  /* PWRSTATE_DLY - [11:10] */
#define WM8325_PWRSTATE_DLY_SHIFT               (    10)  /* PWRSTATE_DLY - [11:10] */
#define WM8325_PWRSTATE_DLY_WIDTH               (     2)  /* PWRSTATE_DLY - [11:10] */
#define WM8325_SWRST_DLY                        (0x0200)  /* SWRST_DLY */
#define WM8325_SWRST_DLY_MASK                   (0x0200)  /* SWRST_DLY */
#define WM8325_SWRST_DLY_SHIFT                  (     9)  /* SWRST_DLY */
#define WM8325_SWRST_DLY_WIDTH                  (     1)  /* SWRST_DLY */
                                               
/*                                                                                                          
 * R16388 (0x4004) - Watchdog                                                              
 */                                             
#define WM8325_WDOG_ENA                         (0x8000)  /* WDOG_ENA */
#define WM8325_WDOG_ENA_MASK                    (0x8000)  /* WDOG_ENA */
#define WM8325_WDOG_ENA_SHIFT                   (    15)  /* WDOG_ENA */
#define WM8325_WDOG_ENA_WIDTH                   (     1)  /* WDOG_ENA */
#define WM8325_WDOG_DEBUG                       (0x4000)  /* WDOG_DEBUG */
#define WM8325_WDOG_DEBUG_MASK                  (0x4000)  /* WDOG_DEBUG */
#define WM8325_WDOG_DEBUG_SHIFT                 (    14)  /* WDOG_DEBUG */
#define WM8325_WDOG_DEBUG_WIDTH                 (     1)  /* WDOG_DEBUG */
#define WM8325_WDOG_RST_SRC                     (0x2000)  /* WDOG_RST_SRC */
#define WM8325_WDOG_RST_SRC_MASK                (0x2000)  /* WDOG_RST_SRC */
#define WM8325_WDOG_RST_SRC_SHIFT               (    13)  /* WDOG_RST_SRC */
#define WM8325_WDOG_RST_SRC_WIDTH               (     1)  /* WDOG_RST_SRC */
#define WM8325_WDOG_SLPENA                      (0x1000)  /* WDOG_SLPENA */
#define WM8325_WDOG_SLPENA_MASK                 (0x1000)  /* WDOG_SLPENA */
#define WM8325_WDOG_SLPENA_SHIFT                (    12)  /* WDOG_SLPENA */
#define WM8325_WDOG_SLPENA_WIDTH                (     1)  /* WDOG_SLPENA */
#define WM8325_WDOG_RESET                       (0x0800)  /* WDOG_RESET */
#define WM8325_WDOG_RESET_MASK                  (0x0800)  /* WDOG_RESET */
#define WM8325_WDOG_RESET_SHIFT                 (    11)  /* WDOG_RESET */
#define WM8325_WDOG_RESET_WIDTH                 (     1)  /* WDOG_RESET */
#define WM8325_WDOG_SECACT_MASK                 (0x0300)  /* WDOG_SECACT - [9:8] */
#define WM8325_WDOG_SECACT_SHIFT                (     8)  /* WDOG_SECACT - [9:8] */
#define WM8325_WDOG_SECACT_WIDTH                (     2)  /* WDOG_SECACT - [9:8] */
#define WM8325_WDOG_PRIMACT_MASK                (0x0030)  /* WDOG_PRIMACT - [5:4] */
#define WM8325_WDOG_PRIMACT_SHIFT               (     4)  /* WDOG_PRIMACT - [5:4] */
#define WM8325_WDOG_PRIMACT_WIDTH               (     2)  /* WDOG_PRIMACT - [5:4] */
#define WM8325_WDOG_TO_MASK                     (0x0007)  /* WDOG_TO - [2:0] */
#define WM8325_WDOG_TO_SHIFT                    (     0)  /* WDOG_TO - [2:0] */
#define WM8325_WDOG_TO_WIDTH                    (     3)  /* WDOG_TO - [2:0] */
                                              
/*                                                                                                        
 * R16389 (0x4005) - ON Pin Control                                                     
 */                                            
#define WM8325_ON_PIN_SECACT_MASK               (0x0300)  /* ON_PIN_SECACT - [9:8] */
#define WM8325_ON_PIN_SECACT_SHIFT              (     8)  /* ON_PIN_SECACT - [9:8] */
#define WM8325_ON_PIN_SECACT_WIDTH              (     2)  /* ON_PIN_SECACT - [9:8] */
#define WM8325_ON_PIN_PRIMACT_MASK              (0x0030)  /* ON_PIN_PRIMACT - [5:4] */
#define WM8325_ON_PIN_PRIMACT_SHIFT             (     4)  /* ON_PIN_PRIMACT - [5:4] */
#define WM8325_ON_PIN_PRIMACT_WIDTH             (     2)  /* ON_PIN_PRIMACT - [5:4] */
#define WM8325_ON_PIN_STS                       (0x0008)  /* ON_PIN_STS */
#define WM8325_ON_PIN_STS_MASK                  (0x0008)  /* ON_PIN_STS */
#define WM8325_ON_PIN_STS_SHIFT                 (     3)  /* ON_PIN_STS */
#define WM8325_ON_PIN_STS_WIDTH                 (     1)  /* ON_PIN_STS */
#define WM8325_ON_PIN_TO_MASK                   (0x0003)  /* ON_PIN_TO - [1:0] */
#define WM8325_ON_PIN_TO_SHIFT                  (     0)  /* ON_PIN_TO - [1:0] */
#define WM8325_ON_PIN_TO_WIDTH                  (     2)  /* ON_PIN_TO - [1:0] */
                                               
/*                                                                                                          
 * R16390 (0x4006) - Reset Control                                                         
 */                                             
#define WM8325_RECONFIG_AT_ON                   (0x8000)  /* RECONFIG_AT_ON */
#define WM8325_RECONFIG_AT_ON_MASK              (0x8000)  /* RECONFIG_AT_ON */
#define WM8325_RECONFIG_AT_ON_SHIFT             (    15)  /* RECONFIG_AT_ON */
#define WM8325_RECONFIG_AT_ON_WIDTH             (     1)  /* RECONFIG_AT_ON */
#define WM8325_SW_RESET_CFG                     (0x0400)  /* SW_RESET_CFG */
#define WM8325_SW_RESET_CFG_MASK                (0x0400)  /* SW_RESET_CFG */
#define WM8325_SW_RESET_CFG_SHIFT               (    10)  /* SW_RESET_CFG */
#define WM8325_SW_RESET_CFG_WIDTH               (     1)  /* SW_RESET_CFG */
#define WM8325_AUXRST_SLPENA                    (0x0040)  /* AUXRST_SLPENA */
#define WM8325_AUXRST_SLPENA_MASK               (0x0040)  /* AUXRST_SLPENA */
#define WM8325_AUXRST_SLPENA_SHIFT              (     6)  /* AUXRST_SLPENA */
#define WM8325_AUXRST_SLPENA_WIDTH              (     1)  /* AUXRST_SLPENA */
#define WM8325_RST_SLP_MSK                      (0x0020)  /* RST_SLP_MSK */
#define WM8325_RST_SLP_MSK_MASK                 (0x0020)  /* RST_SLP_MSK */
#define WM8325_RST_SLP_MSK_SHIFT                (     5)  /* RST_SLP_MSK */
#define WM8325_RST_SLP_MSK_WIDTH                (     1)  /* RST_SLP_MSK */
#define WM8325_RST_SLPENA                       (0x0010)  /* RST_SLPENA */
#define WM8325_RST_SLPENA_MASK                  (0x0010)  /* RST_SLPENA */
#define WM8325_RST_SLPENA_SHIFT                 (     4)  /* RST_SLPENA */
#define WM8325_RST_SLPENA_WIDTH                 (     1)  /* RST_SLPENA */
#define WM8325_RST_DUR_MASK                     (0x0003)  /* RST_DUR - [1:0] */
#define WM8325_RST_DUR_SHIFT                    (     0)  /* RST_DUR - [1:0] */
#define WM8325_RST_DUR_WIDTH                    (     2)  /* RST_DUR - [1:0] */
                                               
/*                                                                                                          
 * R16391 (0x4007) - Control Interface                                                    
 */                                             
#define WM8325_AUTOINC                          (0x0004)  /* AUTOINC */
#define WM8325_AUTOINC_MASK                     (0x0004)  /* AUTOINC */
#define WM8325_AUTOINC_SHIFT                    (     2)  /* AUTOINC */
#define WM8325_AUTOINC_WIDTH                    (     1)  /* AUTOINC */
                                               
/*                                                                                                          
 * R16392 (0x4008) - Security Key                                                          
 */                                                                                                          
#define WM8325_SECURITY_MASK                    (0xFFFF)  /* SECURITY - [15:0] */
#define WM8325_SECURITY_SHIFT                   (     0)  /* SECURITY - [15:0] */
#define WM8325_SECURITY_WIDTH                   (    16)  /* SECURITY - [15:0] */
                                               
/*                                                                                                          
 * R16393 (0x4009) - Software Scratch                                                   
 */                                                                                                          
#define WM8325_SW_SCRATCH_MASK                  (0xFFFF)  /* SW_SCRATCH - [15:0] */
#define WM8325_SW_SCRATCH_SHIFT                 (     0)  /* SW_SCRATCH - [15:0] */
#define WM8325_SW_SCRATCH_WIDTH                 (    16)  /* SW_SCRATCH - [15:0] */
                                              
/*                                                                                                         
 * R16394 (0x400A) - OTP Control                                                          
 */                                            
#define WM8325_OTP_PROG                         (0x8000)  /* OTP_PROG */
#define WM8325_OTP_PROG_MASK                    (0x8000)  /* OTP_PROG */
#define WM8325_OTP_PROG_SHIFT                   (    15)  /* OTP_PROG */
#define WM8325_OTP_PROG_WIDTH                   (     1)  /* OTP_PROG */
#define WM8325_OTP_MEM                          (0x2000)  /* OTP_MEM */
#define WM8325_OTP_MEM_MASK                     (0x2000)  /* OTP_MEM */
#define WM8325_OTP_MEM_SHIFT                    (    13)  /* OTP_MEM */
#define WM8325_OTP_MEM_WIDTH                    (     1)  /* OTP_MEM */
#define WM8325_OTP_FINAL                        (0x0800)  /* OTP_FINAL */
#define WM8325_OTP_FINAL_MASK                   (0x0800)  /* OTP_FINAL */
#define WM8325_OTP_FINAL_SHIFT                  (    11)  /* OTP_FINAL */
#define WM8325_OTP_FINAL_WIDTH                  (     1)  /* OTP_FINAL */
#define WM8325_OTP_VERIFY                       (0x0400)  /* OTP_VERIFY */
#define WM8325_OTP_VERIFY_MASK                  (0x0400)  /* OTP_VERIFY */
#define WM8325_OTP_VERIFY_SHIFT                 (    10)  /* OTP_VERIFY */
#define WM8325_OTP_VERIFY_WIDTH                 (     1)  /* OTP_VERIFY */
#define WM8325_OTP_WRITE                        (0x0200)  /* OTP_WRITE */
#define WM8325_OTP_WRITE_MASK                   (0x0200)  /* OTP_WRITE */
#define WM8325_OTP_WRITE_SHIFT                  (     9)  /* OTP_WRITE */
#define WM8325_OTP_WRITE_WIDTH                  (     1)  /* OTP_WRITE */
#define WM8325_OTP_READ                         (0x0100)  /* OTP_READ */
#define WM8325_OTP_READ_MASK                    (0x0100)  /* OTP_READ */
#define WM8325_OTP_READ_SHIFT                   (     8)  /* OTP_READ */
#define WM8325_OTP_READ_WIDTH                   (     1)  /* OTP_READ */
#define WM8325_OTP_READ_LVL_MASK                (0x00C0)  /* OTP_READ_LVL - [7:6] */
#define WM8325_OTP_READ_LVL_SHIFT               (     6)  /* OTP_READ_LVL - [7:6] */
#define WM8325_OTP_READ_LVL_WIDTH               (     2)  /* OTP_READ_LVL - [7:6] */
#define WM8325_OTP_BULK                         (0x0020)  /* OTP_BULK */
#define WM8325_OTP_BULK_MASK                    (0x0020)  /* OTP_BULK */
#define WM8325_OTP_BULK_SHIFT                   (     5)  /* OTP_BULK */
#define WM8325_OTP_BULK_WIDTH                   (     1)  /* OTP_BULK */
#define WM8325_OTP_PAGE_MASK                    (0x0003)  /* OTP_PAGE - [1:0] */
#define WM8325_OTP_PAGE_SHIFT                   (     0)  /* OTP_PAGE - [1:0] */
#define WM8325_OTP_PAGE_WIDTH                   (     2)  /* OTP_PAGE - [1:0] */
                                               
/*                                                                                                          
 * R16396 (0x400C) - GPIO Level                                                             
 */                                             
#define WM8325_GP12_LVL                         (0x0800)  /* GP12_LVL */
#define WM8325_GP12_LVL_MASK                    (0x0800)  /* GP12_LVL */
#define WM8325_GP12_LVL_SHIFT                   (    11)  /* GP12_LVL */
#define WM8325_GP12_LVL_WIDTH                   (     1)  /* GP12_LVL */
#define WM8325_GP11_LVL                         (0x0400)  /* GP11_LVL */
#define WM8325_GP11_LVL_MASK                    (0x0400)  /* GP11_LVL */
#define WM8325_GP11_LVL_SHIFT                   (    10)  /* GP11_LVL */
#define WM8325_GP11_LVL_WIDTH                   (     1)  /* GP11_LVL */
#define WM8325_GP10_LVL                         (0x0200)  /* GP10_LVL */
#define WM8325_GP10_LVL_MASK                    (0x0200)  /* GP10_LVL */
#define WM8325_GP10_LVL_SHIFT                   (     9)  /* GP10_LVL */
#define WM8325_GP10_LVL_WIDTH                   (     1)  /* GP10_LVL */
#define WM8325_GP9_LVL                          (0x0100)  /* GP9_LVL */
#define WM8325_GP9_LVL_MASK                     (0x0100)  /* GP9_LVL */
#define WM8325_GP9_LVL_SHIFT                    (     8)  /* GP9_LVL */
#define WM8325_GP9_LVL_WIDTH                    (     1)  /* GP9_LVL */
#define WM8325_GP8_LVL                          (0x0080)  /* GP8_LVL */
#define WM8325_GP8_LVL_MASK                     (0x0080)  /* GP8_LVL */
#define WM8325_GP8_LVL_SHIFT                    (     7)  /* GP8_LVL */
#define WM8325_GP8_LVL_WIDTH                    (     1)  /* GP8_LVL */
#define WM8325_GP7_LVL                          (0x0040)  /* GP7_LVL */
#define WM8325_GP7_LVL_MASK                     (0x0040)  /* GP7_LVL */
#define WM8325_GP7_LVL_SHIFT                    (     6)  /* GP7_LVL */
#define WM8325_GP7_LVL_WIDTH                    (     1)  /* GP7_LVL */
#define WM8325_GP6_LVL                          (0x0020)  /* GP6_LVL */
#define WM8325_GP6_LVL_MASK                     (0x0020)  /* GP6_LVL */
#define WM8325_GP6_LVL_SHIFT                    (     5)  /* GP6_LVL */
#define WM8325_GP6_LVL_WIDTH                    (     1)  /* GP6_LVL */
#define WM8325_GP5_LVL                          (0x0010)  /* GP5_LVL */
#define WM8325_GP5_LVL_MASK                     (0x0010)  /* GP5_LVL */
#define WM8325_GP5_LVL_SHIFT                    (     4)  /* GP5_LVL */
#define WM8325_GP5_LVL_WIDTH                    (     1)  /* GP5_LVL */
#define WM8325_GP4_LVL                          (0x0008)  /* GP4_LVL */
#define WM8325_GP4_LVL_MASK                     (0x0008)  /* GP4_LVL */
#define WM8325_GP4_LVL_SHIFT                    (     3)  /* GP4_LVL */
#define WM8325_GP4_LVL_WIDTH                    (     1)  /* GP4_LVL */
#define WM8325_GP3_LVL                          (0x0004)  /* GP3_LVL */
#define WM8325_GP3_LVL_MASK                     (0x0004)  /* GP3_LVL */
#define WM8325_GP3_LVL_SHIFT                    (     2)  /* GP3_LVL */
#define WM8325_GP3_LVL_WIDTH                    (     1)  /* GP3_LVL */
#define WM8325_GP2_LVL                          (0x0002)  /* GP2_LVL */
#define WM8325_GP2_LVL_MASK                     (0x0002)  /* GP2_LVL */
#define WM8325_GP2_LVL_SHIFT                    (     1)  /* GP2_LVL */
#define WM8325_GP2_LVL_WIDTH                    (     1)  /* GP2_LVL */
#define WM8325_GP1_LVL                          (0x0001)  /* GP1_LVL */
#define WM8325_GP1_LVL_MASK                     (0x0001)  /* GP1_LVL */
#define WM8325_GP1_LVL_SHIFT                    (     0)  /* GP1_LVL */
#define WM8325_GP1_LVL_WIDTH                    (     1)  /* GP1_LVL */
                                             
/*                                                                                                      
 * R16397 (0x400D) - System Status                                                   
 */                                           
#define WM8325_THW_STS                          (0x8000)  /* THW_STS */
#define WM8325_THW_STS_MASK                     (0x8000)  /* THW_STS */
#define WM8325_THW_STS_SHIFT                    (    15)  /* THW_STS */
#define WM8325_THW_STS_WIDTH                    (     1)  /* THW_STS */
#define WM8325_XTAL_OK                          (0x0080)  /* XTAL_OK */
#define WM8325_XTAL_OK_MASK                     (0x0080)  /* XTAL_OK */
#define WM8325_XTAL_OK_SHIFT                    (     7)  /* XTAL_OK */
#define WM8325_XTAL_OK_WIDTH                    (     1)  /* XTAL_OK */
#define WM8325_MAIN_STATE_MASK                  (0x001F)  /* MAIN_STATE - [4:0] */
#define WM8325_MAIN_STATE_SHIFT                 (     0)  /* MAIN_STATE - [4:0] */
#define WM8325_MAIN_STATE_WIDTH                 (     5)  /* MAIN_STATE - [4:0] */
                                              
/*                                                                                                        
 * R16398 (0x400E) - ON Source                                                          
 */                                            
#define WM8325_ON_TRANS                         (0x8000)  /* ON_TRANS */
#define WM8325_ON_TRANS_MASK                    (0x8000)  /* ON_TRANS */
#define WM8325_ON_TRANS_SHIFT                   (    15)  /* ON_TRANS */
#define WM8325_ON_TRANS_WIDTH                   (     1)  /* ON_TRANS */
#define WM8325_ON_GPIO                          (0x0800)  /* ON_GPIO */
#define WM8325_ON_GPIO_MASK                     (0x0800)  /* ON_GPIO */
#define WM8325_ON_GPIO_SHIFT                    (    11)  /* ON_GPIO */
#define WM8325_ON_GPIO_WIDTH                    (     1)  /* ON_GPIO */
#define WM8325_ON_SYSLO                         (0x0400)  /* ON_SYSLO */
#define WM8325_ON_SYSLO_MASK                    (0x0400)  /* ON_SYSLO */
#define WM8325_ON_SYSLO_SHIFT                   (    10)  /* ON_SYSLO */
#define WM8325_ON_SYSLO_WIDTH                   (     1)  /* ON_SYSLO */
#define WM8325_ON_WDOG_TO                       (0x0080)  /* ON_WDOG_TO */
#define WM8325_ON_WDOG_TO_MASK                  (0x0080)  /* ON_WDOG_TO */
#define WM8325_ON_WDOG_TO_SHIFT                 (     7)  /* ON_WDOG_TO */
#define WM8325_ON_WDOG_TO_WIDTH                 (     1)  /* ON_WDOG_TO */
#define WM8325_ON_SW_REQ                        (0x0040)  /* ON_SW_REQ */
#define WM8325_ON_SW_REQ_MASK                   (0x0040)  /* ON_SW_REQ */
#define WM8325_ON_SW_REQ_SHIFT                  (     6)  /* ON_SW_REQ */
#define WM8325_ON_SW_REQ_WIDTH                  (     1)  /* ON_SW_REQ */
#define WM8325_ON_RTC_ALM                       (0x0020)  /* ON_RTC_ALM */
#define WM8325_ON_RTC_ALM_MASK                  (0x0020)  /* ON_RTC_ALM */
#define WM8325_ON_RTC_ALM_SHIFT                 (     5)  /* ON_RTC_ALM */
#define WM8325_ON_RTC_ALM_WIDTH                 (     1)  /* ON_RTC_ALM */
#define WM8325_ON_ON_PIN                        (0x0010)  /* ON_ON_PIN */
#define WM8325_ON_ON_PIN_MASK                   (0x0010)  /* ON_ON_PIN */
#define WM8325_ON_ON_PIN_SHIFT                  (     4)  /* ON_ON_PIN */
#define WM8325_ON_ON_PIN_WIDTH                  (     1)  /* ON_ON_PIN */
#define WM8325_RESET_CNV_UV                     (0x0008)  /* RESET_CNV_UV */
#define WM8325_RESET_CNV_UV_MASK                (0x0008)  /* RESET_CNV_UV */
#define WM8325_RESET_CNV_UV_SHIFT               (     3)  /* RESET_CNV_UV */
#define WM8325_RESET_CNV_UV_WIDTH               (     1)  /* RESET_CNV_UV */
#define WM8325_RESET_SW                         (0x0004)  /* RESET_SW */
#define WM8325_RESET_SW_MASK                    (0x0004)  /* RESET_SW */
#define WM8325_RESET_SW_SHIFT                   (     2)  /* RESET_SW */
#define WM8325_RESET_SW_WIDTH                   (     1)  /* RESET_SW */
#define WM8325_RESET_HW                         (0x0002)  /* RESET_HW */
#define WM8325_RESET_HW_MASK                    (0x0002)  /* RESET_HW */
#define WM8325_RESET_HW_SHIFT                   (     1)  /* RESET_HW */
#define WM8325_RESET_HW_WIDTH                   (     1)  /* RESET_HW */
#define WM8325_RESET_WDOG                       (0x0001)  /* RESET_WDOG */
#define WM8325_RESET_WDOG_MASK                  (0x0001)  /* RESET_WDOG */
#define WM8325_RESET_WDOG_SHIFT                 (     0)  /* RESET_WDOG */
#define WM8325_RESET_WDOG_WIDTH                 (     1)  /* RESET_WDOG */
                                            
/*                                                                                                   
 * R16399 (0x400F) - OFF Source                                                     
 */                                          
#define WM8325_OFF_INTLDO_ERR                   (0x2000)  /* OFF_INTLDO_ERR */
#define WM8325_OFF_INTLDO_ERR_MASK              (0x2000)  /* OFF_INTLDO_ERR */
#define WM8325_OFF_INTLDO_ERR_SHIFT             (    13)  /* OFF_INTLDO_ERR */
#define WM8325_OFF_INTLDO_ERR_WIDTH             (     1)  /* OFF_INTLDO_ERR */
#define WM8325_OFF_PWR_SEQ                      (0x1000)  /* OFF_PWR_SEQ */
#define WM8325_OFF_PWR_SEQ_MASK                 (0x1000)  /* OFF_PWR_SEQ */
#define WM8325_OFF_PWR_SEQ_SHIFT                (    12)  /* OFF_PWR_SEQ */
#define WM8325_OFF_PWR_SEQ_WIDTH                (     1)  /* OFF_PWR_SEQ */
#define WM8325_OFF_GPIO                         (0x0800)  /* OFF_GPIO */
#define WM8325_OFF_GPIO_MASK                    (0x0800)  /* OFF_GPIO */
#define WM8325_OFF_GPIO_SHIFT                   (    11)  /* OFF_GPIO */
#define WM8325_OFF_GPIO_WIDTH                   (     1)  /* OFF_GPIO */
#define WM8325_OFF_PVDD                         (0x0400)  /* OFF_PVDD */
#define WM8325_OFF_PVDD_MASK                    (0x0400)  /* OFF_PVDD */
#define WM8325_OFF_PVDD_SHIFT                   (    10)  /* OFF_PVDD */
#define WM8325_OFF_PVDD_WIDTH                   (     1)  /* OFF_PVDD */
#define WM8325_OFF_THERR                        (0x0200)  /* OFF_THERR */
#define WM8325_OFF_THERR_MASK                   (0x0200)  /* OFF_THERR */
#define WM8325_OFF_THERR_SHIFT                  (     9)  /* OFF_THERR */
#define WM8325_OFF_THERR_WIDTH                  (     1)  /* OFF_THERR */
#define WM8325_OFF_SW_REQ                       (0x0040)  /* OFF_SW_REQ */
#define WM8325_OFF_SW_REQ_MASK                  (0x0040)  /* OFF_SW_REQ */
#define WM8325_OFF_SW_REQ_SHIFT                 (     6)  /* OFF_SW_REQ */
#define WM8325_OFF_SW_REQ_WIDTH                 (     1)  /* OFF_SW_REQ */
#define WM8325_OFF_ON_PIN                       (0x0010)  /* OFF_ON_PIN */
#define WM8325_OFF_ON_PIN_MASK                  (0x0010)  /* OFF_ON_PIN */
#define WM8325_OFF_ON_PIN_SHIFT                 (     4)  /* OFF_ON_PIN */
#define WM8325_OFF_ON_PIN_WIDTH                 (     1)  /* OFF_ON_PIN */
                                             
/*                                                                                                      
 * R16400 (0x4010) - System Interrupts                                              
 */                                           
#define WM8325_PS_INT                           (0x8000)  /* PS_INT */
#define WM8325_PS_INT_MASK                      (0x8000)  /* PS_INT */
#define WM8325_PS_INT_SHIFT                     (    15)  /* PS_INT */
#define WM8325_PS_INT_WIDTH                     (     1)  /* PS_INT */
#define WM8325_TEMP_INT                         (0x4000)  /* TEMP_INT */
#define WM8325_TEMP_INT_MASK                    (0x4000)  /* TEMP_INT */
#define WM8325_TEMP_INT_SHIFT                   (    14)  /* TEMP_INT */
#define WM8325_TEMP_INT_WIDTH                   (     1)  /* TEMP_INT */
#define WM8325_GP_INT                           (0x2000)  /* GP_INT */
#define WM8325_GP_INT_MASK                      (0x2000)  /* GP_INT */
#define WM8325_GP_INT_SHIFT                     (    13)  /* GP_INT */
#define WM8325_GP_INT_WIDTH                     (     1)  /* GP_INT */
#define WM8325_ON_PIN_INT                       (0x1000)  /* ON_PIN_INT */
#define WM8325_ON_PIN_INT_MASK                  (0x1000)  /* ON_PIN_INT */
#define WM8325_ON_PIN_INT_SHIFT                 (    12)  /* ON_PIN_INT */
#define WM8325_ON_PIN_INT_WIDTH                 (     1)  /* ON_PIN_INT */
#define WM8325_WDOG_INT                         (0x0800)  /* WDOG_INT */
#define WM8325_WDOG_INT_MASK                    (0x0800)  /* WDOG_INT */
#define WM8325_WDOG_INT_SHIFT                   (    11)  /* WDOG_INT */
#define WM8325_WDOG_INT_WIDTH                   (     1)  /* WDOG_INT */
#define WM8325_AUXADC_INT                       (0x0100)  /* AUXADC_INT */
#define WM8325_AUXADC_INT_MASK                  (0x0100)  /* AUXADC_INT */
#define WM8325_AUXADC_INT_SHIFT                 (     8)  /* AUXADC_INT */
#define WM8325_AUXADC_INT_WIDTH                 (     1)  /* AUXADC_INT */
#define WM8325_PPM_INT                          (0x0080)  /* PPM_INT */
#define WM8325_PPM_INT_MASK                     (0x0080)  /* PPM_INT */
#define WM8325_PPM_INT_SHIFT                    (     7)  /* PPM_INT */
#define WM8325_PPM_INT_WIDTH                    (     1)  /* PPM_INT */
#define WM8325_RTC_INT                          (0x0020)  /* RTC_INT */
#define WM8325_RTC_INT_MASK                     (0x0020)  /* RTC_INT */
#define WM8325_RTC_INT_SHIFT                    (     5)  /* RTC_INT */
#define WM8325_RTC_INT_WIDTH                    (     1)  /* RTC_INT */
#define WM8325_OTP_INT                          (0x0010)  /* OTP_INT */
#define WM8325_OTP_INT_MASK                     (0x0010)  /* OTP_INT */
#define WM8325_OTP_INT_SHIFT                    (     4)  /* OTP_INT */
#define WM8325_OTP_INT_WIDTH                    (     1)  /* OTP_INT */
#define WM8325_HC_INT                           (0x0002)  /* HC_INT */
#define WM8325_HC_INT_MASK                      (0x0002)  /* HC_INT */
#define WM8325_HC_INT_SHIFT                     (     1)  /* HC_INT */
#define WM8325_HC_INT_WIDTH                     (     1)  /* HC_INT */
#define WM8325_UV_INT                           (0x0001)  /* UV_INT */
#define WM8325_UV_INT_MASK                      (0x0001)  /* UV_INT */
#define WM8325_UV_INT_SHIFT                     (     0)  /* UV_INT */
#define WM8325_UV_INT_WIDTH                     (     1)  /* UV_INT */
                                             
/*                                                                                                      
 * R16401 (0x4011) - Interrupt Status 1                                              
 */                                           
#define WM8325_PPM_SYSLO_EINT                   (0x8000)  /* PPM_SYSLO_EINT */
#define WM8325_PPM_SYSLO_EINT_MASK              (0x8000)  /* PPM_SYSLO_EINT */
#define WM8325_PPM_SYSLO_EINT_SHIFT             (    15)  /* PPM_SYSLO_EINT */
#define WM8325_PPM_SYSLO_EINT_WIDTH             (     1)  /* PPM_SYSLO_EINT */
#define WM8325_ON_PIN_CINT                      (0x1000)  /* ON_PIN_CINT */
#define WM8325_ON_PIN_CINT_MASK                 (0x1000)  /* ON_PIN_CINT */
#define WM8325_ON_PIN_CINT_SHIFT                (    12)  /* ON_PIN_CINT */
#define WM8325_ON_PIN_CINT_WIDTH                (     1)  /* ON_PIN_CINT */
#define WM8325_WDOG_TO_EINT                     (0x0800)  /* WDOG_TO_EINT */
#define WM8325_WDOG_TO_EINT_MASK                (0x0800)  /* WDOG_TO_EINT */
#define WM8325_WDOG_TO_EINT_SHIFT               (    11)  /* WDOG_TO_EINT */
#define WM8325_WDOG_TO_EINT_WIDTH               (     1)  /* WDOG_TO_EINT */
#define WM8325_AUXADC_DATA_EINT                 (0x0100)  /* AUXADC_DATA_EINT */
#define WM8325_AUXADC_DATA_EINT_MASK            (0x0100)  /* AUXADC_DATA_EINT */
#define WM8325_AUXADC_DATA_EINT_SHIFT           (     8)  /* AUXADC_DATA_EINT */
#define WM8325_AUXADC_DATA_EINT_WIDTH           (     1)  /* AUXADC_DATA_EINT */
#define WM8325_AUXADC_DCOMP4_EINT               (0x0080)  /* AUXADC_DCOMP4_EINT */
#define WM8325_AUXADC_DCOMP4_EINT_MASK          (0x0080)  /* AUXADC_DCOMP4_EINT */
#define WM8325_AUXADC_DCOMP4_EINT_SHIFT         (     7)  /* AUXADC_DCOMP4_EINT */
#define WM8325_AUXADC_DCOMP4_EINT_WIDTH         (     1)  /* AUXADC_DCOMP4_EINT */
#define WM8325_AUXADC_DCOMP3_EINT               (0x0040)  /* AUXADC_DCOMP3_EINT */
#define WM8325_AUXADC_DCOMP3_EINT_MASK          (0x0040)  /* AUXADC_DCOMP3_EINT */
#define WM8325_AUXADC_DCOMP3_EINT_SHIFT         (     6)  /* AUXADC_DCOMP3_EINT */
#define WM8325_AUXADC_DCOMP3_EINT_WIDTH         (     1)  /* AUXADC_DCOMP3_EINT */
#define WM8325_AUXADC_DCOMP2_EINT               (0x0020)  /* AUXADC_DCOMP2_EINT */
#define WM8325_AUXADC_DCOMP2_EINT_MASK          (0x0020)  /* AUXADC_DCOMP2_EINT */
#define WM8325_AUXADC_DCOMP2_EINT_SHIFT         (     5)  /* AUXADC_DCOMP2_EINT */
#define WM8325_AUXADC_DCOMP2_EINT_WIDTH         (     1)  /* AUXADC_DCOMP2_EINT */
#define WM8325_AUXADC_DCOMP1_EINT               (0x0010)  /* AUXADC_DCOMP1_EINT */
#define WM8325_AUXADC_DCOMP1_EINT_MASK          (0x0010)  /* AUXADC_DCOMP1_EINT */
#define WM8325_AUXADC_DCOMP1_EINT_SHIFT         (     4)  /* AUXADC_DCOMP1_EINT */
#define WM8325_AUXADC_DCOMP1_EINT_WIDTH         (     1)  /* AUXADC_DCOMP1_EINT */
#define WM8325_RTC_PER_EINT                     (0x0008)  /* RTC_PER_EINT */
#define WM8325_RTC_PER_EINT_MASK                (0x0008)  /* RTC_PER_EINT */
#define WM8325_RTC_PER_EINT_SHIFT               (     3)  /* RTC_PER_EINT */
#define WM8325_RTC_PER_EINT_WIDTH               (     1)  /* RTC_PER_EINT */
#define WM8325_RTC_ALM_EINT                     (0x0004)  /* RTC_ALM_EINT */
#define WM8325_RTC_ALM_EINT_MASK                (0x0004)  /* RTC_ALM_EINT */
#define WM8325_RTC_ALM_EINT_SHIFT               (     2)  /* RTC_ALM_EINT */
#define WM8325_RTC_ALM_EINT_WIDTH               (     1)  /* RTC_ALM_EINT */
#define WM8325_TEMP_THW_CINT                    (0x0002)  /* TEMP_THW_CINT */
#define WM8325_TEMP_THW_CINT_MASK               (0x0002)  /* TEMP_THW_CINT */
#define WM8325_TEMP_THW_CINT_SHIFT              (     1)  /* TEMP_THW_CINT */
#define WM8325_TEMP_THW_CINT_WIDTH              (     1)  /* TEMP_THW_CINT */
                                               
/*                                                                                                          
 * R16402 (0x4012) - Interrupt Status 2                                                  
 */                                             
#define WM8325_OTP_CMD_END_EINT                 (0x0020)  /* OTP_CMD_END_EINT */
#define WM8325_OTP_CMD_END_EINT_MASK            (0x0020)  /* OTP_CMD_END_EINT */
#define WM8325_OTP_CMD_END_EINT_SHIFT           (     5)  /* OTP_CMD_END_EINT */
#define WM8325_OTP_CMD_END_EINT_WIDTH           (     1)  /* OTP_CMD_END_EINT */
#define WM8325_OTP_ERR_EINT                     (0x0010)  /* OTP_ERR_EINT */
#define WM8325_OTP_ERR_EINT_MASK                (0x0010)  /* OTP_ERR_EINT */
#define WM8325_OTP_ERR_EINT_SHIFT               (     4)  /* OTP_ERR_EINT */
#define WM8325_OTP_ERR_EINT_WIDTH               (     1)  /* OTP_ERR_EINT */
#define WM8325_PS_POR_EINT                      (0x0004)  /* PS_POR_EINT */
#define WM8325_PS_POR_EINT_MASK                 (0x0004)  /* PS_POR_EINT */
#define WM8325_PS_POR_EINT_SHIFT                (     2)  /* PS_POR_EINT */
#define WM8325_PS_POR_EINT_WIDTH                (     1)  /* PS_POR_EINT */
#define WM8325_PS_SLEEP_OFF_EINT                (0x0002)  /* PS_SLEEP_OFF_EINT */
#define WM8325_PS_SLEEP_OFF_EINT_MASK           (0x0002)  /* PS_SLEEP_OFF_EINT */
#define WM8325_PS_SLEEP_OFF_EINT_SHIFT          (     1)  /* PS_SLEEP_OFF_EINT */
#define WM8325_PS_SLEEP_OFF_EINT_WIDTH          (     1)  /* PS_SLEEP_OFF_EINT */
#define WM8325_PS_ON_WAKE_EINT                  (0x0001)  /* PS_ON_WAKE_EINT */
#define WM8325_PS_ON_WAKE_EINT_MASK             (0x0001)  /* PS_ON_WAKE_EINT */
#define WM8325_PS_ON_WAKE_EINT_SHIFT            (     0)  /* PS_ON_WAKE_EINT */
#define WM8325_PS_ON_WAKE_EINT_WIDTH            (     1)  /* PS_ON_WAKE_EINT */
                                              
/*                                                                                                        
 * R16403 (0x4013) - Interrupt Status 3                                               
 */                                            
#define WM8325_UV_LDO10_EINT                    (0x0200)  /* UV_LDO10_EINT */
#define WM8325_UV_LDO10_EINT_MASK               (0x0200)  /* UV_LDO10_EINT */
#define WM8325_UV_LDO10_EINT_SHIFT              (     9)  /* UV_LDO10_EINT */
#define WM8325_UV_LDO10_EINT_WIDTH              (     1)  /* UV_LDO10_EINT */
#define WM8325_UV_LDO9_EINT                     (0x0100)  /* UV_LDO9_EINT */
#define WM8325_UV_LDO9_EINT_MASK                (0x0100)  /* UV_LDO9_EINT */
#define WM8325_UV_LDO9_EINT_SHIFT               (     8)  /* UV_LDO9_EINT */
#define WM8325_UV_LDO9_EINT_WIDTH               (     1)  /* UV_LDO9_EINT */
#define WM8325_UV_LDO8_EINT                     (0x0080)  /* UV_LDO8_EINT */
#define WM8325_UV_LDO8_EINT_MASK                (0x0080)  /* UV_LDO8_EINT */
#define WM8325_UV_LDO8_EINT_SHIFT               (     7)  /* UV_LDO8_EINT */
#define WM8325_UV_LDO8_EINT_WIDTH               (     1)  /* UV_LDO8_EINT */
#define WM8325_UV_LDO7_EINT                     (0x0040)  /* UV_LDO7_EINT */
#define WM8325_UV_LDO7_EINT_MASK                (0x0040)  /* UV_LDO7_EINT */
#define WM8325_UV_LDO7_EINT_SHIFT               (     6)  /* UV_LDO7_EINT */
#define WM8325_UV_LDO7_EINT_WIDTH               (     1)  /* UV_LDO7_EINT */
#define WM8325_UV_LDO6_EINT                     (0x0020)  /* UV_LDO6_EINT */
#define WM8325_UV_LDO6_EINT_MASK                (0x0020)  /* UV_LDO6_EINT */
#define WM8325_UV_LDO6_EINT_SHIFT               (     5)  /* UV_LDO6_EINT */
#define WM8325_UV_LDO6_EINT_WIDTH               (     1)  /* UV_LDO6_EINT */
#define WM8325_UV_LDO5_EINT                     (0x0010)  /* UV_LDO5_EINT */
#define WM8325_UV_LDO5_EINT_MASK                (0x0010)  /* UV_LDO5_EINT */
#define WM8325_UV_LDO5_EINT_SHIFT               (     4)  /* UV_LDO5_EINT */
#define WM8325_UV_LDO5_EINT_WIDTH               (     1)  /* UV_LDO5_EINT */
#define WM8325_UV_LDO4_EINT                     (0x0008)  /* UV_LDO4_EINT */
#define WM8325_UV_LDO4_EINT_MASK                (0x0008)  /* UV_LDO4_EINT */
#define WM8325_UV_LDO4_EINT_SHIFT               (     3)  /* UV_LDO4_EINT */
#define WM8325_UV_LDO4_EINT_WIDTH               (     1)  /* UV_LDO4_EINT */
#define WM8325_UV_LDO3_EINT                     (0x0004)  /* UV_LDO3_EINT */
#define WM8325_UV_LDO3_EINT_MASK                (0x0004)  /* UV_LDO3_EINT */
#define WM8325_UV_LDO3_EINT_SHIFT               (     2)  /* UV_LDO3_EINT */
#define WM8325_UV_LDO3_EINT_WIDTH               (     1)  /* UV_LDO3_EINT */
#define WM8325_UV_LDO2_EINT                     (0x0002)  /* UV_LDO2_EINT */
#define WM8325_UV_LDO2_EINT_MASK                (0x0002)  /* UV_LDO2_EINT */
#define WM8325_UV_LDO2_EINT_SHIFT               (     1)  /* UV_LDO2_EINT */
#define WM8325_UV_LDO2_EINT_WIDTH               (     1)  /* UV_LDO2_EINT */
#define WM8325_UV_LDO1_EINT                     (0x0001)  /* UV_LDO1_EINT */
#define WM8325_UV_LDO1_EINT_MASK                (0x0001)  /* UV_LDO1_EINT */
#define WM8325_UV_LDO1_EINT_SHIFT               (     0)  /* UV_LDO1_EINT */
#define WM8325_UV_LDO1_EINT_WIDTH               (     1)  /* UV_LDO1_EINT */
                                             
/*                                                                                                      
 * R16404 (0x4014) - Interrupt Status 4                                             
 */                                           
#define WM8325_HC_DC2_EINT                      (0x0200)  /* HC_DC2_EINT */
#define WM8325_HC_DC2_EINT_MASK                 (0x0200)  /* HC_DC2_EINT */
#define WM8325_HC_DC2_EINT_SHIFT                (     9)  /* HC_DC2_EINT */
#define WM8325_HC_DC2_EINT_WIDTH                (     1)  /* HC_DC2_EINT */
#define WM8325_HC_DC1_EINT                      (0x0100)  /* HC_DC1_EINT */
#define WM8325_HC_DC1_EINT_MASK                 (0x0100)  /* HC_DC1_EINT */
#define WM8325_HC_DC1_EINT_SHIFT                (     8)  /* HC_DC1_EINT */
#define WM8325_HC_DC1_EINT_WIDTH                (     1)  /* HC_DC1_EINT */
#define WM8325_XTAL_START_EINT                  (0x0080)  /* XTAL_START_EINT */
#define WM8325_XTAL_START_EINT_MASK             (0x0080)  /* XTAL_START_EINT */
#define WM8325_XTAL_START_EINT_SHIFT            (     7)  /* XTAL_START_EINT */
#define WM8325_XTAL_START_EINT_WIDTH            (     1)  /* XTAL_START_EINT */
#define WM8325_XTAL_TAMPER_EINT                 (0x0040)  /* XTAL_TAMPER_EINT */
#define WM8325_XTAL_TAMPER_EINT_MASK            (0x0040)  /* XTAL_TAMPER_EINT */
#define WM8325_XTAL_TAMPER_EINT_SHIFT           (     6)  /* XTAL_TAMPER_EINT */
#define WM8325_XTAL_TAMPER_EINT_WIDTH           (     1)  /* XTAL_TAMPER_EINT */
#define WM8325_UV_DC4_EINT                      (0x0008)  /* UV_DC4_EINT */
#define WM8325_UV_DC4_EINT_MASK                 (0x0008)  /* UV_DC4_EINT */
#define WM8325_UV_DC4_EINT_SHIFT                (     3)  /* UV_DC4_EINT */
#define WM8325_UV_DC4_EINT_WIDTH                (     1)  /* UV_DC4_EINT */
#define WM8325_UV_DC3_EINT                      (0x0004)  /* UV_DC3_EINT */
#define WM8325_UV_DC3_EINT_MASK                 (0x0004)  /* UV_DC3_EINT */
#define WM8325_UV_DC3_EINT_SHIFT                (     2)  /* UV_DC3_EINT */
#define WM8325_UV_DC3_EINT_WIDTH                (     1)  /* UV_DC3_EINT */
#define WM8325_UV_DC2_EINT                      (0x0002)  /* UV_DC2_EINT */
#define WM8325_UV_DC2_EINT_MASK                 (0x0002)  /* UV_DC2_EINT */
#define WM8325_UV_DC2_EINT_SHIFT                (     1)  /* UV_DC2_EINT */
#define WM8325_UV_DC2_EINT_WIDTH                (     1)  /* UV_DC2_EINT */
#define WM8325_UV_DC1_EINT                      (0x0001)  /* UV_DC1_EINT */
#define WM8325_UV_DC1_EINT_MASK                 (0x0001)  /* UV_DC1_EINT */
#define WM8325_UV_DC1_EINT_SHIFT                (     0)  /* UV_DC1_EINT */
#define WM8325_UV_DC1_EINT_WIDTH                (     1)  /* UV_DC1_EINT */
                                             
/*                                                                                                      
 * R16405 (0x4015) - Interrupt Status 5                                             
 */                                           
#define WM8325_GP12_EINT                        (0x0800)  /* GP12_EINT */
#define WM8325_GP12_EINT_MASK                   (0x0800)  /* GP12_EINT */
#define WM8325_GP12_EINT_SHIFT                  (    11)  /* GP12_EINT */
#define WM8325_GP12_EINT_WIDTH                  (     1)  /* GP12_EINT */
#define WM8325_GP11_EINT                        (0x0400)  /* GP11_EINT */
#define WM8325_GP11_EINT_MASK                   (0x0400)  /* GP11_EINT */
#define WM8325_GP11_EINT_SHIFT                  (    10)  /* GP11_EINT */
#define WM8325_GP11_EINT_WIDTH                  (     1)  /* GP11_EINT */
#define WM8325_GP10_EINT                        (0x0200)  /* GP10_EINT */
#define WM8325_GP10_EINT_MASK                   (0x0200)  /* GP10_EINT */
#define WM8325_GP10_EINT_SHIFT                  (     9)  /* GP10_EINT */
#define WM8325_GP10_EINT_WIDTH                  (     1)  /* GP10_EINT */
#define WM8325_GP9_EINT                         (0x0100)  /* GP9_EINT */
#define WM8325_GP9_EINT_MASK                    (0x0100)  /* GP9_EINT */
#define WM8325_GP9_EINT_SHIFT                   (     8)  /* GP9_EINT */
#define WM8325_GP9_EINT_WIDTH                   (     1)  /* GP9_EINT */
#define WM8325_GP8_EINT                         (0x0080)  /* GP8_EINT */
#define WM8325_GP8_EINT_MASK                    (0x0080)  /* GP8_EINT */
#define WM8325_GP8_EINT_SHIFT                   (     7)  /* GP8_EINT */
#define WM8325_GP8_EINT_WIDTH                   (     1)  /* GP8_EINT */
#define WM8325_GP7_EINT                         (0x0040)  /* GP7_EINT */
#define WM8325_GP7_EINT_MASK                    (0x0040)  /* GP7_EINT */
#define WM8325_GP7_EINT_SHIFT                   (     6)  /* GP7_EINT */
#define WM8325_GP7_EINT_WIDTH                   (     1)  /* GP7_EINT */
#define WM8325_GP6_EINT                         (0x0020)  /* GP6_EINT */
#define WM8325_GP6_EINT_MASK                    (0x0020)  /* GP6_EINT */
#define WM8325_GP6_EINT_SHIFT                   (     5)  /* GP6_EINT */
#define WM8325_GP6_EINT_WIDTH                   (     1)  /* GP6_EINT */
#define WM8325_GP5_EINT                         (0x0010)  /* GP5_EINT */
#define WM8325_GP5_EINT_MASK                    (0x0010)  /* GP5_EINT */
#define WM8325_GP5_EINT_SHIFT                   (     4)  /* GP5_EINT */
#define WM8325_GP5_EINT_WIDTH                   (     1)  /* GP5_EINT */
#define WM8325_GP4_EINT                         (0x0008)  /* GP4_EINT */
#define WM8325_GP4_EINT_MASK                    (0x0008)  /* GP4_EINT */
#define WM8325_GP4_EINT_SHIFT                   (     3)  /* GP4_EINT */
#define WM8325_GP4_EINT_WIDTH                   (     1)  /* GP4_EINT */
#define WM8325_GP3_EINT                         (0x0004)  /* GP3_EINT */
#define WM8325_GP3_EINT_MASK                    (0x0004)  /* GP3_EINT */
#define WM8325_GP3_EINT_SHIFT                   (     2)  /* GP3_EINT */
#define WM8325_GP3_EINT_WIDTH                   (     1)  /* GP3_EINT */
#define WM8325_GP2_EINT                         (0x0002)  /* GP2_EINT */
#define WM8325_GP2_EINT_MASK                    (0x0002)  /* GP2_EINT */
#define WM8325_GP2_EINT_SHIFT                   (     1)  /* GP2_EINT */
#define WM8325_GP2_EINT_WIDTH                   (     1)  /* GP2_EINT */
#define WM8325_GP1_EINT                         (0x0001)  /* GP1_EINT */
#define WM8325_GP1_EINT_MASK                    (0x0001)  /* GP1_EINT */
#define WM8325_GP1_EINT_SHIFT                   (     0)  /* GP1_EINT */
#define WM8325_GP1_EINT_WIDTH                   (     1)  /* GP1_EINT */
                                               
/*                                                                                                          
 * R16407 (0x4017) - IRQ Config                                                            
 */                                             
#define WM8325_IRQ_OD                           (0x0002)  /* IRQ_OD */
#define WM8325_IRQ_OD_MASK                      (0x0002)  /* IRQ_OD */
#define WM8325_IRQ_OD_SHIFT                     (     1)  /* IRQ_OD */
#define WM8325_IRQ_OD_WIDTH                     (     1)  /* IRQ_OD */
#define WM8325_IM_IRQ                           (0x0001)  /* IM_IRQ */
#define WM8325_IM_IRQ_MASK                      (0x0001)  /* IM_IRQ */
#define WM8325_IM_IRQ_SHIFT                     (     0)  /* IM_IRQ */
#define WM8325_IM_IRQ_WIDTH                     (     1)  /* IM_IRQ */
                                              
/*                                                                                                        
 * R16408 (0x4018) - System Interrupts Mask                                        
 */                                            
#define WM8325_IM_PS_INT                        (0x8000)  /* IM_PS_INT */
#define WM8325_IM_PS_INT_MASK                   (0x8000)  /* IM_PS_INT */
#define WM8325_IM_PS_INT_SHIFT                  (    15)  /* IM_PS_INT */
#define WM8325_IM_PS_INT_WIDTH                  (     1)  /* IM_PS_INT */
#define WM8325_IM_TEMP_INT                      (0x4000)  /* IM_TEMP_INT */
#define WM8325_IM_TEMP_INT_MASK                 (0x4000)  /* IM_TEMP_INT */
#define WM8325_IM_TEMP_INT_SHIFT                (    14)  /* IM_TEMP_INT */
#define WM8325_IM_TEMP_INT_WIDTH                (     1)  /* IM_TEMP_INT */
#define WM8325_IM_GP_INT                        (0x2000)  /* IM_GP_INT */
#define WM8325_IM_GP_INT_MASK                   (0x2000)  /* IM_GP_INT */
#define WM8325_IM_GP_INT_SHIFT                  (    13)  /* IM_GP_INT */
#define WM8325_IM_GP_INT_WIDTH                  (     1)  /* IM_GP_INT */
#define WM8325_IM_ON_PIN_INT                    (0x1000)  /* IM_ON_PIN_INT */
#define WM8325_IM_ON_PIN_INT_MASK               (0x1000)  /* IM_ON_PIN_INT */
#define WM8325_IM_ON_PIN_INT_SHIFT              (    12)  /* IM_ON_PIN_INT */
#define WM8325_IM_ON_PIN_INT_WIDTH              (     1)  /* IM_ON_PIN_INT */
#define WM8325_IM_WDOG_INT                      (0x0800)  /* IM_WDOG_INT */
#define WM8325_IM_WDOG_INT_MASK                 (0x0800)  /* IM_WDOG_INT */
#define WM8325_IM_WDOG_INT_SHIFT                (    11)  /* IM_WDOG_INT */
#define WM8325_IM_WDOG_INT_WIDTH                (     1)  /* IM_WDOG_INT */
#define WM8325_IM_AUXADC_INT                    (0x0100)  /* IM_AUXADC_INT */
#define WM8325_IM_AUXADC_INT_MASK               (0x0100)  /* IM_AUXADC_INT */
#define WM8325_IM_AUXADC_INT_SHIFT              (     8)  /* IM_AUXADC_INT */
#define WM8325_IM_AUXADC_INT_WIDTH              (     1)  /* IM_AUXADC_INT */
#define WM8325_IM_PPM_INT                       (0x0080)  /* IM_PPM_INT */
#define WM8325_IM_PPM_INT_MASK                  (0x0080)  /* IM_PPM_INT */
#define WM8325_IM_PPM_INT_SHIFT                 (     7)  /* IM_PPM_INT */
#define WM8325_IM_PPM_INT_WIDTH                 (     1)  /* IM_PPM_INT */
#define WM8325_IM_RTC_INT                       (0x0020)  /* IM_RTC_INT */
#define WM8325_IM_RTC_INT_MASK                  (0x0020)  /* IM_RTC_INT */
#define WM8325_IM_RTC_INT_SHIFT                 (     5)  /* IM_RTC_INT */
#define WM8325_IM_RTC_INT_WIDTH                 (     1)  /* IM_RTC_INT */
#define WM8325_IM_OTP_INT                       (0x0010)  /* IM_OTP_INT */
#define WM8325_IM_OTP_INT_MASK                  (0x0010)  /* IM_OTP_INT */
#define WM8325_IM_OTP_INT_SHIFT                 (     4)  /* IM_OTP_INT */
#define WM8325_IM_OTP_INT_WIDTH                 (     1)  /* IM_OTP_INT */
#define WM8325_IM_HC_INT                        (0x0002)  /* IM_HC_INT */
#define WM8325_IM_HC_INT_MASK                   (0x0002)  /* IM_HC_INT */
#define WM8325_IM_HC_INT_SHIFT                  (     1)  /* IM_HC_INT */
#define WM8325_IM_HC_INT_WIDTH                  (     1)  /* IM_HC_INT */
#define WM8325_IM_UV_INT                        (0x0001)  /* IM_UV_INT */
#define WM8325_IM_UV_INT_MASK                   (0x0001)  /* IM_UV_INT */
#define WM8325_IM_UV_INT_SHIFT                  (     0)  /* IM_UV_INT */
#define WM8325_IM_UV_INT_WIDTH                  (     1)  /* IM_UV_INT */
                                              
/*                                                                                                        
 * R16409 (0x4019) - Interrupt Status 1 Mask                                       
 */                                            
#define WM8325_IM_PPM_SYSLO_EINT                (0x8000)  /* IM_PPM_SYSLO_EINT */
#define WM8325_IM_PPM_SYSLO_EINT_MASK           (0x8000)  /* IM_PPM_SYSLO_EINT */
#define WM8325_IM_PPM_SYSLO_EINT_SHIFT          (    15)  /* IM_PPM_SYSLO_EINT */
#define WM8325_IM_PPM_SYSLO_EINT_WIDTH          (     1)  /* IM_PPM_SYSLO_EINT */
#define WM8325_IM_ON_PIN_CINT                   (0x1000)  /* IM_ON_PIN_CINT */
#define WM8325_IM_ON_PIN_CINT_MASK              (0x1000)  /* IM_ON_PIN_CINT */
#define WM8325_IM_ON_PIN_CINT_SHIFT             (    12)  /* IM_ON_PIN_CINT */
#define WM8325_IM_ON_PIN_CINT_WIDTH             (     1)  /* IM_ON_PIN_CINT */
#define WM8325_IM_WDOG_TO_EINT                  (0x0800)  /* IM_WDOG_TO_EINT */
#define WM8325_IM_WDOG_TO_EINT_MASK             (0x0800)  /* IM_WDOG_TO_EINT */
#define WM8325_IM_WDOG_TO_EINT_SHIFT            (    11)  /* IM_WDOG_TO_EINT */
#define WM8325_IM_WDOG_TO_EINT_WIDTH            (     1)  /* IM_WDOG_TO_EINT */
#define WM8325_IM_AUXADC_DATA_EINT              (0x0100)  /* IM_AUXADC_DATA_EINT */
#define WM8325_IM_AUXADC_DATA_EINT_MASK         (0x0100)  /* IM_AUXADC_DATA_EINT */
#define WM8325_IM_AUXADC_DATA_EINT_SHIFT        (     8)  /* IM_AUXADC_DATA_EINT */
#define WM8325_IM_AUXADC_DATA_EINT_WIDTH        (     1)  /* IM_AUXADC_DATA_EINT */
#define WM8325_IM_AUXADC_DCOMP4_EINT            (0x0080)  /* IM_AUXADC_DCOMP4_EINT */
#define WM8325_IM_AUXADC_DCOMP4_EINT_MASK       (0x0080)  /* IM_AUXADC_DCOMP4_EINT */
#define WM8325_IM_AUXADC_DCOMP4_EINT_SHIFT      (     7)  /* IM_AUXADC_DCOMP4_EINT */
#define WM8325_IM_AUXADC_DCOMP4_EINT_WIDTH      (     1)  /* IM_AUXADC_DCOMP4_EINT */
#define WM8325_IM_AUXADC_DCOMP3_EINT            (0x0040)  /* IM_AUXADC_DCOMP3_EINT */
#define WM8325_IM_AUXADC_DCOMP3_EINT_MASK       (0x0040)  /* IM_AUXADC_DCOMP3_EINT */
#define WM8325_IM_AUXADC_DCOMP3_EINT_SHIFT      (     6)  /* IM_AUXADC_DCOMP3_EINT */
#define WM8325_IM_AUXADC_DCOMP3_EINT_WIDTH      (     1)  /* IM_AUXADC_DCOMP3_EINT */
#define WM8325_IM_AUXADC_DCOMP2_EINT            (0x0020)  /* IM_AUXADC_DCOMP2_EINT */
#define WM8325_IM_AUXADC_DCOMP2_EINT_MASK       (0x0020)  /* IM_AUXADC_DCOMP2_EINT */
#define WM8325_IM_AUXADC_DCOMP2_EINT_SHIFT      (     5)  /* IM_AUXADC_DCOMP2_EINT */
#define WM8325_IM_AUXADC_DCOMP2_EINT_WIDTH      (     1)  /* IM_AUXADC_DCOMP2_EINT */
#define WM8325_IM_AUXADC_DCOMP1_EINT            (0x0010)  /* IM_AUXADC_DCOMP1_EINT */
#define WM8325_IM_AUXADC_DCOMP1_EINT_MASK       (0x0010)  /* IM_AUXADC_DCOMP1_EINT */
#define WM8325_IM_AUXADC_DCOMP1_EINT_SHIFT      (     4)  /* IM_AUXADC_DCOMP1_EINT */
#define WM8325_IM_AUXADC_DCOMP1_EINT_WIDTH      (     1)  /* IM_AUXADC_DCOMP1_EINT */
#define WM8325_IM_RTC_PER_EINT                  (0x0008)  /* IM_RTC_PER_EINT */
#define WM8325_IM_RTC_PER_EINT_MASK             (0x0008)  /* IM_RTC_PER_EINT */
#define WM8325_IM_RTC_PER_EINT_SHIFT            (     3)  /* IM_RTC_PER_EINT */
#define WM8325_IM_RTC_PER_EINT_WIDTH            (     1)  /* IM_RTC_PER_EINT */
#define WM8325_IM_RTC_ALM_EINT                  (0x0004)  /* IM_RTC_ALM_EINT */
#define WM8325_IM_RTC_ALM_EINT_MASK             (0x0004)  /* IM_RTC_ALM_EINT */
#define WM8325_IM_RTC_ALM_EINT_SHIFT            (     2)  /* IM_RTC_ALM_EINT */
#define WM8325_IM_RTC_ALM_EINT_WIDTH            (     1)  /* IM_RTC_ALM_EINT */
#define WM8325_IM_TEMP_THW_CINT                 (0x0002)  /* IM_TEMP_THW_CINT */
#define WM8325_IM_TEMP_THW_CINT_MASK            (0x0002)  /* IM_TEMP_THW_CINT */
#define WM8325_IM_TEMP_THW_CINT_SHIFT           (     1)  /* IM_TEMP_THW_CINT */
#define WM8325_IM_TEMP_THW_CINT_WIDTH           (     1)  /* IM_TEMP_THW_CINT */
                                              
/*                                                                                                        
 * R16410 (0x401A) - Interrupt Status 2 Mask                                       
 */                                            
#define WM8325_IM_OTP_CMD_END_EINT              (0x0020)  /* IM_OTP_CMD_END_EINT */
#define WM8325_IM_OTP_CMD_END_EINT_MASK         (0x0020)  /* IM_OTP_CMD_END_EINT */
#define WM8325_IM_OTP_CMD_END_EINT_SHIFT        (     5)  /* IM_OTP_CMD_END_EINT */
#define WM8325_IM_OTP_CMD_END_EINT_WIDTH        (     1)  /* IM_OTP_CMD_END_EINT */
#define WM8325_IM_OTP_ERR_EINT                  (0x0010)  /* IM_OTP_ERR_EINT */
#define WM8325_IM_OTP_ERR_EINT_MASK             (0x0010)  /* IM_OTP_ERR_EINT */
#define WM8325_IM_OTP_ERR_EINT_SHIFT            (     4)  /* IM_OTP_ERR_EINT */
#define WM8325_IM_OTP_ERR_EINT_WIDTH            (     1)  /* IM_OTP_ERR_EINT */
#define WM8325_IM_PS_POR_EINT                   (0x0004)  /* IM_PS_POR_EINT */
#define WM8325_IM_PS_POR_EINT_MASK              (0x0004)  /* IM_PS_POR_EINT */
#define WM8325_IM_PS_POR_EINT_SHIFT             (     2)  /* IM_PS_POR_EINT */
#define WM8325_IM_PS_POR_EINT_WIDTH             (     1)  /* IM_PS_POR_EINT */
#define WM8325_IM_PS_SLEEP_OFF_EINT             (0x0002)  /* IM_PS_SLEEP_OFF_EINT */
#define WM8325_IM_PS_SLEEP_OFF_EINT_MASK        (0x0002)  /* IM_PS_SLEEP_OFF_EINT */
#define WM8325_IM_PS_SLEEP_OFF_EINT_SHIFT       (     1)  /* IM_PS_SLEEP_OFF_EINT */
#define WM8325_IM_PS_SLEEP_OFF_EINT_WIDTH       (     1)  /* IM_PS_SLEEP_OFF_EINT */
#define WM8325_IM_PS_ON_WAKE_EINT               (0x0001)  /* IM_PS_ON_WAKE_EINT */
#define WM8325_IM_PS_ON_WAKE_EINT_MASK          (0x0001)  /* IM_PS_ON_WAKE_EINT */
#define WM8325_IM_PS_ON_WAKE_EINT_SHIFT         (     0)  /* IM_PS_ON_WAKE_EINT */
#define WM8325_IM_PS_ON_WAKE_EINT_WIDTH         (     1)  /* IM_PS_ON_WAKE_EINT */
                                              
/*                                                                                                        
 * R16411 (0x401B) - Interrupt Status 3 Mask                                        
 */                                            
#define WM8325_IM_UV_LDO10_EINT                 (0x0200)  /* IM_UV_LDO10_EINT */
#define WM8325_IM_UV_LDO10_EINT_MASK            (0x0200)  /* IM_UV_LDO10_EINT */
#define WM8325_IM_UV_LDO10_EINT_SHIFT           (     9)  /* IM_UV_LDO10_EINT */
#define WM8325_IM_UV_LDO10_EINT_WIDTH           (     1)  /* IM_UV_LDO10_EINT */
#define WM8325_IM_UV_LDO9_EINT                  (0x0100)  /* IM_UV_LDO9_EINT */
#define WM8325_IM_UV_LDO9_EINT_MASK             (0x0100)  /* IM_UV_LDO9_EINT */
#define WM8325_IM_UV_LDO9_EINT_SHIFT            (     8)  /* IM_UV_LDO9_EINT */
#define WM8325_IM_UV_LDO9_EINT_WIDTH            (     1)  /* IM_UV_LDO9_EINT */
#define WM8325_IM_UV_LDO8_EINT                  (0x0080)  /* IM_UV_LDO8_EINT */
#define WM8325_IM_UV_LDO8_EINT_MASK             (0x0080)  /* IM_UV_LDO8_EINT */
#define WM8325_IM_UV_LDO8_EINT_SHIFT            (     7)  /* IM_UV_LDO8_EINT */
#define WM8325_IM_UV_LDO8_EINT_WIDTH            (     1)  /* IM_UV_LDO8_EINT */
#define WM8325_IM_UV_LDO7_EINT                  (0x0040)  /* IM_UV_LDO7_EINT */
#define WM8325_IM_UV_LDO7_EINT_MASK             (0x0040)  /* IM_UV_LDO7_EINT */
#define WM8325_IM_UV_LDO7_EINT_SHIFT            (     6)  /* IM_UV_LDO7_EINT */
#define WM8325_IM_UV_LDO7_EINT_WIDTH            (     1)  /* IM_UV_LDO7_EINT */
#define WM8325_IM_UV_LDO6_EINT                  (0x0020)  /* IM_UV_LDO6_EINT */
#define WM8325_IM_UV_LDO6_EINT_MASK             (0x0020)  /* IM_UV_LDO6_EINT */
#define WM8325_IM_UV_LDO6_EINT_SHIFT            (     5)  /* IM_UV_LDO6_EINT */
#define WM8325_IM_UV_LDO6_EINT_WIDTH            (     1)  /* IM_UV_LDO6_EINT */
#define WM8325_IM_UV_LDO5_EINT                  (0x0010)  /* IM_UV_LDO5_EINT */
#define WM8325_IM_UV_LDO5_EINT_MASK             (0x0010)  /* IM_UV_LDO5_EINT */
#define WM8325_IM_UV_LDO5_EINT_SHIFT            (     4)  /* IM_UV_LDO5_EINT */
#define WM8325_IM_UV_LDO5_EINT_WIDTH            (     1)  /* IM_UV_LDO5_EINT */
#define WM8325_IM_UV_LDO4_EINT                  (0x0008)  /* IM_UV_LDO4_EINT */
#define WM8325_IM_UV_LDO4_EINT_MASK             (0x0008)  /* IM_UV_LDO4_EINT */
#define WM8325_IM_UV_LDO4_EINT_SHIFT            (     3)  /* IM_UV_LDO4_EINT */
#define WM8325_IM_UV_LDO4_EINT_WIDTH            (     1)  /* IM_UV_LDO4_EINT */
#define WM8325_IM_UV_LDO3_EINT                  (0x0004)  /* IM_UV_LDO3_EINT */
#define WM8325_IM_UV_LDO3_EINT_MASK             (0x0004)  /* IM_UV_LDO3_EINT */
#define WM8325_IM_UV_LDO3_EINT_SHIFT            (     2)  /* IM_UV_LDO3_EINT */
#define WM8325_IM_UV_LDO3_EINT_WIDTH            (     1)  /* IM_UV_LDO3_EINT */
#define WM8325_IM_UV_LDO2_EINT                  (0x0002)  /* IM_UV_LDO2_EINT */
#define WM8325_IM_UV_LDO2_EINT_MASK             (0x0002)  /* IM_UV_LDO2_EINT */
#define WM8325_IM_UV_LDO2_EINT_SHIFT            (     1)  /* IM_UV_LDO2_EINT */
#define WM8325_IM_UV_LDO2_EINT_WIDTH            (     1)  /* IM_UV_LDO2_EINT */
#define WM8325_IM_UV_LDO1_EINT                  (0x0001)  /* IM_UV_LDO1_EINT */
#define WM8325_IM_UV_LDO1_EINT_MASK             (0x0001)  /* IM_UV_LDO1_EINT */
#define WM8325_IM_UV_LDO1_EINT_SHIFT            (     0)  /* IM_UV_LDO1_EINT */
#define WM8325_IM_UV_LDO1_EINT_WIDTH            (     1)  /* IM_UV_LDO1_EINT */
                                               
/*                                                                                                          
 * R16412 (0x401C) - Interrupt Status 4 Mask                                          
 */                                             
#define WM8325_IM_HC_DC2_EINT                   (0x0200)  /* IM_HC_DC2_EINT */
#define WM8325_IM_HC_DC2_EINT_MASK              (0x0200)  /* IM_HC_DC2_EINT */
#define WM8325_IM_HC_DC2_EINT_SHIFT             (     9)  /* IM_HC_DC2_EINT */
#define WM8325_IM_HC_DC2_EINT_WIDTH             (     1)  /* IM_HC_DC2_EINT */
#define WM8325_IM_HC_DC1_EINT                   (0x0100)  /* IM_HC_DC1_EINT */
#define WM8325_IM_HC_DC1_EINT_MASK              (0x0100)  /* IM_HC_DC1_EINT */
#define WM8325_IM_HC_DC1_EINT_SHIFT             (     8)  /* IM_HC_DC1_EINT */
#define WM8325_IM_HC_DC1_EINT_WIDTH             (     1)  /* IM_HC_DC1_EINT */
#define WM8325_IM_XTAL_START_EINT               (0x0080)  /* IM_XTAL_START_EINT */
#define WM8325_IM_XTAL_START_EINT_MASK          (0x0080)  /* IM_XTAL_START_EINT */
#define WM8325_IM_XTAL_START_EINT_SHIFT         (     7)  /* IM_XTAL_START_EINT */
#define WM8325_IM_XTAL_START_EINT_WIDTH         (     1)  /* IM_XTAL_START_EINT */
#define WM8325_IM_UV_DC4_EINT                   (0x0008)  /* IM_UV_DC4_EINT */
#define WM8325_IM_UV_DC4_EINT_MASK              (0x0008)  /* IM_UV_DC4_EINT */
#define WM8325_IM_UV_DC4_EINT_SHIFT             (     3)  /* IM_UV_DC4_EINT */
#define WM8325_IM_UV_DC4_EINT_WIDTH             (     1)  /* IM_UV_DC4_EINT */
#define WM8325_IM_UV_DC3_EINT                   (0x0004)  /* IM_UV_DC3_EINT */
#define WM8325_IM_UV_DC3_EINT_MASK              (0x0004)  /* IM_UV_DC3_EINT */
#define WM8325_IM_UV_DC3_EINT_SHIFT             (     2)  /* IM_UV_DC3_EINT */
#define WM8325_IM_UV_DC3_EINT_WIDTH             (     1)  /* IM_UV_DC3_EINT */
#define WM8325_IM_UV_DC2_EINT                   (0x0002)  /* IM_UV_DC2_EINT */
#define WM8325_IM_UV_DC2_EINT_MASK              (0x0002)  /* IM_UV_DC2_EINT */
#define WM8325_IM_UV_DC2_EINT_SHIFT             (     1)  /* IM_UV_DC2_EINT */
#define WM8325_IM_UV_DC2_EINT_WIDTH             (     1)  /* IM_UV_DC2_EINT */
#define WM8325_IM_UV_DC1_EINT                   (0x0001)  /* IM_UV_DC1_EINT */
#define WM8325_IM_UV_DC1_EINT_MASK              (0x0001)  /* IM_UV_DC1_EINT */
#define WM8325_IM_UV_DC1_EINT_SHIFT             (     0)  /* IM_UV_DC1_EINT */
#define WM8325_IM_UV_DC1_EINT_WIDTH             (     1)  /* IM_UV_DC1_EINT */
                                              
/*                                                                                                        
 * R16413 (0x401D) - Interrupt Status 5 Mask                                       
 */                                            
#define WM8325_IM_GP12_EINT                     (0x0800)  /* IM_GP12_EINT */
#define WM8325_IM_GP12_EINT_MASK                (0x0800)  /* IM_GP12_EINT */
#define WM8325_IM_GP12_EINT_SHIFT               (    11)  /* IM_GP12_EINT */
#define WM8325_IM_GP12_EINT_WIDTH               (     1)  /* IM_GP12_EINT */
#define WM8325_IM_GP11_EINT                     (0x0400)  /* IM_GP11_EINT */
#define WM8325_IM_GP11_EINT_MASK                (0x0400)  /* IM_GP11_EINT */
#define WM8325_IM_GP11_EINT_SHIFT               (    10)  /* IM_GP11_EINT */
#define WM8325_IM_GP11_EINT_WIDTH               (     1)  /* IM_GP11_EINT */
#define WM8325_IM_GP10_EINT                     (0x0200)  /* IM_GP10_EINT */
#define WM8325_IM_GP10_EINT_MASK                (0x0200)  /* IM_GP10_EINT */
#define WM8325_IM_GP10_EINT_SHIFT               (     9)  /* IM_GP10_EINT */
#define WM8325_IM_GP10_EINT_WIDTH               (     1)  /* IM_GP10_EINT */
#define WM8325_IM_GP9_EINT                      (0x0100)  /* IM_GP9_EINT */
#define WM8325_IM_GP9_EINT_MASK                 (0x0100)  /* IM_GP9_EINT */
#define WM8325_IM_GP9_EINT_SHIFT                (     8)  /* IM_GP9_EINT */
#define WM8325_IM_GP9_EINT_WIDTH                (     1)  /* IM_GP9_EINT */
#define WM8325_IM_GP8_EINT                      (0x0080)  /* IM_GP8_EINT */
#define WM8325_IM_GP8_EINT_MASK                 (0x0080)  /* IM_GP8_EINT */
#define WM8325_IM_GP8_EINT_SHIFT                (     7)  /* IM_GP8_EINT */
#define WM8325_IM_GP8_EINT_WIDTH                (     1)  /* IM_GP8_EINT */
#define WM8325_IM_GP7_EINT                      (0x0040)  /* IM_GP7_EINT */
#define WM8325_IM_GP7_EINT_MASK                 (0x0040)  /* IM_GP7_EINT */
#define WM8325_IM_GP7_EINT_SHIFT                (     6)  /* IM_GP7_EINT */
#define WM8325_IM_GP7_EINT_WIDTH                (     1)  /* IM_GP7_EINT */
#define WM8325_IM_GP6_EINT                      (0x0020)  /* IM_GP6_EINT */
#define WM8325_IM_GP6_EINT_MASK                 (0x0020)  /* IM_GP6_EINT */
#define WM8325_IM_GP6_EINT_SHIFT                (     5)  /* IM_GP6_EINT */
#define WM8325_IM_GP6_EINT_WIDTH                (     1)  /* IM_GP6_EINT */
#define WM8325_IM_GP5_EINT                      (0x0010)  /* IM_GP5_EINT */
#define WM8325_IM_GP5_EINT_MASK                 (0x0010)  /* IM_GP5_EINT */
#define WM8325_IM_GP5_EINT_SHIFT                (     4)  /* IM_GP5_EINT */
#define WM8325_IM_GP5_EINT_WIDTH                (     1)  /* IM_GP5_EINT */
#define WM8325_IM_GP4_EINT                      (0x0008)  /* IM_GP4_EINT */
#define WM8325_IM_GP4_EINT_MASK                 (0x0008)  /* IM_GP4_EINT */
#define WM8325_IM_GP4_EINT_SHIFT                (     3)  /* IM_GP4_EINT */
#define WM8325_IM_GP4_EINT_WIDTH                (     1)  /* IM_GP4_EINT */
#define WM8325_IM_GP3_EINT                      (0x0004)  /* IM_GP3_EINT */
#define WM8325_IM_GP3_EINT_MASK                 (0x0004)  /* IM_GP3_EINT */
#define WM8325_IM_GP3_EINT_SHIFT                (     2)  /* IM_GP3_EINT */
#define WM8325_IM_GP3_EINT_WIDTH                (     1)  /* IM_GP3_EINT */
#define WM8325_IM_GP2_EINT                      (0x0002)  /* IM_GP2_EINT */
#define WM8325_IM_GP2_EINT_MASK                 (0x0002)  /* IM_GP2_EINT */
#define WM8325_IM_GP2_EINT_SHIFT                (     1)  /* IM_GP2_EINT */
#define WM8325_IM_GP2_EINT_WIDTH                (     1)  /* IM_GP2_EINT */
#define WM8325_IM_GP1_EINT                      (0x0001)  /* IM_GP1_EINT */
#define WM8325_IM_GP1_EINT_MASK                 (0x0001)  /* IM_GP1_EINT */
#define WM8325_IM_GP1_EINT_SHIFT                (     0)  /* IM_GP1_EINT */
#define WM8325_IM_GP1_EINT_WIDTH                (     1)  /* IM_GP1_EINT */
                                               
/*                                                                                                          
 * R16416 (0x4020) - RTC Write Counter                                                 
 */                                             
#define WM8325_RTC_WR_CNT_MASK                  (0xFFFF)  /* RTC_WR_CNT - [15:0] */
#define WM8325_RTC_WR_CNT_SHIFT                 (     0)  /* RTC_WR_CNT - [15:0] */
#define WM8325_RTC_WR_CNT_WIDTH                 (    16)  /* RTC_WR_CNT - [15:0] */
                                               
/*                                                                                                          
 * R16417 (0x4021) - RTC Time 1                                                             
 */                                             
#define WM8325_RTC_TIME_1_MASK                  (0xFFFF)  /* RTC_TIME - [15:0] */
#define WM8325_RTC_TIME_1_SHIFT                 (     0)  /* RTC_TIME - [15:0] */
#define WM8325_RTC_TIME_1_WIDTH                 (    16)  /* RTC_TIME - [15:0] */
                                             
/*                                                                                                      
 * R16418 (0x4022) - RTC Time 2                                                        
 */                                           
#define WM8325_RTC_TIME_MASK                    (0xFFFF)  /* RTC_TIME - [15:0] */
#define WM8325_RTC_TIME_SHIFT                   (     0)  /* RTC_TIME - [15:0] */
#define WM8325_RTC_TIME_WIDTH                   (    16)  /* RTC_TIME - [15:0] */
                                               
/*                                                                                                          
 * R16419 (0x4023) - RTC Alarm 1                                                            
 */                                             
#define WM8325_RTC_ALM_1_MASK                   (0xFFFF)  /* RTC_ALM - [15:0] */
#define WM8325_RTC_ALM_1_SHIFT                  (     0)  /* RTC_ALM - [15:0] */
#define WM8325_RTC_ALM_1_WIDTH                  (    16)  /* RTC_ALM - [15:0] */
                                             
/*                                                                                                      
 * R16420 (0x4024) - RTC Alarm 2                                                      
 */                                           
#define WM8325_RTC_ALM_MASK                     (0xFFFF)  /* RTC_ALM - [15:0] */
#define WM8325_RTC_ALM_SHIFT                    (     0)  /* RTC_ALM - [15:0] */
#define WM8325_RTC_ALM_WIDTH                    (    16)  /* RTC_ALM - [15:0] */
                                              
/*                                                                                                        
 * R16421 (0x4025) - RTC Control                                                          
 */                                            
#define WM8325_RTC_VALID                        (0x8000)  /* RTC_VALID */
#define WM8325_RTC_VALID_MASK                   (0x8000)  /* RTC_VALID */
#define WM8325_RTC_VALID_SHIFT                  (    15)  /* RTC_VALID */
#define WM8325_RTC_VALID_WIDTH                  (     1)  /* RTC_VALID */
#define WM8325_RTC_SYNC_BUSY                    (0x4000)  /* RTC_SYNC_BUSY */
#define WM8325_RTC_SYNC_BUSY_MASK               (0x4000)  /* RTC_SYNC_BUSY */
#define WM8325_RTC_SYNC_BUSY_SHIFT              (    14)  /* RTC_SYNC_BUSY */
#define WM8325_RTC_SYNC_BUSY_WIDTH              (     1)  /* RTC_SYNC_BUSY */
#define WM8325_RTC_ALM_ENA                      (0x0400)  /* RTC_ALM_ENA */
#define WM8325_RTC_ALM_ENA_MASK                 (0x0400)  /* RTC_ALM_ENA */
#define WM8325_RTC_ALM_ENA_SHIFT                (    10)  /* RTC_ALM_ENA */
#define WM8325_RTC_ALM_ENA_WIDTH                (     1)  /* RTC_ALM_ENA */
#define WM8325_RTC_PINT_FREQ_MASK               (0x0070)  /* RTC_PINT_FREQ - [6:4] */
#define WM8325_RTC_PINT_FREQ_SHIFT              (     4)  /* RTC_PINT_FREQ - [6:4] */
#define WM8325_RTC_PINT_FREQ_WIDTH              (     3)  /* RTC_PINT_FREQ - [6:4] */
                                             
/*                                                                                                      
 * R16422 (0x4026) - RTC Trim                                                           
 */                                           
#define WM8325_RTC_TRIM_MASK                    (0x03FF)  /* RTC_TRIM - [9:0] */
#define WM8325_RTC_TRIM_SHIFT                   (     0)  /* RTC_TRIM - [9:0] */
#define WM8325_RTC_TRIM_WIDTH                   (    10)  /* RTC_TRIM - [9:0] */
                                             
/*                                                                                                      
 * R16429 (0x402D) - AuxADC Data                                                    
 */                                           
#define WM8325_AUX_DATA_SRC_MASK                (0xF000)  /* AUX_DATA_SRC - [15:12] */
#define WM8325_AUX_DATA_SRC_SHIFT               (    12)  /* AUX_DATA_SRC - [15:12] */
#define WM8325_AUX_DATA_SRC_WIDTH               (     4)  /* AUX_DATA_SRC - [15:12] */
#define WM8325_AUX_DATA_MASK                    (0x0FFF)  /* AUX_DATA - [11:0] */
#define WM8325_AUX_DATA_SHIFT                   (     0)  /* AUX_DATA - [11:0] */
#define WM8325_AUX_DATA_WIDTH                   (    12)  /* AUX_DATA - [11:0] */
                                             
/*                                                                                                      
 * R16430 (0x402E) - AuxADC Control                                                 
 */                                           
#define WM8325_AUX_ENA                          (0x8000)  /* AUX_ENA */
#define WM8325_AUX_ENA_MASK                     (0x8000)  /* AUX_ENA */
#define WM8325_AUX_ENA_SHIFT                    (    15)  /* AUX_ENA */
#define WM8325_AUX_ENA_WIDTH                    (     1)  /* AUX_ENA */
#define WM8325_AUX_CVT_ENA                      (0x4000)  /* AUX_CVT_ENA */
#define WM8325_AUX_CVT_ENA_MASK                 (0x4000)  /* AUX_CVT_ENA */
#define WM8325_AUX_CVT_ENA_SHIFT                (    14)  /* AUX_CVT_ENA */
#define WM8325_AUX_CVT_ENA_WIDTH                (     1)  /* AUX_CVT_ENA */
#define WM8325_AUX_SLPENA                       (0x1000)  /* AUX_SLPENA */
#define WM8325_AUX_SLPENA_MASK                  (0x1000)  /* AUX_SLPENA */
#define WM8325_AUX_SLPENA_SHIFT                 (    12)  /* AUX_SLPENA */
#define WM8325_AUX_SLPENA_WIDTH                 (     1)  /* AUX_SLPENA */
#define WM8325_AUX_RATE_MASK                    (0x003F)  /* AUX_RATE - [5:0] */
#define WM8325_AUX_RATE_SHIFT                   (     0)  /* AUX_RATE - [5:0] */
#define WM8325_AUX_RATE_WIDTH                   (     6)  /* AUX_RATE - [5:0] */
                                             
/*                                                                                                      
 * R16431 (0x402F) - AuxADC Source                                                  
 */                                           
#define WM8325_AUX_BKUP_BATT_SEL                (0x0400)  /* AUX_BKUP_BATT_SEL */
#define WM8325_AUX_BKUP_BATT_SEL_MASK           (0x0400)  /* AUX_BKUP_BATT_SEL */
#define WM8325_AUX_BKUP_BATT_SEL_SHIFT          (    10)  /* AUX_BKUP_BATT_SEL */
#define WM8325_AUX_BKUP_BATT_SEL_WIDTH          (     1)  /* AUX_BKUP_BATT_SEL */
#define WM8325_AUX_PVDD_SEL                     (0x0040)  /* AUX_PVDD_SEL */
#define WM8325_AUX_PVDD_SEL_MASK                (0x0040)  /* AUX_PVDD_SEL */
#define WM8325_AUX_PVDD_SEL_SHIFT               (     6)  /* AUX_PVDD_SEL */
#define WM8325_AUX_PVDD_SEL_WIDTH               (     1)  /* AUX_PVDD_SEL */
#define WM8325_AUX_CHIP_TEMP_SEL                (0x0010)  /* AUX_CHIP_TEMP_SEL */
#define WM8325_AUX_CHIP_TEMP_SEL_MASK           (0x0010)  /* AUX_CHIP_TEMP_SEL */
#define WM8325_AUX_CHIP_TEMP_SEL_SHIFT          (     4)  /* AUX_CHIP_TEMP_SEL */
#define WM8325_AUX_CHIP_TEMP_SEL_WIDTH          (     1)  /* AUX_CHIP_TEMP_SEL */
#define WM8325_AUX_GPIO12_SEL                   (0x0004)  /* AUX_GPIO12_SEL */
#define WM8325_AUX_GPIO12_SEL_MASK              (0x0004)  /* AUX_GPIO12_SEL */
#define WM8325_AUX_GPIO12_SEL_SHIFT             (     2)  /* AUX_GPIO12_SEL */
#define WM8325_AUX_GPIO12_SEL_WIDTH             (     1)  /* AUX_GPIO12_SEL */
#define WM8325_AUX_GPIO11_SEL                   (0x0002)  /* AUX_GPIO11_SEL */
#define WM8325_AUX_GPIO11_SEL_MASK              (0x0002)  /* AUX_GPIO11_SEL */
#define WM8325_AUX_GPIO11_SEL_SHIFT             (     1)  /* AUX_GPIO11_SEL */
#define WM8325_AUX_GPIO11_SEL_WIDTH             (     1)  /* AUX_GPIO11_SEL */
#define WM8325_AUX_GPIO10_SEL                   (0x0001)  /* AUX_GPIO10_SEL */
#define WM8325_AUX_GPIO10_SEL_MASK              (0x0001)  /* AUX_GPIO10_SEL */
#define WM8325_AUX_GPIO10_SEL_SHIFT             (     0)  /* AUX_GPIO10_SEL */
#define WM8325_AUX_GPIO10_SEL_WIDTH             (     1)  /* AUX_GPIO10_SEL */
                                              
/*                                                                                                        
 * R16432 (0x4030) - Comparator Control                                               
 */                                            
#define WM8325_DCOMP4_STS                       (0x0800)  /* DCOMP4_STS */
#define WM8325_DCOMP4_STS_MASK                  (0x0800)  /* DCOMP4_STS */
#define WM8325_DCOMP4_STS_SHIFT                 (    11)  /* DCOMP4_STS */
#define WM8325_DCOMP4_STS_WIDTH                 (     1)  /* DCOMP4_STS */
#define WM8325_DCOMP3_STS                       (0x0400)  /* DCOMP3_STS */
#define WM8325_DCOMP3_STS_MASK                  (0x0400)  /* DCOMP3_STS */
#define WM8325_DCOMP3_STS_SHIFT                 (    10)  /* DCOMP3_STS */
#define WM8325_DCOMP3_STS_WIDTH                 (     1)  /* DCOMP3_STS */
#define WM8325_DCOMP2_STS                       (0x0200)  /* DCOMP2_STS */
#define WM8325_DCOMP2_STS_MASK                  (0x0200)  /* DCOMP2_STS */
#define WM8325_DCOMP2_STS_SHIFT                 (     9)  /* DCOMP2_STS */
#define WM8325_DCOMP2_STS_WIDTH                 (     1)  /* DCOMP2_STS */
#define WM8325_DCOMP1_STS                       (0x0100)  /* DCOMP1_STS */
#define WM8325_DCOMP1_STS_MASK                  (0x0100)  /* DCOMP1_STS */
#define WM8325_DCOMP1_STS_SHIFT                 (     8)  /* DCOMP1_STS */
#define WM8325_DCOMP1_STS_WIDTH                 (     1)  /* DCOMP1_STS */
#define WM8325_DCMP4_ENA                        (0x0008)  /* DCMP4_ENA */
#define WM8325_DCMP4_ENA_MASK                   (0x0008)  /* DCMP4_ENA */
#define WM8325_DCMP4_ENA_SHIFT                  (     3)  /* DCMP4_ENA */
#define WM8325_DCMP4_ENA_WIDTH                  (     1)  /* DCMP4_ENA */
#define WM8325_DCMP3_ENA                        (0x0004)  /* DCMP3_ENA */
#define WM8325_DCMP3_ENA_MASK                   (0x0004)  /* DCMP3_ENA */
#define WM8325_DCMP3_ENA_SHIFT                  (     2)  /* DCMP3_ENA */
#define WM8325_DCMP3_ENA_WIDTH                  (     1)  /* DCMP3_ENA */
#define WM8325_DCMP2_ENA                        (0x0002)  /* DCMP2_ENA */
#define WM8325_DCMP2_ENA_MASK                   (0x0002)  /* DCMP2_ENA */
#define WM8325_DCMP2_ENA_SHIFT                  (     1)  /* DCMP2_ENA */
#define WM8325_DCMP2_ENA_WIDTH                  (     1)  /* DCMP2_ENA */
#define WM8325_DCMP1_ENA                        (0x0001)  /* DCMP1_ENA */
#define WM8325_DCMP1_ENA_MASK                   (0x0001)  /* DCMP1_ENA */
#define WM8325_DCMP1_ENA_SHIFT                  (     0)  /* DCMP1_ENA */
#define WM8325_DCMP1_ENA_WIDTH                  (     1)  /* DCMP1_ENA */
                                              
/*                                                                                                        
 * R16433 (0x4031) - Comparator 1                                                        
 */                                            
#define WM8325_DCMP1_SRC_MASK                   (0xE000)  /* DCMP1_SRC - [15:13] */
#define WM8325_DCMP1_SRC_SHIFT                  (    13)  /* DCMP1_SRC - [15:13] */
#define WM8325_DCMP1_SRC_WIDTH                  (     3)  /* DCMP1_SRC - [15:13] */
#define WM8325_DCMP1_GT                         (0x1000)  /* DCMP1_GT */
#define WM8325_DCMP1_GT_MASK                    (0x1000)  /* DCMP1_GT */
#define WM8325_DCMP1_GT_SHIFT                   (    12)  /* DCMP1_GT */
#define WM8325_DCMP1_GT_WIDTH                   (     1)  /* DCMP1_GT */
#define WM8325_DCMP1_THR_MASK                   (0x0FFF)  /* DCMP1_THR - [11:0] */
#define WM8325_DCMP1_THR_SHIFT                  (     0)  /* DCMP1_THR - [11:0] */
#define WM8325_DCMP1_THR_WIDTH                  (    12)  /* DCMP1_THR - [11:0] */
                                              
/*                                                                                                        
 * R16434 (0x4032) - Comparator 2                                                       
 */                                            
#define WM8325_DCMP2_SRC_MASK                   (0xE000)  /* DCMP2_SRC - [15:13] */
#define WM8325_DCMP2_SRC_SHIFT                  (    13)  /* DCMP2_SRC - [15:13] */
#define WM8325_DCMP2_SRC_WIDTH                  (     3)  /* DCMP2_SRC - [15:13] */
#define WM8325_DCMP2_GT                         (0x1000)  /* DCMP2_GT */
#define WM8325_DCMP2_GT_MASK                    (0x1000)  /* DCMP2_GT */
#define WM8325_DCMP2_GT_SHIFT                   (    12)  /* DCMP2_GT */
#define WM8325_DCMP2_GT_WIDTH                   (     1)  /* DCMP2_GT */
#define WM8325_DCMP2_THR_MASK                   (0x0FFF)  /* DCMP2_THR - [11:0] */
#define WM8325_DCMP2_THR_SHIFT                  (     0)  /* DCMP2_THR - [11:0] */
#define WM8325_DCMP2_THR_WIDTH                  (    12)  /* DCMP2_THR - [11:0] */
                                            
/*                                                                                                   
 * R16435 (0x4033) - Comparator 3                                                  
 */                                          
#define WM8325_DCMP3_SRC_MASK                   (0xE000)  /* DCMP3_SRC - [15:13] */
#define WM8325_DCMP3_SRC_SHIFT                  (    13)  /* DCMP3_SRC - [15:13] */
#define WM8325_DCMP3_SRC_WIDTH                  (     3)  /* DCMP3_SRC - [15:13] */
#define WM8325_DCMP3_GT                         (0x1000)  /* DCMP3_GT */
#define WM8325_DCMP3_GT_MASK                    (0x1000)  /* DCMP3_GT */
#define WM8325_DCMP3_GT_SHIFT                   (    12)  /* DCMP3_GT */
#define WM8325_DCMP3_GT_WIDTH                   (     1)  /* DCMP3_GT */
#define WM8325_DCMP3_THR_MASK                   (0x0FFF)  /* DCMP3_THR - [11:0] */
#define WM8325_DCMP3_THR_SHIFT                  (     0)  /* DCMP3_THR - [11:0] */
#define WM8325_DCMP3_THR_WIDTH                  (    12)  /* DCMP3_THR - [11:0] */
                                              
/*                                                                                                        
 * R16436 (0x4034) - Comparator 4                                                       
 */                                            
#define WM8325_DCMP4_SRC_MASK                   (0xE000)  /* DCMP4_SRC - [15:13] */
#define WM8325_DCMP4_SRC_SHIFT                  (    13)  /* DCMP4_SRC - [15:13] */
#define WM8325_DCMP4_SRC_WIDTH                  (     3)  /* DCMP4_SRC - [15:13] */
#define WM8325_DCMP4_GT                         (0x1000)  /* DCMP4_GT */
#define WM8325_DCMP4_GT_MASK                    (0x1000)  /* DCMP4_GT */
#define WM8325_DCMP4_GT_SHIFT                   (    12)  /* DCMP4_GT */
#define WM8325_DCMP4_GT_WIDTH                   (     1)  /* DCMP4_GT */
#define WM8325_DCMP4_THR_MASK                   (0x0FFF)  /* DCMP4_THR - [11:0] */
#define WM8325_DCMP4_THR_SHIFT                  (     0)  /* DCMP4_THR - [11:0] */
#define WM8325_DCMP4_THR_WIDTH                  (    12)  /* DCMP4_THR - [11:0] */
                                              
/*                                                                                                        
 * R16440 (0x4038) - GPIO1 Control                                                     
 */                                            
                                              
                                              
                                              
                                              
                                              
#define WM8325_GP1_DIR                          (0x8000)  /* GP1_DIR */
#define WM8325_GP1_DIR_MASK                     (0x8000)  /* GP1_DIR */
#define WM8325_GP1_DIR_SHIFT                    (    15)  /* GP1_DIR */
#define WM8325_GP1_DIR_WIDTH                    (     1)  /* GP1_DIR */
#define WM8325_GP1_PULL_MASK                    (0x6000)  /* GP1_PULL - [14:13] */
#define WM8325_GP1_PULL_SHIFT                   (    13)  /* GP1_PULL - [14:13] */
#define WM8325_GP1_PULL_WIDTH                   (     2)  /* GP1_PULL - [14:13] */
#define WM8325_GP1_INT_MODE                     (0x1000)  /* GP1_INT_MODE */
#define WM8325_GP1_INT_MODE_MASK                (0x1000)  /* GP1_INT_MODE */
#define WM8325_GP1_INT_MODE_SHIFT               (    12)  /* GP1_INT_MODE */
#define WM8325_GP1_INT_MODE_WIDTH               (     1)  /* GP1_INT_MODE */
#define WM8325_GP1_PWR_DOM                      (0x0800)  /* GP1_PWR_DOM */
#define WM8325_GP1_PWR_DOM_MASK                 (0x0800)  /* GP1_PWR_DOM */
#define WM8325_GP1_PWR_DOM_SHIFT                (    11)  /* GP1_PWR_DOM */
#define WM8325_GP1_PWR_DOM_WIDTH                (     1)  /* GP1_PWR_DOM */
#define WM8325_GP1_POL                          (0x0400)  /* GP1_POL */
#define WM8325_GP1_POL_MASK                     (0x0400)  /* GP1_POL */
#define WM8325_GP1_POL_SHIFT                    (    10)  /* GP1_POL */
#define WM8325_GP1_POL_WIDTH                    (     1)  /* GP1_POL */
#define WM8325_GP1_OD                           (0x0200)  /* GP1_OD */
#define WM8325_GP1_OD_MASK                      (0x0200)  /* GP1_OD */
#define WM8325_GP1_OD_SHIFT                     (     9)  /* GP1_OD */
#define WM8325_GP1_OD_WIDTH                     (     1)  /* GP1_OD */
#define WM8325_GP1_ENA                          (0x0080)  /* GP1_ENA */
#define WM8325_GP1_ENA_MASK                     (0x0080)  /* GP1_ENA */
#define WM8325_GP1_ENA_SHIFT                    (     7)  /* GP1_ENA */
#define WM8325_GP1_ENA_WIDTH                    (     1)  /* GP1_ENA */
#define WM8325_GP1_FN_MASK                      (0x000F)  /* GP1_FN - [3:0] */
#define WM8325_GP1_FN_SHIFT                     (     0)  /* GP1_FN - [3:0] */
#define WM8325_GP1_FN_WIDTH                     (     4)  /* GP1_FN - [3:0] */

//---------------------------------------------------------------------------------------------------------------------------------------------------------- GPIO DEFINES ======================================

#define WM8325_GPx_DIR_IN                       (1 << WM8325_GP1_DIR_SHIFT) 
#define WM8325_GPx_DIR_OUT                      (0 << WM8325_GP1_DIR_SHIFT) 
#define WM8325_GPx_NO_PULL                      (0 << WM8325_GP1_PULL_SHIFT) 
#define WM8325_GPx_PULL_DOWN                    (1 << WM8325_GP1_PULL_SHIFT)
#define WM8325_GPx_PULL_UP                      (2 << WM8325_GP1_PULL_SHIFT) 
#define WM8325_GPx_INT_MODE_POL_LEVEL           (0 << WM8325_GP1_INT_MODE_SHIFT)
#define WM8325_GPx_INT_MODE_BOTH_LEVELS         (1 << WM8325_GP1_INT_MODE_SHIFT)
#define WM8325_GPx_PWR_DOM_DBVDD                (0 << WM8325_GP1_PWR_DOM_SHIFT)
#define WM8325_GPx_PWR_DOM_LDO12_PVDD           (1 << WM8325_GP1_PWR_DOM_SHIFT)
#define WM8325_GPx_POL_NORMAL                   (1<<WM8325_GP1_POL_SHIFT)
#define WM8325_GPx_POL_INVERTED                 (0<<WM8325_GP1_POL_SHIFT)
#define WM8325_GPx_OD_CMOS                      (0<<WM8325_GP1_POL_SHIFT)
#define WM8325_GPx_OD_OPEN_DRAIN                (1<<WM8325_GP1_POL_SHIFT)
#define WM8325_GPx_ENABLE                       (1<<WM8325_GP1_ENA_SHIFT) 
#define WM8325_GPx_DISABLE                      (0<<WM8325_GP1_ENA_SHIFT) 
#define WM8325_GPx_FN_GPIO                      (0<<WM8325_GP1_FN_SHIFT) 


#define WM8325_GPIO_NORMAL_OUT_MODE (WM8325_GPx_DIR_OUT |  \
                                     WM8325_GPx_NO_PULL | \
                                     WM8325_GPx_INT_MODE_POL_LEVEL | \
                                     WM8325_GPx_PWR_DOM_DBVDD | \
                                     WM8325_GPx_POL_NORMAL | \
                                     WM8325_GPx_OD_CMOS | \
                                     WM8325_GPx_ENABLE | \
                                     WM8325_GPx_FN_GPIO)

#define WM8325_GPIO_NORMAL_IN_MODE  (WM8325_GPx_DIR_IN |  \
                                     WM8325_GPx_NO_PULL | \
                                     WM8325_GPx_INT_MODE_POL_LEVEL | \
                                     WM8325_GPx_PWR_DOM_DBVDD | \
                                     WM8325_GPx_POL_NORMAL | \
                                     WM8325_GPx_OD_CMOS | \
                                     WM8325_GPx_ENABLE | \
                                     WM8325_GPx_FN_GPIO)
                              

//---------------------------------------------------------------------------------------------------------------------------------------------------------- GPIO DEFINES ======================================                              
/*
 * R16441 (0x4039) - GPIO2 Control
 */
#define WM8325_GP2_DIR                          (0x8000)  /* GP2_DIR */
#define WM8325_GP2_DIR_MASK                     (0x8000)  /* GP2_DIR */
#define WM8325_GP2_DIR_SHIFT                    (    15)  /* GP2_DIR */
#define WM8325_GP2_DIR_WIDTH                    (     1)  /* GP2_DIR */
#define WM8325_GP2_PULL_MASK                    (0x6000)  /* GP2_PULL - [14:13] */
#define WM8325_GP2_PULL_SHIFT                   (    13)  /* GP2_PULL - [14:13] */
#define WM8325_GP2_PULL_WIDTH                   (     2)  /* GP2_PULL - [14:13] */
#define WM8325_GP2_INT_MODE                     (0x1000)  /* GP2_INT_MODE */
#define WM8325_GP2_INT_MODE_MASK                (0x1000)  /* GP2_INT_MODE */
#define WM8325_GP2_INT_MODE_SHIFT               (    12)  /* GP2_INT_MODE */
#define WM8325_GP2_INT_MODE_WIDTH               (     1)  /* GP2_INT_MODE */
#define WM8325_GP2_PWR_DOM                      (0x0800)  /* GP2_PWR_DOM */
#define WM8325_GP2_PWR_DOM_MASK                 (0x0800)  /* GP2_PWR_DOM */
#define WM8325_GP2_PWR_DOM_SHIFT                (    11)  /* GP2_PWR_DOM */
#define WM8325_GP2_PWR_DOM_WIDTH                (     1)  /* GP2_PWR_DOM */
#define WM8325_GP2_POL                          (0x0400)  /* GP2_POL */
#define WM8325_GP2_POL_MASK                     (0x0400)  /* GP2_POL */
#define WM8325_GP2_POL_SHIFT                    (    10)  /* GP2_POL */
#define WM8325_GP2_POL_WIDTH                    (     1)  /* GP2_POL */
#define WM8325_GP2_OD                           (0x0200)  /* GP2_OD */
#define WM8325_GP2_OD_MASK                      (0x0200)  /* GP2_OD */
#define WM8325_GP2_OD_SHIFT                     (     9)  /* GP2_OD */
#define WM8325_GP2_OD_WIDTH                     (     1)  /* GP2_OD */
#define WM8325_GP2_ENA                          (0x0080)  /* GP2_ENA */
#define WM8325_GP2_ENA_MASK                     (0x0080)  /* GP2_ENA */
#define WM8325_GP2_ENA_SHIFT                    (     7)  /* GP2_ENA */
#define WM8325_GP2_ENA_WIDTH                    (     1)  /* GP2_ENA */
#define WM8325_GP2_FN_MASK                      (0x000F)  /* GP2_FN - [3:0] */
#define WM8325_GP2_FN_SHIFT                     (     0)  /* GP2_FN - [3:0] */
#define WM8325_GP2_FN_WIDTH                     (     4)  /* GP2_FN - [3:0] */
                                             
/*                                                                                                      
 * R16442 (0x403A) - GPIO3 Control                                                   
 */                                           
#define WM8325_GP3_DIR                          (0x8000)  /* GP3_DIR */
#define WM8325_GP3_DIR_MASK                     (0x8000)  /* GP3_DIR */
#define WM8325_GP3_DIR_SHIFT                    (    15)  /* GP3_DIR */
#define WM8325_GP3_DIR_WIDTH                    (     1)  /* GP3_DIR */
#define WM8325_GP3_PULL_MASK                    (0x6000)  /* GP3_PULL - [14:13] */
#define WM8325_GP3_PULL_SHIFT                   (    13)  /* GP3_PULL - [14:13] */
#define WM8325_GP3_PULL_WIDTH                   (     2)  /* GP3_PULL - [14:13] */
#define WM8325_GP3_INT_MODE                     (0x1000)  /* GP3_INT_MODE */
#define WM8325_GP3_INT_MODE_MASK                (0x1000)  /* GP3_INT_MODE */
#define WM8325_GP3_INT_MODE_SHIFT               (    12)  /* GP3_INT_MODE */
#define WM8325_GP3_INT_MODE_WIDTH               (     1)  /* GP3_INT_MODE */
#define WM8325_GP3_PWR_DOM                      (0x0800)  /* GP3_PWR_DOM */
#define WM8325_GP3_PWR_DOM_MASK                 (0x0800)  /* GP3_PWR_DOM */
#define WM8325_GP3_PWR_DOM_SHIFT                (    11)  /* GP3_PWR_DOM */
#define WM8325_GP3_PWR_DOM_WIDTH                (     1)  /* GP3_PWR_DOM */
#define WM8325_GP3_POL                          (0x0400)  /* GP3_POL */
#define WM8325_GP3_POL_MASK                     (0x0400)  /* GP3_POL */
#define WM8325_GP3_POL_SHIFT                    (    10)  /* GP3_POL */
#define WM8325_GP3_POL_WIDTH                    (     1)  /* GP3_POL */
#define WM8325_GP3_OD                           (0x0200)  /* GP3_OD */
#define WM8325_GP3_OD_MASK                      (0x0200)  /* GP3_OD */
#define WM8325_GP3_OD_SHIFT                     (     9)  /* GP3_OD */
#define WM8325_GP3_OD_WIDTH                     (     1)  /* GP3_OD */
#define WM8325_GP3_ENA                          (0x0080)  /* GP3_ENA */
#define WM8325_GP3_ENA_MASK                     (0x0080)  /* GP3_ENA */
#define WM8325_GP3_ENA_SHIFT                    (     7)  /* GP3_ENA */
#define WM8325_GP3_ENA_WIDTH                    (     1)  /* GP3_ENA */
#define WM8325_GP3_FN_MASK                      (0x000F)  /* GP3_FN - [3:0] */
#define WM8325_GP3_FN_SHIFT                     (     0)  /* GP3_FN - [3:0] */
#define WM8325_GP3_FN_WIDTH                     (     4)  /* GP3_FN - [3:0] */
                                             
/*                                                                                                      
 * R16443 (0x403B) - GPIO4 Control                                                   
 */                                           
#define WM8325_GP4_DIR                          (0x8000)  /* GP4_DIR */
#define WM8325_GP4_DIR_MASK                     (0x8000)  /* GP4_DIR */
#define WM8325_GP4_DIR_SHIFT                    (    15)  /* GP4_DIR */
#define WM8325_GP4_DIR_WIDTH                    (     1)  /* GP4_DIR */
#define WM8325_GP4_PULL_MASK                    (0x6000)  /* GP4_PULL - [14:13] */
#define WM8325_GP4_PULL_SHIFT                   (    13)  /* GP4_PULL - [14:13] */
#define WM8325_GP4_PULL_WIDTH                   (     2)  /* GP4_PULL - [14:13] */
#define WM8325_GP4_INT_MODE                     (0x1000)  /* GP4_INT_MODE */
#define WM8325_GP4_INT_MODE_MASK                (0x1000)  /* GP4_INT_MODE */
#define WM8325_GP4_INT_MODE_SHIFT               (    12)  /* GP4_INT_MODE */
#define WM8325_GP4_INT_MODE_WIDTH               (     1)  /* GP4_INT_MODE */
#define WM8325_GP4_PWR_DOM                      (0x0800)  /* GP4_PWR_DOM */
#define WM8325_GP4_PWR_DOM_MASK                 (0x0800)  /* GP4_PWR_DOM */
#define WM8325_GP4_PWR_DOM_SHIFT                (    11)  /* GP4_PWR_DOM */
#define WM8325_GP4_PWR_DOM_WIDTH                (     1)  /* GP4_PWR_DOM */
#define WM8325_GP4_POL                          (0x0400)  /* GP4_POL */
#define WM8325_GP4_POL_MASK                     (0x0400)  /* GP4_POL */
#define WM8325_GP4_POL_SHIFT                    (    10)  /* GP4_POL */
#define WM8325_GP4_POL_WIDTH                    (     1)  /* GP4_POL */
#define WM8325_GP4_OD                           (0x0200)  /* GP4_OD */
#define WM8325_GP4_OD_MASK                      (0x0200)  /* GP4_OD */
#define WM8325_GP4_OD_SHIFT                     (     9)  /* GP4_OD */
#define WM8325_GP4_OD_WIDTH                     (     1)  /* GP4_OD */
#define WM8325_GP4_ENA                          (0x0080)  /* GP4_ENA */
#define WM8325_GP4_ENA_MASK                     (0x0080)  /* GP4_ENA */
#define WM8325_GP4_ENA_SHIFT                    (     7)  /* GP4_ENA */
#define WM8325_GP4_ENA_WIDTH                    (     1)  /* GP4_ENA */
#define WM8325_GP4_FN_MASK                      (0x000F)  /* GP4_FN - [3:0] */
#define WM8325_GP4_FN_SHIFT                     (     0)  /* GP4_FN - [3:0] */
#define WM8325_GP4_FN_WIDTH                     (     4)  /* GP4_FN - [3:0] */
                                              
/*                                                                                                        
 * R16444 (0x403C) - GPIO5 Control                                                     
 */                                            
#define WM8325_GP5_DIR                          (0x8000)  /* GP5_DIR */
#define WM8325_GP5_DIR_MASK                     (0x8000)  /* GP5_DIR */
#define WM8325_GP5_DIR_SHIFT                    (    15)  /* GP5_DIR */
#define WM8325_GP5_DIR_WIDTH                    (     1)  /* GP5_DIR */
#define WM8325_GP5_PULL_MASK                    (0x6000)  /* GP5_PULL - [14:13] */
#define WM8325_GP5_PULL_SHIFT                   (    13)  /* GP5_PULL - [14:13] */
#define WM8325_GP5_PULL_WIDTH                   (     2)  /* GP5_PULL - [14:13] */
#define WM8325_GP5_INT_MODE                     (0x1000)  /* GP5_INT_MODE */
#define WM8325_GP5_INT_MODE_MASK                (0x1000)  /* GP5_INT_MODE */
#define WM8325_GP5_INT_MODE_SHIFT               (    12)  /* GP5_INT_MODE */
#define WM8325_GP5_INT_MODE_WIDTH               (     1)  /* GP5_INT_MODE */
#define WM8325_GP5_PWR_DOM                      (0x0800)  /* GP5_PWR_DOM */
#define WM8325_GP5_PWR_DOM_MASK                 (0x0800)  /* GP5_PWR_DOM */
#define WM8325_GP5_PWR_DOM_SHIFT                (    11)  /* GP5_PWR_DOM */
#define WM8325_GP5_PWR_DOM_WIDTH                (     1)  /* GP5_PWR_DOM */
#define WM8325_GP5_POL                          (0x0400)  /* GP5_POL */
#define WM8325_GP5_POL_MASK                     (0x0400)  /* GP5_POL */
#define WM8325_GP5_POL_SHIFT                    (    10)  /* GP5_POL */
#define WM8325_GP5_POL_WIDTH                    (     1)  /* GP5_POL */
#define WM8325_GP5_OD                           (0x0200)  /* GP5_OD */
#define WM8325_GP5_OD_MASK                      (0x0200)  /* GP5_OD */
#define WM8325_GP5_OD_SHIFT                     (     9)  /* GP5_OD */
#define WM8325_GP5_OD_WIDTH                     (     1)  /* GP5_OD */
#define WM8325_GP5_ENA                          (0x0080)  /* GP5_ENA */
#define WM8325_GP5_ENA_MASK                     (0x0080)  /* GP5_ENA */
#define WM8325_GP5_ENA_SHIFT                    (     7)  /* GP5_ENA */
#define WM8325_GP5_ENA_WIDTH                    (     1)  /* GP5_ENA */
#define WM8325_GP5_FN_MASK                      (0x000F)  /* GP5_FN - [3:0] */
#define WM8325_GP5_FN_SHIFT                     (     0)  /* GP5_FN - [3:0] */
#define WM8325_GP5_FN_WIDTH                     (     4)  /* GP5_FN - [3:0] */
                                             
/*                                                                                                      
 * R16445 (0x403D) - GPIO6 Control                                                  
 */                                           
#define WM8325_GP6_DIR                          (0x8000)  /* GP6_DIR */
#define WM8325_GP6_DIR_MASK                     (0x8000)  /* GP6_DIR */
#define WM8325_GP6_DIR_SHIFT                    (    15)  /* GP6_DIR */
#define WM8325_GP6_DIR_WIDTH                    (     1)  /* GP6_DIR */
#define WM8325_GP6_PULL_MASK                    (0x6000)  /* GP6_PULL - [14:13] */
#define WM8325_GP6_PULL_SHIFT                   (    13)  /* GP6_PULL - [14:13] */
#define WM8325_GP6_PULL_WIDTH                   (     2)  /* GP6_PULL - [14:13] */
#define WM8325_GP6_INT_MODE                     (0x1000)  /* GP6_INT_MODE */
#define WM8325_GP6_INT_MODE_MASK                (0x1000)  /* GP6_INT_MODE */
#define WM8325_GP6_INT_MODE_SHIFT               (    12)  /* GP6_INT_MODE */
#define WM8325_GP6_INT_MODE_WIDTH               (     1)  /* GP6_INT_MODE */
#define WM8325_GP6_PWR_DOM                      (0x0800)  /* GP6_PWR_DOM */
#define WM8325_GP6_PWR_DOM_MASK                 (0x0800)  /* GP6_PWR_DOM */
#define WM8325_GP6_PWR_DOM_SHIFT                (    11)  /* GP6_PWR_DOM */
#define WM8325_GP6_PWR_DOM_WIDTH                (     1)  /* GP6_PWR_DOM */
#define WM8325_GP6_POL                          (0x0400)  /* GP6_POL */
#define WM8325_GP6_POL_MASK                     (0x0400)  /* GP6_POL */
#define WM8325_GP6_POL_SHIFT                    (    10)  /* GP6_POL */
#define WM8325_GP6_POL_WIDTH                    (     1)  /* GP6_POL */
#define WM8325_GP6_OD                           (0x0200)  /* GP6_OD */
#define WM8325_GP6_OD_MASK                      (0x0200)  /* GP6_OD */
#define WM8325_GP6_OD_SHIFT                     (     9)  /* GP6_OD */
#define WM8325_GP6_OD_WIDTH                     (     1)  /* GP6_OD */
#define WM8325_GP6_ENA                          (0x0080)  /* GP6_ENA */
#define WM8325_GP6_ENA_MASK                     (0x0080)  /* GP6_ENA */
#define WM8325_GP6_ENA_SHIFT                    (     7)  /* GP6_ENA */
#define WM8325_GP6_ENA_WIDTH                    (     1)  /* GP6_ENA */
#define WM8325_GP6_FN_MASK                      (0x000F)  /* GP6_FN - [3:0] */
#define WM8325_GP6_FN_SHIFT                     (     0)  /* GP6_FN - [3:0] */
#define WM8325_GP6_FN_WIDTH                     (     4)  /* GP6_FN - [3:0] */
                                             
/*                                                                                                      
 * R16446 (0x403E) - GPIO7 Control                                                   
 */                                           
#define WM8325_GP7_DIR                          (0x8000)  /* GP7_DIR */
#define WM8325_GP7_DIR_MASK                     (0x8000)  /* GP7_DIR */
#define WM8325_GP7_DIR_SHIFT                    (    15)  /* GP7_DIR */
#define WM8325_GP7_DIR_WIDTH                    (     1)  /* GP7_DIR */
#define WM8325_GP7_PULL_MASK                    (0x6000)  /* GP7_PULL - [14:13] */
#define WM8325_GP7_PULL_SHIFT                   (    13)  /* GP7_PULL - [14:13] */
#define WM8325_GP7_PULL_WIDTH                   (     2)  /* GP7_PULL - [14:13] */
#define WM8325_GP7_INT_MODE                     (0x1000)  /* GP7_INT_MODE */
#define WM8325_GP7_INT_MODE_MASK                (0x1000)  /* GP7_INT_MODE */
#define WM8325_GP7_INT_MODE_SHIFT               (    12)  /* GP7_INT_MODE */
#define WM8325_GP7_INT_MODE_WIDTH               (     1)  /* GP7_INT_MODE */
#define WM8325_GP7_PWR_DOM                      (0x0800)  /* GP7_PWR_DOM */
#define WM8325_GP7_PWR_DOM_MASK                 (0x0800)  /* GP7_PWR_DOM */
#define WM8325_GP7_PWR_DOM_SHIFT                (    11)  /* GP7_PWR_DOM */
#define WM8325_GP7_PWR_DOM_WIDTH                (     1)  /* GP7_PWR_DOM */
#define WM8325_GP7_POL                          (0x0400)  /* GP7_POL */
#define WM8325_GP7_POL_MASK                     (0x0400)  /* GP7_POL */
#define WM8325_GP7_POL_SHIFT                    (    10)  /* GP7_POL */
#define WM8325_GP7_POL_WIDTH                    (     1)  /* GP7_POL */
#define WM8325_GP7_OD                           (0x0200)  /* GP7_OD */
#define WM8325_GP7_OD_MASK                      (0x0200)  /* GP7_OD */
#define WM8325_GP7_OD_SHIFT                     (     9)  /* GP7_OD */
#define WM8325_GP7_OD_WIDTH                     (     1)  /* GP7_OD */
#define WM8325_GP7_ENA                          (0x0080)  /* GP7_ENA */
#define WM8325_GP7_ENA_MASK                     (0x0080)  /* GP7_ENA */
#define WM8325_GP7_ENA_SHIFT                    (     7)  /* GP7_ENA */
#define WM8325_GP7_ENA_WIDTH                    (     1)  /* GP7_ENA */
#define WM8325_GP7_FN_MASK                      (0x000F)  /* GP7_FN - [3:0] */
#define WM8325_GP7_FN_SHIFT                     (     0)  /* GP7_FN - [3:0] */
#define WM8325_GP7_FN_WIDTH                     (     4)  /* GP7_FN - [3:0] */
                                            
/*                                                                                                   
 * R16447 (0x403F) - GPIO8 Control                                                 
 */                                          
#define WM8325_GP8_DIR                          (0x8000)  /* GP8_DIR */
#define WM8325_GP8_DIR_MASK                     (0x8000)  /* GP8_DIR */
#define WM8325_GP8_DIR_SHIFT                    (    15)  /* GP8_DIR */
#define WM8325_GP8_DIR_WIDTH                    (     1)  /* GP8_DIR */
#define WM8325_GP8_PULL_MASK                    (0x6000)  /* GP8_PULL - [14:13] */
#define WM8325_GP8_PULL_SHIFT                   (    13)  /* GP8_PULL - [14:13] */
#define WM8325_GP8_PULL_WIDTH                   (     2)  /* GP8_PULL - [14:13] */
#define WM8325_GP8_INT_MODE                     (0x1000)  /* GP8_INT_MODE */
#define WM8325_GP8_INT_MODE_MASK                (0x1000)  /* GP8_INT_MODE */
#define WM8325_GP8_INT_MODE_SHIFT               (    12)  /* GP8_INT_MODE */
#define WM8325_GP8_INT_MODE_WIDTH               (     1)  /* GP8_INT_MODE */
#define WM8325_GP8_PWR_DOM                      (0x0800)  /* GP8_PWR_DOM */
#define WM8325_GP8_PWR_DOM_MASK                 (0x0800)  /* GP8_PWR_DOM */
#define WM8325_GP8_PWR_DOM_SHIFT                (    11)  /* GP8_PWR_DOM */
#define WM8325_GP8_PWR_DOM_WIDTH                (     1)  /* GP8_PWR_DOM */
#define WM8325_GP8_POL                          (0x0400)  /* GP8_POL */
#define WM8325_GP8_POL_MASK                     (0x0400)  /* GP8_POL */
#define WM8325_GP8_POL_SHIFT                    (    10)  /* GP8_POL */
#define WM8325_GP8_POL_WIDTH                    (     1)  /* GP8_POL */
#define WM8325_GP8_OD                           (0x0200)  /* GP8_OD */
#define WM8325_GP8_OD_MASK                      (0x0200)  /* GP8_OD */
#define WM8325_GP8_OD_SHIFT                     (     9)  /* GP8_OD */
#define WM8325_GP8_OD_WIDTH                     (     1)  /* GP8_OD */
#define WM8325_GP8_ENA                          (0x0080)  /* GP8_ENA */
#define WM8325_GP8_ENA_MASK                     (0x0080)  /* GP8_ENA */
#define WM8325_GP8_ENA_SHIFT                    (     7)  /* GP8_ENA */
#define WM8325_GP8_ENA_WIDTH                    (     1)  /* GP8_ENA */
#define WM8325_GP8_FN_MASK                      (0x000F)  /* GP8_FN - [3:0] */
#define WM8325_GP8_FN_SHIFT                     (     0)  /* GP8_FN - [3:0] */
#define WM8325_GP8_FN_WIDTH                     (     4)  /* GP8_FN - [3:0] */
                                          
/*                                                                                               
 * R16448 (0x4040) - GPIO9 Control                                            
 */                                        
#define WM8325_GP9_DIR                          (0x8000)  /* GP9_DIR */
#define WM8325_GP9_DIR_MASK                     (0x8000)  /* GP9_DIR */
#define WM8325_GP9_DIR_SHIFT                    (    15)  /* GP9_DIR */
#define WM8325_GP9_DIR_WIDTH                    (     1)  /* GP9_DIR */
#define WM8325_GP9_PULL_MASK                    (0x6000)  /* GP9_PULL - [14:13] */
#define WM8325_GP9_PULL_SHIFT                   (    13)  /* GP9_PULL - [14:13] */
#define WM8325_GP9_PULL_WIDTH                   (     2)  /* GP9_PULL - [14:13] */
#define WM8325_GP9_INT_MODE                     (0x1000)  /* GP9_INT_MODE */
#define WM8325_GP9_INT_MODE_MASK                (0x1000)  /* GP9_INT_MODE */
#define WM8325_GP9_INT_MODE_SHIFT               (    12)  /* GP9_INT_MODE */
#define WM8325_GP9_INT_MODE_WIDTH               (     1)  /* GP9_INT_MODE */
#define WM8325_GP9_PWR_DOM                      (0x0800)  /* GP9_PWR_DOM */
#define WM8325_GP9_PWR_DOM_MASK                 (0x0800)  /* GP9_PWR_DOM */
#define WM8325_GP9_PWR_DOM_SHIFT                (    11)  /* GP9_PWR_DOM */
#define WM8325_GP9_PWR_DOM_WIDTH                (     1)  /* GP9_PWR_DOM */
#define WM8325_GP9_POL                          (0x0400)  /* GP9_POL */
#define WM8325_GP9_POL_MASK                     (0x0400)  /* GP9_POL */
#define WM8325_GP9_POL_SHIFT                    (    10)  /* GP9_POL */
#define WM8325_GP9_POL_WIDTH                    (     1)  /* GP9_POL */
#define WM8325_GP9_OD                           (0x0200)  /* GP9_OD */
#define WM8325_GP9_OD_MASK                      (0x0200)  /* GP9_OD */
#define WM8325_GP9_OD_SHIFT                     (     9)  /* GP9_OD */
#define WM8325_GP9_OD_WIDTH                     (     1)  /* GP9_OD */
#define WM8325_GP9_ENA                          (0x0080)  /* GP9_ENA */
#define WM8325_GP9_ENA_MASK                     (0x0080)  /* GP9_ENA */
#define WM8325_GP9_ENA_SHIFT                    (     7)  /* GP9_ENA */
#define WM8325_GP9_ENA_WIDTH                    (     1)  /* GP9_ENA */
#define WM8325_GP9_FN_MASK                      (0x000F)  /* GP9_FN - [3:0] */
#define WM8325_GP9_FN_SHIFT                     (     0)  /* GP9_FN - [3:0] */
#define WM8325_GP9_FN_WIDTH                     (     4)  /* GP9_FN - [3:0] */
                                            
/*                                                                                                   
 * R16449 (0x4041) - GPIO10 Control                                               
 */                                          
#define WM8325_GP10_DIR                         (0x8000)  /* GP10_DIR */
#define WM8325_GP10_DIR_MASK                    (0x8000)  /* GP10_DIR */
#define WM8325_GP10_DIR_SHIFT                   (    15)  /* GP10_DIR */
#define WM8325_GP10_DIR_WIDTH                   (     1)  /* GP10_DIR */
#define WM8325_GP10_PULL_MASK                   (0x6000)  /* GP10_PULL - [14:13] */
#define WM8325_GP10_PULL_SHIFT                  (    13)  /* GP10_PULL - [14:13] */
#define WM8325_GP10_PULL_WIDTH                  (     2)  /* GP10_PULL - [14:13] */
#define WM8325_GP10_INT_MODE                    (0x1000)  /* GP10_INT_MODE */
#define WM8325_GP10_INT_MODE_MASK               (0x1000)  /* GP10_INT_MODE */
#define WM8325_GP10_INT_MODE_SHIFT              (    12)  /* GP10_INT_MODE */
#define WM8325_GP10_INT_MODE_WIDTH              (     1)  /* GP10_INT_MODE */
#define WM8325_GP10_PWR_DOM                     (0x0800)  /* GP10_PWR_DOM */
#define WM8325_GP10_PWR_DOM_MASK                (0x0800)  /* GP10_PWR_DOM */
#define WM8325_GP10_PWR_DOM_SHIFT               (    11)  /* GP10_PWR_DOM */
#define WM8325_GP10_PWR_DOM_WIDTH               (     1)  /* GP10_PWR_DOM */
#define WM8325_GP10_POL                         (0x0400)  /* GP10_POL */
#define WM8325_GP10_POL_MASK                    (0x0400)  /* GP10_POL */
#define WM8325_GP10_POL_SHIFT                   (    10)  /* GP10_POL */
#define WM8325_GP10_POL_WIDTH                   (     1)  /* GP10_POL */
#define WM8325_GP10_OD                          (0x0200)  /* GP10_OD */
#define WM8325_GP10_OD_MASK                     (0x0200)  /* GP10_OD */
#define WM8325_GP10_OD_SHIFT                    (     9)  /* GP10_OD */
#define WM8325_GP10_OD_WIDTH                    (     1)  /* GP10_OD */
#define WM8325_GP10_ENA                         (0x0080)  /* GP10_ENA */
#define WM8325_GP10_ENA_MASK                    (0x0080)  /* GP10_ENA */
#define WM8325_GP10_ENA_SHIFT                   (     7)  /* GP10_ENA */
#define WM8325_GP10_ENA_WIDTH                   (     1)  /* GP10_ENA */
#define WM8325_GP10_FN_MASK                     (0x000F)  /* GP10_FN - [3:0] */
#define WM8325_GP10_FN_SHIFT                    (     0)  /* GP10_FN - [3:0] */
#define WM8325_GP10_FN_WIDTH                    (     4)  /* GP10_FN - [3:0] */
                                            
/*                                                                                                   
 * R16450 (0x4042) - GPIO11 Control                                               
 */                                          
#define WM8325_GP11_DIR                         (0x8000)  /* GP11_DIR */
#define WM8325_GP11_DIR_MASK                    (0x8000)  /* GP11_DIR */
#define WM8325_GP11_DIR_SHIFT                   (    15)  /* GP11_DIR */
#define WM8325_GP11_DIR_WIDTH                   (     1)  /* GP11_DIR */
#define WM8325_GP11_PULL_MASK                   (0x6000)  /* GP11_PULL - [14:13] */
#define WM8325_GP11_PULL_SHIFT                  (    13)  /* GP11_PULL - [14:13] */
#define WM8325_GP11_PULL_WIDTH                  (     2)  /* GP11_PULL - [14:13] */
#define WM8325_GP11_INT_MODE                    (0x1000)  /* GP11_INT_MODE */
#define WM8325_GP11_INT_MODE_MASK               (0x1000)  /* GP11_INT_MODE */
#define WM8325_GP11_INT_MODE_SHIFT              (    12)  /* GP11_INT_MODE */
#define WM8325_GP11_INT_MODE_WIDTH              (     1)  /* GP11_INT_MODE */
#define WM8325_GP11_PWR_DOM                     (0x0800)  /* GP11_PWR_DOM */
#define WM8325_GP11_PWR_DOM_MASK                (0x0800)  /* GP11_PWR_DOM */
#define WM8325_GP11_PWR_DOM_SHIFT               (    11)  /* GP11_PWR_DOM */
#define WM8325_GP11_PWR_DOM_WIDTH               (     1)  /* GP11_PWR_DOM */
#define WM8325_GP11_POL                         (0x0400)  /* GP11_POL */
#define WM8325_GP11_POL_MASK                    (0x0400)  /* GP11_POL */
#define WM8325_GP11_POL_SHIFT                   (    10)  /* GP11_POL */
#define WM8325_GP11_POL_WIDTH                   (     1)  /* GP11_POL */
#define WM8325_GP11_OD                          (0x0200)  /* GP11_OD */
#define WM8325_GP11_OD_MASK                     (0x0200)  /* GP11_OD */
#define WM8325_GP11_OD_SHIFT                    (     9)  /* GP11_OD */
#define WM8325_GP11_OD_WIDTH                    (     1)  /* GP11_OD */
#define WM8325_GP11_ENA                         (0x0080)  /* GP11_ENA */
#define WM8325_GP11_ENA_MASK                    (0x0080)  /* GP11_ENA */
#define WM8325_GP11_ENA_SHIFT                   (     7)  /* GP11_ENA */
#define WM8325_GP11_ENA_WIDTH                   (     1)  /* GP11_ENA */
#define WM8325_GP11_FN_MASK                     (0x000F)  /* GP11_FN - [3:0] */
#define WM8325_GP11_FN_SHIFT                    (     0)  /* GP11_FN - [3:0] */
#define WM8325_GP11_FN_WIDTH                    (     4)  /* GP11_FN - [3:0] */
                                          
/*                                                                                               
 * R16451 (0x4043) - GPIO12 Control                                          
 */                                        
#define WM8325_GP12_DIR                         (0x8000)  /* GP12_DIR */
#define WM8325_GP12_DIR_MASK                    (0x8000)  /* GP12_DIR */
#define WM8325_GP12_DIR_SHIFT                   (    15)  /* GP12_DIR */
#define WM8325_GP12_DIR_WIDTH                   (     1)  /* GP12_DIR */
#define WM8325_GP12_PULL_MASK                   (0x6000)  /* GP12_PULL - [14:13] */
#define WM8325_GP12_PULL_SHIFT                  (    13)  /* GP12_PULL - [14:13] */
#define WM8325_GP12_PULL_WIDTH                  (     2)  /* GP12_PULL - [14:13] */
#define WM8325_GP12_INT_MODE                    (0x1000)  /* GP12_INT_MODE */
#define WM8325_GP12_INT_MODE_MASK               (0x1000)  /* GP12_INT_MODE */
#define WM8325_GP12_INT_MODE_SHIFT              (    12)  /* GP12_INT_MODE */
#define WM8325_GP12_INT_MODE_WIDTH              (     1)  /* GP12_INT_MODE */
#define WM8325_GP12_PWR_DOM                     (0x0800)  /* GP12_PWR_DOM */
#define WM8325_GP12_PWR_DOM_MASK                (0x0800)  /* GP12_PWR_DOM */
#define WM8325_GP12_PWR_DOM_SHIFT               (    11)  /* GP12_PWR_DOM */
#define WM8325_GP12_PWR_DOM_WIDTH               (     1)  /* GP12_PWR_DOM */
#define WM8325_GP12_POL                         (0x0400)  /* GP12_POL */
#define WM8325_GP12_POL_MASK                    (0x0400)  /* GP12_POL */
#define WM8325_GP12_POL_SHIFT                   (    10)  /* GP12_POL */
#define WM8325_GP12_POL_WIDTH                   (     1)  /* GP12_POL */
#define WM8325_GP12_OD                          (0x0200)  /* GP12_OD */
#define WM8325_GP12_OD_MASK                     (0x0200)  /* GP12_OD */
#define WM8325_GP12_OD_SHIFT                    (     9)  /* GP12_OD */
#define WM8325_GP12_OD_WIDTH                    (     1)  /* GP12_OD */
#define WM8325_GP12_ENA                         (0x0080)  /* GP12_ENA */
#define WM8325_GP12_ENA_MASK                    (0x0080)  /* GP12_ENA */
#define WM8325_GP12_ENA_SHIFT                   (     7)  /* GP12_ENA */
#define WM8325_GP12_ENA_WIDTH                   (     1)  /* GP12_ENA */
#define WM8325_GP12_FN_MASK                     (0x000F)  /* GP12_FN - [3:0] */
#define WM8325_GP12_FN_SHIFT                    (     0)  /* GP12_FN - [3:0] */
#define WM8325_GP12_FN_WIDTH                    (     4)  /* GP12_FN - [3:0] */
                                            
/*                                                                                                   
 * R16459 (0x404B) - Backup Charger Control                                    
 */                                          
#define WM8325_BKUP_CHG_ENA                     (0x8000)  /* BKUP_CHG_ENA */
#define WM8325_BKUP_CHG_ENA_MASK                (0x8000)  /* BKUP_CHG_ENA */
#define WM8325_BKUP_CHG_ENA_SHIFT               (    15)  /* BKUP_CHG_ENA */
#define WM8325_BKUP_CHG_ENA_WIDTH               (     1)  /* BKUP_CHG_ENA */
#define WM8325_BKUP_CHG_STS                     (0x4000)  /* BKUP_CHG_STS */
#define WM8325_BKUP_CHG_STS_MASK                (0x4000)  /* BKUP_CHG_STS */
#define WM8325_BKUP_CHG_STS_SHIFT               (    14)  /* BKUP_CHG_STS */
#define WM8325_BKUP_CHG_STS_WIDTH               (     1)  /* BKUP_CHG_STS */
#define WM8325_BKUP_CHG_MODE                    (0x1000)  /* BKUP_CHG_MODE */
#define WM8325_BKUP_CHG_MODE_MASK               (0x1000)  /* BKUP_CHG_MODE */
#define WM8325_BKUP_CHG_MODE_SHIFT              (    12)  /* BKUP_CHG_MODE */
#define WM8325_BKUP_CHG_MODE_WIDTH              (     1)  /* BKUP_CHG_MODE */
#define WM8325_BKUP_BATT_DET_ENA                (0x0800)  /* BKUP_BATT_DET_ENA */
#define WM8325_BKUP_BATT_DET_ENA_MASK           (0x0800)  /* BKUP_BATT_DET_ENA */
#define WM8325_BKUP_BATT_DET_ENA_SHIFT          (    11)  /* BKUP_BATT_DET_ENA */
#define WM8325_BKUP_BATT_DET_ENA_WIDTH          (     1)  /* BKUP_BATT_DET_ENA */
#define WM8325_BKUP_BATT_STS                    (0x0400)  /* BKUP_BATT_STS */
#define WM8325_BKUP_BATT_STS_MASK               (0x0400)  /* BKUP_BATT_STS */
#define WM8325_BKUP_BATT_STS_SHIFT              (    10)  /* BKUP_BATT_STS */
#define WM8325_BKUP_BATT_STS_WIDTH              (     1)  /* BKUP_BATT_STS */
#define WM8325_BKUP_CHG_VLIM                    (0x0010)  /* BKUP_CHG_VLIM */
#define WM8325_BKUP_CHG_VLIM_MASK               (0x0010)  /* BKUP_CHG_VLIM */
#define WM8325_BKUP_CHG_VLIM_SHIFT              (     4)  /* BKUP_CHG_VLIM */
#define WM8325_BKUP_CHG_VLIM_WIDTH              (     1)  /* BKUP_CHG_VLIM */
#define WM8325_BKUP_CHG_ILIM_MASK               (0x0003)  /* BKUP_CHG_ILIM - [1:0] */
#define WM8325_BKUP_CHG_ILIM_SHIFT              (     0)  /* BKUP_CHG_ILIM - [1:0] */
#define WM8325_BKUP_CHG_ILIM_WIDTH              (     2)  /* BKUP_CHG_ILIM - [1:0] */
                                              
/*                                                                                                        
 * R16460 (0x404C) - Status LED 1                                                       
 */                                            
#define WM8325_LED1_SRC_MASK                    (0xC000)  /* LED1_SRC - [15:14] */
#define WM8325_LED1_SRC_SHIFT                   (    14)  /* LED1_SRC - [15:14] */
#define WM8325_LED1_SRC_WIDTH                   (     2)  /* LED1_SRC - [15:14] */
#define WM8325_LED1_MODE_MASK                   (0x0300)  /* LED1_MODE - [9:8] */
#define WM8325_LED1_MODE_SHIFT                  (     8)  /* LED1_MODE - [9:8] */
#define WM8325_LED1_MODE_WIDTH                  (     2)  /* LED1_MODE - [9:8] */
#define WM8325_LED1_SEQ_LEN_MASK                (0x0030)  /* LED1_SEQ_LEN - [5:4] */
#define WM8325_LED1_SEQ_LEN_SHIFT               (     4)  /* LED1_SEQ_LEN - [5:4] */
#define WM8325_LED1_SEQ_LEN_WIDTH               (     2)  /* LED1_SEQ_LEN - [5:4] */
#define WM8325_LED1_DUR_MASK                    (0x000C)  /* LED1_DUR - [3:2] */
#define WM8325_LED1_DUR_SHIFT                   (     2)  /* LED1_DUR - [3:2] */
#define WM8325_LED1_DUR_WIDTH                   (     2)  /* LED1_DUR - [3:2] */
#define WM8325_LED1_DUTY_CYC_MASK               (0x0003)  /* LED1_DUTY_CYC - [1:0] */
#define WM8325_LED1_DUTY_CYC_SHIFT              (     0)  /* LED1_DUTY_CYC - [1:0] */
#define WM8325_LED1_DUTY_CYC_WIDTH              (     2)  /* LED1_DUTY_CYC - [1:0] */
                                             
/*                                                                                                      
 * R16461 (0x404D) - Status LED 2                                                    
 */                                                                               
#define WM8325_LED2_SRC_MASK                    (0xC000)  /* LED2_SRC - [15:14] */
#define WM8325_LED2_SRC_SHIFT                   (    14)  /* LED2_SRC - [15:14] */
#define WM8325_LED2_SRC_WIDTH                   (     2)  /* LED2_SRC - [15:14] */
#define WM8325_LED2_MODE_MASK                   (0x0300)  /* LED2_MODE - [9:8] */
#define WM8325_LED2_MODE_SHIFT                  (     8)  /* LED2_MODE - [9:8] */
#define WM8325_LED2_MODE_WIDTH                  (     2)  /* LED2_MODE - [9:8] */
#define WM8325_LED2_SEQ_LEN_MASK                (0x0030)  /* LED2_SEQ_LEN - [5:4] */
#define WM8325_LED2_SEQ_LEN_SHIFT               (     4)  /* LED2_SEQ_LEN - [5:4] */
#define WM8325_LED2_SEQ_LEN_WIDTH               (     2)  /* LED2_SEQ_LEN - [5:4] */
#define WM8325_LED2_DUR_MASK                    (0x000C)  /* LED2_DUR - [3:2] */
#define WM8325_LED2_DUR_SHIFT                   (     2)  /* LED2_DUR - [3:2] */
#define WM8325_LED2_DUR_WIDTH                   (     2)  /* LED2_DUR - [3:2] */
#define WM8325_LED2_DUTY_CYC_MASK               (0x0003)  /* LED2_DUTY_CYC - [1:0] */
#define WM8325_LED2_DUTY_CYC_SHIFT              (     0)  /* LED2_DUTY_CYC - [1:0] */
#define WM8325_LED2_DUTY_CYC_WIDTH              (     2)  /* LED2_DUTY_CYC - [1:0] */
                                            
/*                                                                                                   
 * R16464 (0x4050) - DCDC Enable                                                   
 */                                          
#define WM8325_EPE2_ENA                         (0x0080)  /* EPE2_ENA */
#define WM8325_EPE2_ENA_MASK                    (0x0080)  /* EPE2_ENA */
#define WM8325_EPE2_ENA_SHIFT                   (     7)  /* EPE2_ENA */
#define WM8325_EPE2_ENA_WIDTH                   (     1)  /* EPE2_ENA */
#define WM8325_EPE1_ENA                         (0x0040)  /* EPE1_ENA */
#define WM8325_EPE1_ENA_MASK                    (0x0040)  /* EPE1_ENA */
#define WM8325_EPE1_ENA_SHIFT                   (     6)  /* EPE1_ENA */
#define WM8325_EPE1_ENA_WIDTH                   (     1)  /* EPE1_ENA */
#define WM8325_DC4_ENA                          (0x0008)  /* DC4_ENA */
#define WM8325_DC4_ENA_MASK                     (0x0008)  /* DC4_ENA */
#define WM8325_DC4_ENA_SHIFT                    (     3)  /* DC4_ENA */
#define WM8325_DC4_ENA_WIDTH                    (     1)  /* DC4_ENA */
#define WM8325_DC3_ENA                          (0x0004)  /* DC3_ENA */
#define WM8325_DC3_ENA_MASK                     (0x0004)  /* DC3_ENA */
#define WM8325_DC3_ENA_SHIFT                    (     2)  /* DC3_ENA */
#define WM8325_DC3_ENA_WIDTH                    (     1)  /* DC3_ENA */
#define WM8325_DC2_ENA                          (0x0002)  /* DC2_ENA */
#define WM8325_DC2_ENA_MASK                     (0x0002)  /* DC2_ENA */
#define WM8325_DC2_ENA_SHIFT                    (     1)  /* DC2_ENA */
#define WM8325_DC2_ENA_WIDTH                    (     1)  /* DC2_ENA */
#define WM8325_DC1_ENA                          (0x0001)  /* DC1_ENA */
#define WM8325_DC1_ENA_MASK                     (0x0001)  /* DC1_ENA */
#define WM8325_DC1_ENA_SHIFT                    (     0)  /* DC1_ENA */
#define WM8325_DC1_ENA_WIDTH                    (     1)  /* DC1_ENA */
                                              
/*                                                                                                        
 * R16465 (0x4051) - LDO Enable                                                          
 */                                            
#define WM8325_LDO11_ENA                        (0x0400)  /* LDO11_ENA */
#define WM8325_LDO11_ENA_MASK                   (0x0400)  /* LDO11_ENA */
#define WM8325_LDO11_ENA_SHIFT                  (    10)  /* LDO11_ENA */
#define WM8325_LDO11_ENA_WIDTH                  (     1)  /* LDO11_ENA */
#define WM8325_LDO10_ENA                        (0x0200)  /* LDO10_ENA */
#define WM8325_LDO10_ENA_MASK                   (0x0200)  /* LDO10_ENA */
#define WM8325_LDO10_ENA_SHIFT                  (     9)  /* LDO10_ENA */
#define WM8325_LDO10_ENA_WIDTH                  (     1)  /* LDO10_ENA */
#define WM8325_LDO9_ENA                         (0x0100)  /* LDO9_ENA */
#define WM8325_LDO9_ENA_MASK                    (0x0100)  /* LDO9_ENA */
#define WM8325_LDO9_ENA_SHIFT                   (     8)  /* LDO9_ENA */
#define WM8325_LDO9_ENA_WIDTH                   (     1)  /* LDO9_ENA */
#define WM8325_LDO8_ENA                         (0x0080)  /* LDO8_ENA */
#define WM8325_LDO8_ENA_MASK                    (0x0080)  /* LDO8_ENA */
#define WM8325_LDO8_ENA_SHIFT                   (     7)  /* LDO8_ENA */
#define WM8325_LDO8_ENA_WIDTH                   (     1)  /* LDO8_ENA */
#define WM8325_LDO7_ENA                         (0x0040)  /* LDO7_ENA */
#define WM8325_LDO7_ENA_MASK                    (0x0040)  /* LDO7_ENA */
#define WM8325_LDO7_ENA_SHIFT                   (     6)  /* LDO7_ENA */
#define WM8325_LDO7_ENA_WIDTH                   (     1)  /* LDO7_ENA */
#define WM8325_LDO6_ENA                         (0x0020)  /* LDO6_ENA */
#define WM8325_LDO6_ENA_MASK                    (0x0020)  /* LDO6_ENA */
#define WM8325_LDO6_ENA_SHIFT                   (     5)  /* LDO6_ENA */
#define WM8325_LDO6_ENA_WIDTH                   (     1)  /* LDO6_ENA */
#define WM8325_LDO5_ENA                         (0x0010)  /* LDO5_ENA */
#define WM8325_LDO5_ENA_MASK                    (0x0010)  /* LDO5_ENA */
#define WM8325_LDO5_ENA_SHIFT                   (     4)  /* LDO5_ENA */
#define WM8325_LDO5_ENA_WIDTH                   (     1)  /* LDO5_ENA */
#define WM8325_LDO4_ENA                         (0x0008)  /* LDO4_ENA */
#define WM8325_LDO4_ENA_MASK                    (0x0008)  /* LDO4_ENA */
#define WM8325_LDO4_ENA_SHIFT                   (     3)  /* LDO4_ENA */
#define WM8325_LDO4_ENA_WIDTH                   (     1)  /* LDO4_ENA */
#define WM8325_LDO3_ENA                         (0x0004)  /* LDO3_ENA */
#define WM8325_LDO3_ENA_MASK                    (0x0004)  /* LDO3_ENA */
#define WM8325_LDO3_ENA_SHIFT                   (     2)  /* LDO3_ENA */
#define WM8325_LDO3_ENA_WIDTH                   (     1)  /* LDO3_ENA */
#define WM8325_LDO2_ENA                         (0x0002)  /* LDO2_ENA */
#define WM8325_LDO2_ENA_MASK                    (0x0002)  /* LDO2_ENA */
#define WM8325_LDO2_ENA_SHIFT                   (     1)  /* LDO2_ENA */
#define WM8325_LDO2_ENA_WIDTH                   (     1)  /* LDO2_ENA */
#define WM8325_LDO1_ENA                         (0x0001)  /* LDO1_ENA */
#define WM8325_LDO1_ENA_MASK                    (0x0001)  /* LDO1_ENA */
#define WM8325_LDO1_ENA_SHIFT                   (     0)  /* LDO1_ENA */
#define WM8325_LDO1_ENA_WIDTH                   (     1)  /* LDO1_ENA */
                                             
/*                                                                                                      
 * R16466 (0x4052) - DCDC Status                                                     
 */                                           
#define WM8325_EPE2_STS                         (0x0080)  /* EPE2_STS */
#define WM8325_EPE2_STS_MASK                    (0x0080)  /* EPE2_STS */
#define WM8325_EPE2_STS_SHIFT                   (     7)  /* EPE2_STS */
#define WM8325_EPE2_STS_WIDTH                   (     1)  /* EPE2_STS */
#define WM8325_EPE1_STS                         (0x0040)  /* EPE1_STS */
#define WM8325_EPE1_STS_MASK                    (0x0040)  /* EPE1_STS */
#define WM8325_EPE1_STS_SHIFT                   (     6)  /* EPE1_STS */
#define WM8325_EPE1_STS_WIDTH                   (     1)  /* EPE1_STS */
#define WM8325_DC4_STS                          (0x0008)  /* DC4_STS */
#define WM8325_DC4_STS_MASK                     (0x0008)  /* DC4_STS */
#define WM8325_DC4_STS_SHIFT                    (     3)  /* DC4_STS */
#define WM8325_DC4_STS_WIDTH                    (     1)  /* DC4_STS */
#define WM8325_DC3_STS                          (0x0004)  /* DC3_STS */
#define WM8325_DC3_STS_MASK                     (0x0004)  /* DC3_STS */
#define WM8325_DC3_STS_SHIFT                    (     2)  /* DC3_STS */
#define WM8325_DC3_STS_WIDTH                    (     1)  /* DC3_STS */
#define WM8325_DC2_STS                          (0x0002)  /* DC2_STS */
#define WM8325_DC2_STS_MASK                     (0x0002)  /* DC2_STS */
#define WM8325_DC2_STS_SHIFT                    (     1)  /* DC2_STS */
#define WM8325_DC2_STS_WIDTH                    (     1)  /* DC2_STS */
#define WM8325_DC1_STS                          (0x0001)  /* DC1_STS */
#define WM8325_DC1_STS_MASK                     (0x0001)  /* DC1_STS */
#define WM8325_DC1_STS_SHIFT                    (     0)  /* DC1_STS */
#define WM8325_DC1_STS_WIDTH                    (     1)  /* DC1_STS */
                                            
/*                                                                                                   
 * R16467 (0x4053) - LDO Status                                                     
 */                                          
#define WM8325_LDO11_STS                        (0x0400)  /* LDO11_STS */
#define WM8325_LDO11_STS_MASK                   (0x0400)  /* LDO11_STS */
#define WM8325_LDO11_STS_SHIFT                  (    10)  /* LDO11_STS */
#define WM8325_LDO11_STS_WIDTH                  (     1)  /* LDO11_STS */
#define WM8325_LDO10_STS                        (0x0200)  /* LDO10_STS */
#define WM8325_LDO10_STS_MASK                   (0x0200)  /* LDO10_STS */
#define WM8325_LDO10_STS_SHIFT                  (     9)  /* LDO10_STS */
#define WM8325_LDO10_STS_WIDTH                  (     1)  /* LDO10_STS */
#define WM8325_LDO9_STS                         (0x0100)  /* LDO9_STS */
#define WM8325_LDO9_STS_MASK                    (0x0100)  /* LDO9_STS */
#define WM8325_LDO9_STS_SHIFT                   (     8)  /* LDO9_STS */
#define WM8325_LDO9_STS_WIDTH                   (     1)  /* LDO9_STS */
#define WM8325_LDO8_STS                         (0x0080)  /* LDO8_STS */
#define WM8325_LDO8_STS_MASK                    (0x0080)  /* LDO8_STS */
#define WM8325_LDO8_STS_SHIFT                   (     7)  /* LDO8_STS */
#define WM8325_LDO8_STS_WIDTH                   (     1)  /* LDO8_STS */
#define WM8325_LDO7_STS                         (0x0040)  /* LDO7_STS */
#define WM8325_LDO7_STS_MASK                    (0x0040)  /* LDO7_STS */
#define WM8325_LDO7_STS_SHIFT                   (     6)  /* LDO7_STS */
#define WM8325_LDO7_STS_WIDTH                   (     1)  /* LDO7_STS */
#define WM8325_LDO6_STS                         (0x0020)  /* LDO6_STS */
#define WM8325_LDO6_STS_MASK                    (0x0020)  /* LDO6_STS */
#define WM8325_LDO6_STS_SHIFT                   (     5)  /* LDO6_STS */
#define WM8325_LDO6_STS_WIDTH                   (     1)  /* LDO6_STS */
#define WM8325_LDO5_STS                         (0x0010)  /* LDO5_STS */
#define WM8325_LDO5_STS_MASK                    (0x0010)  /* LDO5_STS */
#define WM8325_LDO5_STS_SHIFT                   (     4)  /* LDO5_STS */
#define WM8325_LDO5_STS_WIDTH                   (     1)  /* LDO5_STS */
#define WM8325_LDO4_STS                         (0x0008)  /* LDO4_STS */
#define WM8325_LDO4_STS_MASK                    (0x0008)  /* LDO4_STS */
#define WM8325_LDO4_STS_SHIFT                   (     3)  /* LDO4_STS */
#define WM8325_LDO4_STS_WIDTH                   (     1)  /* LDO4_STS */
#define WM8325_LDO3_STS                         (0x0004)  /* LDO3_STS */
#define WM8325_LDO3_STS_MASK                    (0x0004)  /* LDO3_STS */
#define WM8325_LDO3_STS_SHIFT                   (     2)  /* LDO3_STS */
#define WM8325_LDO3_STS_WIDTH                   (     1)  /* LDO3_STS */
#define WM8325_LDO2_STS                         (0x0002)  /* LDO2_STS */
#define WM8325_LDO2_STS_MASK                    (0x0002)  /* LDO2_STS */
#define WM8325_LDO2_STS_SHIFT                   (     1)  /* LDO2_STS */
#define WM8325_LDO2_STS_WIDTH                   (     1)  /* LDO2_STS */
#define WM8325_LDO1_STS                         (0x0001)  /* LDO1_STS */
#define WM8325_LDO1_STS_MASK                    (0x0001)  /* LDO1_STS */
#define WM8325_LDO1_STS_SHIFT                   (     0)  /* LDO1_STS */
#define WM8325_LDO1_STS_WIDTH                   (     1)  /* LDO1_STS */
                                              
/*                                                                                                        
 * R16468 (0x4054) - DCDC UV Status                                                  
 */                                            
#define WM8325_DC2_OV_STS                       (0x2000)  /* DC2_OV_STS */
#define WM8325_DC2_OV_STS_MASK                  (0x2000)  /* DC2_OV_STS */
#define WM8325_DC2_OV_STS_SHIFT                 (    13)  /* DC2_OV_STS */
#define WM8325_DC2_OV_STS_WIDTH                 (     1)  /* DC2_OV_STS */
#define WM8325_DC1_OV_STS                       (0x1000)  /* DC1_OV_STS */
#define WM8325_DC1_OV_STS_MASK                  (0x1000)  /* DC1_OV_STS */
#define WM8325_DC1_OV_STS_SHIFT                 (    12)  /* DC1_OV_STS */
#define WM8325_DC1_OV_STS_WIDTH                 (     1)  /* DC1_OV_STS */
#define WM8325_DC2_HC_STS                       (0x0200)  /* DC2_HC_STS */
#define WM8325_DC2_HC_STS_MASK                  (0x0200)  /* DC2_HC_STS */
#define WM8325_DC2_HC_STS_SHIFT                 (     9)  /* DC2_HC_STS */
#define WM8325_DC2_HC_STS_WIDTH                 (     1)  /* DC2_HC_STS */
#define WM8325_DC1_HC_STS                       (0x0100)  /* DC1_HC_STS */
#define WM8325_DC1_HC_STS_MASK                  (0x0100)  /* DC1_HC_STS */
#define WM8325_DC1_HC_STS_SHIFT                 (     8)  /* DC1_HC_STS */
#define WM8325_DC1_HC_STS_WIDTH                 (     1)  /* DC1_HC_STS */
#define WM8325_DC4_UV_STS                       (0x0008)  /* DC4_UV_STS */
#define WM8325_DC4_UV_STS_MASK                  (0x0008)  /* DC4_UV_STS */
#define WM8325_DC4_UV_STS_SHIFT                 (     3)  /* DC4_UV_STS */
#define WM8325_DC4_UV_STS_WIDTH                 (     1)  /* DC4_UV_STS */
#define WM8325_DC3_UV_STS                       (0x0004)  /* DC3_UV_STS */
#define WM8325_DC3_UV_STS_MASK                  (0x0004)  /* DC3_UV_STS */
#define WM8325_DC3_UV_STS_SHIFT                 (     2)  /* DC3_UV_STS */
#define WM8325_DC3_UV_STS_WIDTH                 (     1)  /* DC3_UV_STS */
#define WM8325_DC2_UV_STS                       (0x0002)  /* DC2_UV_STS */
#define WM8325_DC2_UV_STS_MASK                  (0x0002)  /* DC2_UV_STS */
#define WM8325_DC2_UV_STS_SHIFT                 (     1)  /* DC2_UV_STS */
#define WM8325_DC2_UV_STS_WIDTH                 (     1)  /* DC2_UV_STS */
#define WM8325_DC1_UV_STS                       (0x0001)  /* DC1_UV_STS */
#define WM8325_DC1_UV_STS_MASK                  (0x0001)  /* DC1_UV_STS */
#define WM8325_DC1_UV_STS_SHIFT                 (     0)  /* DC1_UV_STS */
#define WM8325_DC1_UV_STS_WIDTH                 (     1)  /* DC1_UV_STS */
                                          
/*                                                                                               
 * R16469 (0x4055) - LDO UV Status                                           
 */                                        
#define WM8325_INTLDO_UV_STS                    (0x8000)  /* INTLDO_UV_STS */
#define WM8325_INTLDO_UV_STS_MASK               (0x8000)  /* INTLDO_UV_STS */
#define WM8325_INTLDO_UV_STS_SHIFT              (    15)  /* INTLDO_UV_STS */
#define WM8325_INTLDO_UV_STS_WIDTH              (     1)  /* INTLDO_UV_STS */
#define WM8325_LDO10_UV_STS                     (0x0200)  /* LDO10_UV_STS */
#define WM8325_LDO10_UV_STS_MASK                (0x0200)  /* LDO10_UV_STS */
#define WM8325_LDO10_UV_STS_SHIFT               (     9)  /* LDO10_UV_STS */
#define WM8325_LDO10_UV_STS_WIDTH               (     1)  /* LDO10_UV_STS */
#define WM8325_LDO9_UV_STS                      (0x0100)  /* LDO9_UV_STS */
#define WM8325_LDO9_UV_STS_MASK                 (0x0100)  /* LDO9_UV_STS */
#define WM8325_LDO9_UV_STS_SHIFT                (     8)  /* LDO9_UV_STS */
#define WM8325_LDO9_UV_STS_WIDTH                (     1)  /* LDO9_UV_STS */
#define WM8325_LDO8_UV_STS                      (0x0080)  /* LDO8_UV_STS */
#define WM8325_LDO8_UV_STS_MASK                 (0x0080)  /* LDO8_UV_STS */
#define WM8325_LDO8_UV_STS_SHIFT                (     7)  /* LDO8_UV_STS */
#define WM8325_LDO8_UV_STS_WIDTH                (     1)  /* LDO8_UV_STS */
#define WM8325_LDO7_UV_STS                      (0x0040)  /* LDO7_UV_STS */
#define WM8325_LDO7_UV_STS_MASK                 (0x0040)  /* LDO7_UV_STS */
#define WM8325_LDO7_UV_STS_SHIFT                (     6)  /* LDO7_UV_STS */
#define WM8325_LDO7_UV_STS_WIDTH                (     1)  /* LDO7_UV_STS */
#define WM8325_LDO6_UV_STS                      (0x0020)  /* LDO6_UV_STS */
#define WM8325_LDO6_UV_STS_MASK                 (0x0020)  /* LDO6_UV_STS */
#define WM8325_LDO6_UV_STS_SHIFT                (     5)  /* LDO6_UV_STS */
#define WM8325_LDO6_UV_STS_WIDTH                (     1)  /* LDO6_UV_STS */
#define WM8325_LDO5_UV_STS                      (0x0010)  /* LDO5_UV_STS */
#define WM8325_LDO5_UV_STS_MASK                 (0x0010)  /* LDO5_UV_STS */
#define WM8325_LDO5_UV_STS_SHIFT                (     4)  /* LDO5_UV_STS */
#define WM8325_LDO5_UV_STS_WIDTH                (     1)  /* LDO5_UV_STS */
#define WM8325_LDO4_UV_STS                      (0x0008)  /* LDO4_UV_STS */
#define WM8325_LDO4_UV_STS_MASK                 (0x0008)  /* LDO4_UV_STS */
#define WM8325_LDO4_UV_STS_SHIFT                (     3)  /* LDO4_UV_STS */
#define WM8325_LDO4_UV_STS_WIDTH                (     1)  /* LDO4_UV_STS */
#define WM8325_LDO3_UV_STS                      (0x0004)  /* LDO3_UV_STS */
#define WM8325_LDO3_UV_STS_MASK                 (0x0004)  /* LDO3_UV_STS */
#define WM8325_LDO3_UV_STS_SHIFT                (     2)  /* LDO3_UV_STS */
#define WM8325_LDO3_UV_STS_WIDTH                (     1)  /* LDO3_UV_STS */
#define WM8325_LDO2_UV_STS                      (0x0002)  /* LDO2_UV_STS */
#define WM8325_LDO2_UV_STS_MASK                 (0x0002)  /* LDO2_UV_STS */
#define WM8325_LDO2_UV_STS_SHIFT                (     1)  /* LDO2_UV_STS */
#define WM8325_LDO2_UV_STS_WIDTH                (     1)  /* LDO2_UV_STS */
#define WM8325_LDO1_UV_STS                      (0x0001)  /* LDO1_UV_STS */
#define WM8325_LDO1_UV_STS_MASK                 (0x0001)  /* LDO1_UV_STS */
#define WM8325_LDO1_UV_STS_SHIFT                (     0)  /* LDO1_UV_STS */
#define WM8325_LDO1_UV_STS_WIDTH                (     1)  /* LDO1_UV_STS */
                                            
/*                                                                                                   
 * R16470 (0x4056) - DC1 Control 1                                                  
 */                                          
#define WM8325_DC1_RATE_MASK                    (0xC000)  /* DC1_RATE - [15:14] */
#define WM8325_DC1_RATE_SHIFT                   (    14)  /* DC1_RATE - [15:14] */
#define WM8325_DC1_RATE_WIDTH                   (     2)  /* DC1_RATE - [15:14] */
#define WM8325_DC1_PHASE                        (0x1000)  /* DC1_PHASE */
#define WM8325_DC1_PHASE_MASK                   (0x1000)  /* DC1_PHASE */
#define WM8325_DC1_PHASE_SHIFT                  (    12)  /* DC1_PHASE */
#define WM8325_DC1_PHASE_WIDTH                  (     1)  /* DC1_PHASE */
#define WM8325_DC1_FREQ_MASK                    (0x0300)  /* DC1_FREQ - [9:8] */
#define WM8325_DC1_FREQ_SHIFT                   (     8)  /* DC1_FREQ - [9:8] */
#define WM8325_DC1_FREQ_WIDTH                   (     2)  /* DC1_FREQ - [9:8] */
#define WM8325_DC1_FLT                          (0x0080)  /* DC1_FLT */
#define WM8325_DC1_FLT_MASK                     (0x0080)  /* DC1_FLT */
#define WM8325_DC1_FLT_SHIFT                    (     7)  /* DC1_FLT */
#define WM8325_DC1_FLT_WIDTH                    (     1)  /* DC1_FLT */
#define WM8325_DC1_SOFT_START_MASK              (0x0030)  /* DC1_SOFT_START - [5:4] */
#define WM8325_DC1_SOFT_START_SHIFT             (     4)  /* DC1_SOFT_START - [5:4] */
#define WM8325_DC1_SOFT_START_WIDTH             (     2)  /* DC1_SOFT_START - [5:4] */
#define WM8325_DC1_CAP_MASK                     (0x0003)  /* DC1_CAP - [1:0] */
#define WM8325_DC1_CAP_SHIFT                    (     0)  /* DC1_CAP - [1:0] */
#define WM8325_DC1_CAP_WIDTH                    (     2)  /* DC1_CAP - [1:0] */
                                           
/*                                                                                                 
 * R16471 (0x4057) - DC1 Control 2                                                
 */                                         
#define WM8325_DC1_ERR_ACT_MASK                 (0xC000)  /* DC1_ERR_ACT - [15:14] */
#define WM8325_DC1_ERR_ACT_SHIFT                (    14)  /* DC1_ERR_ACT - [15:14] */
#define WM8325_DC1_ERR_ACT_WIDTH                (     2)  /* DC1_ERR_ACT - [15:14] */
#define WM8325_DC1_HWC_SRC_MASK                 (0x1800)  /* DC1_HWC_SRC - [12:11] */
#define WM8325_DC1_HWC_SRC_SHIFT                (    11)  /* DC1_HWC_SRC - [12:11] */
#define WM8325_DC1_HWC_SRC_WIDTH                (     2)  /* DC1_HWC_SRC - [12:11] */
#define WM8325_DC1_HWC_VSEL                     (0x0400)  /* DC1_HWC_VSEL */
#define WM8325_DC1_HWC_VSEL_MASK                (0x0400)  /* DC1_HWC_VSEL */
#define WM8325_DC1_HWC_VSEL_SHIFT               (    10)  /* DC1_HWC_VSEL */
#define WM8325_DC1_HWC_VSEL_WIDTH               (     1)  /* DC1_HWC_VSEL */
#define WM8325_DC1_HWC_MODE_MASK                (0x0300)  /* DC1_HWC_MODE - [9:8] */
#define WM8325_DC1_HWC_MODE_SHIFT               (     8)  /* DC1_HWC_MODE - [9:8] */
#define WM8325_DC1_HWC_MODE_WIDTH               (     2)  /* DC1_HWC_MODE - [9:8] */
#define WM8325_DC1_HC_THR_MASK                  (0x0070)  /* DC1_HC_THR - [6:4] */
#define WM8325_DC1_HC_THR_SHIFT                 (     4)  /* DC1_HC_THR - [6:4] */
#define WM8325_DC1_HC_THR_WIDTH                 (     3)  /* DC1_HC_THR - [6:4] */
#define WM8325_DC1_HC_IND_ENA                   (0x0001)  /* DC1_HC_IND_ENA */
#define WM8325_DC1_HC_IND_ENA_MASK              (0x0001)  /* DC1_HC_IND_ENA */
#define WM8325_DC1_HC_IND_ENA_SHIFT             (     0)  /* DC1_HC_IND_ENA */
#define WM8325_DC1_HC_IND_ENA_WIDTH             (     1)  /* DC1_HC_IND_ENA */
                                             
/*                                                                                                      
 * R16472 (0x4058) - DC1 ON Config                                                  
 */                                           
#define WM8325_DC1_ON_SLOT_MASK                 (0xE000)  /* DC1_ON_SLOT - [15:13] */
#define WM8325_DC1_ON_SLOT_SHIFT                (    13)  /* DC1_ON_SLOT - [15:13] */
#define WM8325_DC1_ON_SLOT_WIDTH                (     3)  /* DC1_ON_SLOT - [15:13] */
#define WM8325_DC1_ON_MODE_MASK                 (0x0300)  /* DC1_ON_MODE - [9:8] */
#define WM8325_DC1_ON_MODE_SHIFT                (     8)  /* DC1_ON_MODE - [9:8] */
#define WM8325_DC1_ON_MODE_WIDTH                (     2)  /* DC1_ON_MODE - [9:8] */
#define WM8325_DC1_ON_VSEL_0_MASK               (0x007C)  /* DC1_ON_VSEL - [6:2] */
#define WM8325_DC1_ON_VSEL_0_SHIFT              (     2)  /* DC1_ON_VSEL - [6:2] */
#define WM8325_DC1_ON_VSEL_0_WIDTH              (     5)  /* DC1_ON_VSEL - [6:2] */
#define WM8325_DC1_ON_VSEL_MASK                 (0x0003)  /* DC1_ON_VSEL - [1:0] */
#define WM8325_DC1_ON_VSEL_SHIFT                (     0)  /* DC1_ON_VSEL - [1:0] */
#define WM8325_DC1_ON_VSEL_WIDTH                (     2)  /* DC1_ON_VSEL - [1:0] */
                                             
/*                                                                                                      
 * R16473 (0x4059) - DC1 SLEEP Control                                             
 */                                           
#define WM8325_DC1_SLP_SLOT_MASK                (0xE000)  /* DC1_SLP_SLOT - [15:13] */
#define WM8325_DC1_SLP_SLOT_SHIFT               (    13)  /* DC1_SLP_SLOT - [15:13] */
#define WM8325_DC1_SLP_SLOT_WIDTH               (     3)  /* DC1_SLP_SLOT - [15:13] */
#define WM8325_DC1_SLP_MODE_MASK                (0x0300)  /* DC1_SLP_MODE - [9:8] */
#define WM8325_DC1_SLP_MODE_SHIFT               (     8)  /* DC1_SLP_MODE - [9:8] */
#define WM8325_DC1_SLP_MODE_WIDTH               (     2)  /* DC1_SLP_MODE - [9:8] */
#define WM8325_DC1_SLP_VSEL_MASK                (0x007F)  /* DC1_SLP_VSEL - [6:0] */
#define WM8325_DC1_SLP_VSEL_SHIFT               (     0)  /* DC1_SLP_VSEL - [6:0] */
#define WM8325_DC1_SLP_VSEL_WIDTH               (     7)  /* DC1_SLP_VSEL - [6:0] */
                                             
/*                                                                                                      
 * R16474 (0x405A) - DC1 DVS Control                                                
 */                                           
#define WM8325_DC1_DVS_SRC_MASK                 (0x1800)  /* DC1_DVS_SRC - [12:11] */
#define WM8325_DC1_DVS_SRC_SHIFT                (    11)  /* DC1_DVS_SRC - [12:11] */
#define WM8325_DC1_DVS_SRC_WIDTH                (     2)  /* DC1_DVS_SRC - [12:11] */
#define WM8325_DC1_DVS_VSEL_MASK                (0x007F)  /* DC1_DVS_VSEL - [6:0] */
#define WM8325_DC1_DVS_VSEL_SHIFT               (     0)  /* DC1_DVS_VSEL - [6:0] */
#define WM8325_DC1_DVS_VSEL_WIDTH               (     7)  /* DC1_DVS_VSEL - [6:0] */
                                               
/*                                                                                                          
 * R16475 (0x405B) - DC2 Control 1                                                         
 */                                             
#define WM8325_DC2_RATE_MASK                    (0xC000)  /* DC2_RATE - [15:14] */
#define WM8325_DC2_RATE_SHIFT                   (    14)  /* DC2_RATE - [15:14] */
#define WM8325_DC2_RATE_WIDTH                   (     2)  /* DC2_RATE - [15:14] */
#define WM8325_DC2_PHASE                        (0x1000)  /* DC2_PHASE */
#define WM8325_DC2_PHASE_MASK                   (0x1000)  /* DC2_PHASE */
#define WM8325_DC2_PHASE_SHIFT                  (    12)  /* DC2_PHASE */
#define WM8325_DC2_PHASE_WIDTH                  (     1)  /* DC2_PHASE */
#define WM8325_DC2_FREQ_MASK                    (0x0300)  /* DC2_FREQ - [9:8] */
#define WM8325_DC2_FREQ_SHIFT                   (     8)  /* DC2_FREQ - [9:8] */
#define WM8325_DC2_FREQ_WIDTH                   (     2)  /* DC2_FREQ - [9:8] */
#define WM8325_DC2_FLT                          (0x0080)  /* DC2_FLT */
#define WM8325_DC2_FLT_MASK                     (0x0080)  /* DC2_FLT */
#define WM8325_DC2_FLT_SHIFT                    (     7)  /* DC2_FLT */
#define WM8325_DC2_FLT_WIDTH                    (     1)  /* DC2_FLT */
#define WM8325_DC2_SOFT_START_MASK              (0x0030)  /* DC2_SOFT_START - [5:4] */
#define WM8325_DC2_SOFT_START_SHIFT             (     4)  /* DC2_SOFT_START - [5:4] */
#define WM8325_DC2_SOFT_START_WIDTH             (     2)  /* DC2_SOFT_START - [5:4] */
#define WM8325_DC2_CAP_MASK                     (0x0003)  /* DC2_CAP - [1:0] */
#define WM8325_DC2_CAP_SHIFT                    (     0)  /* DC2_CAP - [1:0] */
#define WM8325_DC2_CAP_WIDTH                    (     2)  /* DC2_CAP - [1:0] */
                                             
/*                                                                                                      
 * R16476 (0x405C) - DC2 Control 2                                                    
 */                                           
#define WM8325_DC2_ERR_ACT_MASK                 (0xC000)  /* DC2_ERR_ACT - [15:14] */
#define WM8325_DC2_ERR_ACT_SHIFT                (    14)  /* DC2_ERR_ACT - [15:14] */
#define WM8325_DC2_ERR_ACT_WIDTH                (     2)  /* DC2_ERR_ACT - [15:14] */
#define WM8325_DC2_HWC_SRC_MASK                 (0x1800)  /* DC2_HWC_SRC - [12:11] */
#define WM8325_DC2_HWC_SRC_SHIFT                (    11)  /* DC2_HWC_SRC - [12:11] */
#define WM8325_DC2_HWC_SRC_WIDTH                (     2)  /* DC2_HWC_SRC - [12:11] */
#define WM8325_DC2_HWC_VSEL                     (0x0400)  /* DC2_HWC_VSEL */
#define WM8325_DC2_HWC_VSEL_MASK                (0x0400)  /* DC2_HWC_VSEL */
#define WM8325_DC2_HWC_VSEL_SHIFT               (    10)  /* DC2_HWC_VSEL */
#define WM8325_DC2_HWC_VSEL_WIDTH               (     1)  /* DC2_HWC_VSEL */
#define WM8325_DC2_HWC_MODE_MASK                (0x0300)  /* DC2_HWC_MODE - [9:8] */
#define WM8325_DC2_HWC_MODE_SHIFT               (     8)  /* DC2_HWC_MODE - [9:8] */
#define WM8325_DC2_HWC_MODE_WIDTH               (     2)  /* DC2_HWC_MODE - [9:8] */
#define WM8325_DC2_HC_THR_MASK                  (0x0070)  /* DC2_HC_THR - [6:4] */
#define WM8325_DC2_HC_THR_SHIFT                 (     4)  /* DC2_HC_THR - [6:4] */
#define WM8325_DC2_HC_THR_WIDTH                 (     3)  /* DC2_HC_THR - [6:4] */
#define WM8325_DC2_HC_IND_ENA                   (0x0001)  /* DC2_HC_IND_ENA */
#define WM8325_DC2_HC_IND_ENA_MASK              (0x0001)  /* DC2_HC_IND_ENA */
#define WM8325_DC2_HC_IND_ENA_SHIFT             (     0)  /* DC2_HC_IND_ENA */
#define WM8325_DC2_HC_IND_ENA_WIDTH             (     1)  /* DC2_HC_IND_ENA */
                                             
/*                                                                                                      
 * R16477 (0x405D) - DC2 ON Config                                                  
 */                                           
#define WM8325_DC2_ON_SLOT_MASK                 (0xE000)  /* DC2_ON_SLOT - [15:13] */
#define WM8325_DC2_ON_SLOT_SHIFT                (    13)  /* DC2_ON_SLOT - [15:13] */
#define WM8325_DC2_ON_SLOT_WIDTH                (     3)  /* DC2_ON_SLOT - [15:13] */
#define WM8325_DC2_ON_MODE_MASK                 (0x0300)  /* DC2_ON_MODE - [9:8] */
#define WM8325_DC2_ON_MODE_SHIFT                (     8)  /* DC2_ON_MODE - [9:8] */
#define WM8325_DC2_ON_MODE_WIDTH                (     2)  /* DC2_ON_MODE - [9:8] */
#define WM8325_DC2_ON_VSEL_0_MASK               (0x007C)  /* DC2_ON_VSEL - [6:2] */
#define WM8325_DC2_ON_VSEL_0_SHIFT              (     2)  /* DC2_ON_VSEL - [6:2] */
#define WM8325_DC2_ON_VSEL_0_WIDTH              (     5)  /* DC2_ON_VSEL - [6:2] */
#define WM8325_DC2_ON_VSEL_MASK                 (0x0003)  /* DC2_ON_VSEL - [1:0] */
#define WM8325_DC2_ON_VSEL_SHIFT                (     0)  /* DC2_ON_VSEL - [1:0] */
#define WM8325_DC2_ON_VSEL_WIDTH                (     2)  /* DC2_ON_VSEL - [1:0] */
                                               
/*                                                                                                          
 * R16478 (0x405E) - DC2 SLEEP Control                                                 
 */                                             
#define WM8325_DC2_SLP_SLOT_MASK                (0xE000)  /* DC2_SLP_SLOT - [15:13] */
#define WM8325_DC2_SLP_SLOT_SHIFT               (    13)  /* DC2_SLP_SLOT - [15:13] */
#define WM8325_DC2_SLP_SLOT_WIDTH               (     3)  /* DC2_SLP_SLOT - [15:13] */
#define WM8325_DC2_SLP_MODE_MASK                (0x0300)  /* DC2_SLP_MODE - [9:8] */
#define WM8325_DC2_SLP_MODE_SHIFT               (     8)  /* DC2_SLP_MODE - [9:8] */
#define WM8325_DC2_SLP_MODE_WIDTH               (     2)  /* DC2_SLP_MODE - [9:8] */
#define WM8325_DC2_SLP_VSEL_MASK                (0x007F)  /* DC2_SLP_VSEL - [6:0] */
#define WM8325_DC2_SLP_VSEL_SHIFT               (     0)  /* DC2_SLP_VSEL - [6:0] */
#define WM8325_DC2_SLP_VSEL_WIDTH               (     7)  /* DC2_SLP_VSEL - [6:0] */
                                          
/*                                                                                               
 * R16479 (0x405F) - DC2 DVS Control                                         
 */                                        
#define WM8325_DC2_DVS_SRC_MASK                 (0x1800)  /* DC2_DVS_SRC - [12:11] */
#define WM8325_DC2_DVS_SRC_SHIFT                (    11)  /* DC2_DVS_SRC - [12:11] */
#define WM8325_DC2_DVS_SRC_WIDTH                (     2)  /* DC2_DVS_SRC - [12:11] */
#define WM8325_DC2_DVS_VSEL_MASK                (0x007F)  /* DC2_DVS_VSEL - [6:0] */
#define WM8325_DC2_DVS_VSEL_SHIFT               (     0)  /* DC2_DVS_VSEL - [6:0] */
#define WM8325_DC2_DVS_VSEL_WIDTH               (     7)  /* DC2_DVS_VSEL - [6:0] */
                                             
/*                                                                                                      
 * R16480 (0x4060) - DC3 Control 1                                                    
 */                                           
#define WM8325_DC3_PHASE                        (0x1000)  /* DC3_PHASE */
#define WM8325_DC3_PHASE_MASK                   (0x1000)  /* DC3_PHASE */
#define WM8325_DC3_PHASE_SHIFT                  (    12)  /* DC3_PHASE */
#define WM8325_DC3_PHASE_WIDTH                  (     1)  /* DC3_PHASE */
#define WM8325_DC3_FLT                          (0x0080)  /* DC3_FLT */
#define WM8325_DC3_FLT_MASK                     (0x0080)  /* DC3_FLT */
#define WM8325_DC3_FLT_SHIFT                    (     7)  /* DC3_FLT */
#define WM8325_DC3_FLT_WIDTH                    (     1)  /* DC3_FLT */
#define WM8325_DC3_SOFT_START_MASK              (0x0030)  /* DC3_SOFT_START - [5:4] */
#define WM8325_DC3_SOFT_START_SHIFT             (     4)  /* DC3_SOFT_START - [5:4] */
#define WM8325_DC3_SOFT_START_WIDTH             (     2)  /* DC3_SOFT_START - [5:4] */
#define WM8325_DC3_STNBY_LIM_MASK               (0x000C)  /* DC3_STNBY_LIM - [3:2] */
#define WM8325_DC3_STNBY_LIM_SHIFT              (     2)  /* DC3_STNBY_LIM - [3:2] */
#define WM8325_DC3_STNBY_LIM_WIDTH              (     2)  /* DC3_STNBY_LIM - [3:2] */
#define WM8325_DC3_CAP_MASK                     (0x0003)  /* DC3_CAP - [1:0] */
#define WM8325_DC3_CAP_SHIFT                    (     0)  /* DC3_CAP - [1:0] */
#define WM8325_DC3_CAP_WIDTH                    (     2)  /* DC3_CAP - [1:0] */
                                            
/*                                                                                                   
 * R16481 (0x4061) - DC3 Control 2                                                  
 */                                          
#define WM8325_DC3_ERR_ACT_MASK                 (0xC000)  /* DC3_ERR_ACT - [15:14] */
#define WM8325_DC3_ERR_ACT_SHIFT                (    14)  /* DC3_ERR_ACT - [15:14] */
#define WM8325_DC3_ERR_ACT_WIDTH                (     2)  /* DC3_ERR_ACT - [15:14] */
#define WM8325_DC3_HWC_SRC_MASK                 (0x1800)  /* DC3_HWC_SRC - [12:11] */
#define WM8325_DC3_HWC_SRC_SHIFT                (    11)  /* DC3_HWC_SRC - [12:11] */
#define WM8325_DC3_HWC_SRC_WIDTH                (     2)  /* DC3_HWC_SRC - [12:11] */
#define WM8325_DC3_HWC_VSEL                     (0x0400)  /* DC3_HWC_VSEL */
#define WM8325_DC3_HWC_VSEL_MASK                (0x0400)  /* DC3_HWC_VSEL */
#define WM8325_DC3_HWC_VSEL_SHIFT               (    10)  /* DC3_HWC_VSEL */
#define WM8325_DC3_HWC_VSEL_WIDTH               (     1)  /* DC3_HWC_VSEL */
#define WM8325_DC3_HWC_MODE_MASK                (0x0300)  /* DC3_HWC_MODE - [9:8] */
#define WM8325_DC3_HWC_MODE_SHIFT               (     8)  /* DC3_HWC_MODE - [9:8] */
#define WM8325_DC3_HWC_MODE_WIDTH               (     2)  /* DC3_HWC_MODE - [9:8] */
#define WM8325_DC3_OVP                          (0x0080)  /* DC3_OVP */
#define WM8325_DC3_OVP_MASK                     (0x0080)  /* DC3_OVP */
#define WM8325_DC3_OVP_SHIFT                    (     7)  /* DC3_OVP */
#define WM8325_DC3_OVP_WIDTH                    (     1)  /* DC3_OVP */
                                              
/*                                                                                                        
 * R16482 (0x4062) - DC3 ON Config                                                    
 */                                            
#define WM8325_DC3_ON_SLOT_MASK                 (0xE000)  /* DC3_ON_SLOT - [15:13] */
#define WM8325_DC3_ON_SLOT_SHIFT                (    13)  /* DC3_ON_SLOT - [15:13] */
#define WM8325_DC3_ON_SLOT_WIDTH                (     3)  /* DC3_ON_SLOT - [15:13] */
#define WM8325_DC3_ON_MODE_MASK                 (0x0300)  /* DC3_ON_MODE - [9:8] */
#define WM8325_DC3_ON_MODE_SHIFT                (     8)  /* DC3_ON_MODE - [9:8] */
#define WM8325_DC3_ON_MODE_WIDTH                (     2)  /* DC3_ON_MODE - [9:8] */
#define WM8325_DC3_ON_VSEL_0_MASK               (0x007C)  /* DC3_ON_VSEL - [6:2] */
#define WM8325_DC3_ON_VSEL_0_SHIFT              (     2)  /* DC3_ON_VSEL - [6:2] */
#define WM8325_DC3_ON_VSEL_0_WIDTH              (     5)  /* DC3_ON_VSEL - [6:2] */
#define WM8325_DC3_ON_VSEL_MASK                 (0x0003)  /* DC3_ON_VSEL - [1:0] */
#define WM8325_DC3_ON_VSEL_SHIFT                (     0)  /* DC3_ON_VSEL - [1:0] */
#define WM8325_DC3_ON_VSEL_WIDTH                (     2)  /* DC3_ON_VSEL - [1:0] */
                                             
/*                                                                                                      
 * R16483 (0x4063) - DC3 SLEEP Control                                            
 */                                           
#define WM8325_DC3_SLP_SLOT_MASK                (0xE000)  /* DC3_SLP_SLOT - [15:13] */
#define WM8325_DC3_SLP_SLOT_SHIFT               (    13)  /* DC3_SLP_SLOT - [15:13] */
#define WM8325_DC3_SLP_SLOT_WIDTH               (     3)  /* DC3_SLP_SLOT - [15:13] */
#define WM8325_DC3_SLP_MODE_MASK                (0x0300)  /* DC3_SLP_MODE - [9:8] */
#define WM8325_DC3_SLP_MODE_SHIFT               (     8)  /* DC3_SLP_MODE - [9:8] */
#define WM8325_DC3_SLP_MODE_WIDTH               (     2)  /* DC3_SLP_MODE - [9:8] */
#define WM8325_DC3_SLP_VSEL_MASK                (0x007F)  /* DC3_SLP_VSEL - [6:0] */
#define WM8325_DC3_SLP_VSEL_SHIFT               (     0)  /* DC3_SLP_VSEL - [6:0] */
#define WM8325_DC3_SLP_VSEL_WIDTH               (     7)  /* DC3_SLP_VSEL - [6:0] */
                                             
/*                                                                                                      
 * R16484 (0x4064) - DC4 Control 1                                                    
 */                                           
#define WM8325_DC4_SLV                          (0x2000)  /* DC4_SLV */
#define WM8325_DC4_SLV_MASK                     (0x2000)  /* DC4_SLV */
#define WM8325_DC4_SLV_SHIFT                    (    13)  /* DC4_SLV */
#define WM8325_DC4_SLV_WIDTH                    (     1)  /* DC4_SLV */
#define WM8325_DC4_PHASE                        (0x1000)  /* DC4_PHASE */
#define WM8325_DC4_PHASE_MASK                   (0x1000)  /* DC4_PHASE */
#define WM8325_DC4_PHASE_SHIFT                  (    12)  /* DC4_PHASE */
#define WM8325_DC4_PHASE_WIDTH                  (     1)  /* DC4_PHASE */
#define WM8325_DC4_FLT                          (0x0080)  /* DC4_FLT */
#define WM8325_DC4_FLT_MASK                     (0x0080)  /* DC4_FLT */
#define WM8325_DC4_FLT_SHIFT                    (     7)  /* DC4_FLT */
#define WM8325_DC4_FLT_WIDTH                    (     1)  /* DC4_FLT */
#define WM8325_DC4_SOFT_START_MASK              (0x0030)  /* DC4_SOFT_START - [5:4] */
#define WM8325_DC4_SOFT_START_SHIFT             (     4)  /* DC4_SOFT_START - [5:4] */
#define WM8325_DC4_SOFT_START_WIDTH             (     2)  /* DC4_SOFT_START - [5:4] */
#define WM8325_DC4_STNBY_LIM_MASK               (0x000C)  /* DC4_STNBY_LIM - [3:2] */
#define WM8325_DC4_STNBY_LIM_SHIFT              (     2)  /* DC4_STNBY_LIM - [3:2] */
#define WM8325_DC4_STNBY_LIM_WIDTH              (     2)  /* DC4_STNBY_LIM - [3:2] */
#define WM8325_DC4_CAP_MASK                     (0x0003)  /* DC4_CAP - [1:0] */
#define WM8325_DC4_CAP_SHIFT                    (     0)  /* DC4_CAP - [1:0] */
#define WM8325_DC4_CAP_WIDTH                    (     2)  /* DC4_CAP - [1:0] */
                                            
/*                                                                                                   
 * R16485 (0x4065) - DC4 Control 2                                                 
 */                                          
#define WM8325_DC4_ERR_ACT_MASK                 (0xC000)  /* DC4_ERR_ACT - [15:14] */
#define WM8325_DC4_ERR_ACT_SHIFT                (    14)  /* DC4_ERR_ACT - [15:14] */
#define WM8325_DC4_ERR_ACT_WIDTH                (     2)  /* DC4_ERR_ACT - [15:14] */
#define WM8325_DC4_HWC_SRC_MASK                 (0x1800)  /* DC4_HWC_SRC - [12:11] */
#define WM8325_DC4_HWC_SRC_SHIFT                (    11)  /* DC4_HWC_SRC - [12:11] */
#define WM8325_DC4_HWC_SRC_WIDTH                (     2)  /* DC4_HWC_SRC - [12:11] */
#define WM8325_DC4_HWC_VSEL                     (0x0400)  /* DC4_HWC_VSEL */
#define WM8325_DC4_HWC_VSEL_MASK                (0x0400)  /* DC4_HWC_VSEL */
#define WM8325_DC4_HWC_VSEL_SHIFT               (    10)  /* DC4_HWC_VSEL */
#define WM8325_DC4_HWC_VSEL_WIDTH               (     1)  /* DC4_HWC_VSEL */
#define WM8325_DC4_HWC_MODE_MASK                (0x0300)  /* DC4_HWC_MODE - [9:8] */
#define WM8325_DC4_HWC_MODE_SHIFT               (     8)  /* DC4_HWC_MODE - [9:8] */
#define WM8325_DC4_HWC_MODE_WIDTH               (     2)  /* DC4_HWC_MODE - [9:8] */
#define WM8325_DC4_OVP                          (0x0080)  /* DC4_OVP */
#define WM8325_DC4_OVP_MASK                     (0x0080)  /* DC4_OVP */
#define WM8325_DC4_OVP_SHIFT                    (     7)  /* DC4_OVP */
#define WM8325_DC4_OVP_WIDTH                    (     1)  /* DC4_OVP */
                                             
/*                                                                                                      
 * R16486 (0x4066) - DC4 ON Config                                                  
 */                                           
#define WM8325_DC4_ON_SLOT_MASK                 (0xE000)  /* DC4_ON_SLOT - [15:13] */
#define WM8325_DC4_ON_SLOT_SHIFT                (    13)  /* DC4_ON_SLOT - [15:13] */
#define WM8325_DC4_ON_SLOT_WIDTH                (     3)  /* DC4_ON_SLOT - [15:13] */
#define WM8325_DC4_ON_MODE_MASK                 (0x0300)  /* DC4_ON_MODE - [9:8] */
#define WM8325_DC4_ON_MODE_SHIFT                (     8)  /* DC4_ON_MODE - [9:8] */
#define WM8325_DC4_ON_MODE_WIDTH                (     2)  /* DC4_ON_MODE - [9:8] */
#define WM8325_DC4_ON_VSEL_0_MASK               (0x007C)  /* DC4_ON_VSEL - [6:2] */
#define WM8325_DC4_ON_VSEL_0_SHIFT              (     2)  /* DC4_ON_VSEL - [6:2] */
#define WM8325_DC4_ON_VSEL_0_WIDTH              (     5)  /* DC4_ON_VSEL - [6:2] */
#define WM8325_DC4_ON_VSEL_MASK                 (0x0003)  /* DC4_ON_VSEL - [1:0] */
#define WM8325_DC4_ON_VSEL_SHIFT                (     0)  /* DC4_ON_VSEL - [1:0] */
#define WM8325_DC4_ON_VSEL_WIDTH                (     2)  /* DC4_ON_VSEL - [1:0] */
                                             
/*                                                                                                      
 * R16487 (0x4067) - DC4 SLEEP Control                                            
 */                                           
#define WM8325_DC4_SLP_SLOT_MASK                (0xE000)  /* DC4_SLP_SLOT - [15:13] */
#define WM8325_DC4_SLP_SLOT_SHIFT               (    13)  /* DC4_SLP_SLOT - [15:13] */
#define WM8325_DC4_SLP_SLOT_WIDTH               (     3)  /* DC4_SLP_SLOT - [15:13] */
#define WM8325_DC4_SLP_MODE_MASK                (0x0300)  /* DC4_SLP_MODE - [9:8] */
#define WM8325_DC4_SLP_MODE_SHIFT               (     8)  /* DC4_SLP_MODE - [9:8] */
#define WM8325_DC4_SLP_MODE_WIDTH               (     2)  /* DC4_SLP_MODE - [9:8] */
#define WM8325_DC4_SLP_VSEL_MASK                (0x007F)  /* DC4_SLP_VSEL - [6:0] */
#define WM8325_DC4_SLP_VSEL_SHIFT               (     0)  /* DC4_SLP_VSEL - [6:0] */
#define WM8325_DC4_SLP_VSEL_WIDTH               (     7)  /* DC4_SLP_VSEL - [6:0] */
                                             
/*                                                                                                      
 * R16488 (0x4068) - LDO1 Control                                                     
 */                                           
#define WM8325_LDO1_ERR_ACT_MASK                (0xC000)  /* LDO1_ERR_ACT - [15:14] */
#define WM8325_LDO1_ERR_ACT_SHIFT               (    14)  /* LDO1_ERR_ACT - [15:14] */
#define WM8325_LDO1_ERR_ACT_WIDTH               (     2)  /* LDO1_ERR_ACT - [15:14] */
#define WM8325_LDO1_HWC_SRC_MASK                (0x1800)  /* LDO1_HWC_SRC - [12:11] */
#define WM8325_LDO1_HWC_SRC_SHIFT               (    11)  /* LDO1_HWC_SRC - [12:11] */
#define WM8325_LDO1_HWC_SRC_WIDTH               (     2)  /* LDO1_HWC_SRC - [12:11] */
#define WM8325_LDO1_HWC_VSEL                    (0x0400)  /* LDO1_HWC_VSEL */
#define WM8325_LDO1_HWC_VSEL_MASK               (0x0400)  /* LDO1_HWC_VSEL */
#define WM8325_LDO1_HWC_VSEL_SHIFT              (    10)  /* LDO1_HWC_VSEL */
#define WM8325_LDO1_HWC_VSEL_WIDTH              (     1)  /* LDO1_HWC_VSEL */
#define WM8325_LDO1_HWC_MODE_MASK               (0x0300)  /* LDO1_HWC_MODE - [9:8] */
#define WM8325_LDO1_HWC_MODE_SHIFT              (     8)  /* LDO1_HWC_MODE - [9:8] */
#define WM8325_LDO1_HWC_MODE_WIDTH              (     2)  /* LDO1_HWC_MODE - [9:8] */
#define WM8325_LDO1_FLT                         (0x0080)  /* LDO1_FLT */
#define WM8325_LDO1_FLT_MASK                    (0x0080)  /* LDO1_FLT */
#define WM8325_LDO1_FLT_SHIFT                   (     7)  /* LDO1_FLT */
#define WM8325_LDO1_FLT_WIDTH                   (     1)  /* LDO1_FLT */
#define WM8325_LDO1_SWI                         (0x0040)  /* LDO1_SWI */
#define WM8325_LDO1_SWI_MASK                    (0x0040)  /* LDO1_SWI */
#define WM8325_LDO1_SWI_SHIFT                   (     6)  /* LDO1_SWI */
#define WM8325_LDO1_SWI_WIDTH                   (     1)  /* LDO1_SWI */
#define WM8325_LDO1_LP_MODE                     (0x0001)  /* LDO1_LP_MODE */
#define WM8325_LDO1_LP_MODE_MASK                (0x0001)  /* LDO1_LP_MODE */
#define WM8325_LDO1_LP_MODE_SHIFT               (     0)  /* LDO1_LP_MODE */
#define WM8325_LDO1_LP_MODE_WIDTH               (     1)  /* LDO1_LP_MODE */
                                            
/*                                                                                                   
 * R16489 (0x4069) - LDO1 ON Control                                             
 */                                          
#define WM8325_LDO1_ON_SLOT_MASK                (0xE000)  /* LDO1_ON_SLOT - [15:13] */
#define WM8325_LDO1_ON_SLOT_SHIFT               (    13)  /* LDO1_ON_SLOT - [15:13] */
#define WM8325_LDO1_ON_SLOT_WIDTH               (     3)  /* LDO1_ON_SLOT - [15:13] */
#define WM8325_LDO1_ON_MODE                     (0x0100)  /* LDO1_ON_MODE */
#define WM8325_LDO1_ON_MODE_MASK                (0x0100)  /* LDO1_ON_MODE */
#define WM8325_LDO1_ON_MODE_SHIFT               (     8)  /* LDO1_ON_MODE */
#define WM8325_LDO1_ON_MODE_WIDTH               (     1)  /* LDO1_ON_MODE */
#define WM8325_LDO1_ON_VSEL_MASK                (0x001F)  /* LDO1_ON_VSEL - [4:0] */
#define WM8325_LDO1_ON_VSEL_SHIFT               (     0)  /* LDO1_ON_VSEL - [4:0] */
#define WM8325_LDO1_ON_VSEL_WIDTH               (     5)  /* LDO1_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16490 (0x406A) - LDO1 SLEEP Control                                          
 */                                           
#define WM8325_LDO1_SLP_SLOT_MASK               (0xE000)  /* LDO1_SLP_SLOT - [15:13] */
#define WM8325_LDO1_SLP_SLOT_SHIFT              (    13)  /* LDO1_SLP_SLOT - [15:13] */
#define WM8325_LDO1_SLP_SLOT_WIDTH              (     3)  /* LDO1_SLP_SLOT - [15:13] */
#define WM8325_LDO1_SLP_MODE                    (0x0100)  /* LDO1_SLP_MODE */
#define WM8325_LDO1_SLP_MODE_MASK               (0x0100)  /* LDO1_SLP_MODE */
#define WM8325_LDO1_SLP_MODE_SHIFT              (     8)  /* LDO1_SLP_MODE */
#define WM8325_LDO1_SLP_MODE_WIDTH              (     1)  /* LDO1_SLP_MODE */
#define WM8325_LDO1_SLP_VSEL_MASK               (0x001F)  /* LDO1_SLP_VSEL - [4:0] */
#define WM8325_LDO1_SLP_VSEL_SHIFT              (     0)  /* LDO1_SLP_VSEL - [4:0] */
#define WM8325_LDO1_SLP_VSEL_WIDTH              (     5)  /* LDO1_SLP_VSEL - [4:0] */
                                              
/*                                                                                                        
 * R16491 (0x406B) - LDO2 Control                                                       
 */                                            
#define WM8325_LDO2_ERR_ACT_MASK                (0xC000)  /* LDO2_ERR_ACT - [15:14] */
#define WM8325_LDO2_ERR_ACT_SHIFT               (    14)  /* LDO2_ERR_ACT - [15:14] */
#define WM8325_LDO2_ERR_ACT_WIDTH               (     2)  /* LDO2_ERR_ACT - [15:14] */
#define WM8325_LDO2_HWC_SRC_MASK                (0x1800)  /* LDO2_HWC_SRC - [12:11] */
#define WM8325_LDO2_HWC_SRC_SHIFT               (    11)  /* LDO2_HWC_SRC - [12:11] */
#define WM8325_LDO2_HWC_SRC_WIDTH               (     2)  /* LDO2_HWC_SRC - [12:11] */
#define WM8325_LDO2_HWC_VSEL                    (0x0400)  /* LDO2_HWC_VSEL */
#define WM8325_LDO2_HWC_VSEL_MASK               (0x0400)  /* LDO2_HWC_VSEL */
#define WM8325_LDO2_HWC_VSEL_SHIFT              (    10)  /* LDO2_HWC_VSEL */
#define WM8325_LDO2_HWC_VSEL_WIDTH              (     1)  /* LDO2_HWC_VSEL */
#define WM8325_LDO2_HWC_MODE_MASK               (0x0300)  /* LDO2_HWC_MODE - [9:8] */
#define WM8325_LDO2_HWC_MODE_SHIFT              (     8)  /* LDO2_HWC_MODE - [9:8] */
#define WM8325_LDO2_HWC_MODE_WIDTH              (     2)  /* LDO2_HWC_MODE - [9:8] */
#define WM8325_LDO2_FLT                         (0x0080)  /* LDO2_FLT */
#define WM8325_LDO2_FLT_MASK                    (0x0080)  /* LDO2_FLT */
#define WM8325_LDO2_FLT_SHIFT                   (     7)  /* LDO2_FLT */
#define WM8325_LDO2_FLT_WIDTH                   (     1)  /* LDO2_FLT */
#define WM8325_LDO2_SWI                         (0x0040)  /* LDO2_SWI */
#define WM8325_LDO2_SWI_MASK                    (0x0040)  /* LDO2_SWI */
#define WM8325_LDO2_SWI_SHIFT                   (     6)  /* LDO2_SWI */
#define WM8325_LDO2_SWI_WIDTH                   (     1)  /* LDO2_SWI */
#define WM8325_LDO2_LP_MODE                     (0x0001)  /* LDO2_LP_MODE */
#define WM8325_LDO2_LP_MODE_MASK                (0x0001)  /* LDO2_LP_MODE */
#define WM8325_LDO2_LP_MODE_SHIFT               (     0)  /* LDO2_LP_MODE */
#define WM8325_LDO2_LP_MODE_WIDTH               (     1)  /* LDO2_LP_MODE */
                                             
/*                                                                                                      
 * R16492 (0x406C) - LDO2 ON Control                                               
 */                                           
#define WM8325_LDO2_ON_SLOT_MASK                (0xE000)  /* LDO2_ON_SLOT - [15:13] */
#define WM8325_LDO2_ON_SLOT_SHIFT               (    13)  /* LDO2_ON_SLOT - [15:13] */
#define WM8325_LDO2_ON_SLOT_WIDTH               (     3)  /* LDO2_ON_SLOT - [15:13] */
#define WM8325_LDO2_ON_MODE                     (0x0100)  /* LDO2_ON_MODE */
#define WM8325_LDO2_ON_MODE_MASK                (0x0100)  /* LDO2_ON_MODE */
#define WM8325_LDO2_ON_MODE_SHIFT               (     8)  /* LDO2_ON_MODE */
#define WM8325_LDO2_ON_MODE_WIDTH               (     1)  /* LDO2_ON_MODE */
#define WM8325_LDO2_ON_VSEL_MASK                (0x001F)  /* LDO2_ON_VSEL - [4:0] */
#define WM8325_LDO2_ON_VSEL_SHIFT               (     0)  /* LDO2_ON_VSEL - [4:0] */
#define WM8325_LDO2_ON_VSEL_WIDTH               (     5)  /* LDO2_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16493 (0x406D) - LDO2 SLEEP Control                                          
 */                                           
#define WM8325_LDO2_SLP_SLOT_MASK               (0xE000)  /* LDO2_SLP_SLOT - [15:13] */
#define WM8325_LDO2_SLP_SLOT_SHIFT              (    13)  /* LDO2_SLP_SLOT - [15:13] */
#define WM8325_LDO2_SLP_SLOT_WIDTH              (     3)  /* LDO2_SLP_SLOT - [15:13] */
#define WM8325_LDO2_SLP_MODE                    (0x0100)  /* LDO2_SLP_MODE */
#define WM8325_LDO2_SLP_MODE_MASK               (0x0100)  /* LDO2_SLP_MODE */
#define WM8325_LDO2_SLP_MODE_SHIFT              (     8)  /* LDO2_SLP_MODE */
#define WM8325_LDO2_SLP_MODE_WIDTH              (     1)  /* LDO2_SLP_MODE */
#define WM8325_LDO2_SLP_VSEL_MASK               (0x001F)  /* LDO2_SLP_VSEL - [4:0] */
#define WM8325_LDO2_SLP_VSEL_SHIFT              (     0)  /* LDO2_SLP_VSEL - [4:0] */
#define WM8325_LDO2_SLP_VSEL_WIDTH              (     5)  /* LDO2_SLP_VSEL - [4:0] */
                                              
/*                                                                                                        
 * R16494 (0x406E) - LDO3 Control                                                       
 */                                            
#define WM8325_LDO3_ERR_ACT_MASK                (0xC000)  /* LDO3_ERR_ACT - [15:14] */
#define WM8325_LDO3_ERR_ACT_SHIFT               (    14)  /* LDO3_ERR_ACT - [15:14] */
#define WM8325_LDO3_ERR_ACT_WIDTH               (     2)  /* LDO3_ERR_ACT - [15:14] */
#define WM8325_LDO3_HWC_SRC_MASK                (0x1800)  /* LDO3_HWC_SRC - [12:11] */
#define WM8325_LDO3_HWC_SRC_SHIFT               (    11)  /* LDO3_HWC_SRC - [12:11] */
#define WM8325_LDO3_HWC_SRC_WIDTH               (     2)  /* LDO3_HWC_SRC - [12:11] */
#define WM8325_LDO3_HWC_VSEL                    (0x0400)  /* LDO3_HWC_VSEL */
#define WM8325_LDO3_HWC_VSEL_MASK               (0x0400)  /* LDO3_HWC_VSEL */
#define WM8325_LDO3_HWC_VSEL_SHIFT              (    10)  /* LDO3_HWC_VSEL */
#define WM8325_LDO3_HWC_VSEL_WIDTH              (     1)  /* LDO3_HWC_VSEL */
#define WM8325_LDO3_HWC_MODE_MASK               (0x0300)  /* LDO3_HWC_MODE - [9:8] */
#define WM8325_LDO3_HWC_MODE_SHIFT              (     8)  /* LDO3_HWC_MODE - [9:8] */
#define WM8325_LDO3_HWC_MODE_WIDTH              (     2)  /* LDO3_HWC_MODE - [9:8] */
#define WM8325_LDO3_FLT                         (0x0080)  /* LDO3_FLT */
#define WM8325_LDO3_FLT_MASK                    (0x0080)  /* LDO3_FLT */
#define WM8325_LDO3_FLT_SHIFT                   (     7)  /* LDO3_FLT */
#define WM8325_LDO3_FLT_WIDTH                   (     1)  /* LDO3_FLT */
#define WM8325_LDO3_SWI                         (0x0040)  /* LDO3_SWI */
#define WM8325_LDO3_SWI_MASK                    (0x0040)  /* LDO3_SWI */
#define WM8325_LDO3_SWI_SHIFT                   (     6)  /* LDO3_SWI */
#define WM8325_LDO3_SWI_WIDTH                   (     1)  /* LDO3_SWI */
#define WM8325_LDO3_LP_MODE                     (0x0001)  /* LDO3_LP_MODE */
#define WM8325_LDO3_LP_MODE_MASK                (0x0001)  /* LDO3_LP_MODE */
#define WM8325_LDO3_LP_MODE_SHIFT               (     0)  /* LDO3_LP_MODE */
#define WM8325_LDO3_LP_MODE_WIDTH               (     1)  /* LDO3_LP_MODE */
                                             
/*                                                                                                      
 * R16495 (0x406F) - LDO3 ON Control                                               
 */                                           
#define WM8325_LDO3_ON_SLOT_MASK                (0xE000)  /* LDO3_ON_SLOT - [15:13] */
#define WM8325_LDO3_ON_SLOT_SHIFT               (    13)  /* LDO3_ON_SLOT - [15:13] */
#define WM8325_LDO3_ON_SLOT_WIDTH               (     3)  /* LDO3_ON_SLOT - [15:13] */
#define WM8325_LDO3_ON_MODE                     (0x0100)  /* LDO3_ON_MODE */
#define WM8325_LDO3_ON_MODE_MASK                (0x0100)  /* LDO3_ON_MODE */
#define WM8325_LDO3_ON_MODE_SHIFT               (     8)  /* LDO3_ON_MODE */
#define WM8325_LDO3_ON_MODE_WIDTH               (     1)  /* LDO3_ON_MODE */
#define WM8325_LDO3_ON_VSEL_MASK                (0x001F)  /* LDO3_ON_VSEL - [4:0] */
#define WM8325_LDO3_ON_VSEL_SHIFT               (     0)  /* LDO3_ON_VSEL - [4:0] */
#define WM8325_LDO3_ON_VSEL_WIDTH               (     5)  /* LDO3_ON_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R16496 (0x4070) - LDO3 SLEEP Control                                        
 */                                          
#define WM8325_LDO3_SLP_SLOT_MASK               (0xE000)  /* LDO3_SLP_SLOT - [15:13] */
#define WM8325_LDO3_SLP_SLOT_SHIFT              (    13)  /* LDO3_SLP_SLOT - [15:13] */
#define WM8325_LDO3_SLP_SLOT_WIDTH              (     3)  /* LDO3_SLP_SLOT - [15:13] */
#define WM8325_LDO3_SLP_MODE                    (0x0100)  /* LDO3_SLP_MODE */
#define WM8325_LDO3_SLP_MODE_MASK               (0x0100)  /* LDO3_SLP_MODE */
#define WM8325_LDO3_SLP_MODE_SHIFT              (     8)  /* LDO3_SLP_MODE */
#define WM8325_LDO3_SLP_MODE_WIDTH              (     1)  /* LDO3_SLP_MODE */
#define WM8325_LDO3_SLP_VSEL_MASK               (0x001F)  /* LDO3_SLP_VSEL - [4:0] */
#define WM8325_LDO3_SLP_VSEL_SHIFT              (     0)  /* LDO3_SLP_VSEL - [4:0] */
#define WM8325_LDO3_SLP_VSEL_WIDTH              (     5)  /* LDO3_SLP_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16497 (0x4071) - LDO4 Control                                                     
 */                                           
#define WM8325_LDO4_ERR_ACT_MASK                (0xC000)  /* LDO4_ERR_ACT - [15:14] */
#define WM8325_LDO4_ERR_ACT_SHIFT               (    14)  /* LDO4_ERR_ACT - [15:14] */
#define WM8325_LDO4_ERR_ACT_WIDTH               (     2)  /* LDO4_ERR_ACT - [15:14] */
#define WM8325_LDO4_HWC_SRC_MASK                (0x1800)  /* LDO4_HWC_SRC - [12:11] */
#define WM8325_LDO4_HWC_SRC_SHIFT               (    11)  /* LDO4_HWC_SRC - [12:11] */
#define WM8325_LDO4_HWC_SRC_WIDTH               (     2)  /* LDO4_HWC_SRC - [12:11] */
#define WM8325_LDO4_HWC_VSEL                    (0x0400)  /* LDO4_HWC_VSEL */
#define WM8325_LDO4_HWC_VSEL_MASK               (0x0400)  /* LDO4_HWC_VSEL */
#define WM8325_LDO4_HWC_VSEL_SHIFT              (    10)  /* LDO4_HWC_VSEL */
#define WM8325_LDO4_HWC_VSEL_WIDTH              (     1)  /* LDO4_HWC_VSEL */
#define WM8325_LDO4_HWC_MODE_MASK               (0x0300)  /* LDO4_HWC_MODE - [9:8] */
#define WM8325_LDO4_HWC_MODE_SHIFT              (     8)  /* LDO4_HWC_MODE - [9:8] */
#define WM8325_LDO4_HWC_MODE_WIDTH              (     2)  /* LDO4_HWC_MODE - [9:8] */
#define WM8325_LDO4_FLT                         (0x0080)  /* LDO4_FLT */
#define WM8325_LDO4_FLT_MASK                    (0x0080)  /* LDO4_FLT */
#define WM8325_LDO4_FLT_SHIFT                   (     7)  /* LDO4_FLT */
#define WM8325_LDO4_FLT_WIDTH                   (     1)  /* LDO4_FLT */
#define WM8325_LDO4_SWI                         (0x0040)  /* LDO4_SWI */
#define WM8325_LDO4_SWI_MASK                    (0x0040)  /* LDO4_SWI */
#define WM8325_LDO4_SWI_SHIFT                   (     6)  /* LDO4_SWI */
#define WM8325_LDO4_SWI_WIDTH                   (     1)  /* LDO4_SWI */
#define WM8325_LDO4_LP_MODE                     (0x0001)  /* LDO4_LP_MODE */
#define WM8325_LDO4_LP_MODE_MASK                (0x0001)  /* LDO4_LP_MODE */
#define WM8325_LDO4_LP_MODE_SHIFT               (     0)  /* LDO4_LP_MODE */
#define WM8325_LDO4_LP_MODE_WIDTH               (     1)  /* LDO4_LP_MODE */
                                             
/*                                                                                                      
 * R16498 (0x4072) - LDO4 ON Control                                               
 */                                           
#define WM8325_LDO4_ON_SLOT_MASK                (0xE000)  /* LDO4_ON_SLOT - [15:13] */
#define WM8325_LDO4_ON_SLOT_SHIFT               (    13)  /* LDO4_ON_SLOT - [15:13] */
#define WM8325_LDO4_ON_SLOT_WIDTH               (     3)  /* LDO4_ON_SLOT - [15:13] */
#define WM8325_LDO4_ON_MODE                     (0x0100)  /* LDO4_ON_MODE */
#define WM8325_LDO4_ON_MODE_MASK                (0x0100)  /* LDO4_ON_MODE */
#define WM8325_LDO4_ON_MODE_SHIFT               (     8)  /* LDO4_ON_MODE */
#define WM8325_LDO4_ON_MODE_WIDTH               (     1)  /* LDO4_ON_MODE */
#define WM8325_LDO4_ON_VSEL_MASK                (0x001F)  /* LDO4_ON_VSEL - [4:0] */
#define WM8325_LDO4_ON_VSEL_SHIFT               (     0)  /* LDO4_ON_VSEL - [4:0] */
#define WM8325_LDO4_ON_VSEL_WIDTH               (     5)  /* LDO4_ON_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R16499 (0x4073) - LDO4 SLEEP Control                                        
 */                                          
#define WM8325_LDO4_SLP_SLOT_MASK               (0xE000)  /* LDO4_SLP_SLOT - [15:13] */
#define WM8325_LDO4_SLP_SLOT_SHIFT              (    13)  /* LDO4_SLP_SLOT - [15:13] */
#define WM8325_LDO4_SLP_SLOT_WIDTH              (     3)  /* LDO4_SLP_SLOT - [15:13] */
#define WM8325_LDO4_SLP_MODE                    (0x0100)  /* LDO4_SLP_MODE */
#define WM8325_LDO4_SLP_MODE_MASK               (0x0100)  /* LDO4_SLP_MODE */
#define WM8325_LDO4_SLP_MODE_SHIFT              (     8)  /* LDO4_SLP_MODE */
#define WM8325_LDO4_SLP_MODE_WIDTH              (     1)  /* LDO4_SLP_MODE */
#define WM8325_LDO4_SLP_VSEL_MASK               (0x001F)  /* LDO4_SLP_VSEL - [4:0] */
#define WM8325_LDO4_SLP_VSEL_SHIFT              (     0)  /* LDO4_SLP_VSEL - [4:0] */
#define WM8325_LDO4_SLP_VSEL_WIDTH              (     5)  /* LDO4_SLP_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R16500 (0x4074) - LDO5 Control                                                  
 */                                          
#define WM8325_LDO5_ERR_ACT_MASK                (0xC000)  /* LDO5_ERR_ACT - [15:14] */
#define WM8325_LDO5_ERR_ACT_SHIFT               (    14)  /* LDO5_ERR_ACT - [15:14] */
#define WM8325_LDO5_ERR_ACT_WIDTH               (     2)  /* LDO5_ERR_ACT - [15:14] */
#define WM8325_LDO5_HWC_SRC_MASK                (0x1800)  /* LDO5_HWC_SRC - [12:11] */
#define WM8325_LDO5_HWC_SRC_SHIFT               (    11)  /* LDO5_HWC_SRC - [12:11] */
#define WM8325_LDO5_HWC_SRC_WIDTH               (     2)  /* LDO5_HWC_SRC - [12:11] */
#define WM8325_LDO5_HWC_VSEL                    (0x0400)  /* LDO5_HWC_VSEL */
#define WM8325_LDO5_HWC_VSEL_MASK               (0x0400)  /* LDO5_HWC_VSEL */
#define WM8325_LDO5_HWC_VSEL_SHIFT              (    10)  /* LDO5_HWC_VSEL */
#define WM8325_LDO5_HWC_VSEL_WIDTH              (     1)  /* LDO5_HWC_VSEL */
#define WM8325_LDO5_HWC_MODE_MASK               (0x0300)  /* LDO5_HWC_MODE - [9:8] */
#define WM8325_LDO5_HWC_MODE_SHIFT              (     8)  /* LDO5_HWC_MODE - [9:8] */
#define WM8325_LDO5_HWC_MODE_WIDTH              (     2)  /* LDO5_HWC_MODE - [9:8] */
#define WM8325_LDO5_FLT                         (0x0080)  /* LDO5_FLT */
#define WM8325_LDO5_FLT_MASK                    (0x0080)  /* LDO5_FLT */
#define WM8325_LDO5_FLT_SHIFT                   (     7)  /* LDO5_FLT */
#define WM8325_LDO5_FLT_WIDTH                   (     1)  /* LDO5_FLT */
#define WM8325_LDO5_SWI                         (0x0040)  /* LDO5_SWI */
#define WM8325_LDO5_SWI_MASK                    (0x0040)  /* LDO5_SWI */
#define WM8325_LDO5_SWI_SHIFT                   (     6)  /* LDO5_SWI */
#define WM8325_LDO5_SWI_WIDTH                   (     1)  /* LDO5_SWI */
#define WM8325_LDO5_LP_MODE                     (0x0001)  /* LDO5_LP_MODE */
#define WM8325_LDO5_LP_MODE_MASK                (0x0001)  /* LDO5_LP_MODE */
#define WM8325_LDO5_LP_MODE_SHIFT               (     0)  /* LDO5_LP_MODE */
#define WM8325_LDO5_LP_MODE_WIDTH               (     1)  /* LDO5_LP_MODE */
                                           
/*                                                                                                 
 * R16501 (0x4075) - LDO5 ON Control                                          
 */                                         
#define WM8325_LDO5_ON_SLOT_MASK                (0xE000)  /* LDO5_ON_SLOT - [15:13] */
#define WM8325_LDO5_ON_SLOT_SHIFT               (    13)  /* LDO5_ON_SLOT - [15:13] */
#define WM8325_LDO5_ON_SLOT_WIDTH               (     3)  /* LDO5_ON_SLOT - [15:13] */
#define WM8325_LDO5_ON_MODE                     (0x0100)  /* LDO5_ON_MODE */
#define WM8325_LDO5_ON_MODE_MASK                (0x0100)  /* LDO5_ON_MODE */
#define WM8325_LDO5_ON_MODE_SHIFT               (     8)  /* LDO5_ON_MODE */
#define WM8325_LDO5_ON_MODE_WIDTH               (     1)  /* LDO5_ON_MODE */
#define WM8325_LDO5_ON_VSEL_MASK                (0x001F)  /* LDO5_ON_VSEL - [4:0] */
#define WM8325_LDO5_ON_VSEL_SHIFT               (     0)  /* LDO5_ON_VSEL - [4:0] */
#define WM8325_LDO5_ON_VSEL_WIDTH               (     5)  /* LDO5_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16502 (0x4076) - LDO5 SLEEP Control                                          
 */                                           
#define WM8325_LDO5_SLP_SLOT_MASK               (0xE000)  /* LDO5_SLP_SLOT - [15:13] */
#define WM8325_LDO5_SLP_SLOT_SHIFT              (    13)  /* LDO5_SLP_SLOT - [15:13] */
#define WM8325_LDO5_SLP_SLOT_WIDTH              (     3)  /* LDO5_SLP_SLOT - [15:13] */
#define WM8325_LDO5_SLP_MODE                    (0x0100)  /* LDO5_SLP_MODE */
#define WM8325_LDO5_SLP_MODE_MASK               (0x0100)  /* LDO5_SLP_MODE */
#define WM8325_LDO5_SLP_MODE_SHIFT              (     8)  /* LDO5_SLP_MODE */
#define WM8325_LDO5_SLP_MODE_WIDTH              (     1)  /* LDO5_SLP_MODE */
#define WM8325_LDO5_SLP_VSEL_MASK               (0x001F)  /* LDO5_SLP_VSEL - [4:0] */
#define WM8325_LDO5_SLP_VSEL_SHIFT              (     0)  /* LDO5_SLP_VSEL - [4:0] */
#define WM8325_LDO5_SLP_VSEL_WIDTH              (     5)  /* LDO5_SLP_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R16503 (0x4077) - LDO6 Control                                                  
 */                                          
#define WM8325_LDO6_ERR_ACT_MASK                (0xC000)  /* LDO6_ERR_ACT - [15:14] */
#define WM8325_LDO6_ERR_ACT_SHIFT               (    14)  /* LDO6_ERR_ACT - [15:14] */
#define WM8325_LDO6_ERR_ACT_WIDTH               (     2)  /* LDO6_ERR_ACT - [15:14] */
#define WM8325_LDO6_HWC_SRC_MASK                (0x1800)  /* LDO6_HWC_SRC - [12:11] */
#define WM8325_LDO6_HWC_SRC_SHIFT               (    11)  /* LDO6_HWC_SRC - [12:11] */
#define WM8325_LDO6_HWC_SRC_WIDTH               (     2)  /* LDO6_HWC_SRC - [12:11] */
#define WM8325_LDO6_HWC_VSEL                    (0x0400)  /* LDO6_HWC_VSEL */
#define WM8325_LDO6_HWC_VSEL_MASK               (0x0400)  /* LDO6_HWC_VSEL */
#define WM8325_LDO6_HWC_VSEL_SHIFT              (    10)  /* LDO6_HWC_VSEL */
#define WM8325_LDO6_HWC_VSEL_WIDTH              (     1)  /* LDO6_HWC_VSEL */
#define WM8325_LDO6_HWC_MODE_MASK               (0x0300)  /* LDO6_HWC_MODE - [9:8] */
#define WM8325_LDO6_HWC_MODE_SHIFT              (     8)  /* LDO6_HWC_MODE - [9:8] */
#define WM8325_LDO6_HWC_MODE_WIDTH              (     2)  /* LDO6_HWC_MODE - [9:8] */
#define WM8325_LDO6_FLT                         (0x0080)  /* LDO6_FLT */
#define WM8325_LDO6_FLT_MASK                    (0x0080)  /* LDO6_FLT */
#define WM8325_LDO6_FLT_SHIFT                   (     7)  /* LDO6_FLT */
#define WM8325_LDO6_FLT_WIDTH                   (     1)  /* LDO6_FLT */
#define WM8325_LDO6_SWI                         (0x0040)  /* LDO6_SWI */
#define WM8325_LDO6_SWI_MASK                    (0x0040)  /* LDO6_SWI */
#define WM8325_LDO6_SWI_SHIFT                   (     6)  /* LDO6_SWI */
#define WM8325_LDO6_SWI_WIDTH                   (     1)  /* LDO6_SWI */
#define WM8325_LDO6_LP_MODE                     (0x0001)  /* LDO6_LP_MODE */
#define WM8325_LDO6_LP_MODE_MASK                (0x0001)  /* LDO6_LP_MODE */
#define WM8325_LDO6_LP_MODE_SHIFT               (     0)  /* LDO6_LP_MODE */
#define WM8325_LDO6_LP_MODE_WIDTH               (     1)  /* LDO6_LP_MODE */
                                              
/*                                                                                                        
 * R16504 (0x4078) - LDO6 ON Control                                                 
 */                                            
#define WM8325_LDO6_ON_SLOT_MASK                (0xE000)  /* LDO6_ON_SLOT - [15:13] */
#define WM8325_LDO6_ON_SLOT_SHIFT               (    13)  /* LDO6_ON_SLOT - [15:13] */
#define WM8325_LDO6_ON_SLOT_WIDTH               (     3)  /* LDO6_ON_SLOT - [15:13] */
#define WM8325_LDO6_ON_MODE                     (0x0100)  /* LDO6_ON_MODE */
#define WM8325_LDO6_ON_MODE_MASK                (0x0100)  /* LDO6_ON_MODE */
#define WM8325_LDO6_ON_MODE_SHIFT               (     8)  /* LDO6_ON_MODE */
#define WM8325_LDO6_ON_MODE_WIDTH               (     1)  /* LDO6_ON_MODE */
#define WM8325_LDO6_ON_VSEL_MASK                (0x001F)  /* LDO6_ON_VSEL - [4:0] */
#define WM8325_LDO6_ON_VSEL_SHIFT               (     0)  /* LDO6_ON_VSEL - [4:0] */
#define WM8325_LDO6_ON_VSEL_WIDTH               (     5)  /* LDO6_ON_VSEL - [4:0] */
                                           
/*                                                                                                 
 * R16505 (0x4079) - LDO6 SLEEP Control                                     
 */                                         
#define WM8325_LDO6_SLP_SLOT_MASK               (0xE000)  /* LDO6_SLP_SLOT - [15:13] */
#define WM8325_LDO6_SLP_SLOT_SHIFT              (    13)  /* LDO6_SLP_SLOT - [15:13] */
#define WM8325_LDO6_SLP_SLOT_WIDTH              (     3)  /* LDO6_SLP_SLOT - [15:13] */
#define WM8325_LDO6_SLP_MODE                    (0x0100)  /* LDO6_SLP_MODE */
#define WM8325_LDO6_SLP_MODE_MASK               (0x0100)  /* LDO6_SLP_MODE */
#define WM8325_LDO6_SLP_MODE_SHIFT              (     8)  /* LDO6_SLP_MODE */
#define WM8325_LDO6_SLP_MODE_WIDTH              (     1)  /* LDO6_SLP_MODE */
#define WM8325_LDO6_SLP_VSEL_MASK               (0x001F)  /* LDO6_SLP_VSEL - [4:0] */
#define WM8325_LDO6_SLP_VSEL_SHIFT              (     0)  /* LDO6_SLP_VSEL - [4:0] */
#define WM8325_LDO6_SLP_VSEL_WIDTH              (     5)  /* LDO6_SLP_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16506 (0x407A) - LDO7 Control                                                    
 */                                           
#define WM8325_LDO7_ERR_ACT_MASK                (0xC000)  /* LDO7_ERR_ACT - [15:14] */
#define WM8325_LDO7_ERR_ACT_SHIFT               (    14)  /* LDO7_ERR_ACT - [15:14] */
#define WM8325_LDO7_ERR_ACT_WIDTH               (     2)  /* LDO7_ERR_ACT - [15:14] */
#define WM8325_LDO7_HWC_SRC_MASK                (0x1800)  /* LDO7_HWC_SRC - [12:11] */
#define WM8325_LDO7_HWC_SRC_SHIFT               (    11)  /* LDO7_HWC_SRC - [12:11] */
#define WM8325_LDO7_HWC_SRC_WIDTH               (     2)  /* LDO7_HWC_SRC - [12:11] */
#define WM8325_LDO7_HWC_VSEL                    (0x0400)  /* LDO7_HWC_VSEL */
#define WM8325_LDO7_HWC_VSEL_MASK               (0x0400)  /* LDO7_HWC_VSEL */
#define WM8325_LDO7_HWC_VSEL_SHIFT              (    10)  /* LDO7_HWC_VSEL */
#define WM8325_LDO7_HWC_VSEL_WIDTH              (     1)  /* LDO7_HWC_VSEL */
#define WM8325_LDO7_HWC_MODE_MASK               (0x0300)  /* LDO7_HWC_MODE - [9:8] */
#define WM8325_LDO7_HWC_MODE_SHIFT              (     8)  /* LDO7_HWC_MODE - [9:8] */
#define WM8325_LDO7_HWC_MODE_WIDTH              (     2)  /* LDO7_HWC_MODE - [9:8] */
#define WM8325_LDO7_FLT                         (0x0080)  /* LDO7_FLT */
#define WM8325_LDO7_FLT_MASK                    (0x0080)  /* LDO7_FLT */
#define WM8325_LDO7_FLT_SHIFT                   (     7)  /* LDO7_FLT */
#define WM8325_LDO7_FLT_WIDTH                   (     1)  /* LDO7_FLT */
#define WM8325_LDO7_SWI                         (0x0040)  /* LDO7_SWI */
#define WM8325_LDO7_SWI_MASK                    (0x0040)  /* LDO7_SWI */
#define WM8325_LDO7_SWI_SHIFT                   (     6)  /* LDO7_SWI */
#define WM8325_LDO7_SWI_WIDTH                   (     1)  /* LDO7_SWI */
                                              
/*                                                                                                        
 * R16507 (0x407B) - LDO7 ON Control                                                 
 */                                            
#define WM8325_LDO7_ON_SLOT_MASK                (0xE000)  /* LDO7_ON_SLOT - [15:13] */
#define WM8325_LDO7_ON_SLOT_SHIFT               (    13)  /* LDO7_ON_SLOT - [15:13] */
#define WM8325_LDO7_ON_SLOT_WIDTH               (     3)  /* LDO7_ON_SLOT - [15:13] */
#define WM8325_LDO7_ON_MODE                     (0x0100)  /* LDO7_ON_MODE */
#define WM8325_LDO7_ON_MODE_MASK                (0x0100)  /* LDO7_ON_MODE */
#define WM8325_LDO7_ON_MODE_SHIFT               (     8)  /* LDO7_ON_MODE */
#define WM8325_LDO7_ON_MODE_WIDTH               (     1)  /* LDO7_ON_MODE */
#define WM8325_LDO7_ON_VSEL_MASK                (0x001F)  /* LDO7_ON_VSEL - [4:0] */
#define WM8325_LDO7_ON_VSEL_SHIFT               (     0)  /* LDO7_ON_VSEL - [4:0] */
#define WM8325_LDO7_ON_VSEL_WIDTH               (     5)  /* LDO7_ON_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R16508 (0x407C) - LDO7 SLEEP Control                                        
 */                                          
#define WM8325_LDO7_SLP_SLOT_MASK               (0xE000)  /* LDO7_SLP_SLOT - [15:13] */
#define WM8325_LDO7_SLP_SLOT_SHIFT              (    13)  /* LDO7_SLP_SLOT - [15:13] */
#define WM8325_LDO7_SLP_SLOT_WIDTH              (     3)  /* LDO7_SLP_SLOT - [15:13] */
#define WM8325_LDO7_SLP_MODE                    (0x0100)  /* LDO7_SLP_MODE */
#define WM8325_LDO7_SLP_MODE_MASK               (0x0100)  /* LDO7_SLP_MODE */
#define WM8325_LDO7_SLP_MODE_SHIFT              (     8)  /* LDO7_SLP_MODE */
#define WM8325_LDO7_SLP_MODE_WIDTH              (     1)  /* LDO7_SLP_MODE */
#define WM8325_LDO7_SLP_VSEL_MASK               (0x001F)  /* LDO7_SLP_VSEL - [4:0] */
#define WM8325_LDO7_SLP_VSEL_SHIFT              (     0)  /* LDO7_SLP_VSEL - [4:0] */
#define WM8325_LDO7_SLP_VSEL_WIDTH              (     5)  /* LDO7_SLP_VSEL - [4:0] */
                                           
/*                                                                                                 
 * R16509 (0x407D) - LDO8 Control                                               
 */                                         
#define WM8325_LDO8_ERR_ACT_MASK                (0xC000)  /* LDO8_ERR_ACT - [15:14] */
#define WM8325_LDO8_ERR_ACT_SHIFT               (    14)  /* LDO8_ERR_ACT - [15:14] */
#define WM8325_LDO8_ERR_ACT_WIDTH               (     2)  /* LDO8_ERR_ACT - [15:14] */
#define WM8325_LDO8_HWC_SRC_MASK                (0x1800)  /* LDO8_HWC_SRC - [12:11] */
#define WM8325_LDO8_HWC_SRC_SHIFT               (    11)  /* LDO8_HWC_SRC - [12:11] */
#define WM8325_LDO8_HWC_SRC_WIDTH               (     2)  /* LDO8_HWC_SRC - [12:11] */
#define WM8325_LDO8_HWC_VSEL                    (0x0400)  /* LDO8_HWC_VSEL */
#define WM8325_LDO8_HWC_VSEL_MASK               (0x0400)  /* LDO8_HWC_VSEL */
#define WM8325_LDO8_HWC_VSEL_SHIFT              (    10)  /* LDO8_HWC_VSEL */
#define WM8325_LDO8_HWC_VSEL_WIDTH              (     1)  /* LDO8_HWC_VSEL */
#define WM8325_LDO8_HWC_MODE_MASK               (0x0300)  /* LDO8_HWC_MODE - [9:8] */
#define WM8325_LDO8_HWC_MODE_SHIFT              (     8)  /* LDO8_HWC_MODE - [9:8] */
#define WM8325_LDO8_HWC_MODE_WIDTH              (     2)  /* LDO8_HWC_MODE - [9:8] */
#define WM8325_LDO8_FLT                         (0x0080)  /* LDO8_FLT */
#define WM8325_LDO8_FLT_MASK                    (0x0080)  /* LDO8_FLT */
#define WM8325_LDO8_FLT_SHIFT                   (     7)  /* LDO8_FLT */
#define WM8325_LDO8_FLT_WIDTH                   (     1)  /* LDO8_FLT */
#define WM8325_LDO8_SWI                         (0x0040)  /* LDO8_SWI */
#define WM8325_LDO8_SWI_MASK                    (0x0040)  /* LDO8_SWI */
#define WM8325_LDO8_SWI_SHIFT                   (     6)  /* LDO8_SWI */
#define WM8325_LDO8_SWI_WIDTH                   (     1)  /* LDO8_SWI */
                                            
/*                                                                                                   
 * R16510 (0x407E) - LDO8 ON Control                                             
 */                                          
#define WM8325_LDO8_ON_SLOT_MASK                (0xE000)  /* LDO8_ON_SLOT - [15:13] */
#define WM8325_LDO8_ON_SLOT_SHIFT               (    13)  /* LDO8_ON_SLOT - [15:13] */
#define WM8325_LDO8_ON_SLOT_WIDTH               (     3)  /* LDO8_ON_SLOT - [15:13] */
#define WM8325_LDO8_ON_MODE                     (0x0100)  /* LDO8_ON_MODE */
#define WM8325_LDO8_ON_MODE_MASK                (0x0100)  /* LDO8_ON_MODE */
#define WM8325_LDO8_ON_MODE_SHIFT               (     8)  /* LDO8_ON_MODE */
#define WM8325_LDO8_ON_MODE_WIDTH               (     1)  /* LDO8_ON_MODE */
#define WM8325_LDO8_ON_VSEL_MASK                (0x001F)  /* LDO8_ON_VSEL - [4:0] */
#define WM8325_LDO8_ON_VSEL_SHIFT               (     0)  /* LDO8_ON_VSEL - [4:0] */
#define WM8325_LDO8_ON_VSEL_WIDTH               (     5)  /* LDO8_ON_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R16511 (0x407F) - LDO8 SLEEP Control                                         
 */                                          
#define WM8325_LDO8_SLP_SLOT_MASK               (0xE000)  /* LDO8_SLP_SLOT - [15:13] */
#define WM8325_LDO8_SLP_SLOT_SHIFT              (    13)  /* LDO8_SLP_SLOT - [15:13] */
#define WM8325_LDO8_SLP_SLOT_WIDTH              (     3)  /* LDO8_SLP_SLOT - [15:13] */
#define WM8325_LDO8_SLP_MODE                    (0x0100)  /* LDO8_SLP_MODE */
#define WM8325_LDO8_SLP_MODE_MASK               (0x0100)  /* LDO8_SLP_MODE */
#define WM8325_LDO8_SLP_MODE_SHIFT              (     8)  /* LDO8_SLP_MODE */
#define WM8325_LDO8_SLP_MODE_WIDTH              (     1)  /* LDO8_SLP_MODE */
#define WM8325_LDO8_SLP_VSEL_MASK               (0x001F)  /* LDO8_SLP_VSEL - [4:0] */
#define WM8325_LDO8_SLP_VSEL_SHIFT              (     0)  /* LDO8_SLP_VSEL - [4:0] */
#define WM8325_LDO8_SLP_VSEL_WIDTH              (     5)  /* LDO8_SLP_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16512 (0x4080) - LDO9 Control                                                     
 */                                           
#define WM8325_LDO9_ERR_ACT_MASK                (0xC000)  /* LDO9_ERR_ACT - [15:14] */
#define WM8325_LDO9_ERR_ACT_SHIFT               (    14)  /* LDO9_ERR_ACT - [15:14] */
#define WM8325_LDO9_ERR_ACT_WIDTH               (     2)  /* LDO9_ERR_ACT - [15:14] */
#define WM8325_LDO9_HWC_SRC_MASK                (0x1800)  /* LDO9_HWC_SRC - [12:11] */
#define WM8325_LDO9_HWC_SRC_SHIFT               (    11)  /* LDO9_HWC_SRC - [12:11] */
#define WM8325_LDO9_HWC_SRC_WIDTH               (     2)  /* LDO9_HWC_SRC - [12:11] */
#define WM8325_LDO9_HWC_VSEL                    (0x0400)  /* LDO9_HWC_VSEL */
#define WM8325_LDO9_HWC_VSEL_MASK               (0x0400)  /* LDO9_HWC_VSEL */
#define WM8325_LDO9_HWC_VSEL_SHIFT              (    10)  /* LDO9_HWC_VSEL */
#define WM8325_LDO9_HWC_VSEL_WIDTH              (     1)  /* LDO9_HWC_VSEL */
#define WM8325_LDO9_HWC_MODE_MASK               (0x0300)  /* LDO9_HWC_MODE - [9:8] */
#define WM8325_LDO9_HWC_MODE_SHIFT              (     8)  /* LDO9_HWC_MODE - [9:8] */
#define WM8325_LDO9_HWC_MODE_WIDTH              (     2)  /* LDO9_HWC_MODE - [9:8] */
#define WM8325_LDO9_FLT                         (0x0080)  /* LDO9_FLT */
#define WM8325_LDO9_FLT_MASK                    (0x0080)  /* LDO9_FLT */
#define WM8325_LDO9_FLT_SHIFT                   (     7)  /* LDO9_FLT */
#define WM8325_LDO9_FLT_WIDTH                   (     1)  /* LDO9_FLT */
#define WM8325_LDO9_SWI                         (0x0040)  /* LDO9_SWI */
#define WM8325_LDO9_SWI_MASK                    (0x0040)  /* LDO9_SWI */
#define WM8325_LDO9_SWI_SHIFT                   (     6)  /* LDO9_SWI */
#define WM8325_LDO9_SWI_WIDTH                   (     1)  /* LDO9_SWI */
                                            
/*                                                                                                   
 * R16513 (0x4081) - LDO9 ON Control                                             
 */                                          
#define WM8325_LDO9_ON_SLOT_MASK                (0xE000)  /* LDO9_ON_SLOT - [15:13] */
#define WM8325_LDO9_ON_SLOT_SHIFT               (    13)  /* LDO9_ON_SLOT - [15:13] */
#define WM8325_LDO9_ON_SLOT_WIDTH               (     3)  /* LDO9_ON_SLOT - [15:13] */
#define WM8325_LDO9_ON_MODE                     (0x0100)  /* LDO9_ON_MODE */
#define WM8325_LDO9_ON_MODE_MASK                (0x0100)  /* LDO9_ON_MODE */
#define WM8325_LDO9_ON_MODE_SHIFT               (     8)  /* LDO9_ON_MODE */
#define WM8325_LDO9_ON_MODE_WIDTH               (     1)  /* LDO9_ON_MODE */
#define WM8325_LDO9_ON_VSEL_MASK                (0x001F)  /* LDO9_ON_VSEL - [4:0] */
#define WM8325_LDO9_ON_VSEL_SHIFT               (     0)  /* LDO9_ON_VSEL - [4:0] */
#define WM8325_LDO9_ON_VSEL_WIDTH               (     5)  /* LDO9_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16514 (0x4082) - LDO9 SLEEP Control                                          
 */                                           
#define WM8325_LDO9_SLP_SLOT_MASK               (0xE000)  /* LDO9_SLP_SLOT - [15:13] */
#define WM8325_LDO9_SLP_SLOT_SHIFT              (    13)  /* LDO9_SLP_SLOT - [15:13] */
#define WM8325_LDO9_SLP_SLOT_WIDTH              (     3)  /* LDO9_SLP_SLOT - [15:13] */
#define WM8325_LDO9_SLP_MODE                    (0x0100)  /* LDO9_SLP_MODE */
#define WM8325_LDO9_SLP_MODE_MASK               (0x0100)  /* LDO9_SLP_MODE */
#define WM8325_LDO9_SLP_MODE_SHIFT              (     8)  /* LDO9_SLP_MODE */
#define WM8325_LDO9_SLP_MODE_WIDTH              (     1)  /* LDO9_SLP_MODE */
#define WM8325_LDO9_SLP_VSEL_MASK               (0x001F)  /* LDO9_SLP_VSEL - [4:0] */
#define WM8325_LDO9_SLP_VSEL_SHIFT              (     0)  /* LDO9_SLP_VSEL - [4:0] */
#define WM8325_LDO9_SLP_VSEL_WIDTH              (     5)  /* LDO9_SLP_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16515 (0x4083) - LDO10 Control                                                   
 */                                           
#define WM8325_LDO10_ERR_ACT_MASK               (0xC000)  /* LDO10_ERR_ACT - [15:14] */
#define WM8325_LDO10_ERR_ACT_SHIFT              (    14)  /* LDO10_ERR_ACT - [15:14] */
#define WM8325_LDO10_ERR_ACT_WIDTH              (     2)  /* LDO10_ERR_ACT - [15:14] */
#define WM8325_LDO10_HWC_SRC_MASK               (0x1800)  /* LDO10_HWC_SRC - [12:11] */
#define WM8325_LDO10_HWC_SRC_SHIFT              (    11)  /* LDO10_HWC_SRC - [12:11] */
#define WM8325_LDO10_HWC_SRC_WIDTH              (     2)  /* LDO10_HWC_SRC - [12:11] */
#define WM8325_LDO10_HWC_VSEL                   (0x0400)  /* LDO10_HWC_VSEL */
#define WM8325_LDO10_HWC_VSEL_MASK              (0x0400)  /* LDO10_HWC_VSEL */
#define WM8325_LDO10_HWC_VSEL_SHIFT             (    10)  /* LDO10_HWC_VSEL */
#define WM8325_LDO10_HWC_VSEL_WIDTH             (     1)  /* LDO10_HWC_VSEL */
#define WM8325_LDO10_HWC_MODE_MASK              (0x0300)  /* LDO10_HWC_MODE - [9:8] */
#define WM8325_LDO10_HWC_MODE_SHIFT             (     8)  /* LDO10_HWC_MODE - [9:8] */
#define WM8325_LDO10_HWC_MODE_WIDTH             (     2)  /* LDO10_HWC_MODE - [9:8] */
#define WM8325_LDO10_FLT                        (0x0080)  /* LDO10_FLT */
#define WM8325_LDO10_FLT_MASK                   (0x0080)  /* LDO10_FLT */
#define WM8325_LDO10_FLT_SHIFT                  (     7)  /* LDO10_FLT */
#define WM8325_LDO10_FLT_WIDTH                  (     1)  /* LDO10_FLT */
#define WM8325_LDO10_SWI                        (0x0040)  /* LDO10_SWI */
#define WM8325_LDO10_SWI_MASK                   (0x0040)  /* LDO10_SWI */
#define WM8325_LDO10_SWI_SHIFT                  (     6)  /* LDO10_SWI */
#define WM8325_LDO10_SWI_WIDTH                  (     1)  /* LDO10_SWI */
                                            
/*                                                                                                   
 * R16516 (0x4084) - LDO10 ON Control                                           
 */                                          
#define WM8325_LDO10_ON_SLOT_MASK               (0xE000)  /* LDO10_ON_SLOT - [15:13] */
#define WM8325_LDO10_ON_SLOT_SHIFT              (    13)  /* LDO10_ON_SLOT - [15:13] */
#define WM8325_LDO10_ON_SLOT_WIDTH              (     3)  /* LDO10_ON_SLOT - [15:13] */
#define WM8325_LDO10_ON_MODE                    (0x0100)  /* LDO10_ON_MODE */
#define WM8325_LDO10_ON_MODE_MASK               (0x0100)  /* LDO10_ON_MODE */
#define WM8325_LDO10_ON_MODE_SHIFT              (     8)  /* LDO10_ON_MODE */
#define WM8325_LDO10_ON_MODE_WIDTH              (     1)  /* LDO10_ON_MODE */
#define WM8325_LDO10_ON_VSEL_MASK               (0x001F)  /* LDO10_ON_VSEL - [4:0] */
#define WM8325_LDO10_ON_VSEL_SHIFT              (     0)  /* LDO10_ON_VSEL - [4:0] */
#define WM8325_LDO10_ON_VSEL_WIDTH              (     5)  /* LDO10_ON_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R16517 (0x4085) - LDO10 SLEEP Control                                      
 */                                          
#define WM8325_LDO10_SLP_SLOT_MASK              (0xE000)  /* LDO10_SLP_SLOT - [15:13] */
#define WM8325_LDO10_SLP_SLOT_SHIFT             (    13)  /* LDO10_SLP_SLOT - [15:13] */
#define WM8325_LDO10_SLP_SLOT_WIDTH             (     3)  /* LDO10_SLP_SLOT - [15:13] */
#define WM8325_LDO10_SLP_MODE                   (0x0100)  /* LDO10_SLP_MODE */
#define WM8325_LDO10_SLP_MODE_MASK              (0x0100)  /* LDO10_SLP_MODE */
#define WM8325_LDO10_SLP_MODE_SHIFT             (     8)  /* LDO10_SLP_MODE */
#define WM8325_LDO10_SLP_MODE_WIDTH             (     1)  /* LDO10_SLP_MODE */
#define WM8325_LDO10_SLP_VSEL_MASK              (0x001F)  /* LDO10_SLP_VSEL - [4:0] */
#define WM8325_LDO10_SLP_VSEL_SHIFT             (     0)  /* LDO10_SLP_VSEL - [4:0] */
#define WM8325_LDO10_SLP_VSEL_WIDTH             (     5)  /* LDO10_SLP_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R16519 (0x4087) - LDO11 ON Control                                              
 */                                           
#define WM8325_LDO11_ON_SLOT_MASK               (0xE000)  /* LDO11_ON_SLOT - [15:13] */
#define WM8325_LDO11_ON_SLOT_SHIFT              (    13)  /* LDO11_ON_SLOT - [15:13] */
#define WM8325_LDO11_ON_SLOT_WIDTH              (     3)  /* LDO11_ON_SLOT - [15:13] */
#define WM8325_LDO11_FRCENA                     (0x1000)  /* LDO11_FRCENA */
#define WM8325_LDO11_FRCENA_MASK                (0x1000)  /* LDO11_FRCENA */
#define WM8325_LDO11_FRCENA_SHIFT               (    12)  /* LDO11_FRCENA */
#define WM8325_LDO11_FRCENA_WIDTH               (     1)  /* LDO11_FRCENA */
#define WM8325_LDO11_VSEL_SRC                   (0x0080)  /* LDO11_VSEL_SRC */
#define WM8325_LDO11_VSEL_SRC_MASK              (0x0080)  /* LDO11_VSEL_SRC */
#define WM8325_LDO11_VSEL_SRC_SHIFT             (     7)  /* LDO11_VSEL_SRC */
#define WM8325_LDO11_VSEL_SRC_WIDTH             (     1)  /* LDO11_VSEL_SRC */
#define WM8325_LDO11_ON_VSEL_MASK               (0x000F)  /* LDO11_ON_VSEL - [3:0] */
#define WM8325_LDO11_ON_VSEL_SHIFT              (     0)  /* LDO11_ON_VSEL - [3:0] */
#define WM8325_LDO11_ON_VSEL_WIDTH              (     4)  /* LDO11_ON_VSEL - [3:0] */
                                              
/*                                                                                                        
 * R16520 (0x4088) - LDO11 SLEEP Control                                           
 */                                            
#define WM8325_LDO11_SLP_SLOT_MASK              (0xE000)  /* LDO11_SLP_SLOT - [15:13] */
#define WM8325_LDO11_SLP_SLOT_SHIFT             (    13)  /* LDO11_SLP_SLOT - [15:13] */
#define WM8325_LDO11_SLP_SLOT_WIDTH             (     3)  /* LDO11_SLP_SLOT - [15:13] */
#define WM8325_LDO11_SLP_VSEL_MASK              (0x000F)  /* LDO11_SLP_VSEL - [3:0] */
#define WM8325_LDO11_SLP_VSEL_SHIFT             (     0)  /* LDO11_SLP_VSEL - [3:0] */
#define WM8325_LDO11_SLP_VSEL_WIDTH             (     4)  /* LDO11_SLP_VSEL - [3:0] */
                                              
/*                                                                                                        
 * R16521 (0x4089) - EPE1 Control                                                        
 */                                            
#define WM8325_EPE1_ON_SLOT_MASK                (0xE000)  /* EPE1_ON_SLOT - [15:13] */
#define WM8325_EPE1_ON_SLOT_SHIFT               (    13)  /* EPE1_ON_SLOT - [15:13] */
#define WM8325_EPE1_ON_SLOT_WIDTH               (     3)  /* EPE1_ON_SLOT - [15:13] */
#define WM8325_EPE1_HWC_SRC_MASK                (0x1800)  /* EPE1_HWC_SRC - [12:11] */
#define WM8325_EPE1_HWC_SRC_SHIFT               (    11)  /* EPE1_HWC_SRC - [12:11] */
#define WM8325_EPE1_HWC_SRC_WIDTH               (     2)  /* EPE1_HWC_SRC - [12:11] */
#define WM8325_EPE1_HWCENA                      (0x0100)  /* EPE1_HWCENA */
#define WM8325_EPE1_HWCENA_MASK                 (0x0100)  /* EPE1_HWCENA */
#define WM8325_EPE1_HWCENA_SHIFT                (     8)  /* EPE1_HWCENA */
#define WM8325_EPE1_HWCENA_WIDTH                (     1)  /* EPE1_HWCENA */
#define WM8325_EPE1_SLP_SLOT_MASK               (0x00E0)  /* EPE1_SLP_SLOT - [7:5] */
#define WM8325_EPE1_SLP_SLOT_SHIFT              (     5)  /* EPE1_SLP_SLOT - [7:5] */
#define WM8325_EPE1_SLP_SLOT_WIDTH              (     3)  /* EPE1_SLP_SLOT - [7:5] */
                                            
/*                                                                                                   
 * R16522 (0x408A) - EPE2 Control                                                   
 */                                          
#define WM8325_EPE2_ON_SLOT_MASK                (0xE000)  /* EPE2_ON_SLOT - [15:13] */
#define WM8325_EPE2_ON_SLOT_SHIFT               (    13)  /* EPE2_ON_SLOT - [15:13] */
#define WM8325_EPE2_ON_SLOT_WIDTH               (     3)  /* EPE2_ON_SLOT - [15:13] */
#define WM8325_EPE2_HWC_SRC_MASK                (0x1800)  /* EPE2_HWC_SRC - [12:11] */
#define WM8325_EPE2_HWC_SRC_SHIFT               (    11)  /* EPE2_HWC_SRC - [12:11] */
#define WM8325_EPE2_HWC_SRC_WIDTH               (     2)  /* EPE2_HWC_SRC - [12:11] */
#define WM8325_EPE2_HWCENA                      (0x0100)  /* EPE2_HWCENA */
#define WM8325_EPE2_HWCENA_MASK                 (0x0100)  /* EPE2_HWCENA */
#define WM8325_EPE2_HWCENA_SHIFT                (     8)  /* EPE2_HWCENA */
#define WM8325_EPE2_HWCENA_WIDTH                (     1)  /* EPE2_HWCENA */
#define WM8325_EPE2_SLP_SLOT_MASK               (0x00E0)  /* EPE2_SLP_SLOT - [7:5] */
#define WM8325_EPE2_SLP_SLOT_SHIFT              (     5)  /* EPE2_SLP_SLOT - [7:5] */
#define WM8325_EPE2_SLP_SLOT_WIDTH              (     3)  /* EPE2_SLP_SLOT - [7:5] */
                                           
/*                                                                                                 
 * R16526 (0x408E) - Power Good Source 1                                     
 */                                         
#define WM8325_DC4_OK                           (0x0008)  /* DC4_OK */
#define WM8325_DC4_OK_MASK                      (0x0008)  /* DC4_OK */
#define WM8325_DC4_OK_SHIFT                     (     3)  /* DC4_OK */
#define WM8325_DC4_OK_WIDTH                     (     1)  /* DC4_OK */
#define WM8325_DC3_OK                           (0x0004)  /* DC3_OK */
#define WM8325_DC3_OK_MASK                      (0x0004)  /* DC3_OK */
#define WM8325_DC3_OK_SHIFT                     (     2)  /* DC3_OK */
#define WM8325_DC3_OK_WIDTH                     (     1)  /* DC3_OK */
#define WM8325_DC2_OK                           (0x0002)  /* DC2_OK */
#define WM8325_DC2_OK_MASK                      (0x0002)  /* DC2_OK */
#define WM8325_DC2_OK_SHIFT                     (     1)  /* DC2_OK */
#define WM8325_DC2_OK_WIDTH                     (     1)  /* DC2_OK */
#define WM8325_DC1_OK                           (0x0001)  /* DC1_OK */
#define WM8325_DC1_OK_MASK                      (0x0001)  /* DC1_OK */
#define WM8325_DC1_OK_SHIFT                     (     0)  /* DC1_OK */
#define WM8325_DC1_OK_WIDTH                     (     1)  /* DC1_OK */
                                              
/*                                                                                                        
 * R16527 (0x408F) - Power Good Source 2                                            
 */                                            
#define WM8325_LDO10_OK                         (0x0200)  /* LDO10_OK */
#define WM8325_LDO10_OK_MASK                    (0x0200)  /* LDO10_OK */
#define WM8325_LDO10_OK_SHIFT                   (     9)  /* LDO10_OK */
#define WM8325_LDO10_OK_WIDTH                   (     1)  /* LDO10_OK */
#define WM8325_LDO9_OK                          (0x0100)  /* LDO9_OK */
#define WM8325_LDO9_OK_MASK                     (0x0100)  /* LDO9_OK */
#define WM8325_LDO9_OK_SHIFT                    (     8)  /* LDO9_OK */
#define WM8325_LDO9_OK_WIDTH                    (     1)  /* LDO9_OK */
#define WM8325_LDO8_OK                          (0x0080)  /* LDO8_OK */
#define WM8325_LDO8_OK_MASK                     (0x0080)  /* LDO8_OK */
#define WM8325_LDO8_OK_SHIFT                    (     7)  /* LDO8_OK */
#define WM8325_LDO8_OK_WIDTH                    (     1)  /* LDO8_OK */
#define WM8325_LDO7_OK                          (0x0040)  /* LDO7_OK */
#define WM8325_LDO7_OK_MASK                     (0x0040)  /* LDO7_OK */
#define WM8325_LDO7_OK_SHIFT                    (     6)  /* LDO7_OK */
#define WM8325_LDO7_OK_WIDTH                    (     1)  /* LDO7_OK */
#define WM8325_LDO6_OK                          (0x0020)  /* LDO6_OK */
#define WM8325_LDO6_OK_MASK                     (0x0020)  /* LDO6_OK */
#define WM8325_LDO6_OK_SHIFT                    (     5)  /* LDO6_OK */
#define WM8325_LDO6_OK_WIDTH                    (     1)  /* LDO6_OK */
#define WM8325_LDO5_OK                          (0x0010)  /* LDO5_OK */
#define WM8325_LDO5_OK_MASK                     (0x0010)  /* LDO5_OK */
#define WM8325_LDO5_OK_SHIFT                    (     4)  /* LDO5_OK */
#define WM8325_LDO5_OK_WIDTH                    (     1)  /* LDO5_OK */
#define WM8325_LDO4_OK                          (0x0008)  /* LDO4_OK */
#define WM8325_LDO4_OK_MASK                     (0x0008)  /* LDO4_OK */
#define WM8325_LDO4_OK_SHIFT                    (     3)  /* LDO4_OK */
#define WM8325_LDO4_OK_WIDTH                    (     1)  /* LDO4_OK */
#define WM8325_LDO3_OK                          (0x0004)  /* LDO3_OK */
#define WM8325_LDO3_OK_MASK                     (0x0004)  /* LDO3_OK */
#define WM8325_LDO3_OK_SHIFT                    (     2)  /* LDO3_OK */
#define WM8325_LDO3_OK_WIDTH                    (     1)  /* LDO3_OK */
#define WM8325_LDO2_OK                          (0x0002)  /* LDO2_OK */
#define WM8325_LDO2_OK_MASK                     (0x0002)  /* LDO2_OK */
#define WM8325_LDO2_OK_SHIFT                    (     1)  /* LDO2_OK */
#define WM8325_LDO2_OK_WIDTH                    (     1)  /* LDO2_OK */
#define WM8325_LDO1_OK                          (0x0001)  /* LDO1_OK */
#define WM8325_LDO1_OK_MASK                     (0x0001)  /* LDO1_OK */
#define WM8325_LDO1_OK_SHIFT                    (     0)  /* LDO1_OK */
#define WM8325_LDO1_OK_WIDTH                    (     1)  /* LDO1_OK */
                                           
/*                                                                                                 
 * R16528 (0x4090) - Clock Control 1                                              
 */                                         
#define WM8325_CLKOUT_ENA                       (0x8000)  /* CLKOUT_ENA */
#define WM8325_CLKOUT_ENA_MASK                  (0x8000)  /* CLKOUT_ENA */
#define WM8325_CLKOUT_ENA_SHIFT                 (    15)  /* CLKOUT_ENA */
#define WM8325_CLKOUT_ENA_WIDTH                 (     1)  /* CLKOUT_ENA */
#define WM8325_CLKOUT_OD                        (0x2000)  /* CLKOUT_OD */
#define WM8325_CLKOUT_OD_MASK                   (0x2000)  /* CLKOUT_OD */
#define WM8325_CLKOUT_OD_SHIFT                  (    13)  /* CLKOUT_OD */
#define WM8325_CLKOUT_OD_WIDTH                  (     1)  /* CLKOUT_OD */
#define WM8325_CLKOUT_SLOT_MASK                 (0x0700)  /* CLKOUT_SLOT - [10:8] */
#define WM8325_CLKOUT_SLOT_SHIFT                (     8)  /* CLKOUT_SLOT - [10:8] */
#define WM8325_CLKOUT_SLOT_WIDTH                (     3)  /* CLKOUT_SLOT - [10:8] */
#define WM8325_CLKOUT_SLPSLOT_MASK              (0x0070)  /* CLKOUT_SLPSLOT - [6:4] */
#define WM8325_CLKOUT_SLPSLOT_SHIFT             (     4)  /* CLKOUT_SLPSLOT - [6:4] */
#define WM8325_CLKOUT_SLPSLOT_WIDTH             (     3)  /* CLKOUT_SLPSLOT - [6:4] */
                                              
/*                                                                                                        
 * R16529 (0x4091) - Clock Control 2                                                     
 */                                            
#define WM8325_XTAL_INH                         (0x8000)  /* XTAL_INH */
#define WM8325_XTAL_INH_MASK                    (0x8000)  /* XTAL_INH */
#define WM8325_XTAL_INH_SHIFT                   (    15)  /* XTAL_INH */
#define WM8325_XTAL_INH_WIDTH                   (     1)  /* XTAL_INH */
#define WM8325_XTAL_ENA                         (0x2000)  /* XTAL_ENA */
#define WM8325_XTAL_ENA_MASK                    (0x2000)  /* XTAL_ENA */
#define WM8325_XTAL_ENA_SHIFT                   (    13)  /* XTAL_ENA */
#define WM8325_XTAL_ENA_WIDTH                   (     1)  /* XTAL_ENA */
#define WM8325_XTAL_BKUPENA                     (0x1000)  /* XTAL_BKUPENA */
#define WM8325_XTAL_BKUPENA_MASK                (0x1000)  /* XTAL_BKUPENA */
#define WM8325_XTAL_BKUPENA_SHIFT               (    12)  /* XTAL_BKUPENA */
#define WM8325_XTAL_BKUPENA_WIDTH               (     1)  /* XTAL_BKUPENA */
                                            
/*                                                                                                   
 * R30720 (0x7800) - Unique ID 1                                                   
 */                                          
#define WM8325_UNIQUE_ID_7_MASK                 (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_7_SHIFT                (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_7_WIDTH                (    16)  /* UNIQUE_ID - [15:0] */
                                            
/*                                                                                                   
 * R30721 (0x7801) - Unique ID 2                                                    
 */                                          
#define WM8325_UNIQUE_ID_6_MASK                 (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_6_SHIFT                (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_6_WIDTH                (    16)  /* UNIQUE_ID - [15:0] */
                                             
/*                                                                                                      
 * R30722 (0x7802) - Unique ID 3                                                     
 */                                           
#define WM8325_UNIQUE_ID_5_MASK                 (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_5_SHIFT                (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_5_WIDTH                (    16)  /* UNIQUE_ID - [15:0] */
                                           
/*                                                                                                 
 * R30723 (0x7803) - Unique ID 4                                                 
 */                                         
#define WM8325_UNIQUE_ID_4_MASK                 (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_4_SHIFT                (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_4_WIDTH                (    16)  /* UNIQUE_ID - [15:0] */
                                              
/*                                                                                                        
 * R30724 (0x7804) - Unique ID 5                                                        
 */                                            
#define WM8325_UNIQUE_ID_3_MASK                 (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_3_SHIFT                (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_3_WIDTH                (    16)  /* UNIQUE_ID - [15:0] */
                                               
/*                                                                                                          
 * R30725 (0x7805) - Unique ID 6                                                          
 */                                             
#define WM8325_UNIQUE_ID_2_MASK                 (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_2_SHIFT                (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_2_WIDTH                (    16)  /* UNIQUE_ID - [15:0] */
                                              
/*                                                                                                        
 * R30726 (0x7806) - Unique ID 7                                                        
 */                                            
#define WM8325_UNIQUE_ID_1_MASK                 (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_1_SHIFT                (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_1_WIDTH                (    16)  /* UNIQUE_ID - [15:0] */
                                               
/*                                                                                                          
 * R30727 (0x7807) - Unique ID 8                                                          
 */                                             
#define WM8325_UNIQUE_ID_MASK                   (0xFFFF)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_SHIFT                  (     0)  /* UNIQUE_ID - [15:0] */
#define WM8325_UNIQUE_ID_WIDTH                  (    16)  /* UNIQUE_ID - [15:0] */
                                              
/*                                                                                                        
 * R30736 (0x7810) - Customer OTP ID                                                 
 */                                            
#define WM8325_OTP_AUTO_PROG                    (0x8000)  /* OTP_AUTO_PROG */
#define WM8325_OTP_AUTO_PROG_MASK               (0x8000)  /* OTP_AUTO_PROG */
#define WM8325_OTP_AUTO_PROG_SHIFT              (    15)  /* OTP_AUTO_PROG */
#define WM8325_OTP_AUTO_PROG_WIDTH              (     1)  /* OTP_AUTO_PROG */
#define WM8325_OTP_CUST_ID_MASK                 (0x7FFE)  /* OTP_CUST_ID - [14:1] */
#define WM8325_OTP_CUST_ID_SHIFT                (     1)  /* OTP_CUST_ID - [14:1] */
#define WM8325_OTP_CUST_ID_WIDTH                (    14)  /* OTP_CUST_ID - [14:1] */
#define WM8325_OTP_CUST_FINAL                   (0x0001)  /* OTP_CUST_FINAL */
#define WM8325_OTP_CUST_FINAL_MASK              (0x0001)  /* OTP_CUST_FINAL */
#define WM8325_OTP_CUST_FINAL_SHIFT             (     0)  /* OTP_CUST_FINAL */
#define WM8325_OTP_CUST_FINAL_WIDTH             (     1)  /* OTP_CUST_FINAL */
                                              
/*                                                                                                        
 * R30737 (0x7811) - DC1 OTP Control                                                   
 */                                            
#define WM8325_DC2_OTP_SLOT_MASK                (0xE000)  /* DC2_ON_SLOT - [15:13] */
#define WM8325_DC2_OTP_SLOT_SHIFT               (    13)  /* DC2_ON_SLOT - [15:13] */
#define WM8325_DC2_OTP_SLOT_WIDTH               (     3)  /* DC2_ON_SLOT - [15:13] */
#define WM8325_DC2_OTP_VSEL_0_MASK              (0x1F00)  /* DC2_ON_VSEL - [12:8] */
#define WM8325_DC2_OTP_VSEL_0_SHIFT             (     8)  /* DC2_ON_VSEL - [12:8] */
#define WM8325_DC2_OTP_VSEL_0_WIDTH             (     5)  /* DC2_ON_VSEL - [12:8] */
#define WM8325_DC1_OTP_SLOT_MASK                (0x00E0)  /* DC1_ON_SLOT - [7:5] */
#define WM8325_DC1_OTP_SLOT_SHIFT               (     5)  /* DC1_ON_SLOT - [7:5] */
#define WM8325_DC1_OTP_SLOT_WIDTH               (     3)  /* DC1_ON_SLOT - [7:5] */
#define WM8325_DC1_OTP_VSEL_0_MASK              (0x001F)  /* DC1_ON_VSEL - [4:0] */
#define WM8325_DC1_OTP_VSEL_0_SHIFT             (     0)  /* DC1_ON_VSEL - [4:0] */
#define WM8325_DC1_OTP_VSEL_0_WIDTH             (     5)  /* DC1_ON_VSEL - [4:0] */
                                              
/*                                                                                                        
 * R30738 (0x7812) - DC2 OTP Control                                                  
 */                                            
#define WM8325_DC4_OTP_SLOT_MASK                (0xE000)  /* DC4_ON_SLOT - [15:13] */
#define WM8325_DC4_OTP_SLOT_SHIFT               (    13)  /* DC4_ON_SLOT - [15:13] */
#define WM8325_DC4_OTP_SLOT_WIDTH               (     3)  /* DC4_ON_SLOT - [15:13] */
#define WM8325_DC4_OTP_VSEL_0_MASK              (0x1F00)  /* DC4_ON_VSEL - [12:8] */
#define WM8325_DC4_OTP_VSEL_0_SHIFT             (     8)  /* DC4_ON_VSEL - [12:8] */
#define WM8325_DC4_OTP_VSEL_0_WIDTH             (     5)  /* DC4_ON_VSEL - [12:8] */
#define WM8325_DC3_OTP_SLOT_MASK                (0x00E0)  /* DC3_ON_SLOT - [7:5] */
#define WM8325_DC3_OTP_SLOT_SHIFT               (     5)  /* DC3_ON_SLOT - [7:5] */
#define WM8325_DC3_OTP_SLOT_WIDTH               (     3)  /* DC3_ON_SLOT - [7:5] */
#define WM8325_DC3_OTP_VSEL_0_MASK              (0x001F)  /* DC3_ON_VSEL - [4:0] */
#define WM8325_DC3_OTP_VSEL_0_SHIFT             (     0)  /* DC3_ON_VSEL - [4:0] */
#define WM8325_DC3_OTP_VSEL_0_WIDTH             (     5)  /* DC3_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R30739 (0x7813) - DC3 OTP Control                                                
 */                                           
#define WM8325_DC4_OTP_PHASE                    (0x8000)  /* DC4_PHASE */
#define WM8325_DC4_OTP_PHASE_MASK               (0x8000)  /* DC4_PHASE */
#define WM8325_DC4_OTP_PHASE_SHIFT              (    15)  /* DC4_PHASE */
#define WM8325_DC4_OTP_PHASE_WIDTH              (     1)  /* DC4_PHASE */
#define WM8325_DC3_OTP_PHASE                    (0x4000)  /* DC3_PHASE */
#define WM8325_DC3_OTP_PHASE_MASK               (0x4000)  /* DC3_PHASE */
#define WM8325_DC3_OTP_PHASE_SHIFT              (    14)  /* DC3_PHASE */
#define WM8325_DC3_OTP_PHASE_WIDTH              (     1)  /* DC3_PHASE */
#define WM8325_DC2_OTP_PHASE                    (0x2000)  /* DC2_PHASE */
#define WM8325_DC2_OTP_PHASE_MASK               (0x2000)  /* DC2_PHASE */
#define WM8325_DC2_OTP_PHASE_SHIFT              (    13)  /* DC2_PHASE */
#define WM8325_DC2_OTP_PHASE_WIDTH              (     1)  /* DC2_PHASE */
#define WM8325_DC1_OTP_PHASE                    (0x1000)  /* DC1_PHASE */
#define WM8325_DC1_OTP_PHASE_MASK               (0x1000)  /* DC1_PHASE */
#define WM8325_DC1_OTP_PHASE_SHIFT              (    12)  /* DC1_PHASE */
#define WM8325_DC1_OTP_PHASE_WIDTH              (     1)  /* DC1_PHASE */
#define WM8325_DC4_OTP_CAP_MASK                 (0x0C00)  /* DC4_CAP - [11:10] */
#define WM8325_DC4_OTP_CAP_SHIFT                (    10)  /* DC4_CAP - [11:10] */
#define WM8325_DC4_OTP_CAP_WIDTH                (     2)  /* DC4_CAP - [11:10] */
#define WM8325_DC3_OTP_CAP_MASK                 (0x0300)  /* DC3_CAP - [9:8] */
#define WM8325_DC3_OTP_CAP_SHIFT                (     8)  /* DC3_CAP - [9:8] */
#define WM8325_DC3_OTP_CAP_WIDTH                (     2)  /* DC3_CAP - [9:8] */
#define WM8325_DC2_OTP_CAP_MASK                 (0x00C0)  /* DC2_CAP - [7:6] */
#define WM8325_DC2_OTP_CAP_SHIFT                (     6)  /* DC2_CAP - [7:6] */
#define WM8325_DC2_OTP_CAP_WIDTH                (     2)  /* DC2_CAP - [7:6] */
#define WM8325_DC1_OTP_CAP_MASK                 (0x0030)  /* DC1_CAP - [5:4] */
#define WM8325_DC1_OTP_CAP_SHIFT                (     4)  /* DC1_CAP - [5:4] */
#define WM8325_DC1_OTP_CAP_WIDTH                (     2)  /* DC1_CAP - [5:4] */
#define WM8325_DC2_OTP_FREQ_MASK                (0x000C)  /* DC2_FREQ - [3:2] */
#define WM8325_DC2_OTP_FREQ_SHIFT               (     2)  /* DC2_FREQ - [3:2] */
#define WM8325_DC2_OTP_FREQ_WIDTH               (     2)  /* DC2_FREQ - [3:2] */
#define WM8325_DC1_OTP_FREQ_MASK                (0x0003)  /* DC1_FREQ - [1:0] */
#define WM8325_DC1_OTP_FREQ_SHIFT               (     0)  /* DC1_FREQ - [1:0] */
#define WM8325_DC1_OTP_FREQ_WIDTH               (     2)  /* DC1_FREQ - [1:0] */
                                              
/*                                                                                                        
 * R30740 (0x7814) - LDO1/2 OTP Control                                            
 */                                            
#define WM8325_LDO2_OTP_SLOT_MASK               (0xE000)  /* LDO2_ON_SLOT - [15:13] */
#define WM8325_LDO2_OTP_SLOT_SHIFT              (    13)  /* LDO2_ON_SLOT - [15:13] */
#define WM8325_LDO2_OTP_SLOT_WIDTH              (     3)  /* LDO2_ON_SLOT - [15:13] */
#define WM8325_LDO2_OTP_VSEL_MASK               (0x1F00)  /* LDO2_ON_VSEL - [12:8] */
#define WM8325_LDO2_OTP_VSEL_SHIFT              (     8)  /* LDO2_ON_VSEL - [12:8] */
#define WM8325_LDO2_OTP_VSEL_WIDTH              (     5)  /* LDO2_ON_VSEL - [12:8] */
#define WM8325_LDO1_OTP_SLOT_MASK               (0x00E0)  /* LDO1_ON_SLOT - [7:5] */
#define WM8325_LDO1_OTP_SLOT_SHIFT              (     5)  /* LDO1_ON_SLOT - [7:5] */
#define WM8325_LDO1_OTP_SLOT_WIDTH              (     3)  /* LDO1_ON_SLOT - [7:5] */
#define WM8325_LDO1_OTP_VSEL_MASK               (0x001F)  /* LDO1_ON_VSEL - [4:0] */
#define WM8325_LDO1_OTP_VSEL_SHIFT              (     0)  /* LDO1_ON_VSEL - [4:0] */
#define WM8325_LDO1_OTP_VSEL_WIDTH              (     5)  /* LDO1_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R30741 (0x7815) - LDO3/4 OTP Control                                          
 */                                           
#define WM8325_LDO4_OTP_SLOT_MASK               (0xE000)  /* LDO4_ON_SLOT - [15:13] */
#define WM8325_LDO4_OTP_SLOT_SHIFT              (    13)  /* LDO4_ON_SLOT - [15:13] */
#define WM8325_LDO4_OTP_SLOT_WIDTH              (     3)  /* LDO4_ON_SLOT - [15:13] */
#define WM8325_LDO4_OTP_VSEL_MASK               (0x1F00)  /* LDO4_ON_VSEL - [12:8] */
#define WM8325_LDO4_OTP_VSEL_SHIFT              (     8)  /* LDO4_ON_VSEL - [12:8] */
#define WM8325_LDO4_OTP_VSEL_WIDTH              (     5)  /* LDO4_ON_VSEL - [12:8] */
#define WM8325_LDO3_OTP_SLOT_MASK               (0x00E0)  /* LDO3_ON_SLOT - [7:5] */
#define WM8325_LDO3_OTP_SLOT_SHIFT              (     5)  /* LDO3_ON_SLOT - [7:5] */
#define WM8325_LDO3_OTP_SLOT_WIDTH              (     3)  /* LDO3_ON_SLOT - [7:5] */
#define WM8325_LDO3_OTP_VSEL_MASK               (0x001F)  /* LDO3_ON_VSEL - [4:0] */
#define WM8325_LDO3_OTP_VSEL_SHIFT              (     0)  /* LDO3_ON_VSEL - [4:0] */
#define WM8325_LDO3_OTP_VSEL_WIDTH              (     5)  /* LDO3_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R30742 (0x7816) - LDO5/6 OTP Control                                          
 */                                           
#define WM8325_LDO6_OTP_SLOT_MASK               (0xE000)  /* LDO6_ON_SLOT - [15:13] */
#define WM8325_LDO6_OTP_SLOT_SHIFT              (    13)  /* LDO6_ON_SLOT - [15:13] */
#define WM8325_LDO6_OTP_SLOT_WIDTH              (     3)  /* LDO6_ON_SLOT - [15:13] */
#define WM8325_LDO6_OTP_VSEL_MASK               (0x1F00)  /* LDO6_ON_VSEL - [12:8] */
#define WM8325_LDO6_OTP_VSEL_SHIFT              (     8)  /* LDO6_ON_VSEL - [12:8] */
#define WM8325_LDO6_OTP_VSEL_WIDTH              (     5)  /* LDO6_ON_VSEL - [12:8] */
#define WM8325_LDO5_OTP_SLOT_MASK               (0x00E0)  /* LDO5_ON_SLOT - [7:5] */
#define WM8325_LDO5_OTP_SLOT_SHIFT              (     5)  /* LDO5_ON_SLOT - [7:5] */
#define WM8325_LDO5_OTP_SLOT_WIDTH              (     3)  /* LDO5_ON_SLOT - [7:5] */
#define WM8325_LDO5_OTP_VSEL_MASK               (0x001F)  /* LDO5_ON_VSEL - [4:0] */
#define WM8325_LDO5_OTP_VSEL_SHIFT              (     0)  /* LDO5_ON_VSEL - [4:0] */
#define WM8325_LDO5_OTP_VSEL_WIDTH              (     5)  /* LDO5_ON_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R30743 (0x7817) - LDO7/8 OTP Control                                       
 */                                          
#define WM8325_LDO8_OTP_SLOT_MASK               (0xE000)  /* LDO8_ON_SLOT - [15:13] */
#define WM8325_LDO8_OTP_SLOT_SHIFT              (    13)  /* LDO8_ON_SLOT - [15:13] */
#define WM8325_LDO8_OTP_SLOT_WIDTH              (     3)  /* LDO8_ON_SLOT - [15:13] */
#define WM8325_LDO8_OTP_VSEL_MASK               (0x1F00)  /* LDO8_ON_VSEL - [12:8] */
#define WM8325_LDO8_OTP_VSEL_SHIFT              (     8)  /* LDO8_ON_VSEL - [12:8] */
#define WM8325_LDO8_OTP_VSEL_WIDTH              (     5)  /* LDO8_ON_VSEL - [12:8] */
#define WM8325_LDO7_OTP_SLOT_MASK               (0x00E0)  /* LDO7_ON_SLOT - [7:5] */
#define WM8325_LDO7_OTP_SLOT_SHIFT              (     5)  /* LDO7_ON_SLOT - [7:5] */
#define WM8325_LDO7_OTP_SLOT_WIDTH              (     3)  /* LDO7_ON_SLOT - [7:5] */
#define WM8325_LDO7_OTP_VSEL_MASK               (0x001F)  /* LDO7_ON_VSEL - [4:0] */
#define WM8325_LDO7_OTP_VSEL_SHIFT              (     0)  /* LDO7_ON_VSEL - [4:0] */
#define WM8325_LDO7_OTP_VSEL_WIDTH              (     5)  /* LDO7_ON_VSEL - [4:0] */
                                             
/*                                                                                                      
 * R30744 (0x7818) - LDO9/10 OTP Control                                        
 */                                           
#define WM8325_LDO10_OTP_SLOT_MASK              (0xE000)  /* LDO10_ON_SLOT - [15:13] */
#define WM8325_LDO10_OTP_SLOT_SHIFT             (    13)  /* LDO10_ON_SLOT - [15:13] */
#define WM8325_LDO10_OTP_SLOT_WIDTH             (     3)  /* LDO10_ON_SLOT - [15:13] */
#define WM8325_LDO10_OTP_VSEL_MASK              (0x1F00)  /* LDO10_ON_VSEL - [12:8] */
#define WM8325_LDO10_OTP_VSEL_SHIFT             (     8)  /* LDO10_ON_VSEL - [12:8] */
#define WM8325_LDO10_OTP_VSEL_WIDTH             (     5)  /* LDO10_ON_VSEL - [12:8] */
#define WM8325_LDO9_OTP_SLOT_MASK               (0x00E0)  /* LDO9_ON_SLOT - [7:5] */
#define WM8325_LDO9_OTP_SLOT_SHIFT              (     5)  /* LDO9_ON_SLOT - [7:5] */
#define WM8325_LDO9_OTP_SLOT_WIDTH              (     3)  /* LDO9_ON_SLOT - [7:5] */
#define WM8325_LDO9_OTP_VSEL_MASK               (0x001F)  /* LDO9_ON_VSEL - [4:0] */
#define WM8325_LDO9_OTP_VSEL_SHIFT              (     0)  /* LDO9_ON_VSEL - [4:0] */
#define WM8325_LDO9_OTP_VSEL_WIDTH              (     5)  /* LDO9_ON_VSEL - [4:0] */
                                            
/*                                                                                                   
 * R30745 (0x7819) - LDO11/EPE Control                                         
 */                                          
#define WM8325_LDO11_OTP_SLOT_MASK              (0xE000)  /* LDO11_ON_SLOT - [15:13] */
#define WM8325_LDO11_OTP_SLOT_SHIFT             (    13)  /* LDO11_ON_SLOT - [15:13] */
#define WM8325_LDO11_OTP_SLOT_WIDTH             (     3)  /* LDO11_ON_SLOT - [15:13] */
#define WM8325_LDO11_OTP_VSEL_MASK              (0x0F00)  /* LDO11_ON_VSEL - [11:8] */
#define WM8325_LDO11_OTP_VSEL_SHIFT             (     8)  /* LDO11_ON_VSEL - [11:8] */
#define WM8325_LDO11_OTP_VSEL_WIDTH             (     4)  /* LDO11_ON_VSEL - [11:8] */
#define WM8325_EPE2_OTP_SLOT_MASK               (0x00E0)  /* EPE2_ON_SLOT - [7:5] */
#define WM8325_EPE2_OTP_SLOT_SHIFT              (     5)  /* EPE2_ON_SLOT - [7:5] */
#define WM8325_EPE2_OTP_SLOT_WIDTH              (     3)  /* EPE2_ON_SLOT - [7:5] */
#define WM8325_EPE1_OTP_SLOT_MASK               (0x001C)  /* EPE1_ON_SLOT - [4:2] */
#define WM8325_EPE1_OTP_SLOT_SHIFT              ( 	  2)  /* EPE1_ON_SLOT - [4:2] */
#define WM8325_EPE1_OTP_SLOT_WIDTH              (  	  3)  /* EPE1_ON_SLOT - [4:2] */
#define WM8325_DC4_OTP_SLV                      (0x0001)  /* DC4_SLV */
#define WM8325_DC4_OTP_SLV_MASK                 (0x0001)  /* DC4_SLV */
#define WM8325_DC4_OTP_SLV_SHIFT                (     0)  /* DC4_SLV */
#define WM8325_DC4_OTP_SLV_WIDTH                (     1)  /* DC4_SLV */
                                            
/*                                                                                                   
 * R30746 (0x781A) - GPIO1 OTP Control                                          
 */                                          
#define WM8325_GP1_OTP_DIR                      (0x8000)  /* GP1_DIR */
#define WM8325_GP1_OTP_DIR_MASK                 (0x8000)  /* GP1_DIR */
#define WM8325_GP1_OTP_DIR_SHIFT                (    15)  /* GP1_DIR */
#define WM8325_GP1_OTP_DIR_WIDTH                (     1)  /* GP1_DIR */
#define WM8325_GP1_OTP_PULL_MASK                (0x6000)  /* GP1_PULL - [14:13] */
#define WM8325_GP1_OTP_PULL_SHIFT               (    13)  /* GP1_PULL - [14:13] */
#define WM8325_GP1_OTP_PULL_WIDTH               (     2)  /* GP1_PULL - [14:13] */
#define WM8325_GP1_OTP_INT_MODE                 (0x1000)  /* GP1_INT_MODE */
#define WM8325_GP1_OTP_INT_MODE_MASK            (0x1000)  /* GP1_INT_MODE */
#define WM8325_GP1_OTP_INT_MODE_SHIFT           (    12)  /* GP1_INT_MODE */
#define WM8325_GP1_OTP_INT_MODE_WIDTH           (     1)  /* GP1_INT_MODE */
#define WM8325_GP1_OTP_PWR_DOM                  (0x0800)  /* GP1_PWR_DOM */
#define WM8325_GP1_OTP_PWR_DOM_MASK             (0x0800)  /* GP1_PWR_DOM */
#define WM8325_GP1_OTP_PWR_DOM_SHIFT            (    11)  /* GP1_PWR_DOM */
#define WM8325_GP1_OTP_PWR_DOM_WIDTH            (     1)  /* GP1_PWR_DOM */
#define WM8325_GP1_OTP_POL                      (0x0400)  /* GP1_POL */
#define WM8325_GP1_OTP_POL_MASK                 (0x0400)  /* GP1_POL */
#define WM8325_GP1_OTP_POL_SHIFT                (    10)  /* GP1_POL */
#define WM8325_GP1_OTP_POL_WIDTH                (     1)  /* GP1_POL */
#define WM8325_GP1_OTP_OD                       (0x0200)  /* GP1_OD */
#define WM8325_GP1_OTP_OD_MASK                  (0x0200)  /* GP1_OD */
#define WM8325_GP1_OTP_OD_SHIFT                 (     9)  /* GP1_OD */
#define WM8325_GP1_OTP_OD_WIDTH                 (     1)  /* GP1_OD */
#define WM8325_GP1_OTP_ENA                      (0x0100)  /* GP1_ENA */
#define WM8325_GP1_OTP_ENA_MASK                 (0x0100)  /* GP1_ENA */
#define WM8325_GP1_OTP_ENA_SHIFT                (     8)  /* GP1_ENA */
#define WM8325_GP1_OTP_ENA_WIDTH                (     1)  /* GP1_ENA */
#define WM8325_GP1_OTP_FN_MASK                  (0x00F0)  /* GP1_FN - [7:4] */
#define WM8325_GP1_OTP_FN_SHIFT                 (     4)  /* GP1_FN - [7:4] */
#define WM8325_GP1_OTP_FN_WIDTH                 (     4)  /* GP1_FN - [7:4] */
#define WM8325_XTAL_OTP_ENA                     (0x0004)  /* XTAL_ENA */
#define WM8325_XTAL_OTP_ENA_MASK                (0x0004)  /* XTAL_ENA */
#define WM8325_XTAL_OTP_ENA_SHIFT               (     2)  /* XTAL_ENA */
#define WM8325_XTAL_OTP_ENA_WIDTH               (     1)  /* XTAL_ENA */
#define WM8325_XTAL_OTP_INH                     (0x0002)  /* XTAL_INH */
#define WM8325_XTAL_OTP_INH_MASK                (0x0002)  /* XTAL_INH */
#define WM8325_XTAL_OTP_INH_SHIFT               (     1)  /* XTAL_INH */
#define WM8325_XTAL_OTP_INH_WIDTH               (     1)  /* XTAL_INH */
                                               
/*                                                                                                          
 * R30747 (0x781B) - GPIO2 OTP Control                                                
 */                                             
#define WM8325_GP2_OTP_DIR                      (0x8000)  /* GP2_DIR */
#define WM8325_GP2_OTP_DIR_MASK                 (0x8000)  /* GP2_DIR */
#define WM8325_GP2_OTP_DIR_SHIFT                (    15)  /* GP2_DIR */
#define WM8325_GP2_OTP_DIR_WIDTH                (     1)  /* GP2_DIR */
#define WM8325_GP2_OTP_PULL_MASK                (0x6000)  /* GP2_PULL - [14:13] */
#define WM8325_GP2_OTP_PULL_SHIFT               (    13)  /* GP2_PULL - [14:13] */
#define WM8325_GP2_OTP_PULL_WIDTH               (     2)  /* GP2_PULL - [14:13] */
#define WM8325_GP2_OTP_INT_MODE                 (0x1000)  /* GP2_INT_MODE */
#define WM8325_GP2_OTP_INT_MODE_MASK            (0x1000)  /* GP2_INT_MODE */
#define WM8325_GP2_OTP_INT_MODE_SHIFT           (    12)  /* GP2_INT_MODE */
#define WM8325_GP2_OTP_INT_MODE_WIDTH           (     1)  /* GP2_INT_MODE */
#define WM8325_GP2_OTP_PWR_DOM                  (0x0800)  /* GP2_PWR_DOM */
#define WM8325_GP2_OTP_PWR_DOM_MASK             (0x0800)  /* GP2_PWR_DOM */
#define WM8325_GP2_OTP_PWR_DOM_SHIFT            (    11)  /* GP2_PWR_DOM */
#define WM8325_GP2_OTP_PWR_DOM_WIDTH            (     1)  /* GP2_PWR_DOM */
#define WM8325_GP2_OTP_POL                      (0x0400)  /* GP2_POL */
#define WM8325_GP2_OTP_POL_MASK                 (0x0400)  /* GP2_POL */
#define WM8325_GP2_OTP_POL_SHIFT                (    10)  /* GP2_POL */
#define WM8325_GP2_OTP_POL_WIDTH                (     1)  /* GP2_POL */
#define WM8325_GP2_OTP_OD                       (0x0200)  /* GP2_OD */
#define WM8325_GP2_OTP_OD_MASK                  (0x0200)  /* GP2_OD */
#define WM8325_GP2_OTP_OD_SHIFT                 (     9)  /* GP2_OD */
#define WM8325_GP2_OTP_OD_WIDTH                 (     1)  /* GP2_OD */
#define WM8325_GP2_OTP_ENA                      (0x0100)  /* GP2_ENA */
#define WM8325_GP2_OTP_ENA_MASK                 (0x0100)  /* GP2_ENA */
#define WM8325_GP2_OTP_ENA_SHIFT                (     8)  /* GP2_ENA */
#define WM8325_GP2_OTP_ENA_WIDTH                (     1)  /* GP2_ENA */
#define WM8325_GP2_OTP_FN_MASK                  (0x00F0)  /* GP2_FN - [7:4] */
#define WM8325_GP2_OTP_FN_SHIFT                 (     4)  /* GP2_FN - [7:4] */
#define WM8325_GP2_OTP_FN_WIDTH                 (     4)  /* GP2_FN - [7:4] */
#define WM8325_OTP_CLKOUT_SLOT_MASK             (0x000E)  /* CLKOUT_SLOT - [3:1] */
#define WM8325_OTP_CLKOUT_SLOT_SHIFT            (     1)  /* CLKOUT_SLOT - [3:1] */
#define WM8325_OTP_CLKOUT_SLOT_WIDTH            (     3)  /* CLKOUT_SLOT - [3:1] */
#define WM8325_OTP_WDOG_ENA                     (0x0001)  /* WDOG_ENA */
#define WM8325_OTP_WDOG_ENA_MASK                (0x0001)  /* WDOG_ENA */
#define WM8325_OTP_WDOG_ENA_SHIFT               (     0)  /* WDOG_ENA */
#define WM8325_OTP_WDOG_ENA_WIDTH               (     1)  /* WDOG_ENA */
                                             
/*                                                                                                      
 * R30748 (0x781C) - GPIO3 OTP Control                                            
 */                                           
#define WM8325_GP3_OTP_DIR                      (0x8000)  /* GP3_DIR */
#define WM8325_GP3_OTP_DIR_MASK                 (0x8000)  /* GP3_DIR */
#define WM8325_GP3_OTP_DIR_SHIFT                (    15)  /* GP3_DIR */
#define WM8325_GP3_OTP_DIR_WIDTH                (     1)  /* GP3_DIR */
#define WM8325_GP3_OTP_PULL_MASK                (0x6000)  /* GP3_PULL - [14:13] */
#define WM8325_GP3_OTP_PULL_SHIFT               (    13)  /* GP3_PULL - [14:13] */
#define WM8325_GP3_OTP_PULL_WIDTH               (     2)  /* GP3_PULL - [14:13] */
#define WM8325_GP3_OTP_INT_MODE                 (0x1000)  /* GP3_INT_MODE */
#define WM8325_GP3_OTP_INT_MODE_MASK            (0x1000)  /* GP3_INT_MODE */
#define WM8325_GP3_OTP_INT_MODE_SHIFT           (    12)  /* GP3_INT_MODE */
#define WM8325_GP3_OTP_INT_MODE_WIDTH           (     1)  /* GP3_INT_MODE */
#define WM8325_GP3_OTP_PWR_DOM                  (0x0800)  /* GP3_PWR_DOM */
#define WM8325_GP3_OTP_PWR_DOM_MASK             (0x0800)  /* GP3_PWR_DOM */
#define WM8325_GP3_OTP_PWR_DOM_SHIFT            (    11)  /* GP3_PWR_DOM */
#define WM8325_GP3_OTP_PWR_DOM_WIDTH            (     1)  /* GP3_PWR_DOM */
#define WM8325_GP3_OTP_POL                      (0x0400)  /* GP3_POL */
#define WM8325_GP3_OTP_POL_MASK                 (0x0400)  /* GP3_POL */
#define WM8325_GP3_OTP_POL_SHIFT                (    10)  /* GP3_POL */
#define WM8325_GP3_OTP_POL_WIDTH                (     1)  /* GP3_POL */
#define WM8325_GP3_OTP_OD                       (0x0200)  /* GP3_OD */
#define WM8325_GP3_OTP_OD_MASK                  (0x0200)  /* GP3_OD */
#define WM8325_GP3_OTP_OD_SHIFT                 (     9)  /* GP3_OD */
#define WM8325_GP3_OTP_OD_WIDTH                 (     1)  /* GP3_OD */
#define WM8325_GP3_OTP_ENA                      (0x0100)  /* GP3_ENA */
#define WM8325_GP3_OTP_ENA_MASK                 (0x0100)  /* GP3_ENA */
#define WM8325_GP3_OTP_ENA_SHIFT                (     8)  /* GP3_ENA */
#define WM8325_GP3_OTP_ENA_WIDTH                (     1)  /* GP3_ENA */
#define WM8325_GP3_OTP_FN_MASK                  (0x00F0)  /* GP3_FN - [7:4] */
#define WM8325_GP3_OTP_FN_SHIFT                 (     4)  /* GP3_FN - [7:4] */
#define WM8325_GP3_OTP_FN_WIDTH                 (     4)  /* GP3_FN - [7:4] */
                                           
/*                                                                                                 
 * R30749 (0x781D) - GPIO4 OTP Control                                       
 */                                         
#define WM8325_GP4_OTP_DIR                      (0x8000)  /* GP4_DIR */
#define WM8325_GP4_OTP_DIR_MASK                 (0x8000)  /* GP4_DIR */
#define WM8325_GP4_OTP_DIR_SHIFT                (    15)  /* GP4_DIR */
#define WM8325_GP4_OTP_DIR_WIDTH                (     1)  /* GP4_DIR */
#define WM8325_GP4_OTP_PULL_MASK                (0x6000)  /* GP4_PULL - [14:13] */
#define WM8325_GP4_OTP_PULL_SHIFT               (    13)  /* GP4_PULL - [14:13] */
#define WM8325_GP4_OTP_PULL_WIDTH               (     2)  /* GP4_PULL - [14:13] */
#define WM8325_GP4_OTP_INT_MODE                 (0x1000)  /* GP4_INT_MODE */
#define WM8325_GP4_OTP_INT_MODE_MASK            (0x1000)  /* GP4_INT_MODE */
#define WM8325_GP4_OTP_INT_MODE_SHIFT           (    12)  /* GP4_INT_MODE */
#define WM8325_GP4_OTP_INT_MODE_WIDTH           (     1)  /* GP4_INT_MODE */
#define WM8325_GP4_OTP_PWR_DOM                  (0x0800)  /* GP4_PWR_DOM */
#define WM8325_GP4_OTP_PWR_DOM_MASK             (0x0800)  /* GP4_PWR_DOM */
#define WM8325_GP4_OTP_PWR_DOM_SHIFT            (    11)  /* GP4_PWR_DOM */
#define WM8325_GP4_OTP_PWR_DOM_WIDTH            (     1)  /* GP4_PWR_DOM */
#define WM8325_GP4_OTP_POL                      (0x0400)  /* GP4_POL */
#define WM8325_GP4_OTP_POL_MASK                 (0x0400)  /* GP4_POL */
#define WM8325_GP4_OTP_POL_SHIFT                (    10)  /* GP4_POL */
#define WM8325_GP4_OTP_POL_WIDTH                (     1)  /* GP4_POL */
#define WM8325_GP4_OTP_OD                       (0x0200)  /* GP4_OD */
#define WM8325_GP4_OTP_OD_MASK                  (0x0200)  /* GP4_OD */
#define WM8325_GP4_OTP_OD_SHIFT                 (     9)  /* GP4_OD */
#define WM8325_GP4_OTP_OD_WIDTH                 (     1)  /* GP4_OD */
#define WM8325_GP4_OTP_ENA                      (0x0100)  /* GP4_ENA */
#define WM8325_GP4_OTP_ENA_MASK                 (0x0100)  /* GP4_ENA */
#define WM8325_GP4_OTP_ENA_SHIFT                (     8)  /* GP4_ENA */
#define WM8325_GP4_OTP_ENA_WIDTH                (     1)  /* GP4_ENA */
#define WM8325_GP4_OTP_FN_MASK                  (0x00F0)  /* GP4_FN - [7:4] */
#define WM8325_GP4_OTP_FN_SHIFT                 (     4)  /* GP4_FN - [7:4] */
#define WM8325_GP4_OTP_FN_WIDTH                 (     4)  /* GP4_FN - [7:4] */
#define WM8325_LED1_OTP_SRC_MASK                (0x000C)  /* LED1_SRC - [3:2] */
#define WM8325_LED1_OTP_SRC_SHIFT               (     2)  /* LED1_SRC - [3:2] */
#define WM8325_LED1_OTP_SRC_WIDTH               (     2)  /* LED1_SRC - [3:2] */
#define WM8325_LED2_OTP_SRC_MASK                (0x0003)  /* LED2_SRC - [1:0] */
#define WM8325_LED2_OTP_SRC_SHIFT               (     0)  /* LED2_SRC - [1:0] */
#define WM8325_LED2_OTP_SRC_WIDTH               (     2)  /* LED2_SRC - [1:0] */
                                             
/*                                                                                                      
 * R30750 (0x781E) - GPIO5 OTP Control                                            
 */                                           
#define WM8325_GP5_OTP_DIR                      (0x8000)  /* GP5_DIR */
#define WM8325_GP5_OTP_DIR_MASK                 (0x8000)  /* GP5_DIR */
#define WM8325_GP5_OTP_DIR_SHIFT                (    15)  /* GP5_DIR */
#define WM8325_GP5_OTP_DIR_WIDTH                (     1)  /* GP5_DIR */
#define WM8325_GP5_OTP_PULL_MASK                (0x6000)  /* GP5_PULL - [14:13] */
#define WM8325_GP5_OTP_PULL_SHIFT               (    13)  /* GP5_PULL - [14:13] */
#define WM8325_GP5_OTP_PULL_WIDTH               (     2)  /* GP5_PULL - [14:13] */
#define WM8325_GP5_OTP_INT_MODE                 (0x1000)  /* GP5_INT_MODE */
#define WM8325_GP5_OTP_INT_MODE_MASK            (0x1000)  /* GP5_INT_MODE */
#define WM8325_GP5_OTP_INT_MODE_SHIFT           (    12)  /* GP5_INT_MODE */
#define WM8325_GP5_OTP_INT_MODE_WIDTH           (     1)  /* GP5_INT_MODE */
#define WM8325_GP5_OTP_PWR_DOM                  (0x0800)  /* GP5_PWR_DOM */
#define WM8325_GP5_OTP_PWR_DOM_MASK             (0x0800)  /* GP5_PWR_DOM */
#define WM8325_GP5_OTP_PWR_DOM_SHIFT            (    11)  /* GP5_PWR_DOM */
#define WM8325_GP5_OTP_PWR_DOM_WIDTH            (     1)  /* GP5_PWR_DOM */
#define WM8325_GP5_OTP_POL                      (0x0400)  /* GP5_POL */
#define WM8325_GP5_OTP_POL_MASK                 (0x0400)  /* GP5_POL */
#define WM8325_GP5_OTP_POL_SHIFT                (    10)  /* GP5_POL */
#define WM8325_GP5_OTP_POL_WIDTH                (     1)  /* GP5_POL */
#define WM8325_GP5_OTP_OD                       (0x0200)  /* GP5_OD */
#define WM8325_GP5_OTP_OD_MASK                  (0x0200)  /* GP5_OD */
#define WM8325_GP5_OTP_OD_SHIFT                 (     9)  /* GP5_OD */
#define WM8325_GP5_OTP_OD_WIDTH                 (     1)  /* GP5_OD */
#define WM8325_GP5_OTP_ENA                      (0x0100)  /* GP5_ENA */
#define WM8325_GP5_OTP_ENA_MASK                 (0x0100)  /* GP5_ENA */
#define WM8325_GP5_OTP_ENA_SHIFT                (     8)  /* GP5_ENA */
#define WM8325_GP5_OTP_ENA_WIDTH                (     1)  /* GP5_ENA */
#define WM8325_GP5_OTP_FN_MASK                  (0x00F0)  /* GP5_FN - [7:4] */
#define WM8325_GP5_OTP_FN_SHIFT                 (     4)  /* GP5_FN - [7:4] */
#define WM8325_GP5_OTP_FN_WIDTH                 (     4)  /* GP5_FN - [7:4] */
                                            
/*                                                                                                   
 * R30751 (0x781F) - GPIO6 OTP Control                                          
 */                                          
#define WM8325_GP6_OTP_DIR                      (0x8000)  /* GP6_DIR */
#define WM8325_GP6_OTP_DIR_MASK                 (0x8000)  /* GP6_DIR */
#define WM8325_GP6_OTP_DIR_SHIFT                (    15)  /* GP6_DIR */
#define WM8325_GP6_OTP_DIR_WIDTH                (     1)  /* GP6_DIR */
#define WM8325_GP6_OTP_PULL_MASK                (0x6000)  /* GP6_PULL - [14:13] */
#define WM8325_GP6_OTP_PULL_SHIFT               (    13)  /* GP6_PULL - [14:13] */
#define WM8325_GP6_OTP_PULL_WIDTH               (     2)  /* GP6_PULL - [14:13] */
#define WM8325_GP6_OTP_INT_MODE                 (0x1000)  /* GP6_INT_MODE */
#define WM8325_GP6_OTP_INT_MODE_MASK            (0x1000)  /* GP6_INT_MODE */
#define WM8325_GP6_OTP_INT_MODE_SHIFT           (    12)  /* GP6_INT_MODE */
#define WM8325_GP6_OTP_INT_MODE_WIDTH           (     1)  /* GP6_INT_MODE */
#define WM8325_GP6_OTP_PWR_DOM                  (0x0800)  /* GP6_PWR_DOM */
#define WM8325_GP6_OTP_PWR_DOM_MASK             (0x0800)  /* GP6_PWR_DOM */
#define WM8325_GP6_OTP_PWR_DOM_SHIFT            (    11)  /* GP6_PWR_DOM */
#define WM8325_GP6_OTP_PWR_DOM_WIDTH            (     1)  /* GP6_PWR_DOM */
#define WM8325_GP6_OTP_POL                      (0x0400)  /* GP6_POL */
#define WM8325_GP6_OTP_POL_MASK                 (0x0400)  /* GP6_POL */
#define WM8325_GP6_OTP_POL_SHIFT                (    10)  /* GP6_POL */
#define WM8325_GP6_OTP_POL_WIDTH                (     1)  /* GP6_POL */
#define WM8325_GP6_OTP_OD                       (0x0200)  /* GP6_OD */
#define WM8325_GP6_OTP_OD_MASK                  (0x0200)  /* GP6_OD */
#define WM8325_GP6_OTP_OD_SHIFT                 (     9)  /* GP6_OD */
#define WM8325_GP6_OTP_OD_WIDTH                 (     1)  /* GP6_OD */
#define WM8325_GP6_OTP_ENA                      (0x0100)  /* GP6_ENA */
#define WM8325_GP6_OTP_ENA_MASK                 (0x0100)  /* GP6_ENA */
#define WM8325_GP6_OTP_ENA_SHIFT                (     8)  /* GP6_ENA */
#define WM8325_GP6_OTP_ENA_WIDTH                (     1)  /* GP6_ENA */
#define WM8325_GP6_OTP_FN_MASK                  (0x00F0)  /* GP6_FN - [7:4] */
#define WM8325_GP6_OTP_FN_SHIFT                 (     4)  /* GP6_FN - [7:4] */
#define WM8325_GP6_OTP_FN_WIDTH                 (     4)  /* GP6_FN - [7:4] */
#define WM8325_SYSOK_OTP_THR_MASK               (0x000E)  /* SYSOK_THR - [3:1] */
#define WM8325_SYSOK_OTP_THR_SHIFT              (     1)  /* SYSOK_THR - [3:1] */
#define WM8325_SYSOK_OTP_THR_WIDTH              (     3)  /* SYSOK_THR - [3:1] */
                                            
/*                                                                                                   
 * R30759 (0x7827) - ICE CHECK DATA                                            
 */                                          
#define WM8325_ICE_VALID_DATA_MASK              (0xFFFF)  /* ICE_VALID_DATA - [15:0] */
#define WM8325_ICE_VALID_DATA_SHIFT             (     0)  /* ICE_VALID_DATA - [15:0] */
#define WM8325_ICE_VALID_DATA_WIDTH             (    16)  /* ICE_VALID_DATA - [15:0] */


#endif    /* __WM8325DEFINES_H__ */

