#ifndef __AVGPOOL7X7XK_H__
#define __AVGPOOL7X7XK_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{ 
///This kernel performs a vertical average on 49 elements
///@param[in]	srcAddr	- input elements 16-bits floating point - 49 lines
///@param[out]	destAddr	- Output line, 16-bits floating point
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvAvgPool7x7xk, half** srcAddr, half** destAddr, u32 width)
//!@}


#endif //__AVGPOOL7X7XK_H__