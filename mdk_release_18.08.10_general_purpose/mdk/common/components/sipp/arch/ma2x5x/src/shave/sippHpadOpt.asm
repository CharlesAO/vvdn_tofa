.version 00.51.05

.include "myriad2SippDefs.inc"

.code .text.sippHpadOpt
.align 16

//##########################################################
// C header: "void sippHorizontalPadding(SippPipeline *pl)"
// i18 = pl reference
//##########################################################
.lalign
sippHorizontalPadding:

cmu.cpii    i0  i18
lsu0.ldo.32 i1  i0 P_O_N_PAD_FILTS
cmu.cpti    i2  P_SVID
lsu0.ldo.64 i3  i0 P_O_GLOBAL_NFO
iau.add     i0  i0 P_O_PAD_LIST
lsu0.ldi.64 i5  i0
lsu0.ldi.64 i7  i0

cmu.cpiv v0.0 i20
iau.or i1 i1 i1    || lsu0.cp v3.0 i19
peu.pc1i GT        || bru.bra __padNextFilter
cmu.cpiv v0.1 i21  || lsu0.cp v1.2 i26
cmu.cpiv v0.2 i22  || lsu0.cp v1.3 i27
cmu.cpiv v0.3 i23  || lsu0.cp v2.0 i28
cmu.cpiv v1.0 i24  || lsu0.cp v2.1 i29
cmu.cpiv v1.1 i25  || lsu0.cp v2.2 i30
lsu1.ldi.32 i30 i0 || lsu0.cp v2.3 i31


//========================================
.lalign
__sippHpadDone:
//========================================
lsu0.cp i30 v2.2
bru.jmp i30
cmu.cpvi i20 v0.0 || lsu0.cp i26 v1.2
cmu.cpvi i21 v0.1 || lsu0.cp i27 v1.3
cmu.cpvi i22 v0.2 || lsu0.cp i28 v2.0
cmu.cpvi i23 v0.3 || lsu0.cp i29 v2.1
cmu.cpvi i24 v1.0 || lsu0.cp i19 v3.0
cmu.cpvi i25 v1.1 || lsu0.cp i31 v2.3
nop 6

//#####################################################################
.lalign
__padNextFilter:
//#####################################################################
lsu0.ldo.32 i27 i5 F_O_FIRST_O_SLC  || lsu1.ldil   i20 0x80
lsu0.ldo.32 i24 i5 F_O_LNS_PER_ITER || lsu1.ldil   i19 1      || iau.mul i8  i8  i2
iau.mul i30 i30 i20                 || bru.jmp i6
lsu0.ldil   i9  F_O_LN_TO_PAD       || lsu1.cp     i28 i3
lsu0.ldo.32 i10 i5 F_O_EXENO        || iau.add     i9  i9 i5
lsu0.ldil   i26 4                   || iau.add     i25 i8 i7
iau.sub i28 i28 i19                 || sau.add.i32 i9 i9 i30
iau.sub i29 i27 i3
peu.pc1i EQ                         || cmu.cpii    i28 i4


//#######################################################################
.lalign
__padOpt__Pl_1__Pad_8__Bpp_1:
//#######################################################################
lsu0.ldil   i11 __iters_loop_case1               || lsu1.ldih i11 __iters_loop_case1
lsu0.ld.32  i13 i9    || lsu1.ldo.32 i14 i9 0x40
lsu0.ld.64  i15 i25   || iau.add i9 i9 i26
lsu1.ldo.64 i17 i25 8
nop 3

iau.and i10 i10 i19   || bru.rpl i11 i24
peu.pc1i NEQ          || cmu.cpii    i13 i14
iau.add i15 i13 i15   || sau.add.i32 i16 i13 i16
lsu0.ld.64  i20 i15
lsu0.ld.64  i22 i16
lsu0.ld.64  i15 i25
__iters_loop_case1:
lsu0.ldo.64 i17 i25 8
iau.add i17 i13 i17   || lsu0.ld.32 i13 i9    || lsu1.ldo.32 i14 i9 0x40
iau.add i18 i13 i18
iau.sub i29 i27 i2
iau.sub i29 i28 i2    || peu.pcix.EQ 0x06     || cmu.vszm.byte i20 i20 [0000]  || sau.swz i21 i20 [0000]
lsu0.st.64  i20 i17   || peu.pcix.EQ 0x06     || cmu.vszm.byte i22 i22 [0000]  || sau.swz i23 i22 [0000]
lsu0.st.64  i22 i18   || iau.add i9 i9 i26

iau.incs i1 -1
peu.pc1i NEQ          || lsu0.ldi.64 i5  i0
peu.pc1i NEQ          || lsu0.ldi.64 i7  i0
peu.pc1i NEQ          || lsu0.ldi.32 i30 i0   || bru.bra __padNextFilter
peu.pc1i EQ           || bru.bra __sippHpadDone
nop 6


