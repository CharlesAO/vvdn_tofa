///
/// @file OsDrvUsbPhyDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvUsbPhy
/// @{
/// @brief     USB PHY defines
///


#ifndef _OSDRVUSBPHY_DEFINES_H_    /* prevent multiple includes */
#define _OSDRVUSBPHY_DEFINES_H_

#include "mv_types.h"

// 1: Defines
// ----------------------------------------------------------------------------

#define USB_PHY_OTG_DISABLED  0
#define USB_PHY_OTG_ENABLED   1
#define USB_PHY_USE_EXT_CLK   1
#define USB_PHY_HS_ONLY_OFF   0
#define USB_PHY_HS_ONLY_ON    1

// USB PHY register definitions

// USB_PHY_SSC_CTRL_ADR
#define USB_PHY_SSC_CTRL_EN                 (1u << 13)
#define USB_PHY_SSC_CTRL_SSC_RANGE(x)       (((x) & 0x07) << 10)
#define USB_PHY_SSC_CTRL_SSC_RANGE_MASK     (0x07u << 10)
#define USB_PHY_SSC_CTRL_SSC_RANGE_BIT      (10)
#define USB_PHY_SSC_CTRL_REFCLK_SEL(x)      (((x) & 0x1f) << 0)
#define USB_PHY_SSC_CTRL_REFCLK_SEL_MASK    (0x1fu << 0)
#define USB_PHY_SSC_CTRL_REFCLK_SEL_BIT     (0)


// USB_PHY_OTG_CTRL_ADR
#define USB_PHY_OTG_CTRL_OTGDISABLE0        (1 << 0)
#define USB_PHY_OTG_CTRL_VBUSVLDEXTSEL0     (1 << 1)
#define USB_PHY_OTG_CTRL_VBUSVLDEXT0        (1 << 2)
#define USB_PHY_OTG_CTRL_DRVVBUS0           (1 << 3)
#define USB_PHY_OTG_CTRL_IDPULLUP0          (1 << 4)
#define USB_PHY_OTG_CTRL_ADPCHRG0           (1 << 5)
#define USB_PHY_OTG_CTRL_ADPDISCHRG0        (1 << 6)
#define USB_PHY_OTG_CTRL_ADPPRBENB0         (1 << 7)

// USB_PHY_CFG0_ADR
#define USB_PHY_CFG0_PHY_RESET              (1u << 31)
#define USB_PHY_CFG0_LANE0_EXT_PCLK_REQ     (1u << 29)
#define USB_PHY_CFG0_LANE0_TX2RX_LOOPBK     (1u << 28)
#define USB_PHY_CFG0_ALT_CLK_SEL            (1u << 27)
#define USB_PHY_CFG0_ALT_CLK_EN             (1u << 26)
#define USB_PHY_CFG0_COMMONONN              (1u << 24)
#define USB_PHY_CFG0_RETENABLEN             (1u << 23)
#define USB_PHY_CFG0_REFCLKSEL(x)           (((x) & 0x03) << 21)
#define USB_PHY_CFG0_REFCLKSEL_MASK         (0x03u << 21)
#define USB_PHY_CFG0_REFCLKSEL_BIT          (21)
#define USB_PHY_CFG0_REF_USE_PAD            (1u << 19)
#define USB_PHY_CFG0_REF_SSP_EN             (1u << 18)
#define USB_PHY_CFG0_REF_CLKDIV2            (1u << 17)
#define USB_PHY_CFG0_MPLL_REFSSC_CLK_EN     (1u << 16)
#define USB_PHY_CFG0_MPLL_MULTIPLIER(x)     (((x) & 0x7f) << 8)
#define USB_PHY_CFG0_MPLL_MULTIPLIER_MASK   (0x7fu << 8)
#define USB_PHY_CFG0_MPLL_MULTIPLIER_BIT    (8)
#define USB_PHY_CFG0_FSEL(x)                (((x) & 0x3f) << 0)
#define USB_PHY_CFG0_FSEL_MASK              (0x3fu << 0)
#define USB_PHY_CFG0_FSEL_BIT               (0)

