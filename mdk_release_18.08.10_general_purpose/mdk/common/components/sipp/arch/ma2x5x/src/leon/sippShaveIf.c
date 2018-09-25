///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

//Shave symbols used by Master Shave Unit kick
#if defined(SIPP_PC)
#include "wrapperSem.h"
extern u32         dbg_svu_no;
extern Semaphore * svuSyncSem;
#endif

extern int               SVU_SYM(SHAVE_MAIN)(void);

//###########################################################################
//This should is attempted only after all shaves finished current iteration
//and are polling for MUTEX access.
void sippStopSvus(SippPipeline *pl)
{
  #if defined(MYRIAD2)
  u32 svu;

  if(pl->nFiltersSvu)
  {
    //Stop all shaves:
      for (svu = pl->gi.sliceFirst; svu <= pl->gi.sliceLast; svu++)
      {
         DrvSvutStop(svu);
         //SET_REG_WORD(DCU_OCR(svu),                         OCR_STOP_GO); // Set STOP bit in control register
         SET_REG_WORD(SVU_CTRL_ADDR[svu] + SLC_OFFSET_SVU + SVU_IRR, 0x1F); // Clear ints from previous run
         SET_REG_WORD(SVU_CTRL_ADDR[svu] + SLC_OFFSET_SVU + SVU_ICR, 0x20); // Enable SWI interrupt
      }

    //Wait for all to finish:
      for (svu = pl->gi.sliceFirst; svu <= pl->gi.sliceLast; svu++)
      {
#ifndef POWER_MEASURE_NO_SHAVES
          swcWaitShave(svu);
#endif
      }

    //Clear mutexes requests:
      SET_REG_WORD(pl->svuSyncMtxAddr, pl->svuSyncMtx[0] | MTX_CLEAR_ALL_PENDING);
      SET_REG_WORD(pl->svuSyncMtxAddr, pl->svuSyncMtx[1] | MTX_CLEAR_ALL_PENDING);
  }

  #endif
}

//###########################################################################
// - set window regs (code section could be common if outside CMX)
// - load shave elf
//###########################################################################
void sippSetupSvus(SippPipeline *pl)
{
    #if defined(__sparc)
    u32 s;
    u32 sliceSz = pl->gi.sliceSize;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Setup Shaves Win-regs (only using win 0 and 1)
      for (s = pl->gi.sliceFirst; s <= pl->gi.sliceLast; s++)
      {
        sippSetShaveWindow(s, 0, pl->svuWinRegs[0] + s * sliceSz);

       //Allow Shaves to Share the code sections:
       //If code section is located outside CMX, then assume it's going to be a unique
       //section shared via cmxL2 cache by all shaves
        if(pl->flags & PLF_MAP_SVU_CODE_IN_DDR)
           sippSetShaveWindow(s, 1, pl->svuWinRegs[1]); //Shared code
        else //else typical section duplication: each shave with it's own code in own slice
           sippSetShaveWindow(s, 1, pl->svuWinRegs[1] + s * sliceSz);

      }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Load the mbin
    #if defined(MYRIAD2)
        sippLoadMbinMulti(pl->mbinImg, pl->gi.sliceFirst, pl->gi.sliceLast);
    #else //myriad1
      for (s = pl->gi.sliceFirst; s <= pl->gi.sliceLast; s++)
        swcLoadshvdlib(pl->mbinImg, s);
    #endif

 #endif
}


