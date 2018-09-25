.type mvcvCensusMatchingPyrOnePosWindow_asm,@function
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




.data .rodata.mvcvCensusMatchingPyrOnePosWindow_asm
.align 16

.code .text.mvcvCensusMatchingPyrOnePosWindow_asm
.align 16

.lalign
//mvcvCensusMatchingPyrOnePosWindow(    in1,     in2,     predicted,     out,width)//
                                //i18      i17          i16        i15  i14
mvcvCensusMatchingPyrOnePosWindow_asm:
vau.xor v0 v0 v0
lsu0.ldil i0 0x2 || lsu1.ldih i0 0x0
lsu0.ldil i1 0xffff || lsu1.ldih i1 0xffff
lsu0.ldil i2 0x0
lsu0.ldil i3 0x1
cmu.cpiv.x32 v6.0 i0 //-1
cmu.cpiv.x32 v6.1 i3 //-2
cmu.cpiv.x32 v6.2 i2 //0
cmu.cpiv.x32 v6.3 i1 //0
cmu.cpivr.x32 v7 i17
lsu0.ldil i9 0xffff || lsu1.ldih i9 0x01ff 
cmu.cpivr.x32 v21 i9
LSU0.ldil i8, CensusMatchingPyrOnePosWindow_loop || LSU1.ldih i8, CensusMatchingPyrOnePosWindow_loop
iau.shr.u32 i7 i14 1
nop

lsu0.ldi.32.u8.u32 i0 i16 //predicted
lsu0.ldi.32r v0 i18  //0
lsu0.ldi.32r v1 i18  //1
nop 6

//offsetright
cmu.cpivr.x32 v2 i0 //predicted repeted
vau.shl.x32 v5 v2 1
nop
vau.add.i32 v3 v5 v6 //1-4th
vau.sub.i32 v4 v5 2  //5
vau.shl.x32 v8 v3 2 //multiply for integer reason
vau.shl.x32 v9 v4 2
vau.sub.i32 v10 v7 v8 //addresses 
vau.sub.i32 v11 v7 v9
vau.add.i32 v7 v7 8


lsu0.cp i1 v10.0
lsu0.cp i2 v10.1 || lsu1.ldo.64.l  v14 i1 0
lsu0.cp i3 v10.2 || lsu1.ldo.64.l  v15 i2 0
lsu0.cp i4 v10.3 || lsu1.ldo.64.l  v16 i3 0
lsu0.cp i5 v11.0 || lsu1.ldo.64.l  v17 i4 0
                    lsu1.ldo.64.l  v18 i5 0    
nop 2
cmu.cpvi.x32 i1 v14.0
cmu.cpvi.x32 i2 v15.0 || lsu0.cp v12.0 i1
cmu.cpvi.x32 i3 v16.0 || lsu0.cp v12.1 i2
cmu.cpvi.x32 i4 v17.0 || lsu0.cp v12.2 i3
cmu.cpvi.x32 i5 v18.0 || lsu0.cp v12.3 i4
cmu.cpvi.x32 i1 v14.1 || lsu0.cp v13.0 i5 
cmu.cpvi.x32 i2 v15.1 || lsu0.cp v19.0 i1
cmu.cpvi.x32 i3 v16.1 || lsu0.cp v19.1 i2
cmu.cpvi.x32 i4 v17.1 || lsu0.cp v19.2 i3
cmu.cpvi.x32 i5 v18.1 || lsu0.cp v19.3 i4
                         lsu0.cp v20.0 i5

bru.rpl i8 i7
cmu.cmz.i32 v3 || vau.xor v12 v12 v0 || lsu1.ldi.32.u8.u32 i0 i16 //predicted
peu.pvl032 lt || lsu0.cp v12 v21 || vau.xor v19 v19 v1 || lsu1.ldi.32r v0 i18  //0
peu.pvl032 lt || lsu0.cp v19 v21 || sau.onesx.x32 i11 v12 || lsu1.ldi.32r v1 i18  //1
cmu.cmz.i32 v4                   || sau.onesx.x32 i12 v19 || vau.xor v13 v13 v0
peu.pvl032 lt || lsu0.cp v13 v21 || CMU.VSZM.byte i11 i11 [0123] || vau.xor v20 v20 v1
peu.pvl032 lt || lsu0.cp v20 v21 || sau.onesx.x32 i13 v13 || CMU.VSZM.byte i12 i12 [0123]
                                    sau.onesx.x32 i6 v20 

lsu0.sti.8  i13 i15 || cmu.cpivr.x32 v2 i0 //predicted repeted
vau.shl.x32 v5 v2 1
lsu0.sti.32 i11 i15
vau.add.i32 v3 v5 v6 //1-4th
lsu0.sti.8  i6 i15 || vau.sub.i32 v4 v5 2  //5
vau.shl.x32 v8 v3 2 //multiply for integer reason
lsu0.sti.32 i12 i15 || vau.shl.x32 v9 v4 2
vau.sub.i32 v10 v7 v8 //addresses 
vau.sub.i32 v11 v7 v9
vau.add.i32 v7 v7 8
lsu0.cp i1 v10.0
lsu0.cp i2 v10.1 || lsu1.ldo.64.l  v14 i1 0
lsu0.cp i3 v10.2 || lsu1.ldo.64.l  v15 i2 0
lsu0.cp i4 v10.3 || lsu1.ldo.64.l  v16 i3 0
lsu0.cp i5 v11.0 || lsu1.ldo.64.l  v17 i4 0
                    lsu1.ldo.64.l  v18 i5 0    
nop 2
cmu.cpvi.x32 i1 v14.0
cmu.cpvi.x32 i2 v15.0 || lsu0.cp v12.0 i1
cmu.cpvi.x32 i3 v16.0 || lsu0.cp v12.1 i2
cmu.cpvi.x32 i4 v17.0 || lsu0.cp v12.2 i3
CensusMatchingPyrOnePosWindow_loop:
cmu.cpvi.x32 i5 v18.0 || lsu0.cp v12.3 i4
cmu.cpvi.x32 i1 v14.1 || lsu0.cp v13.0 i5 
cmu.cpvi.x32 i2 v15.1 || lsu0.cp v19.0 i1
cmu.cpvi.x32 i3 v16.1 || lsu0.cp v19.1 i2
cmu.cpvi.x32 i4 v17.1 || lsu0.cp v19.2 i3
cmu.cpvi.x32 i5 v18.1 || lsu0.cp v19.3 i4
                         lsu0.cp v20.0 i5



bru.jmp i30
nop 6    

.size mvcvCensusMatchingPyrOnePosWindow_asm,.-mvcvCensusMatchingPyrOnePosWindow_asm
.end
