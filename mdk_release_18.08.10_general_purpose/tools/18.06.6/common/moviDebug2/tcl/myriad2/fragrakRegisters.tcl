# ***************************************************************************
# Copyright (C) 2015 Movidius Ltd. All rights reserved
# ---------------------------------------------------------------------------
# File       : fragrakRegisters.tcl
# Description: fragrakRegisters.h ported from C to Tcl with some enhancements
# Created on : May 19, 2015
# Author     : Hanos-Puskai Peter (peter.hanos@movidius.com)
# ***************************************************************************

# LEON DSU

if {[info commands mdbg::cv::myriad2::LEON_CORE_DSU_DEFS] == ""} {
    mdbg::REG_DEFS {
            LEON_OS_DSU_BASE_ADR                   {$DSU_LEON_OS_BASE_ADR}
            LEON_RT_DSU_BASE_ADR                   {$DSU_LEON_RT_BASE_ADR}
    }
} else {
    mdbg::REG_DEFS [mdbg::cv::myriad2::LEON_CORE_DSU_DEFS]
}

# LEON  registers

proc SetupLeonRegDefs {leon args} {

    if {    [llength $args] > 0
        && ![string match -* [lindex $args 0]]
    } {
        set args [lassign $args index]
        set zero 0
    } else {
        set index ""
        set zero ""
    }

    mdbg::REG_DEFS "
        LEON_${leon}${index}_DSU_CTRL_ADR                   {\$LEON_${leon}${index}_DSU_BASE_ADR + 0x0000}
        LEON_${leon}${index}_DSU_BREAK_SINGLE_STEP_ADR      {\$LEON_${leon}${zero}_DSU_BASE_ADR + 0x0020}

        LEON_${leon}${index}_DSU_IU_REG_ADR                 {\$LEON_${leon}${index}_DSU_BASE_ADR + 0x300000}
        LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR            {\$LEON_${leon}${index}_DSU_BASE_ADR + 0x400000}

        LEON_${leon}${index}_DSU_Y_REG_ADR                  {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0x0}
        LEON_${leon}${index}_DSU_PSR_ADR                    {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0x4}
        LEON_${leon}${index}_DSU_WIM_ADR                    {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0x8}
        LEON_${leon}${index}_DSU_TBR_ADR                    {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0xC}
        LEON_${leon}${index}_DSU_PC_REG_ADR                 {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0x10}
        LEON_${leon}${index}_DSU_NPC_REG_ADR                {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0x14}
        LEON_${leon}${index}_DSU_FSR_REG_ADR                {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0x18}

        LEON_${leon}${index}_DSU_TRAP_REG_ADR               {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + 0x20}
    "

    set DSU_IU "( \$LEON_${leon}${index}_DSU_IU_REG_ADR )"

    if {"-noasr" ni $args} {
        for {set regIndex 16} {$regIndex < 32} {incr regIndex} {
            set offset [format 0x%02X [expr {$regIndex * 4}]]
            mdbg::REG_DEFS "
                LEON_${leon}${index}_DSU_ASR${regIndex}_REG_ADR
                {\$LEON_${leon}${index}_DSU_IU_SPR_BASE_ADR + $offset}
            "
        }
    }

    if {"-nofpu" ni $args} {
        for {set regIndex 0} {$regIndex < 32} {incr regIndex} {
                mdbg::REG_DEFS "
                    LEON_${leon}${index}_REG_F${regIndex}_ADR {\$DSU_LEON_${leon}${index}_FPU_REGFILE_BASE_ADR + $regIndex * 4}
                "
        }
    }

    for {set CWP 0} {$CWP < 8} {incr CWP} {
        for {set regIndex 0} {$regIndex < 8} {incr regIndex} {
            mdbg::REG_DEFS "
                LEON_${leon}${index}_REG_I${regIndex}_\($CWP\) { $DSU_IU + ( ( $CWP * 64 ) + 0x60 + ( $regIndex * 4 ) ) % 0x200}
                LEON_${leon}${index}_REG_L${regIndex}_\($CWP\) { $DSU_IU + ( ( $CWP * 64 ) + 0x40 + ( $regIndex * 4 ) ) % 0x200}
                LEON_${leon}${index}_REG_O${regIndex}_\($CWP\) { $DSU_IU + ( ( $CWP * 64 ) + 0x20 + ( $regIndex * 4 ) ) % 0x200}
            "
        }
    }

    # Theroretical maximum
    # set CWP_MASK 0x1F
    # Actual maximum (8 windows)
    set CWP_MASK 0x7

    set CWP_INDEX "\[expr {\[GET32 LEON_${leon}${index}_DSU_PSR_ADR\] & $CWP_MASK}\]"

    for {set regIndex 0} {$regIndex < 8} {incr regIndex} {
        mdbg::REG_DEFS "
            LEON_${leon}${index}_REG_I${regIndex}_ADR     { \$LEON_${leon}${index}_REG_I${regIndex}_\($CWP_INDEX\)}
            LEON_${leon}${index}_REG_L${regIndex}_ADR     { \$LEON_${leon}${index}_REG_L${regIndex}_\($CWP_INDEX\)}
            LEON_${leon}${index}_REG_O${regIndex}_ADR     { \$LEON_${leon}${index}_REG_O${regIndex}_\($CWP_INDEX\)}
            LEON_${leon}${index}_REG_G${regIndex}_ADR     { $DSU_IU + ( $regIndex * 4 ) + 0x200}
        "
    }
}

