.version 00.50.00

//########################################
.data MBIN_IMGS_A     0x48F00000
.align 4
//########################################

 .align 64
  mbinImgSipp:
    .incbin "output/mbin/sippTop.mbin"
    
.end
