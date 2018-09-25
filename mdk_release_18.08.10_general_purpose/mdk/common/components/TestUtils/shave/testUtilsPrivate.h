/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights
///     reserved      .
///            For License Warranty see: common/license.txt
///
/// @brief     Software test library
///

#ifndef __TEST_UTILS_PRIVATE_H__
#define __TEST_UTILS_PRIVATE_H__

void shaveSetLocalReg( u32 LocalReg,u32 value );

u32 shaveGetLocalReg( u32 LocalReg );

tyHwPlatform shaveGetProcessorType( void );

#endif //__TEST_UTILS_PRIVATE_H__

