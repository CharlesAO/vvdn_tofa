#include <memory>
#include <iostream>
#include <assert.h> 

#include "stereoFilterApiTest.h"
#include "filterApiTestsUtils.h"

#include "stereoResourcesCt.h"
#include "stereoResourcesCtCm.h"

void(StereoFilterApiTest::*StereoFilterApiTest::stereoFilterApiTestMethods[])() = {
    &StereoFilterApiTest::ctSolidColorPv0Ct32,
    &StereoFilterApiTest::ctSolidColorPv0Ct24,
    &StereoFilterApiTest::ctSolidColorPv0Ct64,
    &StereoFilterApiTest::ctSolidColorPv0Ct56,
    &StereoFilterApiTest::ctRandomDataPv0Ct32,
    &StereoFilterApiTest::ctRandomDataPv0Ct64,
    &StereoFilterApiTest::ctRandomDataPv0Ct32BigMask,
    &StereoFilterApiTest::ctRandomDataPv0Ct64BigMask,
    &StereoFilterApiTest::ctCmOmodeDspSingle,
    &StereoFilterApiTest::ctCmOmodeDspCd,
    &StereoFilterApiTest::ctCmOmodeDspMc,
    &StereoFilterApiTest::ctCmOmodeDspMap,
    &StereoFilterApiTest::ctCmOmodeDspComb,
    &StereoFilterApiTest::ctDisp,
    &StereoFilterApiTest::ctDbgMap64,
    &StereoFilterApiTest::ctDbgMap96,
    &StereoFilterApiTest::cmDisp64,
    &StereoFilterApiTest::cmDisp96,
    &StereoFilterApiTest::noWrapMode,
    &StereoFilterApiTest::ctCmFull,
    &StereoFilterApiTest::sgbmFull,
    &StereoFilterApiTest::ctCm10BitFlat,
    &StereoFilterApiTest::ctCm10BitPack
};

StereoFilterApiTest::StereoFilterApiTest(const char* resourcesPath):
    resourcesPath(resourcesPath)
{
    stereoConfig = new StereoConfiguration();

}

StereoFilterApiTest::~StereoFilterApiTest()
{
    delete stereoConfig;
}

void StereoFilterApiTest::testStereoApi()
{
    constexpr char* filterName = "STEREO";

    if (!stereoLib->isLoaded())
    {
        printf("Library '%s' is not loaded!\n", filterName);
        return;
    }

    createFilterObjectPtr createObject = nullptr;
    if (stereoLib->getProcAddress("createFilterObject", createObject))
    {

#ifndef WIN32
        string pathDelimiter = "/";
#else
        string pathDelimiter = "\\";
#endif

        string fileR = resourcesPath;
        fileR.append(pathDelimiter);
        fileR.append("moto_R_640x32.raw");

        string fileL = resourcesPath;
        fileL.append(pathDelimiter);
        fileL.append("moto_L_640x32.raw");

        loadMemFromFile((char*)fileR.c_str(), IMG_H_CT_CM * IMG_W_CT_CM, iBufStereo[0]);
        loadMemFromFile((char*)fileL.c_str(), IMG_H_CT_CM * IMG_W_CT_CM, iBufStereo[1]);

        auto testInputCrc = unitTestCrcCheck(iBufStereo, sizeof(iBufStereo), 0x6cefdd9f);

        assert(testInputCrc == true && "Input Files does not match the mock-up tests CRC!!!");

        string fileRFlat = resourcesPath;
        fileRFlat.append(pathDelimiter);
        fileRFlat.append("moto_R_640x32_10bit_FLAT.raw");

        string fileLFlat = resourcesPath;
        fileLFlat.append(pathDelimiter);
        fileLFlat.append("moto_L_640x32_10bit_FLAT.raw");

        loadMemFromFile((char*)fileRFlat.c_str(), IMG_H_CT_CM * IMG_W_CT_CM * 2, iBufFlat[0]);
        loadMemFromFile((char*)fileLFlat.c_str(), IMG_H_CT_CM * IMG_W_CT_CM * 2, iBufFlat[1]);


        string fileRPack = resourcesPath;
        fileRPack.append(pathDelimiter);
        fileRPack.append("moto_R_640x32_10bit_PACK.raw");

        string fileLPack = resourcesPath;
        fileLPack.append(pathDelimiter);
        fileLPack.append("moto_L_640x32_10bit_PACK.raw");

        loadMemFromFile((char*)fileRPack.c_str(), IMG_H_CT_CM * IMG_W_CT_CM * 5 / 4, (uint8_t*)iBufPack + 0);
        loadMemFromFile((char*)fileLPack.c_str(), IMG_H_CT_CM * IMG_W_CT_CM * 5 / 4, (uint8_t*)iBufPack + IMG_W_CT_CM*IMG_H_CT_CM * 5 / 4);


        printf("START TESTING: %s \n", filterName);

        stereoFilter = createObject(filterName);

        for (auto testMethod : stereoFilterApiTestMethods)
        {
            (this->*testMethod)();
        }

        printf("END TESTING: %s ALL TESTS PASSED\n", filterName);
    }
}

