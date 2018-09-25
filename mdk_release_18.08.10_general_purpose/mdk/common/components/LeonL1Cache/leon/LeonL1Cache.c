///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Unit Test Framework
///
/// Basic utility functions for unit testing
///
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "assert.h"
#include "registersMyriad.h"
#include "stdio.h"
#include "LeonL1CacheApi.h"
#include "swcLeonUtils.h"
#include "DrvRegUtils.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define WORD_SIZE_BITS  (2)
#define WORD_MASK       (0xFFFFFFFC)
                                     // NOTE: This definition is hardcoded to 32 byte line sizes, as this is the case in all 
                                     // movidius Leon processors
#define WORD_INDEX_MASK (0x0000001C) // Selects the bits of the address that index the word within a line

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
                   
// 4: Static Local Data
// ----------------------------------------------------------------------------
// This is a local cache of basic cache params
static tyCacheInfo iCacheInfo;
static tyCacheInfo dCacheInfo;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void LeonL1CacheInitDiagAccess(void)
{
    int i;
    u32 cacheConfigReg;
    u32 iCacheConfigReg;
    u32 dCacheConfigReg;
    tyCacheInfo * pCacheInfo;

    iCacheConfigReg = swcRead32Asi02(ICACHE_CONFIG_REG_OFS);
    dCacheConfigReg = swcRead32Asi02(DCACHE_CONFIG_REG_OFS);

    for (i=0;i<2;i++)
    {
        if (i==0) // First process iCache
        {
            pCacheInfo          = &iCacheInfo;
            cacheConfigReg      = iCacheConfigReg;
        }
        else    // Then process dCache 
        {
            pCacheInfo          = &dCacheInfo;
            cacheConfigReg      = dCacheConfigReg;
        }

        pCacheInfo->lineSizeBits   = DrvRegGetBitField(&cacheConfigReg,16,(18 - 16 + 1));
        pCacheInfo->lineSizeBits  += 2; // Compensates for the fact that lineSize reported in words (2^2)
        pCacheInfo->lineSizeBytes  = (1<<pCacheInfo->lineSizeBits);


        pCacheInfo->waySizeBits    = DrvRegGetBitField(&cacheConfigReg,20,(23 - 20 + 1)); // 2^WSIZE KB (per way)
        pCacheInfo->waySizeBits   += 10; // Compensate for the fact that size is reported in KB (2^10) 
        pCacheInfo->waySizeBytes   = (1<<pCacheInfo->waySizeBits);

        pCacheInfo->numWays        = DrvRegGetBitField(&cacheConfigReg,24,(26 - 24 + 1)) + 1;

        pCacheInfo->totalSizeBytes = pCacheInfo->waySizeBytes * pCacheInfo->numWays;

        pCacheInfo->numLinesPerWay = (1 << (pCacheInfo->waySizeBits - pCacheInfo->lineSizeBits));

        pCacheInfo->tagOffsetMask  = ( ((1 << pCacheInfo->waySizeBits) - 1) - (( 1 << pCacheInfo->lineSizeBits) - 1) );
        pCacheInfo->dataOffsetMask = ( ((1 << pCacheInfo->waySizeBits) - 1) - (( 1 << WORD_SIZE_BITS         ) - 1) );

        pCacheInfo->mmuSupported   = DrvRegGetBitField(&cacheConfigReg, 3,1);
        pCacheInfo->snoopSupported = DrvRegGetBitField(&cacheConfigReg,27,(27 - 27 + 1));

        pCacheInfo->replicationPolicy = DrvRegGetBitField(&cacheConfigReg,28,(29 - 28 + 1));
        pCacheInfo->writePolicy       = WRITE_THROUGH;
    }

    return;
}

u32 LeonL1DCacheReadTagForAddr(u32 address, u32 way)
{
    u32 tagAddress = address & dCacheInfo.tagOffsetMask;
    tagAddress    += (way << dCacheInfo.waySizeBits);     // Add in the way offset  
    return swcRead32Asi0E(tagAddress); 
}

u32 LeonL1ICacheReadTagForAddr(u32 address, u32 way)
{
    u32 tagAddress = address & iCacheInfo.tagOffsetMask;
    tagAddress    += (way << iCacheInfo.waySizeBits);     // Add in the way offset  
    return swcRead32Asi0C(tagAddress); 
}


