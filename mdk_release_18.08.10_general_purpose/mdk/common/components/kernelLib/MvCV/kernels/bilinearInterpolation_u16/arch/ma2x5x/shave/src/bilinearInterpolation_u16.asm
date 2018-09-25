///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

.type mvcvBilinearInterpolation16u_asm,@function
.version 0.50.00

// i30 : return pointer
// i19 : stack pointer
// i18 : pointer to source
// i17 : step (stride) for source
// i16 : pointer to destination
// i15 : step (stride) for destination
// v23 : window size
// v22 : center coordinates

.code .text.bilinearInterpolation_u16
.lalign
mvcvBilinearInterpolation16u_asm:
CMU.CPVI.x32 i3 v23.0 		|| VAU.SHL.X32 v0 v23 0x1F 		|| LSU1.LDIL i0 biInter8 
CMU.CPVI.x32 i11 v23.1 		|| sau.sub.i16 i10 i3 0x08 		|| LSU1.LDIH i0 biInter8 
nop
CMU.CMZ.i32 v0 				|| sau.sub.i16 i10 i10 0          || LSU1.LDIL i1 0x01 // i1 =1		
PEU.PVV32 0x01 				|| VAU.SUB.f32 v22, v22, 0.5 
PEU.PC1s gte 				 	|| LSU1.LDIL i0 biInter16			
PEU.PC1s gte			 		|| LSU1.LDIH i0 biInter16 

IAU.sub i10 i3 0x10
PEU.PC1I GTE || LSU1.LDIL i0 biInter24
PEU.PC1I GTE || LSU1.LDIH i0 biInter24

cmu.cpvi.x32 i4 v22.0		|| IAU.ADD i8 i18 0x01
cmu.cpvi.x32 i5 v22.1		|| sau.frac.f32 i4 i4
sau.frac.f32 i5 i5		|| IAU.ADD i9 i8 i17 
cmu.cpiv.x32 v0.1 i4		|| lsu0.cp v0.0 i4	
cmu.cpiv.x32 v0.3 i5   		|| lsu0.cp v0.2 i5
CMU.CPIVR.i32.f32 v1 i1  
IAU.ADD i10 i16 0 // 
VAU.SUB.f32 v0 v1 v0 // v0 == 1-a // 1-b
IAU.FEXTU i3 i3 0x00 0x03 	|| LSU1.LDIL i2 0x100
IAU.SHL i3 i1 i3 
lsu0.ldil i6 256
lsu0.cp i5 v0.3				|| cmu.cpvi.x32 i4 v0.0 // i4 = 1-a//   i5 = 1-b
cmu.cpii.i32.f32 i6 i6
IAU.SUB i3 i3 0x01 			
sau.mul.f32 i7 i5 i6 //i7 = firstSum
CMU.CMASK v3 i3 0x0 
CMU.CMZ.i8 v3  
cmu.cpii.f32.i16s i14 i7
CMU.CPIVR.x16 v1 i1 
cmu.cpii.i16.f32 i7 i14
nop
sau.sub.f32 i5 i6 i7 //i5 = secondSum
nop
sau.mul.f32 i12 i4 i7	|| LSU1.LDIL i7 0x80
sau.mul.f32 i13 i4 i5	|| cmu.cpii.f32.i16s i5 i5 //i5 secondSum u16
CMU.CPIVR.x16 v12 i7	 	//v12=128
sau.add.f32 i12 i12 0.5 	//i12 = c00
sau.add.f32 i13 i13 0.5 	//i13 = c10
iau.xor i6 i6 i6 
cmu.cpii.f32.i16s i12 i12					//i4 = c01		
cmu.cpii.f32.i16s i13 i13	|| BRU.JMP i0 	//i5 = c11 
iau.sub i4 i14 i12 		
iau.sub i5 i5 i13 			|| CMU.CPVI.x32 i2 v23.1 
cmu.cpivr.x16 v9 i12
cmu.cpivr.x16 v11 i13	
cmu.cpivr.x16 v8 i4		
cmu.cpivr.x16 v10 i5	


