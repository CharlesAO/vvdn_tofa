///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dynamic memory operators override & check API
///

#ifndef __MEM_CHECK_H__
#define __MEM_CHECK_H__

#if defined(ALLOC_DEBUG)

void * operator new  (size_t size, const char *file, int line);
void * operator new[](size_t size, const char *file, int line);

void * operator new  (size_t size);
void * operator new[](size_t size);

#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW

void ShowAllocs   ();
int  CheckOverflow(int echo); //check buffer canaries
int  CheckMemLeaks(int echo);

#endif //  ALLOC_DEBUG
#endif //__MEM_CHECK_H__