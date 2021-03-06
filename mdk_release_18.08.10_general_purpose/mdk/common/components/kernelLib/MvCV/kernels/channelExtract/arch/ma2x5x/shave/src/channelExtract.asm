///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvChannelExtract_asm,@function
.version 00.51.04

.data .data.channelExtract
.align 4

.code .text.channelExtract

mvcvChannelExtract_asm:
.nowarn
LSU1.LD.32  i18  i18 || LSU0.LD.32  i17  i17
LSU0.LDIL i9 0x30 || LSU1.LDIH i9 0x0 || IAU.SUB i10 i10 i10 //48 pixels processed & contor for loop

//void channelExtract(u8** in, u8** out, u32 width, u8 plane)
//                       i18     i17       i16        i15     

LSU0.LDIL i1 channelExtract_loopR || LSU1.LDIH i1 channelExtract_loopR //loop for R-plane
LSU0.LDIL i2 channelExtract_loopG || LSU1.LDIH i2 channelExtract_loopG //loop for G-plane
LSU0.LDIL i3 channelExtract_loopB || LSU1.LDIH i3 channelExtract_loopB //loop for B-plane

LSU0.LDIL i4 0x0780 || LSU1.LDIH i4 0 //load max width
VAU.SUB.i8  v20 v20 v20

LSU0.LDIL i13 channelExtract_exit || LSU1.LDIH i13 channelExtract_exit //if plane number is greater then 2
LSU0.LDIL i6 0x0 || LSU1.LDIH i8 0x0 //compare element for R-plane
LSU0.LDIL i7 0x1 || LSU1.LDIH i7 0x0 //compare element for G-plane
LSU0.LDIL i8 0x2 || LSU1.LDIH i8 0x0 || IAU.SUB i19 i19 4//compare element for B-plane


LSU0.ST.32  i20  i19    || IAU.SUB i19 i19 4
LSU0.ST.32  i21  i19    || IAU.SUB i19 i19 4
LSU0.ST.32  i22  i19    || IAU.SUB i19 i19 4
LSU0.ST.32  i23  i19    || IAU.SUB i19 i19 4
LSU0.ST.32  i24  i19    || IAU.SUB i19 i19 4
LSU0.ST.32  i25  i19    || IAU.SUB i19 i19 4
LSU0.ST.32  i26  i19    

IAU.SUB i19 i19 i9 // making space for pixels compensation
IAU.SUB i11 i19 0  //making copy for stack adress
IAU.ADD i10 i10 i9



LSU0.LDIL i20 channelExtract_loopRR || LSU1.LDIH i20 channelExtract_loopRR || CMU.CMII.i32 i15 i8 // compensation loop for R-plane
PEU.PC1C GT || BRU.JMP i13
LSU0.LDIL i21 channelExtract_loopGG || LSU1.LDIH i21 channelExtract_loopGG // compensation loop for G-plane
LSU0.LDIL i22 channelExtract_loopBB || LSU1.LDIH i22 channelExtract_loopBB // compensation loop for B-plane
LSU0.LDIL i23 channelExtract_loopRRR || LSU1.LDIH i23 channelExtract_loopRRR // compensation loop for R-plane
LSU0.LDIL i24 channelExtract_loopGGG || LSU1.LDIH i24 channelExtract_loopGGG // compensation loop for G-plane
LSU0.LDIL i25 channelExtract_loopBBB || LSU1.LDIH i25 channelExtract_loopBBB // compensation loop for B-plane
LSU0.LDIL i26 0x31 || LSU1.LDIH i26 0x0 //49




CMU.CMII.i32 i15 i6
PEU.PC1C LT || BRU.JMP i13
CMU.CMII.i32 i7 i15
PEU.PC1C EQ || BRU.JMP i2
CMU.CMII.i32 i8 i15
PEU.PC1C EQ || BRU.JMP i3
nop 6

//code part for R-plane
 channelExtract_loopR:
CMU.CMII.i32 i16 i26
PEU.PC1C LT || BRU.JMP i23

nop 6
.lalign
 
LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8   
nop

