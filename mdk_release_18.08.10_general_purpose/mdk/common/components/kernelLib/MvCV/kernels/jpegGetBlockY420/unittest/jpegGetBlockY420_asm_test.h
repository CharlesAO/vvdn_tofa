#ifndef _JPEGGetBlockY420_ASM_TEST_H_
#define _JPEGGetBlockY420_ASM_TEST_H_

extern unsigned int jpegGetBlockY420CycleCount;

void jpegGetBlockY420_asm_test(u32 mode, u8 *imginfo_Y, u32 imginfo_width,u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8]);

#endif //_JPEGGetBlockY420_ASM_TEST_H_
