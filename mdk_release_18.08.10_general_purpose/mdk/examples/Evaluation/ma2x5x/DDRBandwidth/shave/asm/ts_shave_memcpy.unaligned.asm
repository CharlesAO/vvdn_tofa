.version 00.51.05

.code .text.memcpy2_asm

.nowarn

// void *memcpy( void *dest, const void *src, size_t n )//
memcpy2_asm:
  .set dst i18
  .set src i17
  .set n i16
  .set UNALIGNED 0x4
// Enable performance counter to count clock cycles
    LSU0.LDIL I10, 1
    LSU0.STA.32 I10, SHAVE_LOCAL, 0x1800
    LSU0.LDIL I10, 4
    LSU0.STA.32 I10, SHAVE_LOCAL, 0x1834
	

lsu0.ldil i1  UNALIGNED        || lsu1.ldih i1  0x0 	        || iau.xor i8 i8 i8	 || sau.xor i7 i7 i7
		
iau.and i3 dst i1 	           
iau.fextu i4, dst, 31,1        || cmu.cmz.i32 i3 
peu.pcc0i.AND GT GT            || iau.incs i7,1	
	
iau.and i5 src i1 	           
iau.fextu i6, src, 31,1        || cmu.cmz.i32 i5 
peu.pcc0i.AND GT GT            || iau.incs i8,1		

//////i8 = 0 if aligned DDR or CMX src
//////i8 = 1 if unaligned DDR src

//////i7 = 0 if aligned DDR or CMX dst
//////i7 = 1 if unaligned DDR dst

iau.add i7,i7,0                || cmu.cmz.i32 i8 
peu.pcc0i.AND  EQ EQ           || bru.bra memcpy_ALIGNED
peu.pcc0i.AND  GT EQ           || bru.bra memcpy_SUNALIGNED_DALIGNED  
peu.pcc0i.AND  EQ GT           || bru.bra memcpy_SALIGNED_DUNALIGNED
peu.pcc0i.AND  GT GT           || bru.bra memcpy_UNALIGNED 
nop 6

.lalign //added
memcpy_ALIGNED:

lsu0.ldil i2 .L_MEMCPY_same_alignment || lsu1.ldih i2 .L_MEMCPY_same_alignment
bru.swp i2
nop 6
bru.bra end_label
nop 6

memcpy_UNALIGNED:

lsu0.ldi.32 i9,src             || lsu1.ldil i2 .L_MEMCPY_same_alignment       
iau.incs n,-8                  || lsu1.ldih i2 .L_MEMCPY_same_alignment 
bru.swp i2
nop 5
lsu0.sti.32 i9,dst             || cmu.cpii i10,src       
iau.add i10,i10,n          
lsu0.ld.32 i9,i10              || iau.add dst,dst,n  
bru.bra end_label
nop 5                         
lsu0.st.32 i9,dst	

memcpy_SUNALIGNED_DALIGNED:

lsu0.ldil i2 .L_MEMCPY_different_alignment || lsu1.ldih i2 .L_MEMCPY_different_alignment
iau.incs n,-8                              || lsu0.ldi.32    i0, src                    || bru.swp i2
cmu.cpii i11,dst 
cmu.cpii i10,src 
nop 4
iau.add i10,i10,n 
iau.incs i10,-4  
lsu0.ldi.32 i9,i10                         || iau.add i11,i11,n 
lsu0.ld.32 i9,i10
bru.bra end_label
nop 4                        
lsu0.sti.32 i9,i11
lsu0.st.32 i9,i11

memcpy_SALIGNED_DUNALIGNED:

lsu0.ldi.32 i9,src            || cmu.cpii i10,src    || lsu1.ldil i2 .L_MEMCPY_different_alignment 
lsu0.ldi.32 i0, src           || lsu1.ldih i2 .L_MEMCPY_different_alignment 
iau.incs n,-8                 || bru.swp i2
nop 5
lsu0.sti.32 i9,dst            || cmu.cpii i11,dst
iau.add i10,i10,n          
lsu0.ldi.32 i9,i10              || iau.add i11,i11,n 
lsu0.ld.32 i9,i10
bru.bra end_label
nop 4                        
lsu0.sti.32 i9,i11
lsu0.st.32 i9,i11

end_label:

