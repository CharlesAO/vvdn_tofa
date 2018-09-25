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
* @file hat_cm_socket.h
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
#ifndef __HAT_CM_SOCKET_H__
#define __HAT_CM_SOCKET_H__

#include <hal/hat_types.h>
#include <hal/hal_camera_module/hat_cm_hw_resource.h>
#include <platform/inc/plat_socket_pub.h>

typedef enum {
    SOCKET_STATE_UNKNOWN,
    SOCKET_STATE_CREATED,
    SOCKET_STATE_OPENED,
    SOCKET_STATE_CLOSED,
} HAT_CM_SOCKET_STATE_T;

typedef struct {
    PLAT_SOCKET_COMPONENT_T     comp;
    int                         rsrc_type;
    HAT_HW_RESOURCE_ACTION_T    act;
} hat_cm_socket_rsrc_action_descr_t;

typedef union {
    hat_cm_socket_i2c_cmd_t i2c_cmd;
//TODO:    hat_cm_socket_spi_cmd_t spi_cmd;
} hat_cm_socket_rsrc_cmd_t;

typedef enum {
    CMD_DIR_WR,
    CMD_DIR_RD
} HAT_CM_SOCKET_CMD_DIR_T;

typedef struct {
    PLAT_SOCKET_COMPONENT_T      comp;
    int                          rsrc_type;
    PLAT_SOCKET_HW_RSRC_TYPE_T   hw_type;
    HAT_CM_SOCKET_CMD_DIR_T      dir;
    hat_cm_socket_rsrc_cmd_t     cmd;
} hat_cm_socket_rsrc_command_descr_t;

typedef struct {
    hat_cm_socket_rsrc_command_descr_t read_cmd;
    uint8                              *expected_val;
} hat_cm_socket_rsrc_check_descr_t;

typedef union {
    hat_socket_comp_cfg_t i2c_cfg;
//TODO:    hat_cm_socket_spi_cfg_t spi_cmd;
} hat_cm_socket_rsrc_cfg_t;

typedef struct {
    PLAT_SOCKET_COMPONENT_T      comp;
    int                          rsrc_type;
    PLAT_SOCKET_HW_RSRC_TYPE_T   hw_type;
    hat_cm_socket_rsrc_cfg_t     rsrc_cfg;
} hat_cm_socket_rsrc_config_descr_t;

//TODO:
typedef void hat_cm_socket_cci_event_prms_t;
typedef void hat_cm_socket_imgif_event_prms_t;
typedef void hat_cm_socket_imgif_buffer_prms_t;

typedef enum {
    RSRC_INTENT_TYPE_ACTION,
    RSRC_INTENT_TYPE_COMMAND,
    RSRC_INTENT_TYPE_CHECK,
    RSRC_INTENT_TYPE_CONFIG,
} HAL_CM_SOCKET_RSRC_INTENT_TYPE_T;

typedef union {
    hat_cm_socket_rsrc_action_descr_t   action;
    hat_cm_socket_rsrc_command_descr_t  command;
    hat_cm_socket_rsrc_check_descr_t    check;
    hat_cm_socket_rsrc_config_descr_t   config;
} hat_cm_socket_rsrc_intent_descr_t;

typedef struct {
    HAL_CM_SOCKET_RSRC_INTENT_TYPE_T    type;
    hat_cm_socket_rsrc_intent_descr_t   desc;
} hat_cm_socket_rsrc_intent_t;

typedef uint32 hat_cm_socket_delay_us_t;

typedef int (*hat_cm_socket_smart_func_t)(void* in, void* out);

typedef struct {
    hat_cm_socket_smart_func_t  func;
    void*                       in;
    void*                       out;
} hat_cm_socket_smart_func_desc_t;

typedef enum {
    COMMAND_ENTRY_TYPE_DUMMY,
    COMMAND_ENTRY_TYPE_DELAY,
    COMMAND_ENTRY_TYPE_INTENT,
    COMMAND_ENTRY_TYPE_SMART_FUNC,
    COMMAND_ENTRY_TYPE_END,
} HAL_CM_SOCKET_COMMAND_ENTRY_TYPE_T;

