///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvPyrdown_asm,@function
.version 00.50.05
.data .rodata.pyrdown
.align 4
.salign 16

.code .text.pyrdown //text
.salign 16
//void mvcvPyrdown(u8 **inLine,u8 **out,int width)
//                   i18           i17        i16
mvcvPyrdown_asm:

lsu0.ldil i8, pyrdown__GaussDownV_Loop   ||lsu1.ldih i8, pyrdown__GaussDownV_Loop 
iau.shr.u32 i16, i16, 4

lsu0.ld.32 i17 i17
nop 6

lsu0.ldil i9 16
lsu0.ldil i9 64 || cmu.cpivr.x16 v21 i9 // 16
lsu0.ldil i9 96 || cmu.cpivr.x16 v22 i9 // 64
                   cmu.cpivr.x16 v23 i9 // 96 

LSU0.LDO.64 i0,  i18, 0x00 
LSU0.LDO.64 i2,  i18, 0x08 
LSU0.LDO.32 i4,  i18, 0x10
nop 6

IAU.SUB i0 i0 8
IAU.SUB i1 i1 8
IAU.SUB i2 i2 8
IAU.SUB i3 i3 8
IAU.SUB i4 i4 8
LSU0.LDI.128.U8.U16 v0, i0
LSU0.LDI.128.U8.U16 v1, i1
LSU0.LDI.128.U8.U16 v2, i2
LSU0.LDI.128.U8.U16 v3, i3
LSU0.LDI.128.U8.U16 v4, i4
nop 6
LSU0.LDI.128.U8.U16 v5, i0
LSU0.LDI.128.U8.U16 v6, i1
LSU0.LDI.128.U8.U16 v7, i2
LSU0.LDI.128.U8.U16 v8, i3
LSU0.LDI.128.U8.U16 v9, i4
nop 6


VAU.MACPZ.u16  v0, v21
VAU.MACP.u16   v1, v22
VAU.MACP.u16   v2, v23
VAU.MACP.u16   v3, v22
VAU.MACPW.u16 v20, v21, v4 // first column
nop 5
VAU.MACPZ.u16  v5, v21
VAU.MACP.u16   v6, v22
VAU.MACP.u16   v7, v23
VAU.MACP.u16   v8, v22
VAU.MACPW.u16 v2, v21, v9 // second column
nop 5

CMU.VSZM.BYTE v20, v20, [Z3Z1]
CMU.VSZM.BYTE v2, v2, [Z3Z1]

////////////////////////////////////////////////////////////


LSU0.LDI.128.U8.U16 v10, i0
LSU0.LDI.128.U8.U16 v11, i1
LSU0.LDI.128.U8.U16 v12, i2
LSU0.LDI.128.U8.U16 v13, i3
LSU0.LDI.128.U8.U16 v14, i4
LSU0.LDI.128.U8.U16 v15, i0
LSU0.LDI.128.U8.U16 v16, i1
LSU0.LDI.128.U8.U16 v17, i2 || VAU.MACPZ.u16  v10, v21
LSU0.LDI.128.U8.U16 v18, i3 || VAU.MACP.u16   v11, v22
LSU0.LDI.128.U8.U16 v19, i4 || VAU.MACP.u16   v12, v23
VAU.MACP.u16   v13, v22 
VAU.MACPW.u16 v0 , v21, v14 
VAU.MACPZ.u16  v15, v21
VAU.MACP.u16   v16, v22 

bru.rpl i8 i16
VAU.MACP.u16   v17, v23 
VAU.MACP.u16   v18, v22     || cmu.alignvec v3 v20 v2 12 //-2 
VAU.MACPW.u16 v1 , v21, v19 || cmu.alignvec v4 v20 v2 14 //-1
VAU.MACPZ.u16       v3, v21 || CMU.VSZM.BYTE v0, v0, [Z3Z1]// third column
VAU.MACP.u16        v4, v22 || cmu.alignvec v3 v2 v0 12 //-2 
VAU.MACP.u16        v2, v23 || cmu.alignvec v5 v2 v0 2  //+1
VAU.MACP.u16        v5, v22 || cmu.alignvec v6 v2 v0 4 || LSU0.LDI.128.U8.U16 v10, i0 //+2 
VAU.MACPW.u16 v7 , v6, v21  || CMU.VSZM.BYTE v1, v1, [Z3Z1] || LSU0.LDI.128.U8.U16 v11, i1// third column
VAU.MACPZ.u16       v3, v21 || cmu.alignvec v4 v2 v0 14 || LSU0.LDI.128.U8.U16 v12, i2 //-1
VAU.MACP.u16        v4, v22 || cmu.alignvec v5 v0 v1 2 || LSU0.LDI.128.U8.U16 v13, i3 //+1
VAU.MACP.u16        v0, v23 || cmu.alignvec v6 v0 v1 4 || LSU0.LDI.128.U8.U16 v14, i4 //+2 
VAU.MACP.u16        v5, v22 || cmu.cpvv v20 v0 || LSU0.LDI.128.U8.U16 v15, i0
VAU.MACPW.u16 v6 , v6, v21  || cmu.cpvv v2 v1 || LSU0.LDI.128.U8.U16 v16, i1
pyrdown__GaussDownV_Loop:
LSU0.LDI.128.U8.U16 v17, i2 || VAU.MACPZ.u16  v10, v21
LSU0.LDI.128.U8.U16 v18, i3 || VAU.MACP.u16   v11, v22
LSU0.LDI.128.U8.U16 v19, i4 || VAU.MACP.u16   v12, v23
cmu.vdilv.x16 v6 v7 v7 v6   || VAU.MACP.u16   v13, v22 
CMU.VSZM.BYTE v7, v7, [Z3Z1] || VAU.MACPW.u16 v0 , v21, v14 
LSU0.STI.128.U16.U8 v7, i17 || VAU.MACPZ.u16  v15, v21
VAU.MACP.u16   v16, v22 


BRU.JMP i30           
nop 6


.size mvcvPyrdown_asm,.-mvcvPyrdown_asm
.end