void StereoFilterApiTest::loadLibrary(const char* libName)
{
    stereoLib = new os::SharedLibrary(libName);
}

void StereoFilterApiTest::unloadLibrary()
{
    if (stereoLib->isLoaded())
    {
        stereoLib->unload();
    }
}

uint32_t StereoFilterApiTest::stereoCensusTransConfig(uint64_t iBufAdr, uint64_t oBufAdr, uint64_t eBufAdr, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask)
{
    memset(stereoConfig, 0, sizeof(StereoConfiguration));

    uint32_t oBpp = (ctFormat < PV0_CT64) ? 4 : 8;//32 or 64 bit
    uint32_t oSize = IMG_W*IMG_H*oBpp;

    stereoConfig->operation = CT_ONLY;
    stereoConfig->kerSz = kerSz;
    stereoConfig->mask = mask;
    stereoConfig->ctFormat = ctFormat;
    stereoConfig->dd = DBG_NONE;
    stereoConfig->nd = DISP_64;//don't care
    stereoConfig->outMode = OMODE_DSP_MAP;//don't care
    stereoConfig->params = 0;

    stereoConfig->nInPl[0] = 1; //computing CT-des just for 1 plane
    stereoConfig->iBuf[0].width = stereoConfig->oBuf[0].width = IMG_W;
    stereoConfig->iBuf[0].buff.height = stereoConfig->oBuf[0].buff.height = IMG_H;
    stereoConfig->iBuf[0].buff.base = stereoConfig->oBuf[0].buff.base = 0; //full frame

    stereoConfig->iBuf[0].buff.base = iBufAdr;
    stereoConfig->oBuf[0].buff.base = oBufAdr;

    // Start the Stereo model!!!!
    stereoConfig->iFill = IMG_H;

    return oSize;
}

uint32_t StereoFilterApiTest::stereoCensusTransCompandingConfig(uint64_t* iBufAdr, uint64_t oBufS0, uint64_t oBufS1, StereoOut outMode, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask, NumDisp nd, DbgDump dd)
{
    memset(stereoConfig, 0, sizeof(StereoConfiguration));

    stereoConfig->outMode = outMode;
    stereoConfig->operation = CT_CM;
    stereoConfig->kerSz = kerSz;
    stereoConfig->ctFormat = ctFormat;
    stereoConfig->mask = mask;
    stereoConfig->nd = nd;
    stereoConfig->dd = dd;
    stereoConfig->params = ((RATIO_THRESHOLD & 0xFF) << 24)
        | ((CT_THRESHOLD & 0xFF) << 16)
        | ((CM_THRESHOLD & 0xFF) << 8)
        | ((CM_BETA & 0xF) << 4)
        | ((CM_ALFA & 0xF) << 0);

    stereoConfig->iBuf[0].width = stereoConfig->oBuf[0].width = stereoConfig->oBuf[1].width = IMG_W_CT_CM;
    stereoConfig->iBuf[0].buff.height = stereoConfig->oBuf[0].buff.height = stereoConfig->oBuf[1].buff.height = IMG_H_CT_CM;
    stereoConfig->iBuf[0].buff.base = stereoConfig->oBuf[0].buff.base = stereoConfig->oBuf[1].buff.base = 0; //full frame

    stereoConfig->iBuf[0].buff.base = (uint64_t)&iBufAdr[0];
    stereoConfig->oBuf[0].buff.base = oBufS0;
    stereoConfig->oBuf[1].buff.base = oBufS1;

    // Start the Stereo model!!!!
    stereoConfig->iFill = IMG_H_CT_CM;

    auto imageSize = IMG_W_CT_CM * IMG_H_CT_CM;

    return imageSize;
}

