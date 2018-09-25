///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution3x3s4hhhh_asm,@function
.version 00.51.04

.code .text.Convolution3x3s4hhhh


//internal computation are made on fp16, output result is fp16
mvcvConvolution3x3s4hhhh_asm:

        
        lsu0.ld.32    i0 i18     || lsu1.ld.32 i17 i17 
        lsu0.ldo.32   i1 i18 4   
        lsu0.ldo.32   i2 i18 8  
        
        lsu1.ldo.64.l v0 i16 0x00 || lsu0.ldo.64.h v0 i16 0x08
        lsu1.ldo.64.l v1 i16 0x10
   nop 6
        iau.sub i0 i0 16
        iau.sub i1 i1 16
        iau.sub i2 i2 16
        iau.xor i12 i12 i12
      
        
   nop 4
   convolution3x3s4hhhh_loop:
        iau.incs i12 8
        
        lsu0.ld.64.l v2 i0 ||lsu1.ldo.64.h v2 i0 8 
        lsu0.ldo.64.l v3 i0 16 || lsu1.ldo.64.h v3 i0 24
        lsu0.ldo.64.l v4 i0 32 || lsu1.ldo.64.h v4 i0 40
       
   nop 5
        cmu.alignvec v5 v2 v3 14
        vau.mul.f16 v6 v0 v5 || lsu1.swzv8 [00000000]
        vau.mul.f16 v7 v0 v3 || lsu1.swzv8 [11111111] || cmu.alignvec v5 v3 v4 2
        vau.mul.f16 v8 v0 v5 || lsu1.swzv8 [22222222] 
     
        lsu0.ld.64.l v2 i1     || lsu1.ldo.64.h v2 i1 8 
        lsu0.ldo.64.l v3 i1 16 || lsu1.ldo.64.h v3 i1 24
        lsu0.ldo.64.l v4 i1 32 || lsu1.ldo.64.h v4 i1 40
   nop 5
        cmu.alignvec v5 v2 v3 14
        vau.mul.f16 v9 v0 v5  || lsu1.swzv8 [33333333]
        vau.mul.f16 v10 v0 v3 || lsu1.swzv8 [44444444] || cmu.alignvec v5 v3 v4 2
        vau.mul.f16 v11 v0 v5 || lsu1.swzv8 [55555555] 
           
        lsu0.ld.64.l v2 i2 ||lsu1.ldo.64.h v2 i2 8 
        lsu0.ldo.64.l v3 i2 16 || lsu1.ldo.64.h v3 i2 24
        lsu0.ldo.64.l v4 i2 32 || lsu1.ldo.64.h v4 i2 40
   nop 5
        cmu.alignvec v5 v2 v3 14
        vau.mul.f16 v12 v0 v5 || lsu1.swzv8 [66666666]
        vau.mul.f16 v13 v0 v3 || lsu1.swzv8 [77777777] || cmu.alignvec v5 v3 v4 2
        vau.mul.f16 v14 v1 v5 || lsu1.swzv8 [00000000] 
        
        vau.add.f16 v6 v6 v7
        vau.add.f16 v8 v8 v9
        vau.add.f16 v10 v10 v11
        vau.add.f16 v12 v12 v13
   
        vau.add.f16 v6 v6 v8
   nop
        vau.add.f16 v10 v10 v12
   nop 2
        vau.add.f16 v6 v6 v10
   nop 2
        vau.add.f16 v6 v6 v14
   nop 2
       cmu.vdilv.x16 v15 v5 v6 v6
       nop 
       cmu.vdilv.x16 v17 v18 v5 v5
       nop 
       cmu.cpvi.x32 i13 v18.0
       
       lsu0.st.32 i13 i17 || iau.incs i17 4
       nop 6
      
       iau.add i0 i0 16
       iau.add i1 i1 16
       iau.add i2 i2 16
       nop 6
       cmu.cmii.u32 i12 i15
       nop 6
       peu.pc1c LT || bru.bra convolution3x3s4hhhh_loop 
       nop 6
       BRU.JMP i30
       nop 6
.size mvcvConvolution3x3s4hhhh_asm,.-mvcvConvolution3x3s4hhhh_asm
.end
