/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvComputeAD64_asm,@function
.version 00.51.04

.data .data.mvcvComputeAD64
.align 4

.code .text.mvcvComputeAD64
.nowarn
.lalign
//void mvcvComputeAD64_asm_test(unsigned char *inputLeft, unsigned char *inputRight, unsigned char *output, unsigned int width)//
//                                               i18                  i17                     i16                   i15      
mvcvComputeAD64_asm:
cmu.cmz.i32 i14
peu.pc1c neq || bru.bra ___computeAD64flag
nop 6

iau.sub i17 i17 0x3f
lsu0.ldil i1 ___computeAD64Loop  || lsu1.ldih i1 ___computeAD64Loop
lsu0.ld.128.u8.u16 v1 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v2 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v3 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v4 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v5 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v6 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v7 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v8 i17   || iau.add i17 i17 8
lsu0.ldi.8 i0 i18           || iau.shr.u32 i15 i15 3
nop 7
cmu.cpii.u8.u16 i0 i0
cmu.cpivr.x16 v20 i0


vau.sub.i16 v10 v20 v1  || bru.rpl i1 i15
vau.sub.i16 v11 v20 v2  || lsu0.ld.128.u8.u16 v9 i17   || iau.add i17 i17 8     || lsu1.ldi.8 i0 i18  
vau.sub.i16 v12 v20 v3  || cmu.cpvv v10 v10            || lsu0.swzc8 [01234567]
vau.sub.i16 v13 v20 v4  || cmu.cpvv v11 v11            || lsu0.swzc8 [01234567]
vau.sub.i16 v14 v20 v5  || cmu.cpvv v12 v12            || lsu0.swzc8 [01234567]
vau.sub.i16 v15 v20 v6  || cmu.cpvv v13 v13            || lsu0.swzc8 [01234567]
vau.sub.i16 v17 v20 v8  || cmu.cpvv v14 v14            || lsu0.swzc8 [01234567]  
vau.sub.i16 v16 v20 v7  || cmu.cpvv v15 v15            || lsu0.swzc8 [01234567]        
cmu.cpvv v17 v17        || lsu0.swzc8 [01234567]
vau.abs.i16 v17 v17     || cmu.cpvv v16 v16            || lsu0.swzc8 [01234567]
vau.abs.i16 v16 v16     || lsu0.st.128.u16.u8 v17 i16  || iau.add i16 i16 8     || cmu.cpii.u8.u16 i0 i0
vau.abs.i16 v15 v15     || lsu0.st.128.u16.u8 v16 i16  || iau.add i16 i16 8     || cmu.cpivr.x16 v20 i0
vau.abs.i16 v14 v14     || lsu0.st.128.u16.u8 v15 i16  || iau.add i16 i16 8     || cmu.alignvec v18 v1 v2 2
vau.abs.i16 v13 v13     || lsu0.st.128.u16.u8 v14 i16  || iau.add i16 i16 8     || cmu.alignvec v19 v2 v3 2
vau.abs.i16 v12 v12     || lsu0.st.128.u16.u8 v13 i16  || iau.add i16 i16 8     || cmu.alignvec v21 v3 v4 2 
vau.abs.i16 v11 v11     || lsu0.st.128.u16.u8 v12 i16  || iau.add i16 i16 8     || cmu.alignvec v22 v4 v5 2
vau.abs.i16 v10 v10     || lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8     || cmu.alignvec v23 v5 v6 2
   