typedef union {
    hat_cm_socket_delay_us_t        delay;
    hat_cm_socket_rsrc_intent_t     intent;
    hat_cm_socket_smart_func_desc_t smart_func;
} hat_cm_socket_command_entry_descr_t;

typedef struct {
    HAL_CM_SOCKET_COMMAND_ENTRY_TYPE_T    type;
    hat_cm_socket_command_entry_descr_t   desc;
} hat_cm_socket_command_entry_t;

typedef struct {
    uint32  read_val;
    int     match;
} hat_cm_socket_cmd_entry_result_t;

typedef struct {
    int detected;
} hat_cm_socket_cmd_strip_result_t;

typedef struct {
    int id;
    char *name;
} hat_cm_socket_cr_prms_t;

typedef struct hat_cm_socket_obj hat_cm_socket_obj_t;

typedef hat_cm_socket_obj_t* hat_cm_socket_handle_t;

typedef struct {
    hat_cm_socket_handle_t virt_socket_hndl;
    struct {
        int  id;
        char *name;
        void *hndl;
    } sen;
    struct {
        int  id;
        char *name;
        void *hndl;
    } lens;
    struct {
        int  id;
        char *name;
        void *hndl;
    } lights;
    struct {
        int  id;
        char *name;
        void *hndl;
    } nvm;
} hat_cm_socket_detect_result_t;

struct hat_cm_socket_obj {
    void * hal_socket_prvt;
    int (*hal_socket_get_state)(hat_cm_socket_handle_t hndl, HAT_CM_SOCKET_STATE_T *state);
    int (*hal_socket_create)(hat_cm_socket_handle_t hndl);
    int (*hal_socket_rsrc_set)(hat_cm_socket_handle_t hndl,
        hat_cm_socket_rsrc_action_descr_t *action);
    int (*hal_socket_rsrc_cfg)(hat_cm_socket_handle_t hndl,
                    hat_cm_socket_rsrc_config_descr_t *sock_cfg);
    int (*hal_socket_write)(hat_cm_socket_handle_t hndl,
        hat_cm_socket_rsrc_command_descr_t *cmd);
    int (*hal_socket_read)(hat_cm_socket_handle_t hndl,
        hat_cm_socket_rsrc_command_descr_t *cmd);
    int (*hal_socket_cci_event_set)(hat_cm_socket_handle_t hndl,
        PLAT_SOCKET_COMPONENT_T comp,
        hat_cm_socket_cci_event_prms_t *cci_event_prms);
    int (*hal_socket_imgif_event_set)(hat_cm_socket_handle_t hndl,
        PLAT_SOCKET_COMPONENT_T comp,
        hat_cm_socket_imgif_event_prms_t *imgif_event_prms);
    int (*hal_socket_imgif_buffer_set)(hat_cm_socket_handle_t hndl,
        PLAT_SOCKET_COMPONENT_T comp,
        hat_cm_socket_imgif_buffer_prms_t *imgif_buffer_prms);
    int (*hal_socket_open)(hat_cm_socket_handle_t hndl,
        hat_cm_socket_open_params_t *params);
    int (*hal_socket_close)(hat_cm_socket_handle_t hndl);
    int (*hal_socket_destroy)(hat_cm_socket_handle_t hndl);
    int (*hal_socket_query)(hat_cm_socket_handle_t hndl,
        hat_cm_socket_query_params_t query, void* data);
};

typedef int (*hat_cm_socket_cmd_strip_executor_t)(hat_cm_socket_handle_t socket_hndl,
    hat_cm_socket_command_entry_t *cmd_strip,
    hat_cm_socket_cmd_strip_result_t* res);

typedef int (*hat_cm_socket_cmd_entry_executor_t)(hat_cm_socket_handle_t socket_hndl,
    hat_cm_socket_command_entry_t *cmd_entry,
    hat_cm_socket_cmd_entry_result_t* res);


