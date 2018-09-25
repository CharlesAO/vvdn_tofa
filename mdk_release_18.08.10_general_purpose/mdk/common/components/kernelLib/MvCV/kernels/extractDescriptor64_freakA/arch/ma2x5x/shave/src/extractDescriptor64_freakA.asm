.type mvcvExtractDescriptor64_freakA_asm,@function
.version 00.70.00

.data .data.extractDescriptor64_freakA_asm:
.align 16
    _mask64_freakA:
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
        .byte 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00
        .byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
        .byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
        .byte 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x40
        .byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x02, 0x00, 0x40, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
        .byte 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
        .byte 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x10, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
        .byte 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x04, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
        .byte 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20
        .byte 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80
        .byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
        .byte 0x00, 0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00
        .byte 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
        .byte 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00
        .byte 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01
        .byte 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
        .byte 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00
        .byte 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08
        .byte 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
        .byte 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04
        .byte 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40
        .byte 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00
        .byte 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00
        .byte 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00
        .byte 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00

// Execution cycles: 550 cycles
// Memory usage:     3952 bytes
.code .text.extractDescriptor64_freakA_asm
.align 16
//void extractDescriptor64_freakA_asm(unsigned short *in_pixel_array(i18), u8* out_descriptor(i17))
mvcvExtractDescriptor64_freakA_asm:

// irf used
.set addrValues0     i18
.set addrDescriptor  i17
.set iDesc0          i0
.set iDesc1          i1
.set iDesc2          i2
.set iDesc3          i3
.set addrValues1     i4
.set addrMask        i5
.set incr            i6

// vrf used
.set vMask           v15
.set vDESC0          v16
.set vDESC1          v17
.set vDESC2          v18
.set vDESC3          v19
.set vDESC4          v20
.set vDESC5          v21
.set vDESC6          v22
.set vDESC7          v23

