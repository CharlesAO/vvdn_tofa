.type mvcvHuffman_encode_asm,@function
.version 00.50.00

//////
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
/// 


.data .data.mvcvHuffman_encode_asm
.align 16

zig_zag_label:
.int  0
.int  1, 8
.int 16, 9, 2
.int  3,10,17,24
.int 32,25,18,11, 4
.int  5,12,19,26,33,40
.int 48,41,34,27,20,13, 6
.int  7,14,21,28,35,42,49,56
.int 57,50,43,36,29,22,15
.int 23,30,37,44,51,58
.int 59,52,45,38,31
.int 39,46,53,60
.int 61,54,47
.int 55,62
.int 63



.code .text.mvcvHuffman_encode_asm
.align 16

.lalign
//void mvcvHuffman_encode(huffman_t * const ctx, const short data[], unsigned char *jpgbuff, unsigned int *jpgn, bitbuffer_t *bitbuf)
//                                     i18               i17                   i16                 i15                  i14
mvcvHuffman_encode_asm:
//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16
//i2 *hdclen//         u8
//i3 *hdcbit//        u16
//i4 *qtable//         u8
//i5 dc
.nowarn
    IAU.SUB i19 i19 16 || lsu0.ldo.32 i4 i18 16  
	LSU0.STo.64.l v24  i19 0  || LSU1.STo.64.h v24  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v25  i19 0  || LSU1.STo.64.h v25  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v26  i19 0  || LSU1.STo.64.h v26  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v27  i19 0  || LSU1.STo.64.h v27  i19 8 || IAU.SUB i19 i19 16 
	LSU0.STo.64.l v28  i19 0  || LSU1.STo.64.h v28  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v29  i19 0  || LSU1.STo.64.h v29  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v30  i19 0  || LSU1.STo.64.h v30  i19 8 || IAU.SUB i19 i19 16
	LSU0.STo.64.l v31  i19 0  || LSU1.STo.64.h v31  i19 8 || IAU.SUB i19 i19 8
	
lsu0.ldi.128.u16.u32 v16 i4
lsu0.ldi.128.u16.u32 v17 i4
lsu0.ldi.128.u16.u32 v18 i4
lsu0.ldi.128.u16.u32 v19 i4
lsu0.ldi.128.u16.u32 v20 i4
lsu0.ldi.128.u16.u32 v21 i4
lsu0.ldi.128.u16.u32 v22 i4
lsu0.ldi.128.u16.u32 v23 i4
lsu0.ldi.128.u16.u32 v24 i4
lsu0.ldi.128.u16.u32 v25 i4
lsu0.ldi.128.u16.u32 v26 i4
lsu0.ldi.128.u16.u32 v27 i4
lsu0.ldi.128.u16.u32 v28 i4
lsu0.ldi.128.u16.u32 v29 i4
lsu0.ldi.128.u16.u32 v30 i4
lsu0.ldi.128.u16.u32 v31 i4

LSU0.LDi.128.i16.i32 v0  i17
LSU0.LDi.128.i16.i32 v1  i17
LSU0.LDi.128.i16.i32 v2  i17
LSU0.LDi.128.i16.i32 v3  i17
LSU0.LDi.128.i16.i32 v4  i17
LSU0.LDi.128.i16.i32 v5  i17
LSU0.LDi.128.i16.i32 v6  i17
LSU0.LDi.128.i16.i32 v7  i17                                  || vau.mul.i32 v16 v0  v16
LSU0.LDi.128.i16.i32 v8  i17                                  || vau.mul.i32 v17 v1  v17
LSU0.LDi.128.i16.i32 v9  i17                                  || vau.mul.i32 v18 v2  v18
LSU0.LDi.128.i16.i32 v10 i17                                  || vau.mul.i32 v19 v3  v19
LSU0.LDi.128.i16.i32 v11 i17                                  || vau.mul.i32 v20 v4  v20
LSU0.LDi.128.i16.i32 v12 i17                                  || vau.mul.i32 v21 v5  v21
LSU0.LDi.128.i16.i32 v13 i17                                  || vau.mul.i32 v22 v6  v22
LSU0.LDi.128.i16.i32 v14 i17                                  || vau.mul.i32 v23 v7  v23
LSU0.LDi.128.i16.i32 v15 i17                                  || vau.mul.i32 v24 v8  v24
                                                                 vau.mul.i32 v25 v9  v25
                                                                 vau.mul.i32 v26 v10 v26
                                                                 vau.mul.i32 v27 v11 v27
                                                                 vau.mul.i32 v28 v12 v28
                                                                 vau.mul.i32 v29 v13 v29
                                                                 vau.mul.i32 v30 v14 v30
                                                                 vau.mul.i32 v31 v15 v31
