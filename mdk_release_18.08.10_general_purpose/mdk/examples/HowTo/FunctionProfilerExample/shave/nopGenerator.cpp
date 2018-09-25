/*
 * nopGenerator.cpp
 *
 *  Created on: Feb 29, 2016
 */

#include "mv_types.h"
// doing nothing with grace
class Empty {
    public:
        explicit Empty() {
        }
        ~Empty() {
        }
};

template <int howManyTimes>
static inline __attribute__((always_inline)) 
//__attribute__((no_instrument_function))
Empty nop(Empty e) {
	asm volatile ("nop");
	return nop<howManyTimes-1>(e);
}

template<>
inline __attribute__((always_inline))
//__attribute__((no_instrument_function))
Empty nop<0>(Empty e) {
    return e;
}

extern "C"
void exquisiteWorkOf(unsigned count) {
	for (unsigned i=0; i<count; ++i) {
		nop<20>(Empty());
	}
}

inline __attribute__((always_inline))
void escape(void* p) {
	asm volatile("" : : "g"(p) : "memory");
}

inline __attribute__((always_inline))
void clobber(void*) {
	asm volatile("" : : : "memory");
}