//####################################################################################
// [Master Shave unit] kick routine (will kick all shaves in group)
//####################################################################################
void sippKickShaveM1PC(SippPipeline *pl)
{
   u32 s;
   u32 sliceFirst = pl->gi.sliceFirst;
   u32 sliceLast  = pl->gi.sliceLast;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   #if defined(SIPP_PC)
      //note: "sipp_pl" is properly initialized in "sipp_init_svus"
      //1)~~~~~~~~~~ Hpadding for all slices:
       pl->svuCmd = CMD_H_PAD | CMD_EXIT;
       for(s=sliceFirst; s<=sliceLast; s++)
       {
           dbg_svu_no = s;
           SHAVE_MAIN();
       }

       //2)~~~~~~~~~~~ Run for all slices
       pl->svuCmd = CMD_RUN | CMD_EXIT;
       for(s=sliceFirst; s<=sliceLast; s++)
       {
           dbg_svu_no = s;
           SHAVE_MAIN();
       }

    svuSyncSem->Post();

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   #elif defined(__sparc) //Myriad1
      pl->svuCmd = CMD_H_PAD | CMD_RUN | CMD_EXIT;
      for(s=sliceFirst; s<=sliceLast; s++)
      {
#ifndef POWER_MEASURE_NO_SHAVES
          swcResetShave(s);
#endif
        //Compiler doesn't init stack anymore in main
        //so need to do it manually
          DrvSvutIrfWrite(s, 19, pl->svuStack);
          swcStartShave(s, (u32)(SVU_SYM(SHAVE_MAIN)));
      }
   #endif
}

//####################################################################################
//Myriad1 only !
void sippWaitShave(SippPipeline *pl)
{
  #if defined(__sparc)
    u32 s;
    u32 sliceFirst = pl->gi.sliceFirst;
    u32 sliceLast  = pl->gi.sliceLast;

    for(s=sliceFirst; s<=sliceLast; s++) {
#ifndef POWER_MEASURE_NO_SHAVES
        swcWaitShave(s);
#endif
    }
  #endif
}



#if defined(__sparc)

#include <DrvSvuDefines.h>


#if defined(MYRIAD2)
#include <../myriad2/src/swcShaveLoaderPrivate.h>
#else
#include <../myriad1/src/swcShaveLoaderPrivate.h>
#endif

//clean routines (mdk routines are broken; I already spent to much time with mdk issues)
//########################################################################################
//svuNo: 0..11
//winNo: 0..3 (A,B,C,D)
void sippSetShaveWindow(u32 svuNo, u32 winNo, u32 value)
{
  //Calculate address of the Slice config addr
    u32 sliceBase = SHAVE_0_BASE_ADR + SVU_SLICE_OFFSET * svuNo;

  //Set the proper WINDOW register
    SET_REG_WORD(sliceBase + SLC_TOP_OFFSET_WIN_A + 4 * winNo, value);
}

//########################################################################################
u32 sippGetShaveWindow(u32 svuNo, u32 winNo)
{
  //Calculate address of the Slice config addr
    u32 sliceBase = SHAVE_0_BASE_ADR + SVU_SLICE_OFFSET * svuNo;

  //Set the proper WINDOW register
    return GET_REG_WORD_VAL(sliceBase + SLC_TOP_OFFSET_WIN_A + 4 * winNo);
}

//########################################################################################
u32 sippSolveShaveAddr(u32 inAddr, u32 sliceNo)
{
    u32 resolved;
    switch (inAddr >> 24)
    {
        case 0x1C: resolved = (inAddr & 0x00FFFFFF) + sippGetShaveWindow(sliceNo, 0); break;
        case 0x1D: resolved = (inAddr & 0x00FFFFFF) + sippGetShaveWindow(sliceNo, 1); break;
        case 0x1E: resolved = (inAddr & 0x00FFFFFF) + sippGetShaveWindow(sliceNo, 2); break;
        case 0x1F: resolved = (inAddr & 0x00FFFFFF) + sippGetShaveWindow(sliceNo, 3); break;
        default  : resolved =  inAddr;  break; //absolute address, no translation is to be done
    }
    return resolved;
}

static inline void moveStringUncached(const u8 * restrict src, int max, char* restrict dst)
{
    if ((u32)dst & 0x80000000)
        dst = ((char *)((u32) dst | 0x40000000));
    else
        dst = ((char *)((u32) dst | 0x08000000)); // Assume CMX

    while(max--){
        *dst++=*src++;
    }
    *dst = '\0';
}

static inline uint32_t getUnsigned(const u8 * src)
{
    uint32_t val=0;
    for(uint32_t i=0;i<4;i++){
        val=val*10+src[i]-'0';
        //TODO: if(src[i]<'0' || src[i]>'9') treat this also
    }
    return val;
}

