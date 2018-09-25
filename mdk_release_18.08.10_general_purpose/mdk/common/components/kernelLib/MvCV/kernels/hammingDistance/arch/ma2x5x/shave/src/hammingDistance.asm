.type mvcvHammingDistance_asm,@function
.version 00.70.00

.code .text.mvcvHammingDistance_asm
.align 16
//void mvcvHammingDistance_asm((unsigned char *d1(i18), unsigned char *d2(i17), int array_size(i16), int descriptor_size(i15), int *distances(i14))//
mvcvHammingDistance_asm:
    lsu0.cp v21.0 i20
    lsu0.cp v21.1 i21
    lsu0.cp v21.2 i22
    lsu0.cp v21.3 i23
    lsu0.cp v22.0 i24
    lsu0.cp v22.1 i25
    lsu0.cp v22.2 i26
    lsu0.cp v22.3 i27


// irf used
.set addrD1          i18
.set addrD20         i17
.set array_size      i16
.set descriptor_size i15
.set distances       i14

.set iDest0          i0
.set iDest1          i1
.set iDest2          i2
.set iDest3          i3
.set iDest4          i4
.set iDest5          i5
.set iDest6          i6
.set iDest7          i7
.set iCompare        i8
.set iCompare.l      i8.l
.set incr            i9
.set decr            i10
.set incrDist        i11
.set addrD21         i12
.set tmpCompare      i13
.set tmpCompare.l    i13.l
.set decr1           i15

// vrf used
.set vD10        v0
.set vD11        v1
.set vD12        v2
.set vD13        v3
.set vD14        v4
.set vD15        v5
.set vD16        v6
.set vD17        v7
.set vD20        v8
.set vD21        v9
.set vD22        v10
.set vD23        v11
.set vD24        v12
.set vD25        v13
.set vD26        v14
.set vD27        v15
.set vXOR0       v16
.set vXOR1       v17
.set vDest       v18
.set vDest.0     v18.0
.set vDest.1     v18.1
.set vDest.2     v18.2
.set vDest.3     v18.3
.set vDest.4     v18.4
.set vDest.5     v18.5
.set vDest.6     v18.6
.set vDest.7     v18.7
.set vCompare    v20
.set vCompare.0  v20.0

   LSU0.LDIL incr, 32 || LSU1.LDIL iCompare, 0x0400 || IAU.ADDSU addrD21, addrD20, 16
   //                                                                    preparation code depending of descriptor size
   LSU0.LD.64.l vD10, addrD1       || LSU1.LDO.64.h vD10, addrD1,   8
   LSU0.LDO.64.l vD11, addrD1,  16 || LSU1.LDO.64.h vD11, addrD1,  24 || IAU.SUB i0, descriptor_size, 128 // 128 bytes preparation code
   LSU0.LDO.64.l vD12, addrD1,  32 || LSU1.LDO.64.h vD12, addrD1,  40 || PEU.PCIX.EQ 0x1 || BRU.BRA mvcvHammingDistance_start || IAU.SUBSU distances, distances, 4
   LSU0.LDO.64.l vD13, addrD1,  48 || LSU1.LDO.64.h vD13, addrD1,  56 || IAU.XOR decr, decr, decr
   LSU0.LDO.64.l vD14, addrD1,  64 || LSU1.LDO.64.h vD14, addrD1,  72 || IAU.XOR tmpCompare, tmpCompare, tmpCompare
   LSU0.LDO.64.l vD15, addrD1,  80 || LSU1.LDO.64.h vD15, addrD1,  88 || IAU.NOT i1, decr
   LSU0.LDO.64.l vD16, addrD1,  96 || LSU1.LDO.64.h vD16, addrD1, 104 || IAU.INCS decr, 1
   LSU0.LDO.64.l vD17, addrD1, 112 || LSU1.LDO.64.h vD17, addrD1, 120
   NOP

   // 64 bytes preparation code
   IAU.SUB i0, descriptor_size, 64
   PEU.PC1I EQ || BRU.BRA mvcvHammingDistance_start
   CMU.CPVV vD14, vD10 || LSU0.LDIL iCompare, 0x0044
   CMU.CPVV vD15, vD11 || LSU0.LDIL tmpCompare, 1 || IAU.SHL decr, decr, 1
   CMU.CPVV vD16, vD12
   CMU.CPVV vD17, vD13
   NOP 2

   // 32 bytes preparation code
   CMU.CPVV vD12, vD10 || LSU0.LDIL iCompare, 0x2222
   CMU.CPVV vD13, vD11 || LSU0.LDIL tmpCompare, 3 || IAU.SHL decr, decr, 1
   CMU.CPVV vD16, vD10
   CMU.CPVV vD17, vD11