nop
VAU.SHR.I32 v0   v0  15
VAU.SHR.I32 v1   v1  15
VAU.SHR.I32 v2   v2  15
VAU.SHR.I32 v3   v3  15
VAU.SHR.I32 v4   v4  15 || LSU0.ST.64  i20  i19 || IAU.SUB i19 i19 8
VAU.SHR.I32 v5   v5  15 || LSU1.ST.64  i22  i19 || IAU.SUB i19 i19 8
VAU.SHR.I32 v6   v6  15 || LSU0.ST.64  i24  i19 || IAU.SUB i19 i19 8
VAU.SHR.I32 v7   v7  15 || LSU1.ST.64  i26  i19 || IAU.SUB i19 i19 8
VAU.SHR.I32 v8   v8  15 || LSU0.ST.64  i28  i19 || IAU.SUB i19 i19 8
VAU.SHR.I32 v9   v9  15 || LSU1.ST.64  i30  i19 
VAU.SHR.I32 v10  v10 15
VAU.SHR.I32 v11  v11 15
VAU.SHR.I32 v12  v12 15
VAU.SHR.I32 v13  v13 15
VAU.SHR.I32 v14  v14 15
VAU.SHR.I32 v15  v15 15 || lsu0.ldo.64 i0 i18 0  
VAU.sub.I32 v0  v16  v0 || lsu0.ldo.64 i2 i18 8 || iau.add i18 i18 20
VAU.sub.I32 v1  v17  v1 
VAU.sub.I32 v2  v18  v2 
VAU.sub.I32 v3  v19  v3 
VAU.sub.I32 v4  v20  v4 
VAU.sub.I32 v5  v21  v5 
VAU.sub.I32 v6  v22  v6  
VAU.sub.I32 v7  v23  v7  || LSU0.LD.32.i16.i32 i5 i18 || lsu1.ldil i26 0x1 
VAU.sub.I32 v8  v24  v8  || lsu0.ld.32 i21 i15 || lsu1.ldil i9 0x1ff  //jpgn
VAU.sub.I32 v9  v25  v9  || lsu0.ld.64 i24 i14 || cmu.cpivr.x32 v23 i9
VAU.add.I32 v0  v0  v23  || cmu.cpii i31 i13// ((1 << (QTAB_SCALE - 1)) - 1)//buf &n
VAU.sub.I32 v10 v26  v10 || lsu1.ldih i30 0xDEAD 
VAU.SHR.I32 v0   v0  10  || lsu1.ldil i30 0xBEEF
VAU.sub.I32 v11 v27  v11 || lsu1.ldil i28 0x8 
VAU.sub.I32 v12 v28  v12  || cmu.cpvi.x32 i13 v0.0
VAU.sub.I32 v13 v29  v13  || iau.sub i10 i13 i5 // diff
VAU.sub.I32 v14 v30  v14  || iau.shr.i32 i11 i10 15
VAU.sub.I32 v15 v31  v15  || IAU.ADD i12 i11 i10 || lsu0.st.16 i13 i18 // bits
VAU.add.I32 v1  v1  v23   || IAU.ABS i11 i10
VAU.add.I32 v2  v2  v23   || IAU.BSR  i10 i11 
VAU.add.I32 v3  v3  v23   || cmu.cmz.i32 i11 || IAU.ADD i10 i10 1 //magn
VAU.add.I32 v4  v4  v23  || PEU.PCCX.EQ   1 || iau.xor i10 i10 i10
VAU.add.I32 v5  v5  v23  || IAU.SHL i5 i10 1 
VAU.add.I32 v6  v6  v23  || IAU.ADD i5 i3 i5
VAU.add.I32 v7  v7  v23  || IAU.ADD i13 i2 i10
VAU.add.I32 v8  v8  v23  || lsu0.ld.32.U8.U32 i13 i13 // ctx->hdclen[magn]
VAU.add.I32 v9  v9  v23  || lsu0.ld.32.U16.U32 i5 i5 // ctx->hdcbit[magn]
VAU.add.I32 v10 v10 v23  
VAU.add.I32 v11 v11 v23  
VAU.add.I32 v12 v12 v23  
VAU.add.I32 v13 v13 v23  
VAU.add.I32 v14 v14 v23  
VAU.add.I32 v15 v15 v23


iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1
iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits


cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit || lsu1.ldih i29 write_bits_exit || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop || lsu1.ldih i29 write_bits_loop
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4



write_bits_loop:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21

cmu.cmii.i32 i13 i28 || iau.shr.u32 i20 i24 i13
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || lsu0.sti.8 i20 i22 || iau.add i21 i21 1
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit:

//writebits2
cmu.cpii i13 i10 || iau.sub i25 i13 0


iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13 || cmu.cpii i5 i12
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27  // nbits


cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2 || lsu1.ldih i29 write_bits_exit2 || sau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2 || lsu1.ldih i29 write_bits_loop2
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop2:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21

cmu.cmii.i32 i13 i28 || iau.shr.u32 i20 i24 i13
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit2:



VAU.SHR.I32 v1   v1  10 || lsu1.cp i25 i13
VAU.SHR.I32 v2   v2  10 || iau.xor i18 i18 i18    || lsu1.ldil i2 zig_zag_label 
VAU.SHR.I32 v3   v3  10 || cmu.cpvi.x32 i18  v0.1 || lsu1.ldih i2 zig_zag_label
VAU.SHR.I32 v4   v4  10 || cmu.cpvi.x32 i18  v2.0 || lsu0.sto.16 i18 i2 0
VAU.SHR.I32 v5   v5  10
VAU.SHR.I32 v6   v6  10 || cmu.cpvi.x32 i18  v4.0 || lsu0.sto.16 i18 i2 2
VAU.SHR.I32 v7   v7  10 || cmu.cpvi.x32 i18  v2.1 || lsu0.sto.16 i18 i2 4
VAU.SHR.I32 v8   v8  10 || cmu.cpvi.x32 i18  v0.2 || lsu0.sto.16 i18 i2 6
VAU.SHR.I32 v9   v9  10 || cmu.cpvi.x32 i18  v0.3 || lsu0.sto.16 i18 i2 8
VAU.SHR.I32 v10  v10 10 || cmu.cpvi.x32 i18  v2.2 || lsu0.sto.16 i18 i2 10
VAU.SHR.I32 v11  v11 10 || cmu.cpvi.x32 i18  v4.1 || lsu0.sto.16 i18 i2 12
VAU.SHR.I32 v12  v12 10 || cmu.cpvi.x32 i18  v6.0 || lsu0.sto.16 i18 i2 14
VAU.SHR.I32 v13  v13 10 || cmu.cpvi.x32 i18  v8.0 || lsu0.sto.16 i18 i2 16
VAU.SHR.I32 v14  v14 10 || cmu.cpvi.x32 i18  v6.1 || lsu0.sto.16 i18 i2 18
VAU.SHR.I32 v15  v15 10 || cmu.cpvi.x32 i18  v4.2 || lsu0.sto.16 i18 i2 20
nop 

