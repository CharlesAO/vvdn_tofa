///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvConvolution3x3s3hhhh_asm,@function
.version 00.51.04

.code .text.Convolution3x3s3hhhh
.nowarn 10

//internal computation are made on fp16, output result is fp16
mvcvConvolution3x3s3hhhh_asm:

        
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
        iau.xor i8 i8 i8 
        iau.add i8 i8 24
        nop 6
        sau.div.i32 i8 i8 i15
        nop 13
        iau.sub i8 i15 i8
        vau.xor v22 v22 v22
        nop 4

// the rule is x = width/24  then  y=x*24 then z=width-y 
// if z==0 then the convolution3x3s3hhhh_loop works fine
// if z==16 the output has a surplus of 3 elements 
// if z==8 the output has a surplus of 6 elements

        IAU.XOR i3 i3 i3
        IAU.ADD i3 i3 24
        IAU.XOR i10 i10 i10
        IAU.ADD i10 i10 16
        
        CMU.CMII.u32 i10 i15
        PEU.PCXC EQ,0 || BRU.BRA .Lcompensate_3
        nop 6
       
        SAU.DIV.u32 i9 i15 i3
        nop 13
        IAU.MUL i9 i9 i3
        nop 
        IAU.SUB i9 i15 i9
         
        CMU.CMZ.i32 i9
        PEU.PCXC EQ,0 || BRU.BRA convolution3x3s3hhhh_loop
        nop 6
        iau.incs i12 24  // skip a loop, and use compensate instead
        
convolution3x3s3hhhh_loop:
        
        iau.incs i12 24
//--        
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
       
        cmu.cpvi.x16 i11.l v6.0
        cmu.cpvi.x16 i13.l v6.3
        cmu.cpvi.x16 i14.l v6.6
        NOP 6
     
        lsu0.st.16 i11 i17 || iau.incs i17 2
        lsu0.st.16 i13 i17 || iau.incs i17 2
        lsu0.st.16 i14 i17 || iau.incs i17 2
       
        iau.add i0 i0 16
        iau.add i1 i1 16
        iau.add i2 i2 16
        
        
//--
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
       
        cmu.cpvi.x16 i11.l v6.1
        cmu.cpvi.x16 i13.l v6.4
        cmu.cpvi.x16 i14.l v6.7
        NOP 6
        
        lsu0.st.16 i11 i17 || iau.incs i17 2
        lsu0.st.16 i13 i17 || iau.incs i17 2
        lsu0.st.16 i14 i17 || iau.incs i17 2
       
        iau.add i0 i0 16
        iau.add i1 i1 16
        iau.add i2 i2 16

        
//--          
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
       
        cmu.cpvi.x16 i11.l v6.2
        cmu.cpvi.x16 i13.l v6.5
        NOP 6
        lsu0.st.16 i11 i17 || iau.incs i17 2
        lsu0.st.16 i13 i17 || iau.incs i17 2
        iau.add i0 i0 16
        iau.add i1 i1 16
        iau.add i2 i2 16
        nop 6
        cmu.cmii.u32 i12 i15
        nop 6
        peu.pc1c LT || bru.bra convolution3x3s3hhhh_loop 
        nop 6
      
      
      
        CMU.CMII.i32 i9 i10 
        PEU.PCXC EQ,0 || BRU.BRA .Lcompensate_3
        nop 6
        IAU.SUB i10 i10 8
        CMU.CMII.i32 i9 i10
        PEU.PCXC EQ,0 || BRU.BRA .Lcompensate_6
        nop 6
        
BRU.JMP i30
nop 6
 
// widths that cause a surplus of 3 elements
.Lcompensate_3:
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
       
        cmu.cpvi.x16 i11.l v6.0
        cmu.cpvi.x16 i13.l v6.3
        cmu.cpvi.x16 i14.l v6.6
        NOP 6
       
        lsu0.st.16 i11 i17 || iau.incs i17 2
        lsu0.st.16 i13 i17 || iau.incs i17 2
        lsu0.st.16 i14 i17 || iau.incs i17 2
       
        iau.add i0 i0 16
        iau.add i1 i1 16
        iau.add i2 i2 16

        
//--         
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
       
        cmu.cpvi.x16 i11.l v6.1
        cmu.cpvi.x16 i13.l v6.4
        cmu.cpvi.x16 i14.l v6.7
        NOP 6
        lsu0.st.16 i11 i17 || iau.incs i17 2
        lsu0.st.16 i13 i17 || iau.incs i17 2
  
BRU.JMP i30
nop 6

//widths that cause a surplus of 6 elements        
.Lcompensate_6:
    
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
       
       cmu.cpvi.x16 i11.l v6.0
       cmu.cpvi.x16 i13.l v6.3
       cmu.cpvi.x16 i14.l v6.6
       NOP 6
       lsu0.st.16 i11 i17 || iau.incs i17 2
       lsu0.st.16 i13 i17 || iau.incs i17 2
 
 
BRU.JMP i30
nop 6
nop 6
.nowarnend
.size mvcvConvolution3x3s3hhhh_asm,.-mvcvConvolution3x3s3hhhh_asm
.end
