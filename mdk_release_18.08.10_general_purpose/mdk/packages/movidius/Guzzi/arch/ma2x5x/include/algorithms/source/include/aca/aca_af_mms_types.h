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
* @file aca_af_mms_types.h
*
* @Nayden Kanchev ( MM Solutions AD )
*
*/
/* -----------------------------------------------------------------------------
*!
*! Revision History
*! ===================================
*! 25-May-2014 : Nayden Kanchev ( MM Solutions AD )
*! Created
* =========================================================================== */

#ifndef  __FW_AF_INTERFACE_H__
#define  __FW_AF_INTERFACE_H__


#include <osal/osal_stdlib.h>
#include <osal/osal_string.h>
#include <osal/osal_stdtypes.h>
#include <osal/osal_time.h>
#include "aca_hllc_types.h"
#include "aca_af_types.h"
#include "aca_fmv_types.h"

/* =============================================================================
*                               PUBLIC DECLARATIONS
* =========================================================================== */

/*
    Main Auto Focus operating modes are shown in next table.
    AF operation mode enumeration - enum focus_mode
*/
    enum focus_mode {
        FW_AF_MODE_AUTO = 0,
        FW_AF_MODE_MACRO,
        FW_AF_MODE_PORTRAIT,
        FW_AF_MODE_INFINITY,
        FW_AF_MODE_HYPERFOCAL,
        FW_AF_MODE_EXTENDED,
        FW_AF_MODE_MANUAL,
        FW_AF_MODE_CONTINUOUS,
        FW_AF_MODE_CONTINUOUS_NORMAL,
        FW_AF_MODE_CONTINUOUS_EXTENDED,
        FW_AF_MODE_CUSTOM_DEFINED1,
        FW_AF_MODE_CUSTOM_DEFINED2,
        FW_AF_MODE_MODE_COUNT
    };

/*
    AF Control - enum focus_control
*/
enum focus_control
{
    //This mode is used to indicate start of AFFW
    //              If during AFFW running state start is called with different
    //mode than current one, AFFW will return error!
    //Lens end it's current movement if there is some.
    AF_CTRL_MODE_NORMAL = 0,
    //This mode is used to indicate stop of AFFW.
    //          There are two cases:
    // 1) Stop is called before AFFW to return end state(i.e still in running state)
    //then app must wait AFFW to return it's end state before stopping statistics.
    //Lens is driven to it's last focused position.(or default position).
    // 2) Stop is called after AFFW return it's end
    //state(normal case that indicate end of AFFW),
    //then there is no need app to wait AFFW to return it's end state
    //because it already do.
    //Lens end it's current movement if there is some.
    AF_CTRL_MODE_SUSPEND,
    //This mode is used to indicate abort of AFFW.
    //          There is no matter if AFFW returned it's end state or not
    //there is no need app to wait for it.
    //Lens end it's current movement if there is some.
    AF_CTRL_MODE_ABORT
    };
/*
typedef enum {
    AF_SPOT_SINGLE_CENTER = 0,
    AF_SPOT_MULTI_AVERAGE,
    AF_SPOT_MULTI_CENTER,
    AF_SPOT_MULTI_NORMAL
} fw3a_focus_spot_weighting;
*/
/* AF spot weighting - defines 3x3 spots weight */
    struct focus_spot_weighting {
        focus_spot_weighting_t mode;
    };

/*   used from focus_state_t
    enum focus_state {
        AF_STATE_RUNNING = 0,
        AF_STATE_FAIL,
        AF_STATE_SUCCESS,
        AF_STATE_SUSPEND,
        AF_STATE_IDLE,
        AF_STATE_WAIT_STABLE_SCENE
    };
*/

    enum focus_result {
        AF_RESULT_UNKNOWN = 0,
        AF_RESULT_FAIL,
        AF_RESULT_SUCCESS,
    };

    struct focus_window {
        int16  top;
        int16  left;
        uint16 width;
        uint16 height;
    };


//
//Contains coordinates of the zone which AF focus on
//
    struct af_bfz_coordinates {

        // Count of zones AF/CAF is focus on
        // maximum 10 zones!
        uint16 af_caf_good_zones_cnt;
        //Coordinates of zones in terms of 256 from whole frame
        struct focus_window zones[10];
    };

    struct iir_filter {

        int32  coeficients[11];
        uint16 thr; /* Monica only */
        uint32 gain;
    };

    struct fir_filter {
        /* Monica only */
        int32  coeficients[5];
        uint16 thr;
        uint32 gain;
    };

/**
 * Structure that contains H3A filters.
 */
    struct h3a_af_filters {
        struct iir_filter low_light_iir_0;
        struct iir_filter low_light_iir_1;
        struct iir_filter normal_light_iir_0;
        struct iir_filter normal_light_iir_1;
        /* Monica HW */
        struct fir_filter low_light_fir_0;
        struct fir_filter low_light_fir_1;
        struct fir_filter normal_light_fir_0;
        struct fir_filter normal_light_fir_1;
    };