//######################################################################################
 .lalign
 __padOpt__Pl_1__Pad_8__Bpp_2:
//######################################################################################
lsu0.ldil   i11 __iters_loop_case2               || lsu1.ldih i11 __iters_loop_case2
lsu0.ld.32  i13 i9    || lsu1.ldo.32 i14 i9 0x40
lsu0.ld.64  i15 i25   || iau.add i9 i9 i26
lsu1.ldo.64 i17 i25 8
nop 3

iau.and i10 i10 i19   || bru.rpl i11 i24
peu.pc1i NEQ          || cmu.cpii    i13 i14
iau.add i15 i13 i15   || sau.add.i32 i16 i13 i16
lsu0.ld.64  i20 i15
lsu0.ld.64  i22 i16
lsu0.ld.64  i15 i25
__iters_loop_case2:
 .nowarn 10
lsu0.ldo.64 i17 i25 8
 .nowarnend
iau.add i17 i13 i17   || lsu0.ld.32 i13 i9      || lsu1.ldo.32 i14 i9 0x40
iau.add i18 i13 i18
iau.sub i29 i27 i2
iau.sub i29 i28 i2    || peu.pcix.EQ 0x06       || cmu.vszm.byte i20 i20 [1010]  || sau.swz i21 i20 [1010]
lsu0.st.64  i20 i17   || peu.pcix.EQ 0x06       || cmu.vszm.byte i22 i22 [1010]  || sau.swz i23 i22 [1010]
lsu0.st.64  i22 i18   || iau.add i9 i9 i26

iau.incs i1 -1
peu.pc1i NEQ          || lsu0.ldi.64 i5  i0
peu.pc1i NEQ          || lsu0.ldi.64 i7  i0
peu.pc1i NEQ          || lsu0.ldi.32 i30 i0     || bru.bra __padNextFilter
peu.pc1i EQ           || bru.bra __sippHpadDone
nop 6


//#############################################################################################################
 .lalign
 __padOpt__Pl_1__Pad_16__Bpp_1:
//#############################################################################################################
lsu0.ldil   i11 __iters_loop_case3                      || lsu1.ldih   i11 __iters_loop_case3
lsu0.ld.32  i13 i9      || lsu1.ldo.32 i14 i9 0x40
lsu0.ld.64  i15 i25     || iau.add i9 i9 i26
lsu0.ldo.64 i17 i25 8
nop 3
iau.and i10 i10 i19
peu.pc1i NEQ            || cmu.cpii i13 i14

iau.add i15 i13 i15     || bru.rpl i11 i24
iau.add i16 i13 i16     || lsu0.ldo.64.l v10 i15 0      || lsu1.ldo.64.h v10 i15 8       || sau.add.i32 i17 i13 i17
lsu0.ldo.64.l v11 i16 0 || lsu1.ldo.64.h v11 i16 8      || sau.add.i32 i18 i13 i18
lsu0.ld.32  i13 i9      || lsu1.ldo.32 i14 i9 0x40
lsu0.ld.64  i15 i25     || iau.add i9 i9 i26
__iters_loop_case3:
nop
lsu0.ldo.64 i17 i25 8
iau.sub i29 i27 i2
iau.sub i29 i28 i2      || peu.pcix.EQ 0x14             || cmu.vszm.word v10 v10 [0000]  || lsu0.swzmc4.BYTE [0000] [UUUU]
peu.pcix.EQ 0x14        || cmu.vszm.word v11 v11 [0000] || lsu0.swzmc4.BYTE [0000] [UUUU]
lsu0.sto.64.l v10 i17 0 || lsu1.sto.64.h v10 i17 8      || iau.and i10 i10 i19
lsu0.sto.64.l v11 i18 0 || lsu1.sto.64.h v11 i18 8      || peu.pcix.NEQ 0x4              || cmu.cpii i13 i14

iau.incs i1 -1
peu.pc1i NEQ            || lsu0.ldi.64 i5  i0
peu.pc1i NEQ            || lsu0.ldi.64 i7  i0
peu.pc1i NEQ            || lsu0.ldi.32 i30 i0           || bru.bra __padNextFilter
peu.pc1i EQ             || bru.bra __sippHpadDone
nop 6


//######################################################################################
 .lalign
 __padOpt__Pl_1__Pad_16__Bpp_2:
//######################################################################################
lsu0.ldil   i11 __iters_loop_case4                      || lsu1.ldih   i11 __iters_loop_case4
lsu0.ld.32  i13 i9      || lsu1.ldo.32 i14 i9 0x40
lsu0.ld.64  i15 i25     || iau.add i9 i9 i26
lsu0.ldo.64 i17 i25 8
nop 3
iau.and i10 i10 i19
peu.pc1i NEQ            || cmu.cpii i13 i14

