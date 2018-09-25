// ///
// /// @file
// /// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
// ///            For License Warranty see: common/license.txt
// ///
///// This kernel performs 1 maximum line from 3 input lines
///// @param[in] in        - 3 Input lines      
///// @param[out] out      - 1 Output line    
///// @param[in] width     - Width of input line

//--------------------------------------------------------------------------------------------------------------
.type mvcvMaximumV2_asm,@function
.version 00.50.05
.data .rodata.vect_mask 
.salign 16

.code .text.MaximumV2 //text
.salign 16

.align 16  
//void mvcvMaximumV2(u8** in, u8** out, u32 width)
//                         i18      i17      i16
mvcvMaximumV2_asm:
.nowarn
lsu0.ldi.32 i0 i18 || iau.shr.u32 i3 i16 4
lsu0.ldi.32 i1 i18 || IAU.SHL i4 i3 4	
IAU.SUB i5 i16 i4	
nop 6
lsu0.ldi.32 i12 i17 || lsu1.ldil i11 mvcvMaximumV2loop
                       lsu1.ldih i11 mvcvMaximumV2loop
                       
CMU.CMZ.i32 i3
PEU.PC1C eq || BRU.BRA mvcvMaximumV2loop_compensate                       

lsu0.ldo.64.l v0  i0  0 || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16  
lsu0.ldo.64.l V2  i0  0 || lsu1.ldo.64.h V2  i0  8 || iau.add i0  i0  16 
lsu0.ldo.64.l v1  i1  0 || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16
lsu0.ldo.64.l v3  i1  0 || lsu1.ldo.64.h v3  i1  8 || iau.add i1  i1  16
nop 5
mvcvMaximumV2loop:
                           lsu0.ldo.64.l v0  i0  0  || lsu1.ldo.64.h v0  i0  8 || iau.add i0  i0  16 || bru.rpl i11 i3 
                           lsu0.ldo.64.l V2  i0  0  || lsu1.ldo.64.h V2  i0  8 || iau.add i0  i0  16
cmu.max.f16 v5 v0 v1    || lsu0.ldo.64.l v1  i1  0  || lsu1.ldo.64.h v1  i1  8 || iau.add i1  i1  16  
cmu.max.f16 v6 V2 v3    || lsu0.ldo.64.l v3  i1  0  || lsu1.ldo.64.h v3  i1  8 || iau.add i1  i1  16 
lsu0.sto.64.l v5 i12 0  || lsu1.sto.64.h v5  i12 8  || iau.add i12 i12 16 
lsu0.sto.64.l v6 i12 0  || lsu1.sto.64.h v6  i12 8  || iau.add i12 i12 16 
nop

mvcvMaximumV2loop_compensate:
nop 3 // could be nop 2
cmu.max.f16 v5 v0 v1
IAU.SHR.u32 i5 i5 3
PEU.PC1I gt || lsu0.sto.64.l v5 i12 0 || lsu1.sto.64.h v5 i12 8 || iau.add i12 i12 16 

bru.jmp i30 
nop 6  
.nowarnend  
.size mvcvMaximumV2_asm,.-mvcvMaximumV2_asm
.end