/*
    Next table represent hardware layout of AF engine inside OMAP34XX ISP engine.
    Data in this structure was filled from CDM and send to AF algorithm using
    * _process() API.
    AF color paxel - struct af_h3a_color_paxel
*/

    struct af_h3a_color_paxel {
        int32 sum;                              /* Sum of the pixels in the
                                                   paxel for one color */
        int32 fv_sum_1;                         /* FV sum 1 in the paxel */
        int32 fv_sum_2;                         /* FV sum 2 in the paxel */
        int32 valid;                            /* Flag indicating whether
                                                   current paxel is valid
                                                   0:invalid, !0:valid */
    };


/* AF paxel - struct af_h3a_paxel */
    struct af_h3a_paxel {
        struct af_h3a_color_paxel green;        /* Paxel information for
                                                   Green color */
        //NOT USED IM MOVIDIUS implementation
//        struct af_h3a_color_paxel red_blue;     /* Paxel information for
//                                                   Red/Blue colors */
//        struct af_h3a_color_paxel blue_red;     /* Paxel information for
//                                                   Blue/Red colors */
        float  paxel_lens_pos;
    };


/* struct window size*/
    struct wins_size {
        int32 left;         /* Starting up left position of rectange  in pixels*/
        int32 top;          /* Starting top position of rectangle in pixels */
        int32 width;        /* Rectangle width in pixels*/
        int32 height;       /* Rectangle haight in pixels*/
    };

#define FW_FACE_DET_NUMBER (35)     /* Number of max detected faces */
#define FW_REGION_PRIORITY_NUMBER (35)  /* Number of max region priorities */
#define FW_EXP_REGION_PRIORITY_NUMBER (35)  /* Number of max region priorities */

/*
    Following table contain complete AF algorithm configuration. Structure was
    filled from 3AFW and send to Auto focus algorithm using *_control() API.
    AF configuration - struct af_configuration
*/
    struct focus_configuration {
        dtpdb_static_common_t *     dtp_s_common;
        focus_spot_weighting_t      spot_mode;
        enum focus_mode             mode;
        enum focus_control          control;
        uint8                       manual_position; //!< Manual focus position
        uint8                       face_tracking_state;
        uint8                       region_priority_state;
        uint32                      exposure_time;  //!< Current frame exposure time.
        uint32                      analog_gain;    //!< Current frame analog gain.
        void                        *eeprom_buff;   //!< Ponter to EEPROM buffer
        uint32                      eeprom_size;
    };

    /* Cropping, Zoom Rectangle - struct window */
        struct win_rect {
            int32  left;        /* Starting up left position of rectange */
            int32  top;         /* Starting top position of rectangle */
            uint32 width;       /* Rectangle width */
            uint32 height;      /* Rectangle haight */
        };

    /* Structure for face detection result */
        struct face_det_result {
            uint8           update;     /* Update counter (counter = 0 - structure not changed,  counter > 0 - update */
            uint8           enable;
            struct win_rect face_box[FW_FACE_DET_NUMBER]; /* The bounding box of face (sizes converted in manager from pixels to relative) */
            int32           face_num;   /* Number of faces detected */
            int32           face_angle; /* The face angle 1:0 angle 2:90 angle, 3:270 angle */
        };

        /* Structure for region priority */
            struct region_priority {
                uint8           update;     /* Update counter (counter = 0 - structure not changed,  counter > 0 - update */
                uint8           enable;
                struct win_rect reg_box[FW_REGION_PRIORITY_NUMBER]; /* The bounding box of face (sizes converted in manager from pixels to relative) */
                uint32          priority[FW_REGION_PRIORITY_NUMBER];   /* priority for each region - as a per-pixel weight, values from 1 to priority_max*/
                uint32          priority_max;   /* The highest priority value */
                int32           reg_num;    /* Number of faces detected */
            };

            struct   focus_lens_info {
                int32                           cur_lens_pos;                 //!< Current lens position.
                uint8                           lens_moving;                  //!< If true hllc still moving the lens
                uint8                           external_lens_move;
                int32							actual_lens_position;
            };