mvcvHammingDistance_start:
.nowarn 10
      LSU1.LDXVI vD20, addrD20, incr || IAU.SUB decr1, decr, 1 || CMU.CPVI.x32 i20 vCompare.0
.nowarnend
	  LSU0.LDXVI vD21, addrD21, incr || IAU.SHL incrDist, decr, 1
   LSU1.LDXVI vD22, addrD20, incr || IAU.SHL i1, i1, 16
   LSU0.LDXVI vD23, addrD21, incr || IAU.AND tmpCompare, array_size, tmpCompare
   LSU1.LDXVI vD24, addrD20, incr || IAU.SHL tmpCompare, tmpCompare, 2
   LSU0.LDXVI vD25, addrD21, incr || IAU.ROL tmpCompare, i1, tmpCompare
   LSU1.LDXVI vD26, addrD20, incr || IAU.AND tmpCompare, iCompare, tmpCompare
   LSU0.LDXVI vD27, addrD21, incr || IAU.ALIGN i20 i20 i20 2
   LSU1.LDXVI vD20, addrD20, incr || VAU.XOR vXOR0, vD10, vD20                             || IAU.ALIGN i21 tmpCompare tmpCompare 2
   LSU0.LDXVI vD21, addrD21, incr || VAU.XOR vXOR1, vD11, vD21 || SAU.ONESX.x128 i0, vXOR0 || IAU.ALIGN i20 i21 i20 2
   LSU1.LDXVI vD22, addrD20, incr || VAU.XOR vXOR0, vD12, vD22 || SAU.ONESX.x128 i1, vXOR1
   LSU0.LDXVI vD23, addrD21, incr || VAU.XOR vXOR1, vD13, vD23 || SAU.ONESX.x128 i2, vXOR0 || CMU.CPIV.x32 vCompare.0 i20

   LSU1.LDXVI vD24, addrD20, incr || VAU.XOR vXOR0, vD14, vD24 || SAU.ONESX.x128 i3, vXOR1
   LSU0.LDXVI vD25, addrD21, incr || VAU.XOR vXOR1, vD15, vD25 || SAU.ONESX.x128 i4, vXOR0 || IAU.ADDSU i1, i0, i1
   LSU1.LDXVI vD26, addrD20, incr || VAU.XOR vXOR0, vD16, vD26 || SAU.ONESX.x128 i5, vXOR1 || IAU.ADDSU i3, i2, i3
   LSU0.LDXVI vD27, addrD21, incr || VAU.XOR vXOR1, vD17, vD27 || SAU.ONESX.x128 i6, vXOR0 || IAU.SUBSU tmpCompare, array_size, decr1
   LSU1.LDXVI vD20, addrD20, incr || VAU.XOR vXOR0, vD10, vD20 || SAU.ONESX.x128 i7, vXOR1 || IAU.ADDSU i2, i4, i5 || PEU.PCIX.EQ 0x4 || CMU.CPVI.x16 iCompare.l, vCompare.0
                                                                                              IAU.ADDSU i4, i3, i1 || CMU.CPIT C_CMU0, iCompare
   // Header(20)||LSU.LDXVI(20)||VAU.XOR(20)||SAU.ONESX(20)||IAU.ADDSU(24)||CMU.CPIV(20)             = 124
   // Header(20)||LSU.LDXVI(20)||VAU.XOR(20)||SAU.ONESX(20)||IAU.ADDSU(24)||PEU.PCXX(12)||BRU.BRA(16) = 132
   .lalign
   mvcvHammingDistance_loop:
   IAU.ALIGN i22 i1 i1 2
   IAU.ALIGN i23 i22 i3 2
   LSU0.LDXVI vD21, addrD21, incr || VAU.XOR vXOR1, vD11, vD21 || SAU.ONESX.x128 i0, vXOR0 || IAU.ADDSU i6, i6, i7                    || CMU.CPIVD vDest.0, i1
   .nowarn 12,40
   IAU.ALIGN i24 i2 i2 2
   IAU.ALIGN i25 i24 i6 2
   LSU1.LDXVI vD22, addrD20, incr || VAU.XOR vXOR0, vD12, vD22 || SAU.ONESX.x128 i1, vXOR1 || IAU.ADDSU i7, i2, i6
   IAU.ALIGN i26 i4 i4 2
   IAU.ALIGN i27 i26 i7 2

   LSU0.LDXVI vD23, addrD21, incr || VAU.XOR vXOR1, vD13, vD23 || SAU.ONESX.x128 i2, vXOR0  || CMU.CPIV.x32 vDest.0, i23 || IAU.SUBSU array_size, array_size, decr
   LSU1.LDXVI vD24, addrD20, incr || VAU.XOR vXOR0, vD14, vD24 || SAU.ONESX.x128 i3, vXOR1 || IAU.ADDSU i5, i4, i7 || PEU.PCIX.NEQ 0x0 || BRU.BRA mvcvHammingDistance_loop
   LSU0.LDXVI vD25, addrD21, incr || VAU.XOR vXOR1, vD15, vD25 || SAU.ONESX.x128 i4, vXOR0 || IAU.ADDSU i1, i0, i1                    || CMU.CPIVD vDest.2, i4
   LSU1.LDXVI vD26, addrD20, incr || VAU.XOR vXOR0, vD16, vD26 || SAU.ONESX.x128 i5, vXOR1 || IAU.ADDSU i3, i2, i3
   LSU0.LDXVI vD27, addrD21, incr || VAU.XOR vXOR1, vD17, vD27 || SAU.ONESX.x128 i6, vXOR0 || IAU.SUBSU tmpCompare, array_size, decr1 || CMU.CPIV.x32 vDest.1, i25
   LSU1.LDXVI vD20, addrD20, incr || VAU.XOR vXOR0, vD10, vD20 || SAU.ONESX.x128 i7, vXOR1 || IAU.ADDSU i2, i4, i5 || PEU.PCIX.EQ 0x4 || CMU.CPVI.x16 iCompare.l, vCompare.0
   PEU.PVL016 GT || LSU0.ST.64.l vDest, distances                                          || IAU.ADDSU i4, i3, i1                    || CMU.CPIV.x32 vDest.2, i27
   .nowarnend
   PEU.PVL116 LT || LSU1.ST.64.h vDest, distances || IAU.ADDSU distances, distances, incrDist || CMU.CPIT C_CMU0, iCompare

nop 6
lsu0.cp  i20 v21.0
lsu0.cp  i21 v21.1
lsu0.cp  i22 v21.2
lsu0.cp  i23 v21.3
lsu0.cp  i24 v22.0
lsu0.cp  i25 v22.1
lsu0.cp  i26 v22.2
lsu0.cp  i27 v22.3





   BRU.JMP i30
   NOP 6
   .size mvcvHammingDistance_asm,.-mvcvHammingDistance_asm
   .end
