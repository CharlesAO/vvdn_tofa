/*
 * Gaisler Research ethernet MAC driver
 * adapted from Opencores driver by Marko Isomaki
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 *
 *  Modified for Myriad2 by Movidius 2015
 */

#ifndef _GRETH_GBIT_H_
#define _GRETH_GBIT_H_

/* Read/Write to Phy functions that can be overrided if user needs to use I2C PHY */
typedef uint32_t (*rtems_greth_gbit_read_function )(uint32_t , uint32_t);
typedef void (*rtems_greth_gbit_write_function)(uint32_t, uint32_t, uint32_t);

/* Configuration Information */
typedef struct {
    void                 *base_address;     // Ethernet controller base address
    rtems_vector_number   vector;           // on LEON vector is IRQ no.
    uint32_t              txd_count;        //number of Tx descriptors
    uint32_t              rxd_count;        //number of Rx descriptors

    uint8_t irq_priority;
    uint8_t gbit_check;
    rtems_greth_gbit_write_function write; // MII write function, reg_addr is PHY register addr
    rtems_greth_gbit_read_function read;   // MII read function, reg_addr is PHY register addr
} greth_gbit_configuration_t;

/* Ethernet configuration registers */
typedef struct _greth_gbit_regs {
   volatile uint32_t ctrl;         /* Ctrl Register */
   volatile uint32_t status;       /* Status Register */
   volatile uint32_t mac_addr_msb; /* Bit 47-32 of MAC address */
   volatile uint32_t mac_addr_lsb; /* Bit 31-0 of MAC address */
   volatile uint32_t mdio_ctrl;    /* MDIO control and status */
   volatile uint32_t txdesc;       /* Transmit descriptor pointer */
   volatile uint32_t rxdesc;       /* Receive descriptor pointer */
} greth_gbit_regs;


/* Tx BD */
#define GRETH_GBIT_TXD_ENABLE      0x0800 /* Tx BD Enable */
#define GRETH_GBIT_TXD_WRAP        0x1000 /* Tx BD Wrap (last BD) */
#define GRETH_GBIT_TXD_IRQ         0x2000 /* Tx BD IRQ Enable */
#define GRETH_GBIT_TXD_MORE        0x20000 /* Tx BD More (more descs for packet) */
#define GRETH_GBIT_TXD_IPCS        0x40000 /* Tx BD insert ip chksum */
#define GRETH_GBIT_TXD_TCPCS       0x80000 /* Tx BD insert tcp chksum */
#define GRETH_GBIT_TXD_UDPCS       0x100000 /* Tx BD insert udp chksum */

#define GRETH_GBIT_TXD_UNDERRUN    0x4000 /* Tx BD Underrun Status */
#define GRETH_GBIT_TXD_RETLIM      0x8000 /* Tx BD Retransmission Limit Status */
#define GRETH_GBIT_TXD_LATECOL     0x10000 /* Tx BD Late Collision */

#define GRETH_GBIT_TXD_STATS  (GRETH_GBIT_TXD_UNDERRUN        | \
                               GRETH_GBIT_TXD_RETLIM          | \
                               GRETH_GBIT_TXD_LATECOL)

#define GRETH_GBIT_TXD_CS     (GRETH_GBIT_TXD_IPCS            | \
                               GRETH_GBIT_TXD_TCPCS           | \
                               GRETH_GBIT_TXD_UDPCS)

/* Rx BD */
#define GRETH_GBIT_RXD_ENABLE      0x0800 /* Rx BD Enable */
#define GRETH_GBIT_RXD_WRAP        0x1000 /* Rx BD Wrap (last BD) */
#define GRETH_GBIT_RXD_IRQ         0x2000 /* Rx BD IRQ Enable */

#define GRETH_GBIT_RXD_DRIBBLE     0x4000 /* Rx BD Dribble Nibble Status */
#define GRETH_GBIT_RXD_TOOLONG     0x8000 /* Rx BD Too Long Status */
#define GRETH_GBIT_RXD_CRCERR      0x10000 /* Rx BD CRC Error Status */
#define GRETH_GBIT_RXD_OVERRUN     0x20000 /* Rx BD Overrun Status */
#define GRETH_GBIT_RXD_LENERR      0x40000 /* Rx BD Length Error */
#define GRETH_GBIT_RXD_ID          0x40000 /* Rx BD IP Detected */
#define GRETH_GBIT_RXD_IR          0x40000 /* Rx BD IP Chksum Error */
#define GRETH_GBIT_RXD_UD          0x40000 /* Rx BD UDP Detected*/
#define GRETH_GBIT_RXD_UR          0x40000 /* Rx BD UDP Chksum Error */
#define GRETH_GBIT_RXD_TD          0x40000 /* Rx BD TCP Detected */
#define GRETH_GBIT_RXD_TR          0x40000 /* Rx BD TCP Chksum Error */


