#ifndef __DPU_FILTER_API_TEST_H__
#define __DPU_FILTER_API_TEST_H__

#include "filtersConfiguration.h"
#include "filterInterface.h"

#include <os/os.h>
#include <os/SharedLibrary.h>

constexpr uint32_t CMX_DEPTH = 0x180000;

class DpuFilterApiTest
{
public:
    DpuFilterApiTest();
    ~DpuFilterApiTest();

#pragma region Run Tests
    void testDpuApi();
#pragma endregion

#pragma region Dynamic library handling
    void loadLibrary(const char* libName);
    void unloadLibrary();
#pragma endregion

private:

#pragma  region Functional tests full frame
    void ApbWriteReadSimulation();
#pragma endregion

    void dpuApbWriteStartRegister();

private:
    static void(DpuFilterApiTest::*dpuFilterApiTestMethods[])();

    void dpuApbWrites();

    os::SharedLibrary* dpuLib;

    FilterInterface* dpuFilter;
};
#endif
