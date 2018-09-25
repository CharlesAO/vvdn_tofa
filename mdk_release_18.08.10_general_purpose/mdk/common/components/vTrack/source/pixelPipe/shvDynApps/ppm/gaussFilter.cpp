#include <string.h>
#include "mv_types.h"
#include "global_constants.h"
#include "swcFrameTypes.h"
#include "pixelPipeGaussShaveHelper.h"
#include "gaussFilter.h"

inline void startGaussTaskOnMasterShave(fifoCommTask_t* unused, void* params) {
    (void)unused;
    PyrLevel(params);
}
inline void* waitGaussTaskOnMasterShave(fifoCommTask_t* unused) {
    (void)unused;
    return NULL;
}

GaussFilter::GaussFilter(
        fifoCommTask_t* _gaussTask,
        frameSpec spec,
        uint32_t num_input_images,
        u8** pyrInputLine,
        u8** buf,
        void* _helperDmaTrans
) : gaussTask(_gaussTask),
    num_inputImages(num_input_images),
    pyramidInputLineBaseAddrPtr(pyrInputLine),
    lastLineProcessedGauss(0),
    helperDmaTrans(_helperDmaTrans)

{
    uint32_t width = spec.width;
    u8* curAddr = *buf;
    for(int k = 0; k < 4; k++)
    {
        for(int i = 0; i < PP_MAX_INPUT_IMAGES; i++)
            for(int j = 0; j < GAUSS_V_SPAN; j++)
            {
                gaussBuffers[k][i][j] = curAddr;
                curAddr += (width + 2 * PADDING_H);
            }
        width = width >> 1;
    }
    *buf = curAddr;
    for (int j = 0; j < PP_MAX_INPUT_IMAGES; j++)
    {
        for (int i = 0; i < GAUSS_V_SPAN; i++)
        {
            gaussBufferPointers[j][0 * GAUSS_V_SPAN + i] = &gaussBuffers[0][j][i][PADDING_H];
            gaussBufferPointers[j][1 * GAUSS_V_SPAN + i] = &gaussBuffers[1][j][i][PADDING_H];
            gaussBufferPointers[j][2 * GAUSS_V_SPAN + i] = &gaussBuffers[2][j][i][PADDING_H];
            gaussBufferPointers[j][3 * GAUSS_V_SPAN + i] = &gaussBuffers[3][j][i][PADDING_H];
        }
    }
    for (u32 i = 0; i < num_inputImages; i++)
    {
        gaussParameters[i].pyr_width = spec.width;
        gaussParameters[i].pyr_height = spec.height;
    }
    if(gaussTask) {
        startFn = fifoCommMasterAddTask;
        waitFn = fifoCommMasterWaitTask;
    } else {
        startFn = startGaussTaskOnMasterShave;
        waitFn = waitGaussTaskOnMasterShave;
    }   
}
// Starts shave tasks for: calculating Gauss Pyramid lines and fetching the next line.
// if you do not wish to fetch the next line, the third input can be NULL
// waitPyramidLine() should be used afterwards for waiting the task completion.
void
GaussFilter::startTask(u32 line)

{
    u32 i;
    for (i = 0; i < num_inputImages; i++)
    {
        //Gauss filter of current line
        u8* line_ptr = &pyramidInputLineBaseAddrPtr[HARRIS_PADDING][PADDING_H] + i * (2 * PADDING_H + pyramidBuffer[i].pyrLevel[0].spec.width);
        gaussParameters[i].line_buff = (u8*)swcSolveShaveRelAddr(line_ptr);
        gaussParameters[i].line_cnt = line;
        u32 readBack = *(volatile u32*)(&gaussParameters[i].line_cnt);
        (void)readBack;
        startFn(gaussTask, swcSolveShaveRelAddr(&gaussParameters[i]));
    }
    lastLineProcessedGauss = line;
}
// Wait shave for finishing the Gauss task (Pyramid line computation/line fetching)
void
GaussFilter::waitTask()
{
    u32 i;
    for (i = 0; i < num_inputImages; i++)
        waitFn(gaussTask);
}

void
GaussFilter::startFrame(tvPyramidBuffer* _pyramidBuffer) {
    pyramidBuffer = _pyramidBuffer;
    for (u32 i = 0; i < num_inputImages; i++)
    {
        gaussParameters[i].pyr_out = &pyramidBuffer[i];
        gaussParameters[i].gauss_buffers = (u8**)swcSolveShaveRelAddr(gaussBufferPointers[i]);
        gaussParameters[i].gaussDmaTrans = helperDmaTrans;
    }
}

void
GaussFilter::endFrame()
{
    const u32 numPyramidLevels = pyramidBuffer[0].numPyramidLevels;
    for(uint32_t j = 0; j < (1<<(numPyramidLevels-1))-1; j++) {
        startTask(lastLineProcessedGauss + 1);
        waitTask();
    }
    //insert padding on top and bottom
    u32 i, j, k;


    for (i = 0; i < num_inputImages; i++)
    {
        for (j = 0; j < numPyramidLevels; j++)
        {

            u8* firstLine = pyramidBuffer[i].pyrLevel[j].p1;
            u32 stride = pyramidBuffer[i].pyrLevel[j].spec.stride;
            u32 height = pyramidBuffer[i].pyrLevel[j].spec.height;
            u8* lastLine = firstLine + stride * (height - 1);
            for (k = 0; k < PADDING_V; k++)
            {
                memcpy(firstLine - (k + 1) * stride, firstLine, stride);
                memcpy(lastLine + (k + 1) * stride, lastLine, stride);
            }
        }
    }
}