channelExtract_loopRR:
IAU.ADD i10 i10 i9    
VAU.ADD.i16 v7   v1 v20 || LSU1.SWZV8  [03623630]  || LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8     || CMU.CMII.i32 i10 i16          
VAU.ADD.i16 v8   v2 v20 || LSU1.SWZV8  [03623741]  || LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8            
VAU.ADD.i16 v9   v3 v20 || LSU1.SWZV8  [03623752]  || LSU0.ST.128.U16.U8  v7  i17 || IAU.ADD i17 i17 3                  
VAU.ADD.i16 v10  v4 v20 || LSU1.SWZV8  [03623630]  || LSU0.ST.128.U16.U8  v8  i17 || IAU.ADD i17 i17 3                  
VAU.ADD.i16 v11  v5 v20 || LSU1.SWZV8  [03623741]  || LSU0.ST.128.U16.U8  v9  i17 || IAU.ADD i17 i17 2                  
PEU.PC1C LT || BRU.JMP i20
VAU.ADD.i16 v12  v6 v20 || LSU1.SWZV8  [03623752]  || LSU0.ST.128.U16.U8  v10 i17 || IAU.ADD i17 i17 3
LSU0.ST.128.U16.U8  v11 i17 || IAU.ADD i17 i17 3 || LSU1.LD.128.U8.U16  v3  i18 || SAU.ADD.i32 i18 i18 8
LSU0.ST.128.U16.U8  v12 i17 || IAU.ADD i17 i17 2
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8



iau.sub i18 i18 48


 channelExtract_loopRRR:
LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8   
nop


VAU.ADD.i16 v7   v1 v20 || LSU1.SWZV8  [03623630]                     
VAU.ADD.i16 v8   v2 v20 || LSU1.SWZV8  [03623741]                     
VAU.ADD.i16 v9   v3 v20 || LSU1.SWZV8  [03623752]  || LSU0.ST.128.U16.U8  v7  i11 || IAU.ADD i11 i11 3                  
VAU.ADD.i16 v10  v4 v20 || LSU1.SWZV8  [03623630]  || LSU0.ST.128.U16.U8  v8  i11 || IAU.ADD i11 i11 3                  
VAU.ADD.i16 v11  v5 v20 || LSU1.SWZV8  [03623741]  || LSU0.ST.128.U16.U8  v9  i11 || IAU.ADD i11 i11 2                  
VAU.ADD.i16 v12  v6 v20 || LSU1.SWZV8  [03623752]  || LSU0.ST.128.U16.U8  v10 i11 || IAU.ADD i11 i11 3
                                                      LSU0.ST.128.U16.U8  v11 i11 || IAU.ADD i11 i11 3
                                                      LSU0.ST.128.U16.U8  v12 i11 || IAU.ADD i11 i11 2

IAU.SUB i14 i10 i9
IAU.SUB i5 i16 i14
CMU.CMII.i32 i16 i26 //see if the width is smaller then 49
PEU.PC1C LT || IAU.SUB i5 i16 0

IAU.SUB i11 i19 0
LSU0.LDIL i1 channelExtract_compensation || LSU1.LDIH i1 channelExtract_compensation //loop for R-plane


LSU0.LD.8    i12  i11  || IAU.ADD i11 i11 1 || SAU.SUB.i32 i10 i10 i10
 nop 2
 IAU.ADD i10 i10 3
 CMU.CMII.i32 i10 i5
 channelExtract_compensation:
PEU.PC1C LT || BRU.JMP i1
LSU0.LD.8    i12  i11  || IAU.ADD i11 i11 1
LSU0.ST.8    i12  i17  || IAU.ADD i17 i17 1 
IAU.ADD i10 i10 3
CMU.CMII.i32 i10 i5
nop 2




IAU.ADD i19 i19 i9
LSU0.LD.32  i26  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i25  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i24  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19  || IAU.ADD i19 i19 4
BRU.jmp i30
nop 5



//code part for G-plane
 channelExtract_loopG:
CMU.CMII.i32 i16 i26
PEU.PC1C LT || BRU.JMP i24
nop 6



LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8 
LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8 
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8 
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8 
nop
 
 
 
 channelExtract_loopGG:  
 IAU.ADD i10 i10 i9    
