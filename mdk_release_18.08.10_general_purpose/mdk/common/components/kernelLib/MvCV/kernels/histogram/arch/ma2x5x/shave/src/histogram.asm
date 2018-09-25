///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvHistogram_asm,@function
.version 00.51.04

.data .data.histogram
.align 4
	  hist1:
		.fill 256, 4, 1
	  hist2:          
		.fill 256, 4, 1
      hist3:          
		.fill 256, 4, 1
      hist4:          
		.fill 256, 4, 1		
.code .text.histogram

mvcvHistogram_asm:
//void histogram(u8** in, u32 *hist, u32 width)
//                 i18      i17        i16
lsu0.cp v23.0 i20 || lsu1.cp v22.1 i25
lsu0.cp v23.1 i21 || lsu1.cp v22.2 i26
lsu0.cp v23.2 i22 || lsu1.cp v22.3 i27
lsu0.cp v23.3 i23 || lsu1.cp v21.0 i28
lsu0.cp v22.0 i24



lsu0.ld.32 i18 i18
lsu0.ldil i28, hist_loop     ||  lsu1.ldih i28, hist_loop
//lsu0.ldil i5, hist_loop_zero     ||  lsu1.ldih i5, hist_loop_zero
lsu0.ldil i12, hist1     ||  lsu1.ldih i12, hist1
lsu0.ldil i13, hist2     ||  lsu1.ldih i13, hist2
lsu0.ldil i14, hist3     ||  lsu1.ldih i14, hist3
lsu0.ldil i15, hist4     ||  lsu1.ldih i15, hist4
cmu.cpii i1 i12
cmu.cpii i2 i13
cmu.cpii i3 i14
cmu.cpii i4 i15
vau.xor v0 v0 v0 || lsu0.ldil i0, 0x40     ||  lsu1.ldih i0, 0
lsu0.sto.64.l v0 i1 0 || lsu1.sto.64.h v0 i1 8 || iau.add i1 i1 16 || bru.rpi i0
lsu0.sto.64.l v0 i2 0 || lsu1.sto.64.h v0 i2 8 || iau.add i2 i2 16 || bru.rpi i0
lsu0.sto.64.l v0 i3 0 || lsu1.sto.64.h v0 i3 8 || iau.add i3 i3 16 || bru.rpi i0
lsu0.sto.64.l v0 i4 0 || lsu1.sto.64.h v0 i4 8 || iau.add i4 i4 16 || bru.rpi i0


IAU.XOR i4 i4 i4 
IAU.XOR i5 i5 i5 
IAU.XOR i6 i6 i6 
IAU.XOR i7 i7 i7 
IAU.SHR.u32 i16 i16 2
IAU.ADD i0 i18 0
IAU.ADD i1 i0 i16
IAU.ADD i2 i1 i16
IAU.ADD i3 i2 i16
lsu0.ldi.8 i4 i0  
lsu0.ldi.8 i5 i1 
lsu0.ldi.8 i6 i2 
lsu0.ldi.8 i7 i3 
nop 3
IAU.SHL  i8 i4 2 
IAU.SHL  i9 i5 2
IAU.SHL i10 i6 2
IAU.SHL i11 i7 2


// compute 4 intermediary histograms
IAU.ADD i20 i12 i8  || lsu1.ldi.8 i4 i0 
IAU.ADD i21 i13 i9  || lsu1.ldi.8 i5 i1
IAU.ADD i22 i14 i10 || lsu1.ldi.8 i6 i2
IAU.ADD i23 i15 i11 || lsu1.ldi.8 i7 i3
lsu0.ld.32 i24 i20  || IAU.XOR i4 i4 i4   || bru.rpl i28 i16 
lsu0.ld.32 i25 i21  || IAU.XOR i5 i5 i5   
lsu0.ld.32 i26 i22  || IAU.XOR i6 i6 i6   
lsu0.ld.32 i27 i23  || IAU.XOR i7 i7 i7   
IAU.SHL  i8 i4 2
IAU.SHL  i9 i5 2
IAU.SHL i10 i6 2
IAU.SHL i11 i7 2
IAU.ADD i24 i24 1
hist_loop:
IAU.ADD i25 i25 1
IAU.ADD i26 i26 1
IAU.ADD i27 i27 1
lsu0.st.32 i24 i20 || IAU.ADD i20 i12 i8  || lsu1.ldi.8 i4 i0
lsu0.st.32 i25 i21 || IAU.ADD i21 i13 i9  || lsu1.ldi.8 i5 i1
lsu0.st.32 i26 i22 || IAU.ADD i22 i14 i10 || lsu1.ldi.8 i6 i2
lsu0.st.32 i27 i23 || IAU.ADD i23 i15 i11 || lsu1.ldi.8 i7 i3






//sum all 4 intermediary histograms

lsu0.ldil i0, 0x40     ||  lsu1.ldih i0, 0x0
lsu0.ldil i1, hist_loop1     ||  lsu1.ldih i1, hist_loop1

lsu0.ldo.64.l v20 i12 0 || lsu1.ldo.64.h v20 i12 8 || IAU.ADD i12 i12 16 
lsu0.ldo.64.l v19 i13 0 || lsu1.ldo.64.h v19 i13 8 || IAU.ADD i13 i13 16
lsu0.ldo.64.l v18 i14 0 || lsu1.ldo.64.h v18 i14 8 || IAU.ADD i14 i14 16
lsu0.ldo.64.l v17 i15 0 || lsu1.ldo.64.h v17 i15 8 || IAU.ADD i15 i15 16
lsu0.ldo.64.l v13 i17 0 || lsu1.ldo.64.h v13 i17 8 
nop 4

lsu0.ldo.64.l v20 i12 0 || lsu1.ldo.64.h v20 i12 8 || IAU.ADD i12 i12 16 || bru.rpl i1 i0
hist_loop1:          
.nowarn 10                 
vau.add.i32 v16 v20 v19 || lsu0.ldo.64.l v19 i13 0 || lsu1.ldo.64.h v19 i13 8 || IAU.ADD i13 i13 16
.nowarnend
vau.add.i32 v15 v18 v17 || lsu0.ldo.64.l v18 i14 0 || lsu1.ldo.64.h v18 i14 8 || IAU.ADD i14 i14 16
vau.add.i32 v12 v16 v13 || lsu0.ldo.64.l v17 i15 0 || lsu1.ldo.64.h v17 i15 8 || IAU.ADD i15 i15 16
                           lsu0.ldo.64.l v13 i17 16 || lsu1.ldo.64.h v13 i17 24 
vau.add.i32 v14 v12 v15
nop 1
lsu0.sto.64.l v14 i17 0 || lsu1.sto.64.h v14 i17 8 || IAU.ADD i17 i17 16





lsu0.cp i20 v23.0 || lsu1.cp i25 v22.1
lsu0.cp i21 v23.1 || lsu1.cp i26 v22.2
lsu0.cp i22 v23.2 || lsu1.cp i27 v22.3
lsu0.cp i23 v23.3 || lsu1.cp i28 v21.0          
lsu0.cp i24 v22.0






BRU.JMP i30
NOP 6
.size mvcvHistogram_asm,.-mvcvHistogram_asm
.end
