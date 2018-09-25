///
/// @file DrvEfuse.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvEfuse
/// @{
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <mvMacros.h>
#include <registersMyriad.h>
#include <DrvRegUtils.h>
#include <assert.h>
#include <alloca.h>
#include <DrvRegUtilsDefines.h>
#include <swcLeonUtilsDefines.h>
#include <DrvEfuse.h>
#include <DrvCpr.h>
#include <string.h>
#include <stdio.h>
#include <DrvTimer.h>

// warning bitAddress is the same that gets on the eFuse address bus,
// so for example the address of word4.bit3 will be
// 4 | (3 << 5) = 0x64, and the address of FB[2]_A1
// will be 7 << 4 = 0x70. Bits of the same word are not
// at continuous addresses
void DrvEfuseProgramBit(int redundancy, int bitAddress);

// warning bitAddress is the same that gets on the eFuse address bus,
// so for example the address of word4.bit3 will be
// 4 | (3 << 5) = 0x64, and the address of FB[2]_A1
// will be 7 << 4 = 0x70. Bits of the same word are not
// at continuous addresses
void DrvEfuseProgramBits(int redundancy, int n, const int bitAddresses[]);

void DrvEfuseARead(u32 *dst, u32 start, u32 end, int margin);
void DrvEfuseRRead(u32 *dst, int margin);
void DrvEfusePowerDown(void);
int DrvEfusePowerUp(void);

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data 
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
static inline int DrvEfuseSettingIndexToOrMask(int setting_index);
static int redundancy_use_mask(u32 *red);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
static inline int DrvEfuseSettingIndexToOrMask(int setting_index)
{
    switch (setting_index)
    {
        case 0:
            return 0x00 << 4;
        case 1:
            return 0x20 << 4;
        case 2:
            return 0x01 << 4;
        case 3:
            return 0x21 << 4;
        default:
            assert(0);
    }
    return -1; // should never reach this.
}

static int redundancy_use_mask(u32 *red) {
    int res = 0;
    if(red[0] & (0x8fff << 0))
        res |= 1 << 0;
    if(red[0] & (0x8fff << 16))
        res |= 1 << 1;
    if(red[1] & (0x8fff << 0))
        res |= 1 << 2;
    if(red[1] & (0x8fff << 16))
        res |= 1 << 3;
    return res;
}

int DrvEfuseDisableRedundancySetting(int setting_index)
{
    int setting_ormask;
    int ret;
    u32 tmp[2];
    
    if (setting_index < 0 || setting_index >= EFUSE_NUM_RED_BITS)
        return DRVEFUSE_INPUT_ERROR; // error, cannot use this setting
    
    setting_ormask = DrvEfuseSettingIndexToOrMask(setting_index);
    ret = DrvEfusePowerUp();
    if (ret)
        return ret;
    DrvEfuseRRead(tmp, NORMAL_MODE);
    DrvEfuseProgramBit(1, (0xf << 5) | setting_ormask);   // Set FB[setting_index]_Disable
    DrvEfuseRRead(tmp, NORMAL_MODE); // according to the model, must re-read redundancy information after programming it
    DrvEfusePowerDown();
    return DRVEFUSE_SUCCESS;
}

