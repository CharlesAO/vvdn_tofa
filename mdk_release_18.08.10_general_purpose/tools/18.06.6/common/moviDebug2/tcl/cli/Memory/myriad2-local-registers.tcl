# ***************************************************************************
# Copyright (C) 2017 Intel corporation. All rights reserved
# ---------------------------------------------------------------------------
# File       : myriad2-local-registers.tcl
# Description:
# Created on : Sep 14, 2017
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************
namespace eval mdbg::cli::Memory {

    proc BUILD_LOCAL_REGISTER_MAP {} {
        variable LOCAL_REGISTER_MAP
        upvar 0 LOCAL_REGISTER_MAP MAP

        set MAP [dict create]

        for {set shaveNumber 0} {$shaveNumber < 16} { incr shaveNumber } {
            set core S$shaveNumber

            for {set regIndex 0} {$regIndex < 32} { incr regIndex } {
                dict set MAP $core I$regIndex SVU_REG_I$regIndex\($shaveNumber\)

                dict set MAP $core V${regIndex} [list SVU_REG_V${regIndex}_0\($shaveNumber\) 4]

                for {set vIndex 0} {$vIndex < 4} {incr vIndex} {
                    dict set MAP $core V${regIndex}_$vIndex SVU_REG_V${regIndex}_${vIndex}\($shaveNumber\)
                }
            }

            foreach {name reg} {
                IP                          SVU_TRF
                L1_CACHE_DATA               L1_CACHE_DATA
                L1_CACHE_TAG                L1_CACHE_TAG
                SLICE_DBG                   SLICE_DBG
                DCU                         SVU_DCU
                DCR                         SVU_DCR
                OCR                         SVU_OCR
                OSR                         SVU_OSR
                ICR                         SVU_ICR
                ISR                         SVU_ISR
                IRR                         SVU_IRR
                FXE                         SVU_FXE
                FXF                         SVU_FXF
                IXE                         SVU_IXE
                IXF                         SVU_IXF
                PTR                         SVU_PTR
                IHR                         SVU_IHR
                BTH                         SVU_BTH
                PTH                         SVU_PTH
                RF_CFG                      SVU_RF_CFG
                INEXT                       SVU_INEXT
                IH                          SVU_IH
                BRUCTRL                     SVU_BRUCTRL
                IDCCRC                      SVU_IDCCRC
                TRF                         SVU_TRF
                B_IP_0                      SVU_B_IP_0
                B_LBEG                      SVU_B_LBEG
                B_LEND                      SVU_B_LEND
                B_MREPS                     SVU_B_MREPS
                B_STATE                     SVU_B_STATE
                B_RFB                       SVU_B_RFB
                I_STATE                     SVU_I_STATE
                S_STATE                     SVU_S_STATE
                V_STATE                     SVU_V_STATE
                S_ACC                       SVU_S_ACC
                V_ACC0                      SVU_V_ACC0
                V_ACC1                      SVU_V_ACC1
                V_ACC2                      SVU_V_ACC2
                V_ACC3                      SVU_V_ACC3
                NONE                        SVU_NONE
                I_AE                        SVU_I_AE
                F_AE                        SVU_F_AE
                C_CSI                       SVU_C_CSI
                C_CMU0                      SVU_C_CMU0
                C_CMU1                      SVU_C_CMU1
                B_SREPS                     SVU_B_SREPS
                ISR_STAT                    SVU_ISR_STAT
                ISR_RA                      SVU_ISR_RA
                ISR_SA                      SVU_ISR_SA
                P_CFG                       SVU_P_CFG
                P_SVID                      SVU_P_SVID
                P_VERS                      SVU_P_VERS
                P_GPI                       SVU_P_GPI
                P_GPR                       SVU_P_GPR
                IRF                         SVU_IRF
                SRF                         SVU_SRF
                VRF                         SVU_VRF
                DCR                         SVU_DCR
                DSR                         SVU_DSR
                IBA0                        SVU_IBA0
                IBC0                        SVU_IBC0
                IBA1                        SVU_IBA1
                IBC1                        SVU_IBC1
                DBA0                        SVU_DBA0
                DBD0                        SVU_DBD0
                DBC0                        SVU_DBC0
                DBA1                        SVU_DBA1
                DBD1                        SVU_DBD1
                DBC1                        SVU_DBC1
                PC0                         SVU_PC0
                PCC0                        SVU_PCC0
                PC1                         SVU_PC1
                PCC1                        SVU_PCC1
                DBK_BYTE                    SVU_DBK_BYTE
                PC2                         SVU_PC2
                PCC2                        SVU_PCC2
                PC3                         SVU_PC3
                PCC3                        SVU_PCC3
                IDC_FIFO                    SVU_IDC_FIFO
                IDC_FIFO_PTR                SVU_IDC_FIFO_PTR
                TLB_DBL_ENTRY               SVU_TLB_DBL_ENTRY
                CACHE_CTRL                  SVU_CACHE_CTRL
                CACHE_SVU_CTRL              SVU_CACHE_SVU_CTRL
                CACHE_CNT_IN                SVU_CACHE_CNT_IN
                CACHE_CNT_OUT               SVU_CACHE_CNT_OUT
                CACHE_CTRL_TXN_ADDR         SVU_CACHE_CTRL_TXN_ADDR
                CACHE_CTRL_TXN_TYPE         SVU_CACHE_CTRL_TXN_TYPE
                CACHE_STATUS                SVU_CACHE_STATUS
                CACHE_RAM_CFG               SVU_CACHE_RAM_CFG
                CACHE_DATA_RAM_ACCESS       SVU_CACHE_DATA_RAM_ACCESS
                CACHE_TAG_RAM_ACCESS        SVU_CACHE_TAG_RAM_ACCESS
                MUTEX_CTRL                  SVU_MUTEX_CTRL
                MUTEX_STATUS                SVU_MUTEX_STATUS
            } {
                dict set MAP $core $name $reg\($shaveNumber\)
            }
        }

        if {
            [info commands ::mdbg::cv::myriad2::LEON_CORE_SETUP_CONF] != ""
        } {
            set leonCoreSetup [::mdbg::cv::myriad2::LEON_CORE_SETUP_CONF]
        } else {
            set leonCoreSetup {OS RT}
        }

        foreach leonSetup $leonCoreSetup {
            set options [lassign $leonSetup leon]
            set core L$leon
            if {    [llength $options] > 0
                && ![string match -* [lindex $options 0]]
            } {
                set options [lassign $options index]
                append core $index
                append leon $index
                unset index
            }
            foreach R {I L O G} {
                for {set regIndex 0} {$regIndex < 8} {incr regIndex} {
                    set name ${R}${regIndex}
                    dict set MAP $core $name LEON_${leon}_REG_${name}_ADR
                }
            }
            for {set CWP 0} {$CWP < 8} {incr CWP} {
                foreach R {I L O} {
                    for {set regIndex 0} {$regIndex < 8} {incr regIndex} {
                        set name ${R}${regIndex}
                        dict set MAP $core $name\($CWP\) LEON_${leon}_REG_${name}_\($CWP\)
                    }
                }
            }

            if {"-nofpu" ni $options} {
                for {set regIndex 0} {$regIndex < 32} {incr regIndex} {
                    dict set MAP $core F${regIndex} LEON_${leon}_REG_F${regIndex}_ADR
                }
            }

            for {set regIndex 16} {$regIndex < 32} {incr regIndex} {
                dict set MAP $core ASR${regIndex} LEON_${leon}_DSU_ASR${regIndex}_REG_ADR
            }

            foreach {name reg} {
                Y      Y_REG
                PSR    PSR
                WIM    WIM
                TBR    TBR
                PC     PC_REG
                NPC    NPC_REG
                FSR    FSR_REG
            } {
                dict set MAP $core $name LEON_${leon}_DSU_${reg}_ADR
            }
        }
    }

} ; # namespace eval mdbg::cli::Memory