vau.sub.i16 v10 v20 v18 || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8     || cmu.alignvec v18 v6 v7 2
vau.sub.i16 v11 v20 v19 || cmu.alignvec v19 v8 v9 2
vau.sub.i16 v12 v20 v21 || cmu.alignvec v21 v7 v8 2    || lsu0.ldi.8 i0 i18 
vau.sub.i16 v13 v20 v22 || cmu.cpvv v10 v10            || lsu0.swzc8 [01234567]
vau.sub.i16 v14 v20 v23 || cmu.cpvv v11 v11            || lsu0.swzc8 [01234567]
vau.sub.i16 v15 v20 v18 || cmu.cpvv v12 v12            || lsu0.swzc8 [01234567]
vau.sub.i16 v17 v20 v19 || cmu.cpvv v13 v13            || lsu0.swzc8 [01234567]
vau.sub.i16 v16 v20 v21 || cmu.cpvv v14 v14            || lsu0.swzc8 [01234567]
cmu.cpvv v17 v17        || lsu0.swzc8 [01234567] 
vau.abs.i16 v17 v17     || cmu.cpvv v16 v16            || lsu0.swzc8 [01234567]
vau.abs.i16 v16 v16     || cmu.cpvv v15 v15            || lsu0.swzc8 [01234567]
vau.abs.i16 v15 v15     || lsu0.st.128.u16.u8 v17 i16  || iau.add i16 i16 8     || cmu.cpii.u8.u16 i0 i0
vau.abs.i16 v14 v14     || lsu0.st.128.u16.u8 v16 i16  || iau.add i16 i16 8     || cmu.cpivr.x16 v20 i0
vau.abs.i16 v13 v13     || lsu0.st.128.u16.u8 v15 i16  || iau.add i16 i16 8     || cmu.alignvec v18 v1 v2 4
vau.abs.i16 v12 v12     || lsu0.st.128.u16.u8 v14 i16  || iau.add i16 i16 8     || cmu.alignvec v19 v2 v3 4 
vau.abs.i16 v11 v11     || lsu0.st.128.u16.u8 v13 i16  || iau.add i16 i16 8     || cmu.alignvec v21 v3 v4 4
vau.abs.i16 v10 v10     || lsu0.st.128.u16.u8 v12 i16  || iau.add i16 i16 8     || cmu.alignvec v22 v4 v5 4
   
vau.sub.i16 v11 v20 v19 || lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8     || cmu.alignvec v23 v5 v6 4
vau.sub.i16 v10 v20 v18 || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8     || cmu.alignvec v18 v6 v7 4
vau.sub.i16 v12 v20 v21 || cmu.alignvec v19 v7 v8 4
vau.sub.i16 v13 v20 v22 || cmu.alignvec v21 v8 v9 4
vau.sub.i16 v14 v20 v23 || cmu.cpvv v10 v10            || lsu0.swzc8 [01234567] ||lsu1.ldi.8 i0 i18  
vau.sub.i16 v15 v20 v18 || cmu.cpvv v11 v11            || lsu0.swzc8 [01234567]
vau.sub.i16 v17 v20 v21 || cmu.cpvv v12 v12            || lsu0.swzc8 [01234567]
vau.sub.i16 v16 v20 v19 || cmu.cpvv v13 v13            || lsu0.swzc8 [01234567]
cmu.cpvv v17 v17        || lsu0.swzc8 [01234567] 
vau.abs.i16 v17 v17     || cmu.cpvv v14 v14            || lsu0.swzc8 [01234567]
vau.abs.i16 v16 v16     || cmu.cpvv v15 v15            || lsu0.swzc8 [01234567]
vau.abs.i16 v15 v15     || cmu.cpvv v16 v16            || lsu0.swzc8 [01234567]
vau.abs.i16 v14 v14     || lsu0.st.128.u16.u8 v17 i16  || iau.add i16 i16 8    || cmu.cpii.u8.u16 i0 i0
vau.abs.i16 v13 v13     || lsu0.st.128.u16.u8 v16 i16  || iau.add i16 i16 8    || cmu.cpivr.x16 v20 i0
vau.abs.i16 v12 v12     || lsu0.st.128.u16.u8 v15 i16  || iau.add i16 i16 8    || cmu.alignvec v18 v1 v2 6
vau.abs.i16 v11 v11     || lsu0.st.128.u16.u8 v14 i16  || iau.add i16 i16 8    || cmu.alignvec v19 v2 v3 6
vau.abs.i16 v10 v10     || lsu0.st.128.u16.u8 v13 i16  || iau.add i16 i16 8    || cmu.alignvec v21 v3 v4 6