int DrvEfuseFixBitUsingRedundancy(int setting_index, int address, int value)
{
    int setting_ormask;
    int ret;
    u32 red[2];
    
    if ((setting_index >= EFUSE_NUM_RED_BITS) || (setting_index < -1) ||
        (address > 0x3ff))
        return DRVEFUSE_INPUT_ERROR;
    if((ret = DrvEfusePowerUp()) != 0)
        return ret;
    
    DrvEfuseRRead(red, NORMAL_MODE);
    
    if(-1 == setting_index) {
        for (int i = 0; i < EFUSE_NUM_RED_BITS; i++) {
            u32 ared = red[i / 2] >> ((i % 2) * 16);
            if (!(ared & 0x8fff)) {
                setting_index = i;
                break;
            }
        }
    }
    if(-1 == setting_index)
        return DRVEFUSE_NO_RED;
    
    setting_ormask = DrvEfuseSettingIndexToOrMask(setting_index);
    u32 ared = red[setting_index / 2] >> ((setting_index % 2) * 16);
    if (ared & 0x8fff)
        return DRVEFUSE_NO_RED; // error, cannot use this setting as it's already in use or degraded
    
    int i;
    address = ((address >> 5) | (address << 5)) & 0x3ff;
    for (i=0;i<=9;i++)
    {
        // for each address bit
        if (address & (1 << i))
        {
            DrvEfuseProgramBit(1, ((i + 2) << 5) | setting_ormask);  //Set FB[setting_index]_A0-9 depending on address
        }
    }

    if (value)
    {
        DrvEfuseProgramBit(1, ((1) << 5) | setting_ormask); //Set FB[setting_index]_Data if value is not 0
    }
    DrvEfuseProgramBit(1, ((0) << 5) | setting_ormask); // and enable it! 
    
    // according to the model, must re-read redundancy information after programming it
    DrvEfuseRRead(red, MARGIN_MODE);
    u32 ared_margin = red[setting_index / 2] >> ((setting_index % 2) * 16);

    DrvEfuseRRead(red, NORMAL_MODE);
    ared = red[setting_index / 2] >> ((setting_index % 2) * 16);

    // Checking error with redundancy programmation
    if (!(ared & 1))  // Expected enabled, but not enabled
    {
        DrvEfuseDisableRedundancySetting(setting_index);
        return DRVEFUSE_FIX_FAILED;
    }
    if (((ared & 2) && !value) || ((!(ared & 2)) && value)) // error if value is 0 and read 1 or value is 1 and read 0
    {
        DrvEfuseDisableRedundancySetting(setting_index);
        return DRVEFUSE_FIX_FAILED;
    }
    if (((int)((ared >> 2) & 0x3ff)) !=  address)
    {
        DrvEfuseDisableRedundancySetting(setting_index);
        return DRVEFUSE_FIX_FAILED;
    }
    if (ared & 0x8000) // if accidentally was disabled?
    {
        // already disabled, no need to disable in this error condition
        DrvEfusePowerDown();
        return DRVEFUSE_FIX_FAILED;
    }
    if ((ared & 0x8fff) != (ared_margin & 0x8fff)) // Normal and margin read do not match
    {
        DrvEfuseDisableRedundancySetting(setting_index);
        return DRVEFUSE_FIX_FAILED;
    }
    DrvEfusePowerDown();
    return DRVEFUSE_SUCCESS;
}

int DrvEfuseLockRedundancy(u32 token) {
    int res;
    u32 red[2];
    if(token != DRVEFUSE_LOCK_TOKEN)
        return DRVEFUSE_LOCKRED_BADTOKEN;
    DrvEfusePowerDown();
    if((res = DrvEfusePowerUp()) != 0)
        return res;
    DrvEfuseRRead(red, NORMAL_MODE);
    if(!(red[0] & (0x8fff << 0)))
        DrvEfuseDisableRedundancySetting(0);
    if(!(red[0] & (0x8fff << 16)))
        DrvEfuseDisableRedundancySetting(1);
    if(!(red[1] & (0x8fff << 0)))
        DrvEfuseDisableRedundancySetting(2);
    if(!(red[1] & (0x8fff << 16)))
        DrvEfuseDisableRedundancySetting(3);
    DrvEfusePowerDown();
    return DRVEFUSE_SUCCESS;
}

int DrvEfuseReadEFuses(efuseSet *fSet, efuseFix_t fix, efuseMargin_t margin) {
    int res;
    
    if(fSet == NULL) 
        return DRVEFUSE_INPUT_ERROR;
    
    if((res = DrvEfusePowerUp()) != 0)
        return res;
    if(fix) {
        u32 dummy[2];
        DrvEfuseRRead(dummy, NORMAL_MODE);
    }
    DrvEfuseARead((u32*)fSet->values, 0, sizeof(fSet->values) / sizeof(u32) - 1, margin);
    DrvEfusePowerDown();
    return DRVEFUSE_SUCCESS;
}