cmu.cpvi.x32 i18  v2.3 || lsu0.sto.16 i18 i2 22 
cmu.cpvi.x32 i18  v1.0 || lsu0.sto.16 i18 i2 24
cmu.cpvi.x32 i18  v1.1 || lsu0.sto.16 i18 i2 26 || lsu1.ldo.64.l v0 i2 0 
cmu.cpvi.x32 i18  v3.0 || lsu0.sto.16 i18 i2 28 || lsu1.ldo.64.h v0 i2 8 
cmu.cpvi.x32 i18  v4.3 || lsu0.sto.16 i18 i2 30
cmu.cpvi.x32 i18  v6.2 || lsu0.sto.16 i18 i2 32
cmu.cpvi.x32 i18  v8.1 || lsu0.sto.16 i18 i2 34
cmu.cpvi.x32 i18  v10.0 || lsu0.sto.16 i18 i2 36
cmu.cpvi.x32 i18  v12.0 || lsu0.sto.16 i18 i2 38
cmu.cpvi.x32 i18  v10.1 || lsu0.sto.16 i18 i2 40
cmu.cpvi.x32 i18  v8.2 || lsu0.sto.16 i18 i2 42
cmu.cpvi.x32 i18  v6.3 || lsu0.sto.16 i18 i2 44
cmu.cpvi.x32 i18  v5.0 || lsu0.sto.16 i18 i2 46   	  
cmu.cpvi.x32 i18  v3.1 || lsu0.sto.16 i18 i2 48   
cmu.cpvi.x32 i18  v1.2 || lsu0.sto.16 i18 i2 50   
cmu.cpvi.x32 i18  v1.3 || lsu0.sto.16 i18 i2 52   
lsu1.cp      i18  v3.2 || lsu0.sto.16 i18 i2 54   || cmu.alignvec v1 v0 v0 14		
lsu1.cp      i18  v5.1 || lsu0.sto.16 i18 i2 56   || cmu.alignvec v2 v1 v0 2
cmu.cpvi.x32 i18  v7.0 || lsu0.sto.16 i18 i2 58   
cmu.cpvi.x32 i18  v8.3 || lsu0.sto.16 i18 i2 60   || lsu1.ldil i4 extra_zero_skip
cmu.cpvi.x32 i18  v10.2 || lsu0.sto.16 i18 i2 62  || lsu1.ldih i4 extra_zero_skip 
cmu.cpvi.x32 i18  v12.1 || lsu0.sto.16 i18 i2 64  || lsu1.ldih i8 0x0
cmu.cpvi.x32 i18  v14.0 || lsu0.sto.16 i18 i2 66  || lsu1.ldil i8 0x6  
cmu.cpvi.x32 i18  v14.1 || lsu0.sto.16 i18 i2 68   
cmu.cpvi.x32 i18  v12.2 || lsu0.sto.16 i18 i2 70
cmu.cpvi.x32 i18  v10.3 || lsu0.sto.16 i18 i2 72
cmu.cpvi.x32 i18  v9.0 || lsu0.sto.16 i18 i2 74
cmu.cpvi.x32 i18  v7.1 || lsu0.sto.16 i18 i2 76
cmu.cpvi.x32 i18  v5.2 || lsu0.sto.16 i18 i2 78
cmu.cpvi.x32 i18  v3.3 || lsu0.sto.16 i18 i2 80   || lsu1.ldo.64.l v3 i2 14 
cmu.cpvi.x32 i18  v5.3 || lsu0.sto.16 i18 i2 82   || lsu1.ldo.64.h v3 i2 22 
cmu.cpvi.x32 i18  v7.2 || lsu0.sto.16 i18 i2 84   || lsu1.ldo.64.l v4 i2 30 
cmu.cpvi.x32 i18  v9.1 || lsu0.sto.16 i18 i2 86   || lsu1.ldo.64.h v4 i2 38 
cmu.cpvi.x32 i18  v11.0 || lsu0.sto.16 i18 i2 88 
cmu.cpvi.x32 i18  v12.3 || lsu0.sto.16 i18 i2 90
cmu.cpvi.x32 i18  v14.2 || lsu0.sto.16 i18 i2 92
cmu.cpvi.x32 i18  v14.3 || lsu0.sto.16 i18 i2 94
cmu.cpvi.x32 i18  v13.0 || lsu0.sto.16 i18 i2 96
cmu.cpvi.x32 i18  v11.1 || lsu0.sto.16 i18 i2 98
cmu.cpvi.x32 i18  v9.2 || lsu0.sto.16 i18 i2 100
cmu.cpvi.x32 i18  v7.3 || lsu0.sto.16 i18 i2 102
cmu.cpvi.x32 i18  v9.3 || lsu0.sto.16 i18 i2 104
cmu.cpvi.x32 i18  v11.2 || lsu0.sto.16 i18 i2 106
cmu.cpvi.x32 i18  v13.1 || lsu0.sto.16 i18 i2 108
cmu.cpvi.x32 i18  v15.0 || lsu0.sto.16 i18 i2 110
lsu1.cp      i18  v15.1 || lsu0.sto.16 i18 i2 112 
lsu1.cp      i18  v13.2 || lsu0.sto.16 i18 i2 114 
cmu.cpvi.x32 i18  v11.3 || lsu0.sto.16 i18 i2 116
cmu.cpvi.x32 i18  v13.3 || lsu0.sto.16 i18 i2 118 
cmu.cpvi.x32 i18  v15.2 || lsu0.sto.16 i18 i2 120 
cmu.cpvi.x32 i18  v15.3 || lsu0.sto.16 i18 i2 122 
                           lsu0.sto.16 i18 i2 124 || iau.add i2 i2 46
                 
//bru.rpim 0	

      
 
iau.xor i18 i18 i18 || lsu0.ldil i9 huffman_code_loop || lsu1.ldih i9 huffman_code_loop   || cmu.cmz.i16 v2 



//************************************************************


