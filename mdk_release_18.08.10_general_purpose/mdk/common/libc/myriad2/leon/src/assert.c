/*
 * assert.c
 *
 *  Created on: May 16, 2014
 */

#include <stdio.h>
#include <assert.h>
#include <mv_types.h>

void __attribute__ ((noreturn)) __assert_func(const char* file, int line, const char* funcName, const char* message)
{
    #ifdef NO_PRINT
    UNUSED(file);
    UNUSED(line);
    UNUSED(funcName);
    UNUSED(message);
    #else
	printf("Assertion failed in function %s \n on line %d in file %s : %s\n",funcName,line,file,message);
	//return;
    #endif
	//Using the same trap as in mvAssert if debug
	asm("ta 1\n");
	//And lock since the function has to not return
	while (1==1);
}
