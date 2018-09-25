///
/// @file OsDrvUsbPhy.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvUsbPhy
/// @{
/// @brief USB PHY API functions
///


// 1: Includes
// ----------------------------------------------------------------------------
#include "OsDrvUsbPhy.h"
#include <registersMyriad.h>
#include "DrvRegUtils.h"
#include <assert.h>
#include <rtems.h>
#include "OsDrvCpr.h"
#include "OsDrvUsbPhyArch.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define WAIT_US 5

#ifdef USB_PHY_IGNORE_VBUS
#define USB_PHY_OTG_CTRL_IGNORE_VBUS (USB_PHY_OTG_CTRL_VBUSVLDEXTSEL0 | \
                                      USB_PHY_OTG_CTRL_VBUSVLDEXT0)
#else
#define USB_PHY_OTG_CTRL_IGNORE_VBUS (0)
#endif


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

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

// default configuration for register USB_PHY_CFG2_ADR (Bug23082)
#define USB_PHY_CFG2_DEFAULT_VAL                \
    ( USB_PHY_CFG2_PCS_TX_DEEMPH_6DB(0x20)    | \
      USB_PHY_CFG2_PCS_TX_DEEMPH_3P5DB(0x16)  | \
      USB_PHY_CFG2_PCS_RX_LOS_MASK_VAL(0xc8)  | \
      USB_PHY_CFG2_LOS_LEVEL(0x9)             | \
      USB_PHY_CFG2_LOS_BIAS(0x5))

// default configuration for register USB_PHY_CFG3_ADR (Bug23082)
#define USB_PHY_CFG3_DEFAULT_VAL                \
    ( USB_PHY_CFG3_TX_VBOOST_LVL(3)           | \
      USB_PHY_CFG3_PCS_TX_SWING_FULL(0x7a))

// PHY Control register access defines
#define USB_PHY_CTRL_TX_OVRD_IN_LO   0x1000
#define USB_PHY_CTRL_TX_DATA_EN         (1 << 4)
#define USB_PHY_CTRL_TX_DATA_OVRD       (1 << 5)
#define USB_PHY_CTRL_TX_EN              (1 << 6)
#define USB_PHY_CTRL_TX_EN_OVRD         (1 << 7)

#define USB_PHY_CTRL_RX_OVRD_IN_LO   0x1005
#define USB_PHY_CTRL_RX_DATA_EN         (1 << 5)
#define USB_PHY_CTRL_RX_DATA_EN_OVRD    (1 << 4)

#define USB_PHY_CTRL_RX_OVRD_OUT     0x1007
#define USB_PHY_CTRL_RX_VALID           (1 << 0)
#define USB_PHY_CTRL_RX_VALID_OVRD      (1 << 1)

#define USB_PHY_CR_CTRL_CR_READ       0x10000
#define USB_PHY_CR_CTRL_CR_WRITE      0x20000
#define USB_PHY_CR_CTRL_CR_CAP_ADDR   0x40000
#define USB_PHY_CR_CTRL_CR_CAP_DATA   0x80000
#define USB_PHY_CR_CTRL_CR_ACK        (1 << 16)

#define USB_PHY_STAT0_CR_DATA_OUT_MASK 0x0000FFFF;

static inline void osDrvUsbPhyWaitCrAckHigh(void)
{
    while (!(GET_REG_WORD_VAL(USB_PHY_STAT0_ADR) & USB_PHY_CR_CTRL_CR_ACK));
}

static inline void osDrvUsbPhyWaitCrAckLow(void)
{
    while ((GET_REG_WORD_VAL(USB_PHY_STAT0_ADR) & USB_PHY_CR_CTRL_CR_ACK));
}

