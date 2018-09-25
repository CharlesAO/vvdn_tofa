///
/// @file DrvLeonL2C.c
/// @copyright All code copyright Movidius Srl 2014, Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvLeonL2C
/// @{
/// @brief Leon L2 Cache Driver.
///
/// This module contains the helper functions necessary to control the
/// Leon L2 Cache in the Myriad Soc
///

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// 1: Includes
// ----------------------------------------------------------------------------

#include <DrvRegUtils.h>
#include <DrvLeonL2C.h>


// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// Change number of MTRR if hardware changes. Use DrvLL2CGetNumOfMTRRRegs to get the actual number
#define LL2C_MAX_MTTR_REGS 8
// Default LL2C config in case none specified
#define LL2C_CTRL_DEFAULT_CONFIG_VALUE ( LL2C_CTRL_EN | LL2C_CTRL_REPL__LRU | \
										 LL2C_CTRL_NR_LOCKED_WAYS(0) | \
										 LL2C_CTRL_WRITE_POLICY_COPY_BACK)

//MTRR addresses
static u32 MTRRAddrs[LL2C_MAX_MTTR_REGS] =
{
    0, 0, 0, 0, 0, 0, 0, 0
};

void DrvLL2CInitialize(LL2CConfig_t *config)
{
    u32 base = DrvLL2CGetMyBase(), controlRegVal = 0;

    assert(base);

    u32 regAddr = (base) + (L2C_LEON_OS_CONTROL - L2C_LEON_OS_BASE_ADR);
    
    #ifndef NDEBUG
    u32 oldVal = GET_REG_WORD_VAL(regAddr);
    // you may not change this while the L2 Cache is enabled
    assert(!(oldVal & LL2C_CTRL_EN));
    #endif

    // If config is NULL, we can init the Cache with a default init
    if (config == NULL)
    {
        // Config the GPIO using a default config
        controlRegVal = LL2C_CTRL_DEFAULT_CONFIG_VALUE;
    }
    // Use config to configure LL2C
    else {
        // Replacement policy
        controlRegVal |= (controlRegVal & ~LL2C_CTRL_REPL_MASK) |
                        (config->replacePolicy<<LL2C_CTRL_REPL_POS & LL2C_CTRL_REPL_MASK);
        // Set way to replace field
        controlRegVal |= LL2C_CTRL_WAY_TO_REPLACE(config->LL2CWayToReplace) & LL2C_CTRL_WAY_TO_REPLACE_MASK;
        // Set number of locked ways
        controlRegVal |= ((controlRegVal & ~LL2C_CTRL_NR_LOCKED_WAYS_MASK)
                        | (LL2C_CTRL_NR_LOCKED_WAYS(config->LL2CLockedWaysNo) & LL2C_CTRL_NR_LOCKED_WAYS_MASK));
        // Set Bus usage bit
        controlRegVal |= ((controlRegVal & ~LL2C_CTRL_BUS_USAGE_STATUS_MASK) |
                        ((config->busUsage<<LL2C_CTRL_BUS_USAGE_STATUS_POS) & LL2C_CTRL_BUS_USAGE_STATUS_MASK));
        // Set  hit rate bit
        controlRegVal |= ((controlRegVal & ~LL2C_CTRL_HIT_RATE_STATUS_MASK) |
                        (config->hitRate<<LL2C_CTRL_HIT_RATE_STATUS_POS & LL2C_CTRL_HIT_RATE_STATUS_MASK));
        // Set write protection policy
        controlRegVal |= ((controlRegVal & ~LL2C_CTRL_WRITE_POLICY_MASK) |
                        (config->writePolicy<<LL2C_CTRL_WRITE_POLICY_POS & LL2C_CTRL_WRITE_POLICY_MASK));

        // Set enable  bit
        controlRegVal |= ((controlRegVal & ~LL2C_CTRL_EN_MASK) |
                        (config->LL2CEnable << LL2C_CTRL_EN_POS & LL2C_CTRL_EN_MASK));
    }
	// Invalidate entire L2Cache before enabling it
	DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, /*disable cache?:*/ 0);
    SET_REG_WORD(regAddr, controlRegVal);
}

void DrvLL2CInitWriteThrough()
{
    LL2CConfig_t ll2Config;

    ll2Config.LL2CEnable       = 1;
    ll2Config.LL2CLockedWaysNo = 0;
    ll2Config.LL2CWayToReplace = 0;
    ll2Config.busUsage         = BUS_WRAPPING_MODE;
    ll2Config.hitRate          = HIT_WRAPPING_MODE;
    ll2Config.replacePolicy    = LRU;
    ll2Config.writePolicy      = WRITE_THROUGH;

    DrvLL2CInitialize(&ll2Config);
}