////////Stop the clock!
    LSU0.LDIL I10, 0
    LSU0.STA.32 I10, SHAVE_LOCAL, 0x1834
    NOP 10
    bru.jmp            i30
    nop 6

.L_MEMCPY_same_alignment:	
	
  iau.shr.u32   i14,  n  9     || lsu0.ldil     i15, 0x200       	
  peu.pcix.EQ 0x0              || bru.bra .LCompensate         ||  iau.sub i15, dst i15 
 
                                  lsu1.cpfl2    src, 0x00
                                  lsu1.cpfl2    src, 0x04
                                  lsu1.cpfl2    src, 0x08
  lsu0.ldil     i13, .L1       || lsu1.cpfl2    src, 0x0C
  lsu0.ldih     i13, .L1       || lsu1.cpfl2    src, 0x10
                                  lsu1.cpfl2    src, 0x14
.L0:
  lsu0.ld.64.l  v23, src       || lsu1.cpfl2    src  0x18      || bru.rpl i13 i14 .L1
  lsu0.ldo.64.h v23, src 0x040 || lsu1.cpfl2    src  0x1C
  lsu0.ldo.64.l v22, src 0x080
  lsu0.ldo.64.h v22, src 0x0C0
  lsu0.ldo.64.l v21, src 0x100
  lsu0.ldo.64.h v21, src 0x140
  lsu0.ldo.64.l v20, src 0x180 || iau.incs      i15, 0x200
  lsu0.ldo.64.h v20, src 0x1C0 || lsu1.st.64.l  v23, i15
  lsu0.ldo.64.l v19, src 0x008 || lsu1.sto.64.h v23, i15 0x040
  lsu0.ldo.64.h v19, src 0x048 || lsu1.sto.64.l v22, i15 0x080
  lsu0.ldo.64.l v18, src 0x088 || lsu1.sto.64.h v22, i15 0x0C0
  lsu0.ldo.64.h v18, src 0x0C8 || lsu1.sto.64.l v21, i15 0x100
  lsu0.ldo.64.l v17, src 0x108 || lsu1.sto.64.h v21, i15 0x140
  lsu0.ldo.64.h v17, src 0x148 || lsu1.sto.64.l v20, i15 0x180
  lsu0.ldo.64.l v16, src 0x188 || lsu1.sto.64.h v20, i15 0x1C0
  lsu0.ldo.64.h v16, src 0x1C8 || lsu1.sto.64.l v19, i15 0x008
  lsu0.ldo.64.l v15, src 0x010 || lsu1.sto.64.h v19, i15 0x048
  lsu0.ldo.64.h v15, src 0x050 || lsu1.sto.64.l v18, i15 0x088
  lsu0.ldo.64.l v14, src 0x090 || lsu1.sto.64.h v18, i15 0x0C8
  lsu0.ldo.64.h v14, src 0x0D0 || lsu1.sto.64.l v17, i15 0x108
  lsu0.ldo.64.l v13, src 0x110 || lsu1.sto.64.h v17, i15 0x148
  lsu0.ldo.64.h v13, src 0x150 || lsu1.sto.64.l v16, i15 0x188
  lsu0.ldo.64.l v12, src 0x190 || lsu1.sto.64.h v16, i15 0x1C8
  lsu0.ldo.64.h v12, src 0x1D0 || lsu1.sto.64.l v15, i15 0x010
  lsu0.ldo.64.l v11, src 0x018 || lsu1.sto.64.h v15, i15 0x050
  lsu0.ldo.64.h v11, src 0x058 || lsu1.sto.64.l v14, i15 0x090
  lsu0.ldo.64.l v10, src 0x098 || lsu1.sto.64.h v14, i15 0x0D0
  lsu0.ldo.64.h v10, src 0x0D8 || lsu1.sto.64.l v13, i15 0x110
  lsu0.ldo.64.l  v9, src 0x118 || lsu1.sto.64.h v13, i15 0x150
  lsu0.ldo.64.h  v9, src 0x158 || lsu1.sto.64.l v12, i15 0x190
  lsu0.ldo.64.l  v8, src 0x198 || lsu1.sto.64.h v12, i15 0x1D0
  lsu0.ldo.64.h  v8, src 0x1D8 || lsu1.sto.64.l v11, i15 0x018
  lsu0.ldo.64.l v23, src 0x020 || lsu1.sto.64.h v11, i15 0x058
  lsu0.ldo.64.h v23, src 0x060 || lsu1.sto.64.l v10, i15 0x098
  lsu0.ldo.64.l v22, src 0x0A0 || lsu1.sto.64.h v10, i15 0x0D8
  lsu0.ldo.64.h v22, src 0x0E0 || lsu1.sto.64.l  v9, i15 0x118
  lsu0.ldo.64.l v21, src 0x120 || lsu1.sto.64.h  v9, i15 0x158
  lsu0.ldo.64.h v21, src 0x160 || lsu1.sto.64.l  v8, i15 0x198
  lsu0.ldo.64.l v20, src 0x1A0 || lsu1.sto.64.h  v8, i15 0x1D8
  lsu0.ldo.64.h v20, src 0x1E0 || lsu1.sto.64.l v23, i15 0x020
  lsu0.ldo.64.l v19, src 0x028 || lsu1.sto.64.h v23, i15 0x060
  lsu0.ldo.64.h v19, src 0x068 || lsu1.sto.64.l v22, i15 0x0A0
  lsu0.ldo.64.l v18, src 0x0A8 || lsu1.sto.64.h v22, i15 0x0E0
  lsu0.ldo.64.h v18, src 0x0E8 || lsu1.sto.64.l v21, i15 0x120
  lsu0.ldo.64.l v17, src 0x128 || lsu1.sto.64.h v21, i15 0x160
  lsu0.ldo.64.h v17, src 0x168 || lsu1.sto.64.l v20, i15 0x1A0
  lsu0.ldo.64.l v16, src 0x1A8 || lsu1.sto.64.h v20, i15 0x1E0
  lsu0.ldo.64.h v16, src 0x1E8 || lsu1.sto.64.l v19, i15 0x028
  lsu0.ldo.64.l v15, src 0x030 || lsu1.sto.64.h v19, i15 0x068
  lsu0.ldo.64.h v15, src 0x070 || lsu1.sto.64.l v18, i15 0x0A8
  lsu0.ldo.64.l v14, src 0x0B0 || lsu1.sto.64.h v18, i15 0x0E8
  lsu0.ldo.64.h v14, src 0x0F0 || lsu1.sto.64.l v17, i15 0x128
  lsu0.ldo.64.l v13, src 0x130 || lsu1.sto.64.h v17, i15 0x168
  lsu0.ldo.64.h v13, src 0x170 || lsu1.sto.64.l v16, i15 0x1A8
  lsu0.ldo.64.l v12, src 0x1B0 || lsu1.sto.64.h v16, i15 0x1E8
  lsu0.ldo.64.h v12, src 0x1F0 || lsu1.sto.64.l v15, i15 0x030
  lsu0.ldo.64.l v11, src 0x038 || lsu1.sto.64.h v15, i15 0x070
  lsu0.ldo.64.h v11, src 0x078 || lsu1.sto.64.l v14, i15 0x0B0
  lsu0.ldo.64.l v10, src 0x0B8 || lsu1.sto.64.h v14, i15 0x0F0
  lsu0.ldo.64.h v10, src 0x0F8 || lsu1.sto.64.l v13, i15 0x130
  lsu0.ldo.64.l  v9, src 0x138 || lsu1.sto.64.h v13, i15 0x170
  lsu0.ldo.64.h  v9, src 0x178 || lsu1.sto.64.l v12, i15 0x1B0
  lsu0.ldo.64.l  v8, src 0x1B8 || lsu1.sto.64.h v12, i15 0x1F0
  lsu0.ldo.64.h  v8, src 0x1F8 || lsu1.sto.64.l v11, i15 0x038
