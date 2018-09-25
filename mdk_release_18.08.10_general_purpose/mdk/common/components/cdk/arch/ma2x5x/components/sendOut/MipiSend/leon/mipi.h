///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// -----------------------------------------------------------------------------
///
/// Revision History
/// ===================================
/// 05-Nov-2014 : Author ( MM Solutions AD )
/// Created
/// =============================================================================

#ifndef _MIPI_H
#define _MIPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* MIPI Data Types */
enum {

/* CSI2 Data Types */

    /* Sync short packet */
    MIPI_DT_CSI2_SYNC_SHORT_FS         = 0x00,
    MIPI_DT_CSI2_SYNC_SHORT_FE         = 0x01,
    MIPI_DT_CSI2_SYNC_SHORT_LS         = 0x02,
    MIPI_DT_CSI2_SYNC_SHORT_LE         = 0x03,
    MIPI_DT_CSI2_SYNC_SHORT_RESERVED_1 = 0x04,
    MIPI_DT_CSI2_SYNC_SHORT_RESERVED_2 = 0x05,
    MIPI_DT_CSI2_SYNC_SHORT_RESERVED_3 = 0x06,
    MIPI_DT_CSI2_SYNC_SHORT_RESERVED_4 = 0x07,

    /* Generic short packet */
    MIPI_DT_CSI2_GEN_SHORT_1 = 0x08,
    MIPI_DT_CSI2_GEN_SHORT_2 = 0x09,
    MIPI_DT_CSI2_GEN_SHORT_3 = 0x0a,
    MIPI_DT_CSI2_GEN_SHORT_4 = 0x0b,
    MIPI_DT_CSI2_GEN_SHORT_5 = 0x0c,
    MIPI_DT_CSI2_GEN_SHORT_6 = 0x0d,
    MIPI_DT_CSI2_GEN_SHORT_7 = 0x0e,
    MIPI_DT_CSI2_GEN_SHORT_8 = 0x0f,

    /* Generic long packet */
    MIPI_DT_CSI2_GEN_LONG_NULL          = 0x10,
    MIPI_DT_CSI2_GEN_LONG_BLANKING_DATA = 0x11,
    MIPI_DT_CSI2_GEN_LONG_EMBEDDED_8BIT = 0x12,
    MIPI_DT_CSI2_GEN_LONG_RESERVED_1    = 0x13,
    MIPI_DT_CSI2_GEN_LONG_RESERVED_2    = 0x14,
    MIPI_DT_CSI2_GEN_LONG_RESERVED_3    = 0x15,
    MIPI_DT_CSI2_GEN_LONG_RESERVED_4    = 0x16,
    MIPI_DT_CSI2_GEN_LONG_RESERVED_5    = 0x17,

    /* YUV Data */
    MIPI_DT_CSI2_YUV_420_8BIT         = 0x18,
    MIPI_DT_CSI2_YUV_420_10BIT        = 0x19,
    MIPI_DT_CSI2_YUV_420_LEGACY_8BIT  = 0x1a,
    MIPI_DT_CSI2_YUV_RESERVED_1       = 0x1b,
    MIPI_DT_CSI2_YUV_420_8BIT_CSHIFT  = 0x1c,
    MIPI_DT_CSI2_YUV_420_10BIT_CSHIFT = 0x1d,
    MIPI_DT_CSI2_YUV_422_8BIT         = 0x1e,
    MIPI_DT_CSI2_YUV_422_10BIT        = 0x1f,

    /* RGB Data */
    MIPI_DT_CSI2_RGB_444        = 0x20,
    MIPI_DT_CSI2_RGB_555        = 0x21,
    MIPI_DT_CSI2_RGB_565        = 0x22,
    MIPI_DT_CSI2_RGB_666        = 0x23,
    MIPI_DT_CSI2_RGB_888        = 0x24,
    MIPI_DT_CSI2_RGB_RESERVED_1 = 0x25,
    MIPI_DT_CSI2_RGB_RESERVED_2 = 0x26,
    MIPI_DT_CSI2_RGB_RESERVED_3 = 0x27,

    /* RAW Data */
    MIPI_DT_CSI2_RAW_6          = 0x28,
    MIPI_DT_CSI2_RAW_7          = 0x29,
    MIPI_DT_CSI2_RAW_8          = 0x2a,
    MIPI_DT_CSI2_RAW_10         = 0x2b,
    MIPI_DT_CSI2_RAW_12         = 0x2c,
    MIPI_DT_CSI2_RAW_14         = 0x2d,
    MIPI_DT_CSI2_RAW_RESERVED_1 = 0x2e,
    MIPI_DT_CSI2_RAW_RESERVED_2 = 0x2f,

    /* User defined byte-based data */
    MIPI_DT_CSI2_USER_1 = 0x30,
    MIPI_DT_CSI2_USER_2 = 0x31,
    MIPI_DT_CSI2_USER_3 = 0x32,
    MIPI_DT_CSI2_USER_4 = 0x33,
    MIPI_DT_CSI2_USER_5 = 0x34,
    MIPI_DT_CSI2_USER_6 = 0x35,
    MIPI_DT_CSI2_USER_7 = 0x36,
    MIPI_DT_CSI2_USER_8 = 0x37,

    /* Reserved */
    MIPI_DT_CSI2_RESERVED_1 = 0x38,
    MIPI_DT_CSI2_RESERVED_2 = 0x39,
    MIPI_DT_CSI2_RESERVED_3 = 0x3a,
    MIPI_DT_CSI2_RESERVED_4 = 0x3b,
    MIPI_DT_CSI2_RESERVED_5 = 0x3c,
    MIPI_DT_CSI2_RESERVED_6 = 0x3d,
    MIPI_DT_CSI2_RESERVED_7 = 0x3e,
    MIPI_DT_CSI2_RESERVED_8 = 0x3f,
};

#ifdef __cplusplus
}
#endif

#endif /* _MIPI_H */