DRV_RETURN_TYPE DrvLL2CGetRegister(LL2CRegister_t reg, u32 *regVal)
{
    u32 base = DrvLL2CGetMyBase();
    u32 regAddr;

    assert(base);

    switch (reg)
    {
    case LL2C_USAGE_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_BU_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_CYCLE_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_BC_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_HIT_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_HIT_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_ACCESS_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_ACCESS_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_ERROR_ADDRESS:
        regAddr = (base) + (L2C_LEON_OS_ERR_ADDR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_STATUS_REGISTER:
        regAddr = (base) + (L2C_LEON_OS_STATUS - L2C_LEON_OS_BASE_ADR);
        break;
    default:
        return MYR_DRV_ERROR;
    }

	*regVal = GET_REG_WORD_VAL(regAddr);
    return MYR_DRV_SUCCESS;
}

DRV_RETURN_TYPE DrvLL2CSetRegister(LL2CRegister_t reg, u32 value)
{
    u32 base = DrvLL2CGetMyBase();
    u32 regAddr;

    assert(base);

    switch (reg)
    {
    case LL2C_USAGE_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_BU_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_CYCLE_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_BC_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_HIT_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_HIT_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    case LL2C_ACCESS_COUNTER:
        regAddr = (base) + (L2C_LEON_OS_ACCESS_CNTR - L2C_LEON_OS_BASE_ADR);
        break;
    default:
        return MYR_DRV_ERROR;
    }

    SET_REG_WORD(regAddr, value);
    return MYR_DRV_SUCCESS;
}

void DrvLL2CDisable(enum DevLL2COperation operation)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);

    DrvLL2CFlushOpOnAllLines(operation, /*disable cache?*/1);

    u32 reg = (base) + (L2C_LEON_OS_CONTROL - L2C_LEON_OS_BASE_ADR);
    u32 oldVal = GET_REG_WORD_VAL(reg);

    SET_REG_WORD(base, oldVal & (~LL2C_CTRL_EN));
}

void DrvLL2CFlushWay(u32 wayNr, enum DevLL2CWayAndTagOperation flushType)
{
    u32 tag = DrvLL2CDiagGetTag(wayNr, 0);
    DrvLL2CFlushOpOnWayAndTag(flushType, 0, wayNr, tag, 1, 1, 0);
}

void DrvLL2CSetMTRRs(const struct LL2CMemoryRange *ranges, u32 nrRanges)
{
    u32 i;
    u32 base = DrvLL2CGetMyBase();

    assert(base);
    assert(nrRanges < LL2C_MAX_MTTR_REGS);

    for (i = 0; i < nrRanges; i++)
    {
        assert((ranges[i].address & ~LL2C_MTRR_ADDRESS_MASK) == 0);
        assert((ranges[i].mask & ~LL2C_MTRR_ADDRESS_MASK_MASK) == 0);

        u32 reg = (base) + (L2C_LEON_OS_MTRR0 - L2C_LEON_OS_BASE_ADR)
                  + (i << 2);
        u32 val = ranges[i].address & LL2C_MTRR_ADDRESS_MASK;
        val |= (ranges[i].mask >> 16) & LL2C_MTRR_ADDRESS_MASK_MASK;
        val |= (ranges[i].accessMode << 16) & LL2C_MTRR_ACCESS_FIELD_MASK;
        val |= ((ranges[i].writeProtected & 1) << 1) & LL2C_MTRR_WRITE_PROT_MASK;
        val |= (1 & LL2C_MTRR_ENABLE_MASK); // enabled
        SET_REG_WORD(reg, val);
    }

    // Disable the rest of them
    for (i = nrRanges; i < LL2C_MAX_MTTR_REGS; i++)
    {
        u32 regAddr = (base) + (L2C_LEON_OS_MTRR0 - L2C_LEON_OS_BASE_ADR)
                      + (i << 2);
        SET_REG_WORD(regAddr, 0); // disable unused
    }
}

static void drvLL2CSetMTRRSingle(struct LL2CMemoryRange *range, u32 id)
{
    u32 base = DrvLL2CGetMyBase();

    assert(base);
    assert(id < 8);
    u32 reg = (base) + (L2C_LEON_OS_MTRR0 - L2C_LEON_OS_BASE_ADR) + (id << 2);
    u32 val = range->address; // & 0xfffc0000; //masking the maximum chunk
    val |= (range->mask >> 16) & 0xfffffffc;
    val |= (range->accessMode & 3) << 16;
    val |= ((range->writeProtected) ? 1 : 0) << 1;

    if (range->address != 0)
        val |= 1; // enabled
    else
        val |= 0;
    SET_REG_WORD(reg, val);
}

