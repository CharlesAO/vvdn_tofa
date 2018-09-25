///
/// @file DrvCommon.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvCommon
/// @{
/// @brief     Common utility functions
///


// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "DrvCommon.h"


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

void DrvDynInfraCheckReturnCode(int code)
{
    switch(code)
    {
    case MYR_DYN_INFR_SUCCESS:
        printf("Myriad dynamic loading infrastructure: operation successful. \n");
        break;
    case MYR_DYN_INFR_ERROR:
        printf("Myriad dynamic loading infrastructure: operation error. \n");
        break;
    case MYR_DYN_INFR_MULTIPLE_SHAVE_ALLOCATION:
        printf("Myriad dynamic loading infrastructure: operation error. Shave allocated to multiple applications. \n");
        break;
    case MYR_DYN_INFR_INVALID_PARAMETERS:
        printf("Myriad dynamic loading infrastructure: operation error. Invalid function parameter(s). \n");
        break;
    case MYR_DYN_INFR_CANNOT_ALLOC_HEAP:
        printf("Myriad dynamic loading infrastructure: operation error. Cannot allocate heap memory. \n");
        break;
    case MYR_DYN_INFR_CANNOT_ALLOC_GROUPDATA:
        printf("Myriad dynamic loading infrastructure: operation error. Cannot allocate group data memory. \n");
        break;
    case MYR_DYN_INFR_RUNALGO_SHAVES_BUSY:
        printf("Myriad dynamic loading infrastructure: operation error when running shave algo. Target shaves are busy or invalid function parameter.  \n");
        break;
    case MYR_DYN_INFR_SHAVE_BUSY_OR_INVALID:
        printf("Myriad dynamic loading infrastructure: operation error when running shave algo on assigned shave. Target shave busy or not assigned. \n");
        break;
    case MYR_DYN_INFR_INVALID_SAVED_CONFIG:
        printf("Myriad dynamic loading infrastructure: operation error at cleanup. Invalid saved configuration. \n");
        break;
    case MYR_DYN_INFR_ERROR_STACK_OVERFLOW:
    	printf("Myriad dynamic loading infrastructure: operation error at running shave. Stack overflow occured. \n");
    	break;
    case MYR_DYN_INFR_NUMBER_OF_FREE_SHAVE_NOT_AVAILABLE:
        printf("Myriad dynamic loading infrastructure: free shaves number requested not available \n");
        break;
    case MYR_DYN_INFR_SHAVE_NOT_FOUND_IN_CONTEXT_LIST:
        printf("Myriad dynamic loading infrastructure: shave not assigned to the Context structure \n");
        break;
     case MYR_DYN_INFR_HEAP_NOT_SELECTED:
        printf("Myriad dynamic loading infrastructure: aplication does not require heap but heap size is set \n");
        break;
     case MYR_DYN_INFR_HEAP_SIZE_NOT_SET:
        printf("Myriad dynamic loading infrastructure: application require heap but heap size is not set \n");
        break;
     case MYR_DYN_INFR_FUNCTION_NOT_FOUND:
        printf("Myriad dynamic loading infrastructure: application entry could not be found into its own IAT \n");
        break;
    default:
        printf("Myriad dynamic loading infrastructure: unknown return code. Check return value description of causing function. \n");
        break;
    }
}

///@}
