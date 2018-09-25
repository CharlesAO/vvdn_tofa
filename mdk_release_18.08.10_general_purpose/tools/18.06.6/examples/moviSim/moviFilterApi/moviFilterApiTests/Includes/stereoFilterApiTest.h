#ifndef __STEREO_FILTER_API_TEST_H__
#define __STEREO_FILTER_API_TEST_H__

#include <string.h>

#include "filtersConfiguration.h"
#include "filterInterface.h"

#include <os/os.h>
#include <os/SharedLibrary.h>

using namespace std;

class StereoFilterApiTest
{
public:
    StereoFilterApiTest(const char* resourcesPath);
    ~StereoFilterApiTest();

#pragma region Run Tests
    void testStereoApi();
#pragma endregion

#pragma region Dynamic library handling
    void loadLibrary(const char* libName);
    void unloadLibrary();
#pragma endregion

private:
    // @brief Solid-Color u8-input CT only various modes, u8 input
    //Operating on just 1 plane (could also have 2 input/output planes)
    uint32_t stereoCensusTransConfig(uint64_t iBufAdr, uint64_t oBufAdr, uint64_t eBufAdr, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask);

    // @brief real-life dense-depth (operation = CT_CM) all output modes, all debug outputs
    //Operating on just 1 plane (could also have 2 input/output planes)
    uint32_t stereoCensusTransCompandingConfig(uint64_t* iBuf, uint64_t oBuf0, uint64_t oBuf1, StereoOut outMode, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask, NumDisp nd, DbgDump dd);

    // @brief real-life dense-depth (NOWRAP mode)
    //Operating on just 1 plane (could also have 2 input/output planes)
    uint32_t stereoNoWrapConfig(uint64_t* iBuf, uint64_t oBuf1, StereoOut outMode, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask, NumDisp nd, DbgDump dd);

    // @brief CT_CM + SGBM-only => FULL
    uint32_t stereoCensusTransCompandingSgbmAlgoConfig(uint64_t iBuf0, uint64_t iBuf1, uint64_t oBufS0, StereoMode op, DbgDump dbgDmp);

    // @brief real-life dense-depth (operation = CT_CM) 10bit input - FLAT
    //Operating on just 1 plane (could also have 2 input/output planes)
    uint32_t stereoCensusTransCompanding10bitFlatAndPackConfig(uint64_t* iBuf, uint64_t oBuf0, uint64_t oBuf1, StereoOut outMode, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask, NumDisp nd, uint32_t iFmt);

#pragma  region Functional tests full frame
    //[CT-only] solid-color
    void ctSolidColorPv0Ct32();
    void ctSolidColorPv0Ct24();
    void ctSolidColorPv0Ct64();
    void ctSolidColorPv0Ct56();

    //[CT-only] random-data
    void ctRandomDataPv0Ct32();
    void ctRandomDataPv0Ct64();
    void ctRandomDataPv0Ct32BigMask();
    void ctRandomDataPv0Ct64BigMask();

    // [CT_CM] all output modes, all debug outputs
    void ctCmOmodeDspSingle();
    void ctCmOmodeDspCd();
    void ctCmOmodeDspMc();
    void ctCmOmodeDspMap();
    void ctCmOmodeDspComb();

    void ctDisp();
    void ctDbgMap64();
    void ctDbgMap96();

    void cmDisp64();
    void cmDisp96();

    // [NOWRAP mode]
    void noWrapMode();

    // [CT_CM + SGBM-only => FULL]
    void ctCmFull();
    void sgbmFull();

    // [CT_CM 10bit Flat and 10bit Pack]
    void ctCm10BitFlat();
    void ctCm10BitPack();
#pragma endregion

private:
    static void(StereoFilterApiTest::*stereoFilterApiTestMethods[])();

    os::SharedLibrary* stereoLib;

    StereoConfiguration* stereoConfig;
    FilterInterface* stereoFilter;

    string resourcesPath;
};
#endif
