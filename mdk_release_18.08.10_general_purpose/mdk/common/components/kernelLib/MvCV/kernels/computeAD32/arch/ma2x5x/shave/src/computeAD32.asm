/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvComputeAD32_asm,@function
.version 00.51.04

.data .data.mvcvComputeAD32
.align 4
.nowarn
.code .text.mvcvComputeAD32

.lalign
//void mvcvComputeAD32_asm_test(unsigned char *inputLeft, unsigned char *inputRight, unsigned char *output, unsigned int width)//
//                                               i18                  i17                     i16                   i15      
mvcvComputeAD32_asm:
cmu.cmz.i32 i14
peu.pc1c neq || bru.bra ___computeAD32flag
nop 6

lsu0.ldil i1 ___computeAD32Loop  || lsu1.ldih i1 ___computeAD32Loop || iau.sub i17 i17 0x1F
lsu0.ldi.8 i0 i18           || iau.shr.u32 i15 i15 3
lsu0.ld.128.u8.u16 v1 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v2 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v3 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v4 i17   || iau.add i17 i17 8
nop 2
cmu.cpii.u8.u16 i0 i0
cmu.cpivr.x16 v20 i0

vau.sub.i16 v10 v20 v1  || bru.rpl i1 i15
vau.sub.i16 v11 v20 v2  || lsu0.ld.128.u8.u16 v9 i17   || iau.add i17 i17 8     || lsu1.ldi.8 i0 i18  
vau.sub.i16 v12 v20 v3  || cmu.cpvv v10 v10            || lsu0.swzc8 [01234567]
vau.sub.i16 v13 v20 v4  || cmu.cpvv v11 v11            || lsu0.swzc8 [01234567]
cmu.cpvv v12 v12        || lsu0.swzc8 [01234567]
cmu.cpvv v13 v13        || lsu0.swzc8 [01234567]
cmu.alignvec v18 v1 v2 2  
vau.abs.i16 v13 v13     || cmu.alignvec v19 v2 v3 2       
vau.abs.i16 v12 v12     || lsu0.st.128.u16.u8 v13 i16  || iau.add i16 i16 8     || cmu.alignvec v21 v3 v4 2 
vau.abs.i16 v11 v11     || lsu0.st.128.u16.u8 v12 i16  || iau.add i16 i16 8     || cmu.alignvec v22 v4 v9 2
vau.abs.i16 v10 v10     || lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8     || cmu.cpii.u8.u16 i0 i0    
cmu.cpivr.x16 v20 i0     
 
vau.sub.i16 v10 v20 v18 || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8     
vau.sub.i16 v11 v20 v19 
vau.sub.i16 v12 v20 v21 || lsu0.ldi.8 i0 i18 
vau.sub.i16 v13 v20 v22 || cmu.cpvv v10 v10            || lsu0.swzc8 [01234567]
cmu.cpvv v11 v11        || lsu0.swzc8 [01234567]
cmu.cpvv v12 v12        || lsu0.swzc8 [01234567]
cmu.cpvv v13 v13        || lsu0.swzc8 [01234567]
vau.abs.i16 v13 v13     || cmu.alignvec v18 v1 v2 4
vau.abs.i16 v12 v12     || cmu.alignvec v19 v2 v3 4
vau.abs.i16 v11 v11     || lsu0.st.128.u16.u8 v13 i16  || iau.add i16 i16 8     || cmu.alignvec v21 v3 v4 4
vau.abs.i16 v10 v10     || lsu0.st.128.u16.u8 v12 i16  || iau.add i16 i16 8     || cmu.alignvec v22 v4 v9 4
cmu.cpii.u8.u16 i0 i0    
cmu.cpivr.x16 v20 i0      

vau.sub.i16 v11 v20 v19 || lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8    
vau.sub.i16 v10 v20 v18 || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8     
vau.sub.i16 v12 v20 v21 
vau.sub.i16 v13 v20 v22 
cmu.cpvv v10 v10        || lsu0.swzc8 [01234567]      || lsu1.ldi.8 i0 i18  
cmu.cpvv v11 v11        || lsu0.swzc8 [01234567]
cmu.cpvv v12 v12        || lsu0.swzc8 [01234567]
cmu.cpvv v13 v13        || lsu0.swzc8 [01234567]  
vau.abs.i16 v13 v13        
vau.abs.i16 v12 v12     || cmu.alignvec v18 v1 v2 6 
vau.abs.i16 v11 v11     || cmu.alignvec v19 v2 v3 6 
vau.abs.i16 v10 v10     || lsu0.st.128.u16.u8 v13 i16  || iau.add i16 i16 8    || cmu.alignvec v21 v3 v4 6
cmu.cpii.u8.u16 i0 i0  
cmu.cpivr.x16 v20 i0   

vau.sub.i16 v12 v20 v21 || lsu0.st.128.u16.u8 v12 i16  || iau.add i16 i16 8    || cmu.alignvec v22 v4 v9 6
vau.sub.i16 v11 v20 v19 || lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8   
vau.sub.i16 v10 v20 v18 || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8
vau.sub.i16 v13 v20 v22 
cmu.cpvv v10 v10        || lsu1.swzc8 [01234567]       || lsu0.ldi.8 i0 i18  
cmu.cpvv v11 v11        || lsu1.swzc8 [01234567]
cmu.cpvv v13 v13        || lsu1.swzc8 [01234567] 
cmu.cpvv v12 v12        || lsu1.swzc8 [01234567] 
vau.abs.i16 v13 v13     
vau.abs.i16 v12 v12     || lsu0.sti.128.u16.u8 v13 i16 
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 || cmu.alignvec v18 v1 v2 8
cmu.cpii.u8.u16 i0 i0
cmu.cpivr.x16 v20 i0