int DrvEfuseReadRedundancy(redFuseSet *rSet, efuseMargin_t margin) {
    int res;

    if(rSet == NULL)
        return DRVEFUSE_INPUT_ERROR;

    DrvEfusePowerDown();
    DrvTimerSleepMicro(300);
    if((res = DrvEfusePowerUp()) != 0)
        return res;
    DrvEfuseRRead((u32 *)rSet->values, margin);
    for (int i = 0; i < EFUSE_NUM_RED_BITS; i++) {
        rSet->values[i].addr = (rSet->values[i].addr >> 5) | (rSet->values[i].addr << 5);
    }
    DrvEfusePowerDown();

    return DRVEFUSE_SUCCESS;
}

int DrvEfuseProgramWithMask(efuseMaskSet *maskSet, u32 flags) {
    efuseSet lset;
    u32 red[2];
    int bits[EFUSE_NUM_BITS];
    struct {
        u32 addr;
        u32 value;
    } redBits[EFUSE_NUM_RED_BITS];
    u32 redFreeSlots[EFUSE_NUM_RED_BITS];
    int bitCnt, bitNum, curMask, redMask, redFailedCnt, redFreeNum;
    int i, j, firstByte, lastByte, firstWord, lastWord;

    for(i=0; i<(EFUSE_NUM_BITS/8); i++) {
        if(maskSet->mask.values[i] != 0)
            break;
    }
    if(i == (EFUSE_NUM_BITS/8)) {
        /* Nothing to write! */
        if(flags & DRVEFUSE_FLAG_READBACK)
            return DrvEfuseReadEFuses(&maskSet->values, 1, 0);
        return 0;
    }
    firstByte = i;
    firstWord = i / sizeof(u32);
    for(i=((EFUSE_NUM_BITS/8)-1); i>=0; i--) {
        if(maskSet->mask.values[i] != 0)
            break;
    }
    lastByte = i;
    lastWord = i / sizeof(u32);

    DrvEfusePowerDown();
    if((i = DrvEfusePowerUp()) != 0)
        return i;
    DrvEfuseRRead(red, NORMAL_MODE);
    DrvEfuseARead(((u32*)lset.values) + firstWord, firstWord, lastWord, NORMAL_MODE);
    DrvEfusePowerDown();

    bitCnt = 0;
    bitNum = firstByte * 8;
    for(i=firstByte; i<=lastByte; i++) {
        u8 mask = maskSet->mask.values[i];
        if(mask == 0) {
            bitNum += 8;
            continue;
        }
        u8 cval = lset.values[i];
        u8 wval = maskSet->values.values[i];
        u8 dval = (~cval & wval) & mask;
        for(j=0, curMask=1; j<8; j++, curMask<<=1) {
            if(dval & curMask) {
                bits[bitCnt] = ((bitNum>>5) | (bitNum<<5)) & 0x3FF;
                bitCnt++;
            }
            bitNum++;
        }
    }

    if(bitCnt != 0) {
        DrvEfusePowerDown();
        if((i = DrvEfusePowerUp()) != 0)
            return i;
        DrvEfuseProgramBits(0, bitCnt, bits);
        DrvEfusePowerDown();
    }

    if(flags & DRVEFUSE_FLAG_REDUNDANCY) {
        DrvEfusePowerDown();
        if((i = DrvEfusePowerUp()) != 0)
            return i;
        // Redundancy information read normally, array read with margin mode
        DrvEfuseRRead(red, NORMAL_MODE);
        DrvEfuseARead(((u32*)lset.values) + firstWord, firstWord, lastWord, MARGIN_MODE);
        DrvEfusePowerDown();

        // Get the available (unused) redundancy slots
        redMask = redundancy_use_mask(red);
        redFreeNum = 0;
        for (i = 0; i < EFUSE_NUM_RED_BITS; i++) {
            if(redMask & (1 << i))
                continue;
            redFreeSlots[redFreeNum] = i;
            redFreeNum++;
        }

        bitCnt = 0;
        bitNum = firstByte * 8;
        for(i=firstByte; i<=lastByte; i++) {
            u8 mask = maskSet->mask.values[i];
            if(mask == 0) {
                bitNum += 8;
                continue;
            }
            u8 cval = lset.values[i];
            u8 wval = maskSet->values.values[i];
            u8 dval = (cval ^ wval) & mask;
            for(j=0, curMask=1; j<8; j++, curMask<<=1) {
                if(dval & curMask) {
                    if(bitCnt >= redFreeNum)
                        return DRVEFUSE_NO_RED;
                    redBits[bitCnt].addr = bitNum;
                    redBits[bitCnt].value = !!(wval & curMask);
                    bitCnt++;
                }
                bitNum++;
            }
        }

        if(bitCnt != 0) {
            DrvEfusePowerDown();
            if((i = DrvEfusePowerUp()) != 0)
                return i;
            redFailedCnt = 0;
            for(i=0; i<bitCnt; ) {
                if (bitCnt + redFailedCnt > redFreeNum) // Stop early if not enough remaining redundancy bits
                    break;
                if(DrvEfuseFixBitUsingRedundancy(redFreeSlots[i+redFailedCnt],redBits[i].addr, redBits[i].value) != 0)
                    redFailedCnt++; // Redundancy programming failed, will retry using the next free bit
                else
                    i++; // Advance to next bit to be fixed
            }
            DrvEfusePowerDown();
            if(i != bitCnt)
                return DRVEFUSE_PWMERR_REDFAILED;
        }
    }
    if((flags & DRVEFUSE_FLAG_READBACK) || !(flags & DRVEFUSE_FLAG_NO_VERIFY)) {
        int readModes[] = {MARGIN_MODE, NORMAL_MODE};
        int verifyFailed = 0;
        for (j = 0; j < (int)COUNT_OF(readModes) && !verifyFailed; j++) {
            DrvEfusePowerDown();
            if((i = DrvEfusePowerUp()) != 0)
                return i;
            DrvEfuseRRead(red, NORMAL_MODE);
            if(flags & DRVEFUSE_FLAG_READBACK)
                DrvEfuseARead((u32*)lset.values, 0, (EFUSE_NUM_BITS / 32) - 1, readModes[j]);
            else
                DrvEfuseARead(((u32*)lset.values) + firstWord, firstWord, lastWord, readModes[j]);
            DrvEfusePowerDown();
            
            if(!(flags & DRVEFUSE_FLAG_NO_VERIFY)) {
                for(i=firstByte; i<=lastByte; i++) {
                    u8 mask = maskSet->mask.values[i];
                    if(mask == 0)
                        continue;
                    u8 cval = lset.values[i];
                    u8 wval = maskSet->values.values[i];
                    u8 dval = (cval ^ wval) & mask;
                    if(dval != 0) {
                        verifyFailed = 1;
                        break;
                    }
                }
            }
        }

        if(flags & DRVEFUSE_FLAG_READBACK)
            memcpy(&maskSet->values, &lset, sizeof(efuseSet));

        if (verifyFailed)
            return DRVEFUSE_PWMERR_FAILVERIFY;
    }

    return DRVEFUSE_SUCCESS;
}

