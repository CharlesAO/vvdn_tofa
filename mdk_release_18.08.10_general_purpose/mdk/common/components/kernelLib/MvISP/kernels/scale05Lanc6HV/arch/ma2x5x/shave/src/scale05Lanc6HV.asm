///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale with factor 0.5 (half) in each direction with lanczos kernel
///            Kernel size is 6. 
///
// scale05Lanc6HV

.type mvispScale05Lanc6HV_asm,@function
.version 00.51.04

.data .data.scale05Lanc6HV_asm
.align 4
//    kernelsValues:
//        .short -3, 7, 28, 28, 7, -3, 0, 0
     
.code .text.scale05Lanc6HV_asm      
.nowarn
mvispScale05Lanc6HV_asm:
lsu1.ldil i7, -3         ||lsu0.ldo.64 i1, i18, 0x00
lsu1.ldil i8, 7          ||cmu.cpivr.x16 v21, i7     ||lsu0.ldo.64 i3, i18, 0x08
lsu1.ldil i9, 28         ||cmu.cpivr.x16 v22, i8     ||lsu0.ldo.64 i5, i18, 0x10
lsu1.ldil i0, 0xF000     ||lsu0.ldih i0, 0x000F      ||cmu.cpivr.x16 v23, i9
cmu.cpivr.x32 v20, i0    ||lsu1.ldil i12, end_loop_scale05LancHV_asm        ||lsu0.ldih i12, end_loop_scale05LancHV_asm
iau.add i16, i16, 3      ||lsu1.ldil i7, 0xFFFD      ||lsu0.ldih i7, 0xFFFF || cmu.cpivr.x32 v6, i8
iau.shr.u32 i16, i16, 2      ||cmu.cpivr.x32 v5, i7
// 3 vectors generate -  vertical interpolation
iau.sub i1, i1, 0x08 || cmu.cpivr.x32 v7, i9
iau.sub i2, i2, 0x08 ||lsu0.ldi.128.u8.u16 v1, i1
iau.sub i3, i3, 0x08 ||lsu0.ldi.128.u8.u16 v1, i2
iau.sub i4, i4, 0x08 ||lsu0.ldi.128.u8.u16 v1, i3
iau.sub i5, i5, 0x08 ||lsu0.ldi.128.u8.u16 v1, i4
iau.sub i6, i6, 0x08 ||lsu0.ldi.128.u8.u16 v1, i5
lsu0.ldi.128.u8.u16 v1, i6
                                 lsu0.ldi.128.u8.u16 v1, i1
