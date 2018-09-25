///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief apply a convolution 3x3 filter, in the case of symmetric kernel.
///

.type mvcvConvSeparable3x3_asm,@function
.version 00.51.04

.code .text.convSeparable3x3
.nowarn
//void convSeparable3x3_asm(u8* out(i18), u8** in(i17), fp32 conv[2](i16), u32 inWidth(i15))
.lalign
mvcvConvSeparable3x3_asm: 
    LSU0.LDo.64  i1,  i17, 0x00      || iau.shr.u32 i15, i15, 3
    LSU0.LD.64.l  v20, i16 ||LSU1.LDo.32  i3  i17, 0x08
    nop
    nop
    lsu0.ldil i12, _end_loop_convSeparable3x3_asm     ||  lsu1.ldih i12, _end_loop_convSeparable3x3_asm
    nop
    nop
    iau.sub i1, i1, 0x08
    cmu.cpvv.f32.f16 v20, v20   ||iau.sub i3, i3, 0x08          ||lsu0.ldi.128.u8f.f16 v1, i1 
    cmu.cpvi.x16 i0.l, v20.0    ||iau.sub i2, i2, 0x08          ||lsu0.ldi.128.u8f.f16 v3, i3
    cmu.cpivr.x16 v21, i0       ||lsu0.ldi.128.u8f.f16 v2, i2 
    cmu.cpvi.x16 i0.l, v20.1    ||lsu0.ldi.128.u8f.f16 v1, i1   ||lsu1.ldi.128.u8f.f16 v3, i3    
    cmu.cpivr.x16 v22, i0       ||lsu0.ldi.128.u8f.f16 v2, i2 
    lsu0.ldi.128.u8f.f16 v1, i1 
    lsu0.ldi.128.u8f.f16 v2, i2 
    lsu0.ldi.128.u8f.f16 v3, i3    
    vau.add.f16 v4, v1, v3
    vau.mul.f16 v5, v2, v22
    vau.add.f16 v4, v1, v3      ||lsu0.ldi.128.u8f.f16 v1, i1 ||lsu1.ldi.128.u8f.f16 v3, i3
    vau.mul.f16 v6, v4, v21
    vau.mul.f16 v5, v2, v22     ||lsu0.ldi.128.u8f.f16 v2, i2 
    vau.mul.f16 v6, v4, v21
    vau.add.f16 v7, v5, v6
    vau.add.f16 v4, v1, v3      ||lsu0.ldi.128.u8f.f16 v1, i1 ||lsu1.ldi.128.u8f.f16 v3, i3
    vau.add.f16 v8, v5, v6      ||lsu0.ldi.128.u8f.f16 v2, i2 
    vau.mul.f16 v5, v2, v22   
    vau.mul.f16 v6, v4, v21    
    vau.add.f16 v4, v1, v3      ||lsu0.ldi.128.u8f.f16 v1, i1 
    vau.mul.f16 v5, v2, v22     ||lsu0.ldi.128.u8f.f16 v2, i2
    vau.add.f16 v9, v5, v6      ||lsu0.ldi.128.u8f.f16 v3, i3
    vau.mul.f16 v6, v4, v21
    vau.add.f16 v4, v1, v3      ||cmu.alignvec v10, v7, v8, 14
    vau.mul.f16 v5, v2, v22     ||cmu.alignvec v12, v8, v9, 2
    vau.add.f16 v13, v10, v12   ||cmu.cpvv v11, v8
    vau.add.f16 v9, v5, v6      ||cmu.cpvv v7,v8
    vau.mul.f16 v6, v4, v21     ||cmu.cpvv v8,v9
    vau.mul.f16 v14, v11, v22
    vau.mul.f16 v15, v13, v21    
// start loop
.lalign
    vau.add.f16 v4, v1, v3      ||lsu0.ldi.128.u8f.f16 v1, i1  ||cmu.alignvec v10, v7, v8, 14   ||bru.rpl i12, i15, _delay_slot_convSeparable3x3_asm
_end_loop_convSeparable3x3_asm:    

    vau.mul.f16 v5, v2, v22     ||lsu0.ldi.128.u8f.f16 v2, i2  ||lsu1.ldi.128.u8f.f16 v3, i3    ||cmu.alignvec v12, v8, v9, 2

    vau.add.f16 v13, v10, v12   ||cmu.cpvv v11, v8
    vau.add.f16 v9, v5, v6      ||cmu.cpvv v7,v8
    vau.add.f16 v16, v14, v15   ||cmu.cpvv v8,v9
    vau.mul.f16 v6, v4, v21                            
    vau.mul.f16 v14, v11, v22
    vau.mul.f16 v15, v13, v21   ||lsu0.sti.128.f16.u8f v16, i18
_delay_slot_convSeparable3x3_asm:    
// end loop

BRU.jmp i30
nop 6
.nowarnend
.size mvcvConvSeparable3x3_asm,.-mvcvConvSeparable3x3_asm
.end
 
 