uint32_t StereoFilterApiTest::stereoNoWrapConfig(uint64_t* iBufAdr, uint64_t oBufS1, StereoOut outMode, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask, NumDisp nd, DbgDump dd)
{
    memset(stereoConfig, 0, sizeof(StereoConfiguration));

    stereoConfig->outMode = outMode;
    stereoConfig->operation = CT_CM;
    stereoConfig->kerSz = kerSz;
    stereoConfig->ctFormat = ctFormat;
    stereoConfig->mask = mask;
    stereoConfig->nd = nd;
    stereoConfig->dd = dd;
    stereoConfig->params = ((RATIO_THRESHOLD & 0xFF) << 24)
        | ((CT_THRESHOLD & 0xFF) << 16)
        | ((CM_THRESHOLD & 0xFF) << 8)
        | ((CM_BETA & 0xF) << 4)
        | ((CM_ALFA & 0xF) << 0);

    stereoConfig->iBuf[0].width = stereoConfig->oBuf[0].width = stereoConfig->oBuf[1].width = IMG_W_CT_CM;
    stereoConfig->iBuf[0].buff.height = stereoConfig->oBuf[0].buff.height = stereoConfig->oBuf[1].buff.height = IMG_H_CT_CM;
    stereoConfig->iBuf[0].buff.base = stereoConfig->oBuf[0].buff.base = stereoConfig->oBuf[1].buff.base = 0; //full frame

    stereoConfig->iBuf[0].buff.base = (uint64_t)&iBufAdr[0];
    stereoConfig->oBuf[0].buff.base = (uint32_t)NULL;
    stereoConfig->oBuf[1].buff.base = oBufS1;

    return IMG_H_CT_CM * IMG_W_CT_CM;
}

uint32_t StereoFilterApiTest::stereoCensusTransCompandingSgbmAlgoConfig(uint64_t iBuf0, uint64_t iBuf1, uint64_t oBufS0, StereoMode op, DbgDump dbgDmp)
{
    memset(stereoConfig, 0, sizeof(StereoConfiguration));

    stereoConfig->outMode = OMODE_DSP_SINGLE;
    stereoConfig->operation = op;
    stereoConfig->kerSz = CT_KS_5x5;
    stereoConfig->mask = NO_MASK;
    stereoConfig->nd = DISP_64;
    stereoConfig->companding = 0;
    stereoConfig->dd = dbgDmp;
    stereoConfig->params = ((RATIO_THRESHOLD & 0xFF) << 24)
        | ((CT_THRESHOLD & 0xFF) << 16)
        | ((CM_THRESHOLD & 0xFF) << 8)
        | ((CM_BETA & 0xF) << 4)
        | ((CM_ALFA & 0xF) << 0);

    stereoConfig->aggDiv = 3;  //SGBM params
    stereoConfig->aggLutH = stereoConfig->aggLutV = aggLut; //precomputed

    stereoConfig->iBuf[0].width = stereoConfig->iBuf[1].width = stereoConfig->oBuf[0].width = stereoConfig->oBuf[1].width = IMG_W_CT_CM;
    stereoConfig->iBuf[0].buff.height = stereoConfig->iBuf[1].buff.height = stereoConfig->oBuf[0].buff.height = stereoConfig->oBuf[1].buff.height = IMG_H_CT_CM;
    stereoConfig->iBuf[0].buff.base = stereoConfig->iBuf[1].buff.base = stereoConfig->oBuf[0].buff.base = stereoConfig->oBuf[1].buff.base = 0; //full frame

    stereoConfig->iBuf[0].buff.base = iBuf0;//main input (L/R images)
    stereoConfig->iBuf[1].buff.base = iBuf1;//DebugMap-in (SGBM only mode)
    stereoConfig->oBuf[0].buff.base = oBufS0;//DebugMap-out(FULL-PIPE mode)
    stereoConfig->oBuf[1].buff.base = (uint64_t)oBuf1;//disparity

    return IMG_H_CT_CM * IMG_W_CT_CM;
}

uint32_t StereoFilterApiTest::stereoCensusTransCompanding10bitFlatAndPackConfig(uint64_t* iBufAdr, uint64_t oBufS0, uint64_t oBufS1, StereoOut outMode, CtFormat ctFormat, CtKerSz kerSz, uint64_t mask, NumDisp nd, uint32_t iFmt)
{
    memset(stereoConfig, 0, sizeof(StereoConfiguration));

    stereoConfig->inFmt = iFmt;
    stereoConfig->outMode = outMode;
    stereoConfig->operation = CT_CM;
    stereoConfig->kerSz = kerSz;
    stereoConfig->ctFormat = ctFormat;
    stereoConfig->mask = mask;
    stereoConfig->nd = nd;
    stereoConfig->dd = DBG_NONE;
    stereoConfig->params = ((RATIO_THRESHOLD & 0xFF) << 24)
        | ((CT_THRESHOLD & 0xFF) << 16)
        | ((CM_THRESHOLD & 0xFF) << 8)
        | ((CM_BETA & 0xF) << 4)
        | ((CM_ALFA & 0xF) << 0);

    stereoConfig->iBuf[0].width = stereoConfig->oBuf[0].width = stereoConfig->oBuf[1].width = IMG_W_CT_CM;
    stereoConfig->iBuf[0].buff.height = stereoConfig->oBuf[0].buff.height = stereoConfig->oBuf[1].buff.height = IMG_H_CT_CM;
    stereoConfig->iBuf[0].buff.base = stereoConfig->oBuf[0].buff.base = stereoConfig->oBuf[1].buff.base = 0; //full frame

    stereoConfig->iBuf[0].buff.base = (uint64_t)&iBufAdr[0];
    stereoConfig->oBuf[0].buff.base = oBufS0;
    stereoConfig->oBuf[1].buff.base = oBufS1;

    // Start the Stereo model!!!!
    stereoConfig->iFill = IMG_H_CT_CM;

    return IMG_H_CT_CM * IMG_W_CT_CM;
}