#define GRETH_GBIT_RXD_STATS       (GRETH_GBIT_RXD_OVERRUN | \
                                    GRETH_GBIT_RXD_DRIBBLE | \
                                    GRETH_GBIT_RXD_TOOLONG | \
                                    GRETH_GBIT_RXD_CRCERR)

/* CTRL Register */
#define GRETH_GBIT_CTRL_TXEN         0x00000001 /* Transmit Enable */
#define GRETH_GBIT_CTRL_RXEN         0x00000002 /* Receive Enable  */
#define GRETH_GBIT_CTRL_TXIRQ        0x00000004 /* Transmit Enable */
#define GRETH_GBIT_CTRL_RXIRQ        0x00000008 /* Receive Enable  */
#define GRETH_GBIT_CTRL_FULLD        0x00000010 /* Full Duplex */
#define GRETH_GBIT_CTRL_PRO          0x00000020 /* Promiscuous (receive all) */
#define GRETH_GBIT_CTRL_RST          0x00000040 /* Reset MAC */

/* Status Register */
#define GRETH_GBIT_STATUS_RXERR      0x00000001 /* Receive Error */
#define GRETH_GBIT_STATUS_TXERR      0x00000002 /* Transmit Error IRQ */
#define GRETH_GBIT_STATUS_RXIRQ      0x00000004 /* Receive Frame IRQ */
#define GRETH_GBIT_STATUS_TXIRQ      0x00000008 /* Transmit Error IRQ */
#define GRETH_GBIT_STATUS_RXAHBERR   0x00000010 /* Receiver AHB Error */
#define GRETH_GBIT_STATUS_TXAHBERR   0x00000020 /* Transmitter AHB Error */

/* MDIO Control  */
#define GRETH_GBIT_MDIO_WRITE        0x00000001 /* MDIO Write */
#define GRETH_GBIT_MDIO_READ         0x00000002 /* MDIO Read */
#define GRETH_GBIT_MDIO_LINKFAIL     0x00000004 /* MDIO Link failed */
#define GRETH_GBIT_MDIO_BUSY         0x00000008 /* MDIO Link Busy */
#define GRETH_GBIT_MDIO_REGADR       0x000007C0 /* Register Address */
#define GRETH_GBIT_MDIO_PHYADR       0x0000F800 /* PHY address */
#define GRETH_GBIT_MDIO_DATA         0xFFFF0000 /* MDIO DATA */

/* MII registers */
#define GRETH_GBIT_MII_EXTADV_1000FD 0x00000200
#define GRETH_GBIT_MII_EXTADV_1000HD 0x00000100
#define GRETH_GBIT_MII_EXTPRT_1000FD 0x00000800
#define GRETH_GBIT_MII_EXTPRT_1000HD 0x00000400

#define GRETH_GBIT_MII_100T4         0x00000200
#define GRETH_GBIT_MII_100TXFD       0x00000100
#define GRETH_GBIT_MII_100TXHD       0x00000080
#define GRETH_GBIT_MII_10FD          0x00000040
#define GRETH_GBIT_MII_10HD          0x00000020

/* PHY registers */
#define PHY_CONTROL_REG                 0
#define PHY_STATUS_REG                  1
#define PHY_IDENTIFIER1                 2
#define PHY_IDENTIFIER2                 3
#define PHY_AUTO_NEG_ADVERTISEMENT_REG  4
#define PHY_LINK_PARTNER_ABILITY_REG    5
#define PHY_1000BASET_CONTROL_REG       9
#define PHY_1000BASET_STATUS_REG        10
#define PHY_INTERRUPT_STATUS_REG        19

/* Default configuration parameters */
#define GRETH_GBIT_TX_DESCRIPTORS   32
#define GRETH_GBIT_RX_DESCRIPTORS   32
#define GRETH_GBIT_IRQ_PRIORITY     7

/* PHY data */
struct phy_device_info
{
   int vendor;
   int device;
   int rev;

   int adv;
   int part;

   int extadv;
   int extpart;
};


/* Utility structure to pass parameters to setup function */
typedef struct
{
    uint8_t priority;
    uint8_t txcount;
    uint8_t rxcount;
    uint8_t gbit_check;
    rtems_greth_gbit_read_function read_function;
    rtems_greth_gbit_write_function write_function;
} rtems_greth_gbit_hw_params;

/* Attach the GRETH driver to the system
 *
 *  @param[in] config - rtems_bsdnet_ifconfig structure
 *  @param[in] chip - MAC chip specifics
 *  @return    RTEMS_SUCCESSFUL on success, RTEMS_IO_ERROR on failure if the driver has already been attached
 */
int rtems_leon_greth_gbit_driver_attach(struct rtems_bsdnet_ifconfig* config, int attach);

/*  Configure GRETH driver
 *
 *  @param[in] params - Parameters to be used to set up the Ethernet Controller + PHY
 *  @return    RTEMS_SUCCESSFUL on success, RTEMS_IO_ERROR on failure if the driver has already been attached
 */
int rtems_leon_greth_gbit_driver_setup(rtems_greth_gbit_hw_params *params);
#endif //_GRETH_GBIT_H_

