.global mbinImgSipp

!;#####################################################
.section ".data"
.align 4
mbinImgSipp:
 .incbin "output/svuSippImg.shvdlib"

!;#####################################################

!; JPEG DECODER used shaves
.section ".shv0.S.init"
cmxInits0:
 .fill (131072), 1, 0xCD
.section ".shv1.S.init"
cmxInits1:
 .fill (131072), 1, 0xCD
.section ".shv2.S.init"
cmxInits2:
 .fill (131072), 1, 0xCD
.section ".shv3.S.init"
cmxInits3:
 .fill (131072), 1, 0xCD
.section ".shv4.S.init"
cmxInits4:
 .fill (131072), 1, 0xCD

!; SIPP pipline used shaves
.section ".shv5.S.init"
cmxInits5:
 .fill (131072), 1, 0xCD
.section ".shv6.S.init"
cmxInits6:
 .fill (131072), 1, 0xCD
.section ".shv7.S.init"
cmxInits7:
 .fill (131072), 1, 0xCD
.section ".shv8.S.init"
cmxInits8:
 .fill (131072), 1, 0xCD
.section ".shv9.S.init"
cmxInits9:
 .fill (131072), 1, 0xCD
.section ".shv10.S.init"
cmxInits10:
 .fill (131072), 1, 0xCD
