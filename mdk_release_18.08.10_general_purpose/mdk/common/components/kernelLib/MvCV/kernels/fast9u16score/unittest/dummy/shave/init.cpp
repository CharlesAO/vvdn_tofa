#include <fast9u16score.h>
#include <stdio.h>
#include <mv_types.h>
#include <svuCommonShave.h>

#define PADDING 8
#define TEST_FRAME_WIDTH (1920 + PADDING * 2)
#define TEST_FRAME_HEIGHT 9

u8  input[TEST_FRAME_HEIGHT][TEST_FRAME_WIDTH];

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) score_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
u16 __attribute__((section(".kept.data")))  score[TEST_FRAME_WIDTH + 2] __attribute__ ((aligned (16)));
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

int main( void )
{
	for(int i = 0; i < TEST_FRAME_HEIGHT; i++)
	{
		inLine[i] = (u8*)(&input[i][PADDING]);
	}
	inLines = (u8**)inLine;
	
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvfast9u16score(inLines, score, base, tresh, width);
	#else
	mvcvfast9u16score_asm(inLines, score, base, tresh, width);
	#endif
	
	SHAVE_HALT;
	return 0;
}
