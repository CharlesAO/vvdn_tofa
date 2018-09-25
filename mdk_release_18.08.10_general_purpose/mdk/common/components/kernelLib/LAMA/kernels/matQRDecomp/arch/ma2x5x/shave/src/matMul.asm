//matrix multiply
// @param[in]  A   - structure that contains info regarding width, height, pointer to first matrix
// @param[in]  B   - structure that contains info regarding width, height, pointer to second matrix
// @param[out] out - structure that contains info regarding width, height, pointer to output matrix

.type mvcvMatMul_asm,@function
.version 00.50.02
.nowarn
.code .text.mvcvMatMul_asm
.align 16
//
//typedef	struct
//{
//	unsigned int	m, n//
//	unsigned int	max_m, max_n, max_size//
//	Real			**me, *base//
//} MAT//
//
//MAT *mvcvMatMul(const MAT *A, const MAT *B, MAT *out)
//                       i18 ,        i17   ,   i16


    mvcvMatMul_asm:
 	 // ------ IRF usage ------
	.set M		  	i0
	.set N 		  	i1
	.set K 		  	i2
	.set p_lA	  	i3
	.set p_lB 	  	i4
	.set p_lC	  	i5
	.set A 		  	i6
	.set B 			i7
	.set C 			i8
	.set LB0 		i10
	.set LB1 		i11
	.set LB2 		i12
	.set LB3 		i13
	.set C_cond 	i14
	.set one 		i15
	.set repb 		i20
	.set repa 		i21
	.set imask 		i22
	.set inc0 		i23
	.set inc1 		i24
	.set inc2 		i25
	.set inc3 		i26
	.set tmp 		i27
	.set Nx4 		i28
	.set Kx4 		i29

  	// ------ VRF usage ------
	.set a0 		v0
	.set a1 		v1
	.set a2 		v2
	.set a3 		v3
	.set b0 		v4
	.set b1 		v5
	.set b2 		v6
	.set b3 		v7
	.set vmask 		v8
	.set ffff  		v9
	.set c0 		v10
	.set vmaskb 	v11
	.set p0 		v12
	.set p1 		v13
	.set p2 		v14
	.set p3 		v15




        IAU.INCS i19 -8
        LSU0.ST.64 i20 i19 || IAU.INCS i19 -8  || LSU1.LDO.32 		M i18, 0
        LSU0.ST.64 i22 i19 || IAU.INCS i19 -8  || LSU1.LDO.32 		N i18, 4
        LSU0.ST.64 i24 i19 || IAU.INCS i19 -8  || LSU1.LDO.32    p_lA i18, 24
        LSU0.ST.64 i26 i19 || IAU.INCS i19 -8  || LSU1.LDO.32 		K i17, 4
        LSU0.ST.64 i28 i19 || IAU.INCS i19 -16
        VAU.XOR v0 v0 v0   || LSU1.LDO.32 p_lC i16, 24
        LSU0.ST.64.L v0 i19
        LSU0.STO.64.H v0 i19 8
        nop

        IAU.XOR tmp tmp tmp    || LSU1.LDO.32 p_lB i17, 24
        IAU.XOR one one one	   || VAU.XOR ffff ffff ffff
        LSU0.LDIL one 0x1	   || IAU.SHL Nx4 N 2
        IAU.FEXTU tmp K 0 2    || VAU.NOT ffff ffff || 	LSU0.CP  A p_lA
        IAU.SHL tmp one tmp    || SAU.SUB.i32 repa N 4
        IAU.INCS tmp -1
        IAU.SHL Kx4 K 2		   || CMU.CMASK vmaskb tmp 0x2
        SAU.XOR C_cond C_cond C_cond
        nop 2

.lalign
mvcvMatMul_loop_rows_A:
        CMU.CMZ.i32 repa
        PEU.PC1C LTE || BRU.BRA mvcvMatMul_extra_columns_A
        LSU0.LDI.32R a0 A || IAU.ADD LB0  p_lB 0x0
        LSU0.LDI.32R a1 A || IAU.ADD LB1  LB0 Kx4
        LSU0.LDI.32R a2 A || IAU.ADD LB2  LB1 Kx4
        LSU0.LDI.32R a3 A || IAU.ADD LB3  LB2 Kx4
        LSU0.CP C p_lC    || IAU.ADD p_lB LB3 Kx4
        IAU.SUB repb K 0
