#include <registersMyriad.h>
#include <DrvCpr.h>
#include <DrvDdr.h>
#include <DrvLeonL2C.h>
#include <DrvShaveL2Cache.h>

#include "app_config.h"

// LeonOS L2 cache config
static LL2CConfig_t losCacheConfig = {
  .LL2CEnable       = 1,
  .writePolicy      = WRITE_THROUGH
};

// clock configuration
// note: MSS and UPA clocks are defined in .h file instead
static const tyAuxClkDividerCfg
auxClkCfg[] = {
  { AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 0, 0 },
  { 0, 0, 0, 0 }
};
static const tySocClockConfig
clkCfg = {
  .refClk0InputKhz         = 12000, // 12MHz xtal
  .targetPll0FreqKhz       = 600000, // 600MHz target freq
  .clkSrcPll1              = CLK_SRC_REFCLK0,
  .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
  .pAuxClkCfg              = auxClkCfg
};

int initClocksAndMemory()
{
  // initialize CPR
  DrvCprInit();
  // configure the clocks, including DDR
  DrvCprSetupClocks( &clkCfg ); // from the mdk header

  // initialize clocks as specified by app_init.h
  DrvCprSysDeviceAction( MSS_DOMAIN, SET_CLKS,       APP_MSS_CLOCKS );
  DrvCprSysDeviceAction( MSS_DOMAIN, DISABLE_CLKS,  ~APP_MSS_CLOCKS );
  DrvCprSysDeviceAction( MSS_DOMAIN, DEASSERT_RESET, APP_MSS_CLOCKS );
  DrvCprSysDeviceAction( UPA_DOMAIN, SET_CLKS,       APP_UPA_CLOCKS );
  DrvCprSysDeviceAction( UPA_DOMAIN, DISABLE_CLKS,  ~APP_UPA_CLOCKS );
  DrvCprSysDeviceAction( UPA_DOMAIN, DEASSERT_RESET, APP_UPA_CLOCKS );

  // initialize LOS' cache
  DrvLL2CInitialize( &losCacheConfig );

  // configure SHAVE0' L2 cache (single 256KB partition)
  DrvShaveL2CacheSetMode( L2CACHE_CFG );
  DrvShaveL2CacheSetupPartition( SHAVEPART256KB );
  DrvShaveL2CacheAllocateSetPartitions();
  DrvShaveL2CacheSetLSUPartId( 0, 0 );

  return 0;
}
