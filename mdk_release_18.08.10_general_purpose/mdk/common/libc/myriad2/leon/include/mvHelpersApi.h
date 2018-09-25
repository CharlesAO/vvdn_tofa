/*
 * mvHelpers.h
 *
 *  Created on: May 16, 2014
 */

#ifndef MVHELPERS_H_
#define MVHELPERS_H_

#include <mv_types.h>

#ifdef __cplusplus
extern "C" {
#endif

//Input buffer used for reading
extern char* mvInputBuffer;

void mvSetHeap(uint32_t start_address, uint32_t size);
void mvSetReadPointer(char* address);

#ifdef __cplusplus
}
#endif


#endif /* MVHELPERS_H_ */
