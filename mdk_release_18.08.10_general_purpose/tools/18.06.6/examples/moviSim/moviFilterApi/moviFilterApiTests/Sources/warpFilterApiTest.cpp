#include <memory>
#include <string.h>
#include <iostream>
#include <assert.h> 

#include "warpFilterApiTest.h"
#include "warpResources.h"

void(WarpFilterApiTest::*WarpFilterApiTest::warpFilterApiTestMethods[])() = {
    &WarpFilterApiTest::passThroughMeshDiasable,
    &WarpFilterApiTest::passThroughMat3x3Diasable,
    &WarpFilterApiTest::passThroughMat3x3Enabled,
    &WarpFilterApiTest::flipVertically,
    &WarpFilterApiTest::flipHorizontally,
    &WarpFilterApiTest::flipVerticallyHorizontally,
    &WarpFilterApiTest::intShiftTransEdgeReplication,
    &WarpFilterApiTest::intShiftTransClearColor,
    &WarpFilterApiTest::cw90degRotation,
    &WarpFilterApiTest::ccw90degRotation,
    &WarpFilterApiTest::rescaleHalfMeshSizeOne,
    &WarpFilterApiTest::rescaleHalfMeshSizeTwo,
    &WarpFilterApiTest::rescaleDuplicate,
    &WarpFilterApiTest::passThroughPreExpandedRelative,
    &WarpFilterApiTest::intShiftPreExpandedRelative,
    &WarpFilterApiTest::fractionalShiftViaMat3x3,
};

WarpFilterApiTest::WarpFilterApiTest()
{
    warpConfig = new WarpConfiguration();
}

WarpFilterApiTest::~WarpFilterApiTest()
{
    delete warpConfig;
}

void WarpFilterApiTest::testWarpApi()
{
    if (!warpLib->isLoaded())
    {
        printf("Library '%s' is not loaded!!!\n", "WARP" );
        return;
    }

    createFilterObjectPtr createObject = nullptr;
    if (warpLib->getProcAddress("createFilterObject", createObject))
    {
        constexpr char* filterName = "WARP";

        printf("START TESTING: %s \n", filterName);

        warpFilter = createObject(filterName);

        for (auto testMethod : warpFilterApiTestMethods)
        {
            (this->*testMethod)();
        }

        printf("END TESTING: %s ALL TESTS PASSED\n", filterName);
    }
}

void WarpFilterApiTest::loadLibrary(const char* libName)
{
    warpLib = new os::SharedLibrary(libName);
}

void WarpFilterApiTest::unloadLibrary()
{
    if (warpLib->isLoaded())
    {
        warpLib->unload();
    }
}

void WarpFilterApiTest::warpDefaultConfiguration(void *iBuf, void *mesh, uint32_t meshW, uint32_t meshH, uint32_t meshMode, uint32_t outW, uint32_t outH, uint8_t *eBuf, uint32_t interp, float *mat3x3, uint32_t edgeCol)
{
    memset(warpConfig, 0, sizeof(WarpConfiguration));
    warpConfig->base = 0;
    warpConfig->rows = 1;

    warpConfig->inW = IMG_W;
    warpConfig->inH = IMG_H;
    warpConfig->outW = outW;
    warpConfig->outH = outH;

    warpConfig->meshW = meshW;
    warpConfig->meshH = meshH;
    warpConfig->meshMode = meshMode;

    warpConfig->tileW_pwr2 = TILE_W_PWR2;
    warpConfig->tileW = 1 << warpConfig->tileW_pwr2;
    warpConfig->tileH_pwr2 = TILE_H_PWR2;
    warpConfig->tileH = 1 << warpConfig->tileH_pwr2;

    warpConfig->full[W_IN] = iBuf;
    warpConfig->full[W_MESH] = mesh;
    warpConfig->full[W_OUT] = oBuf;
    warpConfig->cBuf[W_OUT].base = 0;
    warpConfig->cBuf[W_OUT].h = 0;
    warpConfig->mat3x3 = mat3x3;
    warpConfig->interp = interp;

    warpConfig->edgeCol = edgeCol;
    warpConfig->meshStartYi = nullptr; //used only in IRQ mode
    warpConfig->meshStartYf = nullptr;

    warpConfig->runs = 0;    //full-frame
    warpConfig->start = (1 << 1) | 1;//do 1x Super-Run
}