void DrvEfuseCopy(efuseSet *dst, const efuseSet *src, u32 startBit, u32 endBit) {
    int curByte, endByte;
    u8 mask;
    u8 *dp;
    const u8 *sp;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    dp = &dst->values[curByte];
    sp = &src->values[curByte];

    if((curByte != endByte) && (startBit != 0)) {
        mask = ((1U << (8 - startBit)) - 1) << startBit;
        u8 maskDp = *dp & ~mask;
        *dp = maskDp | (*sp++ & mask);
        ++dp;
        startBit = 0;
        curByte++;
    }

    while(curByte != endByte) {
        *dp++ = *sp++;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    *dp = (*dp & ~mask) | (*sp & mask);
}

void DrvEfuseClearSet(efuseSet *set) {
    memset(set, 0, sizeof(*set));
}

void DrvEfuseClearMaskSet(efuseMaskSet *maskSet) {
    memset(maskSet, 0, sizeof(*maskSet));
}

void DrvEfuseSetMask(efuseMaskSet *maskSet, u32 startBit, u32 endBit, int state) {
    int curByte, endByte;
    u8 mask;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        mask = ((1U << (8 - startBit)) - 1) << startBit;
        if(state)
            maskSet->mask.values[curByte] |= mask;
        else
            maskSet->mask.values[curByte] &= ~mask;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        maskSet->mask.values[curByte] = state ? 0xff : 0x00;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    if(state)
        maskSet->mask.values[curByte] |= mask;
    else
        maskSet->mask.values[curByte] &= ~mask;
}

void DrvEfuseSetValue1(efuseSet *set, u32 bitIndex, u32 value) {
    int byteIndex = bitIndex / 8;
    u8 mask;
    bitIndex %= 8;
    mask = 1U << bitIndex;
    if(value)
        set->values[byteIndex] |= mask;
    else
        set->values[byteIndex] &= ~mask;
}

void DrvEfuseSetValue32(efuseSet *set, u32 startBit, u32 endBit, u32 value) {
    int curByte, endByte, bitCnt;
    u8 mask;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        set->values[curByte] &= ~mask;
        set->values[curByte] |= (value << startBit) & mask;
        value >>= bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        set->values[curByte] = (u8)value;
        value >>= 8;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    set->values[curByte] &= ~mask;
    set->values[curByte] |= (value << startBit) & mask;
}

void DrvEfuseSetValue64(efuseSet *set, u32 startBit, u32 endBit, u64 value) {
    int curByte, endByte, bitCnt;
    u8 mask;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        set->values[curByte] &= ~mask;
        set->values[curByte] |= (value << startBit) & mask;
        value >>= bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        set->values[curByte] = (u8)value;
        value >>= 8;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    set->values[curByte] &= ~mask;
    set->values[curByte] |= (value << startBit) & mask;
}

void DrvEfuseSetValuePtr(efuseSet *set, u32 startBit, u32 endBit, const u8 *value) {
    int curByte, endByte, bitCnt, valBit;
    u8 mask;
    u16 valBuf;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    valBit = 8;
    valBuf = *value++;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        set->values[curByte] &= ~mask;
        set->values[curByte] |= (valBuf << startBit) & mask;
        valBuf >>= bitCnt;
        valBit -= bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        valBuf |= *value++ << valBit;
        set->values[curByte] = (u8)valBuf;
        valBuf >>= 8;
        curByte++;
    }
    bitCnt = endBit - startBit + 1;
    if(valBit < bitCnt)
        valBuf |= *value << valBit;
    mask = ((1U << bitCnt) - 1) << startBit;
    set->values[curByte] &= ~mask;
    set->values[curByte] |= (valBuf << startBit) & mask;
}

