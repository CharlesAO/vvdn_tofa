.type mvispPurpleFlare_asm,@function
.version 00.51.04

.data .data.purpleFlare_asm
.align 4

 purpleFlare_kernelVal:
        .int 1, 2, 4
 purpleFlare_minInt:
        .int 0x80000001
.nowarn
.code .text.purpleFlare_asm
//void mvispPurpleFlare(u16 *inGreen, u16 *blue[3], u16 *outBlue, u8 strength, u32 width)
//{
//    unsigned   i//
//    int         in, bFilt, hp, out//
//
//    for (i = 0// i < width// i++) {
//   in = blue[1][i]//    /* Centre pixel */
//
//   /* Apply kernel [ 1 2 1// 2 4 2// 1 2 1] */
//        bFilt = (blue[0][i-1] << 0) + (blue[0][i] << 1) + (blue[0][i+1] << 0) +
//                (blue[1][i-1] << 1) + (in         << 2) + (blue[1][i+1] << 1) +
//                (blue[2][i-1] << 0) + (blue[2][i] << 1) + (blue[2][i+1] << 0)//
//
//   /* High pass (blurred version subtracted from centre pixel) */
//        hp = in - (bFilt >> 4)//
//
//   /* Only darken blue pixels, never brighten them */
//   if (hp > 0) {
//       hp = 0//
//   }
//
//   out = in + hp * strength//
//
//   /*
//    * Never darken the blue channel such that its value is less than
//    * that of the green channel.  To achieve this, clamp the new blue
//    * value to be between the old blue and green values.
//    */
//   outBlue[i] = MIN(in, MAX(inGreen[i], out))//
//    }
//}

//                      i18          i17             i16          i15           i14
// void mvispPurpleFlare(u16 *inGreen, u16 *blue[3], u16 *outBlue, u8 strength, u32 width)

