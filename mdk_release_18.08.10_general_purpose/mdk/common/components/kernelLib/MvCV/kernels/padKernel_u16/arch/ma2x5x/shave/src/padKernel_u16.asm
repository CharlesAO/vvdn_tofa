.type mvcvPadKernel_u16_asm,@function
.version 0.50.00

.code .text.nonMax3x3_u8
.align 16

// padKernel, adds pad to a line with pixels at left and/or right side

//void mvcvpadKernel_u16(
//u16* iBuf      - i18
//u32 iBufLen   - i17
//u16* oBuf      - i16
//u32 padSz     - i15
//u32 padMode   - i14
//u32 padType   - i13
//u16 pixValue   - i12
//)

.lalign
mvcvPadKernel_u16_asm:
.nowarn
IAU.SUB i19 i19 4
LSU0.ST.32  i20  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i24  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i25  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i26  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i27  i19 || IAU.SUB i19 i19 4
LSU0.ST.32  i28  i19 
	
iau.sub i0 i0 i0
lsu0.ldil i1 1
lsu0.ldil i2 2
lsu0.ldil i3 3
lsu0.ldil i4 4
lsu0.ldil i21 5
lsu0.ldil i5 0xffff
iau.shr.u32 i17 i17 3
lsu0.ldil i10 padKernel_Left_Mirror             || lsu1.ldih i10 padKernel_Left_Mirror
lsu0.ldil i11 padKernel_Right_Mirror            || lsu1.ldih i11 padKernel_Right_Mirror
lsu0.ldil i28 padKernel_LeftAndRightMirror_loop || lsu1.ldih i28 padKernel_LeftAndRightMirror_loop
lsu0.ldil i20 padKernel_copy                    || lsu1.ldih i20 padKernel_copy
lsu0.ldil i22 padKernel_LeftAndRight_Mirror     || lsu1.ldih i22 padKernel_LeftAndRight_Mirror
lsu0.ldil i23 padKernel_LeftMirror_loop         || lsu1.ldih i23 padKernel_LeftMirror_loop
lsu0.ldil i24 padKernel_RightMirror_loop        || lsu1.ldih i24 padKernel_RightMirror_loop
lsu0.ldil i25 padKernel_LeftRightMirror1_loop   || lsu1.ldih i25 padKernel_LeftRightMirror1_loop
lsu0.ldil i26 padKernel_LeftRightMirror2_loop   || lsu1.ldih i26 padKernel_LeftRightMirror2_loop
lsu0.ldil i27 padKernel_end                     || lsu1.ldih i27 padKernel_end

cmu.cmii.u32 i14 i0 || lsu0.ldil i7 padKernel_Left_loop || lsu1.ldih i7 padKernel_Left_loop
peu.pc1c eq || bru.bra padKernel_Left_loop_head
nop 6

cmu.cmii.u32 i14 i1 || lsu0.ldil i7 padKernel_Right_loop || lsu1.ldih i7 padKernel_Right_loop
peu.pc1c eq || bru.bra padKernel_Right_loop_head
nop 6

cmu.cmii.u32 i14 i2 || lsu0.ldil i7 padKernel_LeftAndRight_loop || lsu1.ldih i7 padKernel_LeftAndRight_loop
peu.pc1c eq || bru.bra padKernel_LeftAndRight_loop_head
nop 6


 padKernel_Left_loop_head:
cmu.cmii.u8 i13 i0 //AllZero
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i1 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i2 //Mirror
peu.pc1c eq || bru.bra padKernel_Left_Mirror
nop 6
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i5 i16 || iau.incs i16 2
nop
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i12 i16 || iau.incs i16 2
 padKernel_copy:
lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i7 i17
padKernel_Left_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16
bru.jmp i27
nop 6

 padKernel_Left_Mirror:
iau.add i18 i18 i15
iau.add i18 i18 i15
iau.sub i18 i18 2
lsu0.ld.16 i6 i18 || bru.rpl i23 i15
padKernel_LeftMirror_loop:
nop 6
lsu0.sti.16 i6 i16 || iau.sub i18 i18 2
iau.add i18 i18 2
bru.bra padKernel_copy
nop 6
bru.jmp i27
nop 6


 padKernel_Right_loop_head:
lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i7 i17
padKernel_Right_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16
cmu.cmii.u8 i13 i0 //AllZero
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i1 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i2 //Mirror
peu.pc1c eq || bru.bra padKernel_Right_Mirror
nop 6
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i5 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i12 i16 || iau.incs i16 2
bru.jmp i27
nop 6

 padKernel_Right_Mirror:
iau.sub i18 i18 2
lsu0.ld.16 i6 i18 || iau.sub i18 i18 2 || bru.rpl i24 i15
padKernel_RightMirror_loop:
nop 6
lsu0.sti.16 i6 i16 || iau.add i16 i16 2
bru.jmp i27
nop 6


 padKernel_LeftAndRight_loop_head:
cmu.cmii.u8 i13 i0 //AllZero
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i1 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i2 //Mirror
peu.pc1c eq || bru.bra padKernel_LeftAndRight_Mirror
nop 6
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i5 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i12 i16 || iau.incs i16 2

lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i7 i17
padKernel_LeftAndRight_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16

cmu.cmii.u8 i13 i0 //AllZero
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i1 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i0 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i5 i16 || iau.incs i16 2
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.16 i12 i16 || iau.incs i16 2
bru.jmp i27
nop 6

 padKernel_LeftAndRight_Mirror:
iau.add i18 i18 i15
iau.add i18 i18 i15
iau.sub i18 i18 2
lsu0.ld.16 i6 i18 || bru.rpl i25 i15
padKernel_LeftRightMirror1_loop:
nop 6
lsu0.sti.16 i6 i16 || iau.sub i18 i18 2
iau.add i18 i18 2
lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i28 i17
padKernel_LeftAndRightMirror_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16
iau.sub i18 i18 2
lsu0.ld.16 i6 i18 || iau.sub i18 i18 2 || bru.rpl i26 i15
padKernel_LeftRightMirror2_loop:
nop 6
lsu0.sti.16 i6 i16 || iau.add i16 i16 2

padKernel_end:
LSU0.LD.32  i28  i19 || IAU.ADD i19 i19 4	
LSU0.LD.32  i27  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i26  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i25  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i24  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19 || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19 || IAU.ADD i19 i19 4
	.nowarnend	
bru.jmp i30
nop 6
.size mvcvPadKernel_u16_asm,.-mvcvPadKernel_u16_asm
.end