void DrvEfuseSetValuePtrRev(efuseSet *set, u32 startBit, u32 endBit, const u8 *value) {
    int startByte, curByte, bitCnt, valBit;
    u8 mask;
    u16 valBuf;

    startByte = startBit / 8;
    startBit %= 8;
    curByte = endBit / 8;
    endBit %= 8;

    valBit = 8;
    valBuf = *value++;

    if((curByte != startByte) && (endBit != 7)) {
        bitCnt = endBit + 1;
        mask = (1U << bitCnt) - 1;
        set->values[curByte] &= ~mask;
        set->values[curByte] |= valBuf & mask;
        valBuf >>= bitCnt;
        valBit -= bitCnt;
        endBit = 0;
        curByte--;
    }
    while(curByte != startByte) {
        valBuf |= *value++ << valBit;
        set->values[curByte] = (u8)valBuf;
        valBuf >>= 8;
        curByte--;
    }
    bitCnt = startBit - endBit;
    if(valBit < bitCnt)
        valBuf |= *value << valBit;
    mask = ((1U << bitCnt) - 1) << startBit;
    set->values[curByte] &= ~mask;
    set->values[curByte] |= (valBuf << startBit) & mask;
}

void DrvEfuseSetValueMask1(efuseMaskSet *maskSet, u32 bitIndex, u32 value) {
    int byteIndex = bitIndex / 8;
    u8 mask;
    bitIndex %= 8;
    mask = 1U << bitIndex;
    maskSet->mask.values[byteIndex] |= mask;
    if(value)
        maskSet->values.values[byteIndex] |= mask;
    else
        maskSet->values.values[byteIndex] &= ~mask;
}

