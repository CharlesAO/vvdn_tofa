/// @file
/// @copyright Three code copyright Movidius Ltd 2015, Three rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
    
.type mvcvCensusMinSubpixel3Window_asm,@function
.version 00.51.04

.data .data.CensusMinSubpixel3Window
.align 4

.code .text.CensusMinSubpixel3Window

.lalign
//void mvcvCensusMinSubpixel3Window(u8 *in, half *out, u8 *outm, u32 width)
//                                    i18      i17        i16        i15
mvcvCensusMinSubpixel3Window_asm:
        IAU.SUB i19 i19 8
	LSU0.ST.64.l v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v24  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v25  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v26  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.l v27  i19  || IAU.SUB i19 i19 8
	LSU0.ST.64.h v27  i19 


iau.shr.u32 i15 i15 3
lsu0.ldil i14 .LmvcvCensusMinSubpixel3Window_asm_loop || lsu1.ldih i14 .LmvcvCensusMinSubpixel3Window_asm_loop
lsu0.ldil i0 0x0
lsu0.ldil i1 0x1
lsu0.ldil i2 0x2
lsu0.ldil i3 0xb800
lsu0.ldil i4 0x3800
cmu.cpivr.x16 v15 i0 //poz 0
cmu.cpivr.x16 v16 i1 //poz 1
cmu.cpivr.x16 v17 i2 //poz 2
cmu.cpivr.x16 v23 i3 // -0.5 fp16
cmu.cpivr.x16 v21 i4 // -0.5 fp16

lsu0.ldi.128.u8.u16 v0 i18 
lsu0.ldi.128.u8.u16 v1 i18
lsu0.ldi.128.u8.u16 v2 i18
lsu0.ldi.128.u8.u16 v24 i18
lsu0.ldi.128.u8.u16 v25 i18
nop 2
nop 3


cmu.alignvec v3 v0 v0 0    || lsu0.swzc8 [43726150] || lsu1.cp i5 v0.2
cmu.alignvec v4 v1 v1 0    || lsu0.swzc8 [50614372] || lsu1.cp i6 v1.0
cmu.alignvec v5 v2 v2 0    || lsu0.swzc8 [72615043] || lsu1.cp i7 v1.3
cmu.alignvec v6 v24 v24 0  || lsu0.swzc8 [50437261] || lsu1.cp i8 v24.0
cmu.alignvec v27 v25 v25 0 || lsu0.swzc8 [72615043] || lsu1.cp i9 v24.2
CMU.TP4R v8 v3                                        
cmu.alignvec  v26  v8  v8 0   || lsu0.swzc8 [47653210] || lsu1.cp i0 v27.0
cmu.alignvec  v9  v9  v9 0   || lsu0.swzc8 [37654210] || lsu1.cp i1 v27.1
cmu.alignvec v10 v10 v10 0   || lsu0.swzc8 [72654310] || lsu1.cp i2 v27.2
cmu.alignvec v11 v11 v11 0   || lsu0.swzc8 [76154320] || lsu1.cp i3 v26.3
cmu.cpiv.x16  v26.7 i0.l                               || lsu1.cp i4 v9.3
cmu.cpiv.x16  v9.7 i0.h 
cmu.cpiv.x16 v10.2 i4.h 
cmu.cpiv.x32 v10.3 i1
cmu.cpiv.x16 v11.5 i9.l
cmu.cpiv.x32 v11.3 i2
cmu.cpiv.x16 v27.0 i5.l
cmu.cpiv.x16 v27.1 i6.h
cmu.cpiv.x16 v27.2 i7.l
cmu.cpiv.x16 v27.3 i3.h
cmu.cpiv.x16 v27.4 i8.l
cmu.cpiv.x16 v27.5 i9.h


bru.rpl i14 i15   




cmu.cpvv v12  v9      || lsu1.ldi.128.u8.u16 v0 i18      //mincost
cmu.cpvv v13  v26     || lsu1.ldi.128.u8.u16 v1 i18    //preCost
cmu.cpvv v14 v10      || lsu1.ldi.128.u8.u16 v2 i18     //postCost
vau.xor v18 v18 v18   || lsu1.ldi.128.u8.u16 v24 i18      // minPos
                      
cmu.cmvv.u16 v10 v12  || lsu1.ldi.128.u8.u16 v25 i18 
peu.pvl016 lt || lsu0.cp v12 v10 //mincost
peu.pvl016 lt || lsu0.cp v13 v9  //precost
peu.pvl016 lt || lsu0.cp v14 v11 //postcost
peu.pvl016 lt || lsu0.cp v18 v16 //minpos 