/******************************************************************************/
/*                                                                            */
/* Common macros                                                              */
/*                                                                            */
/******************************************************************************/
#define LE16(VAL) \
    ((uint8)(         (VAL) & 0x00FF      )), \
    ((uint8)((uint16)((VAL) & 0xFF00) >> 8))

#define LE24(VAL) \
    ((uint8)(         (VAL) & 0x000000FF       )), \
    ((uint8)((uint32)((VAL) & 0x0000FF00) >>  8)), \
    ((uint8)((uint32)((VAL) & 0x00FF0000) >> 16)), \

#define LE32(VAL) \
    ((uint8)(         (VAL) & 0x000000FF       )), \
    ((uint8)((uint32)((VAL) & 0x0000FF00) >>  8)), \
    ((uint8)((uint32)((VAL) & 0x00FF0000) >> 16)), \
    ((uint8)((uint32)((VAL) & 0xFF000000) >> 24))

#define BE16(VAL) \
    ((uint8)((uint16)((VAL) & 0xFF00) >> 8)), \
    ((uint8)(         (VAL) & 0x00FF      ))

#define BE32(VAL) \
    ((uint8)((uint32)((VAL) & 0xFF000000) >> 24)), \
    ((uint8)((uint32)((VAL) & 0x00FF0000) >> 16)), \
    ((uint8)((uint32)((VAL) & 0x0000FF00) >>  8)), \
    ((uint8)(         (VAL) & 0x000000FF       ))

#define BUFSET8(BUF, VAL) \
do { \
    ((uint8 *)(BUF))[0] = ((uint8)((VAL) & 0xFF)); \
} while (0)

#define BUF_SET_LE16(BUF, VAL) \
do { \
    ((uint8 *)(BUF))[0] = ((uint8)(         (VAL) & 0x00FF      )); \
    ((uint8 *)(BUF))[1] = ((uint8)((uint16)((VAL) & 0xFF00) >> 8)); \
} while (0)

#define BUF_SET_LE24(BUF, VAL) \
do { \
    ((uint8 *)(BUF))[0] = ((uint8)(         (VAL) & 0x000000FF       )); \
    ((uint8 *)(BUF))[1] = ((uint8)((uint32)((VAL) & 0x0000FF00) >>  8)); \
    ((uint8 *)(BUF))[2] = ((uint8)((uint32)((VAL) & 0x00FF0000) >> 16)); \
} while (0)

#define BUF_SET_LE32(BUF, VAL) \
do { \
    ((uint8 *)(BUF))[0] = ((uint8)(         (VAL) & 0x000000FF       )); \
    ((uint8 *)(BUF))[1] = ((uint8)((uint32)((VAL) & 0x0000FF00) >>  8)); \
    ((uint8 *)(BUF))[2] = ((uint8)((uint32)((VAL) & 0x00FF0000) >> 16)); \
    ((uint8 *)(BUF))[3] = ((uint8)((uint32)((VAL) & 0xFF000000) >> 24)); \
} while (0)

#define BUF_SET_BE16(BUF, VAL) \
do { \
    ((uint8 *)(BUF))[1] = ((uint8)(         (VAL) & 0x00FF      )); \
    ((uint8 *)(BUF))[0] = ((uint8)((uint16)((VAL) & 0xFF00) >> 8)); \
} while (0)

#define BUF_SET_BE24(BUF, VAL) \
do { \
    ((uint8 *)(BUF))[2] = ((uint8)(         (VAL) & 0x000000FF       )); \
    ((uint8 *)(BUF))[1] = ((uint8)((uint32)((VAL) & 0x0000FF00) >>  8)); \
    ((uint8 *)(BUF))[0] = ((uint8)((uint32)((VAL) & 0x00FF0000) >> 16)); \
} while (0)

#define BUF_SET_BE32(BUF, VAL) \
do { \
    ((uint8 *)(BUF))[3] = ((uint8)(         (VAL) & 0x000000FF       )); \
    ((uint8 *)(BUF))[2] = ((uint8)((uint32)((VAL) & 0x0000FF00) >>  8)); \
    ((uint8 *)(BUF))[1] = ((uint8)((uint32)((VAL) & 0x00FF0000) >> 16)); \
    ((uint8 *)(BUF))[0] = ((uint8)((uint32)((VAL) & 0xFF000000) >> 24)); \
} while (0)