//[CT-only] solid-color
void StereoFilterApiTest::ctSolidColorPv0Ct32()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufA, (uint64_t)oBufStereo, (uint64_t)eBufA_PV0_CT32, PV0_CT32, CT_KS_5x5, NO_MASK);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufA_PV0_CT32, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctSolidColorPv0Ct24()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufA, (uint64_t)oBufStereo, (uint64_t)eBufA_PV8_CT24, PV8_CT24, CT_KS_5x5, NO_MASK);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufA_PV8_CT24, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctSolidColorPv0Ct64()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufA, (uint64_t)oBufStereo, (uint64_t)eBufA_PV0_CT64, PV0_CT64, CT_KS_5x5, NO_MASK);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufA_PV0_CT64, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctSolidColorPv0Ct56()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufA, (uint64_t)oBufStereo, (uint64_t)eBufA_PV8_CT56, PV8_CT56, CT_KS_5x5, NO_MASK);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufA_PV8_CT56, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

//[CT-only] random-data
void StereoFilterApiTest::ctRandomDataPv0Ct32()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufB, (uint64_t)oBufStereo, (uint64_t)eBufB0_PV0_CT32, PV0_CT32, CT_KS_5x5, NO_MASK);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufB0_PV0_CT32, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctRandomDataPv0Ct64()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufB, (uint64_t)oBufStereo, (uint64_t)eBufB1_PV0_CT64, PV0_CT64, CT_KS_7x7, NO_MASK);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufB1_PV0_CT64, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctRandomDataPv0Ct64BigMask()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufB, (uint64_t)oBufStereo, (uint64_t)eBufB1_PV0_CT64, PV0_CT64, CT_KS_7x7, 0xFFFFFFFFFFFFFFFFLL); //0xFF...F or NO_MASK should produce same results

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufB1_PV0_CT64, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctRandomDataPv0Ct32BigMask()
{
    printf("TESTING: %s ", __func__);

    auto oSize = stereoCensusTransConfig((uint64_t)iBufB, (uint64_t)oBufStereo, (uint64_t)eBufB2_PV0_CT32, PV0_CT32, CT_KS_7x7, 0x155F5D6D75F55LL);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto testResult = memcmp((void*)oBufStereo, (void*)eBufB2_PV0_CT32, oSize);

    assert(testResult == 0 && __func__);

    printf(" -> PASS\n");
}

