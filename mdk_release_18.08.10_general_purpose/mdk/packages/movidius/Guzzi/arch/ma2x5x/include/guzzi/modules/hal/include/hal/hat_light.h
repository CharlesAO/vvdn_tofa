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
* @file hat_light.h
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

#ifndef __HAL_TYPES_LIGHT_H__
#define __HAL_TYPES_LIGHT_H__

#include <hal/hat_types.h>

#define MAX_FLASH_INTENSITIES   8

typedef enum {
    HAT_FLASH_TYPE_LED,
    HAT_FLASH_TYPE_XENON
} hat_flash_type_t;

typedef enum {
    LIGHT_SRC_TORCH,
    LIGHT_SRC_VIDEO_LIGHT,
    LIGHT_SRC_AF_ASSIST,
    LIGHT_SRC_PRIVACY,
    LIGHT_SRC_MAIN_FLASH,
    LIGHT_SRC_MAIN_RED_EYE
} hat_light_source_type_t;

typedef struct  {
    uint64                  charge_duration; // Time taken before flash can fire again.[ns]
    uint32                  colour_temp;     //
    uint8                   available;       // Whether this camera has a flash.
    uint8                   max_energy;      //Max energy output of the flash for a full power single flash [lumen-seconds]
    hat_flash_type_t        type;
}hat_light_spec_features_t;

typedef struct  {
    uint32 intensity;   // [cd] light intensity
    uint32 power;       // [0-100] Light power (driver specific value/index)
    uint32 timeout;     // [uSeconds] 0 infinity
    uint32 charge_time; // [us] for LED this is time between two flashes with that power
    void*  command_script; // Driver specific script
}hat_light_entry_t;

typedef struct {
    uint32                      num_light_intensities;   // number of valid entries in the list
    const hat_light_entry_t    *entries; // List of supported intensity/timeout entries.
    hat_light_spec_features_t  spec_features;
    void                       *drv_private;        // driver private data
} hat_light_capabilities_t;

/* Structure for flash light */
typedef struct
{
    hat_light_capabilities_t flash;

    hat_light_capabilities_t torch;

    hat_light_capabilities_t video;

    hat_light_capabilities_t af_assist;

    hat_light_capabilities_t privacy;

    hat_light_capabilities_t red_eye; // Blink period is HW (driver) specific

} hat_flash_bounds_t;

/* Structure for flash light control */
typedef struct  {
    hat_light_source_type_t src;
    uint32                  power;          // Led flash power in percentage [0-100%].  0 - means OFF
    int32                   start_time;     // flash start delay relative to exposure start [ns]
    uint32                  duration;       // Led duration in [us] - 0 - means infinity
}hat_light_ctrl_entry_t;

/* Flash state - struct flash_state reported for each frame for FR */
typedef struct {
    uint32 flash;           /* Main flash (LED/Xenon) intensity */
    uint32 privacy;         /* Privacy LED intensity */
    uint32 torch;           /* Torch LED intensity */
    uint32 video;           /* Torch LED intensity */
    uint32 af_assist;       /* AF assist LED intensity */
    uint32 red_eye;         /* Red Eye LED state 1 - blinking */
} hat_lights_state_t;

typedef struct {
    float intensity;
    float mult;
    int cnt;
}light_params_simulator_t;

typedef struct {
    light_params_simulator_t flash;
    light_params_simulator_t privacy;
    light_params_simulator_t torch;
    light_params_simulator_t video;
    light_params_simulator_t af_assist;
    light_params_simulator_t red_eye;
}light_simulator_t;

#endif // __HAL_TYPES_LIGHT_H__

