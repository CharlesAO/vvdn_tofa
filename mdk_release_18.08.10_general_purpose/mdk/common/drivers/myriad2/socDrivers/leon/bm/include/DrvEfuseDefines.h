///
/// @file DrvEfuseDefines.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvEfuse
/// @{
/// @brief     Macros for EFuse field ranges and values
///


#ifndef DRV_EFUSE_DEFINES_H
#define DRV_EFUSE_DEFINES_H

/* EFuse field start bit indices */
#define EFUSE_BOOTIFCFG_START       0
#define EFUSE_BOOTPLLCFG_START      64
#define EFUSE_BOOTIFSEL_START       80
#define EFUSE_BOOTSYSDIV_START      84
#define EFUSE_PADOVERRIDE_START     95
#define EFUSE_PADORSLEW_START       96
#define EFUSE_PADDRIVE_START        97
#define EFUSE_SECURITY_CODE_START   100     // MA2155 only
#define EFUSE_AES_KEY_START         128     // MA2155 only
#define EFUSE_ECC_PUBKEY_START      256     // MA2155 only
#define EFUSE_CUSTOMER_USE_START    640

/* EFuse field end bit indices */
#define EFUSE_BOOTIFCFG_END         63
#define EFUSE_BOOTPLLCFG_END        79
#define EFUSE_BOOTIFSEL_END         83
#define EFUSE_BOOTSYSDIV_END        86
#define EFUSE_PADOVERRIDE_END       95
#define EFUSE_PADORSLEW_END         96
#define EFUSE_PADDRIVE_END          98
#define EFUSE_SECURITY_CODE_END     115     // MA2155 only
#define EFUSE_AES_KEY_END           255     // MA2155 only
#define EFUSE_ECC_PUBKEY_END        511     // MA2155 only
#define EFUSE_CUSTOMER_USE_END      1023

/* IFSEL values */
#define EFUSE_IFSEL_UART            0x1     //         MA2150, MA2155
#define EFUSE_IFSEL_I2CS            0x3     //         MA2150, MA2155
#define EFUSE_IFSEL_SPIM            0x4     //         MA2150, MA2155
#define EFUSE_IFSEL_SPIS            0x5     //         MA2150, MA2155
#define EFUSE_IFSEL_SPIME           0x7     //         MA2150, MA2155
#define EFUSE_IFSEL_I2CME           0x8     //         MA2150, MA2155
#define EFUSE_IFSEL_USBD            0xA     //         MA2150, MA2155
#define EFUSE_IFSEL_RAM             0xD     //         MA2150, MA2155

/* SECURITY_VALUE values (MA2155 only) */
#define EFUSE_SECURITY_DEFAULT      0x0000
#define EFUSE_SECURITY_SECURE       0x2809
#define EFUSE_SECURITY_MA2150       0xd424

/* PLLCFG calculation macro and pre-calculated values for GPIO Boot frequency ranges */
#define EFUSE_PLLCFG_CTRL0(x)       ((x) >> 2)      // calculates the PLLCFG value given a CPR_PLL_CTRL0 value
#define EFUSE_PLLCFG_RANGE_B        0x4808
#define EFUSE_PLLCFG_RANGE_C        0x360c
#define EFUSE_PLLCFG_RANGE_D        0x7e1c

/* PADDRIVE values */
#define EFUSE_PADDRIVE_2mA          0
#define EFUSE_PADDRIVE_4mA          1
#define EFUSE_PADDRIVE_8mA          2
#define EFUSE_PADDRIVE_12mA         3

/* byte sizes for AES and ECC keys */
#define EFUSE_SIZEOF_AES_KEY        EFUSE_FIELD_SIZEOF(EFUSE_AES_KEY_START, EFUSE_AES_KEY_END)
#define EFUSE_SIZEOF_ECC_PUBKEY     EFUSE_FIELD_SIZEOF(EFUSE_ECC_PUBKEY_START, EFUSE_ECC_PUBKEY_END)


/* Macros to build IFCFG values */

#define EFUSE_IFCFG_FIELD(val, endBit, startBit)    \
    ((((u64)(val)) & ((1ULL << (endBit - startBit + 1)) - 1)) << startBit)

/* SPIM, SPIME and SPIS */
#define EFUSE_IFCFG_SPI_PINSELECT(x)    EFUSE_IFCFG_FIELD(x, 2, 0)
#define EFUSE_IFCFG_SPI_MODE(x)         EFUSE_IFCFG_FIELD(x, 4, 3)

/* SPIM and SPIME */
#define EFUSE_IFCFG_SPIM_CS(x)          EFUSE_IFCFG_FIELD(x, 6, 5)
#define EFUSE_IFCFG_SPIM_ADDRMODE(x)    EFUSE_IFCFG_FIELD(x, 8, 7)
#define EFUSE_IFCFG_SPIM_EEPROM_OFF(x)  (EFUSE_IFCFG_FIELD(x, 23, 9) | EFUSE_IFCFG_FIELD((x >> 15), 63, 56))
#define EFUSE_IFCFG_SPIM_BAUDREG(x)     EFUSE_IFCFG_FIELD(x, 39, 24)
#define EFUSE_IFCFG_SPIM_RXDELAY(x)     EFUSE_IFCFG_FIELD(x, 55, 53)

/* SPIME only */
#define EFUSE_IFCFG_SPIME_READCMD(x)    EFUSE_IFCFG_FIELD(x, 47, 40)
#define EFUSE_IFCFG_SPIME_DUMMYCLKS(x)  EFUSE_IFCFG_FIELD(x, 52, 48)