if {[info commands mdbg::cv::myriad2::LEON_CORE_SETUP_CONF] != ""} {
    foreach leon [mdbg::cv::myriad2::LEON_CORE_SETUP_CONF] {
        SetupLeonRegDefs {*}$leon
    }
} else {
    foreach leon {OS RT} {
        SetupLeonRegDefs $leon
    }
}

rename SetupLeonRegDefs ""

unset leon

# SHAVE
mdbg::REG_DEFS {
        SVU_BASE_ADDRESS                                      {$LPB3_CONTROL_ADR ?? 0x20F00000}
        SVU_SLICE_OFFSET                                      0x00010000
        SVU_DCU_OFFSET                                        {$SLICE_DCU_OFFSET ?? 0x00001000 }

        ROM_BASE_ADR                                          {$LHB_ROM_BASE_ADR ?? 0x7FF00000}

        SVU_WINDOW_A                0x1C000000
        SVU_WINDOW_B                0x1D000000
        SVU_WINDOW_C                0x1E000000
        SVU_WINDOW_D                0x1F000000
}

proc SetupShaveRegDefs {shaveNumber} {
    mdbg::REG_DEFS "
        SVU_SLICE_ADDR\($shaveNumber\)                            {\$SHAVE_${shaveNumber}_BASE_ADR ?? \$SVU_BASE_ADDRESS + ( $shaveNumber * \$SVU_SLICE_OFFSET ) }
        OFFSET_WIN_A\($shaveNumber\)                              {\$SVU_SLICE_ADDR\($shaveNumber\) + 0x10}
        OFFSET_WIN_B\($shaveNumber\)                              {\$SVU_SLICE_ADDR\($shaveNumber\) + 0x14}
        OFFSET_WIN_C\($shaveNumber\)                              {\$SVU_SLICE_ADDR\($shaveNumber\) + 0x18}
        OFFSET_WIN_D\($shaveNumber\)                              {\$SVU_SLICE_ADDR\($shaveNumber\) + 0x1C}

        SLICE_DBG\($shaveNumber\)                                 {\$SVU_SLICE_ADDR\($shaveNumber\) + 0x08}

        SVU_MUTEX_CTRL\($shaveNumber\)                            {\$SVU_SLICE_ADDR\($shaveNumber\) + 0x3000}
        SVU_MUTEX_STATUS\($shaveNumber\)                          {\$SVU_MUTEX_CTRL\($shaveNumber\) + 0x0004}

        L1_CACHE_DATA\($shaveNumber\)                             {\$SVU_SLICE_ADDR\($shaveNumber\) + 0x8000}
        L1_CACHE_TAG\($shaveNumber\)                              {\$SVU_SLICE_ADDR\($shaveNumber\) + 0xC000}

        SVU_DCU\($shaveNumber\)                                   {\$SVU_SLICE_ADDR\($shaveNumber\) + \$SVU_DCU_OFFSET}
        SVU_DCR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x800}
        SVU_OCR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x000}
        SVU_OSR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x004}
        SVU_ICR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x008}
        SVU_ISR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x00C}
        SVU_IRR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x010}
        SVU_FXE\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x014}
        SVU_FXF\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x018}
        SVU_IXE\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x01C}
        SVU_IXF\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x020}
        SVU_PTR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x024}
        SVU_IHR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x028}
        SVU_BTH\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x050}
        SVU_PTH\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x054}
        SVU_RF_CFG\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x058}
        SVU_INEXT\($shaveNumber\)                                 {\$SVU_DCU\($shaveNumber\) + 0x05C}
        SVU_IH\($shaveNumber\)                                    {\$SVU_DCU\($shaveNumber\) + 0x060}
        SVU_BRUCTRL\($shaveNumber\)                               {\$SVU_DCU\($shaveNumber\) + 0x0E0}
        SVU_IDCCRC\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x0E4}
        SVU_TRF\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x100}

        SVU_B_IP_0\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x100}
        SVU_B_LBEG\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x104}
        SVU_B_LEND\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x108}
        SVU_B_MREPS\($shaveNumber\)                               {\$SVU_DCU\($shaveNumber\) + 0x10C}
        SVU_B_STATE\($shaveNumber\)                               {\$SVU_DCU\($shaveNumber\) + 0x110}
        SVU_B_RFB\($shaveNumber\)                                 {\$SVU_DCU\($shaveNumber\) + 0x114}
        SVU_I_STATE\($shaveNumber\)                               {\$SVU_DCU\($shaveNumber\) + 0x118}
        SVU_S_STATE\($shaveNumber\)                               {\$SVU_DCU\($shaveNumber\) + 0x11C}
        SVU_V_STATE\($shaveNumber\)                               {\$SVU_DCU\($shaveNumber\) + 0x120}
        SVU_S_ACC\($shaveNumber\)                                 {\$SVU_DCU\($shaveNumber\) + 0x124}
        SVU_V_ACC0\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x128}
        SVU_V_ACC1\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x12C}
        SVU_V_ACC2\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x130}
        SVU_V_ACC3\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x134}
        SVU_NONE\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x138}
        SVU_I_AE\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x13C}
        SVU_F_AE\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x140}
        SVU_C_CSI\($shaveNumber\)                                 {\$SVU_DCU\($shaveNumber\) + 0x144}
        SVU_C_CMU0\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x148}
        SVU_C_CMU1\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x14C}
        SVU_B_SREPS\($shaveNumber\)                               {\$SVU_DCU\($shaveNumber\) + 0x150}

        SVU_ISR_STAT\($shaveNumber\)                              {\$SVU_DCU\($shaveNumber\) + 0x160}
        SVU_ISR_RA\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x164}
        SVU_ISR_SA\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x168}
        SVU_P_CFG\($shaveNumber\)                                 {\$SVU_DCU\($shaveNumber\) + 0x16C}
        SVU_P_SVID\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x170}
        SVU_P_VERS\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x174}
        SVU_P_GPI\($shaveNumber\)                                 {\$SVU_DCU\($shaveNumber\) + 0x178}
        SVU_P_GPR\($shaveNumber\)                                 {\$SVU_DCU\($shaveNumber\) + 0x17C}

        SVU_IRF\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x180}
        SVU_SRF\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x200}
        SVU_VRF\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x300}
        SVU_DCR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x800}
        SVU_DSR\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x804}
        SVU_IBA0\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x808}
        SVU_IBC0\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x80C}
        SVU_IBA1\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x810}
        SVU_IBC1\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x814}
        SVU_DBA0\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x818}
        SVU_DBD0\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x81C}
        SVU_DBC0\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x820}
        SVU_DBA1\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x824}
        SVU_DBD1\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x828}
        SVU_DBC1\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x82C}
        SVU_PC0\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x830}
        SVU_PCC0\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x834}
        SVU_PC1\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x838}
        SVU_PCC1\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x83C}
        SVU_DBK_BYTE\($shaveNumber\)                              {\$SVU_DCU\($shaveNumber\) + 0x840}
        SVU_PC2\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x844}
        SVU_PCC2\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x848}
        SVU_PC3\($shaveNumber\)                                   {\$SVU_DCU\($shaveNumber\) + 0x84C}
        SVU_PCC3\($shaveNumber\)                                  {\$SVU_DCU\($shaveNumber\) + 0x850}
        SVU_IDC_FIFO\($shaveNumber\)                              {\$SVU_DCU\($shaveNumber\) + 0x900}
        SVU_IDC_FIFO_PTR\($shaveNumber\)                          {\$SVU_DCU\($shaveNumber\) + 0x940}
        TLB_DBL_ENTRY\($shaveNumber\)                             {\$SVU_DCU\($shaveNumber\) + 0x1000}

        CACHE_CTRL\($shaveNumber\)                                {\$SVU_DCU\($shaveNumber\) + 0x6000}
        CACHE_SVU_CTRL\($shaveNumber\)                            {\$SVU_DCU\($shaveNumber\) + 0x6004}
        CACHE_CNT_IN\($shaveNumber\)                              {\$SVU_DCU\($shaveNumber\) + 0x6008}
        CACHE_CNT_OUT\($shaveNumber\)                             {\$SVU_DCU\($shaveNumber\) + 0x600C}
        CACHE_CTRL_TXN_ADDR\($shaveNumber\)                       {\$SVU_DCU\($shaveNumber\) + 0x6010}
        CACHE_CTRL_TXN_TYPE\($shaveNumber\)                       {\$SVU_DCU\($shaveNumber\) + 0x6014}
        CACHE_STATUS\($shaveNumber\)                              {\$SVU_DCU\($shaveNumber\) + 0x6018}
        CACHE_RAM_CFG\($shaveNumber\)                             {\$SVU_DCU\($shaveNumber\) + 0x601C}
        CACHE_DATA_RAM_ACCESS\($shaveNumber\)                     {\$SVU_DCU\($shaveNumber\) + 0x7000}
        CACHE_TAG_RAM_ACCESS\($shaveNumber\)                      {\$SVU_DCU\($shaveNumber\) + 0xB000}
    "
    for {set regIndex 0} {$regIndex < 32} { incr regIndex } {
        mdbg::REG_DEFS "
            SVU_REG_I$regIndex\($shaveNumber\)                     {\$SVU_SLICE_ADDR\($shaveNumber\) + ( $regIndex * 4 ) + 0x1180 }
        "
        for {set vIndex 0} {$vIndex < 4} {incr vIndex} {
            mdbg::REG_DEFS  "
                SVU_REG_V${regIndex}_${vIndex}\($shaveNumber\)       {\$SVU_SLICE_ADDR\($shaveNumber\) + ( $regIndex * 16) + $vIndex * 4 + 0x1300 }
            "
        }
    }
}

