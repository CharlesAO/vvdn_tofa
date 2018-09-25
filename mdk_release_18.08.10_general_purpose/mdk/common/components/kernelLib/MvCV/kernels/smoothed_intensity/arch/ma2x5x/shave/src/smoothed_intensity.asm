/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvSmoothed_intensity_asm,@function
.version 00.51.04

.data .data.mvcvSmoothed_intensity
.align 4


.code .text.mvcvSmoothed_intensity
//void mvcvSmoothed_intensity(float kp_x, float kp_y, u8* integral, int integral_stride, PatternPoint* pattern_pnt_lst, int pattern_pnt_sz, unsigned short* intensities)
//                                i18         i17           i16               i15                         i14                     i13                   i12
.lalign
mvcvSmoothed_intensity_asm:
.nowarn 10,40

lsu0.ldo.32 i1  i14 12 || lsu1.ldo.32 i0  i14  0
lsu0.ldo.32 i2  i14 24 || iau.xor i0 i0 i0 || cmu.cpivr.x32 v0 i18 //kp_x
lsu0.ldo.32 i3  i14 36 || LSU1.LDIH I0, 0x3FC0
lsu0.ldo.32 i4  i14  4 || cmu.cpivr.x32 v14 i0
lsu0.ldo.32 i5  i14 16 || LSU1.LDIL I1, 0xffff
lsu0.ldo.32 i6  i14 28 || LSU1.LDIH I1, 0xffff
lsu0.ldo.32 i7  i14 40 || cmu.cpivr.x32 v17 i1
lsu0.ldo.32 i8  i14  8 || cmu.cpivr.x32 v1 i17 //kp_y
lsu0.ldo.32 i9  i14 20 || cmu.cpivr.x32 v2 i16 //integral
lsu0.ldo.32 i10 i14 32 || cmu.cpivr.x32 v3 i15 //stride
lsu0.ldo.32 i11 i14 44|| cmu.cpiv.x32 v8.0 i4 || iau.add i14 i14 48 //s
                         cmu.cpiv.x32 v8.1 i5 || lsu1.cp v7.2 i2
                         cmu.cpiv.x32 v8.2 i6 || lsu1.cp v7.3 i3
                         cmu.cpiv.x32 v8.3 i7 || lsu1.cp v7.1 i1
                         cmu.cpiv.x32 v9.0 i8 || lsu1.cp v7.0 i0

vau.add.f32 v10 v7 v0     || lsu0.ldo.32 i0  i14  0 || cmu.cpiv.x32 v9.1 i9
vau.add.f32 v11 v8 v1     || lsu0.ldo.32 i1  i14 12 || cmu.cpiv.x32 v9.2 i10
                             lsu0.ldo.32 i2  i14 24 || cmu.cpiv.x32 v9.3 i11
