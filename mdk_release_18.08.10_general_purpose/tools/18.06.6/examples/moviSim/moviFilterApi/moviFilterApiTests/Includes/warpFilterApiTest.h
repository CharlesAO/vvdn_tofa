#ifndef __WARP_FILTER_API_TEST_H__
#define __WARP_FILTER_API_TEST_H__

#include "filtersConfiguration.h"
#include "filterInterface.h"

#include <os/os.h>
#include <os/SharedLibrary.h>

class WarpFilterApiTest
{
public:
    WarpFilterApiTest();
    ~WarpFilterApiTest();

#pragma region Run Tests
    void testWarpApi();
#pragma endregion

#pragma region Dynamic library handling
    void loadLibrary(const char* libName);
    void unloadLibrary();
#pragma endregion

private:

    void warpDefaultConfiguration(void *iBuf, void *mesh, uint32_t meshW, uint32_t meshH, uint32_t meshMode, uint32_t outW, uint32_t outH, uint8_t *eBuf, uint32_t interp, float *mat3x3, uint32_t edgeCol);

#pragma  region Functional tests full frame
    // Pass through
    void passThroughMeshDiasable();
    void passThroughMat3x3Diasable();
    void passThroughMat3x3Enabled();

    // Flip
    void flipVertically();
    void flipHorizontally();
    void flipVerticallyHorizontally();

    //Integer Shift Translation
    void intShiftTransEdgeReplication();
    void intShiftTransClearColor();

    // Rotation
    void cw90degRotation();
    void ccw90degRotation();

    //Rescale
    void rescaleHalfMeshSizeOne();
    void rescaleHalfMeshSizeTwo();
    void rescaleDuplicate();

    // Pre-expanded mesh and fractional shift
    void passThroughPreExpandedRelative();
    void intShiftPreExpandedRelative();
    void fractionalShiftViaMat3x3();
#pragma endregion

private:
    static void(WarpFilterApiTest::*warpFilterApiTestMethods[])();

    os::SharedLibrary* warpLib;

    WarpConfiguration * warpConfig;
    FilterInterface* warpFilter;
};
#endif