PEU.PC8C.AND eq || bru.jmp i4
cmu.cpvi.x16 i6.l v0.0
cmu.cpii.i16.i32 i6 i6
cmu.cpii i3 i6 || lsu0.ldil i7 in_loop_writebits9 || lsu1.ldih i7 in_loop_writebits9
nop 3
cmu.cmz.i32 i6 // i6 = ac
peu.pc1c eq || bru.jmp i7
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits



//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits



iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25  || sau.and i27 i5 i27

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2229 || lsu1.ldih i29 write_bits_exit2229  || sau.or i24 i27 i24 // nbits //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222290 || lsu1.ldih i29 write_bits_loop222290
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop222290:
iau.sub i13 i13 8
iau.shr.u32 i20 i24 i13
sau.add.i32 i22 i16 i21

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit2229:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || lsu1.cp i25 i13 


iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1
iau.add i13 i13 i25 || sau.and i27 i5 i27

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22229 || lsu1.ldih i29 write_bits_exit22229 || sau.or i24 i27 i24  //new pbb->buf // nbits
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222299 || lsu1.ldih i29 write_bits_loop222299
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop222299:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit22229:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits9:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************


cmu.cpvi.x16 i6.l v0.1
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits99 || lsu1.ldih i7 in_loop_writebits99 // i6 = ac
peu.pc1c eq || bru.jmp i7


iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits



//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits


iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22299 || lsu1.ldih i29 write_bits_exit22299 || sau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222909 || lsu1.ldih i29 write_bits_loop2222909
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222909:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit22299:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0


iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222299 || lsu1.ldih i29 write_bits_exit222299 || sau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222999 || lsu1.ldih i29 write_bits_loop2222999
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222999:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222299:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits99:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************

cmu.cpvi.x16 i6.l v0.2
cmu.cpii.i16.i32 i6 i6


cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits999 || lsu1.ldih i7 in_loop_writebits999// i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn

cmu.cmz.i32 i11 ||iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6


iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222999 || lsu1.ldih i29 write_bits_exit222999 || sau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229099 || lsu1.ldih i29 write_bits_loop22229099
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229099:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222999:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222999 || lsu1.ldih i29 write_bits_exit2222999 || sau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229999 || lsu1.ldih i29 write_bits_loop22229999
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0
nop 4

iau.xor i23 i23 i23

write_bits_loop22229999:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30



write_bits_exit2222999:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits999:
peu.pc1c eq || iau.add i18 i18 1


//************************************************************

cmu.cpvi.x16 i6.l v0.3
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits999a || lsu1.ldih i7 in_loop_writebits999a// i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn

cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6


iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits


iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222999a || lsu1.ldih i29 write_bits_exit222999a || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229099a || lsu1.ldih i29 write_bits_loop22229099a
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22229099a:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222999a:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222999a || lsu1.ldih i29 write_bits_exit2222999a || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229999a || lsu1.ldih i29 write_bits_loop22229999a
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22229999a:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222999a:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits999a:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************

cmu.cpvi.x16 i6.l v0.4
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits999aa || lsu1.ldih i7 in_loop_writebits999aa// i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn

cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits
iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222999aa || lsu1.ldih i29 write_bits_exit222999aa || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229099aa || lsu1.ldih i29 write_bits_loop22229099aa
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229099aa:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222999aa:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits
cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222999aa || lsu1.ldih i29 write_bits_exit2222999aa || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229999aa || lsu1.ldih i29 write_bits_loop22229999aa
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229999aa:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit2222999aa:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits999aa:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************

cmu.cpvi.x16 i6.l v0.5
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits999aaa || lsu1.ldih i7 in_loop_writebits999aaa// i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits
iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222999aaa || lsu1.ldih i29 write_bits_exit222999aaa || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229099aaa || lsu1.ldih i29 write_bits_loop22229099aaa
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4 


write_bits_loop22229099aaa:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
lsu0.sti.8 i20 i22 || iau.add i21 i21 1
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222999aaa:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits


cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222999aaa || lsu1.ldih i29 write_bits_exit2222999aaa || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229999aaa || lsu1.ldih i29 write_bits_loop22229999aaa
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229999aaa:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222999aaa:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits999aaa:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************

cmu.cpvi.x16 i6.l v0.6
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits999aaaa || lsu1.ldih i7 in_loop_writebits999aaaa// i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222999aaaa || lsu1.ldih i29 write_bits_exit222999aaaa || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229099aaaa || lsu1.ldih i29 write_bits_loop22229099aaaa
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229099aaaa:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222999aaaa:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222999aaaa || lsu1.ldih i29 write_bits_exit2222999aaaa || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229999aaaa || lsu1.ldih i29 write_bits_loop22229999aaaa
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229999aaaa:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222999aaaa:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebits999aaaa:
peu.pc1c eq || iau.add i18 i18 1

cmu.cmz.i16 v2
extra_zero_skip:
PEU.PC8C.AND eq || iau.add i18 i18 7

//extra_loop_unroll_begin************************************************************

cmu.cmz.i16 v3 || lsu0.ldil i4 fast_zeroxx || lsu1.ldih i4 fast_zeroxx
PEU.PC8C.AND eq || bru.jmp i4
nop 3

CMU.CPVV.i16.i32 v1 v3 || lsu0.ldil i17 fast_zero_smallxx || lsu1.ldih i17 fast_zero_smallxx
cmu.alignvec v2 v3 v3 8
CMU.CPVV.i16.i32 v2 v2
cmu.cmz.i32 v1

PEU.PC4C.AND eq || bru.jmp i17
nop 4

cmu.cpvi.x16 i6.l v3.0
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits0xxx || lsu1.ldih i7 in_loop_writebits0xxx // i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits


iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2220xxx || lsu1.ldih i29 write_bits_exit2220xxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22220xxx || lsu1.ldih i29 write_bits_loop22220xxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22220xxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30
write_bits_exit2220xxx:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0


iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22220xxx || lsu1.ldih i29 write_bits_exit22220xxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222290xxx || lsu1.ldih i29 write_bits_loop222290xxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop222290xxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28

peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22220xxx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebits0xxx:
PEU.PCCX.EQ  1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v3.1

//************************************************************


cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits0x1xx || lsu1.ldih i7 in_loop_writebits0x1xx// i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2220x1xx || lsu1.ldih i29 write_bits_exit2220x1xx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22220x1xx || lsu1.ldih i29 write_bits_loop22220x1xx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 5

write_bits_loop22220x1xx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28

peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2220x1xx:
//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22220x1xx || lsu1.ldih i29 write_bits_exit22220x1xx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222290x1xx || lsu1.ldih i29 write_bits_loop222290x1xx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop222290x1xx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28

peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22220x1xx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebits0x1xx:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v3.2

//************************************************************

cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits0x9xx || lsu1.ldih i7 in_loop_writebits0x9xx// i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits
iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2220x9xx || lsu1.ldih i29 write_bits_exit2220x9xx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22220x9xx || lsu1.ldih i29 write_bits_loop22220x9xx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22220x9xx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2220x9xx:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22220x9xx || lsu1.ldih i29 write_bits_exit22220x9xx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222290x9xx || lsu1.ldih i29 write_bits_loop222290x9xx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop222290x9xx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28

peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22220x9xx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits0x9xx:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v3.3

//************************************************************
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsxx || lsu1.ldih i7 in_loop_writebitsxx// i6 = ac
peu.pc1c eq || bru.jmp i7


iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits
iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222xx || lsu1.ldih i29 write_bits_exit222xx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222xx || lsu1.ldih i29 write_bits_loop2222xx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222xx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222xx:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222xx || lsu1.ldih i29 write_bits_exit2222xx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229xx || lsu1.ldih i29 write_bits_loop22229xx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22229xx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28

peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22

iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222xx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebitsxx:
peu.pc1c eq || iau.add i18 i18 1

cmu.cmz.i32 v1
fast_zero_smallxx:

PEU.PC4C.AND eq || iau.add i18 i18 4


//************************************************************
cmu.cmz.i32 v2 || lsu0.ldil i17 fast_zero_small1xx || lsu1.ldih i17 fast_zero_small1xx
PEU.PC4C.AND eq || bru.jmp i17
cmu.cpvi.x16 i6.l v3.4
cmu.cpii.i16.i32 i6 i6
nop 4

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsbxx || lsu1.ldih i7 in_loop_writebitsbxx // i6 = ac
peu.pc1c eq || bru.jmp i7


iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222bxx || lsu1.ldih i29 write_bits_exit222bxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222bxx || lsu1.ldih i29 write_bits_loop2222bxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop2222bxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13 || cmu.cmii.i32 i13 i28

peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222bxx:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222bxx || lsu1.ldih i29 write_bits_exit2222bxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229bxx || lsu1.ldih i29 write_bits_loop22229bxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22229bxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit2222bxx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebitsbxx:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************

cmu.cpvi.x16 i6.l v3.5
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsbbxx || lsu1.ldih i7 in_loop_writebitsbbxx // i6 = ac
peu.pc1c eq || bru.jmp i7

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits


//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6

IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222bbxx || lsu1.ldih i29 write_bits_exit222bbxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222bbxx || lsu1.ldih i29 write_bits_loop2222bbxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222bbxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222bbxx:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222bbxx || lsu1.ldih i29 write_bits_exit2222bbxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229bbxx || lsu1.ldih i29 write_bits_loop22229bbxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229bbxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit2222bbxx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebitsbbxx:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************

cmu.cpvi.x16 i6.l v3.6
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsbbbxx || lsu1.ldih i7 in_loop_writebitsbbbxx// i6 = ac
peu.pc1c eq || bru.jmp i7


iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222bbbxx || lsu1.ldih i29 write_bits_exit222bbbxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222bbbxx || lsu1.ldih i29 write_bits_loop2222bbbxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222bbbxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222bbbxx:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222bbbxx || lsu1.ldih i29 write_bits_exit2222bbbxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229bbbxx || lsu1.ldih i29 write_bits_loop22229bbbxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229bbbxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || lsu0.sti.8 i20 i22 || iau.add i21 i21 1
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222bbbxx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebitsbbbxx:
peu.pc1c eq || iau.add i18 i18 1

//************************************************************

cmu.cpvi.x16 i6.l v3.7
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsbbbbxx || lsu1.ldih i7 in_loop_writebitsbbbbxx// i6 = ac
peu.pc1c eq || bru.jmp i7


iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6


iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits


cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222bbbbxx || lsu1.ldih i29 write_bits_exit222bbbbxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222bbbbxx || lsu1.ldih i29 write_bits_loop2222bbbbxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop2222bbbbxx:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222bbbbxx:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits


cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222bbbbxx || lsu1.ldih i29 write_bits_exit2222bbbbxx || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229bbbbxx || lsu1.ldih i29 write_bits_loop22229bbbbxx
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229bbbbxx:
iau.sub i13 i13 8 || Sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222bbbbxx:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebitsbbbbxx:
peu.pc1c eq || iau.add i18 i18 1

cmu.cmz.i32 v2
fast_zero_small1xx:
PEU.PC4C.AND eq || iau.add i18 i18 4
nop 6

//************************************************************
cmu.cmz.i16 v3
fast_zeroxx:
PEU.PC8C.AND eq || iau.add i18 i18 8


//extra_loop_unroll_end************************************************************

lsu0.ldil i4 fast_zero || lsu1.ldih i4 fast_zero
//lsu0.ldil i14 0   || lsu1.ldih i14 0
//lsu0.ldil i15 0x1 || lsu1.ldih i15 0



cmu.cmz.i16 v4 || bru.rpl i9 i8 
PEU.PC8C.AND eq || bru.jmp i4
nop 3