LSU0.LDIL addrMask, _mask64_freakA || LSU1.LDIH addrMask, _mask64_freakA || IAU.ADDSU addrValues1, addrValues0, 8 || CMU.CPZV vDESC0, 3
LSU0.LDIL incr, 16 || CMU.CPZV vDESC4, 3
LSU0.LDI.64.l v0, addrValues0, incr || LSU1.LDI.64.h v0, addrValues1, incr
LSU0.LDI.64.l v1, addrValues0, incr || LSU1.LDI.64.h v1, addrValues1, incr
LSU0.LDI.64.l v2, addrValues0, incr || LSU1.LDI.64.h v2, addrValues1, incr
LSU0.LDI.64.l v3, addrValues0, incr || LSU1.LDI.64.h v3, addrValues1, incr
LSU0.LDI.64.l v4, addrValues0, incr || LSU1.LDI.64.h v4, addrValues1, incr
LSU0.LDI.64.l v5, addrValues0, incr || LSU1.LDI.64.h v5, addrValues1, incr
LSU1.LDI.128.u8.u16 vMask, addrMask
LSU1.LDI.128.u8.u16 vMask, addrMask
LSU1.LDI.128.u8.u16 vMask, addrMask
LSU1.LDI.128.u8.u16 vMask, addrMask
LSU1.LDI.128.u8.u16 vMask, addrMask
LSU1.LDI.128.u8.u16 vMask, addrMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [11111111]
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 LTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v1, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v2, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 LTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v1, v3 || LSU0.SWZC8 [44444444] || PEU.PVV16 LTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D] || VAU.XOR vDESC1, vDESC1, vDESC1
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0 || VAU.XOR vDESC0, vDESC0, vDESC0
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2 || VAU.XOR vDESC3, vDESC3, vDESC3
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || VAU.XOR vDESC2, vDESC2, vDESC2
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CPZV vDESC4, 3 || SAU.SUMX.u16 iDesc3, vDESC6
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc0, addrDescriptor || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc1, addrDescriptor || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc2, addrDescriptor || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc3, addrDescriptor || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v4 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D] || VAU.XOR vDESC1, vDESC1, vDESC1
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0 || VAU.XOR vDESC0, vDESC0, vDESC0
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2 || VAU.XOR vDESC3, vDESC3, vDESC3
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || VAU.XOR vDESC2, vDESC2, vDESC2
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CPZV vDESC4, 3 || SAU.SUMX.u16 iDesc3, vDESC6
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v3 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v4 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v5 || LSU0.SWZC8 [11111111] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc0, addrDescriptor || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc1, addrDescriptor || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc2, addrDescriptor || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc3, addrDescriptor || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D] || VAU.XOR vDESC1, vDESC1, vDESC1
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0 || VAU.XOR vDESC0, vDESC0, vDESC0
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2 || VAU.XOR vDESC3, vDESC3, vDESC3
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || VAU.XOR vDESC2, vDESC2, vDESC2
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CPZV vDESC4, 3 || SAU.SUMX.u16 iDesc3, vDESC6
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v5 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v3 || LSU0.SWZC8 [33333333] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc0, addrDescriptor || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc1, addrDescriptor || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc2, addrDescriptor || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc3, addrDescriptor || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D] || VAU.XOR vDESC1, vDESC1, vDESC1
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0 || VAU.XOR vDESC0, vDESC0, vDESC0
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2 || VAU.XOR vDESC3, vDESC3, vDESC3
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || VAU.XOR vDESC2, vDESC2, vDESC2
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CPZV vDESC4, 3 || SAU.SUMX.u16 iDesc3, vDESC6
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v5 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v4 || LSU0.SWZC8 [66666666] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 LTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 LTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v3 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 LTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v4 || LSU0.SWZC8 [66666666] || PEU.PVV16 LTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v4 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc0, addrDescriptor || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc1, addrDescriptor || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc2, addrDescriptor || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc3, addrDescriptor || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D] || VAU.XOR vDESC1, vDESC1, vDESC1
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0 || VAU.XOR vDESC0, vDESC0, vDESC0
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2 || VAU.XOR vDESC3, vDESC3, vDESC3
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || VAU.XOR vDESC2, vDESC2, vDESC2
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CPZV vDESC4, 3 || SAU.SUMX.u16 iDesc3, vDESC6
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 LTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v4 || LSU0.SWZC8 [55555555] || PEU.PVV16 LTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 LTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 LTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v5 || LSU0.SWZC8 [33333333] || PEU.PVV16 LTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc0, addrDescriptor || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc1, addrDescriptor || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc2, addrDescriptor || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc3, addrDescriptor || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D] || VAU.XOR vDESC1, vDESC1, vDESC1
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0 || VAU.XOR vDESC0, vDESC0, vDESC0
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2 || VAU.XOR vDESC3, vDESC3, vDESC3
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || VAU.XOR vDESC2, vDESC2, vDESC2
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CPZV vDESC4, 3 || SAU.SUMX.u16 iDesc3, vDESC6
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v4 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v3 || LSU0.SWZC8 [33333333] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc0, addrDescriptor || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc1, addrDescriptor || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc2, addrDescriptor || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc3, addrDescriptor || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D] || VAU.XOR vDESC1, vDESC1, vDESC1
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0 || VAU.XOR vDESC0, vDESC0, vDESC0
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2 || VAU.XOR vDESC3, vDESC3, vDESC3
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || VAU.XOR vDESC2, vDESC2, vDESC2
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CPZV vDESC4, 3 || SAU.SUMX.u16 iDesc3, vDESC6
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v5, v4 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC0, vDESC0, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v5 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v3 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v4 || LSU0.SWZC8 [44444444] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v4 || LSU0.SWZC8 [44444444] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 LTE || VAU.OR vDESC1, vDESC1, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v3 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC2, vDESC2, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v4 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v3 || LSU0.SWZC8 [77777777] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v5 || LSU0.SWZC8 [22222222] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v1 || LSU0.SWZC8 [44444444] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 LTE || VAU.OR vDESC3, vDESC3, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v0 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC4, vDESC4, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v1 || LSU0.SWZC8 [33333333] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v1, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC5, vDESC5, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v3 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v0, v0 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [44444444] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v2, v0 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v4, v4 || LSU0.SWZC8 [55555555] || PEU.PVV16 GTE || VAU.OR vDESC6, vDESC6, vMask
LSU1.LDI.128.u8.u16 vMask, addrMask || CMU.CMVV.u16 v3, v1 || LSU0.SWZC8 [00000000] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v3, v0 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [11111111] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc0, addrDescriptor || CMU.CMVV.u16 v4, v1 || LSU0.SWZC8 [22222222] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc1, addrDescriptor || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [66666666] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc2, addrDescriptor || CMU.CMVV.u16 v2, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
LSU1.STI.16 iDesc3, addrDescriptor || CMU.CMVV.u16 v3, v2 || LSU0.SWZC8 [77777777] || PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
PEU.PVV16 GTE || VAU.OR vDESC7, vDESC7, vMask
CMU.VSZM.BYTE vDESC0, vDESC1, [2D0D]
CMU.VSZM.BYTE vDESC2, vDESC3, [2D0D] || SAU.SUMX.u16 iDesc0, vDESC0
CMU.VSZM.BYTE vDESC4, vDESC5, [2D0D] || SAU.SUMX.u16 iDesc1, vDESC2
CMU.VSZM.BYTE vDESC6, vDESC7, [2D0D] || SAU.SUMX.u16 iDesc2, vDESC4 || LSU1.STI.16 iDesc0, addrDescriptor
SAU.SUMX.u16 iDesc3, vDESC6 || LSU1.STI.16 iDesc1, addrDescriptor
LSU1.STI.16 iDesc2, addrDescriptor
LSU1.STI.16 iDesc3, addrDescriptor
BRU.JMP i30
NOP 6
.size mvcvExtractDescriptor64_freakA_asm,.-mvcvExtractDescriptor64_freakA_asm
.end