namespace eval mdbg::myriad2 {

    proc getNumberOfShaves {socRev} {
        if {[string match "ma2?8?" $socRev]} {
            return 16
        }
        return 12
    }


    set CFG_NUM_SHAVES [getNumberOfShaves [mdbg::MV_SOC_REV]]
    for {set shaveNumber 0} {$shaveNumber < $CFG_NUM_SHAVES} {incr shaveNumber} {
        ::SetupShaveRegDefs $shaveNumber
    }

    rename ::SetupShaveRegDefs ""
    unset CFG_NUM_SHAVES
    unset shaveNumber
}

# FLAGS
namespace eval mdbg::flags {
    set OCR_TRACE_ENABLE        0x0800
    set OCR_IDC_128             0x0020
    set OCR_STOP_GO             0x0004
    set OCR_HOI                 0x0001

    set DCR_DBGE                0x001
    set DCR_HALT                0x080
}

# DDR
mdbg::REG_DEFS {
    DDR_BASE_ADR                                         {$DRAM_BASE_ADR ?? 0x80000000}
}

# MORE FLAGS
namespace eval mdbg::flags {
    set LEON_STARTUP_ALIGNMENT_MASK     0xFFF
    set LEON_IRQI_RUN                   [expr 1 << 0]
    set LEON_IRQI_RESET                 [expr 1 << 1]
}

namespace eval mdbg::flags {
    set LEON_RT_MSS_CLOCK_ENABLE    4
}

