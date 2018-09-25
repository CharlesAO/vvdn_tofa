///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     simple shave vectorization
///

// This provides easier access to vector types
#include <moviVectorUtils.h>
#include <svuCommonShave.h>
// Data shared with Leon
volatile unsigned int myint1[4];
volatile unsigned int myint2[4];
volatile unsigned int myintrez[4];

__attribute__((dllexport)) int main( void )
{
    // declare pointers to integer vectors
    // TODO:  This works but we get compiler warnings - find cleaner way
    uint4* v_1   = (uint4*)&(myint1[0]);
    uint4* v_2   = (uint4*)&(myint2[0]);
    uint4* v_res = (uint4*)&(myintrez[0]);

    // Vector addition
    *v_res = *v_1 + *v_2;

//     *v_res = __builtin_shave_vau_iadds_u32_rr(*v_1, *v_2);

    SHAVE_HALT;

    return 0;
}