.lalign
mvcvMatMul_loop_columns_A:
        VAU.XOR c0 c0 c0  		 || CMU.CMZ.i32 C_cond
        LSU0.LDO.64.L b0 LB0 0x0 || LSU1.LDO.64.H b0 LB0 0x8 || IAU.INCS LB0 16
        LSU0.LDO.64.L b1 LB1 0x0 || LSU1.LDO.64.H b1 LB1 0x8 || IAU.INCS LB1 16
        LSU0.LDO.64.L b2 LB2 0x0 || LSU1.LDO.64.H b2 LB2 0x8 || IAU.INCS LB2 16
        LSU0.LDO.64.L b3 LB3 0x0 || LSU1.LDO.64.H b3 LB3 0x8 || IAU.INCS LB3 16
        LSU0.LDO.64.l c0 C 0x0   || LSU1.LDO.64.H c0 C 0x8   || PEU.PC1C GT
        nop 2
.lalign
mvcvMatMul_loop_columns_B:
        vau.mul.f32 p0 b0 a0
        vau.mul.f32 p1 b1 a1 || LSU0.LDO.64.L b0 LB0 0x0 || LSU1.LDO.64.H b0 LB0 0x8 || IAU.INCS LB0 16
        vau.mul.f32 p2 b2 a2 || LSU0.LDO.64.L b1 LB1 0x0 || LSU1.LDO.64.H b1 LB1 0x8 || IAU.INCS LB1 16
        vau.mul.f32 p3 b3 a3 || LSU0.LDO.64.L b2 LB2 0x0 || LSU1.LDO.64.H b2 LB2 0x8 || IAU.INCS LB2 16
        vau.add.f32 p0 p0 p1 || LSU0.LDO.64.L b3 LB3 0x0 || LSU1.LDO.64.H b3 LB3 0x8 || IAU.INCS LB3 16
        nop
        vau.add.f32 p2 p2 p3 ||	iau.sub repb repb 4
        vau.add.f32 c0 c0 p0 || CMU.CMZ.i32 repb
        nop
        PEU.PC1C GT 		 || BRU.BRA mvcvMatMul_loop_columns_B
        vau.add.f32 c0 c0 p2 || CMU.CMZ.i32 ffff
        PEU.PC1I LT 		 || CMU.CMZ.i32 vmaskb
        nop
        LSU0.STXV c0 C   || PEU.PVL032 NEQ 			 || IAU.ADD C C 16
        VAU.XOR c0 c0 c0 || CMU.CMZ.i32 C_cond
        PEU.PC1C GT		 || LSU0.LDO.64.l c0 C 0x0   || LSU1.LDO.64.H c0 C 0x8

//////////////////////////////////// end of mvcvMatMul_loop_columns_B ////////////////////////////////////////

        IAU.INCS repa -4
        CMU.CMZ.i32 repa  || IAU.INCS C_cond 1
        PEU.PC1C GT       || BRU.BRA mvcvMatMul_loop_columns_A
        LSU0.LDI.32R a0 A || IAU.ADD LB0  p_lB 0x0
        LSU0.LDI.32R a1 A || IAU.ADD LB1  LB0 Kx4
        LSU0.LDI.32R a2 A || IAU.ADD LB2  LB1 Kx4
        LSU0.LDI.32R a3 A || IAU.ADD LB3  LB2 Kx4
        LSU0.CP C p_lC    || IAU.ADD p_lB LB3 Kx4
        IAU.SUB repb K 0

//////////////////////////////////// end of mvcvMatMul_loop_columns_A ////////////////////////////////////////