cmu.cmvv.u16 v11 v12
peu.pvl016 lt || lsu0.cp v12 v11 //mincost   -f1     outm[i] = minCost;
peu.pvl016 lt || lsu0.cp v13 v10 //precost   -f0
peu.pvl016 lt || lsu0.cp v14 v27 //postcost  -f2
peu.pvl016 lt || lsu0.cp v18 v17 //minpos


vau.shl.x16 v20 v12 1   // 2 * f1
vau.add.i16 v19 v13 v14 // f0 + f2
nop 2
vau.sub.i16 v19 v19 v20 || cmu.cpivr.x32 v22 i18//a
nop 2
//bru.rpim 0

cmu.cmz.i16 v19
peu.pvv16 eq || vau.xor v22 v19 v19  

vau.shl.x16 v19 v19 1 //denom
vau.sub.i16 v20 v13 v14 //number

//bru.rpim 0
cmu.cpvv.i16.f16 v19 v19 //denom
cmu.cpvv.i16.f16 v20 v20 //number
nop
lsu0.cp i0 v19.0 || lsu1.cp i4 v20.0
lsu0.cp i1 v19.1 || lsu1.cp i5 v20.1
lsu0.cp i2 v19.2 || lsu1.cp i6 v20.2
lsu0.cp i3 v19.3 || lsu1.cp i7 v20.3

sau.div.f16 i0 i4 i0 
sau.div.f16 i1 i5 i1 || cmu.alignvec v3 v0 v0 0    || lsu0.swzc8 [43726150] || lsu1.cp i5 v0.2
sau.div.f16 i2 i6 i2 || cmu.alignvec v4 v1 v1 0    || lsu0.swzc8 [50614372] || lsu1.cp i6 v1.0
sau.div.f16 i3 i7 i3 || cmu.alignvec v5 v2 v2 0    || lsu0.swzc8 [72615043] || lsu1.cp i7 v1.3
cmu.alignvec v6 v24 v24 0  || lsu0.swzc8 [50437261] || lsu1.cp i8 v24.0
cmu.alignvec v27 v25 v25 0 || lsu0.swzc8 [72615043] || lsu1.cp i9 v24.2
CMU.TP4R v8 v3                                        
cmu.alignvec  v26  v8  v8 0   || lsu0.swzc8 [47653210] 
cmu.cpiv.x32 v20.0 i0
cmu.cpiv.x32 v20.1 i1
cmu.cpiv.x32 v20.2 i2
cmu.cpiv.x32 v20.3 i3

cmu.cmz.i16 v22
peu.pvl016 eq || lsu0.cp v20 v22 
CMU.CLAMPAB.f16 v20 v23 v21
nop
cmu.cpvv.i16.f16 v18 v18 //minpos in fp16
nop
vau.add.f16 v20 v20 v18  // out[i] = (half)minPos + f;
cmu.alignvec  v9  v9  v9 0   || lsu0.swzc8 [37654210]  || lsu1.cp i0 v27.0
cmu.alignvec v10 v10 v10 0   || lsu0.swzc8 [72654310]  || lsu1.cp i1 v27.1

lsu0.sto.64.l v20 i17 0 || lsu1.sto.64.h v20 i17 8 || iau.add i17 i17 16 
cmu.alignvec v11 v11 v11 0   || lsu0.swzc8 [76154320]  || lsu1.cp i2 v27.2
lsu0.sti.128.u16.u8 v12 i16  || cmu.cpiv.x16  v26.7 i0.l                               || lsu1.cp i3 v26.3
cmu.cpiv.x16  v9.7 i0.h                                || lsu1.cp i4 v9.3
cmu.cpiv.x16 v10.2 i4.h 
cmu.cpiv.x32 v10.3 i1
cmu.cpiv.x16 v11.5 i9.l
.LmvcvCensusMinSubpixel3Window_asm_loop:
cmu.cpiv.x32 v11.3 i2
cmu.cpiv.x16 v27.0 i5.l
cmu.cpiv.x16 v27.1 i6.h
cmu.cpiv.x16 v27.2 i7.l
cmu.cpiv.x16 v27.3 i3.h
cmu.cpiv.x16 v27.4 i8.l
cmu.cpiv.x16 v27.5 i9.h


	LSU0.LD.64.h  v27  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v27  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v26  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v25  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v25  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64.h  v24  i19 || IAU.ADD i19 i19 8    
	LSU0.LD.64.l  v24  i19 || IAU.ADD i19 i19 8	


bru.jmp i30            
nop 6                  
.size mvcvCensusMinSubpixel3Window_asm,.-mvcvCensusMinSubpixel3Window_asm
.end
