///
/// @file DrvSippDefines.h
/// 
/// 
/// @date   Jul 22, 2014
/// @ingroup DrvSipp
/// @{
///


#ifndef DRVSIPPDEFINES_H_
#define DRVSIPPDEFINES_H_


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
#include "DrvSippDefines_ma2x5x.h"
#endif

#include <mv_types.h>



//======================================================================================
//==========  THIS IS A DRIVER FOR SIPP MIPI ONLY, NOT FOR GENERAL SIPP USAGE! =========
//======================================================================================


// 1: Defines
// ----------------------------------------------------------------------------


// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

/// SIPP buffer register map structure.
/// This structure can be mapped over Input/Output buffers of SIPP filters
typedef struct sipp_buf {
    volatile u32 base;///< buffer base address
    volatile u32 cfg;///< buffer configuration(format/Number of planes/etc.)
    volatile u32 line_stride;///< buffer line stride(must be multiple of 8 bytes)
    volatile u32 plane_stride;///< buffer plane stride(must be multiple of 8 bytes)
    volatile u32 lcs;///< [31:16] - Line chunk size in bytes for slice chunking address mode
                     ///< [5:4] - Buffer fill level decrement interrupt configuration
                     ///< [0:3] - Reserved
    volatile u32 fill_control;///< Buffer fill control register
    volatile u32 context;///< Frame and line context/status
} sipp_buf_t;

// SIPP Tx filter register map structure
/// This structure can be mapped over MIPI Tx configuration registers
typedef struct sipp_tx {
    volatile u32 frm_dim;///< MIPI Tx filter frame dimensions in pixels
    volatile u32 cfg;///< MIPI Tx filter configuration
    volatile u32 line_count;///< Line count
    volatile u32 line_comp;///< Line count at which to generate line compare interrupt
    volatile u32 vstatus;///< Vertical status register
    volatile u32 vcomp;///< Vertical interval in which to generate vertical interval interrupt
                       ///< 00 – VSYNC
                       ///< 01 – Back Porch
                       ///< 10 – Active
                       ///< 11 – Front Porch
    volatile u32 hsync_width;///< Specifies the width, in PCLK clock periods,
                             ///< of the horizontal sync pulse (value programmed is HSW-1)
    volatile u32 h_backporch;///< Specifies the number of PCLK clocks from the
                             ///< end of the horizontal sync pulse to the start of
                             ///< horizontal active (value programmed is HBP so a back porch of 0 cycles can be set)
    volatile u32 h_activewidth;///< Specifies the number of PCLK clocks in the horizontal active section
                               ///< (value programmed is AVW-1)
    volatile u32 h_frontporch;///< Specifies the number of PCLK clocks from end of
                              ///< active video to the start of horizontal sync
                              ///< (value programmed is HFP)
    volatile u32 vsync_width;///< Specifies the width in lines of the vertical sync pulse
                             ///< (value programmed is VSW-1).This value is used for the odd field when in interlace mode.
    volatile u32 v_backporch;///< Specifies the number of lines from the end of the vertical sync pulse
                             ///< to the start of vertical active (value programmed is VBP)
    volatile u32 v_activeheight;///< Specifies the number of lines in the vertical active section
                                ///< (value programmed is AVH-1)
    volatile u32 v_frontporch;///< Specifies the number of lines from the end of active data
                              ///< to the start of vertical sync pulse (value programmed is VFP).
    volatile u32 vsync_start_offset;///< Number of PCLKs from the start of the last horizontal sync pulse in the Vertical Front Porch
                                    ///< to the start of the vertical sync pulse.
    volatile u32 vsync_end_offset;///< Number of PCLKs from the end of the last horizontal sync pulse in the Vertical Sync Active
                                  ///< to the end of the vertical sync pulse
    volatile u32 vsync_width_even;///< Specifies the width in lines of the vertical sync pulse
                                  ///< for even field when LCD is operating in interlace mode. (value programmed is VSW-1).
    volatile u32 v_backporch_even;///< Specifies the number of lines from the end of
                                  ///<the vertical sync pulse to the start of vertical active for even field when LCD is operating
                                  ///< in interlace scan mode. (value programmed is VBP)
    volatile u32 v_activeheight_even;///< Specifies the number of lines in the vertical active section for even field
                                     ///< when LCD is operating in interlace scan mode.(value programmed is AVH-1)
    volatile u32 v_frontporch_even;///< Specifies the number of lines from the end of active data to the start of
                                   ///< vertical sync pulse for even field when LCD is operating In interlace scan mode.(value programmed is VFP)
    volatile u32 vsync_start_offset_even;///< Number of PCLKs from the start of the last horizontal sync pulse in the
                                         ///< Vertical Front Porch to the start of the vertical sync pulse for even field.
    volatile u32 vsync_end_offset_even;///< Number of PCLKs from the end of the last horizontal sync pulse in the Vertical Sync Active
                                       ///< to the end of the vertical sync pulse for even field.
    volatile u32 timing_gen_trig;///< Timing generator start trigger.
} sipp_tx_t;

/// SIPP Rx filter register map structure
/// This structure can be mapped over MIPI Rx configuration registers
typedef struct sipp_rx {
    volatile u32 frm_dim;///< MIPI Rx filter output frame dimensions in pixels
    volatile u32 cfg;///< MIPI Rx filter configuration
    volatile u32 x0;///< Window 0 x co-ordinate configuration
    volatile u32 x1;///< Window 1 x co-ordinate configuration
    volatile u32 x2;///< Window 2 x co-ordinate configuration
    volatile u32 x3;///< Window 3 x co-ordinate configuration
    volatile u32 y0;///< Window 0 y co-ordinate configuration
    volatile u32 y1;///< Window 1 y co-ordinate configuration
    volatile u32 y2;///< Window 2 y co-ordinate configuration
    volatile u32 y3;///< Window 3 y co-ordinate configuration
    volatile u32 sel01;///< Selection enables and least significant bit for windows 0 and 1
    volatile u32 sel23;///< Selection enables and least significant bit for windows 2 and 3
    volatile u32 mask0;///< Selection mask for window 0
    volatile u32 mask1;///< Selection mask for window 1
    volatile u32 mask2;///< Selection mask for window 2
    volatile u32 mask3;///< Selection mask for window 3
    volatile u32 black01;///< Black levels for windows 0 and 1 or even lines of RAW Bayer data
    volatile u32 black23;///< Black levels for windows 2 and 3 or odd lines of RAW Bayer data
    volatile u32 vbp;///< Vertical back porch and private slice stride
} sipp_rx_t;




#endif /* DRVSIPPDEFINES_H_ */

///@}