/*
Data supplied to AF algorithm input can be seen in next table.
AF statistic structure - struct af_calculation_input
*/

    struct af_calculation_input {
        uint32                 win_count;       /* Total number of AF H3A Windows. */
        struct af_h3a_paxel   *paxels;          /* Output from AF statistic engine. */
        uint32                 frame_number;    /* Number of frame. */
        uint32                 exposure_time;   /* Current AE request exposure time. */
        float                 analog_gain;     /* Current AE request analog gain. */
        uint32                 ae_target_texp;  /* Current AE target total exposure. */
        uint32                 sensor_exp;      /* Current frame exposure time. */
        float                 sensor_gain;     /* Current frame analog gain. */
        osal_timeval           timestamp;       /* Timestamp of the statistics (end of WOI). */
        struct face_det_result face_detect;     /* Structure for face detection */
        struct region_priority region_pri;      /* Structure for region priority */
        uint16                 h3a_start_x;     /* H3A horizontal start position. */
        uint16                 h3a_size_x;      /* H3A horizontal size. */
        uint16                 h3a_start_y;     /* H3A vertical start position. */
        uint16                 h3a_size_y;      /* H3A vertical size. */
        uint16                 h3a_hz_cnt;      /* H3A horizontal paxels count. */
        uint16                 h3a_vt_cnt;      /* H3A vertical paxels count. */
        uint16                 h3a_decim_x;     /* H3A horizontal decimation. */
        uint16                 h3a_decim_y;     /* H3A vertical decimation. */
        uint16                 ae_conv_sts;     /* AE algo convergence status*/
        struct focus_lens_info lens_info;
        aca_fmv_calc_output_t   fmv_input;
    };

/*
Output from AF algorithm is represented in next table and it is filled from
algorithm at the end of *_process() API.
AF computation complete - struct af_calculation_output
*/
    struct af_calculation_output {
        uint32 distance_mm;
        uint32 near_distance_mm;
        uint32 far_distance_mm;
        int32  lens_pos;
        enum focus_mode     mode;
        /* with continuous AF might have several bits on */
        focus_state_t state;
        /* Last status of AF/CAF algorithms*/
        enum focus_result   result;
        /* Coordinates of converged active window. Valid only when state is AF_STATE_SUCCESS. */
        struct focus_window af_window;
        //Coordinates AF/CAF focus on
        struct af_bfz_coordinates bfz_coord;
        hllc_do_cmd_t              lens_do;
    };


    struct af_framework_status {
        uint32 distance_mm;
        uint32 near_distance_mm;
        uint32 far_distance_mm;
        uint32 lens_pos;
        enum focus_mode     mode;
        /* with continuous AF might have several bits on */
        focus_state_t state;
        /* Last status of AF/CAF algorithms*/
        enum focus_result   result;
        /* Coordinates of converged active window. Valid only when state is AF_STATE_SUCCESS. */
        struct focus_window af_window;
        //Coordinates AF/CAF focus on
        struct af_bfz_coordinates bfz_coord;
    };

    enum af_h3a_acc_mode {
        AF_H3A_ACC_SUM,
        AF_H3A_ACC_PEAK
    };

/* Red, Green, and blue pixel location in the AF windows */
    enum affw_h3a_rgbpos {
        AFFW_H3A_GR_GB_BAYER,    /* GR and GB as Bayer pattern */
        AFFW_H3A_RG_GB_BAYER,    /* RG and GB as Bayer pattern */
        AFFW_H3A_GR_BG_BAYER,    /* GR and BG as Bayer pattern */
        AFFW_H3A_RG_BG_BAYER,    /* RG and BG as Bayer pattern */
        AFFW_H3A_GG_RB_CUSTOM,   /* GG and RB as custom pattern */
        AFFW_H3A_RB_GG_CUSTOM    /* RB and GG as custom pattern */
    };

/**
 *  Structure that contains output parameters for the *_create() API of AFFW.
 */
    struct af_config_output {
        uint32                hor_cnt; /* Horizontal number of H3A windows. */
        uint32                ver_cnt; /* Vertical number of H3A windows. */
        uint32                win_h_size_num; /* H3A window horizontal size part numerator (denominator is 256). */
        uint32                win_v_size_num; /* H3A window vertical size part numerator (denominator is 256). */
        int32                 max_exp_time; /* Maximum exposure time. */
        //enum af_h3a_alaw      h3a_alaw; /* Enabled or disabled. */
        uint8                 hmf_enable; /* Flag for HMF enable/disable. */
        uint8 assist_request;       /* Flag shows if AF request calculation with AF assist*/
        uint32                hmf_threshold;
        enum af_h3a_acc_mode  h3a_mode; /* H3A Accumulator mode */
        struct h3a_af_filters filts; /* H3A filters. */
        hllc_do_cmd_t              lens_do;
     };

    /* Enumerate for 3AFW modes */
        enum mode_3a {
            CAMERA_DEFAULT,
            CAMERA_PREVIEW_HQ,
            CAMERA_PREVIEW_HS,
            CAMERA_PREVIEW_VIDEO,
            CAMERA_HS_CAPTURE,
            CAMERA_HQ_CAPTURE,
            CAMERA_PREVIEW_GESTURE,
            CAMERA_PREVIEW_ZSL,
        };

        typedef struct {
            uint8 *pEepromBuff;
            uint32 nRawDataSize;
            void  *pParsedEEpromData;
        } eeprom_data_t;

    /* generic structure which chould be passes to all
     algorithm frameworks during creaion time */
        struct fw_create_cofiguration {
            /* Dynamic Tunning server handler,
            should be used for all request to this server. */
            void *dtpserver;
            eeprom_data_t *eepromData;
            enum mode_3a mode;
        };


