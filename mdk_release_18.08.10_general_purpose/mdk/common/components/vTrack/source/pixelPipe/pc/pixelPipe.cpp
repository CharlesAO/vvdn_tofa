///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     pixelPipeline for vtrack project
///

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "mv_types.h"
#include "mvMacros.h"
#include "pc_constants.h"

#include "opencv2/opencv.hpp"


#include "vTrack.h"
#include "vTrackHarrisResponse.h"
#include "pixelPipeHelper.h"
#include "PixelPipeApi.h"



// Structure for image grid configuration
struct CellGrid {
    cv::Rect roi;           // cell coordinates
    float fastThreshold;    // cell threshold for FAST
    u32 numberOfCorners;    // total number of corners found in cell
};


//// LOCAL PC IMPLEMENTATIONS
///////////////////////////////////////////////////////////////////////////////
//  Function to actually call FAST + Harris score
///////////////////////////////////////////////////////////////////////////////
static void pixelPipe(cv::Mat& in_img,                                      // IN
                      fp32* thresholds,                                 // IN
                      u32 fast_max_cell_cand,                              // IN
                      ppThresholds_t* thresholdCfg,                        // IN
                      CellGrid* cell,                                      // IN/OUT
                      tvFeatureCell* featureLines,                         // OUT
                      u32 nCells, u32 targetNumFeatures);                     // IN

static void createPyramid(cv::Mat image,
                          int scaledWidth,
                          int scaledHeight,
                          u32 num_pyr_levels,
                          cv::Mat **pyr);

static void keypointsInitCells(CellGrid *cell,
                               int cellWidth,
                               int cellHeight,
                                u32 cellGridDimension);

static bool cmd(const cv::KeyPoint &k1, const cv::KeyPoint &k2);

PixelPipe::PixelPipe() {
    pixelPipeParams = (pixelPipeParams_t*) shaveBuf;
    pixelPipeParams->isInitialised = false;
}

void
PixelPipe::ppInit(t_pPipeResourceCfg* vpResource, pyramidAlgoType_t pyrAlg, cornerConfig_t corCfg) {
    return initPixelPipe(vpResource, pyrAlg, corCfg);
}

void
PixelPipe::initPixelPipe(t_pPipeResourceCfg* ppResource,  pyramidAlgoType_t pyrAlg, cornerConfig_t corCfg)
{
    (void)ppResource;
    t_pPipeShaveConfig *pp_shaveCfg = ppResource->shaveConfig;

    if(pyrAlg == PYRAMID_ON)
        gaussNumShaves = 1;
    else
        gaussNumShaves = 0;
//    assert(pyrAlg == PYRAMID_ON);
//    assert(corCfg.alg == HARRIS);
    //No init required
}

u32
PixelPipe::ppRun(frameBuffer* in_img, tvFeatureCell** feature_cells,
              tvPyramidBuffer* frameBuffer,
              fp32* thresholds,
              u32 num_pyr_levels, u32 num_pyrs,
              u32 cellGridDimension, u32 maxNumFeatures,
              u32 targetNumFeatures, ppThresholds_t* thresholdCfg) {
    return pixelPipe(in_img,
                    feature_cells,
                    frameBuffer,
                    thresholds,
                    num_pyr_levels, num_pyrs,
                    cellGridDimension, maxNumFeatures,
                    targetNumFeatures,
                    thresholdCfg);
}

