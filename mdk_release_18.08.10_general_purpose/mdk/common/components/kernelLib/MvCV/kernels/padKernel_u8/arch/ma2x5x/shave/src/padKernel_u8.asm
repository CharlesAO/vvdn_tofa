.type mvcvPadKernel_u8_asm,@function
.version 0.50.00

.code .text.nonMax3x3_u8
.align 16

// padKernel, adds pad to a line with pixels at left and/or right side
.nowarn
//void mvcvpadKernel_u8(
//u8* iBuf      - i18
//u32 iBufLen   - i17
//u8* oBuf      - i16
//u32 padSz     - i15
//u32 padMode   - i14
//u32 padType   - i13
//u8 pixValue   - i12
//)

.lalign
mvcvPadKernel_u8_asm:

cmu.cpiv.x32 v23.0 i20 
cmu.cpiv.x32 v23.1 i21 
cmu.cpiv.x32 v23.2 i22 
cmu.cpiv.x32 v23.3 i23 
cmu.cpiv.x32 v22.0 i24 

	
iau.sub i0 i0 i0
lsu0.ldil i1 1
lsu0.ldil i2 2
lsu0.ldil i3 3
lsu0.ldil i4 4
lsu0.ldil i21 5
lsu0.ldil i5 0xff
iau.shr.u32 i17 i17 4
lsu0.ldil i10 padKernel_Left_Mirror             || lsu1.ldih i10 padKernel_Left_Mirror
lsu0.ldil i11 padKernel_Right_Mirror            || lsu1.ldih i11 padKernel_Right_Mirror
lsu0.ldil i24 padKernel_LeftAndRightMirror_loop || lsu1.ldih i24 padKernel_LeftAndRightMirror_loop
lsu0.ldil i20 padKernel_copy                    || lsu1.ldih i20 padKernel_copy
lsu0.ldil i22 padKernel_LeftAndRight_Mirror     || lsu1.ldih i22 padKernel_LeftAndRight_Mirror
lsu0.ldil i23 padKernel_end                     || lsu1.ldih i23 padKernel_end

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
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1 
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i1 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i2 //Mirror
peu.pc1c eq || bru.bra padKernel_Left_Mirror
nop 6
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i5 i16 || iau.incs i16 1
nop
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i12 i16 || iau.incs i16 1
 padKernel_copy:
lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i7 i17
 padKernel_Left_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16
bru.jmp i23
nop 6

 padKernel_Left_Mirror:
lsu0.ld.8 i6 i18 
nop 6
iau.and i6 i6 i5
iau.shr.u32 i9 i6 4
iau.shl i8 i6 4
iau.or i6 i8 i9
iau.and i6 i6 i5
bru.rpi i15 || lsu0.sti.8 i6 i16 || iau.incs i16 1
bru.bra padKernel_copy
nop 6
bru.jmp i23
nop 6


 padKernel_Right_loop_head:
lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i7 i17
padKernel_Right_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16
cmu.cmii.u8 i13 i0 //AllZero
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i1 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i2 //Mirror
peu.pc1c eq || bru.bra padKernel_Right_Mirror
nop 6
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i5 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i12 i16 || iau.incs i16 1
bru.jmp i23
nop 6

 padKernel_Right_Mirror:
cmu.cpvi.x16 i6.l v1.7
iau.shr.u32 i6 i6 8
iau.and i6 i6 i5
iau.shr.u32 i9 i6 4
iau.shl i8 i6 4
iau.or i6 i8 i9
iau.and i6 i6 i5
bru.rpi i15 || lsu0.sti.8 i6 i16 || iau.incs i16 1
bru.jmp i23
nop 6


 padKernel_LeftAndRight_loop_head:
cmu.cmii.u8 i13 i0 //AllZero
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i1 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i2 //Mirror
peu.pc1c eq || bru.bra padKernel_LeftAndRight_Mirror
nop 6
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i5 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i12 i16 || iau.incs i16 1

lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i7 i17
padKernel_LeftAndRight_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16

cmu.cmii.u8 i13 i0 //AllZero
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i1 //AllOne
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i1 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i3 //BlackPixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i0 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i4 //WhitePixel
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i5 i16 || iau.incs i16 1
cmu.cmii.u8 i13 i21 //PixelValue
peu.pc1c eq || bru.rpi i15 || lsu0.sti.8 i12 i16 || iau.incs i16 1
bru.jmp i23
nop 6

 padKernel_LeftAndRight_Mirror:
lsu0.ld.8 i6 i18 
nop 6
iau.and i6 i6 i5
iau.shr.u32 i9 i6 4
iau.shl i8 i6 4
iau.or i6 i8 i9
iau.and i6 i6 i5
bru.rpi i15 || lsu0.sti.8 i6 i16 || iau.incs i16 1
lsu0.ldo.64.l v1 i18 0 || lsu1.ldo.64.h v1 i18 8 || iau.add i18 i18 16 || bru.rpl i24 i17
padKernel_LeftAndRightMirror_loop:
nop 6
lsu0.sto.64.l v1 i16 0 || lsu1.sto.64.h v1 i16 8 || iau.add i16 i16 16
cmu.cpvi.x16 i6.l v1.7
iau.shr.u32 i6 i6 8
iau.and i6 i6 i5
iau.shr.u32 i9 i6 4
iau.shl i8 i6 4
iau.or i6 i8 i9
iau.and i6 i6 i5
bru.rpi i15 || lsu0.sti.8 i6 i16 || iau.incs i16 1

padKernel_end:
cmu.cpvi.x32 i20 v23.0 
cmu.cpvi.x32 i21 v23.1 
cmu.cpvi.x32 i22 v23.2 
cmu.cpvi.x32 i23 v23.3 
cmu.cpvi.x32 i24 v22.0 
		
bru.jmp i30
nop 6
.nowarnend
.size mvcvPadKernel_u8_asm,.-mvcvPadKernel_u8_asm
.end