// handle line widths up to 8 pixels
.lalign
biInter8:
// take off
cmu.cpii i13, i17            	|| LSU0.LDO.8 i4 i8 -1   			|| iau.xor i4 i4 i4 
cmu.cpii i12 i15  	     		|| LSU0.LDI.128.U8.U16 v4 i8 i13   	
IAU.SUB i11 i11 0x01         	|| LSU0.LDO.8 i5 i8 -1 			
IAU.SHR.u32 i11 i11 0x01     	|| LSU0.LDI.128.U8.U16 v5 i8 i13 				
iau.xor i5 i5 i5
LSU0.LDO.8 i6 i8 -1  
LSU1.LDIL i0 bilInter8_LoopEnd	
LSU0.LDIH i0 bilInter8_LoopEnd
LSU0.LDI.128.U8.U16 v6 i8 i13 	
nop 
CMU.SHLIV.x16 V4 v4 i4    		|| VAU.MACPZ.u16 v4 v8         
CMU.CPVV v4 v5          		|| IAU.ADD i4 i5 0     				|| VAU.MACP.u16 v4 v9
CMU.SHLIV.x16 V5 v5 i5       	|| VAU.MACP.u16 v5 v10       		|| LSU0.LDO.8 i5 i8 -1
VAU.MACP.u16 v5 v11       		|| LSU0.LDI.128.U8.U16 v5 i8 i13	|| BRU.BRA biInter8_Loop 
CMU.CPVV v5 v4    				|| VAU.MACPW.u16 v16 v12 v1   
CMU.SHLIV.x16 V5 v5 i5       	|| VAU.MACPZ.u16 v5 v8        		|| LSU0.LDO.8 i6 i8 -1
CMU.CPVV v4 v6          		|| IAU.ADD i4 i6 0     				|| VAU.MACP.u16 v5 v9 				|| LSU0.LDI.128.U8.U16 v6 i8 i13 
CMU.SHLIV.x16 V6 v6  i6         || VAU.MACP.u16 v6 v10       
VAU.MACP.u16 v6 v11
VAU.MACPW.u16 v17 v12 v1  

.lalign
biInter8_Loop:
// warp zone
.nowarn 10
CMU.SHLIV.x16 V4 v4 i4         	|| VAU.MACPZ.u16 v4 v8 				|| BRU.RPL i0 i11
CMU.CPVV v4 v5          		|| IAU.ADD i4 i5 0     				|| VAU.MACP.u16 v4 v9
CMU.SHLIV.x16 V5 v5 i5       	|| VAU.MACP.u16 v5 v10       		|| LSU0.LDO.8 i5 i8 -1
bilInter8_LoopEnd:
CMU.VSZM.BYTE  v16 v16 [Z3Z1] 	|| VAU.MACP.u16 v5 v11       		|| LSU0.LDI.128.U8.U16 v5 i8 i13
CMU.CPVV v5 v4          		|| PEU.PVL08 0x6              		|| VAU.MACPW.u16 v16 v12 v1  	 	|| LSU0.STI.128.U16.U8 v16 i10 i12
CMU.SHLIV.x16 V5 v5 i5      	|| VAU.MACPZ.u16 v5 v8        		|| LSU0.LDO.8 i6 i8 -1
CMU.CPVV v4 v6          		|| IAU.ADD i4 i6 0     				|| VAU.MACP.u16 v5 v9        		|| LSU0.LDI.128.U8.U16 v6 i8 i13 
CMU.SHLIV.x16 V6 v6 i6        	|| VAU.MACP.u16 v6 v10       
CMU.VSZM.BYTE v17 v17 [Z3Z1] 	|| VAU.MACP.u16 v6 v11
IAU.FEXTU i3 i2 0 1    			|| PEU.PVL08 0x6              		|| VAU.MACPW.u16 v17 v12 v1   		|| LSU0.STI.128.U16.U8 v17 i10 i12

// landing
PEU.PC1I 0x6 	|| BRU.JMP i30 
PEU.PC1I 0x1 	|| BRU.JMP i30 
CMU.VSZM.BYTE v16 v16 [Z3Z1] 
PEU.PVL08 0x6 	|| LSU0.STI.128.U16.U8 v16 i10 i12 
CMU.VSZM.BYTE v17 v17 [Z3Z1]
PEU.PVL08 0x6 	|| LSU0.ST.128.U16.U8 v17 i10 
nop 3


// handle line widths up to 16 pixels
.lalign
biInter16:
//take off
IAU.SUB i13 i17 0x08          	|| LSU1.LDO.8 i4 i8 -1
IAU.SUB i12 i15 0x08          	|| LSU1.LDI.128.U8.U16 v4,  i8
IAU.SUB i11 i11 0x01         	|| LSU1.LDO.8 i6 i9 -1   			
LSU1.LDI.128.U8.U16 v6 i9   	|| iau.xor i4 i4 i4 
LSU0.LDIL i0 bilInter16_LoopEnd
LSU0.LDIH i0 bilInter16_LoopEnd
LSU1.LDI.128.U8.U16 v5 i8 i13
LSU1.LDI.128.U8.U16 v7 i9 i13 