// USB_PHY_CFG1_ADR
#define USB_PHY_CFG1_TXVREFTUNE0(x)        (((x) & 0x0f) << 26)
#define USB_PHY_CFG1_TXVREFTUNE0_MASK      (0x0f << 26)
#define USB_PHY_CFG1_TXVREFTUNE0_BIT       (26)
#define USB_PHY_CFG1_TXRISETUNE0(x)        (((x) & 0x03) << 24)
#define USB_PHY_CFG1_TXRISETUNE0_MASK      (0x03 << 24)
#define USB_PHY_CFG1_TXRISETUNE0_BIT       (24)
#define USB_PHY_CFG1_TXRESTUNE0(x)         (((x) & 0x03) << 22)
#define USB_PHY_CFG1_TXRESTUNE0_MASK       (0x03 << 22)
#define USB_PHY_CFG1_TXRESTUNE0_BIT        (22)
#define USB_PHY_CFG1_TXPREEMPPULSETUNE0    (1 << 21)
#define USB_PHY_CFG1_TXPREEMPAMPTUNE0(x)   (((x) & 0x03) << 19)
#define USB_PHY_CFG1_TXPREEMPAMPTUNE0_MASK (0x03 << 19)
#define USB_PHY_CFG1_TXPREEMPAMPTUNE0_BIT  (19)
#define USB_PHY_CFG1_TXHSXVTUNE0(x)        (((x) & 0x03) << 17)
#define USB_PHY_CFG1_TXHSXVTUNE0_MASK      (0x03 << 17)
#define USB_PHY_CFG1_TXHSXVTUNE0_BIT       (17)
#define USB_PHY_CFG1_TXFSLSTUNE0(x)        (((x) & 0x0f) << 13)
#define USB_PHY_CFG1_TXFSLSTUNE0_MASK      (0x0f << 13)
#define USB_PHY_CFG1_TXFSLSTUNE0_BIT       (13)
#define USB_PHY_CFG1_SQRXTUNE0(x)          (((x) & 0x07) << 10)
#define USB_PHY_CFG1_SQRXTUNE0_MASK        (0x07 << 10)
#define USB_PHY_CFG1_SQRXTUNE0_BIT         (10)
#define USB_PHY_CFG1_OTGTUNE0(x)           (((x) & 0x07) << 7)
#define USB_PHY_CFG1_OTGTUNE0_MASK         (0x07 << 7)
#define USB_PHY_CFG1_OTGTUNE0_BIT          (7)
#define USB_PHY_CFG1_COMPDISTUNE0(x)       (((x) & 0x07) << 4)
#define USB_PHY_CFG1_COMPDISTUNE0_MASK     (0x07 << 4)
#define USB_PHY_CFG1_COMPDISTUNE0_BIT      (4)
#define USB_PHY_CFG1_RTUNE_REQ             (1 << 3)
#define USB_PHY_CFG1_XBITSTUFFENH0         (1 << 1)
#define USB_PHY_CFG1_TXBITSTUFFEN0         (1 << 0)

// default configuration for register USB_PHY_CFG1_ADR (Bug22285)
#define USB_PHY_CFG1_DEFAULT_VAL       \
    ( USB_PHY_CFG1_TXVREFTUNE0(3)    | \
    USB_PHY_CFG1_TXRISETUNE0(0)      | \
    USB_PHY_CFG1_TXRESTUNE0(1)       | \
    USB_PHY_CFG1_TXPREEMPAMPTUNE0(2) | \
    USB_PHY_CFG1_TXHSXVTUNE0(3)      | \
    USB_PHY_CFG1_TXFSLSTUNE0(7)      | \
    USB_PHY_CFG1_SQRXTUNE0(3)        | \
    USB_PHY_CFG1_OTGTUNE0(3)         | \
    USB_PHY_CFG1_COMPDISTUNE0(4))

// USB_PHY_CFG2_ADR
#define USB_PHY_CFG2_PCS_TX_DEEMPH_6DB(x)      (((x) & 0x3f) << 24)
#define USB_PHY_CFG2_PCS_TX_DEEMPH_6DB_MASK    (0x3f << 24)
#define USB_PHY_CFG2_PCS_TX_DEEMPH_6DB_BIT     (24)
#define USB_PHY_CFG2_PCS_TX_DEEMPH_3P5DB(x)    (((x) & 0x3f) << 18)
#define USB_PHY_CFG2_PCS_TX_DEEMPH_3P5DB_MASK  (0x3f << 18)
#define USB_PHY_CFG2_PCS_TX_DEEMPH_3P5DB_BIT   (18)
#define USB_PHY_CFG2_PCS_RX_LOS_MASK_VAL(x)    (((x) & 0x3ff) << 8)
#define USB_PHY_CFG2_PCS_RX_LOS_MASK_VAL_MASK  (0x3ff << 8)
#define USB_PHY_CFG2_PCS_RX_LOS_MASK_VAL_BIT   (8)
#define USB_PHY_CFG2_LOS_LEVEL(x)              (((x) & 0x1f) << 3)
#define USB_PHY_CFG2_LOS_LEVEL_MASK            (0x1f << 3)
#define USB_PHY_CFG2_LOS_LEVEL_BIT             (3)
#define USB_PHY_CFG2_LOS_BIAS(x)               (((x) & 0x7) << 0)
#define USB_PHY_CFG2_LOS_BIAS_MASK             (0x7 << 0)
#define USB_PHY_CFG2_LOS_BIAS_BIT              (0)