/* SPIS only */
#define EFUSE_IFCFG_SPIS_TXRX           (0 << 8)
#define EFUSE_IFCFG_SPIS_RECVONLY       (1 << 8)

/* I2CME and I2CS */
#define EFUSE_IFCFG_I2C_PINSELECT(x)    EFUSE_IFCFG_FIELD(x, 2, 0)
#define EFUSE_IFCFG_I2C_SLAVEADDR(x)    EFUSE_IFCFG_FIELD(x, 9, 3)

/* I2CME only */
#define EFUSE_IFCFG_I2CME_SCL_HCNT(x)   EFUSE_IFCFG_FIELD(x, 22, 10)
#define EFUSE_IFCFG_I2CME_SCL_LCNT(x)   EFUSE_IFCFG_FIELD(x, 35, 23)
#define EFUSE_IFCFG_I2CME_TX_HOLD(x)    EFUSE_IFCFG_FIELD(x, 47, 36)
#define EFUSE_IFCFG_I2CME_BOOTADDR(x)   EFUSE_IFCFG_FIELD(x, 49, 48)
#define EFUSE_IFCFG_I2CME_RX_HOLD(x)    EFUSE_IFCFG_FIELD(x, 55, 50)
#define EFUSE_IFCFG_I2CME_SPKLEN(x)     EFUSE_IFCFG_FIELD(x, 63, 56)

/* I2CS only */
#define EFUSE_IFCFG_I2CS_SDA_SETUP(x)   EFUSE_IFCFG_FIELD(x, 17, 10)
#define EFUSE_IFCFG_I2CS_RX_HOLD(x)     EFUSE_IFCFG_FIELD(x, 31, 24)
#define EFUSE_IFCFG_I2CS_TX_HOLD(x)     EFUSE_IFCFG_FIELD(x, 47, 32)
#define EFUSE_IFCFG_I2CS_SPKLEN(x)      EFUSE_IFCFG_FIELD(x, 55, 48)

/* UART */
#define EFUSE_IFCFG_UART_AUX_DENOM(x)   EFUSE_IFCFG_FIELD(x, 11, 0)
#define EFUSE_IFCFG_UART_PINSELECT(x)   EFUSE_IFCFG_FIELD(x, 13, 12)
#define EFUSE_IFCFG_UART_PARITY(x)      EFUSE_IFCFG_FIELD(x, 15, 14)
#define EFUSE_IFCFG_UART_AUX_NUM(x)     EFUSE_IFCFG_FIELD(x, 26, 16)
#define EFUSE_IFCFG_UART_HSHAKE_DIS     (0 << 27)
#define EFUSE_IFCFG_UART_HSHAKE_EN      (1 << 27)
#define EFUSE_IFCFG_UART_AUX_CLKSEL(x)  EFUSE_IFCFG_FIELD(x, 30, 28)
#define EFUSE_IFCFG_UART_STOPBITS_1     (0 << 31)
#define EFUSE_IFCFG_UART_STOPBITS_2     (1 << 31)
#define EFUSE_IFCFG_UART_DIVLATCH(x)    EFUSE_IFCFG_FIELD(x, 47, 32)

#define EFUSE_IFCFG_UART_PARITY_NONE    0
#define EFUSE_IFCFG_UART_PARITY_ODD     1
#define EFUSE_IFCFG_UART_PARITY_EVEN    2

/* USBD */
#define EFUSE_IFCFG_USBD_AUX24_DENOM(x) EFUSE_IFCFG_FIELD(x, 9, 0)
#define EFUSE_IFCFG_USBD_FSEL(x)        EFUSE_IFCFG_FIELD(x, 15, 10)
#define EFUSE_IFCFG_USBD_AUX24_NUM(x)   EFUSE_IFCFG_FIELD(x, 24, 16)
#define EFUSE_IFCFG_USBD_MODS_EN        (0 << 25)
#define EFUSE_IFCFG_USBD_MODS_DIS       (1 << 25)
#define EFUSE_IFCFG_USBD_CLKSEL_AUX24   (0 << 26)
#define EFUSE_IFCFG_USBD_CLKSEL_EXT     (1 << 26)
#define EFUSE_IFCFG_USBD_CLKSEL_AUX14   (2 << 26)
#define EFUSE_IFCFG_USBD_AUX24_CLKSEL(x) EFUSE_IFCFG_FIELD(x, 30, 28)
#define EFUSE_IFCFG_USBD_STR_MV         (0 << 31)
#define EFUSE_IFCFG_USBD_STR_BLANK      (1 << 31)
#define EFUSE_IFCFG_USBD_VID(x)         EFUSE_IFCFG_FIELD(x, 47, 32)
#define EFUSE_IFCFG_USBD_PID(x)         EFUSE_IFCFG_FIELD(x, 63, 48)

/* used for UART EFUSE_IFCFG_UART_AUX_CLKSEL and USBD EFUSE_IFCFG_USBD_AUX_CLKSEL */
#define EFUSE_IFCFG_CLKSEL_PLL0         0
#define EFUSE_IFCFG_CLKSEL_PLL1         1
#define EFUSE_IFCFG_CLKSEL_REFCLK0      2
#define EFUSE_IFCFG_CLKSEL_REFCLK1      3
#define EFUSE_IFCFG_CLKSEL_32KHZ        4
#define EFUSE_IFCFG_CLKSEL_AUX28        5
#define EFUSE_IFCFG_CLKSEL_SYSCLK       6
#define EFUSE_IFCFG_CLKSEL_SLOWSYSCLK   7

#endif // DRV_EFUSE_DEFINES_H

///@}