s32 DrvLL2CSetMTRRfunc(void *addr, u32 size, u32 mode, u32 wp)
{
    LL2CMemoryRange_t memoryRange;
    u32 i = 0;

    // Create the mask here
    u32 tempsize, mask;
    tempsize = size + 1;

    do
    {
        tempsize >>= 1;
        i++;
    } while ((tempsize != 0) && (i < 32));

    if (i > 31)
    {
        return -2;
    }

    // pow(2, i);
    u32 val = 1, ii;
    for (ii = 0; ii < i; ii++)
    {
        val = val * 2;
    }

    mask = val - 1; //2^i is not working
    // \Create the mask here

    memoryRange.address = (u32) addr & ~mask;
    memoryRange.mask = ~mask;
    memoryRange.accessMode = mode & (LL2C_MTRR_ACCESS_FIELD_MASK >> LL2C_MTRR_ACCESS_FIELD_POS);
    memoryRange.writeProtected = wp & (LL2C_MTRR_WRITE_PROT_MASK >> LL2C_MTRR_WRITE_PROT_POS);

    i = 0;
    while (MTRRAddrs[i++] != 0)
    {
        if (i > ((LL2C_MAX_MTTR_REGS) - 1))
        {

            return -1;
        }
    }
    drvLL2CSetMTRRSingle(&memoryRange, i - 1);
    MTRRAddrs[i - 1] = (u32) addr;

    return 0;
}

s32 DrvLL2CRemoveMTRRfunc(void *addr)
{
    UNUSED(addr); // hush the compiler warning.

    struct LL2CMemoryRange memoryRange;
    u32 i = 0;
    while (MTRRAddrs[i++] != 0)
    {
        if (i > ((8) - 1))
        {

            return -1;
        }
    }
    memoryRange.address = 0;
    memoryRange.mask = 0;
    memoryRange.accessMode = 2;
    memoryRange.writeProtected = 0;

    drvLL2CSetMTRRSingle(&memoryRange, i - 1);

    MTRRAddrs[i - 1] = 0;

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////DEPRECATED FUNCTIONS HERE//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void DrvLL2CFlushAll(enum DevLL2COperation flushType)
{
    DrvLL2CFlushOpOnAllLines(flushType, 0);
}

void DrvLL2CSetMTRR(u32 base, const struct LL2CMemoryRange *ranges, u32 nr_ranges)
{
    u32 i;
    assert(nr_ranges <= 8);
    for (i=0;i<nr_ranges;i++)
    {
        assert((ranges[i].address & 0x3ffff) == 0);
        assert((ranges[i].mask & ~0xfffc0000) == 0);
        u32 reg = (base) + (L2C_LEON_OS_MTRR0 - L2C_LEON_OS_BASE_ADR) + (i << 2);
        u32 val = ranges[i].address & 0xfffc0000;
        val |= (ranges[i].mask >> 16) & 0x0000fffc;
        val |= (ranges[i].accessMode & 3) << 16;
        val |= ((ranges[i].writeProtected) ? 1 : 0) << 1;
        val |= 1; // enabled
        SET_REG_WORD(reg, val);
    }
    for (;i<8;i++)
    {
        u32 reg = (base) + (L2C_LEON_OS_MTRR0 - L2C_LEON_OS_BASE_ADR) + (i << 2);
        SET_REG_WORD(reg, 0); // disable unused
    }
}

static u32 custom_pow(u32 base, u32 v){
    u32 val = 1,i;
    for (i=0;i<v;i++){
        val = val * base;
    }
    return val;
}

s32 SetMTRRfunc(void *addr, u32 size, u32 mode, u32 wp){
    struct LL2CMemoryRange memory_range;
    u32 i=0;
    u32 tempsize,mask;
    tempsize = size + 1;
    do{
        tempsize>>=1;
        i++;
    }while((tempsize!=0) && (i<32));
    if (i>31)
        return -2;
   
    mask = custom_pow(2,i) - 1; //2^i is not working
    memory_range.address = (u32)addr & ~mask;
    memory_range.mask = ~mask;
    memory_range.accessMode = mode & 0x3;//LL2C_WRITE_THROUGH; //LL2C_UNCACHED
    memory_range.writeProtected = wp & 0x1;
    i=0;
    while (MTRRAddrs[i++]!=0){
        if (i>((8) -1) ){
            
            return -1;
        }
    }    
    drvLL2CSetMTRRSingle(&memory_range, i-1);
    MTRRAddrs[i-1] = (u32)addr;
    
    return 0;

}

u32 DrvLL2CGetControlRegister()
{
    u32 base = DrvLL2CGetMyBase();
    
    assert(base);
    
    u32 reg = (base) + (L2C_LEON_OS_CONTROL - L2C_LEON_OS_BASE_ADR);

    return GET_REG_WORD_VAL(reg);
}

#pragma GCC diagnostic pop

/// @}