VAU.ADD.i16 v7   v1 v20 || LSU1.SWZV8  [03623741]    || LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8      || CMU.CMII.i32 i10 i16     
VAU.ADD.i16 v8   v2 v20 || LSU1.SWZV8  [03623752]    || LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8         
VAU.ADD.i16 v9   v3 v20 || LSU1.SWZV8  [03623630]    ||  LSU0.ST.128.U16.U8  v7  i17 || IAU.ADD i17 i17 3                
VAU.ADD.i16 v10  v4 v20 || LSU1.SWZV8  [03623741]    ||  LSU0.ST.128.U16.U8  v8  i17 || IAU.ADD i17 i17 2               
VAU.ADD.i16 v11  v5 v20 || LSU1.SWZV8  [03623752]    ||  LSU0.ST.128.U16.U8  v9  i17 || IAU.ADD i17 i17 3               
PEU.PC1C LT || BRU.JMP i21
VAU.ADD.i16 v12  v6 v20 || LSU1.SWZV8  [03623630]    ||  LSU0.ST.128.U16.U8  v10 i17 || IAU.ADD i17 i17 3
LSU0.ST.128.U16.U8  v11 i17 || IAU.ADD i17 i17 2
LSU0.ST.128.U16.U8  v12 i17 || sAU.ADD.i32 i17 i17 3 || LSU1.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8 
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8 
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8 
    
  


iau.sub i18 i18 48


 channelExtract_loopGGG:
LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8   
nop


VAU.ADD.i16 v7   v1 v20 || LSU1.SWZV8  [03623741]                   
VAU.ADD.i16 v8   v2 v20 || LSU1.SWZV8  [03623752]                     
VAU.ADD.i16 v9   v3 v20 || LSU1.SWZV8  [03623630]   || LSU0.ST.128.U16.U8  v7  i11 || IAU.ADD i11 i11 3                 
VAU.ADD.i16 v10  v4 v20 || LSU1.SWZV8  [03623741]   || LSU0.ST.128.U16.U8  v8  i11 || IAU.ADD i11 i11 2                 
VAU.ADD.i16 v11  v5 v20 || LSU1.SWZV8  [03623752]   || LSU0.ST.128.U16.U8  v9  i11 || IAU.ADD i11 i11 3                 
VAU.ADD.i16 v12  v6 v20 || LSU1.SWZV8  [03623630]   || LSU0.ST.128.U16.U8  v10 i11 || IAU.ADD i11 i11 3
                                                    LSU0.ST.128.U16.U8  v11 i11 || IAU.ADD i11 i11 2
                                                    LSU0.ST.128.U16.U8  v12 i11 || IAU.ADD i11 i11 3


IAU.SUB i14 i10 i9
IAU.SUB i5 i16 i14
CMU.CMII.i32 i16 i26 //see if the width is smaller then 49
PEU.PC1C LT || IAU.SUB i5 i16 0

IAU.SUB i11 i19 0
LSU0.LDIL i1 channelExtract_compensationG || LSU1.LDIH i1 channelExtract_compensationG //loop for R-plane
IAU.SUB i10 i10 i10


LSU0.LD.8    i12  i11  || IAU.ADD i11 i11 1
nop 2
IAU.ADD i10 i10 3
CMU.CMII.i32 i10 i5
channelExtract_compensationG:
PEU.PC1C LT || BRU.JMP i1
LSU0.LD.8    i12  i11  || IAU.ADD i11 i11 1 
LSU0.ST.8    i12  i17  || IAU.ADD i17 i17 1 
IAU.ADD i10 i10 3
CMU.CMII.i32 i10 i5
nop 2

IAU.ADD i19 i19 i9
LSU0.LD.32  i26  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i25  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i24  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19  || IAU.ADD i19 i19 4
BRU.jmp i30
nop 5




//code part for B-plane
 channelExtract_loopB:
CMU.CMII.i32 i16 i26
PEU.PC1C LT || BRU.JMP i25
nop 6

 

LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8   
nop