nop
cmu.cpvi.x32 i5 v4.3 
CMU.SHLIV.x16 V4 v4 i4    		|| VAU.MACPZ.u16 v4 v8       
CMU.CPVV v4 v6           		|| VAU.MACP.u16 v4 v9
cmu.cpvi.x32 i7 v4.3
CMU.SHLIV.x16 V4 v4 i6       	|| VAU.MACP.u16 v4 v10       		|| LSU0.LDO.8 i6 i9 -1
VAU.MACP.u16 v4 v11       		|| LSU0.LDI.128.U8.U16 v6 i9		|| BRU.BRA biInter16_Loop
CMU.CPVV v4 v6          		|| IAU.SHR.u32 i5 i5 0x10  			|| VAU.MACPW.u16 v16 v12 v1   
CMU.SHLIV.x16 V5 v5 i5    		|| IAU.ADD i4 i6 0     				|| VAU.MACPZ.u16 v5 v8        		|| LSU0.LDI.128.U8.U16 v7 i9 i13
CMU.CPVV v5 v7          		|| IAU.SHR.u32 i7 i7 0x10  			|| VAU.MACP.u16 v5 v9         
CMU.SHLIV.x16 V5 v5 i7          || VAU.MACP.u16 v5 v10       
VAU.MACP.u16 v5 v11
CMU.CPVV v5 v7         			|| VAU.MACPW.u16 v17 v12 v1 

.lalign
biInter16_Loop:
// warp zone
cmu.cpvi.x32 i5 v4.3			|| BRU.RPL i0 i11
CMU.SHLIV.x16 V4 v4,  i4       	|| VAU.MACPZ.u16 v4 v8        		
CMU.CPVV v4 v6               	|| VAU.MACP.u16 v4 v9
cmu.cpvi.x32 i7 v4.3
CMU.SHLIV.x16 V4 v4 i6         	|| VAU.MACP.u16 v4 v10       		|| LSU0.LDO.8 i6 i9 -1
bilInter16_LoopEnd:
CMU.VSZM.BYTE v16 v16 [Z3Z1]    || VAU.MACP.u16 v4 v11       		|| LSU0.LDI.128.U8.U16 v6 i9
.nowarnend
CMU.CPVV v4 v6          		|| IAU.SHR.u32 i5 i5 0x10  			|| VAU.MACPW.u16 v16 v12 v1   		|| LSU0.STI.128.U16.U8 v16 i10
CMU.SHLIV.x16 V5 v5 i5 			|| IAU.ADD i4 i6 0     				|| VAU.MACPZ.u16 v5 v8        		|| LSU0.LDI.128.U8.U16 v7 i9 i13
CMU.CPVV v5 v7          		|| IAU.SHR.u32 i7 i7 0x10  			|| VAU.MACP.u16 v5 v9         
CMU.SHLIV.x16 V5 v5 i7    		|| VAU.MACP.u16 v5 v10       
CMU.VSZM.BYTE v17 v17 [Z3Z1] 	|| VAU.MACP.u16 v5 v11
CMU.CPVV v5 v7          		|| PEU.PVL08 0x6  					|| VAU.MACPW.u16 v17 v12 v1   		|| LSU0.STI.128.U16.U8 v17 i10 i12  

// landing
BRU.JMP i30
CMU.VSZM.BYTE  v16, v16, [Z3Z1]
LSU0.STI.128.U16.U8 v16, i10
CMU.VSZM.BYTE  v17, v17, [Z3Z1]
PEU.PVL08 0x6 || LSU0.ST.128.U16.U8 v17, i10
nop 2




// handle line widths up to 24 pixels
.lalign
biInter24:
IAU.SUB i13 i17 0x10	|| LSU1.LDO.8 i4 i8 -1
IAU.SUB i12 i15 0x10	|| LSU1.LDI.128.U8.U16 v4,  i8
IAU.SUB i11 i11 0x01    || LSU1.LDO.8 i1 i9 -1
LSU1.LDI.128.U8.U16 v13 i9   	|| iau.xor i4 i4 i4
LSU0.LDIL i0 bilInter24_LoopEnd || IAU.XOR i2 i2 i2
LSU0.LDIH i0 bilInter24_LoopEnd || IAU.XOR i3 i3 i3
LSU1.LDI.128.U8.U16 v5 i8
nop

cmu.cpvi.x32 i5 v4.3			|| LSU1.LDI.128.U8.U16 v14 i9
CMU.SHLIV.x16 V4 v4 i4    		|| VAU.MACPZ.u16 v4 v8 || LSU1.LD.128.U8.U16 v6 i8
CMU.CPVV v4 v13           		|| VAU.MACP.u16 v4 v9
cmu.cpvi.x32 i2 v4.3			|| IAU.ADD i8 i8 i13   || LSU1.LD.128.U8.U16 v15 i9
CMU.SHLIV.x16 V4 v4 i1       	|| VAU.MACP.u16 v4 v10 || IAU.ADD i9 i9 i13
VAU.MACP.u16 v4 v11				|| IAU.SHR.u32 i5 i5 0x10
VAU.MACPW.u16 v16 v12 v1		|| IAU.SHR.u32 i2 i2 0x10  || CMU.CPVV v4 v13

