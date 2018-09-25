/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvComputeADPyrOnePosWindow_asm,@function
.version 00.51.04

.data .data.mvcvComputeADPyrOnePosWindow
.align 4

.code .text.mvcvComputeADPyrOnePosWindow

.lalign
//void mvcvComputeADPyrOnePosWindow(u8* inputLeft, u8* inputRight, u8* output, u32 width)
//                              i18              i17            i16        i15
mvcvComputeADPyrOnePosWindow_asm:
.nowarn
lsu0.ldil i12 0xf
iau.shr.u32 i14 i15 4
iau.and i13 i15 i12

cmu.cmii.i32 i15 i12
peu.pc1c lt || bru.bra mvcvComputeADPyrOnePosWindow_asm_skip1
nop 6

vau.xor v21 v21 v21
LSU0.ldil i0, mvcvComputeADPyrOnePosWindow_asm_loop || LSU1.ldih i0, mvcvComputeADPyrOnePosWindow_asm_loop

lsu0.ldo.64.l v2 i17  0 || lsu1.ldo.64.h v2 i17  8 
lsu0.ldo.64.l v3 i17 16 || lsu1.ldo.64.h v1 i17 -8 || iau.add i17 i17 16
lsu0.ldo.64.l v0 i18  0 || lsu1.ldo.64.h v0 i18  8 || iau.add i18 i18 16     
nop 6
cmu.alignvec v4 v1 v2 14
VAU.ADIFF.u8  v8 v0 v4  || cmu.alignvec v5 v1 v2 15 || lsu0.ldo.64.l v2 i17  0 || lsu1.ldo.64.h v2 i17  8 
VAU.ADIFF.u8  v9 v0 v5  || cmu.alignvec v6 v2 v3 1  || lsu0.ldo.64.l v3 i17 16 || lsu1.ldo.64.h v1 i17 -8 || iau.add i17 i17 16
VAU.ADIFF.u8 v10 v0 v2  || cmu.alignvec v7 v2 v3 2  || lsu0.ldo.64.l v0 i18  0 || lsu1.ldo.64.h v0 i18  8 || iau.add i18 i18 16      
VAU.ADIFF.u8 v11 v0 v6  
cmu.vilv.x8 v13 v14  v9  v8 || VAU.ADIFF.u8 v12 v0 v7 
cmu.vilv.x8 v15 v16 v11 v10
cmu.vilv.x8 v22 v23 v21 v12
cmu.vilv.x16 v15 v13 v15 v13
cmu.vilv.x16 v16 v14 v16 v14
cmu.vilv.x16 v17 v18 v21 v22
cmu.vilv.x16 v22 v23 v21 v23
cmu.vilv.x32 v17 v15 v17 v15
cmu.vilv.x32 v18 v13 v18 v13
cmu.vilv.x32 v16 v22 v22 v16
cmu.vilv.x32 v14 v23 v23 v14

bru.rpl i0 i14                     

lsu0.sto.64.l v17 i16  0 || cmu.alignvec v4 v1 v2 14
lsu0.sto.64.h v17 i16  5 || VAU.ADIFF.u8  v8 v0 v4  || cmu.alignvec v5 v1 v2 15 
lsu0.sto.64.l v15 i16 10 || VAU.ADIFF.u8  v9 v0 v5  || cmu.alignvec v6 v2 v3 1  
lsu0.sto.64.h v15 i16 15 || VAU.ADIFF.u8 v10 v0 v2  || cmu.alignvec v7 v2 v3 2       
lsu0.sto.64.l v18 i16 20 || VAU.ADIFF.u8 v11 v0 v6  
lsu0.sto.64.h v18 i16 25
lsu0.sto.64.l v13 i16 30
lsu0.sto.64.h v13 i16 35
lsu0.sto.64.l v16 i16 40
lsu0.sto.64.h v16 i16 45
lsu0.sto.64.l v22 i16 50 || lsu1.ldo.64.l v2 i17  0 
lsu0.sto.64.h v22 i16 55 || lsu1.ldo.64.h v2 i17  8 
lsu0.sto.64.l v14 i16 60 || lsu1.cp i2 v23.2
lsu0.sto.64.h v14 i16 65 || lsu1.cp i3 v23.3 || cmu.vilv.x8 v13 v14  v9  v8 || VAU.ADIFF.u8 v12 v0 v7 
lsu0.sto.64.l v23 i16 70 || cmu.vilv.x8 v15 v16 v11 v10 || lsu1.ldo.64.l v3 i17 16 
lsu0.sto.32    i2 i16 75 || cmu.vilv.x8 v22 v23 v21 v12 || lsu1.ldo.64.h v1 i17 -8 || iau.add i17 i17 16
lsu0.sto.8     i3 i16 79 || iau.add i16 i16 80 || cmu.vilv.x16 v15 v13 v15 v13 || lsu1.ldo.64.l v0 i18  0
mvcvComputeADPyrOnePosWindow_asm_loop:
cmu.vilv.x16 v16 v14 v16 v14  || lsu1.ldo.64.h v0 i18  8 || iau.add i18 i18 16 
cmu.vilv.x16 v17 v18 v21 v22
cmu.vilv.x16 v22 v23 v21 v23
cmu.vilv.x32 v17 v15 v17 v15
cmu.vilv.x32 v18 v13 v18 v13  
cmu.vilv.x32 v16 v22 v22 v16  
cmu.vilv.x32 v14 v23 v23 v14   