void DrvEfuseSetValueMask32(efuseMaskSet *maskSet, u32 startBit, u32 endBit, u32 value) {
    int curByte, endByte, bitCnt;
    u8 mask;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        maskSet->mask.values[curByte] |= mask;
        maskSet->values.values[curByte] &= ~mask;
        maskSet->values.values[curByte] |= (value << startBit) & mask;
        value >>= bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        maskSet->mask.values[curByte] = 0xff;
        maskSet->values.values[curByte] = (u8)value;
        value >>= 8;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    maskSet->mask.values[curByte] |= mask;
    maskSet->values.values[curByte] &= ~mask;
    maskSet->values.values[curByte] |= (value << startBit) & mask;
}

void DrvEfuseSetValueMask64(efuseMaskSet *maskSet, u32 startBit, u32 endBit, u64 value) {
    int curByte, endByte, bitCnt;
    u8 mask;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        maskSet->mask.values[curByte] |= mask;
        maskSet->values.values[curByte] &= ~mask;
        maskSet->values.values[curByte] |= (value << startBit) & mask;
        value >>= bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        maskSet->mask.values[curByte] = 0xff;
        maskSet->values.values[curByte] = (u8)value;
        value >>= 8;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    maskSet->mask.values[curByte] |= mask;
    maskSet->values.values[curByte] &= ~mask;
    maskSet->values.values[curByte] |= (value << startBit) & mask;
}

void DrvEfuseSetValueMaskPtr(efuseMaskSet *maskSet, u32 startBit, u32 endBit, const u8 *value) {
    int curByte, endByte, bitCnt, valBit;
    u8 mask;
    u16 valBuf;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    valBit = 8;
    valBuf = *value++;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        maskSet->mask.values[curByte] |= mask;
        maskSet->values.values[curByte] &= ~mask;
        maskSet->values.values[curByte] |= (valBuf << startBit) & mask;
        valBuf >>= bitCnt;
        valBit -= bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        valBuf |= *value++ << valBit;
        maskSet->mask.values[curByte] = 0xff;
        maskSet->values.values[curByte] = (u8)valBuf;
        valBuf >>= 8;
        curByte++;
    }
    bitCnt = endBit - startBit + 1;
    if(valBit < bitCnt)
        valBuf |= *value << valBit;
    mask = ((1U << bitCnt) - 1) << startBit;
    maskSet->mask.values[curByte] |= mask;
    maskSet->values.values[curByte] &= ~mask;
    maskSet->values.values[curByte] |= (valBuf << startBit) & mask;
}