vau.sub.f32 v12 v10  v9   || lsu0.ldo.32 i3  i14 36
vau.sub.f32 v13 v11  v9   || lsu0.ldo.32 i4  i14  4
vau.add.f32 v15 v10  v9   || lsu0.ldo.32 i5  i14 16
vau.add.f32 v16 v11  v9   || lsu0.ldo.32 i6  i14 28
vau.add.f32 v12 v12 +0.5  || lsu0.ldo.32 i7  i14 40|| cmu.cpiv.x32 v7.0 i0
vau.add.f32 v13 v13 +0.5  || lsu0.ldo.32 i8  i14  8|| cmu.cpiv.x32 v7.1 i1
vau.add.f32 v15 v15  v14  || lsu0.ldo.32 i9  i14 20|| cmu.cpiv.x32 v7.2 i2
vau.add.f32 v16 v16  v14  || lsu0.ldo.32 i10 i14 32|| cmu.cpiv.x32 v7.3 i3
CMU.CPVV.F32.I32S v12 v12 || lsu0.ldo.32 i11 i14 44|| lsu1.cp v8.0 i4 || iau.add i14 i14 48 //s
CMU.CPVV.F32.I32S v13 v13                          || lsu0.cp v8.1 i5
CMU.CPVV.F32.I32S v15 v15                          || lsu0.cp v8.2 i6
CMU.CPVV.F32.I32S v16 v16                          || lsu0.cp v8.3 i7
vau.add.i32 v12 v12 v17                            || lsu0.cp v9.0 i8
vau.add.i32 v13 v13 v17                            || lsu0.cp v9.1 i9
vau.shl.x32 v18 v12 2                              || lsu0.cp v9.2 i10
vau.add.i32 v15 v15 v17                            || lsu0.cp v9.3 i11
vau.add.i32 v16 v16 v17 || IAU.SUB i19 i19 8
vau.shl.x32 v19 v15 2   || LSU0.ST.64  i20  i19 || IAU.SUB i19 i19 8
vau.sub.i32 v5 v16 v13  || LSU1.ST.64  i22  i19 || IAU.SUB i19 i19 8
vau.sub.i32 v4 v15 v12  || LSU0.ST.64  i24  i19 || IAU.SUB i19 i19 8
vau.mul.i32 v20 v13 v3  || LSU1.ST.64  i26  i19 || IAU.SUB i19 i19 8
vau.mul.i32 v21 v16 v3  || LSU1.ST.64  i28  i19
vau.mul.i32 v6 v4 v5    || iau.shr.u32 i28  i13 2
nop
vau.add.i32 v22  v21 v19
vau.add.i32 v23  v21 v18 || cmu.cpvi.x32 i20 v6.0
vau.add.i32 v21  v20 v18 || cmu.cpvi.x32 i21 v6.1
vau.add.i32 v20  v20 v19 || cmu.cpvi.x32 i22 v6.2
vau.add.i32 v22 v22 v2   || cmu.cpvi.x32 i23 v6.3
vau.add.i32 v23 v23 v2   || LSU0.LDIL i27, mvcvSmoothed_intensity_loop
vau.add.i32 v21 v21 v2   || LSU1.LDIH I27, mvcvSmoothed_intensity_loop
vau.add.i32 v20 v20 v2

cmu.cpvi.x32 i0  v22.0                          || vau.add.f32 v10 v7 v0
cmu.cpvi.x32 i1 v23.0  || lsu0.ld.32 i0  i0     || vau.add.f32 v11 v8 v1
cmu.cpvi.x32 i2 v22.1  || lsu0.ld.32 i1  i1
cmu.cpvi.x32 i3 v23.1  || lsu0.ld.32 i2  i2     || vau.sub.f32 v12 v10  v9
cmu.cpvi.x32 i4 v22.2  || lsu0.ld.32 i3  i3     || vau.sub.f32 v13 v11  v9
cmu.cpvi.x32 i5 v23.2  || lsu0.ld.32 i4  i4     || vau.add.f32 v15 v10  v9
cmu.cpvi.x32 i6 v22.3  || lsu0.ld.32 i5  i5     || vau.add.f32 v16 v11  v9
cmu.cpvi.x32 i7 v23.3  || lsu0.ld.32 i6  i6     || vau.add.f32 v12 v12 +0.5
cmu.cpvi.x32 i8 v21.0  || lsu0.ld.32 i7  i7     || vau.add.f32 v13 v13 +0.5
cmu.cpvi.x32 i9 v20.0  || lsu0.ld.32 i8  i8     || vau.add.f32 v15 v15  v14
lsu1.cp      i10 v21.1 || lsu0.ld.32 i9  i9     || vau.add.f32 v16 v16  v14  || CMU.CPVV.F32.I32S v12 v12
lsu1.cp      i11 v20.1 || lsu0.ld.32 i10 i10    || CMU.CPVV.F32.I32S v13 v13
lsu1.cp      i26 v21.2 || lsu0.ld.32 i11 i11    || CMU.CPVV.F32.I32S v15 v15 || vau.add.i32 v12 v12 v17
lsu1.cp      i25 v20.2 || lsu0.ld.32 i26 i26    || CMU.CPVV.F32.I32S v16 v16 || vau.add.i32 v13 v13 v17
lsu1.cp      i17 v21.3 || lsu0.ld.32 i25 i25    || vau.shl.x32 v18 v12 2
cmu.cpvi.x32 i24 v20.3 || lsu0.ld.32 i17 i17    || vau.add.i32 v15 v15 v17
                          lsu0.ld.32 i24 i24    || vau.add.i32 v16 v16 v17
                                                   vau.shl.x32 v19 v15 2      || lsu0.ldo.32 i0  i14  0
                                                   vau.sub.i32 v5 v16 v13     || lsu0.ldo.32 i2  i14 24
                                                   vau.sub.i32 v4 v15 v12     || lsu0.ldo.32 i4  i14  4