iau.sub i17 i17 16
iau.sub i18 i18 16
iau.sub i17 i17 16
iau.sub i18 i18 16

mvcvComputeADPyrOnePosWindow_asm_skip1:

cmu.cmz.i32 i13
peu.pc1c eq || bru.bra mvcvComputeADPyrOnePosWindow_asm_skip
nop 6

lsu0.ldo.64.l v0 i18 0 || lsu1.ldo.64.h v0 i18 8 || iau.add i18 i18 16
nop 6
iau.sub i17 i17 16
lsu0.ldo.64.l v1 i17 0 || lsu1.ldo.64.h v1 i17 8 || iau.add i17 i17 16
lsu0.ldo.64.l v2 i17 0 || lsu1.ldo.64.h v2 i17 8 || iau.add i17 i17 16
lsu0.ldo.64.l v3 i17 0 || lsu1.ldo.64.h v3 i17 8
nop 6
cmu.alignvec v4 v1 v2 14
cmu.alignvec v5 v1 v2 15
cmu.alignvec v6 v2 v3 1
cmu.alignvec v7 v2 v3 2

vau.xor v21 v21 v21
VAU.ADIFF.u8  v8 v0 v4  
VAU.ADIFF.u8  v9 v0 v5  
VAU.ADIFF.u8 v10 v0 v2  
VAU.ADIFF.u8 v11 v0 v6  
VAU.ADIFF.u8 v12 v0 v7  
nop 2
cmu.vilv.x8 v13 v14  v9  v8
cmu.vilv.x8 v15 v16 v11 v10
cmu.vilv.x8 v22 v23 v21 v12

cmu.vilv.x16 v15 v13 v15 v13
cmu.vilv.x16 v16 v14 v16 v14

cmu.vilv.x16 v17 v18 v21 v22
cmu.vilv.x16 v22 v23 v21 v23

cmu.vilv.x32 v17 v15 v17 v15
cmu.vilv.x32 v18 v13 v18 v13
cmu.vilv.x32 v16 v22 v22 v16
cmu.vilv.x32 v14 v23 v23 v14

lsu0.sto.64.l v17 i16  0
nop
lsu1.sto.64.h v17 i16  5 || cmu.cpvi.x32 i2 v13.2
nop
lsu0.sto.64.l v15 i16 10
nop
lsu1.sto.64.h v15 i16 15 || cmu.cpvi.x32 i3 v13.3
nop
lsu0.sto.64.l v18 i16 20
nop
lsu1.sto.64.h v18 i16 25
nop
lsu0.sto.64.l v13 i16 30
nop
lsu1.sto.32    i2 i16 35
nop
lsu0.sto.8     i3 i16 39




mvcvComputeADPyrOnePosWindow_asm_skip:
BRU.JMP i30
nop 6


.nowarnend
.size mvcvComputeADPyrOnePosWindow_asm,.-mvcvComputeADPyrOnePosWindow_asm
.end
