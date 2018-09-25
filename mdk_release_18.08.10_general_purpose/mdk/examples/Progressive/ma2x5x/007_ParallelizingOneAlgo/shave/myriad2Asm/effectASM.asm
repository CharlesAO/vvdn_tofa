///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.version 00.51.05


.code .text.BrightenLineAsm

//Calling convention
//void BrightenLineAsm(u8* inLine(i18), u8* outLine(i17), int width(i16))//

BrightenLineAsm:

    //Small setup phase. Two v registers need to be filled with 100
    //(brightness value to add) and 255 (value used for clamping)
    lsu0.ldil i0  255   ||  lsu1.ldih i0  255
    cmu.cpivr.x32 v1 i0 || lsu0.ldil i0 30 || lsu1.ldih i0 30
    cmu.cpivr.x32 v2 i0

    //Processing eight pixels batches. Get the number of batches to process
    iau.shr.u32 i15 i16 3

    //Using three underscores label to make it local(see moviAsm documentation about local labels)
 effectASM_EightPixelsPatch:
        lsu0.ld.128.u8.u16 v0 i18 || iau.add i18 i18 8
        nop
        nop
        nop
        nop
        nop
        nop

        //Result available now. Need to add 100 and clamp.
        vau.add.i16 v0 v0 v2
        nop
        cmu.clamp0.i16 v0 v0 v1
        nop
        lsu0.st.128.u16.u8 v0 i17 || iau.add i17 i17 8


        iau.sub i15 i15 1
        peu.pc1i GT || bru.bra effectASM_EightPixelsPatch
        nop
        nop
        nop
        nop
        nop
	    nop


        bru.jmp i30
        nop
        nop
        nop
        nop
        nop
        nop

.end
