///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Shave hello world source code
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <svuCommonShave.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

void exquisiteWorkOf(unsigned count);

int dv = 0;

void dummy() {
	dv++;
}

static void aStaticFunction() {
	dv++;
}

void f2() {
	exquisiteWorkOf(20);
}

void f1() {
	dummy();
	exquisiteWorkOf(50);

	aStaticFunction();

	f2();
}


//void pointofnoreturn(void (*f)(void)) {
//	f();
//	SHAVE_HALT;
//}
//#define DECLARE_SHAVE(name) int name(void) {

int shave_zero(void)
{
	exquisiteWorkOf(5);

	SHAVE_HALT;
    return 0;

}

int shave_one(void)
{
	for(int i=0; i<10; i++)
		dummy();

	SHAVE_HALT;
    return 0;
}

int shave_two(void)
{
	f1();
	dummy(); dummy();

	// is it really needed?
	SHAVE_HALT;
    return 0;
}