channelExtract_loopBB:
IAU.ADD i10 i10 i9               
VAU.ADD.i16 v7   v1 v20 || LSU1.SWZV8  [03623652] || CMU.CMII.i32 i10 i16    || LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8                  
VAU.ADD.i16 v8   v2 v20 || LSU1.SWZV8  [03623630]                            || LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8
VAU.ADD.i16 v9   v3 v20 || LSU1.SWZV8  [03623741] || LSU0.ST.128.U16.U8  v7  i17 || IAU.ADD i17 i17 2                   
VAU.ADD.i16 v10  v4 v20 || LSU1.SWZV8  [03623652] || LSU0.ST.128.U16.U8  v8  i17 || IAU.ADD i17 i17 3                   
VAU.ADD.i16 v11  v5 v20 || LSU1.SWZV8  [03623630] || LSU0.ST.128.U16.U8  v9  i17 || IAU.ADD i17 i17 3                   
PEU.PC1C LT || BRU.JMP i22
VAU.ADD.i16 v12  v6 v20 || LSU1.SWZV8  [03623741] || LSU0.ST.128.U16.U8  v10 i17 || IAU.ADD i17 i17 2
LSU0.ST.128.U16.U8  v11 i17 || IAU.ADD i17 i17 3
LSU0.ST.128.U16.U8  v12 i17 || SAU.ADD.i32 i17 i17 3 || LSU1.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8  


IAU.sub i18 i18 48

 channelExtract_loopBBB:
LSU0.LD.128.U8.U16  v1  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v2  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v3  i18 || IAU.ADD i18 i18 8
LSU0.LD.128.U8.U16  v4  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v5  i18 || IAU.ADD i18 i18 8   
LSU0.LD.128.U8.U16  v6  i18 || IAU.ADD i18 i18 8   
nop


VAU.ADD.i16 v7   v1 v20 || LSU1.SWZV8  [03623652]                    
VAU.ADD.i16 v8   v2 v20 || LSU1.SWZV8  [03623630]                     
VAU.ADD.i16 v9   v3 v20 || LSU1.SWZV8  [03623741]   || LSU0.ST.128.U16.U8  v7  i11 || IAU.ADD i11 i11 2                 
VAU.ADD.i16 v10  v4 v20 || LSU1.SWZV8  [03623652]   || LSU0.ST.128.U16.U8  v8  i11 || IAU.ADD i11 i11 3                 
VAU.ADD.i16 v11  v5 v20 || LSU1.SWZV8  [03623630]   || LSU0.ST.128.U16.U8  v9  i11 || IAU.ADD i11 i11 3                 
VAU.ADD.i16 v12  v6 v20 || LSU1.SWZV8  [03623741]   || LSU0.ST.128.U16.U8  v10 i11 || IAU.ADD i11 i11 2
                                                    LSU0.ST.128.U16.U8  v11 i11 || IAU.ADD i11 i11 3
                                                    LSU0.ST.128.U16.U8  v12 i11 || IAU.ADD i11 i11 3







IAU.SUB i14 i10 i9
IAU.SUB i5 i16 i14
CMU.CMII.i32 i16 i26 //see if the width is smaller then 49
PEU.PC1C LT || IAU.SUB i5 i16 0


IAU.SUB i11 i19 0
LSU0.LDIL i1 channelExtract_compensationB || LSU1.LDIH i1 channelExtract_compensationB //loop for R-plane
IAU.SUB i10 i10 i10


LSU0.LD.8    i12  i11  || IAU.ADD i11 i11 1
nop 2
IAU.ADD i10 i10 3
CMU.CMII.i32 i10 i5
 channelExtract_compensationB:
PEU.PC1C LT || BRU.JMP i1
LSU0.LD.8    i12  i11  || IAU.ADD i11 i11 1
LSU0.ST.8    i12  i17  || IAU.ADD i17 i17 1 
IAU.ADD i10 i10 3
CMU.CMII.i32 i10 i5
nop 2

 channelExtract_exit:
IAU.ADD i19 i19 i9
LSU0.LD.32  i26  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i25  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i24  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i23  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i22  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i21  i19  || IAU.ADD i19 i19 4
LSU0.LD.32  i20  i19  || IAU.ADD i19 i19 4
BRU.jmp i30
nop 5



.nowarnend
.size mvcvChannelExtract_asm,.-mvcvChannelExtract_asm
.end
