mdbg::REG_DEFS {
    EDLAR_UNLOCK_CODE         0xC5ACCE55
}

mdbg::REG_DEFS {
    ARM_ROMPIDR0_OFFS         0xFE0
    ARM_ROMPIDR1_OFFS         0xFE4
    ARM_ROMPIDR2_OFFS         0xFE8
    ARM_ROMPIDR3_OFFS         0xFEC

    ARM_ROMPIDR4_OFFS         0xFD0
    ARM_ROMPIDR5_OFFS         0xFD4
    ARM_ROMPIDR6_OFFS         0xFD8
    ARM_ROMPIDR7_OFFS         0xFDC

    ARM_ROMCIDR0_OFFS         0xFF0
    ARM_ROMCIDR1_OFFS         0xFF4
    ARM_ROMCIDR2_OFFS         0xFF8
    ARM_ROMCIDR3_OFFS         0xFFC
}

mdbg::REG_DEFS {
    ARM_ROMENTRY0_OFFS        0x000
    ARM_ROMENTRY1_OFFS        0x004
    ARM_ROMENTRY2_OFFS        0x008
    ARM_ROMENTRY3_OFFS        0x00C
    ARM_ROMENTRY4_OFFS        0x010
    ARM_ROMENTRY5_OFFS        0x014
    ARM_ROMENTRY6_OFFS        0x018
    ARM_ROMENTRY7_OFFS        0x01C
    ARM_ROMENTRY8_OFFS        0x020
    ARM_ROMENTRY9_OFFS        0x024
    ARM_ROMENTRY10_OFFS       0x028
    ARM_ROMENTRY11_OFFS       0x02C
    ARM_ROMENTRY12_OFFS       0x030
    ARM_ROMENTRY13_OFFS       0x034
    ARM_ROMENTRY14_OFFS       0x038
    ARM_ROMENTRY15_OFFS       0x03C
}

mdbg::REG_DEFS {
    # ARMv7 memory map - not supported
    ARM_DBG_ROMPIDR0_MEM_MAP_V7_VALUE   0xA3
    # ARMv8 debug memory map
    ARM_DBG_ROMPIDR0_MEM_MAP_V8_VALUE   0xA1
}

mdbg::REG_DEFS {

    ARM_DBG_ROMENTRY_CORE0_DBG_PRESENT  0x010003
    ARM_DBG_ROMENTRY_CORE0_CTI_PRESENT  0x020003
    ARM_DBG_ROMENTRY_CORE0_PMU_PRESENT  0x030003
    ARM_DBG_ROMENTRY_CORE0_ETM_PRESENT  0x040003

    ARM_DBG_ROMENTRY_CORE1_DBG_PRESENT  0x110003
    ARM_DBG_ROMENTRY_CORE1_CTI_PRESENT  0x120003
    ARM_DBG_ROMENTRY_CORE1_PMU_PRESENT  0x130003
    ARM_DBG_ROMENTRY_CORE1_ETM_PRESENT  0x140003

    ARM_DBG_ROMENTRY_CORE2_DBG_PRESENT  0x210003
    ARM_DBG_ROMENTRY_CORE2_CTI_PRESENT  0x220003
    ARM_DBG_ROMENTRY_CORE2_PMU_PRESENT  0x230003
    ARM_DBG_ROMENTRY_CORE2_ETM_PRESENT  0x240003

    ARM_DBG_ROMENTRY_CORE3_DBG_PRESENT  0x310003
    ARM_DBG_ROMENTRY_CORE3_CTI_PRESENT  0x320003
    ARM_DBG_ROMENTRY_CORE3_PMU_PRESENT  0x330003
    ARM_DBG_ROMENTRY_CORE3_ETM_PRESENT  0x340003
}

