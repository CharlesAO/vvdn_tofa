/******************************************************************************
 @File         : gaussHx2.h
 @Brief        : Contains a function that applay downscale 2x horizontal with a gaussian filters with kernel 5x5. 
                 Have to be use in combination with GaussVx2 for obtain correct output. 
                 Gaussian 5x5 filter was decomposed in liniar 1x5, applayed horizontal and vertical
 Date          : 10 - 01 - 2013
 E-mail        : marius.truica@movidius.com
 Copyright     :  Movidius Srl 2013, Movidius Ltd 2013

HISTORY
 Version        | Date       | Owner         | Purpose
 ---------------------------------------------------------------------------
 1.0            | 10.09.2012 | Marius Truica    | First implementation
 1.1            | 14.09.2012 | Marius Truica    | in conformity with mail RE: 5x5 gaussian SIPP style - 9/13/2012, split the original file in 3, remove tabs
 2.0            | 10.01.2013 | Marius Truica    | in conformity with mail Gauss x2 unit tests... din 1/9/2013 3:16 PM, change name and integrated in MvCvtests
  ***************************************************************************/

#ifndef __GAUSSHX2_H__
#define __GAUSSHX2_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
///Apply downscale 2x horizontal with a gaussian filters with kernel 5x5. 
///Have to be used in combination with GaussVx2 to obtain correct output.
///works on width multiple of 8 and greater then or equal with 24
///@param[in]  inLine   input line
///@param[out] outLine output resized line
///@param[in]  width input line width

MVCV_FUNC(void, mvcvGaussHx2, u8 *inLine,u8 *outLine,int width)
//!@}

#endif //__GAUSSHX2_H__