.L1:
  iau.incs    src, 0x200       || lsu1.sto.64.h v11, i15 0x078
  lsu0.cpfl2       src 0x00    || lsu1.sto.64.l v10, i15 0x0B8
  lsu0.cpfl2       src 0x04    || lsu1.sto.64.h v10, i15 0x0F8
  lsu0.cpfl2       src 0x08    || lsu1.sto.64.l  v9, i15 0x138
  lsu0.cpfl2       src 0x0C    || lsu1.sto.64.h  v9, i15 0x178
  lsu0.cpfl2       src 0x10    || lsu1.sto.64.l  v8, i15 0x1B8
  lsu0.cpfl2       src 0x14    || lsu1.sto.64.h  v8, i15 0x1F8
.Lend:

.LCompensate:

  iau.shl i13 i14,9            || lsu0.ldil  i12, 0x200     
  iau.sub i13,n,i13            || sau.add.i32 i15,i15,i12
  
  lsu0.ld.64.l  v23, src       ||  peu.pcix.EQ 0x0             || bru.jmp i2
  lsu0.ldo.64.h v23, src 0x008  
  lsu0.ldo.64.l v22, src 0x010 
  lsu0.ldo.64.h v22, src 0x018
  lsu0.ldo.64.l v21, src 0x020
  lsu0.ldo.64.h v21, src 0x028
  lsu0.ldo.64.l v20, src 0x030 
  lsu0.ldo.64.h v20, src 0x038 || lsu1.st.64.l  v23, i15        || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v19, src 0x040 || lsu1.sto.64.h v23, i15 0x008  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v19, src 0x048 || lsu1.sto.64.l v22, i15 0x010  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v18, src 0x050 || lsu1.sto.64.h v22, i15 0x018  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v18, src 0x058 || lsu1.sto.64.l v21, i15 0x020  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v17, src 0x060 || lsu1.sto.64.h v21, i15 0x028  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v17, src 0x068 || lsu1.sto.64.l v20, i15 0x030  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v16, src 0x070 || lsu1.sto.64.h v20, i15 0x038  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v16, src 0x078 || lsu1.sto.64.l v19, i15 0x040  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v15, src 0x080 || lsu1.sto.64.h v19, i15 0x048  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v15, src 0x088 || lsu1.sto.64.l v18, i15 0x050  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v14, src 0x090 || lsu1.sto.64.h v18, i15 0x058  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v14, src 0x098 || lsu1.sto.64.l v17, i15 0x060  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v13, src 0x0a0 || lsu1.sto.64.h v17, i15 0x068  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v13, src 0x0a8 || lsu1.sto.64.l v16, i15 0x070  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v12, src 0x0b0 || lsu1.sto.64.h v16, i15 0x078  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v12, src 0x0b8 || lsu1.sto.64.l v15, i15 0x080  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v11, src 0x0c0 || lsu1.sto.64.h v15, i15 0x088  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v11, src 0x0c8 || lsu1.sto.64.l v14, i15 0x090  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v10, src 0x0d0 || lsu1.sto.64.h v14, i15 0x098  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v10, src 0x0d8 || lsu1.sto.64.l v13, i15 0x0a0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l  v9, src 0x0e0 || lsu1.sto.64.h v13, i15 0x0a8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h  v9, src 0x0e8 || lsu1.sto.64.l v12, i15 0x0b0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l  v8, src 0x0f0 || lsu1.sto.64.h v12, i15 0x0b8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h  v8, src 0x0f8 || lsu1.sto.64.l v11, i15 0x0c0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v23, src 0x100 || lsu1.sto.64.h v11, i15 0x0c8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v23, src 0x108 || lsu1.sto.64.l v10, i15 0x0d0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v22, src 0x110 || lsu1.sto.64.h v10, i15 0x0d8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v22, src 0x118 || lsu1.sto.64.l  v9, i15 0x0e0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v21, src 0x120 || lsu1.sto.64.h  v9, i15 0x0e8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v21, src 0x128 || lsu1.sto.64.l  v8, i15 0x0f0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v20, src 0x130 || lsu1.sto.64.h  v8, i15 0x0f8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v20, src 0x138 || lsu1.sto.64.l v23, i15 0x100  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v19, src 0x140 || lsu1.sto.64.h v23, i15 0x108  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v19, src 0x148 || lsu1.sto.64.l v22, i15 0x110  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v18, src 0x150 || lsu1.sto.64.h v22, i15 0x118  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v18, src 0x158 || lsu1.sto.64.l v21, i15 0x120  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v17, src 0x160 || lsu1.sto.64.h v21, i15 0x128  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v17, src 0x168 || lsu1.sto.64.l v20, i15 0x130  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v16, src 0x170 || lsu1.sto.64.h v20, i15 0x138  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v16, src 0x178 || lsu1.sto.64.l v19, i15 0x140  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v15, src 0x180 || lsu1.sto.64.h v19, i15 0x148  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v15, src 0x188 || lsu1.sto.64.l v18, i15 0x150  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v14, src 0x190 || lsu1.sto.64.h v18, i15 0x158  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v14, src 0x198 || lsu1.sto.64.l v17, i15 0x160  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v13, src 0x1a0 || lsu1.sto.64.h v17, i15 0x168  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v13, src 0x1a8 || lsu1.sto.64.l v16, i15 0x170  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v12, src 0x1b0 || lsu1.sto.64.h v16, i15 0x178  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v12, src 0x1b8 || lsu1.sto.64.l v15, i15 0x180  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v11, src 0x1c0 || lsu1.sto.64.h v15, i15 0x188  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v11, src 0x1c8 || lsu1.sto.64.l v14, i15 0x190  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l v10, src 0x1d0 || lsu1.sto.64.h v14, i15 0x198  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h v10, src 0x1d8 || lsu1.sto.64.l v13, i15 0x1a0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l  v9, src 0x1e0 || lsu1.sto.64.h v13, i15 0x1a8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h  v9, src 0x1e8 || lsu1.sto.64.l v12, i15 0x1b0  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.l  v8, src 0x1f0 || lsu1.sto.64.h v12, i15 0x1b8  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldo.64.h  v8, src 0x1f8 || lsu1.sto.64.l v11, i15 0x1c0  || peu.pc1i neq || iau.incs i13,-8
                                  lsu1.sto.64.h v11, i15 0x1c8  || peu.pc1i neq || iau.incs i13,-8
                                  lsu1.sto.64.l v10, i15 0x1d0  || peu.pc1i neq || iau.incs i13,-8
                                  lsu1.sto.64.h v10, i15 0x1d8  || peu.pc1i neq || iau.incs i13,-8
                                  lsu1.sto.64.l  v9, i15 0x1e0  || peu.pc1i neq || iau.incs i13,-8
                                  lsu1.sto.64.h  v9, i15 0x1e8  || peu.pc1i neq || iau.incs i13,-8
                                  lsu1.sto.64.l  v8, i15 0x1f0  || peu.pc1i neq || iau.incs i13,-8
                                  lsu1.sto.64.h  v8, i15 0x1f8  || peu.pc1i neq 
								 