.lalign
mvcvMatMul_extra_columns_A:
        IAU.INCS repa 4
        IAU.SHL imask one repa || vau.xor a0 a0 a0
        SAU.XOR inc0 inc0 inc0 || IAU.INCS imask -1
        SAU.XOR inc1 inc1 inc1 || LSU1.LDIL inc0 4 || CMU.CMASK vmask imask 0x2 || IAU.SUB A A 16
        SAU.XOR inc2 inc2 inc2 || LSU1.LDIl inc1 4 || LSU0.CP C p_lC
        SAU.XOR inc3 inc3 inc3 || LSU1.LDIL inc2 4 || LSU0.LDXV a0 A
        LSU1.LDIL inc3 4
        IAU.INCS repa -1
        PEU.PC1I LT || LSU0.CP LB0  i19 || LSU1.LDIL inc0 0 || IAU.INCS repa -1
        PEU.PC1I LT || LSU0.CP LB1  i19 || LSU1.LDIl inc1 0 || IAU.INCS repa -1
        PEU.PC1I LT || LSU0.CP LB2  i19 || LSU1.LDIL inc2 0 || IAU.INCS repa -1

        VAU.XOR c0 c0 c0  		 || CMU.CMZ.i32 C_cond   || IAU.SUB repb K 0
        LSU0.LDO.64.L b0 LB0 0x0 || LSU1.LDO.64.H b0 LB0 0x8 || IAU.INCS LB0 16
        VAU.AND a0 a0 vmask
        LSU0.LDO.64.L b1 LB1 0x0 || LSU1.LDO.64.H b1 LB1 0x8 || IAU.INCS LB1 16 || 	CMU.VILV.x32 a0 a2 a0 a0
        LSU0.LDO.64.L b2 LB2 0x0 || LSU1.LDO.64.H b2 LB2 0x8 || IAU.INCS LB2 16 || 	CMU.VILV.X32 a0 a1 a0 a0
        LSU0.LDO.64.L b3 LB3 0x0 || LSU1.LDO.64.H b3 LB3 0x8 || IAU.INCS LB3 16 || 	CMU.VILV.x32 a2 a3 a2 a2
        LSU0.LDO.64.l c0 C 0x0   || LSU1.LDO.64.H c0 C 0x8   || PEU.PC1C GT
        nop 2

.lalign
mvcvMatMul_extra_loop_columns_B:
        vau.mul.f32 p0 b0 a0
        vau.mul.f32 p1 b1 a1 || LSU0.LDO.64.L b0 LB0 0x0 || LSU1.LDO.64.H b0 LB0 0x8 || IAU.INCS LB0 16
        vau.mul.f32 p2 b2 a2 || LSU0.LDO.64.L b1 LB1 0x0 || LSU1.LDO.64.H b1 LB1 0x8 || IAU.INCS LB1 16
        vau.mul.f32 p3 b3 a3 || LSU0.LDO.64.L b2 LB2 0x0 || LSU1.LDO.64.H b2 LB2 0x8 || IAU.INCS LB2 16
        vau.add.f32 p0 p0 p1 || LSU0.LDO.64.L b3 LB3 0x0 || LSU1.LDO.64.H b3 LB3 0x8 || IAU.INCS LB3 16
        nop
        vau.add.f32 p2 p2 p3 || iau.sub repb repb 4
        vau.add.f32 c0 c0 p0 || CMU.CMZ.i32 repb
        nop
        PEU.PC1C GT || BRU.BRA mvcvMatMul_extra_loop_columns_B
        vau.add.f32 c0 c0 p2
        CMU.CMZ.i32 ffff
        PEU.PC1I LT      || CMU.CMZ.i32 vmaskb
        LSU1.STXV c0 C   || PEU.PVL132 NEQ
        VAU.XOR c0 c0 c0 || IAU.ADD C C 16 			 || CMU.CMZ.i32 C_cond
        PEU.PC1C GT	     || LSU0.LDO.64.l c0 C 0x0   || LSU1.LDO.64.H c0 C 0x8

//////////////////////////////////// end of mvcvMatMul_extra_loop_columns_B ////////////////////////////////////////

        LSU0.LDO.32 p_lB i17, 24 || SAU.SUB.i32 repa N 4     || IAU.INCS M -1
        PEU.PC1I GT              || BRU.BRA  mvcvMatMul_loop_rows_A
        IAU.XOR C_cond  C_cond C_cond
        IAU.ADD p_lA p_lA Nx4
        LSU0.CP  A p_lA          || IAU.ADD p_lC p_lC Kx4
        nop 3

//////////////////////////////////// end of mvcvMatMul_loop_rows_A ////////////////////////////////////////////////////////////

        IAU.INCS i19 16
        LSU0.LD.64 i28 i19 || IAU.INCS i19 8
        LSU0.LD.64 i26 i19 || IAU.INCS i19 8
        LSU0.LD.64 i24 i19 || IAU.INCS i19 8
        LSU0.LD.64 i22 i19 || IAU.INCS i19 8
        LSU0.LD.64 i20 i19 || IAU.INCS i19 8

        bru.jmp i30
        iau.add i18 i16 0
        nop 5

.size mvcvMatMul_asm,.-mvcvMatMul_asm
.nowarnend
.end
