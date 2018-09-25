/// =====================================================================================
///
///        @file:      poolSizes.h
///        @brief:     Defines for flic pipe pool sizes
///        @author:    csoka, attila.csok@movidius.com
///        @copyright: All code copyright Movidius Ltd 2013, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------

/// Application Includes
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Max sizes
/// Mipi source
#define POOL_SRC_W      (4056)
#define POOL_SRC_H      (3040)
#define POOL_SRC_FSZ    (POOL_SRC_W * POOL_SRC_H * 5/4) /// 1.2 bpp (packed 10bpp)
#define POOL_SRC_COUNT  (3)
#define POOL_SRC_SZ     (POOL_SRC_FSZ * POOL_SRC_COUNT)
/// Mipi Mono source
#define POOL_MONO_SRC_W      (1280)
#define POOL_MONO_SRC_H      (720)
#define POOL_MONO_SRC_FSZ    (POOL_MONO_SRC_W * POOL_MONO_SRC_H * 2) /// 2 bpp
#define POOL_MONO_SRC_COUNT  (3)
#define POOL_MONO_SRC_SZ     (POOL_MONO_SRC_FSZ * POOL_MONO_SRC_COUNT)
/// ISP Video Output
#define POOL_ISP_VDO_W      (4032)
#define POOL_ISP_VDO_H      (3040)
#define POOL_ISP_VDO_FSZ    (POOL_ISP_VDO_W * POOL_ISP_VDO_H * 3 / 2) /// 1.5 bpp
#define POOL_ISP_VDO_COUNT  (4)
#define POOL_ISP_VDO_SZ     (POOL_ISP_VDO_FSZ * POOL_ISP_VDO_COUNT)
/// Mono ISP Video Output
#define POOL_MONO_ISP_VDO_W      (1296)
#define POOL_MONO_ISP_VDO_H      (816)
#define POOL_MONO_ISP_VDO_FSZ    (POOL_MONO_ISP_VDO_W * POOL_MONO_ISP_VDO_H) /// 1 bpp
#define POOL_MONO_ISP_VDO_COUNT  (4)
#define POOL_MONO_ISP_VDO_SZ     (POOL_MONO_ISP_VDO_FSZ * POOL_MONO_ISP_VDO_COUNT)
/// ISP Still Output
#define POOL_ISP_STL_W      (4032) /// croping to alling with JPEG
#define POOL_ISP_STL_H      (3040)
#define POOL_ISP_STL_FSZ    (POOL_ISP_STL_W * POOL_ISP_STL_H * 3 / 2) /// 1.5 bpp
#define POOL_ISP_STL_COUNT  (1)
#define POOL_ISP_STL_SZ     (POOL_ISP_STL_FSZ * POOL_ISP_STL_COUNT)
/// ISP Post processor - Big frame
#define POOL_PPB_W     (3840)
#define POOL_PPB_H     (2160)
#define POOL_PPB_FSZ   (POOL_PPB_W * POOL_PPB_H * 3 / 2) /// 1.5 bpp
#define POOL_PPB_COUNT (6)
#define POOL_PPB_SZ    (POOL_PPB_FSZ * POOL_PPB_COUNT)
/// ISP Post processor - Small frame
#define POOL_PPS_W     (3840 / 3) /// 720p
#define POOL_PPS_H     (2160 / 3) /// 720p
#define POOL_PPS_FSZ   (POOL_PPS_W * POOL_PPS_H * 3 / 2) /// 1.5 bpp
#define POOL_PPS_COUNT (3)
#define POOL_PPS_SZ    (POOL_PPS_FSZ * POOL_PPS_COUNT)
/// Encoder - heap allocated so don't include in RGN_POOL_SZ
#define POOL_ENC_COUNT  (5)
#define POOL_ENC_FSZ    (2*1024*1024)
#define POOL_ENC_SZ     (POOL_ENC_FSZ * POOL_ENC_COUNT)

#define POOL_JPEG_COUNT  (2)
#define POOL_JPEG_FSZ    (2*1024*1024)
#define POOL_JPEG_SZ     (POOL_JPEG_FSZ * POOL_JPEG_COUNT)

#define POOL_SW_ENC_COUNT       (6)
#define POOL_SW_ENC_W           (1280)
#define POOL_SW_ENC_H           (720)
#define POOL_SW_ENC_FSZ         (256*1024)
#define POOL_SW_ENC_SZ          (POOL_SW_ENC_FSZ * POOL_SW_ENC_COUNT)

#define POOL_STEREO_COUNT       (3)
#define POOL_STEREO_W           (1280)
#define POOL_STEREO_H           (720)
#define POOL_STEREO_FSZ         (POOL_STEREO_W * POOL_STEREO_H)
#define POOL_STEREO_SZ          (POOL_STEREO_FSZ * POOL_STEREO_COUNT * 3) // DISP + DEPTH

#define RGN_POOL_PADDING_LOSS_OS   (1*1024*1024)
#define RGN_POOL_PADDING_LOSS_RT   (2*1024*1024)

#define RGN_POOL_SIZE_OS   (RGN_POOL_PADDING_LOSS_OS)

#define RGN_POOL_SIZE_RT   (POOL_SRC_SZ + \
                            POOL_MONO_SRC_SZ + \
                            POOL_ISP_VDO_SZ + \
                            POOL_ISP_STL_SZ + \
                            POOL_PPB_SZ + \
                            POOL_PPS_SZ + \
                            POOL_ENC_SZ + \
                            POOL_SW_ENC_SZ +\
                            POOL_MONO_ISP_VDO_SZ + \
                            POOL_STEREO_SZ + \
                            RGN_POOL_PADDING_LOSS_RT)

#define PLUGINS_MEM_SECT    SECTION(".cmx_direct.data")
#define CMX_DATA_S15 		SECTION(".shv15.S.data")