// The access to the PHY control register is made through CR port: a 16-bit
// data/address parallel port. This interface is completely asynchronous—using
// a handshake between the cr_cap_addr, cr_cap_data, cr_read, and cr_write
// input commands with cr_ack acknowledgements and cd_data_out outputs from the
// PHY. CR port access is divided into address, read, and write transactions
//
// Eache type of transaction is described by the diagrams below:
//
// CR Bus Address Capture Transaction (see Fig. 3-21 p49 from USB PHY databook):
//
//   cr_data_in[15:0]  I _____/``````\__________
//                            \______/
//
//   cr_cap_addr       I _______/````\__________
//
//   cr_ack            O __________/````\_______
//
static inline void osDrvPhyCtrlCaptureAddr(uint32_t address)
{
    uint32_t wdata;

    wdata = USB_PHY_CR_CTRL_CR_CAP_ADDR | address;
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);
    osDrvUsbPhyWaitCrAckHigh();
    wdata = address; // set cr_cap_addr low
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);  // captured
    osDrvUsbPhyWaitCrAckLow();
}

// CR Bus Write Transaction (see Fig. 3-23 p49 from USB PHY databook):
//
//   cr_data_in[15:0]  I ________/``````\_______________
//                               \______/
//
//   cr_cap_data       I ________/````\_________________
//
//   cr_write          I _____________________/````\____
//
//   cr_ack            O ___________/````\______/````\__
//
static void osDrvUsbPhyCtrlRegWrite(uint32_t address, uint32_t data)
{
    uint32_t wdata;

    osDrvPhyCtrlCaptureAddr(address);

    wdata = USB_PHY_CR_CTRL_CR_CAP_DATA | data;
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);
    osDrvUsbPhyWaitCrAckHigh();
    wdata = data; // set cr_cap_data low
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);  // captured
    osDrvUsbPhyWaitCrAckLow();

    wdata = USB_PHY_CR_CTRL_CR_WRITE | data;  // write
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);
    osDrvUsbPhyWaitCrAckHigh();
    wdata = data;  // set cr_write low
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);  // written
    osDrvUsbPhyWaitCrAckLow();
}

// CR Bus Read Transaction (see Fig. 3-22 p49 from USB PHY databook):
//
//   cr_data_in[15:0]  O ________/`````\________
//                               \_____/
//
//   cr_read           I ______/````\___________
//
//   cr_ack            O ________/````\_________
//
static uint32_t osDrvUsbPhyCtrlRegRead(uint32_t address)
{
    uint32_t wdata;
    uint32_t val, readData;

    osDrvPhyCtrlCaptureAddr(address);

    wdata = USB_PHY_CR_CTRL_CR_READ | address;
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);
    osDrvUsbPhyWaitCrAckHigh();
    wdata = address;  // set cr_read low
    SET_REG_WORD(USB_PHY_CR_CTRL_ADR, wdata);
    val = GET_REG_WORD_VAL(USB_PHY_STAT0_ADR);
    readData = val & USB_PHY_STAT0_CR_DATA_OUT_MASK;
    osDrvUsbPhyWaitCrAckLow();

    return readData;
}

