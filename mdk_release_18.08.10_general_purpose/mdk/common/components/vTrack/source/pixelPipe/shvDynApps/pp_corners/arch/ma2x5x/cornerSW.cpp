#include "stdio.h"
#include "assert.h"
#include <mv_types.h>
#include "fifoCommApiDefines.h"
#include "fifoCommApi.h"
#include "pixelPipeCornerShaveHelper.h"
#include "cornerImpl.h"


MvCvFilter::MvCvFilter(fifoCommTask_t* _cornerTask, frameSpec spec, uint32_t num_input_images, u32 _maxNumFeatures, u32 _cellGridDimension, bool insertAndSort) :
    cornerTask(_cornerTask),
    num_inputImages(num_input_images),
    paddedWidth(spec.width + (PADDING_H*2)),
    height(spec.height),
    cellGridDimension(_cellGridDimension) {
    for (uint32_t i = 0; i < num_inputImages; i++) {
        cornerParameters[i].maxNumFeatures = _maxNumFeatures;
        cornerParameters[i].width = spec.width;
        cornerParameters[i].height = spec.height;
        cornerParameters[i].insertAndSort = insertAndSort;
    }
    if(cornerTask) {
        startFn = fifoCommMasterAddTask;
        waitFn = fifoCommMasterWaitTask;
    } else {
        startFn = startCornerTaskOnMasterShave;
        waitFn = waitCornerTaskOnMasterShave;
    }
    nCells=cellGridDimension*cellGridDimension;
}

// Wait shave for finishing the corner line computation.
void
MvCvFilter::waitFilter()
{

    u32 i;
    for (i = 0; i < num_inputImages; i++)
        waitFn(cornerTask);

}

void
MvCvFilter::initFrame(tvFeatureCell** _detectedFeatures, fp32* thresholds) {
    for (u32 i = 0; i < num_inputImages; i++)
    {
        cornerParameters[i].cellGridDimension = cellGridDimension;
        for(u32 j=0; j<nCells;j++){
            cornerParameters[i].thresholds[j] = thresholds[j];
        }
        cornerParameters[i].outCellData = _detectedFeatures[i];
    }
}


void
MvCvFilter::callFilter(u32 lineNo, HARRIS_OUTPUT_TYPE* (&lines)[3]) {
    u32 i;
    for (i = 0; i < num_inputImages; i++)
    {
        int offset = i*paddedWidth;
        cornerParameters[i].corners1 = (HARRIS_OUTPUT_TYPE*)swcSolveShaveRelAddr(lines[0] + offset);
        cornerParameters[i].corners2 = (HARRIS_OUTPUT_TYPE*)swcSolveShaveRelAddr(lines[1] + offset);
        cornerParameters[i].corners3 = (HARRIS_OUTPUT_TYPE*)swcSolveShaveRelAddr(lines[2] + offset);
        cornerParameters[i].lineNo   = lineNo;
        startFn(cornerTask, swcSolveShaveRelAddr(&cornerParameters[i]));
    }
    lastLinesProcessed[0]=lines[1];
    lastLinesProcessed[1]=lines[2];
}

void
MvCvFilter::endFrame() {
    HARRIS_OUTPUT_TYPE* line_ptrs[3] = {lastLinesProcessed[0], lastLinesProcessed[1], lastLinesProcessed[1]};
    callFilter(height-1, line_ptrs);
    waitFilter();
}

