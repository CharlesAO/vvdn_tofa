//========================================================================================
// andreil 12.02.2014
//========================================================================================
.version 00.51.05

//Disable false warning as workaround until resolution of Bugzilla http://dub30/bugzilla/show_bug.cgi?id=22833
.nowarn 32
.include "myriad2SippDefs.inc"
.include "svuCommonDefinitions.incl"
.nowarnend

.code .text.sippSvuSync
.align 16

//########################################################################################
// C header: "void waitLeonStart(u32 mtxStopStart)"
// i18 = mtxStopStart
//########################################################################################
.lalign
waitLeonStart:
//Disable RPIM warning check since we know this one is intentional
.nowarn 18
   lsu0.ldil     i0, MUTEX_REQUEST_0
   iau.add       i3, i0, i18                      || lsu1.ldil   i1, MUTEX_RELEASE_0
   lsu0.sta.32   i3, SHAVE_LOCAL, MUTEX_CTRL_ADDR || lsu1.ldil   i2, MUTEX_GRANT_MASK
   nop 5
   cmu.cmti.bitp i2, P_GPI                        || iau.add     i4, i1, i18
   cmu.cmti.bitp i2, P_GPI                        || bru.rpim     0  || peu.pc1c EQ
   lsu0.sta.32   i4, SHAVE_LOCAL, MUTEX_CTRL_ADDR || bru.jmp    i30
   nop 6
.nowarnend

//########################################################################################
// C header: "void syncBarrier(SippPipeline *pl, u32 mtxStopStart)"
// i18 = mtxStopStart
//########################################################################################
.lalign
syncBarrier:
   //Disable RPIM warning check since we know this one is intentional
   .nowarn 18
   lsu0.ldil     i0, MUTEX_REQUEST_0              || lsu1.ldil i1, MUTEX_RELEASE_0
   iau.add       i3, i0, i17                      || lsu1.ldil i2, MUTEX_GRANT_MASK
   lsu0.sta.32   i3, SHAVE_LOCAL, MUTEX_CTRL_ADDR || iau.add i4, i1, i17
   nop 5
   cmu.cmti.bitp i2, P_GPI
   cmu.cmti.bitp i2, P_GPI                        || peu.pc1c EQ || bru.rpim 0
   lsu0.ldo.32 i7, i18, P_O_SVU_SYNC_SEM
   nop 6
   .nowarnend
   iau.incs    i7, -1
   lsu0.sto.32 i7, i18, P_O_SVU_SYNC_SEM
   lsu0.ldo.32 i7, i18, P_O_SVU_SYNC_SEM
   bru.jmp i30
   iau.incs i7, 0 // compare with ZERO
   nop
   nop
   nop
   nop
   lsu0.sta.32   i4, SHAVE_LOCAL, MUTEX_CTRL_ADDR || peu.pcix.EQ 0x00 || bru.swic 0x9

.end