/// osDrvUsbPhyDisableRxTxSignals
///
/// @brief Disables Rx and Tx signals allowing only HighSpeed mode
///
/// This function disables Rx and Tx USB signals, which are used for SuperSpeed
/// transfer. This will force the USB device to be enumerated and to operate as
/// a HighSpeed device.
/// Note: This functionality should only be used when is necessary to operate only
/// in HighSpeed mode (maybe because of some hardware issued that caused
/// USB malfunction in SuperSpeed mode).
///
/// @return no return value
static void osDrvUsbPhyDisableRxTxSignals(void)
{
    uint32_t RX_OVR_IN_Aux;
    // Override TX enable and Data enable
    RX_OVR_IN_Aux = osDrvUsbPhyCtrlRegRead(USB_PHY_CTRL_TX_OVRD_IN_LO);
    RX_OVR_IN_Aux |= USB_PHY_CTRL_TX_EN_OVRD |
                     USB_PHY_CTRL_TX_DATA_OVRD;
    RX_OVR_IN_Aux &= ~(USB_PHY_CTRL_TX_EN |
                       USB_PHY_CTRL_TX_DATA_EN);
    osDrvUsbPhyCtrlRegWrite(USB_PHY_CTRL_TX_OVRD_IN_LO, RX_OVR_IN_Aux);

    // Override RX data en and RX valid
    RX_OVR_IN_Aux = osDrvUsbPhyCtrlRegRead(USB_PHY_CTRL_RX_OVRD_OUT);
    RX_OVR_IN_Aux |= USB_PHY_CTRL_RX_VALID_OVRD;
    RX_OVR_IN_Aux &= ~USB_PHY_CTRL_RX_VALID;
    osDrvUsbPhyCtrlRegWrite(USB_PHY_CTRL_RX_OVRD_OUT, RX_OVR_IN_Aux);

    // and RX data EN
    osDrvUsbPhyCtrlRegRead(USB_PHY_CTRL_RX_OVRD_IN_LO);
    RX_OVR_IN_Aux |= USB_PHY_CTRL_RX_DATA_EN_OVRD;
    RX_OVR_IN_Aux &= ~USB_PHY_CTRL_RX_DATA_EN;
    osDrvUsbPhyCtrlRegWrite(USB_PHY_CTRL_RX_OVRD_IN_LO, RX_OVR_IN_Aux);
}

static int osDrvUsbPhyCheckClocks(void) {
  s32 clkFreqKhz;
  if (OsDrvCprGetClockFreqKhz(AUX_CLK_USB_CTRL_SUSPEND_CLK,
       NULL, &clkFreqKhz) == OS_MYR_DRV_SUCCESS) {
    // TODO currently only 20MHz is supported
    assert(clkFreqKhz == 20000 && "AUX_CLK_USB_CTRL_SUSPEND_CLK must be set to 20MHz");
  } else {
    return RTEMS_NOT_CONFIGURED;
  }
  return RTEMS_SUCCESSFUL;
}

