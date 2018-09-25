.version 00.51.04

.data .data.sampleApp
.align 4

.code .text.sampleApp
.align 16

//void sampleApp_asm (unsigned char *in, unsigned int width, unsigned char value, unsigned char operation, unsigned int pxToChange, unsigned char *out)//
sampleApp_asm:

// in (i18)
// width (i17)/8
// value (i16)
// operation (i15)
// pxToChange (i14)
// out (i13)


IAU.SHR.u32 i1 i14 3 || LSU0.LDIL i9, sampleApp_remainingPixels || LSU1.LDIH i9, sampleApp_remainingPixels 
LSU0.LDIL i2, sampleApp_loopAdd || LSU1.LDIH i2, sampleApp_loopAdd 
CMU.CMZ.i8 i15 || LSU0.LDIL i3 sampleApp_subtraction || LSU1.LDIH i3 sampleApp_subtraction

PEU.PC1C eq || BRU.JMP i3
CMU.CPIVR.x8 v1 i16 // repeat 'value' into v1
CMU.CPVV.u8.u16 v1 v1
CMU.CPII i11 i18
NOP 3
// else addition
sampleApp_addition:
CMU.CPII i10 i13 || LSU0.LDIL i0 0xFF || LSU1.LDIH i0 0x00
LSU0.LD.64.L v0 i11 || BRU.RPL i2 i1
NOP 6
CMU.CPVV.u8.u16 v0 v0 
VAU.ADD.i16 v2 v0 v1
sampleApp_loopAdd:
CMU.CPIVR.x16 v4 i0 // repeat '0x00FF' into v4 
CMU.CLAMP0.i16 v3 v2 v4
NOP
CMU.CPVV.u16.u8s v3 v3
LSU0.ST.64.L v3 i10
IAU.ADD i10 i10 8
IAU.ADD i11 i11 8

BRU.JMP i9
NOP 6

// subtractions
sampleApp_subtraction:
LSU0.LDIL i2, sampleApp_loopMinus || LSU1.LDIH i2, sampleApp_loopMinus
CMU.CPII i10 i13 || LSU0.LDIL i0 0xFF || LSU1.LDIH i0 0x00
LSU0.LD.64.L v0 i11 || BRU.RPL i2 i1
NOP 6
CMU.CPVV.u8.u16 v0 v0 
VAU.SUB.i16 v2 v0 v1
sampleApp_loopMinus:
CMU.CPIVR.x16 v4 i0 // repeat '0x00FF' into v4 
CMU.CLAMP0.i16 v3 v2 v4
NOP
CMU.CPVV.u16.u8s v3 v3
LSU0.ST.64.L v3 i10
IAU.ADD i10 i10 8
IAU.ADD i11 i11 8

//sampleApp_remainingPixels
sampleApp_remainingPixels:
IAU.SUB i12 i17 i14 || LSU0.LDIL i0 sampleApp_remainingPixels_loop || LSU1.LDIH i0 sampleApp_remainingPixels_loop
// avoid infinite loop
CMU.CMZ.i32 i12
PEU.PC1C eq || BRU.JMP i30
NOP 6
IAU.SHR.u32 i12 i12 3 // divide by 8
IAU.ADD i10 i18 i14
IAU.ADD i11 i13 i14
//.....
LSU0.LD.64.L v0 i10 || BRU.RPL i0 i12 
IAU.ADD i10 i10 8
NOP 5
sampleApp_remainingPixels_loop:
LSU0.ST.64.L v0 i11
NOP 5
IAU.ADD i11 i11 8
//.....
BRU.JMP i30
NOP 6