u32 LeonL1DCacheReadDataWordForAddr(u32 address, u32 way)
{
    u32 dataAddress = address & dCacheInfo.dataOffsetMask;  // Mask off the upper bits and word align
    dataAddress    += (way <<  dCacheInfo.waySizeBits);     // Add in the way offset  
    return swcRead32Asi0F(dataAddress); 
}

u32 LeonL1ICacheReadDataWordForAddr(u32 address, u32 way)
{
    u32 dataAddress = address & iCacheInfo.dataOffsetMask;  // Mask off the upper bits and word align
    dataAddress    += (way <<  iCacheInfo.waySizeBits);     // Add in the way offset  
    return swcRead32Asi0D(dataAddress); 
}


void LeonL1CacheGetInfo(tyCacheInfo ** ppICacheInfo,tyCacheInfo ** ppDCacheInfo)
{
    if (ppICacheInfo != NULL)
        *ppICacheInfo = &iCacheInfo;
    if (ppDCacheInfo != NULL)
        *ppDCacheInfo = &dCacheInfo;
    return;
}


void LeonL1CacheDisplayInfo(tyCacheType cache)
{
    tyCacheInfo * pCacheInfo;

    if (cache == L1_ICACHE)
    {
        printf("\n I-Cache Paramerters:\n");
        pCacheInfo     = &iCacheInfo;
    }
    else
    {
        printf("\n D-Cache Paramerters:\n");
        pCacheInfo     = &dCacheInfo;
    }

    printf("\n Total Cache Size : %lu bytes",pCacheInfo->totalSizeBytes);
    printf("\n Way Size         : %lu bytes",pCacheInfo->waySizeBytes);
    printf("\n Num Ways         : %lu",pCacheInfo->numWays);
    printf("\n Num Lines per way: %lu",pCacheInfo->numLinesPerWay);
    printf("\n Lines Size       : %lu bytes",pCacheInfo->lineSizeBytes);
    printf("\n MMU Support      : %lu",pCacheInfo->mmuSupported);
    printf("\n Snoop Support    : %lu",pCacheInfo->snoopSupported);

    printf("\n lineSizeBits     : %lu",pCacheInfo ->lineSizeBits);
    printf("\n waySizeBits      : %lu",pCacheInfo ->waySizeBits);
    printf("\n tagOffsetMask    : %08lX",pCacheInfo ->tagOffsetMask);
    printf("\n dataOffsetMask   : %08lX",pCacheInfo ->dataOffsetMask);


    switch (pCacheInfo->replicationPolicy)
    {
    case NONE:
        printf("\n Replace Policy   : None (Direct Mapped)\n");
        break;
    case LRU:                    
        printf("\n Replace Policy   : Least Recently Used\n");
        break;
    case RAND:
        printf("\n Replace Policy   : Random\n");
        break;
    default:
        assert(FALSE);
        break;
    }
    return;
}

u32 LeonL1CacheIsAddressICached(u32 address)
{
    u32 wordOffset;
    u32 tag;
    u32 tagMask;
    u32 found=0;
    u32 way;
    wordOffset   = address & WORD_INDEX_MASK;
    wordOffset   = wordOffset >> WORD_SIZE_BITS;
    tagMask = ~((1 << iCacheInfo.waySizeBits) -1);
    for (way=0;way<iCacheInfo.numWays;way++)
    {
        tag = LeonL1ICacheReadTagForAddr(address,way);
        if (tag & ( 1 << wordOffset)) // First check if word is valid, as otherwise it doesn't matter
        {
            if ((tag & tagMask) == (address & tagMask))
            {         
                found = 1;
            }
        }
    }
    return found;
}

u32 LeonL1CacheIsAddressDCached(u32 address)
{
    u32 wordOffset;
    u32 tag;
    u32 tagMask;
    u32 found=0;
    u32 way;
    wordOffset   = address & WORD_INDEX_MASK;
    wordOffset   = wordOffset >> WORD_SIZE_BITS;
    tagMask = ~((1 << dCacheInfo.waySizeBits) -1);
    for (way=0;way<dCacheInfo.numWays;way++)
    {
        tag = LeonL1DCacheReadTagForAddr(address,way);
        if (tag & ( 1 << wordOffset)) // First check if word is valid, as otherwise it doesn't matter
        {
            if ((tag & tagMask) == (address & tagMask))
            {         
                found = 1;
            }
        }
    }
    return found;
}

