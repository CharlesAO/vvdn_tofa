/* =============================================================================
* Copyright (c) 2013-2014 MM Solutions AD
* All rights reserved. Property of MM Solutions AD.
*
* This source code may not be used against the terms and conditions stipulated
* in the licensing agreement under which it has been supplied, or without the
* written permission of MM Solutions. Rights to use, copy, modify, and
* distribute or disclose this source code and its documentation are granted only
* through signed licensing agreement, provided that this copyright notice
* appears in all copies, modifications, and distributions and subject to the
* following conditions:
* THIS SOURCE CODE AND ACCOMPANYING DOCUMENTATION, IS PROVIDED AS IS, WITHOUT
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED. MM SOLUTIONS SPECIFICALLY DISCLAIMS
* ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN
* NO EVENT SHALL MM SOLUTIONS BE LIABLE TO ANY PARTY FOR ANY CLAIM, DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
* PROFITS, OR OTHER LIABILITY, ARISING OUT OF THE USE OF OR IN CONNECTION WITH
* THIS SOURCE CODE AND ITS DOCUMENTATION.
* =========================================================================== */
/**
* @file hat_types.h
*
* @author ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 05-Nov-2013 : Author ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef __HAL_TYPES_COMMON_H__
#define __HAL_TYPES_COMMON_H__

#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>

/*
 *   Documentation: http://www.digitalsignallabs.com/fp.pdf
 *
 *   Example for unsigned format: U8Q8 (8 + 8 -> total 16 bits)
 *
 *           15                          8   7                           0
 *        +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * Bit    |D15|D14|D13|D12|D11|D10| D9| D8| D7| D6| D5| D4| D3| D2| D1| D0|
 *        +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * Weight | ^7| ^6| ^5| ^4| ^3| ^2| ^1| ^0|^-1|^-2|^-3|^-4|^-5|^-6|^-7|^-8|
 *        +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *
 *    Value  = D15 * 2^7
 *    Value += D14 * 2^6
 *    Value += D13 * 2^5
 *    Value += D12 * 2^4
 *    Value += D11 * 2^3
 *    Value += D10 * 2^2
 *    Value += D9  * 2^1
 *    Value += D8  * 2^0
 *    Value += D7  * 2^(-1)
 *    Value += D6  * 2^(-2)
 *    Value += D5  * 2^(-3)
 *    Value += D4  * 2^(-4)
 *    Value += D3  * 2^(-5)
 *    Value += D2  * 2^(-6)
 *    Value += D1  * 2^(-7)
 *    Value += D0  * 2^(-8)
 */

/*
 *   Example for signed format: S4Q8 (1 + 4 + 8 -> total 13 bits)
 *
 *          15                          8   7                           0
 *        +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * Bit    |D15|D14|D13|D12|D11|D10| D9| D8| D7| D6| D5| D4| D3| D2| D1| D0|
 *        +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 * Weight | S | S | S | S | ^3| ^2| ^1| ^0|^-1|^-2|^-3|^-4|^-5|^-6|^-7|^-8|
 *        +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
 *
 *    Value  = {D15|D14|D13|D12} * -1
 *    Value += D11 * 2^3
 *    Value += D10 * 2^2
 *    Value += D9  * 2^1
 *    Value += D8  * 2^0
 *    Value += D7  * 2^(-1)
 *    Value += D6  * 2^(-2)
 *    Value += D5  * 2^(-3)
 *    Value += D4  * 2^(-4)
 *    Value += D3  * 2^(-5)
 *    Value += D2  * 2^(-6)
 *    Value += D1  * 2^(-7)
 *    Value += D0  * 2^(-8)
 *
 */

#define HAL_SIZE2MUL(size)      ((size)->w * (size)->h)
#define HAL_POINT2MUL(point)    ((point)->x * (point)->y)
#define HAL_BOUND2MUL(bound)    ((bound)->width * (bound)->height)
#define HAL_IN_RANGE(val, r)    (((r)->min <= val) && (val <= (r)->max))

// Position
typedef struct {
    int32 x;                    // Horizontal
    int32 y;                    // Vertical
}hat_point_t;

// Size
typedef struct {
    uint32 w;                   // Width
    uint32 h;                   // Height
}hat_size_t;

// Size
typedef struct {
    uint32 x;                   // Width
    uint32 y;                   // Height
} hat_dim_2d_t;


/* Rectangle */
typedef struct {
    int32 x;                    // Left
    int32 y;                    // Top
    uint32 w;                   // Width
    uint32 h;                   // Height
}hat_rect_t;

/* Rectangle float */
typedef struct {
    float x;                    // Left
    float y;                    // Top
    float w;                    // Width
    float h;                    // Height
} hat_rect_float_t;

/* Range [uint32] */
typedef struct {
    uint32 min;                 // min
    uint32 max;                 // max
}hat_range_t;


/* Range [float] */
typedef struct {
    float min;                 // min
    float max;                 // max
}hat_range_float_t;

// Fract struct
typedef struct  {
    int32 num;                  // Numerator
    int32 denom;                // Denominator
}hat_fract_t;

typedef struct {
    uint32 frame_number;        // Frame number
    osal_timeval time;          // Time
}hat_timestamp_t;

// Structure controlling bounds of parameters
typedef struct {
    int32 initially;            // default value
    int32 minimum;              // minimum value that can be sent to hardware
    int32 maximum;              // maximum value that can be sent to hardware
    int32 step;                 // granulatiry
}hat_ctrl_bounds_t;

// Structure controlling bounds of parameters
typedef struct {
    float initially;            // default value
    float minimum;              // minimum value that can be sent to hardware
    float maximum;              // maximum value that can be sent to hardware
    float step;                 // granulatiry
}hat_bounds_float_t;

typedef float hat_strength_t;

#endif // __HAL_TYPES_COMMON_H__

