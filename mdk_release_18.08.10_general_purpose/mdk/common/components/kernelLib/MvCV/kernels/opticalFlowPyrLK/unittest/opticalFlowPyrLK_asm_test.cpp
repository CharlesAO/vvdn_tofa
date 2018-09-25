#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include "opticalFlowPyrLK_asm_test.h"
#include <cstdio>
#include <cv.h>
#include "helpers.h"

#ifndef PC
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif

EXTERN void setBuffers();
EXTERN void Init();
EXTERN void Run();

TestRunner opticalFlowTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int opticalFlowCycleCount;

void OpticalFlowPyrLK_asm_test(std::vector<cv::Mat>pyrA, std::vector<cv::Mat>pyrB,
	std::vector<cv::Point2f> prevPts, std::vector<cv::Point2f>& nextPtsShave,
	std::vector<u8>& statusShave, std::vector<fp32>& errorShave, cv::Size winSize,
	s32 levels, cv::TermCriteria termCriteria, s32 flags)
{
    TargetCore t = SHAVE0;

    FunctionInfo& functionInfo = FunctionInfo::Instance();

    opticalFlowTestRunner.Init();

    // Set input parameters
    opticalFlowTestRunner.SetInput("width", (unsigned int)pyrA[0].cols, t);
    opticalFlowTestRunner.SetInput("height", (unsigned int)pyrA[0].rows, t);
    opticalFlowTestRunner.SetInput("pyrLevels", (unsigned int)levels, t);
    opticalFlowTestRunner.SetInput("searchWinSz", (u8*)&winSize, sizeof(winSize), t);
    opticalFlowTestRunner.SetInput("stopCrit", (u8*)&termCriteria, sizeof(termCriteria), t);
    opticalFlowTestRunner.SetInput("flags", (unsigned int)flags, t);
    opticalFlowTestRunner.SetInput("pointsCount", (unsigned int)prevPts.size(), t);
    opticalFlowTestRunner.SetInput("cornersA", (float*)prevPts.data(), prevPts.size() * 2, t);

    // Copy pre-computed pyramids to Myriad
    opticalFlowTestRunner.SetInput("inputFrameA", pyrA[0].data, pyrA[0].step1() * pyrA[0].rows, LEON_OS);
    opticalFlowTestRunner.SetInput("pyrAL1", pyrA[1].data, pyrA[1].step1() * pyrA[1].rows, LEON_OS);
    opticalFlowTestRunner.SetInput("pyrAL2", pyrA[2].data, pyrA[2].step1() * pyrA[2].rows, LEON_OS);
    opticalFlowTestRunner.SetInput("pyrAL3", pyrA[3].data, pyrA[3].step1() * pyrA[3].rows, LEON_OS);
    if (levels >= 4)
        opticalFlowTestRunner.SetInput("pyrAL4", pyrA[4].data, pyrA[4].step1() * pyrA[4].rows, LEON_OS);

    opticalFlowTestRunner.SetInput("inputFrameB", pyrB[0].data, pyrB[0].step1() * pyrB[0].rows, LEON_OS);
    opticalFlowTestRunner.SetInput("pyrBL1", pyrB[1].data, pyrB[1].step1() * pyrB[1].rows, LEON_OS);
    opticalFlowTestRunner.SetInput("pyrBL2", pyrB[2].data, pyrB[2].step1() * pyrB[2].rows, LEON_OS);
    opticalFlowTestRunner.SetInput("pyrBL3", pyrB[3].data, pyrB[3].step1() * pyrB[3].rows, LEON_OS);
    if (levels >= 4)
        opticalFlowTestRunner.SetInput("pyrBL4", pyrB[4].data, pyrB[4].step1() * pyrB[4].rows, LEON_OS);

    //opticalFlowTestRunner.GuardInsert("cornersB", t, prevPts.size() * 2, (fp32*)nextPtsShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersStatus", t, prevPts.size(), (u8*)statusShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersError", t, prevPts.size(), (fp32*)errorShave.data());
    opticalFlowTestRunner.Run(LEON_OS, "startTest");
    //opticalFlowTestRunner.GuardInsert("cornersB", t, prevPts.size() * 2, (fp32*)nextPtsShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersStatus", t, prevPts.size(), (u8*)statusShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersError", t, prevPts.size(), (fp32*)errorShave.data());
    opticalFlowTestRunner.Run(LEON_OS, "setBuffers");
    //opticalFlowTestRunner.GuardInsert("cornersB", t, prevPts.size() * 2, (fp32*)nextPtsShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersStatus", t, prevPts.size(), (u8*)statusShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersError", t, prevPts.size(), (fp32*)errorShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersB", t, prevPts.size() * 2, (fp32*)nextPtsShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersStatus", t, prevPts.size(), (u8*)statusShave.data());
    //opticalFlowTestRunner.GuardInsert("cornersError", t, prevPts.size(), (fp32*)errorShave.data());
    opticalFlowTestRunner.Run(SHAVE0);

    opticalFlowTestRunner.GetOutput("cornersB", t, prevPts.size() * 2, (fp32*)nextPtsShave.data());
    //opticalFlowTestRunner.GuardCheck("cornersB", t, prevPts.size() * 2, (fp32*)nextPtsShave.data());
    opticalFlowTestRunner.GetOutput("cornersStatus", t, prevPts.size(), (u8*)statusShave.data());
    //opticalFlowTestRunner.GuardCheck("cornersStatus", t, prevPts.size(), (u8*)statusShave.data());
    opticalFlowTestRunner.GetOutput("cornersError", t, prevPts.size(), (fp32*)errorShave.data());
    //opticalFlowTestRunner.GuardCheck("cornersError", t, prevPts.size(), (fp32*)errorShave.data());

    opticalFlowCycleCount = opticalFlowTestRunner.GetVariableValue(std::string("cycleCount"));

    functionInfo.AddCyclePerPixelInfo(opticalFlowCycleCount / prevPts.size());
}