// USB_PHY_CFG3_ADR
#define USB_PHY_CFG3_ACJT_LEVEL(x)             (((x) & 0x1f) << 15)
#define USB_PHY_CFG3_ACJT_LEVEL_MASK           (0x1f << 15)
#define USB_PHY_CFG3_ACJT_LEVEL_BIT            (15)
#define USB_PHY_CFG3_TX_VBOOST_LVL(x)          (((x) & 0x7) << 12)
#define USB_PHY_CFG3_TX_VBOOST_LVL_MASK        (0x7 << 12)
#define USB_PHY_CFG3_TX_VBOOST_LVL_BIT         (12)
#define USB_PHY_CFG3_LANE0_TX_TERM_OFFSET(x)   (((x) & 0x1f) << 7)
#define USB_PHY_CFG3_LANE0_TX_TERM_OFFSET_MASK (0x1f << 7)
#define USB_PHY_CFG3_LANE0_TX_TERM_OFFSET_BIT  (7)
#define USB_PHY_CFG3_PCS_TX_SWING_FULL(x)      (((x) & 0xff) << 0)
#define USB_PHY_CFG3_PCS_TX_SWING_FULL_MASK    (0xff << 0)
#define USB_PHY_CFG3_PCS_TX_SWING_FULL_BIT     (0)

// USB_CTRL_CFG0
#define USB_CTRL_CFG0_CTRL_RESET             (1u << 31)
#define USB_CTRL_CFG0_AHB_MSTR_HLOCK         (1u << 20)
#define USB_CTRL_CFG0_BUS_FLT_BYP(x)         (((x) & 0x0f) << 16)
#define USB_CTRL_CFG0_BUS_FLT_BYP_MASK       (0x0fu << 16)
#define USB_CTRL_CFG0_BUS_FLT_BYP_BIT        (16)
#define USB_CTRL_CFG0_BIGENDIAN_GS           (1u << 15)
#define USB_CTRL_CFG0_HUBPRT_OVRCNT(x)       (((x) & 0x03) << 13)
#define USB_CTRL_CFG0_HUBPRT_OVRCNT_MASK     (0x03u << 13)
#define USB_CTRL_CFG0_HUBPRT_OVRCNT_BIT      (13)
#define USB_CTRL_CFG0_HUBPRT_PRM_ATCH(x)     (((x) & 0x03) << 11)
#define USB_CTRL_CFG0_HUBPRT_PRM_ATCH_MASK   (0x03u << 11)
#define USB_CTRL_CFG0_HUBPRT_PRM_ATCH_BIT    (11)
#define USB_CTRL_CFG0_HOST_U3_PORT_DIS       (1u << 10)
#define USB_CTRL_CFG0_HOST_U2_PORT_DIS       (1u <<  9)
#define USB_CTRL_CFG0_H_POW_CTRL             (1u <<  8)
#define USB_CTRL_CFG0_HOST_MSI_EN            (1u <<  7)
#define USB_CTRL_CFG0_FLADJ30MHZ(x)          (((x) & 0x3f) << 1)
#define USB_CTRL_CFG0_FLADJ30MHZ_MASK        (0x3fu << 1)
#define USB_CTRL_CFG0_FLADJ30MHZ_BIT         (1)
#define USB_CTRL_CFG0_PME_EN                 (1u << 0)