// [CT_CM] all output modes, all debug outputs
void StereoFilterApiTest::ctCmOmodeDspSingle()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, NULL, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_NONE);

    stereoFilter->reset();
    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult = unitTestCrcCheck(oBuf1, imageSize * 1, 0xd9226814);//a)

    assert(crcResult == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctCmOmodeDspCd()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, NULL, (uint64_t)oBuf1, OMODE_DSP_CD, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_NONE);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult = unitTestCrcCheck(oBuf1, imageSize * 1, 0x2d6ff356);//f)

    assert(crcResult == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctCmOmodeDspMc()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, NULL, (uint64_t)oBuf1, OMODE_DSP_MC, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_NONE);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult = unitTestCrcCheck(oBuf1, imageSize * 2, 0x51a3e2e1);//e)

    assert(crcResult == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctCmOmodeDspMap()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, NULL, (uint64_t)oBuf1, OMODE_DSP_MAP, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_NONE);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult = unitTestCrcCheck(oBuf1, imageSize * 4, 0x682ae61c);//d)

    assert(crcResult == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctCmOmodeDspComb()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, NULL, (uint64_t)oBuf1, OMODE_DSP_COMB, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_NONE);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult = unitTestCrcCheck(oBuf1, imageSize * 4, 0xac6e28fa);//g)

    assert(crcResult == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctDisp()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, (uint64_t)oBuf0, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, CT_des);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize * 1, 0xd9226814); //disp should be as a)
    auto crcResult0 = unitTestCrcCheck(oBuf0, imageSize * 4 * 2, 0x6a84ffaa); //dump CT_des (4Bytes/pix, 2 planes)

    assert(crcResult1 == true && __func__);
    assert(crcResult0 == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctDbgMap64()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, (uint64_t)oBuf0, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_map);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize * 1, 0xd9226814); //disp should be as a)
    auto crcResult0 = unitTestCrcCheck(oBuf0, imageSize * 64, 0xa2bc0111); //dump DBG_map (64Bytes/pix, 1 planes)

    assert(crcResult1 == true && __func__);
    assert(crcResult0 == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctDbgMap96()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, (uint64_t)oBuf0, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_96, DBG_map);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize * 1, 0x80065bd0); //different than a) as we're using 96
    auto crcResult0 = unitTestCrcCheck(oBuf0, imageSize * 96, 0xd25bf652); //dump DBG_map (96ytes/pix, 1 planes)

    assert(crcResult1 == true && __func__);
    assert(crcResult0 == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::cmDisp64()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, NULL, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_NONE);
    stereoConfig->companding = 1;

    // CALL RESET CONFIGURATION FROM THE API
    stereoFilter->reset();
    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize * 1, 0xa06b41ec);

    assert(crcResult1 == true && __func__);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::cmDisp96()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingConfig((uint64_t*)iBufStereo, NULL, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_96, DBG_NONE);
    stereoConfig->companding = 1;

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize * 1, 0x0ff33850);

    assert(crcResult1 == true && __func__);

    printf(" -> PASS\n");
}

// [NOWRAP mode]
void StereoFilterApiTest::noWrapMode()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoNoWrapConfig((uint64_t*)iBufStereo, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, DBG_NONE);

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start(NO_WRAP);

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize * 1, 0xd9226814);

    assert(crcResult1 == true && __func__);

    printf(" -> PASS\n");
}

// [CT_CM + SGBM-only => FULL]
void StereoFilterApiTest::ctCmFull()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingSgbmAlgoConfig((uint64_t)iBufStereo[0], NULL, (uint64_t)oBuf0, CT_CM, DBG_map); //RC1: LR images
    stereoConfig->iFill = IMG_H_CT_CM; // this will start the Stereo filter

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize, 0xd9226814);
    auto crcResult0 = unitTestCrcCheck(oBuf0, imageSize * 64, 0xa2bc0111);

    const char* function = __func__;
    assert(crcResult1 == true && function);
    assert(crcResult0 == true && function);

    printf(" -> PASS\n");
}

void StereoFilterApiTest::sgbmFull()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompandingSgbmAlgoConfig((uint64_t)iBufStereo[1], (uint64_t)oBuf0, NULL, SGBM_ONLY, DBG_NONE); //RC1:Left img
    stereoConfig->iFill = (IMG_H_CT_CM << 16) | IMG_H_CT_CM; // this will start the Stereo filter

    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize, 0x267b5f4c);

    assert(crcResult1 == true || assert_msg(__func__));

    printf(" -> PASS\n");
}

// [CT_CM 10bit Flat and 10bit Pack]
void StereoFilterApiTest::ctCm10BitFlat()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompanding10bitFlatAndPackConfig((uint64_t*)iBufFlat, NULL, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, BITS_10_FLAT);

    // CALL RESET CONFIGURATION FROM THE API
    stereoFilter->reset();
    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize, 0xd9226814);

    assert(crcResult1 == true || assert_msg(__func__));

    printf(" -> PASS\n");
}

void StereoFilterApiTest::ctCm10BitPack()
{
    printf("TESTING: %s ", __func__);

    auto imageSize = stereoCensusTransCompanding10bitFlatAndPackConfig((uint64_t*)iBufPack, NULL, (uint64_t)oBuf1, OMODE_DSP_SINGLE, PV8_CT24, CT_KS_5x5, NO_MASK, DISP_64, BITS_10_PACK);

    // CALL RESET CONFIGURATION FROM THE API
    stereoFilter->reset();
    // CALL SET CONFIGURATION FROM THE API
    stereoFilter->configure(stereoConfig);
    // CALL RUN FROM THE API
    stereoFilter->start();

    auto crcResult1 = unitTestCrcCheck(oBuf1, imageSize, 0xd9226814);

    assert(crcResult1 == true || assert_msg(__func__));

    printf(" -> PASS\n");
}
