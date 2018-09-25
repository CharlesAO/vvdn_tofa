#include <fast9ScoreCv.h>
#include <stdio.h>
#include <string.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define PADDING 8
#define TEST_FRAME_WIDTH (1920 + PADDING * 2)
#define TEST_FRAME_HEIGHT 9

u8  input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) oscore_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u8 __attribute__((section(".kept.data")))  score[TEST_FRAME_WIDTH + 4] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) score_u32postPad[4] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) base_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data"))) base[TEST_FRAME_WIDTH + 2] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) base_u32postPad[4] __attribute__ ((aligned (16)));

unsigned int width;
unsigned int tresh;
u8* inLine[TEST_FRAME_HEIGHT];
u8** inLines;
u8 inMatrix[7][48] =        //0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
{//  0   1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
    {11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
    {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
    {13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 9, 0, 9, 0, 0, 7, 0, 0, 9, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
    {14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 8, 8, 8, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 8, 9, 8, 0, 9, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
    {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 8, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 8, 8, 8, 9, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0},
    {16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0}
};

u8 bulkBuff[20*1920];
int main( void )
{

//	for(int i = 0; i < 7; i++) {
//		for(int j = 0; j < 48; j++) {
//			input[i][j] = inMatrix[i][j];
//		}
//	}
//    width = 32;
//    tresh = 1;

    for (int i = 0; i < 20* 1920; i++)
    {
        bulkBuff[i] = 0xff;
    }
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++) {
		inLine[i] = (u8*)(&input[i][PADDING]);
	}
	inLines = (u8**)inLine;

#ifdef UNIT_TEST_USE_C_VERSION
    mvcvfast9ScoreCv(inLines, score, base, tresh, width, (void*)bulkBuff);
#else
    mvcvfast9ScoreCv_asm(inLines, score, base, tresh, width, (void*)bulkBuff );
#endif

	SHAVE_HALT;
	return 0;
}
