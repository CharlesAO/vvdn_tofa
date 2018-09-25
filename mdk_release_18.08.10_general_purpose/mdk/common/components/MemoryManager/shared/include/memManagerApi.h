#ifndef _MEMMANAGERAPI_H_
#define _MEMMANAGERAPI_H_

#include "memManagerAreas.h"


//Error codes
typedef enum{
    NO_ERROR = 0,						/*no error*/
    MEMALLOC_INVALID_SIZE = 10,		    /*Invalid size value*/
    MEMALLOC_SIZE_EXCEEDED = 11,	    /*Size bigger than available space*/
    MEM_INVALID_ADDRESS = 12,			/*Invalid address used*/
    MEM_EXPORT_ALLOCMEM = 13,           /*Allocate more memory for export buffer*/
}MemMgrErrorCode;

////////////////////////////////////// Prototypes ////////////////////////////////////////
                                        /*APIS*/
/// Allocate memory equal to the specified size inside the given memory area
/// @param[in] size        - size of the allocation
/// @param[in] area        - memory area where to allocate
/// @return    address     - address where memory was allocated
void* MemMgrAlloc(size_t size,
	              MemMgrAreas area,
	              unsigned int MemAlignment);

/// Free up memory allocated at the inputed address
/// @param[in] address     - address where to free memory from
/// @return    error       - error code
MemMgrErrorCode MemMgrFree(void *address);

/// Get memory’s current state as a buffer structed into a .csv style format for easy export
/// @param[in]  bufSize        - array of pointers to output lines
/// @param[out] csvBuffer      - array of pointers to input lines
/// @param[out] csvLenght      - array of values from convolution
/// @return    error       - error code
MemMgrErrorCode MemMgrBufferExport( size_t bufSize,
                                    char *csvBuffer,
                                    size_t *csvLenght);

/// Graphically illustrate the current state of the memory mapped inside the memory manager
/// @param[in]  dispWidth   - number of characters to display on one line
void MemMgrExportDraw(int dispWidth);

#endif
