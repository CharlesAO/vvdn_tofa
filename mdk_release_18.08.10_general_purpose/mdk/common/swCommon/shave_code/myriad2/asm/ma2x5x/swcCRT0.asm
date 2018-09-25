///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief ASM file to be handling entry points
///

.version 00.51.05
//-------------------------------------------------------------------------------
.data .data.regsaves
.align 4
___regsaves:
//I regs:
  .int 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
//V regs
  .int 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  .int 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  .int 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
  .int 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
___i1save:
  .int 0x00000000
___dynContextPointer:
  .int 0x00000000

.code .text.AllTimeEntryPoint
___AllTimeEntryPoint:
//Save the dyncontext entry pointer first
lsu0.ldil i1, ___dynContextPointer || lsu1.ldih i1, ___dynContextPointer

lsu0.st.32 i21, i1
//At entry we'll have all registers set up and i0 will hold the next entry point,
//but let's allow for possibility to call things from C, so saving all possible registers in memory
lsu0.ldil i1, ___regsaves || lsu1.ldih i1, ___regsaves
lsu0.sto.32 i21, i1, (4*0) || lsu1.sto.32 i1, i1, (4*1)
lsu0.sto.32 i2, i1, (4*2) || lsu1.sto.32 i3, i1, (4*3)
lsu0.sto.32 i4, i1, (4*4) || lsu1.sto.32 i5, i1, (4*5)
lsu0.sto.32 i6, i1, (4*6) || lsu1.sto.32 i7, i1, (4*7)
lsu0.sto.32 i8, i1, (4*8) || lsu1.sto.32 i9, i1, (4*9)
lsu0.sto.32 i10, i1, (4*10) || lsu1.sto.32 i11, i1, (4*11)
lsu0.sto.32 i12, i1, (4*12) || lsu1.sto.32 i13, i1, (4*13)
lsu0.sto.32 i14, i1, (4*14) || lsu1.sto.32 i15, i1, (4*15)
lsu0.sto.32 i16, i1, (4*16) || lsu1.sto.32 i17, i1, (4*17)
lsu0.sto.32 i18, i1, (4*18) || lsu1.sto.32 i19, i1, (4*19)
lsu0.sto.32 i20, i1, (4*20) || lsu1.sto.32 i0, i1, (4*21)
lsu0.sto.32 i22, i1, (4*22) || lsu1.sto.32 i23, i1, (4*23)
lsu0.sto.32 i24, i1, (4*24) || lsu1.sto.32 i25, i1, (4*25)
lsu0.sto.32 i26, i1, (4*26) || lsu1.sto.32 i27, i1, (4*27)
lsu0.sto.32 i28, i1, (4*28) || lsu1.sto.32 i29, i1, (4*29)
lsu0.sto.32 i30, i1, (4*30) || lsu1.sto.32 i31, i1, (4*31) || iau.add i1, i1, (4*32)

lsu0.sto.64.l v0, i1, (0*8) || lsu1.sto.64.h v0, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v1, i1, (0*8) || lsu1.sto.64.h v1, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v2, i1, (0*8) || lsu1.sto.64.h v2, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v3, i1, (0*8) || lsu1.sto.64.h v3, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v4, i1, (0*8) || lsu1.sto.64.h v4, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v5, i1, (0*8) || lsu1.sto.64.h v5, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v6, i1, (0*8) || lsu1.sto.64.h v6, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v7, i1, (0*8) || lsu1.sto.64.h v7, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v8, i1, (0*8) || lsu1.sto.64.h v8, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v9, i1, (0*8) || lsu1.sto.64.h v9, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v10, i1, (0*8) || lsu1.sto.64.h v10, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v11, i1, (0*8) || lsu1.sto.64.h v11, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v12, i1, (0*8) || lsu1.sto.64.h v12, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v13, i1, (0*8) || lsu1.sto.64.h v13, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v14, i1, (0*8) || lsu1.sto.64.h v14, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v15, i1, (0*8) || lsu1.sto.64.h v15, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v16, i1, (0*8) || lsu1.sto.64.h v16, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v17, i1, (0*8) || lsu1.sto.64.h v17, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v18, i1, (0*8) || lsu1.sto.64.h v18, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v19, i1, (0*8) || lsu1.sto.64.h v19, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v20, i1, (0*8) || lsu1.sto.64.h v20, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v21, i1, (0*8) || lsu1.sto.64.h v21, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v22, i1, (0*8) || lsu1.sto.64.h v22, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v23, i1, (0*8) || lsu1.sto.64.h v23, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v24, i1, (0*8) || lsu1.sto.64.h v24, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v25, i1, (0*8) || lsu1.sto.64.h v25, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v26, i1, (0*8) || lsu1.sto.64.h v26, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v27, i1, (0*8) || lsu1.sto.64.h v27, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v28, i1, (0*8) || lsu1.sto.64.h v28, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v29, i1, (0*8) || lsu1.sto.64.h v29, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v30, i1, (0*8) || lsu1.sto.64.h v30, i1, (1*8) || iau.add i1, i1, 16
lsu0.sto.64.l v31, i1, (0*8) || lsu1.sto.64.h v31, i1, (1*8)
//And save i1:
lsu0.ldil i2, ___i1save || lsu1.ldih i2, ___i1save
lsu0.st.32 i1, i2

