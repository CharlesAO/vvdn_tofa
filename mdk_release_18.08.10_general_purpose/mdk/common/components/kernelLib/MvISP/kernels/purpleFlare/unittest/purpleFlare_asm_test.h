#ifndef _PURPLEFLARE_ASM_TEST_H_
#define _PURPLEFLARE_ASM_TEST_H_

extern unsigned int PurpleFlareCycleCount;

void purpleFlare_asm_test(unsigned short *inGreen, unsigned short *blue[3], unsigned short *outBlue, unsigned char strength, unsigned int width);

#endif //_PURPLEFLARE_ASM_TEST_H_
