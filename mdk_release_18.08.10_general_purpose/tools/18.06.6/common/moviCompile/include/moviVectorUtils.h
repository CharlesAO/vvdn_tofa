/*  ---------------------------------------------------------------------------
 *  Copyright (C) 2013-2018 Movidius Ltd. All rights reserved
 *  ---------------------------------------------------------------------------
 *  File       :   moviVectorUtils.h
 *  Description:   Vector Utilities support library for SHAVE
 *  --------------------------------------------------------------------------- */

#ifndef _MOVIVECTORUTILS_H_
#define _MOVIVECTORUTILS_H_ (1)

#ifndef __MOVICOMPILE__
# error "moviVectorUtils.h: this header must be included only in a file compiled with moviCompile!"
#elif defined(__shavenn__)
# error "moviVectorUtils.h: is not supported for SHAVE NN"
#endif


#pragma GCC system_header

/* Include the sub-components for the Movidius Vector Utilities library */
#include <moviVectorTypes.h>
#include <moviVectorConvert.h>
#include <moviVectorAsTypes.h>
#include <moviVectorFunctions.h>


#endif /* _MOVIVECTORUTILS_H_ */
