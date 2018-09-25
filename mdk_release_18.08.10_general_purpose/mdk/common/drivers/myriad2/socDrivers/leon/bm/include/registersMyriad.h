///
/// @file registersMyriad.h
/// 
/// 
/// @defgroup registersMyriad registersMyriad Driver
/// @{
///

// This file just allows us to have a common header for both Myriad1 and Myriad2 register definitions

#ifndef REGISTERS_MYRIAD_H
#define REGISTERS_MYRIAD_H

#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "registersMyriadMa2x5x.h"
#elif defined(MA2480) || defined(MA2485) || defined(MA2485)
#include "registersMyriadMa2x8x.h"
#elif defined(MA2490)
#include "registersMyriadMa2x9x.h"
#else
#error "Unknown CPU architecture"
#endif


#endif

///@}
