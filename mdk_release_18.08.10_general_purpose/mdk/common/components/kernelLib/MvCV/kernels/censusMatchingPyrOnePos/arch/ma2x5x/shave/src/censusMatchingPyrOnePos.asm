.type mvcvCensusMatchingPyrOnePos_asm,@function
.version 00.50.00

//////
/// @file
/// @coPyrOnePosight All code coPyrOnePosight Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching32 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMatchingPyrOnePos_asm
.align 16

.code .text.mvcvCensusMatchingPyrOnePos_asm
.align 16

.lalign
//mvcvCensusMatchingPyrOnePos(in1,in2,predicted,out,width)//
                      //i18  i17    i16    i15  i14
mvcvCensusMatchingPyrOnePos_asm:

cmu.cpiv.x32 v19.0 i20 || LSU0.ldil i0, 0x0 || LSU1.ldih i0, 0x0
cmu.cpiv.x32 v19.1 i21 || LSU0.ldil i1, 0x1 || LSU1.ldih i1, 0x0
cmu.cpiv.x32 v19.2 i22 || LSU0.ldil i2, 0x2 || LSU1.ldih i2, 0x0
cmu.cpiv.x32 v19.3 i23 || LSU0.ldil i3, 0x3 || LSU1.ldih i3, 0x0
cmu.cpiv.x32 v18.0 i24 || LSU0.ldil i8, 0xffff || LSU1.ldih i8, 0x01ff


lsu0.cp v2.0 i3
lsu0.cp v2.1 i2
lsu0.cp v2.2 i1
lsu0.cp v2.3 i0
cmu.cpivr.x32 v3 i8


IAU.SUB i0 i0 i0
IAU.SUB i2 i2 i2


LSU0.ldil i1, CensusMatchingPyrOnePos_loop || LSU1.ldih i1, CensusMatchingPyrOnePos_loop

IAU.SHR.u32 i3 i0 1
IAU.ADD     i13 i16 i3
LSU0.LD.8 i2 i13  || cmu.cpii i11 i0

nop 6

iau.ADD i0 i0 1
IAU.SHR.u32 i3 i0 1
IAU.ADD     i13 i16 i3
IAU.SHL i5 i2 1 || LSU0.LD.8 i2 i13  
IAU.SUB i6 i5 1
IAU.SUB i7 i11 i6 || cmu.cpivr.x32 v4 i6
IAU.SHL i8 i7 2   || cmu.cpii i11 i0
IAU.ADD i9 i17 i8 || vau.add.i32 v6 v4 v2 
IAU.SUB i10 i9 12 

LSU0.LDi.32r v0 i18                                     || iau.ADD i0 i0 1
lsu0.ldo.64.l v22 i10 0  || lsu1.ldo.64.h v22 i10 8     || IAU.SHR.u32 i3 i0 1
IAU.ADD     i13 i16 i3
IAU.SHL i5 i2 1 || LSU0.LD.8 i2 i13  
IAU.SUB i6 i5 1
IAU.SUB i7 i11 i6 || cmu.cpivr.x32 v4 i6
IAU.SHL i8 i7 2   || cmu.cpii i11 i0
IAU.ADD i9 i17 i8 
IAU.SUB i10 i9 12 || vau.xor v21 v22 v0
cmu.cmz.i32 v6 || vau.add.i32 v6 v4 v2 

LSU0.LDi.32r v0 i18                                     || iau.ADD i0 i0 1
lsu0.ldo.64.l v22 i10 0  || lsu1.ldo.64.h v22 i10 8     || IAU.SHR.u32 i3 i0 1 || peu.pvv32 LT || vau.sub.i32 v21 v3 0
IAU.ADD     i13 i16 i3
IAU.SHL i5 i2 1 || LSU0.LD.8 i2 i13  || sau.onesx.x32 i12 v21 
IAU.SUB i6 i5 1
IAU.SUB i7 i11 i6 || cmu.cpivr.x32 v4 i6
IAU.SHL i8 i7 2   || cmu.cpii i11 i0
IAU.ADD i9 i17 i8  || cmu.vszm.byte i21 i12 [0123]
IAU.SUB i10 i9 12 || vau.xor v21 v22 v0
IAU.SHR.u32 i22 i21 8 || cmu.cmz.i32 v6 || vau.add.i32 v6 v4 v2 


IAU.SHR.u32 i23 i22 8 || bru.rpl i1 i14
LSU0.LDi.32r v0 i18                                     || iau.ADD i0 i0 1
lsu0.ldo.64.l v22 i10 0  || lsu1.ldo.64.h v22 i10 8     || IAU.SHR.u32 i3 i0 1 || peu.pvv32 LT || vau.sub.i32 v21 v3 0
IAU.ADD     i13 i16 i3                                             || lsu1.sti.8 i21  i15
CensusMatchingPyrOnePos_loop:
.nowarn 10
IAU.SHL i5 i2 1 || LSU0.LD.8 i2 i13  || sau.onesx.x32 i12 v21      || lsu1.sti.8 i22  i15
.nowarnend
IAU.SUB i6 i5 1                                                    || lsu1.sti.8 i23  i15
IAU.SUB i7 i11 i6 || cmu.cpivr.x32 v4 i6
IAU.SHL i8 i7 2   || cmu.cpii i11 i0
IAU.ADD i9 i17 i8  || cmu.vszm.byte i21 i12 [0123]
IAU.SUB i10 i9 12 || vau.xor v21 v22 v0
IAU.SHR.u32 i22 i21 8 || cmu.cmz.i32 v6 || vau.add.i32 v6 v4 v2 





bru.jmp i30
cmu.cpvi.x32  i20 v19.0
cmu.cpvi.x32  i21 v19.1
cmu.cpvi.x32  i22 v19.2
cmu.cpvi.x32  i23 v19.3
cmu.cpvi.x32  i24 v18.0	
nop	

.size mvcvCensusMatchingPyrOnePos_asm,.-mvcvCensusMatchingPyrOnePos_asm
.end