iau.add i15 i13 i15     || bru.rpl i11 i24
iau.add i16 i13 i16     || lsu0.ldo.64.l v10 i15 0      || lsu1.ldo.64.h v10 i15 8       || sau.add.i32 i17 i13 i17
lsu0.ldo.64.l v11 i16 0 || lsu1.ldo.64.h v11 i16 8      || sau.add.i32 i18 i13 i18
lsu0.ld.32  i13 i9      || lsu1.ldo.32 i14 i9 0x40
lsu0.ld.64  i15 i25     || iau.add i9 i9 i26
__iters_loop_case4:
nop
lsu0.ldo.64 i17 i25 8
iau.sub i29 i27 i2
iau.sub i29 i28 i2      || peu.pcix.EQ 0x14             || cmu.vszm.word v10 v10 [0000]  || lsu0.swzmc4.BYTE [1010] [UUUU]
peu.pcix.EQ 0x14        || cmu.vszm.word v11 v11 [0000] || lsu0.swzmc4.BYTE [1010] [UUUU]
lsu0.sto.64.l v10 i17 0 || lsu1.sto.64.h v10 i17 8      || iau.and i10 i10 i19
lsu0.sto.64.l v11 i18 0 || lsu1.sto.64.h v11 i18 8      || peu.pcix.NEQ 0x4              || cmu.cpii i13 i14

iau.incs i1 -1
peu.pc1i NEQ            || lsu0.ldi.64 i5  i0
peu.pc1i NEQ            || lsu0.ldi.64 i7  i0
peu.pc1i NEQ            || lsu0.ldi.32 i30 i0           || bru.bra __padNextFilter
peu.pc1i EQ             || bru.bra __sippHpadDone
nop 6


//#############################################################################################################
 .lalign
 __padOpt__Pl_3__Pad_8__Bpp_1:
//#############################################################################################################
lsu0.ldil   i11 __iters_loop_case5                       || lsu1.ldih   i11 __iters_loop_case5
lsu0.ld.32  i13 i9       || lsu1.ldo.32 i14 i9 0x40
lsu0.ld.64  i15 i25      || iau.add i9 i9 i26
lsu0.ldo.64 i17 i25 0x8
nop 3
iau.and i10 i10 i19
peu.pc1i NEQ             || cmu.cpii i13 i14
lsu0.ldo.64 i15 i25 0x10 || bru.rpl i11 i24
lsu0.ldo.64 i17 i25 0x18
lsu0.ldo.64 i15 i25 0x20
lsu0.ldo.64 i17 i25 0x28
nop
iau.add i20 i13 i15      || lsu0.ld.32   i13 i9          || lsu1.ldo.32 i14 i9 0x40
iau.add i21 i13 i16      || lsu0.ld.64.l v10 i20         || sau.add.i32 i20 i13 i17        || lsu1.ld.64  i15 i25
iau.add i22 i13 i15      || lsu0.ld.64.l v11 i21         || sau.add.i32 i21 i13 i18        || lsu1.ldo.64 i17 i25 0x8
iau.add i23 i13 i16      || lsu0.ld.64.l v12 i22         || sau.add.i32 i22 i13 i17
iau.add i12 i13 i15      || lsu0.ld.64.l v13 i23         || sau.add.i32 i23 i13 i18
iau.add i31 i13 i16      || lsu0.ld.64.l v14 i12         || sau.add.i32 i12 i13 i17
lsu0.ld.64.l v15 i31     || sau.add.i32 i31 i13 i18
iau.sub i29 i27 i2       || sau.add.i32 i9 i9 i26
iau.sub i29 i28 i2       || peu.pcix.EQ  0x14            || cmu.vszm.word v10 v10 [0000]   || lsu0.swzmc4.BYTE [0000] [UUUU]
__iters_loop_case5:
 .nowarn 10
peu.pcix.EQ  0x14        || cmu.vszm.word v11 v11 [0000] || lsu0.swzmc4.BYTE [0000] [UUUU] || lsu1.st.64.l v10 i20
 .nowarnend
iau.sub i29 i27 i2
iau.sub i29 i28 i2       || peu.pcix.EQ  0x14            || cmu.vszm.word v12 v12 [0000]   || lsu0.swzmc4.BYTE [0000] [UUUU] || lsu1.st.64.l v11 i21
iau.sub i29 i27 i2       || peu.pcix.EQ  0x14            || cmu.vszm.word v13 v13 [0000]   || lsu0.swzmc4.BYTE [0000] [UUUU] || lsu1.st.64.l v12 i22
iau.sub i29 i28 i2       || peu.pcix.EQ  0x14            || cmu.vszm.word v14 v14 [0000]   || lsu0.swzmc4.BYTE [0000] [UUUU] || lsu1.st.64.l v13 i23
peu.pcix.EQ 0x14         || cmu.vszm.word v15 v15 [0000] || lsu0.swzmc4.BYTE [0000] [UUUU] || lsu1.st.64.l v14 i12           || iau.and i10 i10 i19
peu.pcix.NEQ 0x4         || cmu.cpii i13 i14             || lsu0.st.64.l v15 i31