u32
PixelPipe::pixelPipe(frameBuffer* in_img,
              tvFeatureCell** feature_cells,
              tvPyramidBuffer* pyramidBuffer,
              fp32* thresholds,
              u32 num_pyr_levels, u32 num_pyrs,
              u32 cellGridDimension, u32 maxNumFeatures,
              u32 targetNumFeatures,
              ppThresholds_t* thresholdCfg)
{
    u32 nCells=cellGridDimension*cellGridDimension;
    cv::Mat currImage(cvSize(in_img->spec.width, in_img->spec.height), CV_8UC1, in_img->p1);
    cv::Mat* imagePyr[MAX_PYRAMID_LEVELS];
    for(int i = 0; i < num_pyr_levels; i++) {
        imagePyr[i] = new cv::Mat((in_img->spec.height >> i)+(2*PADDING_V), (in_img->spec.width >> i)+(2*PADDING_H), CV_8UC1, pyramidBuffer->pyrLevel[i].p1 - (((2*PADDING_H)+(in_img->spec.width>>i))*PADDING_V));
    }
    int cell_width, cell_height;
    CellGrid cell[nCells];

    if(gaussNumShaves)
        createPyramid(currImage, currImage.cols, currImage.rows, num_pyr_levels, imagePyr);

    // Run feature detector
    cell_width = currImage.cols / cellGridDimension;
    cell_height = currImage.rows / cellGridDimension;
    keypointsInitCells(cell, cell_width, cell_height, cellGridDimension);

    ::pixelPipe(currImage, thresholds, maxNumFeatures/nCells,
                    thresholdCfg, cell, *feature_cells, nCells, targetNumFeatures);
    for(u32 i=0; i<nCells; i++) {
         harrisThresholds[i] = thresholds[i];
    }

    // Convert pyramid to tvPyramidBuffer
    pyramidBuffer->numPyramidLevels = num_pyr_levels;
    for (int i = 0; i < pyramidBuffer->numPyramidLevels; i++) {
        pyramidBuffer->pyrLevel[i].spec.type = YUV400p;
        pyramidBuffer->pyrLevel[i].spec.height = imagePyr[i]->rows;
        pyramidBuffer->pyrLevel[i].spec.width = imagePyr[i]->cols;
        pyramidBuffer->pyrLevel[i].spec.stride = imagePyr[i]->cols;
        pyramidBuffer->pyrLevel[i].spec.bytesPP = 1;
    }
    for(int i = 0; i < num_pyr_levels; i++) {
        delete imagePyr[i];
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//  Sorting function
///////////////////////////////////////////////////////////////////////////////
static bool cmd(const cv::KeyPoint &k1, const cv::KeyPoint &k2)
{
    return k1.response > k2.response;
}

static void keypointsInitCells(CellGrid *cell,
                               int cellWidth,
                               int cellHeight,
                                u32 cellGridDimension)
{
    u32 nCells=cellGridDimension*cellGridDimension;
    for (int i = 0; i < nCells; i++)
    {
        cell[i].roi.x = cellWidth*(i%cellGridDimension);
        cell[i].roi.y = cellHeight*(i/cellGridDimension);
        cell[i].roi.width = cellWidth;
        cell[i].roi.height = cellHeight;
        // Unused
        //cell[i].numberOfCorners = 0;
        //cell[i].fastThreshold = cfgFastInit;
    }
}

static void createPyramid(cv::Mat image,
                          int scaledWidth,
                          int scaledHeight,
                          u32 num_pyr_levels,
                          cv::Mat **pyr)
{
    cv::Mat* pyr_no_border[num_pyr_levels];
    for(int i = 0; i<num_pyr_levels; i++){
        pyr_no_border[i] = new cv::Mat(scaledWidth>>i, scaledHeight>>i,CV_8UC1);
    }
    // First level of pyramid is the input image
    *pyr_no_border[0] = image;
    cv::copyMakeBorder(*pyr_no_border[0], *pyr[0], PADDING_V, PADDING_V, PADDING_H, PADDING_H, cv::BORDER_REPLICATE);
    // Construct next levels of pyramid
    for (int i = 1; i < num_pyr_levels; i++)
    {
        pyr_no_border[i]->create(scaledHeight>>i + (2*PADDING_V), scaledWidth>>i + (2*PADDING_H), CV_8UC1);
        pyrDown(*pyr_no_border[i-1], *pyr_no_border[i], cv::Size(scaledWidth>>i, scaledHeight>>i), cv::BORDER_REPLICATE);
        cv::copyMakeBorder(*pyr_no_border[i], *pyr[i], PADDING_V, PADDING_V, PADDING_H, PADDING_H, cv::BORDER_REPLICATE);
        delete pyr_no_border[i-1];
    }
    delete pyr_no_border[num_pyr_levels-1];

}


static void
pixelPipe(cv::Mat& in_img,                                      // IN
               fp32* thresholds,                                // IN/Out
               u32 fast_max_cell_cand,                              // IN
               ppThresholds_t* thresholdCfg,                        // IN
               CellGrid* cell,                                      // IN/OUT
               tvFeatureCell* featureCells,
                u32 nCells, u32 targetNumFeatures)                         // OUT
{

    // Compute Harris score; we need this to sort and filter points
    cv::Mat harrisScores(in_img.size(), CV_32F, 0.0 );
    u32 padding = ((HARRIS_KERNEL_SIZE - 1)/2);
    cv::Mat in_img_buffered(in_img.cols + 2*padding, in_img.rows + 2*padding, CV_8UC1);
    cv::copyMakeBorder(in_img, in_img_buffered, padding, padding, padding, padding, cv::BORDER_REPLICATE);
    fp32 harrisScore;
    for(int j = padding; j < in_img_buffered.rows - padding; j++){
        for(int i = padding; i < in_img_buffered.cols - padding; i++) {
            harrisScore = harrisResponse(in_img_buffered.data, i, j, in_img_buffered.cols, 0.04);
            harrisScores.at<float>(j-padding,i-padding) = harrisScore;
        }
    }

    //    cornerHarris(in_img, harrisScores, HARRIS_KERNEL_SIZE - (cfgHarrisApertureSize - 1), cfgHarrisApertureSize, 0.04);

    /// Normalizing
    cv::Mat harrisScores_norm(in_img.size(), CV_32FC1, 0.0);
    if( 1 == cfgNormaliseHarris) {
        normalize( harrisScores, harrisScores_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat() );
    }else {
        harrisScores.copyTo(harrisScores_norm);
    }

    // Detect new corners
    std::vector< std::vector<cv::KeyPoint> > keyPoints;
    std::vector<cv::KeyPoint> keyPointsCand;
    std::vector<cv::KeyPoint> keyPointsCell;

    for (int i = 0; i < nCells; i++)
    {
        // Clean previous cell corners
        keyPointsCand.clear();
        keyPointsCell.clear();

        if(cfgDoHarrisOrFast == 0) {
            // Treshold Harris score and do nonMax suppression
            for (int l = cell[i].roi.y; l < cell[i].roi.y + cell[i].roi.height; l++)
                for (int c = cell[i].roi.x; c < cell[i].roi.x + cell[i].roi.width; c++)
                {
                    cv::KeyPoint point;
                    float score = harrisScores_norm.at<float>(l, c);

                    // check score
                    if (score < thresholds[i]) {
                        continue;
                    }

                    // nonmax
                    int prevL = l - 1;
                    if (l == 0) prevL = 0;
                    int nextL = l + 1;
                    if (l == (cell[i].roi.y + cell[i].roi.height - 1)) nextL = cell[i].roi.y + cell[i].roi.height - 1;

                    int prevC = c - 1;
                    if (c == 0) prevC = c;
                    int nextC = c + 1;
                    if (c == (cell[i].roi.x + cell[i].roi.width - 1)) nextC = cell[i].roi.x + cell[i].roi.width - 1;

                    if (score <= harrisScores_norm.at<float>(prevL, prevC) ||
                                    score <= harrisScores_norm.at<float>(prevL, c) ||
                                    score <= harrisScores_norm.at<float>(prevL, nextC) ||
                                    score <= harrisScores_norm.at<float>(l, prevC) ||
                                    score <= harrisScores_norm.at<float>(l, nextC) ||
                                    score <= harrisScores_norm.at<float>(nextL, prevC) ||
                                    score <= harrisScores_norm.at<float>(nextL, c) ||
                                    score <= harrisScores_norm.at<float>(nextL, nextC))
                        continue;

                    point.pt.x = c;
                    point.pt.y = l;
                    point.response = score;
                    keyPointsCand.push_back( point );
                }

        } else {
            // Compute FAST corners current cell
            cv::Mat grayImgRoi = in_img(cell[i].roi);
            FAST(grayImgRoi, keyPointsCand, thresholds[i], true);

            // Sort corners by Harris score, and take into account ROI of each point to obtain real coordinates
            for (u32 j = 0; j < keyPointsCand.size(); j++)
            {
                keyPointsCand[j].pt.x = keyPointsCand[j].pt.x + cell[i].roi.x;
                keyPointsCand[j].pt.y = keyPointsCand[j].pt.y + cell[i].roi.y;
                keyPointsCand[j].response = harrisScores.at<float>(keyPointsCand[j].pt.y,
                                                                   keyPointsCand[j].pt.x); //3000000 ???
            }

        }

        // sort by harris score at point
        std::sort(keyPointsCand.begin(), keyPointsCand.end(), cmd);

        // Save number of corners of each cell
        u32 cellSize = keyPointsCand.size();
        if (cellSize > fast_max_cell_cand) {
            cellSize = fast_max_cell_cand;
        }

        // copy only first N_BEST cell corners
        for (u32 j = 0; j < cellSize; j++)
        {
            keyPointsCell.push_back(keyPointsCand[j]);
        }

        // Save the points to final structure
        keyPoints.push_back(keyPointsCell);
    }

    // Copy to the Movi struct
    for (u32 i = 0; i < nCells; i++) {
        featureCells[i].featureCount = keyPoints.at(i).size();
        for (u32 j = 0; j < keyPoints.at(i).size(); j++) {
            featureCells[i].features[j].x = keyPoints.at(i).at(j).pt.x;
            featureCells[i].features[j].y = keyPoints.at(i).at(j).pt.y;
            featureCells[i].features[j].harris_score = keyPoints.at(i).at(j).response;
        }
    }

    // Update Fast Thresholds
    if ((cfgDoHarrisOrFast != 1) || cfgDoFastThresholdUpdate) {
        UpdateCellThresholds(thresholds, featureCells, thresholdCfg, nCells, targetNumFeatures);

    }

    // Clean mem
    keyPoints.clear();
    keyPointsCand.clear();
    keyPointsCell.clear();
}