CMU.CPVV.i16.i32 v1 v4 || lsu0.ldil i17 fast_zero_small || lsu1.ldih i17 fast_zero_small
cmu.alignvec v2 v4 v4 8
CMU.CPVV.i16.i32 v2 v2
cmu.cmz.i32 v1

PEU.PC4C.AND eq || bru.jmp i17
nop 4

cmu.cpvi.x16 i6.l v4.0
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits0x || lsu1.ldih i7 in_loop_writebits0x // i6 = ac
peu.pc1c eq || bru.jmp i7
nop 6

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_ex0x || lsu1.ldih i10 in_loop_writebits_ex0x
peu.pc1c lt || bru.jmp i10
lsu0.ldil i10 in_loop_writebits_160x || lsu1.ldih i10 in_loop_writebits_160x
nop 



in_loop_writebits_160x:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16

lsu0.ldil i13 0xb4 || lsu1.ldih i13 0
iau.shl i5  i13 1 
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5


//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit220x || lsu1.ldih i29 write_bits_exit220x || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop220x || lsu1.ldih i29 write_bits_loop220x
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop220x:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

 cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
 iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit220x:
iau.sub i18 i18 0x10 || cmu.cpii i25 i13 
cmu.cmii.i32 i18 i7
peu.pc1c gte || bru.jmp i10
nop 6
in_loop_writebits_ex0x:

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn

cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2220x || lsu1.ldih i29 write_bits_exit2220x || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22220x || lsu1.ldih i29 write_bits_loop22220x
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22220x:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30
write_bits_exit2220x:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22220x || lsu1.ldih i29 write_bits_exit22220x || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222290x || lsu1.ldih i29 write_bits_loop222290x
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop222290x:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22220x:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebits0x:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v4.1

//************************************************************
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits0x1 || lsu1.ldih i7 in_loop_writebits0x1// i6 = ac
peu.pc1c eq || bru.jmp i7
nop 6

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_ex0x1 || lsu1.ldih i10 in_loop_writebits_ex0x1
peu.pc1c lt || bru.jmp i10
lsu0.ldil i10 in_loop_writebits_160x1 || lsu1.ldih i10 in_loop_writebits_160x1
nop 



in_loop_writebits_160x1:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16

lsu0.ldil i13 0xb4 || lsu1.ldih i13 0
iau.shl i5  i13 1 

iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit220x1 || lsu1.ldih i29 write_bits_exit220x1 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop220x1 || lsu1.ldih i29 write_bits_loop220x1
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop220x1:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

 cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
 iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit220x1:

iau.sub i18 i18 0x10
cmu.cmii.i32 i18 i7 || iau.sub i25 i13 0
peu.pc1c gte || bru.jmp i10
nop 6
in_loop_writebits_ex0x1:

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2220x1 || lsu1.ldih i29 write_bits_exit2220x1 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22220x1 || lsu1.ldih i29 write_bits_loop22220x1
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 5

write_bits_loop22220x1:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
 iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2220x1:
//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22220x1 || lsu1.ldih i29 write_bits_exit22220x1 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222290x1 || lsu1.ldih i29 write_bits_loop222290x1
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop222290x1:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22220x1:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebits0x1:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v4.2

//************************************************************
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits0x9 || lsu1.ldih i7 in_loop_writebits0x9// i6 = ac
peu.pc1c eq || bru.jmp i7
nop 5

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_ex0x9 || lsu1.ldih i10 in_loop_writebits_ex0x9
peu.pc1c lt || bru.jmp i10
nop 
lsu0.ldil i10 in_loop_writebits_160x9 || lsu1.ldih i10 in_loop_writebits_160x9


in_loop_writebits_160x9:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16

lsu0.ldil i13 0xb4 || lsu1.ldih i13 0
iau.shl i5  i13 1 

iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5
//writebits


iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit220x9 || lsu1.ldih i29 write_bits_exit220x9 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop220x9 || lsu1.ldih i29 write_bits_loop220x9
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop220x9:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22

iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit220x9:
iau.sub i18 i18 0x10
cmu.cmii.i32 i18 i7 || iau.sub i25 i13 0
peu.pc1c gte || bru.jmp i10
nop 6
in_loop_writebits_ex0x9:

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2220x9 || lsu1.ldih i29 write_bits_exit2220x9 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22220x9 || lsu1.ldih i29 write_bits_loop22220x9
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22220x9:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2220x9:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22220x9 || lsu1.ldih i29 write_bits_exit22220x9 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop222290x9 || lsu1.ldih i29 write_bits_loop222290x9
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop222290x9:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22220x9:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits0x9:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v4.3

//************************************************************
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebits || lsu1.ldih i7 in_loop_writebits// i6 = ac
peu.pc1c eq || bru.jmp i7
nop 5

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_ex || lsu1.ldih i10 in_loop_writebits_ex
peu.pc1c lt || bru.jmp i10
nop 
lsu0.ldil i10 in_loop_writebits_16 || lsu1.ldih i10 in_loop_writebits_16


in_loop_writebits_16:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16

lsu0.ldil i13 0xb4 || lsu1.ldih i13 0x0
iau.shl i5  i13 1 

iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits


iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22 || lsu1.ldih i29 write_bits_exit22 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22 || lsu1.ldih i29 write_bits_loop22
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22:

iau.sub i18 i18 0x10 || cmu.cpii i25 i13
cmu.cmii.i32 i18 i7
peu.pc1c gte || bru.jmp i10
nop 6
in_loop_writebits_ex:

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits


iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222 || lsu1.ldih i29 write_bits_exit222 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222 || lsu1.ldih i29 write_bits_loop2222
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222 || lsu1.ldih i29 write_bits_exit2222 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229 || lsu1.ldih i29 write_bits_loop22229
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22229:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22

iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebits:
peu.pc1c eq || iau.add i18 i18 1

cmu.cmz.i32 v1
fast_zero_small:

PEU.PC4C.AND eq || iau.add i18 i18 4


