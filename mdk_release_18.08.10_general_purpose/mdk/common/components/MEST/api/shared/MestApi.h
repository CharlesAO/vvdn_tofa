///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     MEST Component Functions API.
///
/// This is the API to the MEST subsystem component
///

#ifndef INC_MESTAPI_H_
#define INC_MESTAPI_H_

#include <stdint.h>

#include <mvMacros.h>

#include <MestPublicTypes.h>

#ifndef ALIGNED
    #define ALIGNED(x) __attribute__((aligned(x)))
#endif

// functions

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void *privateData;
} mestHandler_t;

mestError_t mestInitStaticConfigVGA(mestParamConfig_t* config);
mestRuntimeConfig_t mestGetRuntimeConfig(const mestHandler_t *hnd);
mestError_t mestSetRuntimeConfig(mestHandler_t *hnd, const mestRuntimeConfig_t *config);
mestError_t mestMutexInit();
mestError_t mestMutexDeinit();
mestError_t mestInit(
        mestHandler_t *hnd,
        const mestMode_t mode,
        const mestResourceConfig_t* memory,
        const mestParamConfig_t *config);
mestError_t mestDestroy();
mestError_t mestRun(
        mestHandler_t *hnd,
        frameBuffer prevFrame,
        frameBuffer currFrame,
        mestBulkOutput_t* result);
uint32_t mestGetResultSize(mestHandler_t *hnd);
int mestRunning(mestHandler_t *hnd);

/**
 * Adds a ROI to internal queue.
 * @param [in] roi          - ROI pointer to the element that needs to be added in queue
 * @param [in] locations    - If needed, a custom target search points array can be supplied.
 *                            Use NULL if it is configured for RASTER mode.
 * @param [in] locationsCnt - Size of the 'locations' array. Value is ignored if 'locations' is NULL.
 * @param [in] policy       - Search policy for MEST. If is set to one of the RASTER modes, set 'locations' to NULL.
 * @param [in] id           - ID of the current ROI
 * @return                  - MEST_ROI_SUCCESS
 *                            MEST_ROI_ERROR
 *                            MEST_ROI_INVALID_POINTS
 */
mestError_t mestAddROI(
    mestHandler_t *hnd,
    const mestROI_t *roi,
    const xyPos_t *locations,
    const uint32_t locationsCnt,
    const mestFeatureSelPolicy_t policy,
    mestROIId_t id
    );

/**
 * Removes a ROI from queue by its ID.
 * @param [in] id - ID of the ROI to be removed
 * @return        - MEST_SUCCESS
 *                  MEST_ROI_NOT_FOUND
 */
mestError_t mestRemoveROI(mestHandler_t *hnd, const mestROIId_t id);

/**
 * Removes all ROIs from queue.
 * @return        - MEST_SUCCESS
 */
mestError_t mestRemoveAllROI(mestHandler_t *hnd);

/**
 * Replaces a ROI from queue specified by ID with a new ROI.
 * @param [in] id     - ID of the ROI to be replaced
 * @param [in] newRoi - new ROI that replaces the old one
 * @return            - MEST_SUCCESS
 *                      MEST_ROI_NOT_FOUND
 */
mestError_t mestUpdateROI(
        mestHandler_t *hnd,
        const mestROIId_t id,
        const mestROI_t * newRoi);

/**
 * Replaces a ROI's list of points to search.
 * @param [in] id           - ID of the ROI to be updated
 * @param [in] locations    - new array of coordinates
 * @param [in] locationsCnt - new array length
 * @return                  - MEST_SUCCESS
 *                            MEST_ROI_NOT_FOUND
 *                            MEST_ROI_QUEUE_EMPTY
 *                            MEST_ROI_INVALID_POINTS
 */
mestError_t mestUpdateROIFeatures(
        mestHandler_t *hnd,
        const mestROIId_t id,
        xyPos_t* locations,
        uint32_t locationsCnt);

/**
 * Get the next ROI ID that will be inserted in queue.
 * @return The ID of the next element in queue that will be inserted.
 */
mestROIId_t mestROIgetNextId(mestHandler_t *hnd);

/**
 * Writes a buffer to a binary file.
 * @param [in] buf - Address of the buffer to be written
 * @param [in] sz  - Buffer length
 * @param [in] fn  - Filename
 */
//void debugDump(void *buf, int sz, const char *fn);

#ifdef __cplusplus
}
#endif

#define MEST_GET_COORDS_HEAP_BYTES(ptsCnt) (ptsCnt * sizeof(uint32_t))
#define MEST_GET_RES_HEAP_BYTES(ptsCnt) (ptsCnt * sizeof(uint32_t))
#define MEST_GET_RES_BYTES(ptsCnt) (\
        ALIGN_UP( (ptsCnt) * sizeof((*(mestMeta_t*)(0)).outputFlows[0]), 8 ) + \
        (ptsCnt) * sizeof((*(mestMeta_t*)(0)).costs[0]) + \
        sizeof(mestOutput_t) \
    )

extern const uint16_t MEST_WRONG_INLIER_COST;

#endif /* INC_MESTAPI_H_ */
