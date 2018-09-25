/// @file
/// @copyright Three code copyright Movidius Ltd 2015, Three rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
    
.type mvcvCensusMinSubpixel3_asm,@function
.version 00.51.04

.data .data.CensusMinSubpixel3
.align 4

.code .text.CensusMinSubpixel3

.lalign
//void mvcvCensusMinSubpixel3(u8 *in, half *out, u8 *outm, u32 width)
//                              i18    i17        i16        i15
mvcvCensusMinSubpixel3_asm:
.nowarn 10
lsu0.ldil i8 .LmvcvCensusMinSubpixel3_asm_loop || lsu1.ldih i8 .LmvcvCensusMinSubpixel3_asm_loop || iau.shr.u32 i15 i15 3
lsu0.ldil i0 0x0 || lsu1.ldil i1 0x1
lsu0.ldil i2 0x2 || lsu1.ldil i3 0xb800
lsu0.ldil i4 0x3800 || cmu.cpivr.x16 v15 i0 //poz 0
cmu.cpivr.x16 v16 i1 //poz 1
cmu.cpivr.x16 v17 i2 //poz 2
cmu.cpivr.x16 v23 i3 // -0.5 fp16
cmu.cpivr.x16 v21 i4 || lsu0.ldil i9 0x2 //number of iterations for debugg// -0.5 fp16



lsu0.ldi.128.u8.u16 v0 i18 
lsu0.ldi.128.u8.u16 v1 i18
lsu0.ldi.128.u8.u16 v2 i18
nop 4
cmu.alignvec v3 v0 v0 0 || lsu0.swzc8 [52741630] //00011122
cmu.alignvec v4 v1 v1 0 || lsu0.swzc8 [52741630] //22200011
cmu.alignvec v5 v2 v2 0 || lsu0.swzc8 [52741630] //11122200
cmu.alignvec v6 v3 v4 6 //11122222
cmu.alignvec v7 v4 v5 6 //00011111
cmu.alignvec v8 v7 v6 6 //11111111
cmu.alignvec v10 v8 v8 0 || lsu0.swzc8 [43210765] //final 11111111  (disp[1])
cmu.alignvec v8 v5 v5 6
cmu.alignvec v11 v6 v8 6 //final 22222222 (disp[2])
cmu.alignvec v9 v3 v7 10 || lsu0.swzc8 [21010765] || lsu1.cp i0 v5.3 //00000011
cmu.cpiv.x32 v9.3 i0 // final 00000000 (disp[0])
cmu.cpvv v12  v9 //mincost
cmu.cpvv v13  v9 //preCost
cmu.cpvv v14 v10 || vau.xor v18 v18 v18//postCost

cmu.cmvv.u16 v10 v12   // minPos
peu.pvl016 lt || lsu0.cp v12 v10 //mincost
peu.pvl016 lt || lsu0.cp v13 v9 || bru.rpl i8 i15 //precost
peu.pvl016 lt || lsu0.cp v14 v11 //postcost
peu.pvl016 lt || lsu0.cp v18 v16 || cmu.cmvv.u16 v11 v12 //minpos 
peu.pvl016 lt || lsu0.cp v12 v11 //mincost   -f1     outm[i] = minCost//
peu.pvl016 lt || lsu0.cp v13 v10 || vau.shl.x16 v20 v12 1   // 2 * f1 //precost   -f0
peu.pvl016 lt || lsu0.cp v14 v11 //postcost  -f2
peu.pvl016 lt || lsu0.cp v18 v17 || vau.add.i16 v19 v13 v14 // f0 + f2//minpos
nop 
vau.sub.i16 v19 v19 v20 || cmu.cpivr.x32 v22 i18 || lsu0.ldi.128.u8.u16 v0 i18 //a
lsu0.ldi.128.u8.u16 v1 i18 || lsu1.sti.128.u16.u8 v12 i16
cmu.cmz.i16 v19 || lsu0.ldi.128.u8.u16 v2 i18
peu.pvv16 eq || vau.xor v22 v19 v19  
vau.shl.x16 v19 v19 1 //denom
vau.sub.i16 v20 v13 v14 //number
cmu.cpvv.i16.f16 v19 v19 //denom
cmu.cpvv.i16.f16 v20 v20 //number
nop
lsu0.cp i0 v19.0 || lsu1.cp i4 v20.0     
sau.div.f16 i4 i4 i0 || lsu0.cp i1 v19.1 || lsu1.cp i5 v20.1
sau.div.f16 i5 i5 i1 || lsu0.cp i2 v19.2 || lsu1.cp i6 v20.2
sau.div.f16 i6 i6 i2 || lsu0.cp i3 v19.3 || lsu1.cp i7 v20.3
sau.div.f16 i7 i7 i3 || cmu.alignvec v3 v0 v0 0 || lsu0.swzc8 [52741630]
cmu.alignvec v4 v1 v1 0 || lsu0.swzc8 [52741630]
cmu.alignvec v5 v2 v2 0 || lsu0.swzc8 [52741630]
cmu.alignvec v6 v3 v4 6 
cmu.alignvec v7 v4 v5 6 

lsu0.cp v20.0 i4 || cmu.alignvec v8 v7 v6 6 
lsu0.cp v20.1 i5 || cmu.alignvec v10 v8 v8 0 || lsu1.swzc8 [43210765] 
lsu0.cp v20.2 i6 || cmu.alignvec v8 v5 v5 6
lsu0.cp v20.3 i7 || cmu.alignvec v11 v6 v8 6
cmu.cmz.i16 v22  || lsu1.cp i0 v5.3
peu.pvl016 eq || lsu0.cp v20 v22 || cmu.alignvec v9 v3 v7 10 || lsu1.swzc8 [21010765] 
CMU.CLAMPAB.f16 v20 v23 v21   || lsu0.cp v9.3 i0
.LmvcvCensusMinSubpixel3_asm_loop:
lsu0.cp v12  v9 || lsu1.cp v13  v9
cmu.cpvv.i16.f16 v18 v18 || lsu0.cp v14 v10 //minpos in fp16
nop
vau.add.f16 v20 v20 v18 || cmu.cpzv v18 0 // out[i] = (half)minPos + f//
cmu.cmvv.u16 v10 v12   // minPos
peu.pvl016 lt || lsu0.cp v12 v10 //mincost
lsu0.sto.64.l v20 i17 0 || lsu1.sto.64.h v20 i17 8 || iau.add i17 i17 16 


//bru.rpim 0
.nowarnend

bru.jmp i30            
nop 6                  
.size mvcvCensusMinSubpixel3_asm,.-mvcvCensusMinSubpixel3_asm
.end