u32 LeonL1CacheDiagCountValidLines(tyCacheType cache)
{
    u32 totalValidLines=0;
    u32 address;
    u32 way;                      
    u32 tag;
    u32 validBits;
    tyCacheInfo * pCacheInfo;

    u32 (*readTagForAddr)(u32 address, u32 way);

    if (cache == L1_ICACHE)
    {
        readTagForAddr = &LeonL1ICacheReadTagForAddr;
        pCacheInfo     = &iCacheInfo;
    }
    else
    {
        readTagForAddr = &LeonL1DCacheReadTagForAddr;
        pCacheInfo     = &dCacheInfo;
    }

    for (way=0; way < pCacheInfo->numWays ;way++)
    {
        for (address=0;address<pCacheInfo->totalSizeBytes;address+=pCacheInfo->lineSizeBytes)
        {
            tag = readTagForAddr(address,way);
            validBits = tag & 0xFF;
            if (validBits)
                totalValidLines++;
        }
    }

    return totalValidLines;
}

void LeonL1CacheDiagDisplay(tyCacheType cache)
{
    u32 address;
    u32 tag;
    u32 way;
    u32 validBits;
    u32 tagAddress;
    u32 cacheConfigReg;
    UNUSED(cacheConfigReg);
    u32 iCacheConfigReg;
    u32 dCacheConfigReg;
    tyCacheInfo * pCacheInfo;
    u32 (*readTagForAddr)(u32 address, u32 way);
    u32 (*readDataForAddr)(u32 address, u32 way);
    u32 numLinesFound=0;

    iCacheConfigReg = swcRead32Asi02(ICACHE_CONFIG_REG_OFS);
    dCacheConfigReg = swcRead32Asi02(DCACHE_CONFIG_REG_OFS);

    if (cache == L1_ICACHE)
    {
        puts("\nDumping ICACHE Contents:\n");
        readTagForAddr      = &LeonL1ICacheReadTagForAddr;
        readDataForAddr     = &LeonL1ICacheReadDataWordForAddr;
        pCacheInfo          = &iCacheInfo;
        cacheConfigReg      = iCacheConfigReg;
    }
    else
    {
        puts("\nDumping DCACHE Contents:\n");
        readTagForAddr      = &LeonL1DCacheReadTagForAddr;
        readDataForAddr     = &LeonL1DCacheReadDataWordForAddr;
        pCacheInfo          = &dCacheInfo;
        cacheConfigReg      = dCacheConfigReg;
    }


    for (address=0;address<pCacheInfo->totalSizeBytes;address+=pCacheInfo->lineSizeBytes)
    {
        for (way=0; way < pCacheInfo->numWays ;way++)
        {
            tag = readTagForAddr(address,way);
            validBits = tag & 0xFF;
            tagAddress = (tag & (~(pCacheInfo->tagOffsetMask)) & (~0x1F)) + address; // Clearing all the bottom bits
            if (validBits) // Only display lines in which some words are valid
            {

                printf("\n0x%08lX V:%01lX : %08lX %08lX %08lX %08lX",tagAddress,(validBits&0xF)
                                                                , readDataForAddr(address+0x0,way)
                                                                , readDataForAddr(address+0x4,way) 
                                                                , readDataForAddr(address+0x8,way) 
                                                                , readDataForAddr(address+0xC,way) );

                if (pCacheInfo->lineSizeBytes == 0x20)
                {
                    printf("\n0x%08lX V:%01lX : %08lX %08lX %08lX %08lX",tagAddress+0x10,((validBits>>4)&0xF)
                                                                    , readDataForAddr(address+0x10,way)
                                                                    , readDataForAddr(address+0x14,way) 
                                                                    , readDataForAddr(address+0x18,way) 
                                                                    , readDataForAddr(address+0x1C,way) );
                }
                numLinesFound++;
            }
        }
    }

    printf("\nTotal Valid Lines: %lu\n\n",numLinesFound);

    return;
}