void WarpFilterApiTest::passThroughMeshDiasable()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, nullptr, 0, 0, 0, IMG_W, IMG_H, iBuf, NONE, mat3x3NN, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, iBuf, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}


void WarpFilterApiTest::passThroughMat3x3Diasable()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshA, MESH_W, MESH_H, S_F32, IMG_W, IMG_H, iBuf, BILIN, nullptr, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, iBuf, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::passThroughMat3x3Enabled()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshA, MESH_W, MESH_H, S_F32, IMG_W, IMG_H, iBuf, BILIN, mat3x3I, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, iBuf, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::flipVertically()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshB, MESH_W, MESH_H, S_F32, IMG_W, IMG_H, eBufB, BILIN, nullptr, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufB, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::flipHorizontally()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshC, MESH_W, MESH_H, S_F32, IMG_W, IMG_H, eBufC, BILIN, nullptr, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufC, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::flipVerticallyHorizontally()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshD, MESH_W, MESH_H, S_F32, IMG_W, IMG_H, eBufD, NONE, mat3x3NN, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufD, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::intShiftTransEdgeReplication()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshA, MESH_W, MESH_H, S_F32, IMG_W, IMG_H, eBufE, BILIN, mat3x3T0, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufE, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::intShiftTransClearColor()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshA, MESH_W, MESH_H, S_F32, IMG_W, IMG_H, eBufE2, BILIN, mat3x3T0, COLOR(CCOL));

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufE2, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::cw90degRotation()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshF, MESH_H, MESH_W, S_F32, IMG_H, IMG_W, eBufF, NONE, mat3x3NN, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufF, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::ccw90degRotation()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshG, MESH_H, MESH_W, S_F32, IMG_H, IMG_W, eBufG, NONE, mat3x3NN, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufG, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::rescaleHalfMeshSizeOne()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshH, 4, 2, S_F32, IMG_W / 2, IMG_H / 2, eBufH, NONE, mat3x3NN, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufH, (IMG_H / 2) * (IMG_W / 2));

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::rescaleHalfMeshSizeTwo()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshK, 4, 2, S_F32, IMG_W / 2, IMG_H / 2, eBufK, NONE, mat3x3NN, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufK, (IMG_H / 2) * (IMG_W / 2));

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::rescaleDuplicate()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshA, MESH_W, MESH_H, S_F32, IMG_W * 2, IMG_H * 2, eBufJ, NONE, mat3x3NN, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufJ, (IMG_H * 2) * (IMG_W * 2));

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::passThroughPreExpandedRelative()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshL, IMG_W, IMG_H, D_I16_REL, IMG_W, IMG_H, iBuf, BILIN, nullptr, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, iBuf, IMG_H * IMG_W );

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::intShiftPreExpandedRelative()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBuf, meshM, IMG_W, IMG_H, D_I16_REL, IMG_W, IMG_H, eBufE2, BILIN, NULL, COLOR(CCOL));

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufE2, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void WarpFilterApiTest::fractionalShiftViaMat3x3()
{
    printf("TESTING: %s ", __func__);

    warpDefaultConfiguration(iBufN, meshL, IMG_W, IMG_H, D_I16_REL, IMG_W, IMG_H, eBufN, BILIN, mat3x3Dx, REPL);

    // CALL SET CONFIGURATION FROM THE API
    warpFilter->configure(warpConfig);
    // CALL RUN FROM THE API
    warpFilter->start();

    auto testResult = memcmp(oBuf, eBufN, IMG_H * IMG_W);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}
