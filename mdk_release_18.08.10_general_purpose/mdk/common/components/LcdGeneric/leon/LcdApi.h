///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup LcdGeneric LCD Generic API
/// @ingroup  LcdGeneric
/// @{
/// @brief     LCD Generic Component Functions API.
///
/// This is the API to the LCD controller subsystem component
///

#ifndef _LCD_API_H_
#define _LCD_API_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "LcdApiDefines.h"
#include "swcFrameTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// @brief Set callbacks for assign new frame and frame ready.
/// @param[in] hndl        - Pointer to the lcd handle
/// @param[in] assignFrame - Function pointer to get new frame
/// @param[in] frameReady  - Function pointer for frame ready
/// @param[in] highres 	   - Function pointer to set high frequency resolution
/// @param[in] lowres 	   - Function pointer to set low frequency resolution
/// @return    void
///
void LCDSetupCallbacks(LCDHandle* hndl,
        allocateLcdFn*  assignFrame, frameReadyLcdFn* frameReady, freqLcdFn *highres, freqLcdFn *lowres);

/// @brief This will Stop the LCD interface
/// @param[in] hndl - Pointer to the lcd handle
/// @return    void
///
void LCDStop(LCDHandle *hndl);

/// @brief This will initialize the LCD interface
/// @param[in] hndl   - Pointer to the lcd handle
/// @param[in] lcdsp  - LCD configuration
/// @param[in] fsp    - Frame specification
/// @param[in] lcdNum - LCD number 0 or 1
/// @return    void
///
void LCDInit(LCDHandle *hndl, const LCDDisplayCfg *lcdsp, const frameSpec *fsp, unsigned int lcdNum);

/// @brief This will one of the LCD layers if fsp argument of LCDInit is NULL
/// @param[in] hndl     - Pointer to the lcd handle
/// @param[in] layer    - LCD layer ( VL1, VL2, GL1 or GL2)
/// @param[in] fsp      - Frame spec
/// @param[in] position - layer position on the screen (from top-left corner)
/// @return    void
///
void LCDInitLayer(LCDHandle *hndl, int layer, frameSpec *fsp, LCDLayerOffset position);

/// @brief This will enable YUV420p to Yuv422i format conversion on LCD interface
/// @param[in] -
/// @return    void
///
void LCDEnYuv422i(void);

/// @brief This will start the LCD interface
/// @param[in] hndl - Pointer to the lcd handle
/// @return    void
///
void LCDStart(LCDHandle *hndl);

/// @brief This will start the LCD interface in one-shot mode
///
/// Of the callbacks that can be set up with LCDSetupCallbacks,
/// only the frameReady callback is used, and it is optional.
/// In one-shot mode the LCD interface may own a minimum of zero,
/// or a maximum of two frames: one that is currently being displayed,
/// and one that is in the shadow registers. If you have successfully
/// queued 3 frames, then you may assume that the first one you queued
/// is done, and free to use for other purposes. The frame can become
/// available much sooner then two consecutive frames, so it's best
/// to use the frameReady callback to know when a frame is done.
/// @param[in] hndl - Pointer to the lcd handle
/// @return    void
///
void LCDStartOneShot(LCDHandle *hndl);

/// @brief Queue a frame in One-Shot mode
/// @param[in] handle    - Pointer to the lcd handle
/// @param[in] VL1Buffer - Video Layer 1 frameBuffer pointer (may be NULL if the layer is not enabled)
/// @param[in] VL2Buffer - Video Layer 2
/// @param[in] GL1Buffer - Graphics Layer 1
/// @param[in] GL2Buffer - Graphics Layer 2
/// @return     		
///						 - 0 on success
///						 - <0 on error (for example if two frames were already queued, and none were given back)
///
int LCDQueueFrame(LCDHandle *handle, frameBuffer *VL1Buffer, frameBuffer *VL2Buffer, frameBuffer *GL1Buffer, frameBuffer *GL2Buffer);

/// @brief Tells you whether you can successfully queue a frame in one-shot mode
/// @param[in]  handle - Pointer to the lcd handle
/// @return     	  
///					   - zero, if the LCD already has two frames it hasn't given back.
///					   - non-zero, if a new frame can be queued.
int LCDCanQueueFrame(LCDHandle *handle);

/// @brief This will enable video layer 1
/// @param[in] hndl - Pointer to the lcd handle
/// @return    void
///
void LCDInitVL2Enable(LCDHandle *hndl);

/// @brief This will disable video layer 1
/// @param[in] hndl - Pointer to the lcd handle
/// @return    void
///
void LCDInitVL2Disable(LCDHandle *hndl);

/// @brief This will enable graphical layer
/// @param[in] hndl  - 	Pointer to the lcd handle
/// @param[in] layer -  1 for GLl or 2 for GL2
/// @param[in] fr 	 - 	graphical layer frame spec
/// @return    void
///
void LCDInitGLEnable(LCDHandle *hndl, int layer, const frameSpec *fr);

/// @brief This will enable graphical layer
/// @param[in]  hndl  - Pointer to the lcd handle
/// @param[in]  layer - 1 for GLl or 2 for GL2
/// @return     void
///
void LCDInitGLDisable(LCDHandle *hndl, int layer);

/// @brief This will enable graphical layer
/// @param[in] hndl 	  -	Pointer to the lcd handle
/// @param[in] layer 	  -	graphical layer number
/// @param[in] colorTable - color table
/// @param[in] number 	  - number of colors from table
/// @return    void
///
///@note  needs to be called again after LcdStop call can be used during runtime.
///
void LCDSetColorTable(LCDHandle *hndl, int layer, unsigned int *colorTable, int number);

/// Sets output datapath of LCD (Parallel port or Mipi)
/// @param[in] hndl     - Pointer to the lcd handle
/// @param[in] dataPath - type of datapath (LCD_TO_PARALLEL or LCD_TO_MIPI)
/// @return    void
///
void LCDSetOutput(LCDHandle *hndl, lcdDatapath_t dataPath);


#ifdef __cplusplus
}
#endif

/// @}
#endif // _LCD_API_H_