.LCompensateEnd:	
bru.jmp i2
nop 6

.L_MEMCPY_different_alignment:	

  iau.shr.u32   i14,  n  9 || lsu0.ldil i13, .L1_U    || lsu1.ldih i13, .L1_U  
  peu.pcix.EQ 0x0          || bru.bra .LCompensate_U  
  nop 5
  cmu.cpiv.x32   v0.3 i0       

.L0_U:                           
 
 lsu0.ldi.64.l v1,  src    || bru.rpl i13 i14 .L1_U
 lsu0.ldi.64.h v1,  src    
 lsu0.ldi.64.l v2,  src    
 lsu0.ldi.64.h v2,  src
 lsu0.ldi.64.l v3,  src    
 lsu0.ldi.64.h v3,  src
 lsu0.ldi.64.l v4,  src    
 lsu0.ldi.64.h v4,  src
 lsu0.ldi.64.l v5,  src    || cmu.alignvec v0 v0 v1 12 
 lsu0.ldi.64.h v5,  src                                           || lsu1.sti.64.l v0, dst   
 lsu0.ldi.64.l v6,  src    || cmu.alignvec v1 v1 v2 12            || lsu1.sti.64.h v0, dst   
 lsu0.ldi.64.h v6,  src                                           || lsu1.sti.64.l v1, dst   
 lsu0.ldi.64.l v7,  src    || cmu.alignvec v2 v2 v3 12            || lsu1.sti.64.h v1, dst   
 lsu0.ldi.64.h v7,  src                                           || lsu1.sti.64.l v2, dst   
 lsu0.ldi.64.l v8,  src    || cmu.alignvec v3 v3 v4 12            || lsu1.sti.64.h v2, dst   
 lsu0.ldi.64.h v8,  src                                           || lsu1.sti.64.l v3, dst   
 lsu0.ldi.64.l v9,  src    || cmu.alignvec v4 v4 v5 12            || lsu1.sti.64.h v3, dst   
 lsu0.ldi.64.h v9,  src                                           || lsu1.sti.64.l v4, dst   
 lsu0.ldi.64.l v10, src    || cmu.alignvec v5 v5 v6 12            || lsu1.sti.64.h v4, dst   
 lsu0.ldi.64.h v10, src                                           || lsu1.sti.64.l v5, dst   
 lsu0.ldi.64.l v11, src    || cmu.alignvec v6 v6 v7 12            || lsu1.sti.64.h v5, dst   
 lsu0.ldi.64.h v11, src                                           || lsu1.sti.64.l v6, dst   
 lsu0.ldi.64.l v12, src    || cmu.alignvec v7 v7 v8 12            || lsu1.sti.64.h v6, dst   
 lsu0.ldi.64.h v12, src                                           || lsu1.sti.64.l v7, dst   
 lsu0.ldi.64.l v13, src    || cmu.alignvec v8 v8 v9 12            || lsu1.sti.64.h v7, dst   
 lsu0.ldi.64.h v13, src                                           || lsu1.sti.64.l v8, dst   
 lsu0.ldi.64.l v14, src    || cmu.alignvec v9 v9 v10 12           || lsu1.sti.64.h v8, dst   
 lsu0.ldi.64.h v14, src                                           || lsu1.sti.64.l v9, dst   
 lsu0.ldi.64.l v15, src    || cmu.alignvec v10 v10 v11 12         || lsu1.sti.64.h v9, dst   
 lsu0.ldi.64.h v15, src                                           || lsu1.sti.64.l v10, dst  
 lsu0.ldi.64.l v16, src    || cmu.alignvec v11 v11 v12 12         || lsu1.sti.64.h v10, dst  
 lsu0.ldi.64.h v16, src                                           || lsu1.sti.64.l v11, dst  
 lsu0.ldi.64.l v1,  src    || cmu.alignvec v12 v12 v13 12         || lsu1.sti.64.h v11, dst  
 lsu0.ldi.64.h v1,  src                                           || lsu1.sti.64.l v12, dst  
 lsu0.ldi.64.l v2,  src    || cmu.alignvec v13 v13 v14 12         || lsu1.sti.64.h v12, dst  
 lsu0.ldi.64.h v2,  src                                           || lsu1.sti.64.l v13, dst  
 lsu0.ldi.64.l v3,  src    || cmu.alignvec v14 v14 v15 12         || lsu1.sti.64.h v13, dst  
 lsu0.ldi.64.h v3,  src                                           || lsu1.sti.64.l v14, dst  
 lsu0.ldi.64.l v4,  src    || cmu.alignvec v15 v15 v16 12         || lsu1.sti.64.h v14, dst  
 lsu0.ldi.64.h v4,  src                                           || lsu1.sti.64.l v15, dst  
 lsu0.ldi.64.l v5,  src    || cmu.alignvec v16 v16 v1 12          || lsu1.sti.64.h v15, dst  
 lsu0.ldi.64.h v5,  src                                           || lsu1.sti.64.l v16, dst  
 lsu0.ldi.64.l v6,  src    || cmu.alignvec v1 v1 v2 12            || lsu1.sti.64.h v16, dst  
 lsu0.ldi.64.h v6,  src                                           || lsu1.sti.64.l v1, dst   
 lsu0.ldi.64.l v7,  src    || cmu.alignvec v2 v2 v3 12            || lsu1.sti.64.h v1, dst   
 lsu0.ldi.64.h v7,  src                                           || lsu1.sti.64.l v2, dst   
 lsu0.ldi.64.l v8,  src    || cmu.alignvec v3 v3 v4 12            || lsu1.sti.64.h v2, dst   
 lsu0.ldi.64.h v8,  src                                           || lsu1.sti.64.l v3, dst   
 lsu0.ldi.64.l v9,  src    || cmu.alignvec v4 v4 v5 12            || lsu1.sti.64.h v3, dst   
 lsu0.ldi.64.h v9,  src                                           || lsu1.sti.64.l v4, dst   
 lsu0.ldi.64.l v10, src    || cmu.alignvec v5 v5 v6 12            || lsu1.sti.64.h v4, dst   
 lsu0.ldi.64.h v10, src                                           || lsu1.sti.64.l v5, dst   
 lsu0.ldi.64.l v11, src    || cmu.alignvec v6 v6 v7 12            || lsu1.sti.64.h v5, dst   
 lsu0.ldi.64.h v11, src                                           || lsu1.sti.64.l v6, dst   
 lsu0.ldi.64.l v12, src    || cmu.alignvec v7 v7 v8 12            || lsu1.sti.64.h v6, dst   
 lsu0.ldi.64.h v12, src                                           || lsu1.sti.64.l v7, dst   
 lsu0.ldi.64.l v13, src    || cmu.alignvec v8 v8 v9 12            || lsu1.sti.64.h v7, dst   
 lsu0.ldi.64.h v13, src                                           || lsu1.sti.64.l v8, dst   
 lsu0.ldi.64.l v14, src    || cmu.alignvec v9 v9 v10 12           || lsu1.sti.64.h v8, dst   
 lsu0.ldi.64.h v14, src                                           || lsu1.sti.64.l v9, dst   
 lsu0.ldi.64.l v15, src    || cmu.alignvec v10 v10 v11 12         || lsu1.sti.64.h v9, dst   
 lsu0.ldi.64.h v15, src                                           || lsu1.sti.64.l v10, dst  
 lsu0.ldi.64.l v16, src    || cmu.alignvec v11 v11 v12 12         || lsu1.sti.64.h v10, dst  
 lsu0.ldi.64.h v16, src                                           || lsu1.sti.64.l v11, dst  
                              cmu.alignvec v12 v12 v13 12         || lsu1.sti.64.h v11, dst  
 						                                             lsu1.sti.64.l v12, dst  
 .L1_U:																 
 						      cmu.alignvec v13 v13 v14 12         || lsu1.sti.64.h v12, dst  
 						                                             lsu1.sti.64.l v13, dst  
 						      cmu.alignvec v14 v14 v15 12         || lsu1.sti.64.h v13, dst  
 						                                             lsu1.sti.64.l v14, dst  
 						      cmu.alignvec v15 v15 v16 12         || lsu1.sti.64.h v14, dst  
 						                                             lsu1.sti.64.l v15, dst  
                              cmu.cpvv v0,v16                     || lsu1.sti.64.h v15, dst  