void DrvEfuseSetValueMaskPtrRev(efuseMaskSet *maskSet, u32 startBit, u32 endBit, const u8 *value) {
    int startByte, curByte, bitCnt, valBit;
    u8 mask;
    u16 valBuf;

    startByte = startBit / 8;
    startBit %= 8;
    curByte = endBit / 8;
    endBit %= 8;

    valBit = 8;
    valBuf = *value++;

    if((curByte != startByte) && (endBit != 7)) {
        bitCnt = endBit + 1;
        mask = (1U << bitCnt) - 1;
        maskSet->mask.values[curByte] |= mask;
        maskSet->values.values[curByte] &= ~mask;
        maskSet->values.values[curByte] |= valBuf & mask;
        valBuf >>= bitCnt;
        valBit -= bitCnt;
        endBit = 0;
        curByte--;
    }
    while(curByte != startByte) {
        valBuf |= *value++ << valBit;
        maskSet->mask.values[curByte] = 0xff;
        maskSet->values.values[curByte] = (u8)valBuf;
        valBuf >>= 8;
        curByte--;
    }
    bitCnt = startBit - endBit;
    if(valBit < bitCnt)
        valBuf |= *value << valBit;
    mask = ((1U << bitCnt) - 1) << startBit;
    maskSet->mask.values[curByte] |= mask;
    maskSet->values.values[curByte] &= ~mask;
    maskSet->values.values[curByte] |= (valBuf << startBit) & mask;
}

u32 DrvEfuseGetValue1(const efuseSet *set, u32 bitIndex) {
    int byteIndex = bitIndex / 8;
    u8 mask;
    bitIndex %= 8;
    mask = 1U << bitIndex;
    return !!(set->values[byteIndex] & mask);
}

u32 DrvEfuseGetValue32(const efuseSet *set, u32 startBit, u32 endBit) {
    int curByte, endByte, bitCnt, bitPos = 0;
    u8 mask;
    u32 value = 0;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        value = (set->values[curByte] & mask) >> startBit;
        bitPos = bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        value |= set->values[curByte] << bitPos;
        bitPos += 8;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    value |= ((set->values[curByte] & mask) >> startBit) << bitPos;
    return value;
}

u64 DrvEfuseGetValue64(const efuseSet *set, u32 startBit, u32 endBit) {
    int curByte, endByte, bitCnt, bitPos = 0;
    u8 mask;
    u64 value = 0;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        value = (set->values[curByte] & mask) >> startBit;
        bitPos = bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        value |= ((u64)set->values[curByte]) << bitPos;
        bitPos += 8;
        curByte++;
    }
    mask = ((1U << (endBit - startBit + 1)) - 1) << startBit;
    value |= ((u64)((set->values[curByte] & mask) >> startBit)) << bitPos;
    return value;
}

void DrvEfuseGetValuePtr(const efuseSet *set, u32 startBit, u32 endBit, u8 *value) {
    int curByte, endByte, bitCnt, bitPos = 0;
    u8 mask;
    u16 valBuf = 0;

    curByte = startBit / 8;
    startBit %= 8;
    endByte = endBit / 8;
    endBit %= 8;

    if((curByte != endByte) && (startBit != 0)) {
        bitCnt = 8 - startBit;
        mask = (((1U << bitCnt) - 1) << startBit);
        valBuf = (set->values[curByte] & mask) >> startBit;
        bitPos = bitCnt;
        startBit = 0;
        curByte++;
    }
    while(curByte != endByte) {
        valBuf |= set->values[curByte] << bitPos;
        *value++ = (u8)valBuf;
        valBuf >>= 8;
        curByte++;
    }
    bitCnt = endBit - startBit;
    mask = ((1U << (bitCnt + 1)) - 1) << startBit;
    valBuf |= ((set->values[curByte] & mask) >> startBit) << bitPos;
    *value++ = (u8)valBuf;
    if(bitCnt + bitPos >= 8)
        *value = valBuf >> 8;
}

int DrvEfuseCompareSets(const efuseSet *set1, const efuseSet *set2, efuseSet *outSet) {
    unsigned int i;
    int res;
    const u8 *p1 = set1->values, *p2 = set2->values;
    u8 *pd;
    if(outSet == NULL) {
        for(i=0; i<sizeof(set1->values); i++)
            if(*p1++ != *p2++)
                return 1;
        return 0;
    } else {
        pd = outSet->values;
        res = 0;
        for(i=0; i<sizeof(set1->values); i++)
            res = res || (*pd++ = (*p1++ ^ *p2++));
        return res;
    }
}

///@}