vau.sub.i16 v10 v20 v18 || lsu0.sti.128.u16.u8 v10 i16 || cmu.alignvec v19 v2 v3 8
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 8    || lsu0.ldi.8 i0 i18
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v9 8
vau.sub.i16 v13 v20 v22 
cmu.cpvv v10 v10        || lsu1.swzc8 [01234567] 
cmu.cpvv v13 v13        || lsu1.swzc8 [01234567] 
vau.abs.i16 v13 v13     || cmu.cpvv v12 v12            || lsu1.swzc8 [01234567] 
vau.abs.i16 v12 v12     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 
cmu.cpii.u8.u16 i0 i0 
cmu.cpivr.x16 v20 i0 

cmu.alignvec v18 v1 v2 10 || lsu0.sti.128.u16.u8 v10 i16  
vau.sub.i16 v10 v20 v18 || cmu.alignvec v19 v2 v3 10
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 10
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v9 10
vau.sub.i16 v13 v20 v22 
lsu0.ldi.8 i0 i18  
cmu.cpvv v13 v13        || lsu1.swzc8 [01234567]
cmu.cpvv v12 v12        || lsu1.swzc8 [01234567]
vau.abs.i16 v13 v13     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567]
vau.abs.i16 v12 v12     || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16 
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 
cmu.cpii.u8.u16 i0 i0  
cmu.cpivr.x16 v20 i0  
 
cmu.alignvec v18 v1 v2 12 || lsu0.sti.128.u16.u8 v10 i16 
vau.sub.i16 v10 v20 v18 || cmu.alignvec v19 v2 v3 12
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 12
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v9 12
vau.sub.i16 v13 v20 v22 
lsu0.ldi.8 i0 i18
cmu.cpvv v13 v13        || lsu1.swzc8 [01234567]
cmu.cpvv v12 v12        || lsu1.swzc8 [01234567]
vau.abs.i16 v13 v13     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567]
vau.abs.i16 v12 v12     || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 
cmu.cpii.u8.u16 i0 i0 
cmu.cpivr.x16 v20 i0 
 
cmu.alignvec v18 v1 v2 14 || lsu0.sti.128.u16.u8 v10 i16 
vau.sub.i16 v10 v20 v18 || cmu.alignvec v19 v2 v3 14   || lsu0.ldi.8 i0 i18 
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 14   || lsu0.cp v1 v2 
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v9 14   || lsu0.cp v2 v3
vau.sub.i16 v13 v20 v22 || lsu0.cp v3 v4
lsu0.cp v4 v9
___computeAD32Loop:
cmu.cpvv v13 v13        || lsu1.swzc8 [01234567]  
cmu.cpvv v12 v12        || lsu1.swzc8 [01234567] 
vau.abs.i16 v13 v13     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567]  
vau.abs.i16 v12 v12     || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16 
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 || cmu.cpii.u8.u16 i0 i0 
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 || cmu.cpivr.x16 v20 i0
lsu0.sti.128.u16.u8 v10 i16  



bru.jmp i30
nop 6

___computeAD32flag:
lsu0.ldil i1 ___computeAD32Loop2  || lsu1.ldih i1 ___computeAD32Loop2

lsu0.ldi.8 i0 i18           
lsu0.ld.128.u8.u16 v1 i17   || iau.add i17 i17 8 
lsu0.ld.128.u8.u16 v2 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v3 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v4 i17   || iau.add i17 i17 8
nop 2
cmu.cpii.u8.u16 i0 i0
cmu.cpivr.x16 v20 i0

vau.sub.i16 v10 v20 v1  || iau.sub i17 i17 0x1F || lsu0.ldi.8 i0 i18           || bru.rpl i1 i15
vau.sub.i16 v11 v20 v2  || lsu0.ld.128.u8.u16 v1 i17   || iau.add i17 i17 8
vau.sub.i16 v12 v20 v3  || lsu0.ld.128.u8.u16 v2 i17   || iau.add i17 i17 8
___computeAD32Loop2:
vau.sub.i16 v13 v20 v4  || lsu0.ld.128.u8.u16 v3 i17   || iau.add i17 i17 8
                           lsu0.ld.128.u8.u16 v4 i17   || iau.add i17 i17 8
vau.abs.i16 v10 v10     
vau.abs.i16 v11 v11     || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8     
vau.abs.i16 v12 v12     || lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8 || cmu.cpii.u8.u16 i0 i0    
vau.abs.i16 v13 v13     || lsu0.st.128.u16.u8 v12 i16  || iau.add i16 i16 8 || cmu.cpivr.x16 v20 i0    
                           lsu0.st.128.u16.u8 v13 i16  || iau.add i16 i16 8     




bru.jmp i30
nop 6
.nowarnend
.size mvcvComputeAD32_asm,.-mvcvComputeAD32_asm
.end
