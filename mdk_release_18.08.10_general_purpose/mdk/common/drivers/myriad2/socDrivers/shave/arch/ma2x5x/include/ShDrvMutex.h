///
/// @file ShDrvMutex.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup ShDrvMutex ShMutex Driver
/// @{
/// @brief Shave Mutex API implementation for MA2x5x
///


#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVMUTEX_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVMUTEX_H_

__asm(
".include svuCommonDefinitions.incl\n");

/// @brief Mutex Request
///
/// @note For a detailed explanation, please see the "Mutexes" section in the MDKMyriad2Programmer_Guide
///
/// @param[in] mutex_num - mutex number requested
///
/// @return     void
///
static inline void ShDrvMutexRequest(uint32_t mutex_num)
{
    uint32_t local_var_tmp;

    asm volatile (                                                                 \
        "// 'ShDrvMutexRequest()' defined in ShDrvMutex.h used in " __FILE__"\n\t" \
        "//Need this nop to ensure absolutely that there is no portclash."  "\n\t" \
        "NOP"                                                               "\n\t" \
        "LSU1.LDIL     %[tmp], MUTEX_REQUEST_0"                             "\n\t" \
        "//Disabling WARNING for RPIM since it is used here on purpose."    "\n\t" \
        ".nowarn 18"                                                        "\n\t" \
        "IAU.OR        %[tmp], %[tmp], %[mutex_num_]"                       "\n\t" \
        "LSU0.STA.32   %[tmp], SHAVE_LOCAL, MUTEX_CTRL_ADDR"                "\n\t" \
        "NOP 4"        /* mandatory latency for correct operation */        "\n\t" \
        "LSU0.LDIL     %[tmp], MUTEX_GRANT_MASK"                            "\n\t" \
        "CMU.CMTI.BITP %[tmp], P_GPI"                                       "\n\t" \
        "PEU.PC1C EQ   || BRU.RPIM 0 || CMU.CMTI.BITP %[tmp], P_GPI"        "\n\t" \
        "//Need this nop to ensure absolutely that there is no portclash."  "\n\t" \
        "NOP"                                                               "\n\t" \
        ".nowarnend"                                                               \
        : [tmp] "=&r" (local_var_tmp)                                              \
        : [mutex_num_] "r" (mutex_num)                                             \
        : "cc", "memory"                                                           \
        );
}

/// @brief  This will release mutex
///
/// @note For a detailed explanation, please see the "Mutexes" section in the MDKMyriad2Programmer_Guide
///
/// @param[in]  mutex_num - mutex number that will be released
///
/// @return     void
///
static inline void ShDrvMutexRelease(uint32_t mutex_num)
{
    uint32_t local_var_tmp;

    asm volatile (                                                                 \
        "// 'ShDrvMutexRelease()' defined in ShDrvMutex.h used in " __FILE__"\n\t" \
        "//Need this nop to ensure absolutely that there is no portclash."  "\n\t" \
        "NOP"                                                               "\n\t" \
        "LSU1.LDIL     %[tmp], MUTEX_RELEASE_0"                             "\n\t" \
        "//Disabling WARNING for RPIM since it is used here on purpose."    "\n\t" \
        ".nowarn 18"                                                        "\n\t" \
        "IAU.OR        %[tmp], %[tmp], %[mutex_num_]"                       "\n\t" \
        "LSU0.STA.32   %[tmp], SHAVE_LOCAL, MUTEX_CTRL_ADDR"                "\n\t" \
        "NOP 4"        /* mandatory latency for correct operation */        "\n\t" \
        "LSU0.LDIL     %[tmp], MUTEX_GRANT_MASK"                            "\n\t" \
        "CMU.CMTI.BITP %[tmp], P_GPI"                                       "\n\t" \
        "PEU.PC1C NEQ  || BRU.RPIM 0 || CMU.CMTI.BITP %[tmp], P_GPI"        "\n\t" \
        "//Need this nop to ensure absolutely that there is no portclash."  "\n\t" \
        "NOP"                                                               "\n\t" \
        ".nowarnend"                                                               \
        : [tmp] "=&r" (local_var_tmp)                                              \
        : [mutex_num_] "r" (mutex_num)                                             \
        : "cc", "memory"                                                           \
    );
}

#endif // COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_SHAVE_ARCH_MA2X5X_INCLUDE_SHDRVMUTEX_H_

///@}