iau.sub i1 i0  i1        || vau.mul.i32 v20 v13 v3 || lsu0.ldo.32 i8  i14  8
iau.sub i3 i2  i3        || vau.mul.i32 v21 v16 v3
iau.sub i5 i4  i5        || vau.mul.i32 v6 v4 v5    || lsu1.ldo.32 i10 i14 32
iau.sub i7 i6  i7        || lsu0.ldo.32 i6  i14 28
iau.sub i9 i8  i9        || vau.add.i32 v22  v21 v19 || lsu0.ldo.32 i1  i14 12
iau.sub i11 i10 i11      || vau.add.i32 v23  v21 v18 || lsu0.ldo.32 i3  i14 36
iau.sub i25 i26 i25      || vau.add.i32 v21  v20 v18 || lsu0.ldo.32 i5  i14 16 || lsu1.cp v8.0 i4  //s
iau.sub i24 i17 i24      || vau.add.i32 v20  v20 v19 || lsu0.ldo.32 i7  i14 40|| cmu.cpiv.x32 v7.0 i0
iau.add i9  i1 i9       || vau.add.i32 v22 v22 v2   || lsu0.ldo.32 i9  i14 20  || lsu1.ldo.32 i11 i14 44
iau.add i11 i3 i11      || vau.add.i32 v23 v23 v2   || lsu1.cp v7.2 i2 || cmu.cpiv.x32 v9.0 i8
iau.add i25 i5 i25      || vau.add.i32 v21 v21 v2 ||  lsu0.cp v8.2 i6
iau.add i24 i7 i24      || vau.add.i32 v20 v20 v2 || cmu.cpiv.x32 v7.1 i1
sau.div.i32 i16 i9  i20  || cmu.cpiv.x32 v7.3 i3  || lsu0.cp v9.2 i10 || lsu1.cp i20 v6.0
sau.div.i32 i15 i11 i21  || iau.add i14 i14 48    || lsu0.cp v8.1 i5  || lsu1.cp i21 v6.1
sau.div.i32 i18 i25 i22  || lsu0.cp v8.3 i7       || cmu.cpvi.x32 i22 v6.2
sau.div.i32 i13 i24 i23  || lsu0.cp v9.1 i9       || lsu1.cp v9.3 i11  || cmu.cpvi.x32 i23 v6.3

//.align 16
loop_start:
cmu.cpvi.x32 i0  v22.0                          || vau.add.f32 v10 v7 v0 ||     bru.rpl i27 i28//x
cmu.cpvi.x32 i1 v23.0  || lsu0.ld.32 i0  i0     || vau.add.f32 v11 v8 v1
cmu.cpvi.x32 i2 v22.1  || lsu0.ld.32 i1  i1
cmu.cpvi.x32 i3 v23.1  || lsu0.ld.32 i2  i2     || vau.sub.f32 v12 v10  v9
cmu.cpvi.x32 i4 v22.2  || lsu0.ld.32 i3  i3     || vau.sub.f32 v13 v11  v9
cmu.cpvi.x32 i5 v23.2  || lsu0.ld.32 i4  i4     || vau.add.f32 v15 v10  v9
cmu.cpvi.x32 i6 v22.3  || lsu0.ld.32 i5  i5     || vau.add.f32 v16 v11  v9
cmu.cpvi.x32 i7 v23.3  || lsu0.ld.32 i6  i6     || vau.add.f32 v12 v12 +0.5
cmu.cpvi.x32 i8 v21.0  || lsu0.ld.32 i7  i7     || vau.add.f32 v13 v13 +0.5
cmu.cpvi.x32 i9 v20.0  || lsu0.ld.32 i8  i8     || vau.add.f32 v15 v15  v14
lsu1.cp      i10 v21.1 || lsu0.ld.32 i9  i9     || vau.add.f32 v16 v16  v14  || CMU.CPVV.F32.I32S v12 v12
lsu1.cp      i11 v20.1 || lsu0.ld.32 i10 i10    || CMU.CPVV.F32.I32S v13 v13
lsu1.cp      i26 v21.2 || lsu0.ld.32 i11 i11    || CMU.CPVV.F32.I32S v15 v15 || vau.add.i32 v12 v12 v17
lsu1.cp      i25 v20.2 || lsu0.ld.32 i26 i26    || CMU.CPVV.F32.I32S v16 v16 || vau.add.i32 v13 v13 v17
lsu1.cp      i17 v21.3 || lsu0.ld.32 i25 i25    || vau.shl.x32 v18 v12 2
cmu.cpvi.x32 i24 v20.3 || lsu0.ld.32 i17 i17    || vau.add.i32 v15 v15 v17
                          lsu0.ld.32 i24 i24    || vau.add.i32 v16 v16 v17
                                                   vau.shl.x32 v19 v15 2      || lsu0.ldo.32 i0  i14  0
                                                   vau.sub.i32 v5 v16 v13     || lsu1.ldo.32 i2  i14 24    || lsu0.sto.16 i16 i12 0
                                                   vau.sub.i32 v4 v15 v12     || lsu1.ldo.32 i4  i14  4    || lsu0.sto.16 i15 i12 2
