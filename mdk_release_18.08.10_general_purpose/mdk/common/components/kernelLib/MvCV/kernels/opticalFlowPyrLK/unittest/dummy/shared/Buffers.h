#include <mv_types.h>

#include <Sections.h>
#include <Defines.h>

#ifdef MYRIAD2
#define DDR_BUFF DDR_DATA //DDR_DIRECT_BSS // L2 cached
#else
#define DDR_BUFF DDR_DATA //DDR_DIRECT_BSS // L2 bypass
#endif

// Add 1 pixel to the bottom and right because the tracking window might
// read an extra pixel when it is close to the border due to the bilinear
// interpolation used. The border should be initialized to 0 ideally.
u8 DDR_BUFF inputFrameA[(MAX_FRAME_WIDTH + SEARCH_WIN_SZ * 2) * MAX_FRAME_HEIGHT];
u8 DDR_BUFF pyrAL1[(MAX_FRAME_WIDTH / 2 + SEARCH_WIN_SZ * 2)  * (MAX_FRAME_HEIGHT / 2)];
u8 DDR_BUFF pyrAL2[(MAX_FRAME_WIDTH / 4 + SEARCH_WIN_SZ * 2) * (MAX_FRAME_HEIGHT / 4)];
u8 DDR_BUFF pyrAL3[(MAX_FRAME_WIDTH / 8 + SEARCH_WIN_SZ * 2) * (MAX_FRAME_HEIGHT / 8)];
u8 DDR_BUFF pyrAL4[(MAX_FRAME_WIDTH / 16 + SEARCH_WIN_SZ * 2) * (MAX_FRAME_HEIGHT / 16)];

u8 DDR_BUFF inputFrameB[(MAX_FRAME_WIDTH + SEARCH_WIN_SZ * 2) * MAX_FRAME_HEIGHT];
u8 DDR_BUFF pyrBL1[(MAX_FRAME_WIDTH / 2 + SEARCH_WIN_SZ * 2) * (MAX_FRAME_HEIGHT / 2)];
u8 DDR_BUFF pyrBL2[(MAX_FRAME_WIDTH / 4 + SEARCH_WIN_SZ * 2) * (MAX_FRAME_HEIGHT / 4)];
u8 DDR_BUFF pyrBL3[(MAX_FRAME_WIDTH / 8 + SEARCH_WIN_SZ * 2) * (MAX_FRAME_HEIGHT / 8)];
u8 DDR_BUFF pyrBL4[(MAX_FRAME_WIDTH / 16 + SEARCH_WIN_SZ * 2)* (MAX_FRAME_HEIGHT / 16)];