//************************************************************
cmu.cmz.i32 v2 || lsu0.ldil i17 fast_zero_small1 || lsu1.ldih i17 fast_zero_small1
PEU.PC4C.AND eq || bru.jmp i17
cmu.cpvi.x16 i6.l v4.4
cmu.cpii.i16.i32 i6 i6
nop 4



cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsb || lsu1.ldih i7 in_loop_writebitsb // i6 = ac
peu.pc1c eq || bru.jmp i7
nop 6

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_exb || lsu1.ldih i10 in_loop_writebits_exb
peu.pc1c lt || bru.jmp i10
nop 6
lsu0.ldil i10 in_loop_writebits_16b || lsu1.ldih i10 in_loop_writebits_16b


in_loop_writebits_16b:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16


iau.xor i13 i13 i13 || sau.xor i5 i5 i5

iau.add i13 i13 180
iau.shl i5  i13 1 

iau.add i13 i0 i13 || sau.add.i32  i5 i1 i5 
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22b || lsu1.ldih i29 write_bits_exit22b || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22b || lsu1.ldih i29 write_bits_loop22b
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22b:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22b:

iau.sub i18 i18 0x10
cmu.cmii.i32 i18 i7 || iau.sub i25 i13 0
peu.pc1c gte || bru.jmp i10
nop 6
in_loop_writebits_exb:

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222b || lsu1.ldih i29 write_bits_exit222b || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222b || lsu1.ldih i29 write_bits_loop2222b
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop2222b:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222b:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222b || lsu1.ldih i29 write_bits_exit2222b || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229b || lsu1.ldih i29 write_bits_loop22229b
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22229b:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit2222b:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebitsb:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v4.5

//************************************************************




cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsbb || lsu1.ldih i7 in_loop_writebitsbb // i6 = ac
peu.pc1c eq || bru.jmp i7
nop 6

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_exbb || lsu1.ldih i10 in_loop_writebits_exbb
peu.pc1c lt || bru.jmp i10
nop 6
lsu0.ldil i10 in_loop_writebits_16bb || lsu1.ldih i10 in_loop_writebits_16bb


in_loop_writebits_16bb:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16


iau.xor i13 i13 i13 || sau.xor i5 i5 i5

iau.add i13 i13 180
iau.shl i5  i13 1 


iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5


//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22bb || lsu1.ldih i29 write_bits_exit22bb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22bb || lsu1.ldih i29 write_bits_loop22bb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22bb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30

cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit22bb:



iau.sub i18 i18 0x10
cmu.cmii.i32 i18 i7 || iau.sub i25 i13 0
peu.pc1c gte || bru.jmp i10
nop 6
in_loop_writebits_exbb:

cmu.cpii.i16.i32 i6 i6



iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits


//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6

IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222bb || lsu1.ldih i29 write_bits_exit222bb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222bb || lsu1.ldih i29 write_bits_loop2222bb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222bb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222bb:

//writebits

cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222bb || lsu1.ldih i29 write_bits_exit2222bb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229bb || lsu1.ldih i29 write_bits_loop22229bb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229bb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30


write_bits_exit2222bb:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebitsbb:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v4.6

//************************************************************
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsbbb || lsu1.ldih i7 in_loop_writebitsbbb// i6 = ac
peu.pc1c eq || bru.jmp i7
nop 6

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_exbbb || lsu1.ldih i10 in_loop_writebits_exbbb
peu.pc1c lt || bru.jmp i10
nop 6
lsu0.ldil i10 in_loop_writebits_16bbb || lsu1.ldih i10 in_loop_writebits_16bbb


in_loop_writebits_16bbb:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16

iau.xor i13 i13 i13 || sau.xor i5 i5 i5

iau.add i13 i13 180
iau.shl i5  i13 1 


iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22bbb || lsu1.ldih i29 write_bits_exit22bbb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22bbb || lsu1.ldih i29 write_bits_loop22bbb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 5

write_bits_loop22bbb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22bbb:

iau.sub i18 i18 0x10
cmu.cmii.i32 i18 i7 || iau.sub i25 i13 0
peu.pc1c gte || bru.jmp i10
nop 6
in_loop_writebits_exbbb:

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits




//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6

iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27 // nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222bbb || lsu1.ldih i29 write_bits_exit222bbb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222bbb || lsu1.ldih i29 write_bits_loop2222bbb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop2222bbb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222bbb:




//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0
//iau.add i14 i14 1
//cmu.cmii.i32 i14 i15
//peu.pc1c eq || bru.rpim 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222bbb || lsu1.ldih i29 write_bits_exit2222bbb || iau.or i24 i27 i24  //new pbb->buf

peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229bbb || lsu1.ldih i29 write_bits_loop22229bbb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229bbb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || lsu0.sti.8 i20 i22 || iau.add i21 i21 1
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222bbb:



cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13
in_loop_writebitsbbb:
PEU.PCCX.EQ 1 || iau.add i18 i18 1 || cmu.cpvi.x16 i6.l v4.7

//************************************************************
cmu.cpii.i16.i32 i6 i6

cmu.cmz.i32 i6 || iau.sub i3 i6 0 || lsu0.ldil i7 in_loop_writebitsbbbb || lsu1.ldih i7 in_loop_writebitsbbbb// i6 = ac
peu.pc1c eq || bru.jmp i7
nop 6

lsu0.ldil i7 0x10 || lsu1.ldih i7 0x0
cmu.cmii.i32 i18 i7 || lsu0.ldil i10 in_loop_writebits_exbbbb || lsu1.ldih i10 in_loop_writebits_exbbbb
peu.pc1c lt || bru.jmp i10
nop 6
lsu0.ldil i10 in_loop_writebits_16bbbb || lsu1.ldih i10 in_loop_writebits_16bbbb


in_loop_writebits_16bbbb:

//i0 (*haclen)[12]//   u8
//i1 (*hacbit)[12]//  u16


iau.xor i13 i13 i13 || sau.xor i5 i5 i5