//Jump int a C function
lsu0.ldil i30, masterEntry || lsu1.ldih i30, masterEntry
bru.swp i30 || cmu.cpii i18, i21
nop
nop
nop
nop
nop
nop
//Restore the registers before going into the entry point.
//Restore i1 for restoring
lsu0.ldil i2, ___i1save || lsu1.ldih i2, ___i1save
lsu0.ld.32 i1, i2
nop
nop
nop
nop
nop
nop

//Restore all regs
lsu0.ldo.64.l v31, i1, (0*8) || lsu1.ldo.64.h v31, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v30, i1, (0*8) || lsu1.ldo.64.h v30, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v29, i1, (0*8) || lsu1.ldo.64.h v29, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v28, i1, (0*8) || lsu1.ldo.64.h v28, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v27, i1, (0*8) || lsu1.ldo.64.h v27, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v26, i1, (0*8) || lsu1.ldo.64.h v26, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v25, i1, (0*8) || lsu1.ldo.64.h v25, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v24, i1, (0*8) || lsu1.ldo.64.h v24, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v23, i1, (0*8) || lsu1.ldo.64.h v23, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v22, i1, (0*8) || lsu1.ldo.64.h v22, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v21, i1, (0*8) || lsu1.ldo.64.h v21, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v20, i1, (0*8) || lsu1.ldo.64.h v20, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v19, i1, (0*8) || lsu1.ldo.64.h v19, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v18, i1, (0*8) || lsu1.ldo.64.h v18, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v17, i1, (0*8) || lsu1.ldo.64.h v17, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v16, i1, (0*8) || lsu1.ldo.64.h v16, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v15, i1, (0*8) || lsu1.ldo.64.h v15, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v14, i1, (0*8) || lsu1.ldo.64.h v14, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v13, i1, (0*8) || lsu1.ldo.64.h v13, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v12, i1, (0*8) || lsu1.ldo.64.h v12, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v11, i1, (0*8) || lsu1.ldo.64.h v11, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v10, i1, (0*8) || lsu1.ldo.64.h v10, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v9, i1, (0*8) || lsu1.ldo.64.h v9, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v8, i1, (0*8) || lsu1.ldo.64.h v8, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v7, i1, (0*8) || lsu1.ldo.64.h v7, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v6, i1, (0*8) || lsu1.ldo.64.h v6, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v5, i1, (0*8) || lsu1.ldo.64.h v5, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v4, i1, (0*8) || lsu1.ldo.64.h v4, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v3, i1, (0*8) || lsu1.ldo.64.h v3, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v2, i1, (0*8) || lsu1.ldo.64.h v2, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v1, i1, (0*8) || lsu1.ldo.64.h v1, i1, (1*8) || iau.sub i1, i1, 16
lsu0.ldo.64.l v0, i1, (0*8) || lsu1.ldo.64.h v0, i1, (1*8) || iau.sub i1, i1, (4*32)

//                        // no reloading of i1 because this holds the loading address
lsu0.ldo.32 i21, i1, (4*0)// || lsu1.ldo.32 i1, i1, (4*1)
lsu0.ldo.32 i2, i1, (4*2) || lsu1.ldo.32 i3, i1, (4*3)
lsu0.ldo.32 i4, i1, (4*4) || lsu1.ldo.32 i5, i1, (4*5)
lsu0.ldo.32 i6, i1, (4*6) || lsu1.ldo.32 i7, i1, (4*7)
lsu0.ldo.32 i8, i1, (4*8) || lsu1.ldo.32 i9, i1, (4*9)
lsu0.ldo.32 i10, i1, (4*10) || lsu1.ldo.32 i11, i1, (4*11)
lsu0.ldo.32 i12, i1, (4*12) || lsu1.ldo.32 i13, i1, (4*13)
lsu0.ldo.32 i14, i1, (4*14) || lsu1.ldo.32 i15, i1, (4*15)
lsu0.ldo.32 i16, i1, (4*16) || lsu1.ldo.32 i17, i1, (4*17)
lsu0.ldo.32 i18, i1, (4*18) || lsu1.ldo.32 i19, i1, (4*19)
lsu0.ldo.32 i20, i1, (4*20) || lsu1.ldo.32 i0, i1, (4*21)
lsu0.ldo.32 i22, i1, (4*22) || lsu1.ldo.32 i23, i1, (4*23)
lsu0.ldo.32 i24, i1, (4*24) || lsu1.ldo.32 i25, i1, (4*25)
lsu0.ldo.32 i26, i1, (4*26) || lsu1.ldo.32 i27, i1, (4*27)
lsu0.ldo.32 i28, i1, (4*28) || lsu1.ldo.32 i29, i1, (4*29)
lsu0.ldo.32 i30, i1, (4*30) || lsu1.ldo.32 i31, i1, (4*31)
//Need to ensure slot delay
nop
nop
nop
nop
nop
nop

cmu.cpii i30, i29

//And jump into the entry point
bru.swp i30
nop
nop
nop
nop
nop
nop
//On returning from the entry point, jump into a fini tye of function
//The DynContext address was saved in ___dynContextPointer
lsu0.ldil i21, ___dynContextPointer || lsu1.ldih i21, ___dynContextPointer
lsu0.ldil i30, masterExit || lsu1.ldih i30, masterExit
lsu0.ld.32 i18, i21 || bru.swp i30
nop
nop
nop
nop
nop
nop
//End by stopping shave
bru.swih 0x1F
nop
nop
nop
nop
nop
nop
