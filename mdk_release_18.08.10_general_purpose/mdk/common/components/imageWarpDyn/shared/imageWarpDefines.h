#ifndef __IMAGE_WARP_DEFINES_H__
#define __IMAGE_WARP_DEFINES_H__

#define MESH_CELL_SIZE 16
#define OUT_TILE_WIDTH 16
#define OUT_TILE_HEIGHT 16

#define OUT_TILE_SIZE (OUT_TILE_WIDTH * OUT_TILE_HEIGHT * sizeof(char))
#define TILES_AT_LINE 2
#define TILES_AT_ONCE (TILES_AT_LINE * TILES_AT_LINE)
#define CMX_DMA __attribute__((section(".cmx.cdmaDescriptors")))
#define CMX_MESH __attribute__((section("cmx.cmx_other")))
#define HORIZONTAL_PAD 16 // horizontal padding in PIXELS
#define VERTICAL_PAD 24  // vertical padding in LINES
#define CMX_BUFFER_LINES 25
#define CMX_BUFFER_SIZE ((CMX_BUFFER_LINES + 2) * (1920 + 2 * HORIZONTAL_PAD))

#define CIRCULAR_BUFFER_SIZE (CMX_BUFFER_LINES + 2 * VERTICAL_PAD)

#define MESH_LUT_LENGTH 1080


typedef struct
{
    unsigned int dstOffsetDDR;
    unsigned char* cmxInBuffP[CIRCULAR_BUFFER_SIZE];
    unsigned char* swapP;

    unsigned char* cmxOutBuff;
#ifdef __MOVICOMPILE__
    half4 xCoords;
    half4 yCoords;
#else
    fp16 xCoords[4];
    fp16 yCoords[4];
#endif
    int adjustment;
    int isInsideImg;

} reTileEntry;

typedef struct tileList
{
    struct tileList* next;
    u8 x;
    u8 y;
}tileList;

typedef struct
{
    unsigned int meshWidth;
    unsigned int meshHeight;
    float* meshX;
    float* meshY;
} meshStruct;
// Algorithm parameters

#endif