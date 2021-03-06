#include <stdint.h>

constexpr uint64_t NCE_DPU0_BASE_ADDR  = 0x40300000;
constexpr uint64_t NCE_DPU1_BASE_ADDR  = 0x40302000;
constexpr uint64_t NCE_DPU2_BASE_ADDR  = 0x40304000;
constexpr uint64_t NCE_DPU3_BASE_ADDR  = 0x40306000;
constexpr uint64_t NCE_DPU4_BASE_ADDR  = 0x40308000;
constexpr uint64_t NCE_DPU5_BASE_ADDR  = 0x4030A000;
constexpr uint64_t NCE_DPU6_BASE_ADDR  = 0x4030C000;
constexpr uint64_t NCE_DPU7_BASE_ADDR  = 0x4030E000;
constexpr uint64_t NCE_DPU8_BASE_ADDR  = 0x40310000;
constexpr uint64_t NCE_DPU9_BASE_ADDR  = 0x40312000;
constexpr uint64_t NCE_DPU10_BASE_ADDR  = 0x40314000;
constexpr uint64_t NCE_DPU11_BASE_ADDR  = 0x40316000;

constexpr uint64_t NCE_DPU_SE_ADDR_OFFSET          = 0x000;
constexpr uint64_t NCE_DPU_SPARSITY_ADDR_OFFSET    = 0x004;
constexpr uint64_t NCE_DPU_Z_CONFIG_OFFSET         = 0x008;
constexpr uint64_t NCE_DPU_KERNAL_PAD_CFG_OFFSET   = 0x00C;
constexpr uint64_t NCE_DPU_WEIGHT_SIZE_OFFSET      = 0x010;
constexpr uint64_t NCE_DPU_WEIGHT_NUM_OFFSET       = 0x014;
constexpr uint64_t NCE_DPU_WEIGHT_START_OFFSET     = 0x018;
constexpr uint64_t NCE_DPU_WORKLOAD_SIZE0_OFFSET   = 0x01C;
constexpr uint64_t NCE_DPU_WORKLOAD_SIZE1_OFFSET   = 0x020;
constexpr uint64_t NCE_DPU_WORKLOAD_START0_OFFSET  = 0x024;
constexpr uint64_t NCE_DPU_WORKLOAD_START1_OFFSET  = 0x028;
constexpr uint64_t NCE_DPU_OFFSET_ADDR_OFFSET      = 0x02C;
constexpr uint64_t NCE_DPU_TENSOR_SIZE0_OFFSET     = 0x030;
constexpr uint64_t NCE_DPU_TENSOR_SIZE1_OFFSET     = 0x034;
constexpr uint64_t NCE_DPU_TENSOR_START_OFFSET     = 0x038;
constexpr uint64_t NCE_DPU_TENSOR_MODE_OFFSET      = 0x03C;
constexpr uint64_t NCE_DPU_ODU_CFG_OFFSET          = 0x040;
constexpr uint64_t NCE_DPU_DATA_BUFFER_OFFSET      = 0x044;
constexpr uint64_t NCE_DPU_SE_SIZE_OFFSET          = 0x048;
constexpr uint64_t NCE_DPU_TE_END0_OFFSET          = 0x04C;
constexpr uint64_t NCE_DPU_TE_END1_OFFSET          = 0x050;
constexpr uint64_t NCE_DPU_TE_BEG0_OFFSET          = 0x054;
constexpr uint64_t NCE_DPU_TE_BEG1_OFFSET          = 0x058;
constexpr uint64_t NCE_DPU_TE_DIM0_OFFSET          = 0x05C;
constexpr uint64_t NCE_DPU_TE_DIM1_OFFSET          = 0x060;
constexpr uint64_t NCE_DPU_SE_BASE_PTR_OFFSET      = 0x064;
constexpr uint64_t NCE_DPU_SP_BASE_PTR_OFFSET      = 0x068;
constexpr uint64_t NCE_DPU_DB_BASE_PTR0_OFFSET     = 0x06C;
constexpr uint64_t NCE_DPU_DB_BASE_PTR1_OFFSET     = 0x070;
constexpr uint64_t NCE_DPU_DB_BASE_PTR2_OFFSET     = 0x074;
constexpr uint64_t NCE_DPU_DB_BASE_PTR3_OFFSET     = 0x078;
constexpr uint64_t NCE_DPU_DB_BASE_PTR4_OFFSET     = 0x07C;
constexpr uint64_t NCE_DPU_DB_BASE_PTR5_OFFSET     = 0x080;
constexpr uint64_t NCE_DPU_DB_BASE_PTR6_OFFSET     = 0x084;
constexpr uint64_t NCE_DPU_DB_BASE_PTR7_OFFSET     = 0x088;
constexpr uint64_t NCE_DPU_DB_BASE_PTR8_OFFSET     = 0x08C;
constexpr uint64_t NCE_DPU_DB_BASE_PTR9_OFFSET     = 0x090;
constexpr uint64_t NCE_DPU_DB_BASE_PTR10_OFFSET    = 0x094;
constexpr uint64_t NCE_DPU_DB_BASE_PTR11_OFFSET    = 0x098;
constexpr uint64_t NCE_DPU_DB_BASE_PTR12_OFFSET    = 0x09C;
constexpr uint64_t NCE_DPU_DB_BASE_PTR13_OFFSET    = 0x0A0;
constexpr uint64_t NCE_DPU_DB_BASE_PTR14_OFFSET    = 0x0A4;
constexpr uint64_t NCE_DPU_DB_BASE_PTR15_OFFSET    = 0x0A8;
constexpr uint64_t NCE_DPU_MPE_CFG_OFFSET          = 0x0AC;
constexpr uint64_t NCE_DPU_MPE_BUS_DATA_SEL_OFFSET = 0x0B0;
constexpr uint64_t NCE_DPU_MPE_BUS_DATA_OFFSET     = 0x0B4;
constexpr uint64_t NCE_DPU_PPE_CFG_OFFSET          = 0x0B8;
constexpr uint64_t NCE_DPU_DPU_CFG_OFFSET          = 0x0BC;
constexpr uint64_t NCE_DPU_DPU_STS_OFFSET          = 0x0C0;
