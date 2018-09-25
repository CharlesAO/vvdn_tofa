///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon-rt file 
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <swcLeonUtils.h>
#include <DrvLeonL2C.h>
#include <DrvLeon.h>
#include "DrvSvu.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// Include here for convenience for complex shave modules split to separate shave_export.h file or similar

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern u32 diff_sharing0_main;
extern u32 diff_sharing1_main;
extern u32 global_sharing2_main;
extern u32 global_sharing3_main;

extern volatile int diff_sharing0_myint1[4];
extern volatile int diff_sharing0_myint2[4];
extern volatile int diff_sharing1_myint1[4];
extern volatile int diff_sharing1_myint2[4];

extern int diff_sharing0_myintrez[4];
extern int diff_sharing1_myintrez[4];

extern int global_sharing2_myintrez[4];
extern int global_sharing3_myintrez[4];

volatile int __attribute__((section(".cmx.shared"))) shared1[4];
volatile int __attribute__((section(".cmx.shared"))) shared2[4];
volatile int shared3[4];

// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
// This is a trivial test where we pass in two vectors to a shave that adds them together
// the results is checked using the RISC processor.
//
void sharing_test(void)
{
    u32          i;
    u32          test_pass = 1;

    swcLeonDisableDcache();

    // ExampleA: Show Simple sharing between Leon and Shave0
    // --------------------------------------------------------------------------
    printf("\n<<< ExampleA : Simple Leon - Shave0 Sharing using global data  >>>\n\n");

    // Set input parameters Shaves
    diff_sharing0_myint1[0] = 4;
    diff_sharing0_myint1[1] = 25;
    diff_sharing0_myint1[2] = 313;
    diff_sharing0_myint1[3] = 13;

    diff_sharing0_myint2[0] = 4;
    diff_sharing0_myint2[1] = 30;
    diff_sharing0_myint2[2] = 400;
    diff_sharing0_myint2[3] = 500;

    // Run and wait for shave to complete
    printf("Start Shave0 at addr:0x%X \n", (unsigned int)&diff_sharing0_main);
    swcResetShave(0);
    swcSetAbsoluteDefaultStack(0);
    swcStartShave(0, (u32)&diff_sharing0_main);
    swcWaitShave(0);

    // Check the result
    printf("res  : %d %d %d %d\n",diff_sharing0_myintrez[0], diff_sharing0_myintrez[1], diff_sharing0_myintrez[2], diff_sharing0_myintrez[3]);

    for (i = 0; i < 4; i++)
        if (diff_sharing0_myintrez[i] != (diff_sharing0_myint1[i] + diff_sharing0_myint2[i]))
            test_pass = 0;

    if (test_pass)
        printf("\n\nDiff_sharing Executed Successfully on Shave0\n\n");
    else 
        printf("Diff_sharing failed on Shave0\n");

    // ExampleB: Show Simple sharing between Leon and Shave1
    // --------------------------------------------------------------------------
    printf("\n<<< ExampleB : Simple Leon - Shave1 Sharing using global data  >>>\n\n");
    test_pass = 1;

    // Set input parameters Shaves
    diff_sharing1_myint1[0] = 4;
    diff_sharing1_myint1[1] = 25;
    diff_sharing1_myint1[2] = 313;
    diff_sharing1_myint1[3] = 13;

    diff_sharing1_myint2[0] = 4;
    diff_sharing1_myint2[1] = 30;
    diff_sharing1_myint2[2] = 400;
    diff_sharing1_myint2[3] = 500;

    // Run and wait for shave to complete
    printf("Start Shave1 at addr:0x%X \n", (unsigned int)&diff_sharing1_main);
    swcResetShave(1);
    swcSetAbsoluteDefaultStack(1);
    swcStartShave(1, (u32)&diff_sharing1_main);
    swcWaitShave(1);

    // check the result
    printf("res  : %d %d %d %d\n",diff_sharing1_myintrez[0], diff_sharing1_myintrez[1], diff_sharing1_myintrez[2], diff_sharing1_myintrez[3]);

    for (i = 0; i < 4; i++)
        if (diff_sharing1_myintrez[i] != (diff_sharing1_myint1[i] + diff_sharing1_myint2[i]))
            test_pass = 0;

    if (test_pass)
        printf("\n\nDiff_sharing Executed Successfully on Shave 1\n\n");
    else
        printf("Diff_sharing failed on Shave 1\n");

    // ExampleC: Show data sharing between Shaves 2 and 3
    // --------------------------------------------------------------------------
    printf("\n<<< ExampleC : Share data between Shaves 2 and 3  >>>\n\n");
    test_pass = 1;

    // Initialize the two arrays that will be shared between shave2 and shave3
    shared1[0] = 4;
    shared1[1] = 25;
    shared1[2] = 313;
    shared1[3] = 13;

    shared2[0] = 4;
    shared2[1] = 30;
    shared2[2] = 400;
    shared2[3] = 500;

    // Run and wait for shave to complete
    printf("Start Shave2 at addr:0x%X \n", (unsigned int)&global_sharing2_main);
    swcResetShave(2);
    swcSetAbsoluteDefaultStack(2);
    swcStartShave(2, (u32)&global_sharing2_main);
    swcWaitShave(2);

    // Check the result
    printf("res  : %d %d %d %d\n",global_sharing2_myintrez[0], global_sharing2_myintrez[1], global_sharing2_myintrez[2], global_sharing2_myintrez[3]);

    for (i = 0; i < 4; i++)
        if (global_sharing2_myintrez[i] != (shared1[i] + shared2[i]))
            test_pass = 0;    

    if (test_pass)
        printf("\n\nGlobal_sharing Executed Successfully on Shave 2\n\n");
    else
        printf("Global_sharing failed on Shave 2\n");


    // ExampleC -- The Shave 3 part
    printf("\n<<< ExampleC - Shave 3 part >>>\n\n");
    test_pass = 1;

    // Run and wait for shave to complete
    printf("Start Shave3 at addr:0x%X \n", (unsigned int)&global_sharing3_main);
    swcResetShave(3);
    swcSetAbsoluteDefaultStack(3);
    swcStartShave(3,(u32)&global_sharing3_main);
    swcWaitShave(3);

    // Check the result
    printf("res  : %d %d %d %d\n",global_sharing3_myintrez[0], global_sharing3_myintrez[1], global_sharing3_myintrez[2], global_sharing3_myintrez[3]);

    for (i = 0; i < 4; i++)
        if (global_sharing3_myintrez[i] != (shared1[i] + shared2[i]))
            test_pass = 0;

    if (test_pass)
        printf("\n\nGlobal_sharing Executed Successfully on Shave 3\n\n");
    else
        printf("Global_sharing failed on Shave 3\n");


    // ExampleD: Sharing data between the Leons by using the symbol names of Leon RT variables
    // ---------------------------------------------------------------------------------------
    printf("\n<<< ExampleD : Sharing data between the Leons  >>>\n\n");

    // Initialize the data that will be shared between the Leons
    for (i = 0; i < 4; i++) {
        shared3[i] = shared1[i] + shared2[i];
    }

    return;
}

int main(void)
{
    printf("Executing sharing test...\n\n");

    sharing_test();

    DrvLeonRTSignalStop();

    return 0;
}