/**
 * Structure that contains create parameters for AFFW.
 */
    struct af_create_params {
        struct fw_create_cofiguration create;      //!< Create parameters.
        struct focus_configuration    configuration; //!< Input configuration.
        struct af_config_output       output;  //!< Output data.
    };

/**
 * Structure that contains control parameters for AFFW.
 */
    struct af_control_params {
        struct focus_configuration *configuration; //!< Input configuration.
        struct af_config_output    *output;        //!< Output data.
    };


/* ============================================================================
 *                                   CODE SECTION
 * ========================================================================== */


/* ========================================================================== */
/**
*   @func auto_focus_create()
*
*   @brief Create Auto Focus algorithm instance.
*
*   The above API should be used to create Auto Focus algorithm instance.
*   On successful return from this function first argument will point to
*   algorithm object and will be used as reference for sequential access to
*   exported algorithm functionality. This parameter is opaque to 3AFW and
*   is responsibility for algorithm to fill appropriate information. Second
*   parameter can be used from 3AFW to setup initial algorithm processing
*   parameters.
*
*   @param  algorithm - void ** - Algorithm descriptor.
*   @param  params - struct af_create_params * - Function parameters.
*
*   @return int - (0) - Success, (1) - Fail.
*/
/* ========================================================================== */
    int aca_focus_create(void * *algorithm, struct af_create_params *params);

/* ========================================================================== */
/**
*   @func auto_focus_delete()
*
*   @brief Delete Auto Focus algorithm instance.
*
*   This API should be used when it is needed to remove and free any resources
*   owned from Auto Focus algorithm. First argument is descriptor returned
*   from auto_focus_create() API call.
*
*   @param   algorithm - void * - AFFW algorithm context.
*
*   @return int - (0) - Success, (1) - Fail.
*/
/* ========================================================================== */
    int aca_focus_destroy(void *algorithm);

/* ========================================================================== */
/**
*   @brief auto_focus_control()
*
*   @brief Use this API for fine algorithm tunning.
*
*   This API will be used to configure, start and stop the Auto Focus
*   algorithm. First argument is descriptor returned from auto_focus_create() API call.
*
*   @param  algorithm - void * - Algorithm descriptor.
*   @param  params - struct af_control_params * - Function parameters.
*
*   @return int - (0) - Success, (1) - Fail.
*/
/* ========================================================================== */
    int auto_focus_control(void *algorithm, struct af_control_params *params);

/* ========================================================================== */
/**
*   @func auto_focus_process()
*
*   @brief Main Auto Focus calculation function.
*
*   Above API will be called when 3AFW read new H3A Auto Focus statistic buffer
*   from Camera driver. Input parameter hold address of H3A statistic buffer,
*   it is not allowed algorithm to keep reference to this buffer after
*   competition of this function. 3AFW is free to reuse memory occupied from
*   statistic for other purposes. On successful competition of this call Auto
*   Focus algorithm should return its current status. This API call should be
*   completed for maximum 5 msec. 3AFW will process output from algorithm and
*   change ISP configuration. Responsibility of Auto focus algorithm framework
*   is to move lens.
*
*   @param  algorithm - void * - Algorithm descriptor.
*   @param  input - struct af_calculation_input * - Input data.
*   @param  output - struct af_calculation_output * - Output data.
*
*   @return int - (0) - Success, (1) - Fail.
*/
/* ========================================================================== */
    int auto_focus_process(void *algorithm, struct af_calculation_input *input, struct af_calculation_output *output);

/* ========================================================================== */
/**
*  auto_focus_mknote()    Get AF part of maker note data.
*
*  @param   algorithm - void * - algorithm descriptor
*
*  @param   data  - void * - pointer to AF part of maker note data
*
*  @param   size - uint32 - Size of AF part of maker note data
*
*  @return  0 for success else error code (EBUSY, EINVAL)
*/
/* ========================================================================== */
    int auto_focus_mknote(void *algorithm, void *data, uint32 size);

/* ========================================================================== */
/**
*  auto_focus_status()    Get AF status
*
*  @param   algorithm - void * - algorithm descriptor
*
*  @param   data  - struct af_framework_status* - where to store data.
*
*  @return  0 for success else error code (EBUSY, EINVAL)
*/
/* ========================================================================== */
    int auto_focus_status(void *algorithm, struct af_framework_status *data);

/* =============================================================================
*                                   CODE SECTION
* =========================================================================== */


#endif /* __FW_AF_INTERFACE_H__ */

