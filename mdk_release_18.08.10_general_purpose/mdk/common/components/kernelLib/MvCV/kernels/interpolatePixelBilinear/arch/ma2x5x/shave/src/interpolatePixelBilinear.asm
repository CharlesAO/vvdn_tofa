.type mvcvInterpolatePixelBilinear_asm,@function
.version 00.70.00

.code .text.mvcvInterpolatePixelBilinear_asm
.align 16
//half interpolatePixelBilinear_C(half* line1(i18), half* line2(i17), float x(i16), float y(i15))
mvcvInterpolatePixelBilinear_asm:

// irf used
.set addrLine1     i18
.set addrLine2      i17

.set xp          i0
.set yp          i1
.set p0          i2
.set p1          i3
.set width2      i4

// srf used
.set result     i18
.set x          i16
.set y          i15

.set w0         i5
.set sFracX     i6
.set sFracY     i7

// vrf used
.set vP0       v0
.set vP1       v1
.set vW        v2
.set vFracXY   v3
.set vFracXY.0 v3.0
.set vFracXY.1 v3.1
.set vFracXY_1 v4
.set vOne      v5
.set vWTmp     v6
//{
//    int  xp, yp//
//    int  xp_plus_1, yp_plus_1//
//    half w0, w1, w2, w3//
//    half *p0, *p1//
//    half result//
//
//    p0 = &line1[xp]//
//    p1 = &line2[xp]//
//    w0 = (xp_plus_1 - x)  * (yp_plus_1 - y)//
//    w1 = (x         - xp) * (yp_plus_1 - y)//
//    w2 = (xp_plus_1 - x)  * (y         - yp)//
//    w3 = (x         - xp) * (y         - yp)//
//    result = w0*p0[0] + w1*p0[1] + w2*p1[0] + w3*p1[1]//
//    return result//
//}
// 17 cycles vs 42 cycles
   CMU.CPII.f32.i32s xp, x              || SAU.FRAC.f32 sFracX, x         || VAU.XOR vOne, vOne, vOne
   CMU.CPII.f32.i32s yp, y              || SAU.FRAC.f32 sFracY, y         || VAU.ADD.f32 vOne, vOne, 1.0
   IAU.SHL xp, xp, 1                    || CMU.CPIV.x32 vFracXY.0, sFracX
   CMU.CPIV.x32 vFracXY.1, sFracY || IAU.ADD p0, addrLine1, xp
   CMU.CPVV.f32.f16 vFracXY, vFracXY || IAU.ADD p1, addrLine2, xp || VAU.SUB.f32 vFracXY_1, vOne, vFracXY
   LSU0.LD.32R vP1, p1 || LSU1.LD.32R vP0, p0
   NOP
   CMU.CPVV.f32.f16 vFracXY_1, vFracXY_1
   CMU.VILV.x16 vFracXY, vFracXY_1, vFracXY, vFracXY_1
   CMU.VILV.x32 vFracXY, vFracXY_1, vFracXY, vFracXY
   VAU.MUL.f16 vW, vFracXY, vFracXY     || LSU0.SWZV8 [00005544]
   VAU.XOR vWTmp, vWTmp, vWTmp
   CMU.ALIGNVEC vP0, vP0, vP1, 12
   PEU.PVEN4.WORD 0x3 || VAU.MUL.f16 vWTmp, vW, vP0
   BRU.JMP i30
   NOP
   SAU.SUMX.f16 result, vWTmp
   NOP 4
   .size mvcvInterpolatePixelBilinear_asm,.-mvcvInterpolatePixelBilinear_asm
   .end
