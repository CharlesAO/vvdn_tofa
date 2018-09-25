#ifndef _DEFINES_H_
#define _DEFINES_H_

// Used for buffers static memory allocation
#define MAX_FRAME_WIDTH	    (1280)
#define MAX_FRAME_HEIGHT	(720)
#define MAX_FEATURES	    (300)
#define MAX_PYRAMID_LEVELS	(4)

// The only window size supported in our optimized kernels
#define SEARCH_WIN_SZ	    (5)

// Algorithm termination criteria
#define STOP_CRIT_MAX_ITER 	(9)
#define STOP_CRIT_EPS		(0.01f)

// The algorithm uses malloc so a heap is needed
#define HEAP_SIZE (20 * 1024)

#endif