VAU.MACPZ.i16  v21  v1         ||lsu0.ldi.128.u8.u16 v1, i2
VAU.MACP.i16  v22  v1          ||lsu0.ldi.128.u8.u16 v1, i3
VAU.MACP.i16  v23  v1          ||lsu0.ldi.128.u8.u16 v1, i4
VAU.MACP.i16  v23  v1          ||lsu0.ldi.128.u8.u16 v1, i5
VAU.MACP.i16  v22  v1          ||lsu0.ldi.128.u8.u16 v1, i6
VAU.MACPW.i16  v8  v21  v1     ||lsu0.ldi.128.u8.u16 v1, i1    
VAU.MACPZ.i16  v21  v1         ||lsu0.ldi.128.u8.u16 v1, i2
VAU.MACP.i16  v22  v1          ||lsu0.ldi.128.u8.u16 v1, i3
VAU.MACP.i16  v23  v1          ||lsu0.ldi.128.u8.u16 v1, i4
VAU.MACP.i16  v23  v1          ||lsu0.ldi.128.u8.u16 v1, i5
VAU.MACP.i16  v22  v1          ||lsu0.ldi.128.u8.u16 v1, i6
VAU.MACPW.i16  v9  v21  v1     ||lsu0.ldi.128.u8.u16 v1, i1
VAU.MACPZ.i16  v21  v1         ||lsu0.ldi.128.u8.u16 v1, i2
VAU.MACP.i16  v22  v1          ||lsu0.ldi.128.u8.u16 v1, i3
VAU.MACP.i16  v23  v1          ||lsu0.ldi.128.u8.u16 v1, i4
VAU.MACP.i16  v23  v1          ||lsu0.ldi.128.u8.u16 v1, i5
VAU.MACP.i16  v22  v1          ||lsu0.ldi.128.u8.u16 v1, i6
VAU.MACPW.i16  v10 v21  v1    
VAU.MACPZ.i16  v21  v1         ||cmu.alignvec v2, v8, v9, 12      ||lsu1.cp v8, v9    
VAU.MACP.i16  v22  v1          ||cmu.vdilv.x16 v13, v12, v2, v2
VAU.MACP.i16  v23  v1          ||cmu.vdilv.x16 v15, v14, v9, v9
VAU.MACP.i16  v23  v1          ||cmu.alignvec v3, v9, v10, 4      ||lsu1.cp v9, v10
VAU.MACP.i16  v22  v1          ||cmu.vdilv.x16 v17, v16, v3, v3
VAU.MACPW.i16  v10 v21  v1     ||cmu.cpvv.i16.i32 v12, v12        ||lsu0.ldi.128.u8.u16 v1, i1
cmu.cpvv.i16.i32 v13, v13      ||VAU.MACPZ.i32 v5   v12           ||lsu0.ldi.128.u8.u16 v1, i2 
cmu.cpvv.i16.i32 v14, v14      ||VAU.MACP.i32  v6   v13           ||lsu0.ldi.128.u8.u16 v1, i3 
cmu.cpvv.i16.i32 v15, v15      ||VAU.MACP.i32  v7   v14           ||lsu0.ldi.128.u8.u16 v1, i4 
cmu.cpvv.i16.i32 v16, v16      ||VAU.MACP.i32  v7   v15           ||lsu0.ldi.128.u8.u16 v1, i5 
cmu.cpvv.i16.i32 v17, v17      ||VAU.MACP.i32  v6   v16           ||lsu0.ldi.128.u8.u16 v1, i6 
VAU.MACPW.i32  v18, v5   v17  
VAU.MACPZ.i16  v21  v1         ||cmu.alignvec v2, v8, v9, 12      ||lsu1.cp v8, v9    
VAU.MACP.i16  v22  v1          ||cmu.vdilv.x16 v13, v12, v2, v2
VAU.MACP.i16  v23  v1          ||cmu.vdilv.x16 v15, v14, v9, v9
VAU.MACP.i16  v23  v1          ||cmu.alignvec v3, v9, v10, 4      ||lsu1.cp v9, v10
VAU.MACP.i16  v22  v1          ||cmu.vdilv.x16 v17, v16, v3, v3
VAU.MACPW.i16  v10 v21  v1     ||cmu.cpvv.i16.i32 v12, v12
cmu.cpvv.i16.i32 v13, v13      ||VAU.MACPZ.i32 v5   v12           ||lsu0.ldi.128.u8.u16 v1, i1
cmu.cpvv.i16.i32 v14, v14      ||VAU.MACP.i32  v6   v13           ||lsu0.ldi.128.u8.u16 v1, i2     
cmu.cpvv.i16.i32 v15, v15      ||VAU.MACP.i32  v7   v14           ||lsu0.ldi.128.u8.u16 v1, i3 
cmu.cpvv.i16.i32 v16, v16      ||VAU.MACP.i32  v7   v15           ||lsu0.ldi.128.u8.u16 v1, i4 
cmu.cpvv.i16.i32 v17, v17      ||VAU.MACP.i32  v6   v16           ||lsu0.ldi.128.u8.u16 v1, i5 
VAU.MACPW.i32  v18, v5   v17   ||cmu.clamp0.i32 v19, v18, v20     ||lsu0.ldi.128.u8.u16 v1, i6 
nop
// loop start 
VAU.MACPZ.i16  v21  v1         ||cmu.alignvec v2, v8, v9, 12                          ||lsu1.cp i7 , v19.0       || bru.rpl i12, i16, _end_loop_scale05LancHV_asm 
VAU.MACP.i16  v22  v1          ||cmu.vdilv.x16 v13, v12, v2, v2                       ||lsu1.cp i8 , v19.1       || iau.shr.u32 i7 , i7 , 12
VAU.MACP.i16  v23  v1          ||cmu.vdilv.x16 v15, v14, v9, v9                       ||lsu1.cp i9 , v19.2       || iau.shr.u32 i8 , i8 , 12
VAU.MACP.i16  v23  v1          ||cmu.alignvec v3, v9, v10, 4                          ||lsu1.cp i10, v19.3       || iau.shr.u32 i9 , i9 , 12
VAU.MACP.i16  v22  v1          ||cmu.vdilv.x16 v17, v16, v3, v3   ||lsu1.cp v8, v9    ||iau.shr.u32 i10, i10, 12
VAU.MACPW.i16  v10 v21  v1     ||cmu.cpvv.i16.i32 v12, v12        ||lsu1.cp v9, v10   ||iau.fins i11, i7 , 0, 8
end_loop_scale05LancHV_asm:                                                         
cmu.cpvv.i16.i32 v13, v13      ||VAU.MACPZ.i32 v5   v12                               ||iau.fins i11, i8 , 8, 8  ||lsu0.ldi.128.u8.u16 v1, i1
cmu.cpvv.i16.i32 v14, v14      ||VAU.MACP.i32  v6   v13                               ||iau.fins i11, i9 ,16, 8  ||lsu0.ldi.128.u8.u16 v1, i2
cmu.cpvv.i16.i32 v15, v15      ||VAU.MACP.i32  v7   v14                               ||iau.fins i11, i10,24, 8  ||lsu0.ldi.128.u8.u16 v1, i3
cmu.cpvv.i16.i32 v16, v16      ||VAU.MACP.i32  v7   v15                               ||lsu1.sti.32 i11, i17     ||lsu0.ldi.128.u8.u16 v1, i4
cmu.cpvv.i16.i32 v17, v17      ||VAU.MACP.i32  v6   v16                                                          ||lsu0.ldi.128.u8.u16 v1, i5
VAU.MACPW.i32  v18, v5   v17   ||cmu.clamp0.i32 v19, v18, v20                                                    ||lsu0.ldi.128.u8.u16 v1, i6
nop                                                                                 
_end_loop_scale05LancHV_asm:                                                        
    
bru.jmp i30 
nop 6

.size mvispScale05Lanc6HV_asm,.-mvispScale05Lanc6HV_asm
.nowarnend
.end