iau.incs i1 -1
peu.pc1i NEQ             || lsu0.ldi.64 i5 i0
peu.pc1i NEQ             || lsu0.ldi.64 i7 i0
peu.pc1i NEQ             || lsu0.ldi.32 i30 i0           || bru.bra __padNextFilter
peu.pc1i EQ              || bru.bra __sippHpadDone
nop 6


 //#############################################################################################################
 // Arbitrary BPP, hPadding and number of planes
 .lalign
 __padGeneral:
 //#############################################################################################################
lsu0.ldo.32 i12 i5 F_O_BYTESPP
lsu0.ldo.32 i11 i5 F_O_H_PADDING
lsu0.ldil   i31 __iters_loop_caseGen             || lsu1.ldih   i31 __iters_loop_caseGen
nop 5
iau.shl i11 i11 i12

lsu0.ld.32  i13 i9    || lsu1.ldo.32 i14 i9 0x40 || bru.rpl i31 i24
iau.add i25 i8 i7
lsu0.ldi.64 i15 i25
lsu0.ldi.64 i17 i25
iau.add i9 i9 i26
lsu0.ldo.32 i23 i5 4
iau.and i10 i10 i19
peu.pc1i NEQ          || cmu.cpii i13 i14
nop

__plane_loop:
iau.add i15 i13 i15   || sau.shr.u32 i20 i11 3
iau.sub i29 i27 i2    || sau.shr.u32 i21 i11 3  || cmu.cmii.u32 i12 i19 //BPP comparison with 0,1,2 for 1,2,4 BPP
                                                                        //so comparing with middle value 1 (i.e. i19)
//Left Padding / Replication for aribtrary BPP and hPadding
peu.pcc0i.AND LT EQ   || lsu0.ld.8R  v10 i15    || lsu1.ldil i20 0
peu.pcc0i.AND EQ EQ   || lsu0.ld.16R v10 i15    || lsu1.ldil i20 0
peu.pcc0i.AND GT EQ   || lsu0.ld.32R v10 i15    || lsu1.ldil i20 0
//Above code prepares replication// below code prepares loads (if needed) and the mandatory STORES
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i15  || iau.subsu i20 i20 1  || sau.shr.u32 i22 i11 3
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i15  || iau.subsu i20 i20 1  || sau.add.i32 i16 i13 i16
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i15  || iau.subsu i20 i20 1  || sau.add.i32 i17 i13 i17
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i15  || iau.subsu i20 i20 1  || sau.add.i32 i18 i13 i18
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i15  || iau.subsu i20 i20 1
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i15  || iau.subsu i20 i20 1
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i15
lsu0.ldi.64.l v10 i15 || lsu1.sti.64.l v10 i17  || bru.rpi i22
//Right Padding / Replication for arbitrary BPP and hPadding
iau.sub i29 i28 i2
peu.pcc0i.AND LT EQ   || lsu0.ld.8R  v10 i16    || lsu1.ldil i21 0
peu.pcc0i.AND EQ EQ   || lsu0.ld.16R v10 i16    || lsu1.ldil i21 0
peu.pcc0i.AND GT EQ   || lsu0.ld.32R v10 i16    || lsu1.ldil i21 0
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i16  || iau.subsu i21 i21 1  || sau.sub.i32 i23 i23 1
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i16  || iau.subsu i21 i21 1
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i16  || iau.subsu i21 i21 1  || cmu.cmz.i32 i23
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i16  || iau.subsu i21 i21 1
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i16  || iau.subsu i21 i21 1
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i16  || iau.subsu i21 i21 1
peu.pcix.NEQ 0x10     || lsu0.ldi.64.l v10 i16
lsu0.ldi.64.l v10 i16 || lsu1.sti.64.l v10 i18  || bru.rpi i22
//go to : (1)Next Plane or (2)Next filter or (3)End of hPAdding
peu.pc1c GT           || lsu0.ldi.64 i15 i25    || bru.bra __plane_loop
peu.pc1c GT           || lsu0.ldi.64 i17 i25
nop 5
__iters_loop_caseGen:
nop 7

peu.pc1c EQ           || iau.incs i1 -1
peu.pcc0i.and EQ GT   || lsu0.ldi.64 i5 i0
peu.pcc0i.and EQ GT   || lsu0.ldi.64 i7 i0
peu.pcc0i.and EQ GT   || lsu0.ldi.32 i30 i0     || bru.bra __padNextFilter
peu.pcc0i.and EQ EQ   || bru.bra __sippHpadDone
nop 6

.end