iau.sub i1 i0  i1        || vau.mul.i32 v20 v13 v3 || lsu0.ldo.32 i8  i14  8                               || lsu1.sto.16 i18 i12 4
iau.sub i3 i2  i3        || vau.mul.i32 v21 v16 v3                                                         || lsu1.sto.16 i13 i12 6
iau.sub i5 i4  i5        || vau.mul.i32 v6 v4 v5      || lsu1.ldo.32 i10 i14 32
iau.sub i7 i6  i7        || lsu0.ldo.32 i6  i14 28
iau.sub i9 i8  i9        || vau.add.i32 v22  v21 v19 || lsu0.ldo.32 i1  i14 12
iau.sub i11 i10 i11      || vau.add.i32 v23  v21 v18 || lsu0.ldo.32 i3  i14 36
iau.sub i25 i26 i25      || vau.add.i32 v21  v20 v18 || lsu0.ldo.32 i5  i14 16 || lsu1.cp v8.0 i4  //s
iau.sub i24 i17 i24      || vau.add.i32 v20  v20 v19 || lsu0.ldo.32 i7  i14 40|| cmu.cpiv.x32 v7.0 i0
iau.add i9  i1 i9       || vau.add.i32 v22 v22 v2   || lsu0.ldo.32 i9  i14 20  || lsu1.ldo.32 i11 i14 44
mvcvSmoothed_intensity_loop:
iau.add i11 i3 i11      || vau.add.i32 v23 v23 v2   || lsu1.cp v7.2 i2 || cmu.cpiv.x32 v9.0 i8
iau.add i25 i5 i25      || vau.add.i32 v21 v21 v2 ||  lsu0.cp v8.2 i6
iau.add i24 i7 i24      || vau.add.i32 v20 v20 v2 || cmu.cpiv.x32 v7.1 i1
sau.div.i32 i16 i9  i20  || cmu.cpiv.x32 v7.3 i3  || lsu0.cp v9.2 i10 || lsu1.cp i20 v6.0
sau.div.i32 i15 i11 i21  || iau.add i14 i14 48   || lsu0.cp v8.1 i5    || lsu1.cp i21 v6.1
sau.div.i32 i18 i25 i22  || lsu0.cp v8.3 i7                   || cmu.cpvi.x32 i22 v6.2 || IAU.ADD I12, I12, 0x8
sau.div.i32 i13 i24 i23  || lsu0.cp v9.1 i9 || lsu1.cp v9.3 i11  || cmu.cpvi.x32 i23 v6.3


LSU0.LD.64  i28  i19 || IAU.ADD i19 i19 8
LSU0.LD.64  i26  i19 || IAU.ADD i19 i19 8
LSU0.LD.64  i24  i19 || IAU.ADD i19 i19 8
LSU0.LD.64  i22  i19 || IAU.ADD i19 i19 8
LSU0.LD.64  i20  i19 || IAU.ADD i19 i19 8 || BRU.JMP I30

NOP 6
.nowarnend
.size mvcvSmoothed_intensity_asm,.-mvcvSmoothed_intensity_asm
.end
