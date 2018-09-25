#ifndef __JPEG_DEFINES_H__
#define __JPEG_DEFINES_H__

#define SHAVE_OUTPUT_BUFFER_SIZE 7*1024
#define SHAVE_MAX_NO 12
#define MAX_RST_MARKERS 8
#define INDEX_NOT_FOUND 999
#define JPG_NO_FREE_SHAVE 13
#define JPG_SHAVE_IDLE 0
#define JPG_SHAVE_BUSY 1
#define JPG_SHAVE_STOPPED 2

typedef struct{
    unsigned int maskX;
    unsigned int maskY;
    unsigned int blockW;
    int blockH;
    unsigned int lumaBufferSize;
    unsigned int chromaBufferSize;
    unsigned short subsampling;
    unsigned int getBlockCoefX;
    unsigned int getBlockCoefY;
    unsigned int blockSize;
    unsigned int lumaValuesPerChroma;
    int maxBlocksPerShv;
} formatSpecific;

typedef struct
{
    unsigned char first;
    unsigned char last;
    unsigned short restartMarker;
    unsigned int currentShave;
    unsigned int shavesNo;
    unsigned int inputBuffSizePerShave;
    dmaTransactionList_t* task1;
    dmaTransactionList_t* task2;
    dmaTransactionList_t* task3;
    volatile dmaTransactionList_t* *ref1;
    volatile dmaTransactionList_t* *ref2;
    volatile dmaTransactionList_t* *ref3;
} shaveinfotype;

typedef struct
{
    unsigned char outCMXdata[SHAVE_OUTPUT_BUFFER_SIZE];
    unsigned char used;
    unsigned char done;
    int sliceNo;
    int sliceSize;
    int shaveNo;
} jpegCMXoutput;

typedef void (*fp);

#endif //__JPEG_DEFINES_H__
