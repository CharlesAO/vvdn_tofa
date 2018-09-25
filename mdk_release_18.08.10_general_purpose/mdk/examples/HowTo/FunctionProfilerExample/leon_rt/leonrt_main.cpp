///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file 
///


// 1: Includes
// ----------------------------------------------------------------------------

#include <DrvLeonL2C.h>
#include <DrvLeon.h>
#include <DrvShaveL2Cache.h>
#include <stdio.h>
#include <string.h>
#include <swcLeonUtils.h>
#include <DrvSvu.h>
#include <swcShaveLoader.h>
#include <DrvTimer.h>

#include "Shave.h"

#include <array>

extern u32 SimpleShave0_shave_zero;
extern u32 SimpleShave1_shave_one;
extern u32 SimpleShave2_shave_two;




// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

enum ShaveId {
	SHAVE0 = 0,
	SHAVE1 = 1,
	SHAVE2 = 2 // etc
};
//const std:array<ShaveId,3> shaves = {SHAVE0, SHAVE1, SHAVE2};

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------


// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
char __attribute__((section(".cmx.data"))) msgbuff[128];
//extern "C" {


#define DO_NOPS(n) {for (int i=0; i<n; i++) { asm volatile ("nop"); } }

int totalCopyCtorsCalled = 0;

class Gogoloi {
public:
	Gogoloi() {
	}
	Gogoloi(Gogoloi&) {
		DO_NOPS(500);
		totalCopyCtorsCalled++;
	}
    int getUselessEnergyWasted() { return totalCopyCtorsCalled; }
};


int f1(Gogoloi g);
int f2(Gogoloi g);
int f3(Gogoloi g);


int f1(Gogoloi g) {
	DO_NOPS(100);
	for (int i=0; i<500; i++){
		f2(g);
		DO_NOPS(30);
		f3(g);
	}
	DO_NOPS(300);
    return g.getUselessEnergyWasted();
}

int f2(Gogoloi g) {
    int ret = g.getUselessEnergyWasted();
	DO_NOPS(22);
	ret += f3(g) + f3(g) + f3(g);
	DO_NOPS(500);
    return ret;
}

int f3(Gogoloi g) {
	DO_NOPS(30);
    return g.getUselessEnergyWasted();
}

Shave *shave;
int  main(void)
{
	DrvTimerInit();
    strncpy(msgbuff, "LeonRT Started.\n", 128);

    //This should be singleton design pattern, but dynamic allocation is not working?
    Shave* workers [] = {
    	Shave::gethandle(SHAVE0),
		Shave::gethandle(SHAVE1),
		Shave::gethandle(SHAVE2)
    };

    constexpr unsigned totalWorkers = sizeof(workers)/sizeof(workers[0]);
    uint32_t code[totalWorkers] = {
    	(uint32_t) &SimpleShave0_shave_zero,
		(uint32_t) &SimpleShave1_shave_one,
		(uint32_t) &SimpleShave2_shave_two
    };

    for ( auto dorel : workers ) {
    	dorel->reset();
    	dorel->setdefaultstack();
    }

    for (unsigned int i=0; i<totalWorkers; ++i) {
    	workers[i]->start(code[i]);
    }

    // do incredible things here
    DO_NOPS(20);
//    printf("start to do amazing stuff...");
    Gogoloi g; // don't want optimizations here
    f1(g);
//    printf("Woooo!!!! I'm mesmerized!");
    DO_NOPS(30);

    // wait for all
    for ( auto obositu : workers ) {
    	obositu->wait();
    }

    Shave::destroy(SHAVE0);
    Shave::destroy(SHAVE1);
    Shave::destroy(SHAVE2);
    // ...

    printf("All shaves finished work\n");

    DrvLeonRTSignalStop();

    return 0;
}
//}
