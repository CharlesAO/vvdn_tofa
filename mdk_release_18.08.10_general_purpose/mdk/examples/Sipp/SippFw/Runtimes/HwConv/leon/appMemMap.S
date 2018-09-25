.global mbinImgSipp

!;#####################################################
.section ".data"
.align 4
mbinImgSipp:
 .incbin "output/svuSippImg.shvdlib"

!;#####################################################
.section ".shv0.S.init"
cmxInits1:
 .fill (131072), 1, 0xCD
.section ".shv1.S.init"
cmxInits2:
 .fill (131072), 1, 0xCD
.section ".shv2.S.init"
cmxInits3:
 .fill (131072), 1, 0xCD
.section ".shv3.S.init"
cmxInits4:
 .fill (131072), 1, 0xCD
.section ".shv4.S.init"
cmxInits5:
 .fill (131072), 1, 0xCD
.section ".shv5.S.init"
cmxInits6:
 .fill (131072), 1, 0xCD
.section ".shv6.S.init"
cmxInits7:
 .fill (131072), 1, 0xCD
.section ".shv7.S.init"
cmxInits8:
 .fill (131072), 1, 0xCD