mdbg::REG_DEFS {
    # External Debug Event Status Register
    ARM_CPU_DBG_EDESR_OFFS                  0x020
    # External Debug Execution Control Register
    ARM_CPU_DBG_EDECR_OFFS                  0x024
    # External Debug Watchpoint Address Register (64-bit, RO)
    ARM_CPU_DBG_EDWAR_OFFS                  0x030

    # Debug Data Transfer Register, Receive
    ARM_CPU_DBG_DBGDTRRX_EL0_OFFS           0x080
    # External Debug Instruction Transfer Register (WO)
    ARM_CPU_DBG_EDITR_OFFS                  0x084
    # External Debug Status and Control Register
    ARM_CPU_DBG_EDSCR_OFFS                  0x088
    # Debug Data Transfer Register, Transmit
    ARM_CPU_DBG_DBGDTRTX_EL0_OFFS           0x08C
    # External Debug Reserve Control Register (WO)
    ARM_CPU_DBG_EDRCR_OFFS                  0x090
    # External Debug Auxiliary Control Register
    ARM_CPU_DBG_EDACR_OFFS                  0x094
    # Reserved                              0x09C
    # External Debug Exception Catch Control Register
    ARM_CPU_DBG_EDECCR_OFFS                 0x098
    # External Debug Program Counter Sample Register, low word
    ARM_CPU_DBG_EDPCSR_LO_OFFS              0x0A0
    # External Debug Context ID Sample Register
    ARM_CPU_DBG_EDCIDSR_OFFS                0x0A4
    # External Debug Virtual Context Sample Register
    ARM_CPU_DBG_EDVIDSR_OFFS                0x0A8
    # External Debug Program Counter Sample Register, high word
    ARM_CPU_DBG_EDPCSR_HI_OFFS              0x0AC
    # Reserved                              0x0B0-0x2FC
    # OS Lock Access Register
    ARM_CPU_DBG_OSLAR_EL1_OFFS              0x300
    # Reserved                              0x304-0x30C
    # External Debug Power/Reset Control Register
    ARM_CPU_DBG_EDPRCR_OFFS                 0x310
    # External Debug Processor Status Register
    ARM_CPU_DBG_EDPRSR_OFFS                 0x314
    # Reserved                              0x318-0x3FC
    # Debug Breakpoint Value Register 0 (64-bit)
    ARM_CPU_DBG_DBGBVR0_EL1_OFFS            0x400
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGBCR0_EL1_OFFS            0x408
    # Debug Breakpoint Value Register 1 (64-bit)
    ARM_CPU_DBG_DBGBVR1_EL1_OFFS            0x410
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGBCR1_EL1_OFFS            0x418
    # Debug Breakpoint Value Register 2 (64-bit)
    ARM_CPU_DBG_DBGBVR2_EL1_OFFS            0x420
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGBCR2_EL1_OFFS            0x428
    # Debug Breakpoint Value Register 3 (64-bit)
    ARM_CPU_DBG_DBGBVR3_EL1_OFFS            0x430
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGBCR3_EL1_OFFS            0x438
    # Debug Breakpoint Value Register 4 (64-bit)
    ARM_CPU_DBG_DBGBVR4_EL1_OFFS            0x440
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGBCR4_EL1_OFFS            0x448
    # Debug Breakpoint Value Register 5 (64-bit)
    ARM_CPU_DBG_DBGBVR5_EL1_OFFS            0x450
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGBCR5_EL1_OFFS            0x458
    # Reserved                              0x45C-0x7FC
    # Debug Watchpoint Value Register 0 (64-bit)
    ARM_CPU_DBG_DBGWVR0_EL1_OFFS            0x800
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGWCR0_EL1_OFFS            0x808
    # Debug Watchpoint Value Register 1 (64-bit)
    ARM_CPU_DBG_DBGWVR1_EL1_OFFS            0x810
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGWCR1_EL1_OFFS            0x818
    # Debug Watchpoint Value Register 2 (64-bit)
    ARM_CPU_DBG_DBGWVR2_EL1_OFFS            0x820
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGWCR2_EL1_OFFS            0x828
    # Debug Watchpoint Value Register 3 (64-bit)
    ARM_CPU_DBG_DBGWVR3_EL1_OFFS            0x830
    # Debug Breakpoint Control Registers, EL1
    ARM_CPU_DBG_DBGWCR3_EL1_OFFS            0x838
    # Reserved                              0x83C-0xCFC
    # Main ID Register, EL1
    ARM_CPU_DBG_MIDR_EL1_OFFS               0xD00
    # Reserved                              0xD04-0xD1C
    # ...
    # External Debug Integration Mode Control Register
    ARM_CPU_DBG_EDITCTRL_OFFS               0xF00
    # Reserved                              0xF04-0xF9C
    # Debug Claim Tag Set register
    ARM_CPU_DBG_DBGCLAIMSET_EL1_OFFS        0xFA0
    # Debug Claim Tag Clear Register
    ARM_CPU_DBG_DBGCLAIMCLR_EL1_OFFS        0xFA4
    # Multiprocessor Affinity Register
    ARM_CPU_DBG_EDDEVAFF0_OFFS              0xFA8
    # External Debug Device Affinity
    ARM_CPU_DBG_EDDEVAFF1_OFFS              0xFAC
    # External Debug Lock Access Register
    ARM_CPU_DBG_EDLAR_OFFS                  0xFB0
    # External Debug Lock Status Register
    ARM_CPU_DBG_EDLSR_OFFS                  0xFB4
    # Debug Authentication Status Register
    ARM_CPU_DBG_DBGAUTHSTATUS_EL1_OFFS      0xFB8
    # External Debug Device Architecture Register
    ARM_CPU_DBG_EDDEVARCH_OFFS              0xFBC
    # External Debug Device ID Register 2, RES 0
    ARM_CPU_DBG_EDDEVID2_OFFS               0xFC0
    # External Debug Device ID Register 1
    ARM_CPU_DBG_EDDEVID1_OFFS               0xFC4
    # External Debug Device ID Register 0
    ARM_CPU_DBG_EDDEVID_OFFS                0xFC8
    # External Debug Device Type Register
    ARM_CPU_DBG_EDDEVTYPE_OFFS              0xFCC
}