cmu.cpvi.x32 i6 v5.3			|| IAU.ADD i4 i1 0
CMU.SHLIV.x16 V5 v5 i5    		|| VAU.MACPZ.u16 v5 v8    || LSU1.LDO.8 i1 i9 -1
CMU.CPVV v5 v14           		|| VAU.MACP.u16 v5 v9
CMU.CPVI.x32 i3 v5.3			|| LSU1.LDI.128.U8.U16 v13 i9
CMU.SHLIV.x16 v5 v5 i2			|| VAU.MACP.u16 v5 v10
VAU.MACP.u16 v5 v11				|| IAU.SHR.u32 i6 i6 0x10
VAU.MACPW.u16 v17 v12 v1		|| IAU.SHR.u32 i3 i3 0x10  || CMU.CPVV v5 v14

CMU.SHLIV.x16 V6 v6 i6    		|| VAU.MACPZ.u16 v6 v8	   || LSU1.LDI.128.U8.U16 v14 i9
CMU.CPVV v6 v15           		|| VAU.MACP.u16 v6 v9
CMU.SHLIV.x16 v6 v6 i3			|| VAU.MACP.u16 v6 v10
VAU.MACP.u16 v6 v11				|| LSU1.LD.128.U8.U16 v15 i9
VAU.MACPW.u16 v18 v12 v1		|| CMU.CPVV v6 v15         || IAU.ADD i9 i9 i13

.lalign
biInter24_loop:
cmu.cpvi.x32 i5 v4.3			|| BRU.RPL i0 i11
CMU.SHLIV.x16 V4 v4 i4    		|| VAU.MACPZ.u16 v4 v8
CMU.CPVV v4 v13           		|| VAU.MACP.u16 v4 v9
cmu.cpvi.x32 i2 v4.3
CMU.SHLIV.x16 V4 v4 i1       	|| VAU.MACP.u16 v4 v10
VAU.MACP.u16 v4 v11				|| IAU.SHR.u32 i5 i5 0x10  || CMU.VSZM.BYTE v16 v16 [Z3Z1]
VAU.MACPW.u16 v16 v12 v1		|| IAU.SHR.u32 i2 i2 0x10  || CMU.CPVV v4 v13	|| LSU0.STI.128.U16.U8 v16 i10

cmu.cpvi.x32 i6 v5.3			|| IAU.ADD i4 i1 0
CMU.SHLIV.x16 V5 v5 i5    		|| VAU.MACPZ.u16 v5 v8    || LSU1.LDO.8 i1 i9 -1
CMU.CPVV v5 v14           		|| VAU.MACP.u16 v5 v9
CMU.CPVI.x32 i3 v5.3			|| LSU1.LDI.128.U8.U16 v13 i9
CMU.SHLIV.x16 v5 v5 i2			|| VAU.MACP.u16 v5 v10
VAU.MACP.u16 v5 v11				|| IAU.SHR.u32 i6 i6 0x10  || CMU.VSZM.BYTE v17 v17 [Z3Z1]
bilInter24_LoopEnd:
VAU.MACPW.u16 v17 v12 v1		|| IAU.SHR.u32 i3 i3 0x10  || CMU.CPVV v5 v14	|| LSU0.STI.128.U16.U8 v17 i10

CMU.SHLIV.x16 V6 v6 i6    		|| VAU.MACPZ.u16 v6 v8	   || LSU1.LDI.128.U8.U16 v14 i9
CMU.CPVV v6 v15           		|| VAU.MACP.u16 v6 v9
CMU.SHLIV.x16 v6 v6 i3			|| VAU.MACP.u16 v6 v10
VAU.MACP.u16 v6 v11				|| LSU1.LD.128.U8.U16 v15 i9	|| CMU.VSZM.BYTE v18 v18 [Z3Z1]
VAU.MACPW.u16 v18 v12 v1		|| CMU.CPVV v6 v15         || IAU.ADD i9 i9 i13	|| LSU0.ST.128.U16.U8 v18 i10  || PEU.PVL08 0x6
IAU.ADD i10 i10 i12

// end of loop


BRU.JMP i30
CMU.VSZM.BYTE v16 v16 [Z3Z1]
LSU0.STI.128.U16.U8 v16 i10
CMU.VSZM.BYTE v17 v17 [Z3Z1]
LSU0.STI.128.U16.U8 v17 i10
CMU.VSZM.BYTE v18 v18 [Z3Z1]
LSU0.ST.128.U16.U8 v18 i10  || PEU.PVL08 0x6

.size mvcvBilinearInterpolation16u_asm,.-mvcvBilinearInterpolation16u_asm
.end