vau.sub.i16 v12 v20 v21 || lsu0.st.128.u16.u8 v12 i16  || iau.add i16 i16 8    || cmu.alignvec v22 v4 v5 6
vau.sub.i16 v11 v20 v19 || lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8    || cmu.alignvec v23 v5 v6 6
vau.sub.i16 v10 v20 v18 || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8
vau.sub.i16 v13 v20 v22 || cmu.alignvec v18 v6 v7 6
vau.sub.i16 v14 v20 v23 || cmu.alignvec v19 v7 v8 6
vau.sub.i16 v15 v20 v18 || cmu.alignvec v21 v8 v9 6
vau.sub.i16 v17 v20 v21 || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.ldi.8 i0 i18  
vau.sub.i16 v16 v20 v19 || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567]
cmu.cpvv v17 v17        || lsu1.swzc8 [01234567] 
vau.abs.i16 v17 v17     || cmu.cpvv v16 v16            || lsu1.swzc8 [01234567]
vau.abs.i16 v16 v16     || cmu.cpvv v15 v15            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v17 i16
vau.abs.i16 v15 v15     || cmu.cpvv v14 v14            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v16 i16
vau.abs.i16 v14 v14     || cmu.cpvv v13 v13            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v15 i16
vau.abs.i16 v13 v13     || cmu.cpvv v12 v12            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v14 i16
vau.abs.i16 v12 v12     || lsu0.sti.128.u16.u8 v13 i16 || cmu.cpii.u8.u16 i0 i0
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 || cmu.cpivr.x16 v20 i0
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 || cmu.alignvec v18 v1 v2 8

vau.sub.i16 v10 v20 v18 || lsu0.sti.128.u16.u8 v10 i16 || cmu.alignvec v19 v2 v3 8
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 8
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v5 8
vau.sub.i16 v13 v20 v22 || cmu.alignvec v23 v5 v6 8
vau.sub.i16 v14 v20 v23 || cmu.alignvec v18 v6 v7 8
vau.sub.i16 v15 v20 v18 || cmu.alignvec v21 v8 v9 8
vau.sub.i16 v17 v20 v21 || cmu.alignvec v19 v7 v8 8
vau.sub.i16 v16 v20 v19 || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.ldi.8 i0 i18
cmu.cpvv v17 v17        || lsu1.swzc8 [01234567] 
vau.abs.i16 v17 v17     || cmu.cpvv v16 v16            || lsu1.swzc8 [01234567]
vau.abs.i16 v16 v16     || cmu.cpvv v15 v15            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v17 i16 
vau.abs.i16 v15 v15     || cmu.cpvv v14 v14            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v16 i16 
vau.abs.i16 v14 v14     || cmu.cpvv v13 v13            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v15 i16 
vau.abs.i16 v13 v13     || cmu.cpvv v12 v12            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v14 i16 
vau.abs.i16 v12 v12     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 || cmu.cpii.u8.u16 i0 i0
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 || cmu.cpivr.x16 v20 i0
 
cmu.alignvec v18 v1 v2 10 || lsu0.sti.128.u16.u8 v10 i16  
vau.sub.i16 v10 v20 v18 || cmu.alignvec v19 v2 v3 10
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 10
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v5 10
vau.sub.i16 v13 v20 v22 || cmu.alignvec v23 v5 v6 10
vau.sub.i16 v14 v20 v23 || cmu.alignvec v21 v8 v9 10
vau.sub.i16 v17 v20 v21 || cmu.alignvec v19 v7 v8 10
vau.sub.i16 v16 v20 v19 || cmu.alignvec v18 v6 v7 10
vau.sub.i16 v15 v20 v18 || cmu.cpvv v17 v17            || lsu1.swzc8 [01234567] || lsu0.ldi.8 i0 i18  
cmu.cpvv v16 v16        || lsu1.swzc8 [01234567] 
vau.abs.i16 v17 v17     || cmu.cpvv v15 v15            || lsu1.swzc8 [01234567]
vau.abs.i16 v16 v16     || cmu.cpvv v14 v14            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v17 i16
vau.abs.i16 v15 v15     || cmu.cpvv v13 v13            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v16 i16 
vau.abs.i16 v14 v14     || cmu.cpvv v12 v12            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v15 i16
vau.abs.i16 v13 v13     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v14 i16
vau.abs.i16 v12 v12     || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16 
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 || cmu.cpii.u8.u16 i0 i0
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 || cmu.cpivr.x16 v20 i0
  