//########################################################################################
//Compute maximum size for Code and Data section
void sippGetSvuSecSize(u8 *svuElf, u32 *maxCode, u32 *maxData)
{
    Elf32Ehdr ElfHeader;
    u32 SecHeaders;
    u32 phAddr;
    u32 SecSize;

    *maxCode = 0x1D000000; //Code
    *maxData = 0x1C000000; //StaticData, Stack, DynamicData(lnPool)

    // check if the loaded context has debug information at the beginning of the ELF
    // number of the ELF header
    if(*(u32*)svuElf != 0x464c457f){
        unsigned int stringLength;

        // get the length of the name from the loaded library
        stringLength = getUnsigned(svuElf);
        // set the address to the beginning of the elf section header
        svuElf += 4 + ((stringLength + 3) & 0xFFFFFFFC);
    }

    //get the section header
    swcU32memcpy((u32*)&ElfHeader, (u32*)svuElf, sizeof(Elf32Ehdr));

    //Reading section headers table offset
    phAddr = (u32)svuElf + ElfHeader.eShoff;

    //parse section headers:
    for (SecHeaders = 0; SecHeaders < ElfHeader.eShnum; SecHeaders++)
    {
        Elf32Section ElfSecHeader;
        u32 SecOffset;

        SecOffset = phAddr + sizeof(Elf32Section) * SecHeaders;
        swcU32memcpy((u32*)&ElfSecHeader, (u32*)SecOffset, sizeof(Elf32Section));

        SecSize = ElfSecHeader.shSize;

      //Only look at PROGBITS sections
        if ((ElfSecHeader.shType == SHT_PROGBITS) && (SecSize > 0))
        {
            if((ElfSecHeader.shAddr >> 24) == 0x1D) //code
            {
                if((ElfSecHeader.shAddr + SecSize) > *maxCode)
                    *maxCode = ElfSecHeader.shAddr + SecSize;
            }

            if((ElfSecHeader.shAddr >> 24) == 0x1C) //data
            {
                if((ElfSecHeader.shAddr + SecSize) > *maxData)
                    *maxData = ElfSecHeader.shAddr + SecSize;
            }
        }
    }

  //Subtract offsets and round to 16Bytes
    *maxCode -= 0x1D000000; *maxCode = (*maxCode + 15) & (~15);
    *maxData -= 0x1C000000; *maxData = (*maxData + 15) & (~15);

    VCS_PRINT_INT(*maxCode);
    VCS_PRINT_INT(*maxData);
}

//########################################################################################
void sippLoadMbinMulti(u8 *sAddr, u32 firstSlc, u32 lastSlc)
{
    Elf32Ehdr ElfHeader;
    u32 SecHeaders;
    u32 phAddr, s;
    u32 *srcAddr;
    u32 *dstAddr;
    u32 SecSize;

    // check if the loaded context has debug information at the beginning of the ELF
    // the first byte from the magic number of the ELF header
    if(*(u32*)sAddr != 0x464c457f){
        unsigned int stringLength;

        // get the length of the name from the loaded library
        stringLength = getUnsigned(sAddr);
        // set the address to the beginning of the elf section header
        sAddr += 4 + ((stringLength + 3) & 0xFFFFFFFC);
    }

    //get the section header
    swcU32memcpy((u32*)&ElfHeader, (u32*)sAddr, sizeof(Elf32Ehdr));

    //Reading section headers table offset
    phAddr = (u32)sAddr + ElfHeader.eShoff;

    //parse section headers:
    for (SecHeaders = 0; SecHeaders < ElfHeader.eShnum; SecHeaders++)
    {
        Elf32Section ElfSecHeader;
        u32 SecOffset;
        u32 SecDataOffset;

        SecOffset = phAddr + sizeof(Elf32Section) * SecHeaders;
        swcU32memcpy((u32*)&ElfSecHeader, (u32*)SecOffset, sizeof(Elf32Section));
        SecDataOffset = (u32)sAddr + ElfSecHeader.shOffset;

        srcAddr = (u32*)SecDataOffset;

        SecSize = ElfSecHeader.shSize;

      //Only load PROGBITS sections
        if ((ElfSecHeader.shType == SHT_PROGBITS) && (SecSize > 0))
        {
         //Copy to all required destinations
           for(s=firstSlc; s<=lastSlc; s++)
           {
              dstAddr = (u32*)sippSolveShaveAddr(ElfSecHeader.shAddr, s);

             #if defined(SIPP_VCS)
              vcsHookFastMemCpy((void*)dstAddr, (void*)srcAddr, SecSize);
             #else
              swcU32memcpy((u32*)dstAddr, (u32*)srcAddr, SecSize);
             #endif
           }
       }
    }
}