// USB_GUSB2PHYCFG_REGS
#define USB_GUSB2PHYCFG_PHYSOFTRST                (1u << 31)
#define USB_GUSB2PHYCFG_U2_FREECLK_EXISTS         (1u << 30)
#define USB_GUSB2PHYCFG_ULPI_LPM_WITH_OPMODE_CHK  (1u << 29)
#define USB_GUSB2PHYCFG_HSIC_CON_WIDTH_ADJ(x)     (((x) & 0x03) << 27)
#define USB_GUSB2PHYCFG_HSIC_CON_WIDTH_ADJ_MASK   (0x03u << 27)
#define USB_GUSB2PHYCFG_HSIC_CON_WIDTH_ADJ_BIT    (27)
#define USB_GUSB2PHYCFG_INV_SEL_HSIC              (1u << 26)
#define USB_GUSB2PHYCFG_LSTRD(x)                  (((x) & 0x07) << 22)
#define USB_GUSB2PHYCFG_LSTRD_MASK                (0x07u << 22)
#define USB_GUSB2PHYCFG_LSTRD_BIT                 (22)
#define USB_GUSB2PHYCFG_LSIPD(x)                  (((x) & 0x07) << 19)
#define USB_GUSB2PHYCFG_LSIPD_MASK                (0x07u << 19)
#define USB_GUSB2PHYCFG_LSIPD_BIT                 (19)
#define USB_GUSB2PHYCFG_ULPIEXTVBUSINDIACTOR      (1u << 18)
#define USB_GUSB2PHYCFG_ULPIEXTVBUSDRV            (1u << 17)
#define USB_GUSB2PHYCFG_ULPIAUTORES               (1u << 15)
#define USB_GUSB2PHYCFG_USBTRDTIM(x)              (((x) & 0x0f) << 10)
#define USB_GUSB2PHYCFG_USBTRDTIM_MASK            (0x0fu << 10)
#define USB_GUSB2PHYCFG_USBTRDTIM_BIT             (10)
#define USB_GUSB2PHYCFG_XCVRDLY                   (1u << 9)
#define USB_GUSB2PHYCFG_ENBLSLPM                  (1u << 8)
#define USB_GUSB2PHYCFG_PHYSEL                    (1u << 7)
#define USB_GUSB2PHYCFG_SUSPENDUSB20              (1u << 6)
#define USB_GUSB2PHYCFG_FSINTF                    (1u << 5)
#define USB_GUSB2PHYCFG_ULPI_UTMI_SEL             (1u << 4)
#define USB_GUSB2PHYCFG_PHYIF                     (1u << 3)
#define USB_GUSB2PHYCFG_TOUTCAL(x)                (((x) & 0x07) << 0)
#define USB_GUSB2PHYCFG_TOUTCAL_MASK              (0x07u << 0)
#define USB_GUSB2PHYCFG_TOUTCAL_BIT               (0)


// USB controller register bits:

// USB_GSBUSCFG0
#define USB_GSBUSCFG0_DATRDREQINFO(x)           (((x) & 0x0f) << 28)
#define USB_GSBUSCFG0_DATRDREQINFO_MASK         (0x0fu << 28)
#define USB_GSBUSCFG0_DATRDREQINFO_BIT          (28)
#define USB_GSBUSCFG0_DESRDREQINFO(x)           (((x) & 0x0f) << 24)
#define USB_GSBUSCFG0_DESRDREQINFO_MASK         (0x0fu << 24)
#define USB_GSBUSCFG0_DESRDREQINFO_BIT          (24)
#define USB_GSBUSCFG0_DATWRREQINFO(x)           (((x) & 0x0f) << 20)
#define USB_GSBUSCFG0_DATWRREQINFO_MASK         (0x0fu << 20)
#define USB_GSBUSCFG0_DATWRREQINFO_BIT          (20)
#define USB_GSBUSCFG0_DESWRREQINFO(x)           (((x) & 0x0f) << 16)
#define USB_GSBUSCFG0_DESWRREQINFO_MASK         (0x0fu << 16)
#define USB_GSBUSCFG0_DESWRREQINFO_BIT          (16)
#define USB_GSBUSCFG0_DATBIGEND             (1u << 11)
#define USB_GSBUSCFG0_DESBIGEND             (1u << 10)
#define USB_GSBUSCFG0_INCR256BRSTENA            (1u << 7)
#define USB_GSBUSCFG0_INCR128BRSTENA            (1u << 6)
#define USB_GSBUSCFG0_INCR64BRSTENA         (1u << 5)
#define USB_GSBUSCFG0_INCR32BRSTENA         (1u << 4)
#define USB_GSBUSCFG0_INCR16BRSTENA         (1u << 3)
#define USB_GSBUSCFG0_INCR8BRSTENA          (1u << 2)
#define USB_GSBUSCFG0_INCR4BRSTENA          (1u << 1)
#define USB_GSBUSCFG0_INCRBRSTENA           (1u << 0)

