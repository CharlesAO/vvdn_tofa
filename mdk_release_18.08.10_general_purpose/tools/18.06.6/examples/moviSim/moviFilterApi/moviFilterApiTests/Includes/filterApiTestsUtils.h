#ifndef __FILTER_API_TESTS_UTILS_H__
#define __FILTER_API_TESTS_UTILS_H__

#include <stdint.h>

#define assert_msg(x) !(std::cerr << "Assertion failed in test: " << x << std::endl)

void loadMemFromFile(char *fileName, uint32_t length, void * buffer);
bool unitTestCrcCheck(void* buffer, uint32_t size, uint32_t expectedCrc);
#endif