#define GET_LE16_BUF(BUF) \
            ((uint16)(((((uint8 *)(BUF))[0])       & 0x00FF) | \
                     (((((uint8 *)(BUF))[1]) << 8) & 0xFF00)))

#define GET_LE32_BUF(BUF) \
            ((uint32)(((((uint8 *)(BUF))[0])        & 0x000000FF) | \
                     (((((uint8 *)(BUF))[1]) << 8)  & 0x0000FF00) | \
                     (((((uint8 *)(BUF))[2]) << 16) & 0x00FF0000) | \
                     (((((uint8 *)(BUF))[3]) << 24) & 0xFF000000)))

#define GET_BE16_BUF(BUF) \
            ((uint16)(((((uint8 *)(BUF))[0]) << 8) & 0xFF00) | \
                      ((((uint8 *)(BUF))[1])       & 0x00FF))

#define GET_BE32_BUF(BUF) \
            ((uint32)(((((uint8 *)(BUF))[0]) << 24) & 0xFF000000) | \
                     (((((uint8 *)(BUF))[1]) << 16) & 0x00FF0000) | \
                     (((((uint8 *)(BUF))[2]) << 8)  & 0x0000FF00) | \
                      ((((uint8 *)(BUF))[3])        & 0x000000FF))

#define ARR_SIZE(a) (sizeof(a)/sizeof(a[0]))

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CMD_ENTRY_END()                                     \
    {                                                       \
        .type = COMMAND_ENTRY_TYPE_END,                     \
    }                                                       \

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CMD_ENTRY_DUMMY()                                   \
    {                                                       \
        .type = COMMAND_ENTRY_TYPE_DUMMY,                   \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_ACTION(VSOCK, rsrc, val)          \
    {                                                       \
        .type = COMMAND_ENTRY_TYPE_INTENT,                  \
        .desc = {                                           \
            .intent = {                                     \
                .type = RSRC_INTENT_TYPE_ACTION,            \
                .desc = {                                   \
                    .action = {                             \
                    .comp = SOCKET_COMP_##VSOCK,          \
                    .rsrc_type = rsrc,                  \
                        .act = val                          \
                    },                                      \
                },                                          \
            },                                              \
        },                                                  \
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_CMD_CCI_I2C_WR(VSOCK, RSRC_TYPE, REG, ...)                       \
    {                                                                   \
        .type = COMMAND_ENTRY_TYPE_INTENT,                              \
        .desc = {                                                       \
            .intent = {                                                 \
                .type = RSRC_INTENT_TYPE_COMMAND,                       \
                .desc = {                                               \
                    .command = {                                        \
                        .comp = SOCKET_COMP_ ## VSOCK,                   \
                        .rsrc_type = RSRC_TYPE ## _RSRC_CCI,             \
                        .hw_type = HW_RSRC_I2C,                         \
                        .dir = CMD_DIR_WR,                              \
                        .cmd = {                                        \
                            .i2c_cmd = {                                \
                                .reg = REG,                             \
                                .num = sizeof((uint8 []){__VA_ARGS__}), \
                                .val = (uint8 []){__VA_ARGS__},         \
                            },                                          \
                        },                                              \
                    },                                                  \
                },                                                      \
            },                                                          \
        },                                                              \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_CMD_CCI_I2C_WR_BUF(VSOCK, RSRC_TYPE, REG, NUM, BUF)                   \
    {                                                                   \
        .type = COMMAND_ENTRY_TYPE_INTENT,                              \
        .desc = {                                                       \
            .intent = {                                                 \
                .type = RSRC_INTENT_TYPE_COMMAND,                       \
                .desc = {                                               \
                    .command = {                                        \
                        .comp = SOCKET_COMP_ ## VSOCK,                  \
                        .rsrc_type = RSRC_TYPE ## _RSRC_CCI,            \
                        .hw_type = HW_RSRC_I2C,                         \
                        .dir = CMD_DIR_WR,                              \
                        .cmd = {                                        \
                            .i2c_cmd = {                                \
                                .reg = REG,                             \
                                .num = NUM,                             \
                                .val = BUF,                             \
                            },                                          \
                        },                                              \
                    },                                                  \
                },                                                      \
            },                                                          \
        },                                                              \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_CMD_CCI_I2C_RD(VSOCK, RSRC_TYPE, REG, NUM, PTR) \
    {                                                       \
        .type = COMMAND_ENTRY_TYPE_INTENT,                  \
        .desc = {                                           \
            .intent = {                                     \
                .type = RSRC_INTENT_TYPE_COMMAND,           \
                .desc = {                                   \
                    .command = {                            \
                        .comp = SOCKET_COMP_ ## VSOCK,      \
                        .rsrc_type = RSRC_TYPE ##_RSRC_CCI, \
                        .hw_type = HW_RSRC_I2C,             \
                        .dir = CMD_DIR_RD,                  \
                        .cmd = {                            \
                            .i2c_cmd = {                    \
                                .reg = REG,                 \
                                .num = NUM,                 \
                                .val = PTR,                 \
                            },                              \
                        },                                  \
                    },                                      \
                },                                          \
            },                                              \
        },                                                  \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_CMD_CCI_I2C_RD_CHECK(VSOCK, RSRC_TYPE, REG, ...)                    \
    {                                                                       \
        .type = COMMAND_ENTRY_TYPE_INTENT,                                  \
        .desc = {                                                           \
            .intent = {                                                     \
                .type = RSRC_INTENT_TYPE_CHECK,                             \
                .desc = {                                                   \
                    .check = {                                              \
                        .read_cmd = {                                       \
                            .comp = SOCKET_COMP_ ## VSOCK,                   \
                            .rsrc_type = RSRC_TYPE ## _RSRC_CCI,             \
                            .hw_type = HW_RSRC_I2C,                         \
                            .dir = CMD_DIR_RD,                              \
                            .cmd = {                                        \
                                .i2c_cmd = {                                \
                                    .reg = REG,                             \
                                    .num = sizeof((uint8 []){__VA_ARGS__}), \
                                    .val = NULL,                            \
                                },                                          \
                            },                                              \
                        },                                                  \
                        .expected_val = (uint8 []){__VA_ARGS__},            \
                    },                                                      \
                },                                                          \
            },                                                              \
        },                                                                  \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_CMD_CCI_I2C_CONFIG(VSOCK, RSRC_TYPE, DEV_ADDR, REG_SIZE, ADDR_BIT_SZ)       \
    {                                                                       \
        .type = COMMAND_ENTRY_TYPE_INTENT,                                  \
        .desc = {                                                           \
            .intent = {                                                     \
                .type = RSRC_INTENT_TYPE_CONFIG,                            \
                .desc = {                                                   \
                    .config = {                                             \
                        .comp = SOCKET_COMP_ ## VSOCK,                      \
                        .rsrc_type = RSRC_TYPE ## _RSRC_CCI,                \
                        .hw_type = HW_RSRC_I2C,                             \
                        .rsrc_cfg = {                                       \
                            .i2c_cfg = {                                    \
                                .address = DEV_ADDR,                        \
                                .register_size = REG_SIZE,                  \
                                .slave_addr_bit_size = ADDR_BIT_SZ,         \
                            },                                              \
                        },                                                  \
                    },                                                      \
                },                                                          \
            },                                                              \
        },                                                                  \
    }                                                                       \

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, ...) \
do { \
    entry.desc.intent.desc.command.cmd.i2c_cmd.num = sizeof((uint8 []){__VA_ARGS__}); \
    entry.desc.intent.desc.command.cmd.i2c_cmd.val = (uint8 []){__VA_ARGS__}; \
} while (0)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)   \
    entry.desc.intent.desc.command.cmd.i2c_cmd.reg = new_val

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define RSRC_DELAY(val)                     \
    {                                                                   \
        .type = COMMAND_ENTRY_TYPE_DELAY,   \
        .desc = {                                                       \
            .delay = val,                   \
    }                                                                   \
    } 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SENS_INTENT_CMD_CLOCK_I2C_WR(REG, ...)                            \
    {                                                                   \
        .type = COMMAND_ENTRY_TYPE_INTENT,                              \
        .desc = {                                                       \
            .intent = {                                                 \
                .type = RSRC_INTENT_TYPE_COMMAND,                       \
                .desc = {                                               \
                    .command = {                                        \
                        .comp = SOCKET_COMP_SENSOR,                      \
                        .rsrc_type = SEN_RSRC_CLOCK,                     \
                        .hw_type = HW_RSRC_I2C,                         \
                        .dir = CMD_DIR_WR,                              \
                        .cmd = {                                        \
                            .i2c_cmd = {                                \
                                .reg = REG,                             \
                                .num = sizeof((uint8 []){__VA_ARGS__}), \
                                .val = (uint8 []){__VA_ARGS__},         \
                            },                                          \
                        },                                              \
                    },                                                  \
                },                                                      \
            },                                                          \
        },                                                              \
    }

/*##################################################       SENSOR      #############################################*/
#define SENS_INTENT_ACTION(sen_rsrc, val)                           RSRC_INTENT_ACTION(SENSOR, sen_rsrc, val)
#define SENS_INTENT_CMD_CCI_I2C_WR(REG, ...)                        RSRC_INTENT_CMD_CCI_I2C_WR(SENSOR, SEN,  REG, ##__VA_ARGS__)
#define SENS_INTENT_CMD_CCI_I2C_WR_BUF(REG, NUM, BUF)               RSRC_INTENT_CMD_CCI_I2C_WR_BUF(SENSOR, SEN,  REG, NUM, BUF)
#define SENS_INTENT_CMD_CCI_I2C_RD(REG, NUM, PTR)                   RSRC_INTENT_CMD_CCI_I2C_RD(SENSOR, SEN,  REG, NUM, PTR)
#define SENS_INTENT_CMD_CCI_I2C_RD_CHECK(REG, ...)                  RSRC_INTENT_CMD_CCI_I2C_RD_CHECK(SENSOR, SEN,  REG, __VA_ARGS__)
#define SENS_INTENT_CMD_CCI_I2C_CONFIG(DEV_ADDR, REG_SIZE, ADDR_BIT_SZ) RSRC_INTENT_CMD_CCI_I2C_CONFIG(SENSOR, SEN, DEV_ADDR, REG_SIZE, ADDR_BIT_SZ)
#define SENS_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, ...)           RSRC_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, __VA_ARGS__)
#define SENS_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)  RSRC_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)
#define SENS_DELAY(val)                                             RSRC_DELAY(val) // delay in microsecons

/*##################################################        LENS       #############################################*/
#define LENS_INTENT_ACTION(sen_rsrc, val)                           RSRC_INTENT_ACTION(LENS,   sen_rsrc, val)
#define LENS_INTENT_CMD_CCI_I2C_WR(REG, ...)                        RSRC_INTENT_CMD_CCI_I2C_WR(LENS,   LENS, REG, ##__VA_ARGS__)
#define LENS_INTENT_CMD_CCI_I2C_WR_BUF(REG, NUM, BUF)               RSRC_INTENT_CMD_CCI_I2C_WR_BUF(LENS,   LENS, REG, NUM, BUF)
#define LENS_INTENT_CMD_CCI_I2C_RD(REG, NUM, PTR)                   RSRC_INTENT_CMD_CCI_I2C_RD(LENS,   LENS, REG, NUM, PTR)
#define LENS_INTENT_CMD_CCI_I2C_RD_CHECK(REG, ...)                  RSRC_INTENT_CMD_CCI_I2C_RD_CHECK(LENS,   LENS, REG, __VA_ARGS__)
#define LENS_INTENT_CMD_CCI_I2C_CONFIG(DEV_ADDR,REG_SIZE,ADDR_BIT_SZ)        RSRC_INTENT_CMD_CCI_I2C_CONFIG(LENS, LENS, DEV_ADDR,REG_SIZE,ADDR_BIT_SZ)
#define LENS_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, ...)           RSRC_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, __VA_ARGS__)
#define LENS_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)  RSRC_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)
#define LENS_DELAY(val)                                             RSRC_DELAY(val) // delay in microsecons

/*##################################################         NVM      #############################################*/
#define NVM_INTENT_ACTION(nvm_rsrc, val)                            RSRC_INTENT_ACTION(NVM,    nvm_rsrc, val)
#define NVM_INTENT_CMD_CCI_I2C_WR(REG, ...)                         RSRC_INTENT_CMD_CCI_I2C_WR(NVM,    NVM,  REG, ##__VA_ARGS__)
#define NVM_INTENT_CMD_CCI_I2C_WR_BUF(REG, NUM, BUF)                RSRC_INTENT_CMD_CCI_I2C_WR_BUF(NVM,    NVM,  REG, NUM, BUF)
#define NVM_INTENT_CMD_CCI_I2C_RD(REG, NUM, PTR)                    RSRC_INTENT_CMD_CCI_I2C_RD(NVM,    NVM,  REG, NUM, PTR)
#define NVM_INTENT_CMD_CCI_I2C_RD_CHECK(REG, ...)                   RSRC_INTENT_CMD_CCI_I2C_RD_CHECK(NVM,    NVM,  REG, __VA_ARGS__)
#define NVM_INTENT_CMD_CCI_I2C_CONFIG(DEV_ADDR,REG_SIZE,ADDR_BIT_SZ)         RSRC_INTENT_CMD_CCI_I2C_CONFIG(NVM, NVM, DEV_ADDR,REG_SIZE,ADDR_BIT_SZ)
#define NVM_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, ...)            RSRC_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, __VA_ARGS__)
#define NVM_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)   RSRC_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)
#define NVM_DELAY(val)                                              RSRC_DELAY(val) // delay in microsecons


/*##################################################         LIGHT      #############################################*/
#define LIGHT_INTENT_ACTION(nvm_rsrc, val)                            RSRC_INTENT_ACTION(NVM,    nvm_rsrc, val)
#define LIGHT_INTENT_CMD_CCI_I2C_WR(REG, ...)                         RSRC_INTENT_CMD_CCI_I2C_WR(NVM,    NVM,  REG, ##__VA_ARGS__)
#define LIGHT_INTENT_CMD_CCI_I2C_WR_BUF(REG, NUM, BUF)                RSRC_INTENT_CMD_CCI_I2C_WR_BUF(NVM,    NVM,  REG, NUM, BUF)
#define LIGHT_INTENT_CMD_CCI_I2C_RD(REG, NUM, PTR)                    RSRC_INTENT_CMD_CCI_I2C_RD(NVM,    NVM,  REG, NUM, PTR)
#define LIGHT_INTENT_CMD_CCI_I2C_RD_CHECK(REG, ...)                   RSRC_INTENT_CMD_CCI_I2C_RD_CHECK(NVM,    NVM,  REG, __VA_ARGS__)
#define LIGHT_INTENT_CMD_CCI_I2C_CONFIG(DEV_ADDR,REG_SIZE,ADDR_BIT_SZ)         RSRC_INTENT_CMD_CCI_I2C_CONFIG(NVM, NVM, DEV_ADDR,REG_SIZE,ADDR_BIT_SZ)
#define LIGHT_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, ...)            RSRC_INTENT_CMD_CCI_I2C_WR_VAL_UPDATE(entry, __VA_ARGS__)
#define LIGHT_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)   RSRC_INTENT_CMD_CCI_I2C_WR_REG_ADDR_UPDATE(entry, new_val)
#define LIGHT_DELAY(val)                                              RSRC_DELAY(val) // delay in microsecons



#endif // __HAT_CM_SOCKET_H__
