.type mvcvCensusMatchingPyr_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 
/// mvcvCensusMatching32 
//     @param[in] in1            - pointer to input lines of the left image
//     @param[in] in2            - pointer to input lines of the right image
//     @param[out] out           - array of disparity cost
//     @param[in] width          - width of the input lines
//     @return    Nothing




.data .rodata.mvcvCensusMatchingPyr_asm
.align 16

.code .text.mvcvCensusMatchingPyr_asm
.align 16

.lalign
//mvcvCensusMatchingPyr(in1,in2,predicted,out,width)//
                      //i18  i17    i16    i15  i14
mvcvCensusMatchingPyr_asm:

cmu.cpiv.x32 v19.0 i20
cmu.cpiv.x32 v19.1 i21
cmu.cpiv.x32 v19.2 i22
cmu.cpiv.x32 v19.3 i23
cmu.cpiv.x32 v18.0 i24



LSU0.ldil i0, 0x0 || LSU1.ldih i0, 0x0
LSU0.ldil i1, 0x1 || LSU1.ldih i1, 0x0
LSU0.ldil i2, 0x2 || LSU1.ldih i2, 0x0
LSU0.ldil i3, 0x3 || LSU1.ldih i3, 0x0
LSU0.ldil i4, 0x4 || LSU1.ldih i4, 0x0
LSU0.ldil i5, 0x5 || LSU1.ldih i5, 0x0
LSU0.ldil i6, 0x6 || LSU1.ldih i6, 0x0
LSU0.ldil i7, 0x7 || LSU1.ldih i7, 0x0
LSU0.ldil i8, 0xffff || LSU1.ldih i8, 0x01ff

cmu.cpiv.x32 v1.0 i7 || lsu0.cp v2.0 i3
cmu.cpiv.x32 v1.1 i6 || lsu0.cp v2.1 i2
cmu.cpiv.x32 v1.2 i5 || lsu0.cp v2.2 i1
cmu.cpiv.x32 v1.3 i4 || lsu0.cp v2.3 i0
cmu.cpivr.x32 v3 i8


IAU.SUB i0 i0 i0
IAU.SUB i2 i2 i2


LSU0.ldil i1, CensusMatchingPyr_loop || LSU1.ldih i1, CensusMatchingPyr_loop



IAU.SHR.u32 i3 i0 1 
IAU.ADD     i13 i16 i3

LSU0.LD.8 i2 i13  
nop 6

IAU.SHL i5 i2 1
IAU.SUB i6 i5 3
IAU.SUB i7 i0 i6 
IAU.ADD i0 i0 1
IAU.SHR.u32 i3 i0 1 
IAU.ADD     i13 i16 i3
IAU.SHL i8 i7 2   || LSU0.LD.8 i2 i13  
IAU.ADD i9 i17 i8 
IAU.SUB i10 i9 28 
nop 5




LSU0.LDi.32r v0 i18  || cmu.cpivr.x32 v4 i6                                          || IAU.SHL i5 i2 1
lsu0.ldo.64.l v23 i10 0  || lsu1.ldo.64.h v23 i10 8  || vau.add.i32 v5 v4 v1         || IAU.SUB i6 i5 3
lsu0.ldo.64.l v22 i10 16 || lsu1.ldo.64.h v22 i10 24 || vau.add.i32 v6 v4 v2         || IAU.SUB i7 i0 i6 
                                                                                     IAU.ADD i0 i0 1
                                                                                     IAU.SHR.u32 i3 i0 1 
                                                                                     IAU.ADD     i13 i16 i3
                                                                                     IAU.SHL i8 i7 2   || LSU0.LD.8 i2 i13  
                                                                                     IAU.ADD i9 i17 i8 
                                                                                     IAU.SUB i10 i9 28 
nop 5
vau.xor v20 v23 v0   || LSU0.LDi.32r v0 i18  || cmu.cpivr.x32 v4 i6                                          || IAU.SHL i5 i2 1
vau.xor v21 v22 v0   || lsu0.ldo.64.l v23 i10 0  || lsu1.ldo.64.h v23 i10 8           || IAU.SUB i6 i5 3
cmu.cmz.i32 v5       || lsu0.ldo.64.l v22 i10 16 || lsu1.ldo.64.h v22 i10 24 || vau.add.i32 v5 v4 v1         || IAU.SUB i7 i0 i6 
peu.pvv32 LT || vau.sub.i32 v20 v3 0          || IAU.ADD i0 i0 1                                
cmu.cmz.i32 v6 || vau.add.i32 v6 v4 v2        || IAU.SHR.u32 i3 i0 1 
peu.pvv32 LT || vau.sub.i32 v21 v3 0          || IAU.ADD     i13 i16 i3
sau.onesx.x32 i11 v20                         || IAU.SHL i8 i7 2   || LSU0.LD.8 i2 i13  
sau.onesx.x32 i12 v21                         || IAU.ADD i9 i17 i8 
IAU.SUB i10 i9 28 
nop 5                                              


bru.rpl i1 i14 
vau.xor v20 v23 v0   || LSU0.LDi.32r v0 i18  || cmu.cpivr.x32 v4 i6                                          || IAU.SHL i5 i2 1
vau.xor v21 v22 v0   || lsu0.ldo.64.l v23 i10 0  || lsu1.ldo.64.h v23 i10 8           || IAU.SUB i6 i5 3 || cmu.vszm.byte i20 i12 [0123]
cmu.cmz.i32 v5       || lsu0.ldo.64.l v22 i10 16 || lsu1.ldo.64.h v22 i10 24 || vau.add.i32 v5 v4 v1         || IAU.SUB i7 i0 i6 
peu.pvv32 LT || vau.sub.i32 v20 v3 0          || IAU.ADD i0 i0 1    || cmu.vszm.byte i21 i11 [0123]                            
cmu.cmz.i32 v6 || vau.add.i32 v6 v4 v2        || IAU.SHR.u32 i3 i0 1 
CensusMatchingPyr_loop:
.nowarn 10
peu.pvv32 LT || vau.sub.i32 v21 v3 0          || IAU.ADD     i13 i16 i3
.nowarnend
IAU.SHR.u32 i22 i21 8  || LSU0.LD.8 i2 i13  
IAU.SHL i8 i7 2  
sau.onesx.x32 i11 v20           || IAU.SHR.u32 i23 i22 8              
sau.onesx.x32 i12 v21                         || IAU.ADD i9 i17 i8 
                                              IAU.SUB i10 i9 28 || lsu0.st.32 i20 i15  || lsu1.sto.8 i21  i15 4
                                                                lsu0.sto.8 i22  i15 5  || lsu1.sto.8 i23  i15 6 || IAU.ADD i15 i15 7                     
                                                                                     



//cmu.cmii.i32 i0 i14
//peu.pc1c LT || bru.jmp i1
//nop 6


//bru.rpim 0
//nop 6
	
cmu.cpvi.x32  i20 v19.0
cmu.cpvi.x32  i21 v19.1
cmu.cpvi.x32  i22 v19.2
cmu.cpvi.x32  i23 v19.3
cmu.cpvi.x32  i24 v18.0	
	
bru.jmp i30
nop 6
.size mvcvCensusMatchingPyr_asm,.-mvcvCensusMatchingPyr_asm
.end
