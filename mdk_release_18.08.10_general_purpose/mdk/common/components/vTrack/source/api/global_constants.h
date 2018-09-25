///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file is to be used for all the parameters that need to be known at compile time.
///            These are generally parameters that affect buffer sizes, etc.
///
#ifndef GLOBAL_PARAMETERS_H_
#define GLOBAL_PARAMETERS_H_
#pragma once

#define ALIGNED(x) __attribute__((aligned(x)))

//Macros that affect sizes in vTrack (constraints of vTrack)
#define MAX_FRAME_WIDTH 1280

//TODO: currently the buffer sizes will be wrong if PP_MAX_INPUT_IMAGES is changed
#ifndef PP_MAX_INPUT_IMAGES
#define PP_MAX_INPUT_IMAGES 1 //by default there should be only 1 input image
#endif

//cell management---------------------------------
/// If this is changed, update MAX_FEAT_ID_DISTINGUISHER_OFFSET also
#define MAX_CELL_GRID_DIMENSION     4
#define MAX_NUM_CELLS	            (MAX_CELL_GRID_DIMENSION * MAX_CELL_GRID_DIMENSION)
/// every cell will produce specific feature ids by adding the cell id to the LSBs of the feature id.
/// This offset determines the number of LSBs to use
//  ** this should be log2(N_CELLS) rounded up to nearest integer **
#define MAX_FEAT_ID_DISTINGUISHER_OFFSET (4)

#define MAX_PYRAMID_LEVELS 5
#define PADDING_H 8
#define PADDING_V 6


#endif //GLOBAL_PARAMETERS_H_