cmu.alignvec v18 v1 v2 12 || lsu0.sti.128.u16.u8 v10 i16 
vau.sub.i16 v10 v20 v18 || cmu.alignvec v19 v2 v3 12
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 12
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v5 12
vau.sub.i16 v13 v20 v22 || cmu.alignvec v23 v5 v6 12
vau.sub.i16 v14 v20 v23 || cmu.alignvec v21 v8 v9 12
vau.sub.i16 v17 v20 v21 || cmu.alignvec v19 v7 v8 12
vau.sub.i16 v16 v20 v19 || cmu.alignvec v18 v6 v7 12
vau.sub.i16 v15 v20 v18 || cmu.cpvv v17 v17            || lsu1.swzc8 [01234567] || lsu0.ldi.8 i0 i18
cmu.cpvv v16 v16        || lsu1.swzc8 [01234567]
vau.abs.i16 v17 v17     || cmu.cpvv v15 v15            || lsu1.swzc8 [01234567]
vau.abs.i16 v16 v16     || cmu.cpvv v14 v14            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v17 i16
vau.abs.i16 v15 v15     || cmu.cpvv v13 v13            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v16 i16 
vau.abs.i16 v14 v14     || cmu.cpvv v12 v12            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v15 i16 
vau.abs.i16 v13 v13     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v14 i16 
vau.abs.i16 v12 v12     || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 || cmu.cpii.u8.u16 i0 i0
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 || cmu.cpivr.x16 v20 i0
 
cmu.alignvec v18 v1 v2 14 || lsu0.sti.128.u16.u8 v10 i16 
vau.sub.i16 v10 v20 v18 || cmu.alignvec v19 v2 v3 14
vau.sub.i16 v11 v20 v19 || cmu.alignvec v21 v3 v4 14   || lsu0.cp v1 v2 
vau.sub.i16 v12 v20 v21 || cmu.alignvec v22 v4 v5 14   || lsu0.cp v2 v3
vau.sub.i16 v13 v20 v22 || cmu.alignvec v23 v5 v6 14   || lsu0.cp v3 v4
vau.sub.i16 v14 v20 v23 || cmu.alignvec v21 v8 v9 14   || lsu0.cp v4 v5
vau.sub.i16 v17 v20 v21 || cmu.alignvec v19 v7 v8 14   || lsu0.cp v5 v6
vau.sub.i16 v16 v20 v19 || cmu.alignvec v18 v6 v7 14   || lsu0.cp v6 v7
vau.sub.i16 v15 v20 v18 || cmu.cpvv v17 v17            || lsu1.swzc8 [01234567] || lsu0.ldi.8 i0 i18  
cmu.cpvv v16 v16        || lsu1.swzc8 [01234567]       || lsu0.cp v7 v8
vau.abs.i16 v17 v17     || cmu.cpvv v15 v15            || lsu1.swzc8 [01234567]
vau.abs.i16 v16 v16     || cmu.cpvv v14 v14            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v17 i16

