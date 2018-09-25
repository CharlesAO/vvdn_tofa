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
* @file gzz_scene_types.h
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

#ifndef GZZ_SCENE_TYPES_H_
#define GZZ_SCENE_TYPES_H_

/* Framework scenes supported */
typedef enum {
    CAM_SCENE_NONE,
    CAM_SCENE_MANUAL,
    CAM_SCENE_AUTO,
    CAM_SCENE_CLOSEUP,
    CAM_SCENE_PORTRAIT,
    CAM_SCENE_LANDSCAPE,
    CAM_SCENE_UNDERWATER,
    CAM_SCENE_SPORT,
    CAM_SCENE_SNOW_BEACH,
    CAM_SCENE_MOOD,
    CAM_SCENE_NIGHT_PORTRAIT,
    CAM_SCENE_NIGHT_INDOOR,
    CAM_SCENE_FIREWORKS,
    CAM_SCENE_NIGHT,
    CAM_SCENE_INDOOR,
    CAM_SCENE_AUTO_VIDEO,
    CAM_SCENE_NIGHT_PORTRAIT_VIDEO,
    CAM_SCENE_ACTION,
    CAM_SCENE_BEACH,
    CAM_SCENE_CANDLELIGHT,
    CAM_SCENE_PARTY,
    CAM_SCENE_SNOW,
    CAM_SCENE_STEADYPHOTO,
    CAM_SCENE_SUNSET,
    CAM_SCENE_THEATRE,
    CAM_SCENE_DOCUMENT,
    CAM_SCENE_BARCODE,
    CAM_SCENE_SUPERNIGHT,
    CAM_SCENE_CINE,
    CAM_SCENE_OLDFILM,
    MAX_CAM_SCENE_MODE
} gzz_scene_modes_t;

#endif /* GZZ_SCENE_TYPES_H_ */