#endif

//###########################################################################
void sippKickSvus (SippPipeline *pl)
{
    #if defined(__sparc)
    u32 s;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Tell shaves where is the main Pipeline structure
    for (s = pl->gi.sliceFirst; s <= pl->gi.sliceLast; s++)
    {
        *((SippPipeline **)sippSolveShaveAddr((u32)&SVU_SYM(sipp_pl), s)) = pl;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //M2 only: Reset and Start shaves just once at pipeline creation
    for (s = pl->gi.sliceFirst; s <= pl->gi.sliceLast; s++)
    {
        //u32 data;

        //Reset Shave and slice stuff (not sure how much is required).
        //Shaves are already HATLTED, no need to set STOP bit again !
        // Reset selection signals
        //    parameter   SVE_RST_L1C   = 5;
        //    parameter   SVE_RST_FIFO  = 4;
        //    parameter   SVE_RST_TLB   = 3;
        //    parameter   SVE_RST_SLCT  = 2;
        //    parameter   SVE_RST_DCU   = 1;
        //    parameter   SVE_RST_SVU   = 0;

        #ifndef POWER_MEASURE_NO_SHAVES
        swcResetShave(s);
        #endif


        //L1-data cache only caches DDR accesses, so for now don't reset (would require re-enable + invalidation)
        //L1_CACHE_RESET resets both D-dache and I-cache !

        /*
        //Cancel bSize consideration that's embedded in opcodes:
        extern u32 SVU_CTRL_ADDR[TOTAL_NUM_SHAVES];
        data  = GET_REG_WORD_VAL(SVU_CTRL_ADDR[s] + SLC_OFFSET_SVU + SVU_BRUCTL);
        data |= (1<<25);
        SET_REG_WORD(SVU_CTRL_ADDR[s] + SLC_OFFSET_SVU + SVU_BRUCTL, data);
        */

        #if 1 //VCS debug only !
        //Enable profiling ocunters
        DrvSvuEnablePerformanceCounter(s, 0,  1<<8); //Go count enable
        DrvSvuEnablePerformanceCounter(s, 1,  1<<1); //Executed instruction count
        #endif


        // So now we should check if the runtime wants us to register an interrupt handler and
        // also what then pass interrupt handler actaully is....
        if (pl->useSyncRuntime == 0)
        {
            SET_REG_WORD (DCU_IRR(s), 0xFFFFFFFF);     // Clear any interrupts from previous test
            SET_REG_WORD (DCU_ICR(s), ICR_SWI_ENABLE); // Enable SWIC interrupt
            DrvIcbSetupIrq(IRQ_SVE_0 + s,  5, POS_EDGE_INT,  sippSvuDoneIrqHandler);
        }
        else
        {
            DrvIcbDisableIrq (IRQ_SVE_0 + s);
        }

        //Start Shave
        DrvSvutIrfWrite(s, 19, pl->svuStack);
        #ifndef POWER_MEASURE_NO_SHAVES
        DrvSvuStart(s, (u32)SVU_SYM(SHAVE_MAIN));
        #endif
    }

    #elif defined(SIPP_PC)
    SVU_SYM(sipp_pl) = pl;  //Tell shave where is the pipe struct
    #endif
}
