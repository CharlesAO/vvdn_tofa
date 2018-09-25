///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#ifndef __SIPP_H__
#define __SIPP_H__


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "sipp_ma2x5x.h"
#endif

#if defined(MA2480) || defined(MA2485)
#include "sipp_ma2x8x.h"
#endif

#endif // __SIPP_H__
