/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   shaveCRT0.S
 *  Description:   This file implements the startup code for running a
 *                 standalone program on the SHAVE.
 *
 *                 It defines the function '_EP_start' which is the effective
 *                 CRT0 Entry-Point for a standalone C/C++ program.  This  also
 *                 passes on the arguments from 'I18' and 'I17' to 'main' as
 *                 its 'argc' and 'argv' values.
 *
 *                 The '_EP_start' symbol is also exposed as an IAT entry.
 *
 *  --------------------------------------------------------------------------- */


.file __FILE__


#include "assembly_portability_support.h"


//-------------------------------------------------------------------------------
//

.data .data.regsaves
.align 4
___MainAddress:
  .int 0x00000000

.code   .text..crt0._EP_start
.align  16
.type   _EP_start, @function

    // Effective signature is '__attribute__((dllexport)) int _EP_start(int, const char*[])'
    _EP_start:
        //Save the address of the main function from the application
        LSU0.LDIL i1, ___MainAddress || LSU1.LDIH i1, ___MainAddress
        LSU0.ST.32 i29, i1

        LSU0.LDIL             i30, __crtinit
            || LSU1.LDIH      i30, __crtinit
        BRU.SWP               i30                 // Execute the system initialisation '__crtinit'
            || LSU0.LDIL      i0, __stackMaximumExtent
            || LSU1.LDIH      i0, __stackMaximumExtent
        LSU1.STO.32           i20, i0, 0          // Capture the maximum stack address (I20) and Context pointer (I21)
            || CMU.CPII       i22, i18            // Save 'argc' for 'main'
            || IAU.OR         i23, i17, i17       // Save 'argv' for 'main'
        LSU1.STO.32           i19, i0, 8          // Capture the original SP address
        NOP                   4

        // Jump to 'main' and fake the return address so that it comes back to 'exit'
        LSU0.LDIL             i29, ___MainAddress
            || LSU1.LDIH      i29, ___MainAddress

        LSU0.LDO.32           i30, i29, 0

        NOP                   6

        BRU.JMP               i30                 // Call 'main'
            || CMU.CPII       i18, i22            // Set 'argc' to what was passed to '_EP_start' in IRF:18
            || IAU.OR         i17, i23, i23       // Set 'argv' to what was passed to '_EP_start' in IRF:17
            || LSU0.LDIL      i30, exit
            || LSU1.LDIH      i30, exit           // Fake the return address to go directly to 'exit'
        NOP                   __SHAVE_BRANCH_DELAY_SLOTS__
.size   _EP_start, .-_EP_start


// This symbol is required by the 'cxa_atexit' mechanism, but its use is simply as a cookie
.data   .rodata4..crt.dso_handle
.align  4

    __dso_handle:
        .word   0x5A5A5A5A


// The following exposes '_EP_start' as an IAT function
.data   ..iat._EP_start
.align  4

        .word   _EP_start

.data   ..str..iatstrings

    .L.iatname_0:
        .string "_EP_start"


.data   ..iatnames._EP_start
.align  4

        .word   .L.iatname_0

.data   .gnu.linkonce..iatnamesend
.linkonce
.align  4

        .word   0


.end