___computeAD64Loop:
vau.abs.i16 v15 v15     || cmu.cpvv v13 v13            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v16 i16 
vau.abs.i16 v14 v14     || cmu.cpvv v12 v12            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v15 i16
vau.abs.i16 v13 v13     || cmu.cpvv v11 v11            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v14 i16 
vau.abs.i16 v12 v12     || cmu.cpvv v10 v10            || lsu1.swzc8 [01234567] || lsu0.sti.128.u16.u8 v13 i16 
vau.abs.i16 v11 v11     || lsu0.sti.128.u16.u8 v12 i16 || cmu.cpii.u8.u16 i0 i0 || lsu1.cp v8 v9
vau.abs.i16 v10 v10     || lsu0.sti.128.u16.u8 v11 i16 || cmu.cpivr.x16 v20 i0
lsu0.sti.128.u16.u8 v10 i16  

bru.jmp i30
nop 6

___computeAD64flag:
lsu0.ldil i1 ___computeAD64Loop2  || lsu1.ldih i1 ___computeAD64Loop2

lsu0.ldi.8 i0 i18   
lsu0.ld.128.u8.u16 v1 i17   || iau.add i17 i17 8 
lsu0.ld.128.u8.u16 v2 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v3 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v4 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v5 i17   || iau.add i17 i17 8 
lsu0.ld.128.u8.u16 v6 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v7 i17   || iau.add i17 i17 8
lsu0.ld.128.u8.u16 v8 i17   || iau.add i17 i17 8
    
cmu.cpii.u8.u16 i0 i0
cmu.cpivr.x16 v20 i0 || iau.sub i17 i17 0x3f

vau.sub.i16 v10 v20 v1 || bru.rpl i1 i15 || lsu0.ldi.8 i0 i18   
vau.sub.i16 v11 v20 v2 || lsu0.ld.128.u8.u16 v1 i17   || iau.add i17 i17 8 
vau.sub.i16 v12 v20 v3 || lsu0.ld.128.u8.u16 v2 i17   || iau.add i17 i17 8
vau.sub.i16 v13 v20 v4 || lsu0.ld.128.u8.u16 v3 i17   || iau.add i17 i17 8
vau.sub.i16 v14 v20 v5 || lsu0.ld.128.u8.u16 v4 i17   || iau.add i17 i17 8
vau.sub.i16 v15 v20 v6 || lsu0.ld.128.u8.u16 v5 i17   || iau.add i17 i17 8 
vau.sub.i16 v16 v20 v7 || lsu0.ld.128.u8.u16 v6 i17   || iau.add i17 i17 8
vau.sub.i16 v17 v20 v8 || lsu0.ld.128.u8.u16 v7 i17   || iau.add i17 i17 8
                          lsu0.ld.128.u8.u16 v8 i17   || iau.add i17 i17 8
vau.abs.i16 v9  v10                                                         || cmu.cpii.u8.u16 i0 i0 || iau.sub i17 i17 0x3f
vau.abs.i16 v18 v11  || lsu0.st.128.u16.u8 v9  i16  || iau.add i16 i16 8    || cmu.cpivr.x16 v20 i0 
___computeAD64Loop2:
vau.abs.i16 v19 v12  || lsu0.st.128.u16.u8 v18 i16  || iau.add i16 i16 8
vau.abs.i16 v21 v13  || lsu0.st.128.u16.u8 v19 i16  || iau.add i16 i16 8
vau.abs.i16 v22 v14  || lsu0.st.128.u16.u8 v21 i16  || iau.add i16 i16 8
vau.abs.i16 v23 v15  || lsu0.st.128.u16.u8 v22 i16  || iau.add i16 i16 8
vau.abs.i16 v10 v16  || lsu0.st.128.u16.u8 v23 i16  || iau.add i16 i16 8
vau.abs.i16 v11 v17  || lsu0.st.128.u16.u8 v10 i16  || iau.add i16 i16 8
                        lsu0.st.128.u16.u8 v11 i16  || iau.add i16 i16 8 || vau.sub.i16 v10 v20 v1 

bru.jmp i30
nop 6
.nowarnend
.size mvcvComputeAD64_asm,.-mvcvComputeAD64_asm
.end
