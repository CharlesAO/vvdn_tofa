/*-----------------------------------------------------------------------------
  * The confidential and proprietary information contained in this file may
  * only be used by a person authorised under and to the extent permitted
  * by a subsisting licensing agreement from ARM Limited.
  *
  *            (C) COPYRIGHT 2013-2014 ARM Limited.
  *                ALL RIGHTS RESERVED
  *
  * This entire notice must be reproduced on all copies of this file
  * and copies of this file may only be made by a person if such person is
  * permitted to do so under the terms of a subsisting license agreement
  * from ARM Limited.
  *
  *      SVN Information
  *
  *      Checked In          : $Date: 2013-03-11 17:54:35 +0000 (Mon, 11 Mar 2013) $
  *
  *      Revision            : $Revision: 240933 $
  *
  *      Release Information : CORTEXA53-r0p4-00rel1
  *
  *-----------------------------------------------------------------------------
  */
 _RAM_START_LOS_DEFAULT = ABSOLUTE(0x70200000);
 _CMX_MEMORY_TOP_CACHED = ABSOLUTE(0x70280000);
 _CMX_MEMORY_TOP_UNCACHED = ABSOLUTE(0x78280000);
 /* Entry Point */
 ENTRY(bootcode)
 
  
 /*MEMORY
 {
   DDR (rwx)  : ORIGIN = 0x80000000, LENGTH = 0x1000000
   CMX (rwx)  : ORIGIN = 0x70000000, LENGTH = 0x200000
 }
 */
 
 
 SECTIONS
 {
  /* defining DDR sections */
  . = DEFINED( _DDR_SECT_START ) ? _DDR_SECT_START : (0x80000000);
  .ddr.text ALIGN(4) : {  *(.ddr.text*)
        *(.lrt.ddr.text*)
        *(.*.ddr.text*)
       }
  .ddr.rodata : { *(.ddr.rodata*) 
         *(.lrt.ddr.rodata*)
         *(.*.ddr.rodata*)
       }
  .ddr.data : { KEEP(*(.ddr.data*)) 
       KEEP(*(.lrt.ddr.data*)) 
       KEEP(*(.*.ddr.data*)) 
         }
  .ddr.bss (NOLOAD) : {
    __ddr_bss_start = ALIGN(8);
     *(.ddr.bss*)
     *(.lrt.ddr.bss*)
      *(.*.ddr.bss*)
    __ddr_bss_end = ALIGN(8);
  }
  DDR_END = .;

  /* defining DDR sections */
  . = . + 0x40000000;
  .ddr_direct.text ALIGN(4) : {  *(.ddr_direct.text*)
        *(.lrt.ddr_direct.text*)
        *(.*.ddr_direct.text*)
       }
  .ddr_direct.rodata : { *(.ddr_direct.rodata*) 
         *(.lrt.ddr_direct.rodata*)
         *(.*.ddr_direct.rodata*)
       }
  .ddr_direct.data : { KEEP(*(.ddr_direct.data*)) 
       KEEP(*(.lrt.ddr_direct.data*)) 
       KEEP(*(.*.ddr_direct.data*)) 
         }
  .ddr_direct.bss (NOLOAD) : {
    __ddr_direct_bss_start = ALIGN(8);
     *(.ddr_direct.bss*)
     *(.lrt.ddr_direct.bss*)
      *(.*.ddr_direct.bss*)
    __ddr_direct_bss_end = ALIGN(8);
  }
  DDR_UNCACHED_END = .;
   . = _RAM_START_LOS_DEFAULT;
   /* Code starts with vectors, then bootcode, then other code */
   .text :
   {
     *vectors.o(vectors)
     *bootcode.o(boot)
     *(.text) /* remainder of code */
   } =0
 
   /* Data */
    __data_start = .;
   .data  : { *(.data)  }
   .data1 : { *(.data1) }
 
   /* Notes section
    * This is not used so we discard it. Although not used it needs to be
    * explicitly mentioned in the linker script as some toolchains will place
    * the notes section at adderss 0 if it is not explicitly mentioned. */
   /DISCARD/ : { *(.note*) }
 
   /* Uninitialized data */
   __bss_start   = .;
   __bss_start__ = .;
   .bss   : { *(.bss)   }
   __bss_end   = .;
   __bss_end__ = .;
 
   . = ALIGN(4);
   __end   = .;
   __end__ = .;
 }
 
