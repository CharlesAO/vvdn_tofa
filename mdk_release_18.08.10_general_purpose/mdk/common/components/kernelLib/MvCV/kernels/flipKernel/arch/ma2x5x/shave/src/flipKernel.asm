.type mvcvFlipKernel_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
///mvcvFlipKernel - flips elements of a given line (first element becames last)
//    @param[in] in             - pointer to input line
//    @param[out] out           - pointer to output line
//    @param[in] width          - width of the input line
//    @return    Nothing
//    
    



.data .rodata.mvcvFlipKernel_asm
.align 4
.salign 16

.code .text.mvcvFlipKernel_asm
.salign 16

.lalign
//mvcvFlipKernel(u8 *in, u8 *out, u32 width)
//                  i18      i17        i16
mvcvFlipKernel_asm:
.nowarn
    LSU0.ldil i14, 0x40 || LSU1.ldih i14, 0x00
    cmu.cmii.i32 i16 i14 
    peu.pc1c lt || bru.bra FlipKernel_compensate
    IAU.SHR.u32 i0 i16 6
    IAU.SHL i1 i0 6    
    IAU.SUB i2 i16 i1
    LSU0.ldil i15, FlipKernel_loop || LSU1.ldih i15, FlipKernel_loop
    iau.add i17 i17 i16
    iau.sub i17 i17 8
    

    iau.shr.u32 i16 i16 6
    lsu0.ldi.128.u8.u16 v0 i18 
    lsu0.ldi.128.u8.u16 v1 i18 
    lsu0.ldi.128.u8.u16 v2 i18 
    lsu0.ldi.128.u8.u16 v3 i18 
    lsu0.ldi.128.u8.u16 v4 i18 
    lsu0.ldi.128.u8.u16 v5 i18 
    lsu0.ldi.128.u8.u16 v6 i18 
    lsu0.ldi.128.u8.u16 v7 i18 


    cmu.cpvv  v8 v0 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v0 i18 || bru.rpl i15 i16
    cmu.cpvv  v9 v1 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v1 i18 
    cmu.cpvv v10 v2 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v2 i18 
    cmu.cpvv v11 v3 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v3 i18 
    cmu.cpvv v12 v4 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v4 i18 
    cmu.cpvv v13 v5 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v5 i18 
    cmu.cpvv v14 v6 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v6 i18 
    cmu.cpvv v15 v7 || LSU1.SWZC8 [01234567] || lsu0.ldi.128.u8.u16 v7 i18 
    lsu0.st.128.u16.u8  v8 i17 || iau.sub i17 i17 8
    FlipKernel_loop:
    lsu0.st.128.u16.u8  v9 i17 || iau.sub i17 i17 8
    lsu0.st.128.u16.u8 v10 i17 || iau.sub i17 i17 8
    lsu0.st.128.u16.u8 v11 i17 || iau.sub i17 i17 8
    lsu0.st.128.u16.u8 v12 i17 || iau.sub i17 i17 8
    lsu0.st.128.u16.u8 v13 i17 || iau.sub i17 i17 8
    lsu0.st.128.u16.u8 v14 i17 || iau.sub i17 i17 8
    lsu0.st.128.u16.u8 v15 i17 || iau.sub i17 i17 8

    iau.sub i18 i18 0x40
    FlipKernel_compensate:
    cmu.cmz.i32 i2
    peu.pc1c eq || bru.bra FlipKernel_exit
    lsu0.ldi.128.u8.u16 v0 i18 
    lsu0.ldi.128.u8.u16 v1 i18 
    lsu0.ldi.128.u8.u16 v2 i18 
    lsu0.ldi.128.u8.u16 v3 i18 
    lsu0.ldi.128.u8.u16 v4 i18 
    lsu0.ldi.128.u8.u16 v5 i18 
    lsu0.ldi.128.u8.u16 v6 i18 
    lsu0.ldi.128.u8.u16 v7 i18 
    cmu.cpvv  v8 v0 || LSU0.SWZC8 [01234567] 
    cmu.cpvv  v9 v1 || LSU0.SWZC8 [01234567] 
    cmu.cpvv v10 v2 || LSU0.SWZC8 [01234567] 
    cmu.cpvv v11 v3 || LSU0.SWZC8 [01234567] 
    cmu.cpvv v12 v4 || LSU0.SWZC8 [01234567] 
    cmu.cpvv v13 v5 || LSU0.SWZC8 [01234567] 
    cmu.cpvv v14 v6 || LSU0.SWZC8 [01234567] 
    cmu.cpvv v15 v7 || LSU0.SWZC8 [01234567] 
    
    lsu0.st.128.u16.u8  v8 i17 || iau.sub i17 i17 8
    iau.sub i2 i2 8
    peu.pc1i gt || lsu0.st.128.u16.u8  v9 i17 || iau.sub i17 i17 8
    iau.sub i2 i2 8
    peu.pc1i gt || lsu0.st.128.u16.u8 v10 i17 || iau.sub i17 i17 8
    iau.sub i2 i2 8
    peu.pc1i gt || lsu0.st.128.u16.u8 v11 i17 || iau.sub i17 i17 8
    iau.sub i2 i2 8
    peu.pc1i gt || lsu0.st.128.u16.u8 v12 i17 || iau.sub i17 i17 8
    iau.sub i2 i2 8
    peu.pc1i gt || lsu0.st.128.u16.u8 v13 i17 || iau.sub i17 i17 8
    iau.sub i2 i2 8
    peu.pc1i gt || lsu0.st.128.u16.u8 v14 i17 || iau.sub i17 i17 8
    iau.sub i2 i2 8
    peu.pc1i gt || lsu0.st.128.u16.u8 v15 i17 || iau.sub i17 i17 8

FlipKernel_exit:

bru.jmp i30
nop 6
.size mvcvFlipKernel_asm,.-mvcvFlipKernel_asm
.nowarnend
.end