// USB_GUSB3PIPECTL_REGS
#define USB_GUSB3PIPECTL_PHYSOFTRST             (1u << 31)
#define USB_GUSB3PIPECTL_HSTPRTCMPL             (1u << 30)
#define USB_GUSB3PIPECTL_U2SSINACTP3OK          (1u << 29)
#define USB_GUSB3PIPECTL_DISRXDETP3             (1u << 28)
#define USB_GUSB3PIPECTL_UX_EXIT_IN_PX          (1u << 27)
#define USB_GUSB3PIPECTL_PING_ENHANCEMENT_EN    (1u << 26)
#define USB_GUSB3PIPECTL_U1U2EXITFAIL_TO_RECOV  (1u << 25)
#define USB_GUSB3PIPECTL_REQUEST_P1P2P3         (1u << 24)
#define USB_GUSB3PIPECTL_STARTRXDETU3RXDET      (1u << 23)
#define USB_GUSB3PIPECTL_DISRXDETU3RXDET        (1u << 22)
#define USB_GUSB3PIPECTL_DELAY_P1P2P3(x)        (((x) & 0x07) << 19)
#define USB_GUSB3PIPECTL_DELAY_P1P2P3_MASK      (0x07u << 19)
#define USB_GUSB3PIPECTL_DELAY_P1P2P3_BIT       (19)
#define USB_GUSB3PIPECTL_DELAY_PWRCHNG_P1P2P3   (1u << 18)
#define USB_GUSB3PIPECTL_SUSPEND_EN             (1u << 17)
#define USB_GUSB3PIPECTL_DATWIDTH(x)            (((x) & 0x03) << 15)
#define USB_GUSB3PIPECTL_DATWIDTH_MASK          (0x03u << 15)
#define USB_GUSB3PIPECTL_DATWIDTH_BIT           (15)
#define USB_GUSB3PIPECTL_ABORTRXDETINU2         (1u << 14)
#define USB_GUSB3PIPECTL_SKIPRXDET              (1u << 13)
#define USB_GUSB3PIPECTL_LFPSP0ALGN             (1u << 12)
#define USB_GUSB3PIPECTL_P3P2TRANOK             (1u << 11)
#define USB_GUSB3PIPECTL_P3EXSIGP2              (1u << 10)
#define USB_GUSB3PIPECTL_LFPSFILT               (1u <<  9)
#define USB_GUSB3PIPECTL_RX_DETECT_TO_POLL_CTL  (1u <<  8)
#define USB_GUSB3PIPECTL_TXSWING                (1u <<  6)
#define USB_GUSB3PIPECTL_TXMARGIN(x)            (((x) & 0x07) << 3)
#define USB_GUSB3PIPECTL_TXMARGIN_MASK          (0x07u << 3)
#define USB_GUSB3PIPECTL_TXMARGIN_BIT           (3)
#define USB_GUSB3PIPECTL_TXDEEMPHASIS(x)        (((x) & 0x03) << 1)
#define USB_GUSB3PIPECTL_TXDEEMPHASIS_MASK      (0x03u << 1)
#define USB_GUSB3PIPECTL_TXDEEMPHASIS_BIT       (1)
#define USB_GUSB3PIPECTL_ELASTICBUFFERMODE      (1u << 0)

// USB_GUSB3PIPECTL_DATWIDTH values
#define USB_GUSB3PIPECTL_DATWIDTH_32BITS        (0)
#define USB_GUSB3PIPECTL_DATWIDTH_16BITS        (1)
#define USB_GUSB3PIPECTL_DATWIDTH_8BITS         (2)

#define USB_HIGH_SPEED_CLK_CONFIG  (3)  //HS PLL uses EXTREFCLK as clock reference.
#define USB_SUPER_SPEED_CLK_CONFIG (2)  //HS PLL uses either ref_pad_clk{p,m} or ref_alt_clk_{p,m} as reference.

#define USB_REFCLK_20MHZ  (0b110001)  //TODO: add and implement multiple/flexible clock configurations


//2: Types
// ----------------------------------------------------------------------------

typedef struct
{
    u8 enableOtgBlock;
    u8 useExternalClock;
    u8 fSel;
    u8 refClkSel0;
    u8 forceHsOnly;
}osDrvUsbPhyParam_t;

#endif // _OSDRVUSBPHY_DEFINES_H_

///@}