iau.add i13 i13 180
iau.shl i5  i13 1 

iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits
iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit22bbbb || lsu1.ldih i29 write_bits_exit22bbbb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22bbbb || lsu1.ldih i29 write_bits_loop22bbbb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22bbbb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit22bbbb:
iau.sub i18 i18 0x10
cmu.cmii.i32 i18 i7 || lsu0.cp i25 i13
peu.pc1c gte || bru.jmp i10
nop 10
in_loop_writebits_exbbbb:

iau.xor i10 i10 i10
iau.shr.i32 i10 i6 15
iau.add i10 i6 i10 //bits

//huffman_magn
IAU.ABS i11 i6
IAU.BSR  i6 i11 
IAU.ADD i6 i6 1 //magn
nop
cmu.cmz.i32 i11 || iau.mul i13 i18 12
peu.pc1c eq || sau.xor i6 i6 i6


iau.add i13 i13 i6
IAU.shl i5 i13 1
iau.add  i5 i1 i5 
iau.add i13 i0 i13
lsu0.ld.32.U8.U32 i13 i13 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i5 // ctx->hacbit
nop 5

//writebits

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits


cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit222bbbb || lsu1.ldih i29 write_bits_exit222bbbb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop2222bbbb || lsu1.ldih i29 write_bits_loop2222bbbb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop2222bbbb:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit222bbbb:

//writebits
cmu.cpii i13 i6 || lsu0.cp i5 i10 || iau.sub i25 i13 0

iau.shl i24 i24 i13
iau.shl i27 i26 i13
iau.sub i27 i27 1

iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits


cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2222bbbb || lsu1.ldih i29 write_bits_exit2222bbbb || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22229bbbb || lsu1.ldih i29 write_bits_loop22229bbbb
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4

write_bits_loop22229bbbb:
iau.sub i13 i13 8 || Sau.add.i32 i22 i16 i21
iau.shr.u32 i20 i24 i13

cmu.cmii.i32 i21 i31 || iau.add i21 i21 1 || lsu0.sti.8 i20 i22
peu.pc1c gt || cmu.cpii i21 i30
cmu.cmii.i32 i13 i28
peu.pc1c gte || bru.jmp i29
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22
peu.pc1c eq || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30

write_bits_exit2222bbbb:

cmu.cmz.i32 i3 || iau.xor i18 i18 i18 || lsu0.cp i25 i13 
in_loop_writebitsbbbb:
peu.pc1c eq || iau.add i18 i18 1

cmu.cmz.i32 v2
fast_zero_small1:
PEU.PC4C.AND eq || iau.add i18 i18 4
nop 6

//************************************************************
cmu.cmz.i16 v4
fast_zero:
huffman_code_loop:
lsu0.ldo.64.l v4 i2 0 || lsu1.ldo.64.h v4 i2 8 || iau.add i2 i2 16
PEU.PC8C.AND eq || iau.add i18 i18 8
nop 5


//************************************************************


cmu.cmz.i32 i18 || lsu0.ldil i9 huffman_final || lsu1.ldih i9 huffman_final
peu.pc1c eq || bru.jmp i9

lsu0.ld.32.U8.U32 i13 i0 // ctx->haclen
lsu0.ld.32.U16.U32 i5 i1 // ctx->hacbit
nop 5

//writebits



iau.shl i27 i26 i13 || sau.shl.x32 i24 i24 i13
iau.sub i27 i27 1
iau.add i13 i13 i25 || sau.and i27 i5 i27// nbits

cmu.cmii.i32 i13 i28 || lsu0.ldil i29 write_bits_exit2221 || lsu1.ldih i29 write_bits_exit2221 || iau.or i24 i27 i24  //new pbb->buf
peu.pc1c lt || bru.jmp i29 
lsu0.ldil i29 write_bits_loop22221 || lsu1.ldih i29 write_bits_loop22221
lsu0.ldil i27 0xff || lsu1.ldih i27 0x0 || iau.xor i23 i23 i23
nop 4


write_bits_loop22221:
iau.sub i13 i13 8 || sau.add.i32 i22 i16 i21


cmu.cmii.i32 i13 i28 || iau.shr.u32 i20 i24 i13
peu.pc1c gte || bru.jmp i29
iau.add i21 i21 1 || lsu0.sti.8 i20 i22
iau.and i20 i20 i27
cmu.cmii.i32 i20 i27
peu.pc1c eq || lsu0.st.8 i23 i22 || iau.add i21 i21 1
cmu.cmii.i32 i21 i31
peu.pc1c gt || cmu.cpii i21 i30



write_bits_exit2221:
iau.sub i25 i13 0

huffman_final:





	LSU0.LD.64  i30  i19 || IAU.ADD i19 i19 8 || lsu1.st.32 i21 i15
	LSU0.LD.64  i28  i19 || IAU.ADD i19 i19 8 || lsu1.st.64 i24 i14
	LSU0.LD.64  i26  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64  i24  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64  i22  i19 || IAU.ADD i19 i19 8
	LSU0.LD.64  i20  i19 || IAU.ADD i19 i19 8
	LSU0.LDo.64.h  v31  i19 8 || LSU1.LDo.64.l  v31  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v30  i19 8 || LSU1.LDo.64.l  v30  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v29  i19 8 || LSU1.LDo.64.l  v29  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v28  i19 8 || LSU1.LDo.64.l  v28  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v27  i19 8 || LSU1.LDo.64.l  v27  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v26  i19 8 || LSU1.LDo.64.l  v26  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v25  i19 8 || LSU1.LDo.64.l  v25  i19 0 ||  IAU.ADD i19 i19 16
	LSU0.LDo.64.h  v24  i19 8 || LSU1.LDo.64.l  v24  i19 0 ||  IAU.ADD i19 i19 16 || BRU.jmp i30
    nop 6
	.nowarnend
.size mvcvHuffman_encode_asm,.-mvcvHuffman_encode_asm
.end
