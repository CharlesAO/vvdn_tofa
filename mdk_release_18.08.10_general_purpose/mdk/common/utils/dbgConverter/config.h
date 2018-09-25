/*
 * config.h
 *
 *  Created on: Apr 10, 2014
 */

#ifndef CONFIG_H_
#define CONFIG_H_

/* This is the size of the buffer, which will
 * contains the data written by Movidius debug engine.
 */
#define INPUT_BUFFER_SIZE 1024

/* This define will include byte swapping functionality
 * in order to parse the data from inputFile correctly.
 * If this is not needed just comment the define.
 */
//#define USE_BYTE_SWAP

/* This is the debug tracer version.It is used
 * in the .vcd file header.
 */
#define TRACER_VERSION "MoviDebugLogVer 1"

/* This is the timescale used in .vcd file header */
#define TIME_SCALE "10 us"

#endif /* CONFIG_H_ */