mvispPurpleFlare_asm:
    LSU0.LDO.64 i0 i17 0 || IAU.ADD i17 i17 8
    LSU1.LD.32 i2 i17    || IAU.SHL i15 i15 24 //load blue - pointers to the first 3 lines 
    LSU0.LDIL i8 purpleFlare_endLoop   || LSU1.LDIH i8 purpleFlare_endLoop   || IAU.SHR.i32 i14 i14 2 // process 4 pixels
    LSU0.LDIL i3 purpleFlare_kernelVal || LSU1.LDIH i3 purpleFlare_kernelVal || IAU.SHR.u32 i15 i15 24
    LSU0.LDIL i4 purpleFlare_minInt    || LSU1.LDIH i4 purpleFlare_minInt    || CMU.CPIVR.x32 v19 i15
    
    //load inGreen, outBlue, strength
    NOP
    //|| LSU1.LD.32 i16 i16
    
    LSU0.LDO.64.L v1 i3 0 || LSU1.LDO.64.H v1 i3 8
    
    LSU0.LD.32 i4 i4
    // load 4 pixels from each Line
    IAU.SUB i0 i0 8
    IAU.SUB i1 i1 8
    IAU.SUB i2 i2 8

    LSU0.LDi.128.U16.U32 v2 i0 || LSU1.LDi.128.U16.U32 v3 i1
    LSU0.LDi.128.U16.U32 v4 i2
    LSU0.LDi.128.U16.U32 v5 i0 || LSU1.LDi.128.U16.U32 v6 i1
    LSU0.LDi.128.U16.U32 v7 i2
    LSU0.LDi.128.U16.U32 v8 i0 || LSU1.LDi.128.U16.U32 v9 i1
    LSU0.LDi.128.U16.U32 v10 i2
    
    // minInt, zero
    CMU.CPIVR.x32 v17 i4 || VAU.XOR v18 v18 v18
    NOP 2
    CMU.ALIGNVEC v11 v2  v5 12
    CMU.ALIGNVEC v12 v3  v6 12
    CMU.ALIGNVEC v14 v5  v8 4
    CMU.ALIGNVEC v13 v4  v7 12
    CMU.ALIGNVEC v15 v6  v9 4
    CMU.ALIGNVEC v16 v7 v10 4
    
    VAU.MACPZ.i32 v1  v5        || LSU1.SWZMV4.WORD [1111] [UUUU]   || LSU0.LDi.128.U16.U32 v8  i0 || CMU.CPVV v2 v5
    VAU.MACP.i32  v1  v6        || LSU1.SWZMV4.WORD [2222] [UUUU]   || LSU0.LDi.128.U16.U32 v9  i1 || CMU.CPVV v3 v6
    VAU.MACP.i32  v1  v7        || LSU1.SWZMV4.WORD [1111] [UUUU]   || LSU0.LDi.128.U16.U32 v10 i2 || CMU.CPVV v4 v7
    VAU.MACP.i32  v1  v11       || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.CPVV v5 v8              || lsu0.CP v6 v9
    VAU.MACP.i32  v1  v14       || LSU1.SWZMV4.WORD [0000] [UUUU]   || lsu0.CP v7 v10              || CMU.ALIGNVEC v11 v2 v5 12
    VAU.MACP.i32  v1  v13       || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.ALIGNVEC v13 v4 v7 12
    VAU.MACP.i32  v1  v12       || LSU1.SWZMV4.WORD [1111] [UUUU]   || CMU.ALIGNVEC v12 v3 v6 12 
    VAU.MACP.i32  v1  v15       || LSU1.SWZMV4.WORD [1111] [UUUU]   || CMU.ALIGNVEC v14 v5 v8 4
    VAU.MACPW.i32 v20 v1 v16    || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.ALIGNVEC v15 v6 v9  4
    CMU.ALIGNVEC v16 v7 v10 4
    
    //****
    
    VAU.MACPZ.i32 v1  v5        || LSU1.SWZMV4.WORD [1111] [UUUU]   || LSU0.LDi.128.U16.U32 v9  i1  || CMU.CPVV v2 v5
    VAU.MACP.i32  v1  v6        || LSU1.SWZMV4.WORD [2222] [UUUU]   || LSU0.LDi.128.U16.U32 v10  i2 || CMU.CPVV v22 v6
    VAU.SHR.u32 v20 v20 4       || CMU.CPVV v5 v8 
    VAU.MACP.i32  v1  v7        || LSU1.SWZMV4.WORD [1111] [UUUU]   || LSU0.LDi.128.U16.U32 v8 i0   || CMU.CPVV v4 v7 
    VAU.SUB.i32 v20 v3 v20      || CMU.CPVV v7 v10
    VAU.MACP.i32  v1  v11       || LSU1.SWZMV4.WORD [0000] [UUUU]   || lsu0.CP v6 v9                || CMU.ALIGNVEC v11 v2 v5 12
    CMU.CLAMPAB.i32 v20 v17 v18 || LSU0.LDi.128.U16.U32 v0 i18  
    VAU.MACP.i32  v1  v14       || LSU1.SWZMV4.WORD [0000] [UUUU] 
    VAU.MUL.i32 v20 v20 v19     || LSU0.CP v23 v3
    VAU.MACP.i32  v1  v15       || LSU1.SWZMV4.WORD [1111] [UUUU]   || CMU.ALIGNVEC v15 v6 v9 4
    VAU.MACP.i32  v1  v13       || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.ALIGNVEC v14 v5 v8 4 
    nop 2
    VAU.ADD.i32 v21 v3 v20      || CMU.ALIGNVEC v13 v4 v7 12        || LSU0.CP v3 v22
    VAU.MACP.i32  v1  v12       || LSU1.SWZMV4.WORD [1111] [UUUU]   || CMU.ALIGNVEC v12 v3 v6 12 
    VAU.MACPW.i32 v20 v1 v16    || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.ALIGNVEC v16 v7 v10 4
    CMU.MAX.i32 v21 v0 v21

    VAU.MACPZ.i32 v1  v5        || LSU1.SWZMV4.WORD [1111] [UUUU]   || LSU0.LDi.128.U16.U32 v9  i1  || BRU.RPL i8 i14 purpleFlare_endDelaySlot
    VAU.MACP.i32  v1  v6        || LSU1.SWZMV4.WORD [2222] [UUUU]   || LSU0.LDi.128.U16.U32 v10  i2 || CMU.CPVV v22 v6
    VAU.SHR.u32 v20 v20 4       || CMU.CPVV v5 v8                   || LSU0.CP v2 v5
    VAU.MACP.i32  v1  v7        || LSU1.SWZMV4.WORD [1111] [UUUU]   || LSU0.LDi.128.U16.U32 v8 i0   || CMU.CPVV v4 v7 
    VAU.SUB.i32 v20 v3 v20      || CMU.CPVV v7 v10
    VAU.MACP.i32  v1  v11       || LSU1.SWZMV4.WORD [0000] [UUUU]   || lsu0.CP v6 v9                || CMU.ALIGNVEC v11 v2 v5 12
    CMU.CLAMPAB.i32 v20 v17 v18 || LSU0.LDi.128.U16.U32 v0 i18  
    VAU.MACP.i32  v1  v14       || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.MIN.i32 v21 v23 v21 
    VAU.MUL.i32 v20 v20 v19     || LSU0.CP v23 v3
    VAU.MACP.i32  v1  v15       || LSU1.SWZMV4.WORD [1111] [UUUU]   || CMU.ALIGNVEC v15 v6 v9 4

purpleFlare_endLoop:
    VAU.MACP.i32  v1  v13       || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.ALIGNVEC v14 v5 v8 4 
    nop 2
    VAU.ADD.i32 v21 v3 v20      || CMU.ALIGNVEC v13 v4 v7 12        || LSU1.STI.128.u32.u16 v21 i16 || LSU0.CP v3 v22
    VAU.MACP.i32  v1  v12       || LSU1.SWZMV4.WORD [1111] [UUUU]   || CMU.ALIGNVEC v12 v3 v6 12  
    VAU.MACPW.i32 v20 v1 v16    || LSU1.SWZMV4.WORD [0000] [UUUU]   || CMU.ALIGNVEC v16 v7 v10 4 
    CMU.MAX.i32 v21 v0 v21
purpleFlare_endDelaySlot:

    BRU.JMP i30
    NOP 6
    .nowarnend
.size mvispPurpleFlare_asm,.-mvispPurpleFlare_asm
.end