.LCompensate_U:
  iau.shl i13 i14,9               
  iau.sub i13,n,i13              

  lsu0.ldi.64.l v1,  src    ||  peu.pcix.EQ 0x0                   || bru.jmp i2
  lsu0.ldi.64.h v1,  src 
  lsu0.ldi.64.l v2,  src    
  lsu0.ldi.64.h v2,  src
  lsu0.ldi.64.l v3,  src    
  lsu0.ldi.64.h v3,  src
  lsu0.ldi.64.l v4,  src    
  lsu0.ldi.64.h v4,  src
  lsu0.ldi.64.l v5,  src    || cmu.alignvec v0 v0 v1 12 
  lsu0.ldi.64.h v5,  src                                           || lsu1.sti.64.l v0, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v6,  src    || cmu.alignvec v1 v1 v2 12            || lsu1.sti.64.h v0, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v6,  src                                           || lsu1.sti.64.l v1, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v7,  src    || cmu.alignvec v2 v2 v3 12            || lsu1.sti.64.h v1, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v7,  src                                           || lsu1.sti.64.l v2, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v8,  src    || cmu.alignvec v3 v3 v4 12            || lsu1.sti.64.h v2, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v8,  src                                           || lsu1.sti.64.l v3, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v9,  src    || cmu.alignvec v4 v4 v5 12            || lsu1.sti.64.h v3, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v9,  src                                           || lsu1.sti.64.l v4, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v10, src    || cmu.alignvec v5 v5 v6 12            || lsu1.sti.64.h v4, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v10, src                                           || lsu1.sti.64.l v5, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v11, src    || cmu.alignvec v6 v6 v7 12            || lsu1.sti.64.h v5, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v11, src                                           || lsu1.sti.64.l v6, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v12, src    || cmu.alignvec v7 v7 v8 12            || lsu1.sti.64.h v6, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v12, src                                           || lsu1.sti.64.l v7, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v13, src    || cmu.alignvec v8 v8 v9 12            || lsu1.sti.64.h v7, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v13, src                                           || lsu1.sti.64.l v8, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v14, src    || cmu.alignvec v9 v9 v10 12           || lsu1.sti.64.h v8, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v14, src                                           || lsu1.sti.64.l v9, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v15, src    || cmu.alignvec v10 v10 v11 12         || lsu1.sti.64.h v9, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v15, src                                           || lsu1.sti.64.l v10, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v16, src    || cmu.alignvec v11 v11 v12 12         || lsu1.sti.64.h v10, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v16, src                                           || lsu1.sti.64.l v11, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v1,  src    || cmu.alignvec v12 v12 v13 12         || lsu1.sti.64.h v11, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v1,  src                                           || lsu1.sti.64.l v12, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v2,  src    || cmu.alignvec v13 v13 v14 12         || lsu1.sti.64.h v12, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v2,  src                                           || lsu1.sti.64.l v13, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v3,  src    || cmu.alignvec v14 v14 v15 12         || lsu1.sti.64.h v13, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v3,  src                                           || lsu1.sti.64.l v14, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v4,  src    || cmu.alignvec v15 v15 v16 12         || lsu1.sti.64.h v14, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v4,  src                                           || lsu1.sti.64.l v15, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v5,  src    || cmu.alignvec v16 v16 v1 12          || lsu1.sti.64.h v15, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v5,  src                                           || lsu1.sti.64.l v16, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v6,  src    || cmu.alignvec v1 v1 v2 12            || lsu1.sti.64.h v16, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v6,  src                                           || lsu1.sti.64.l v1, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v7,  src    || cmu.alignvec v2 v2 v3 12            || lsu1.sti.64.h v1, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v7,  src                                           || lsu1.sti.64.l v2, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v8,  src    || cmu.alignvec v3 v3 v4 12            || lsu1.sti.64.h v2, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v8,  src                                           || lsu1.sti.64.l v3, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v9,  src    || cmu.alignvec v4 v4 v5 12            || lsu1.sti.64.h v3, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v9,  src                                           || lsu1.sti.64.l v4, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v10, src    || cmu.alignvec v5 v5 v6 12            || lsu1.sti.64.h v4, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v10, src                                           || lsu1.sti.64.l v5, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v11, src    || cmu.alignvec v6 v6 v7 12            || lsu1.sti.64.h v5, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v11, src                                           || lsu1.sti.64.l v6, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v12, src    || cmu.alignvec v7 v7 v8 12            || lsu1.sti.64.h v6, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v12, src                                           || lsu1.sti.64.l v7, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v13, src    || cmu.alignvec v8 v8 v9 12            || lsu1.sti.64.h v7, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v13, src                                           || lsu1.sti.64.l v8, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v14, src    || cmu.alignvec v9 v9 v10 12           || lsu1.sti.64.h v8, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v14, src                                           || lsu1.sti.64.l v9, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v15, src    || cmu.alignvec v10 v10 v11 12         || lsu1.sti.64.h v9, dst   || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v15, src                                           || lsu1.sti.64.l v10, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.l v16, src    || cmu.alignvec v11 v11 v12 12         || lsu1.sti.64.h v10, dst  || peu.pc1i neq || iau.incs i13,-8
  lsu0.ldi.64.h v16, src                                           || lsu1.sti.64.l v11, dst  || peu.pc1i neq || iau.incs i13,-8
                               cmu.alignvec v12 v12 v13 12         || lsu1.sti.64.h v11, dst  || peu.pc1i neq || iau.incs i13,-8
  						                                              lsu1.sti.64.l v12, dst  || peu.pc1i neq || iau.incs i13,-8    
  						       cmu.alignvec v13 v13 v14 12         || lsu1.sti.64.h v12, dst  || peu.pc1i neq || iau.incs i13,-8
  						                                              lsu1.sti.64.l v13, dst  || peu.pc1i neq || iau.incs i13,-8
  						       cmu.alignvec v14 v14 v15 12         || lsu1.sti.64.h v13, dst  || peu.pc1i neq || iau.incs i13,-8
  						                                              lsu1.sti.64.l v14, dst  || peu.pc1i neq || iau.incs i13,-8
  						       cmu.alignvec v15 v15 v16 12         || lsu1.sti.64.h v14, dst  || peu.pc1i neq || iau.incs i13,-8
  						                                              lsu1.sti.64.l v15, dst  || peu.pc1i neq || iau.incs i13,-8
                               cmu.cpvv v0,v16                     || lsu1.sti.64.h v15, dst  || peu.pc1i neq || iau.incs i13,-8
.LCompensateEnd_U:
bru.jmp i2
nop 6
.nowarnend