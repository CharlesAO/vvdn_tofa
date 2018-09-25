.type mvcvConvolution3x3s2xhhx_asm,@function
.version 00.51.05

.data .data.convolution3x3
.align 16


.code .text.convolution3x3s2xhhx

//void Convolution3x3_asm(u8** in(i18), u8** out(i17), half conv[9](i16), u32 inWidth(i15))
mvcvConvolution3x3s2xhhx_asm:
     
            
        
        lsu0.ld.32    i0 i18 || lsu1.ld.32 i17 i17 
        lsu0.ldo.32   i1 i18 4   
        lsu0.ldo.32   i2 i18 8  
        nop 4
        lsu1.ldo.64.l v0 i16 0x00 || lsu0.ldo.64.h v0 i16 0x08 || iau.sub i0 i0 8
        lsu1.ldo.64.l v1 i16 0x10 || iau.sub i1 i1 8
  
        lsu1.ldil i11 0x00ff || iau.sub i2 i2 8 || vau.xor v20 v20 v20 
        cmu.cpivr.x8 v19 i11
        cmu.cpvv.u8.u16 v19 v19
        cmu.cpvv.i16.f16 v19 v19|| iau.xor i12 i12 i12
        
 .L_Conv3x3s3xhhx_loop:
        
        
        lsu0.ld.128.u8.f16 v2 i0 || iau.add i0 i0 8
        lsu0.ld.128.u8.f16 v3 i0 || iau.add i0 i0 8
        lsu0.ld.128.u8.f16 v4 i0 || iau.add i0 i0 8
   nop 5
        cmu.alignvec v5 v2 v3 14
        vau.mul.f16 v6 v0 v5 || lsu1.swzv8 [00000000]  ||iau.incs i12 8
        vau.mul.f16 v7 v0 v3 || lsu1.swzv8 [11111111]  ||cmu.alignvec v5 v3 v4 2 || lsu0.ld.128.u8.f16 v2 i1 || iau.add i1 i1 8 
        vau.mul.f16 v8 v0 v5 || lsu1.swzv8 [22222222]  ||lsu0.ld.128.u8.f16 v3 i1 || iau.add i1 i1 8
        lsu0.ld.128.u8.f16 v4 i1 || iau.add i1 i1 8
   nop 5
        cmu.alignvec v5 v2 v3 14
        vau.mul.f16 v9 v0 v5     || lsu1.swzv8 [33333333]
        vau.mul.f16 v10 v0 v3    || lsu1.swzv8 [44444444] || cmu.alignvec v5 v3 v4 2 || lsu0.ld.128.u8.f16 v2 i2 || iau.add i2 i2 8
        vau.mul.f16 v11 v0 v5    || lsu1.swzv8 [55555555] || lsu0.ld.128.u8.f16 v3 i2 || iau.add i2 i2 8
        lsu0.ld.128.u8.f16 v4 i2 || iau.add i2 i2 8       || vau.add.f16 v6 v6 v7
        vau.add.f16 v8 v8 v9
   nop 5
        cmu.alignvec v5 v2 v3 14
        vau.mul.f16 v12 v0 v5 || lsu1.swzv8 [66666666]
        vau.mul.f16 v13 v0 v3 || lsu1.swzv8 [77777777] || cmu.alignvec v5 v3 v4 2
        vau.mul.f16 v14 v1 v5 || lsu1.swzv8 [00000000] 
        
       
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
        cmu.clampab.f16 v6 v20 v19
        nop
        cmu.cpvv.f16.i16s v6 v6 
        nop 2
        cmu.cpvv.u16.u8s v6 v6 || iau.sub i0 i0 16
        nop 2
        cmu.vdilv.x8 v15 v5 v6 v6 || iau.sub i1 i1 16
        cmu.cpvi.x32 i13 v5.0 || iau.sub i2 i2 16
        lsu0.st.32 i13 i17 || iau.incs i17 4 || cmu.cmii.u32 i12 i15
        
        peu.pc1c LT || bru.bra .L_Conv3x3s3xhhx_loop
        nop 6 
        BRU.JMP i30
	NOP 6
	
.size mvcvConvolution3x3s2xhhx_asm,.-mvcvConvolution3x3s2xhhx_asm
.end	
	












      