int OsDrvUsbPhyInit(osDrvUsbPhyParam_t *initParam)
{
    u32 tmpData;
    u32 waitTicks;
    u32 usPerTick;
    u32 useExternalClock;
    u32 fSel;
    u32 refClkSel;
    u32 forceHs;
    rtems_status_code status;

    usPerTick = 1000000 / rtems_clock_get_ticks_per_second();

    if (usPerTick >= WAIT_US)
    {
        waitTicks = 1;
    }
    else
    {
        waitTicks = (WAIT_US + (usPerTick - 1)) / usPerTick;
    }

    status = osDrvUsbPhyCheckClocks();
    if (status != RTEMS_SUCCESSFUL)
      return status;

    // Writing registers that drive the PHY ports
    // --------------------------------------------------------------------
    // USB3 Phy setup = 3 STEPS for controlling pins
    // --------------------------------------------------------------------
    if (initParam) {
      useExternalClock = initParam->useExternalClock;
      fSel = initParam->  fSel;;
      refClkSel = initParam->refClkSel0;
      forceHs = initParam->forceHsOnly;
    }
    else {
      useExternalClock = USB_PHY_USE_EXT_CLK;
      fSel = USB_REFCLK_20MHZ;
      refClkSel = USB_SUPER_SPEED_CLK_CONFIG;
      forceHs = USB_PHY_HS_ONLY_OFF;
    }

    // Registers have been defined in usb30_apbctl.v that connect to the
    // inputs of the PHY instanced in usb30_wrap.v

    // Setup USB_PHY_SSC_CTRL_ADR, see pins in PHY databook
    // USB 3.0 needs spread spectrum clock, but ssc_en only affects SS PHY
    // USB 2.0 has no spread spectrum clock, even though ssc_en = 0x1 (p39)
    // [ 8: 0] ssc_ref_clk_sel = 0x108, 52 MHz ref clock in SS mode (p40)
    // [12:10] ssc_range = 0x0 = -4,980 ppm (p39)
    // [13   ] ssc_en = 0x1 = enabled
    SET_REG_WORD(
            USB_PHY_SSC_CTRL_ADR,
            USB_PHY_SSC_CTRL_EN |
            USB_PHY_SSC_CTRL_SSC_RANGE(0) |
            USB_PHY_SSC_CTRL_REFCLK_SEL(0x108)  // SETUP SSC CLOCK
    );

    // Disable OTG block, power saving
    SET_REG_WORD(USB_PHY_OTG_CTRL_ADR, USB_PHY_OTG_CTRL_OTGDISABLE0 |
                                       USB_PHY_OTG_CTRL_IGNORE_VBUS);

    // Setup USB_PHY_CFG0_ADR, see PHY databook
    // See table 3-1 p37, row 6, shared onchip ref clock

    // keep PHY in reset, select ref clock frequency
    // [ 5: 0] fsel               = 0x31 = 20 MHz (table 3-1)
    // [14: 8] mpll_mult          = 0x7D, for 20 Mhz (value depending on fsel)
    // [16   ] mpll_refssc_clk_en = 0x0, value depending on fsel
    // [17   ] ref_clkdiv2        = 0x0, value depending on fsel
    // [18   ] ref_ssp_en         = 0x1, in testmode needs to enable ref clock out
    // [19   ] ref_use_pad        = 0x0, no external ref clock
    // [22:21] refclksel          = 0x2, superspeed clk config(table 3-1)
    // [23   ] retenablen         = 0x1, normal operation, analog powered
    // [24   ] commononn          = 0x0, normal operation, bias/pll on in suspend
    // [28   ] lane0_txrx_lpbk    = 0x0, no loopback
    // [29   ] lane0_ext_lpbk     = 0x0, no loopback
    // [31   ] phy_reset          = 0x1, reset active
    SET_REG_WORD(
            USB_PHY_CFG0_ADR,
            USB_PHY_CFG0_PHY_RESET |
            USB_PHY_CFG0_REF_SSP_EN     // RESET PHY + ref clock out
    );

    rtems_task_wake_after(waitTicks);

    tmpData =  USB_PHY_CFG0_PHY_RESET |
               USB_PHY_CFG0_RETENABLEN |
               USB_PHY_CFG0_REFCLKSEL(0x2 | refClkSel) |
               USB_PHY_CFG0_REF_SSP_EN |
               USB_PHY_CFG0_MPLL_MULTIPLIER(0x7D) | //20Mhz; TODO: add and implement multiple/flexible clock configurations
               USB_PHY_CFG0_FSEL(fSel);
    if (useExternalClock)
    {
        // for external usb clock
        tmpData |= USB_PHY_CFG0_REF_USE_PAD;
    }

    // KEEP PHY in RESET + CFG REF CLOCKS
    SET_REG_WORD(USB_PHY_CFG0_ADR, tmpData);

    SET_REG_WORD(USB_PHY_CFG1_ADR, USB_PHY_CFG1_DEFAULT_VAL);
    SET_REG_WORD(USB_PHY_CFG2_ADR, USB_PHY_CFG2_DEFAULT_VAL);
    SET_REG_WORD(USB_PHY_CFG3_ADR, USB_PHY_CFG3_DEFAULT_VAL);

    rtems_task_wake_after(waitTicks);

    //  make bit 31 zero, get PHY out of reset
    tmpData &= ~USB_PHY_CFG0_PHY_RESET;
    SET_REG_WORD(USB_PHY_CFG0_ADR, tmpData);

    rtems_task_wake_after(waitTicks);

    // Writing registers that drive the CONTROLLER ports
    // --------------------------------------------------------------------
    // USB3 Controller setup = one register for pin control
    // --------------------------------------------------------------------

    // Registers have been defined in usb30_apbctl.v that connect to the
    // inputs of the controller instanced in usb30_wrap.v

    // Setup USB_CTRL_CFG0_ADR, see USB CTRL databook
    // [ 0   ] pme_en               = 0x0, if pme_en is 0x1 the core
    //                                   asserts pme_generation in suspend (p366)
    // [ 6: 1] fladj_30mhz_reg      = 0x20 = jitter control in HS, trimming value? (p389)
    // [ 7   ] host_msi_enable      = 0x0, DRD mode (p365)
    // [ 8   ] host_port_power_control_present
    //                              = 0x1 = power control enabled
    // [ 9   ] host_u2_port_disable = 0x0  port enabled (p378)
    // [10   ] host_u3_port_disable = 0x0, port enabled (p378)
    // [12:11] hub_port_perm_attach = 0x0, no permantly attached indication, bit 1 SS bit 0 HS (p375)
    // [14:13] hub_port_overcurrent = 0x0, no overcurrent indication, bit 1 SS bit 0 HS (p374)
    // [15   ] bigendian_gs         = 0x0, little endian AMBA slave
    // [31   ] vcc_reset_n          = 0x1, inactive high reset
    SET_REG_WORD(
            USB_CTRL_CFG0_ADR,
            USB_CTRL_CFG0_CTRL_RESET |
            USB_CTRL_CFG0_FLADJ30MHZ(32)
    );
    rtems_task_wake_after(waitTicks);
    // ------------------------------------------------------------------------
    // USB3 Controller setup = internal core registers
    // ------------------------------------------------------------------------
    // p852 device mode power up (chapter 9.1.1)
    // Writing registers internal to the CONTROLLER
    OsDrvUsbPhyInitBusSettings();

    // USB2 and USB3 PHY are NOT allowed to SUSPEND!!!
    // Setup USB_GUSB2PHYCFG_REGS
    // Deasserting Resets USB 2.0 PHY
    // p677 in USB ctrl databook
    // [31] PHYSOFTRST    = PHY soft reset
    // [15] p852: must be zero (auto resume not allowed for device)
    // [6 ] SUSPENDUSB20  = if suspend conditions present PHY allowed to suspend
    // Both bits need to be cleared before config, afterwards bit 6 must be set.
    tmpData  = GET_REG_WORD_VAL(USB_GUSB2PHYCFG_REGS);
    tmpData &= ~(USB_GUSB2PHYCFG_PHYSOFTRST |
            USB_GUSB2PHYCFG_ULPIAUTORES |
            USB_GUSB2PHYCFG_SUSPENDUSB20);
    // p677 USB2.0 PHY out of reset
    SET_REG_WORD(USB_GUSB2PHYCFG_REGS, tmpData);

    // Setup USB_GUSB3PIPECTL_REGS
    // Deasserting Resets USB 3.0 PHY
    // p694 in USB ctrl databook
    // [31] PHYSOFTRST    = PHY soft reset
    // [17] SUSPENDENABLE = if suspend conditions present PHY allowed to suspend
    // Both bits need to be cleared before config, afterwards bit 17 must be set.
    tmpData  = GET_REG_WORD_VAL(USB_GUSB3PIPECTL_REGS);
    tmpData &= ~(USB_GUSB3PIPECTL_SUSPEND_EN |
                 USB_GUSB3PIPECTL_PHYSOFTRST |
                 USB_GUSB3PIPECTL_DATWIDTH_MASK);
    tmpData |= USB_GUSB3PIPECTL_DATWIDTH(USB_GUSB3PIPECTL_DATWIDTH_8BITS);
    SET_REG_WORD(USB_GUSB3PIPECTL_REGS, tmpData);

    rtems_task_wake_after(waitTicks);

    if (forceHs)
        osDrvUsbPhyDisableRxTxSignals();
    return RTEMS_SUCCESSFUL;
}


///@}
