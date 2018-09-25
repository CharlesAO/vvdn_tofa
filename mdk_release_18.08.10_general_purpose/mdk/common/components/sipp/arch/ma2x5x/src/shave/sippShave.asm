.version 00.51.05

//Disable false warning as workaround until resolution of Bugzilla http://dub30/bugzilla/show_bug.cgi?id=22833
.nowarn 32
.include "myriad2SippDefs.inc"
.nowarnend

//########################################################################################
.data .data.__static_data__
.align 16
    sipp_pl:
     .word 0

//########################################################################################
.code .text.__MAIN__sect
.align 16
main:

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //DEBUG only: clear all regs to avoid XXX in VCS
  //(compiler saves some uninitialized regs on stack)
  lsu0.ldil  i0, 0 || lsu1.ldil i16, 0
  lsu0.ldil  i1, 0 || lsu1.ldil i17, 0 || cmu.cpivr.x32  v0, i0
  lsu0.ldil  i2, 0 || lsu1.ldil i18, 0 || cmu.cpivr.x32  v1, i0 || vau.or v16, v0, v0
  lsu0.ldil  i3, 0                     || cmu.cpivr.x32  v2, i0 || vau.or v17, v0, v0
  lsu0.ldil  i4, 0 || lsu1.ldil i20, 0 || cmu.cpivr.x32  v3, i0 || vau.or v18, v0, v0
  lsu0.ldil  i5, 0 || lsu1.ldil i21, 0 || cmu.cpivr.x32  v4, i0 || vau.or v19, v0, v0
  lsu0.ldil  i6, 0 || lsu1.ldil i22, 0 || cmu.cpivr.x32  v5, i0 || vau.or v20, v0, v0
  lsu0.ldil  i7, 0 || lsu1.ldil i23, 0 || cmu.cpivr.x32  v6, i0 || vau.or v21, v0, v0
  lsu0.ldil  i8, 0 || lsu1.ldil i24, 0 || cmu.cpivr.x32  v7, i0 || vau.or v22, v0, v0
  lsu0.ldil  i9, 0 || lsu1.ldil i25, 0 || cmu.cpivr.x32  v8, i0 || vau.or v23, v0, v0
  lsu0.ldil i10, 0 || lsu1.ldil i26, 0 || cmu.cpivr.x32  v9, i0 || vau.or v24, v0, v0
  lsu0.ldil i11, 0 || lsu1.ldil i27, 0 || cmu.cpivr.x32 v10, i0 || vau.or v25, v0, v0
  lsu0.ldil i12, 0 || lsu1.ldil i28, 0 || cmu.cpivr.x32 v11, i0 || vau.or v26, v0, v0
  lsu0.ldil i13, 0 || lsu1.ldil i29, 0 || cmu.cpivr.x32 v12, i0 || vau.or v27, v0, v0
  lsu0.ldil i14, 0 || lsu1.ldil i30, 0 || cmu.cpivr.x32 v13, i0 || vau.or v28, v0, v0
  lsu0.ldil i15, 0 || lsu1.ldil i31, 0 || cmu.cpivr.x32 v14, i0 || vau.or v29, v0, v0
                                          cmu.cpivr.x32 v15, i0 || vau.or v30, v0, v0
                                                                   vau.or v31, v0, v0

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //This is MAIN// no need to save [preserved] registers on stack, as
  //there's no system level code above MAIN that we need to preserve !

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup stack and do main inits
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    lsu0.ldil  i20, sipp_pl || lsu1.ldih i20, sipp_pl
    lsu0.ld.32 i20, i20     || lsu1.ldil i21, 0
    nop 6
    lsu0.ldo.32 i22, i20, P_O_N_SVU_FILTS
    lsu0.ldo.32 i23, i20, P_O_SCHED_NFO


//@@@@@@@@@@@@@@@@@@@@@@@@
.lalign
__main_loop:
//@@@@@@@@@@@@@@@@@@@@@@@@

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //0)My loop debug
 //   lsu0.ldil i30, sippShaveDebug || lsu1.ldih i30, sippShaveDebug
 //   lsu0.ldil i17, 5              || lsu1.ldih i17, 0
 //   bru.swp i30                   || cmu.cpii  i18, i20
 //   nop 6

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //1)Wait Leon start
    lsu0.ldil i30, waitLeonStart || lsu1.ldih i30, waitLeonStart || iau.add i31, i20, i21
    bru.swp i30                  || lsu0.ldo.32 i18, i31, P_O_SVU_SYNC_MTX
    nop 6

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //2)Call h-Padding
    lsu0.ldil i30, sippHorizontalPadding || lsu1.ldih i30, sippHorizontalPadding
    bru.swp i30                          || cmu.cpii  i18, i20
    nop 6

 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //3)Filter call setup
    lsu0.ldo.32 i24, i20, P_O_ITER || cmu.cpii i26, i20
                                      cmu.cpii i27, i22
    lsu0.ldo.32 i29, i26, P_O_SVU_LIST
    nop 4
    iau.mul     i24, i24, SZ_SCH_INFO
    nop
    iau.add     i24, i24, i23          || lsu0.ldo.32 i30, i29, F_O_SVU_RUN
    lsu0.ldo.32 i25, i24, S_O_SVU_MASK
    nop 6

 //@@@@@@@@@@@@@@@@@@@@@@@@
 .lalign
 __filter_call_loop:
 //@@@@@@@@@@@@@@@@@@@@@@@@
                                                            iau.add    i26, i26, 4
       iau.fextu i31, i25, 0, 1 || sau.shr.u32 i25,i25,1 || lsu0.ldo.32 i29, i26, P_O_SVU_LIST
       peu.pcix.NEQ 0x00        || bru.swp i30
       cmu.cpii  i18, i29
       iau.subsu i27, i27, 1
       nop 4

     //On return from filter function, go to next filter
       cmu.cmz.i32 i27
       peu.pc1c GT || bru.bra __filter_call_loop || lsu0.ldo.32 i30, i29, F_O_SVU_RUN
       nop 6

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //4) syncBarrier
    lsu0.ldil i30, syncBarrier || lsu1.ldih i30, syncBarrier || iau.add i31, i20, i21
    cmu.cpii  i18, i20         || bru.swp   i30 || lsu0.ldo.32 i17, i31, P_O_SVU_SYNC_MTX
    nop 6

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //5) toggle mtxParity and go to main loop
    bru.bra __main_loop        || lsu1.ldil i31, 4
    iau.sub i21, i31, i21
    nop 5


.end
