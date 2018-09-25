// ---------------------------------------------------------------------------
// Myriad2 Register Definitions
// ---------------------------------------------------------------------------
#if __myriad2__ < 3
// Definitions for Myriad2.1 and Myriad2.2
#define CPR_BASE_ADR                    (0x10F00000)
#define CPR_DETECT_HW_SIM_REGISTER      (CPR_BASE_ADR + 0x00000114)

#define DW_UART0_BASE_ADDR              (0x20E00000)
#else
// Definitions for Myriad2.3
#define CPR_BASE_ADR                    (0x20300000)
#define CPR_DETECT_HW_SIM_REGISTER      (CPR_BASE_ADR + 0x0000011C)

#define DW_UART0_BASE_ADDR              (0x20180000)
#endif // __myriad2__

#define VCS_SIMULATION_MARKER           (1)

#define LHB_ROM_BASE_ADR                (0x7FF00000)
#define DEBUG_MSG_ADR                   (LHB_ROM_BASE_ADR + 0x00007FF8 + 0x00010000)

#define DW_UART0_USR_ADDR               (DW_UART0_BASE_ADDR + 0x7C)
#define DW_UART0_THR_ADDR               (DW_UART0_BASE_ADDR + 0x00)
#define DW_UART0_TFL_ADDR               (DW_UART0_BASE_ADDR + 0x80)
#define DW_UART0_RFL_ADDR               (DW_UART0_BASE_ADDR + 0x84)

#define DW_UART0_RX_FIFO_SIZE           (64)
#define DW_UART0_USR_TFNF               (1 << 1)


// ---------------------------------------------------------------------------
// Register access macros
// ---------------------------------------------------------------------------
#define READ_REGISTER_32(addr)          (*(volatile unsigned int *)(((unsigned int)(addr))))
#define WRITE_REGISTER_32(addr,value)   ((void)(*(volatile unsigned int *)(((unsigned int )(addr))) = (unsigned int)(value)))


// Raw character write routine for SHAVE's UART
void __attribute((weak, no_instrument_function)) _put_output_char(int ch)
{
    // In VCS we don't use the UART as a loopback adapter, we have a magic infinite depth register to handle putchar quickly
    if ((( READ_REGISTER_32 ( CPR_DETECT_HW_SIM_REGISTER ) >> 29 ) & 0x7 ) == VCS_SIMULATION_MARKER )
        WRITE_REGISTER_32 ( DEBUG_MSG_ADR, ch & 0xFF );
    else
    {   // Check that there is space in the TX FIFO */
        while (( READ_REGISTER_32 ( DW_UART0_USR_ADDR ) & DW_UART0_USR_TFNF ) == 0 )
            ;   // Wait until the FIFO is not full

        // Assumes the UART is configured in loopback mode.  Checking that
        // there is enough free space.  Limitation: if RX FIFO is full, sent
        // bytes from TX will be lost.
        while (( READ_REGISTER_32 ( DW_UART0_TFL_ADDR ) + READ_REGISTER_32 ( DW_UART0_RFL_ADDR ) + 2 ) >= DW_UART0_RX_FIFO_SIZE )
            ;   // Additional check for default case of loopback mode

        WRITE_REGISTER_32 ( DW_UART0_THR_ADDR, ch & 0xFF );
    }
}