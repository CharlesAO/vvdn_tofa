#include "gtest/gtest.h"
#include <tuple>
#include "opticalFlowPyrLK_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <cv.h>

#include <mvcv.h>
#include <ArrayChecker.h>
#include <helpers.h>
#include <dummy/shared/Defines.h>
#include <opticalFlowPyrLK.h>

#include <opencv/cv.hpp> 

#if CV_MAJOR_VERSION==2 
  #include <opencv2/contrib/contrib.hpp> 
  #include <opencv/highgui.h> 
#elif CV_MAJOR_VERSION==3 
  #include <opencv2/highgui/highgui.hpp> 
  #include <opencv2/imgproc.hpp> 
#endif


#ifndef VISUAL_OF_TEST
#define printf(...)
#endif
using namespace std;

using ::testing::TestWithParam;
using ::testing::Values;
using ::testing::ValuesIn;
using ::testing::Combine;

void resizeAndSave()
{
    IplImage* imgAyuv = loadImageYUV("../../../../../../../resources/imgtest_640x480_Yuv400_8bpp.bin", 640, 480);
    IplImage* dst = cvCreateImage(cvSize(1280, 720), imgAyuv->depth, imgAyuv->nChannels);
    IplImage* dst1 = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 1);
    IplImage* dst2 = cvCreateImage(cvSize(1280, 720), IPL_DEPTH_8U, 1);

    cvResize(imgAyuv, dst);
    cvCvtColor(dst, dst1, CV_RGB2GRAY);
    cvThreshold(dst1, dst2, 150, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    ofstream f("checkerboard_with_dots.bw", ios::out);
    f.write(dst2->imageData, 1280*720);
    f.close();

//    testPause();
}

void testPause()
{
#ifdef VISUAL_OF_TEST

    int key = 0;

    while ((key = cv::waitKey(1)) != 27) // <- That's the code for ESC...
    {
// Uncomment to see the key code if you want to change it. Its not ASCII
//        printf("%d\n", key);
    }
#endif
}

void showPyramid(std::vector<cv::Mat> pyr, std::string name)
{
    for (u32 level = 0; level < pyr.size(); level++)
    {
        printf("Level %d: %d x %d\n", level, pyr[level].cols, pyr[level].rows);

        char winName[16];
        sprintf(winName, "%s Level L%u", name.c_str(), level);
#ifdef VISUAL_OF_TEST
        cv::imshow(winName, pyr[level]);
#endif
    }
}

template <class T>
void drawCircles(cv::Mat img, std::vector<T> pts, u32 radius, cv::Scalar color)
{
    for (u32 i = 0; i < pts.size(); i++)
    {
        cv::circle(img, pts[i], radius, color);
    }
}

void drawTrails(cv::Mat img, std::vector<cv::Point2f>& a,
                std::vector<cv::Point2f>& b, std::vector<u8>& status)
{
    for (u32 i = 0; i < a.size(); i++)
    {
        //if (!status.empty())
        if (status[i] != 0)
        {
            cv::line(img, a[i], b[i], CV_RGB(0, 255, 0));
            //printf("%f %f %f %f\n", a[i].x, a[i].y, b[i].x, b[i].y);
        }
    }
}

cv::Mat shift(cv::Mat src, u32 pixels)
{
    cv::Mat imgShift = cv::Mat::zeros(src.size(), src.type());

    src(cv::Rect(0, 0, src.cols - pixels, src.rows))
    .copyTo(imgShift(cv::Rect(pixels, 0, src.cols - pixels, src.rows)));

    return imgShift;
}

pair<cv::Mat, cv::Mat> loadImages(string a, string b, cv::Size loadSz, cv::Size resize)
                {


    IplImage* imgAyuv = loadImageYUV(a.c_str(), loadSz.width, loadSz.height);
    IplImage* imgByuv = loadImageYUV(b.c_str(), loadSz.width, loadSz.height);

    cv::Mat imgA = cv::cvarrToMat(imgAyuv);
    cv::Mat imgB = cv::cvarrToMat(imgByuv);;
    cv::Mat imgArsz;
    cv::Mat imgBrsz;


    

    cvtColor(imgA, imgA, CV_RGB2GRAY);
    cvtColor(imgB, imgB, CV_RGB2GRAY);

    cv::resize(imgA, imgArsz, cv::Size(resize.width, resize.height));
    cv::resize(imgB, imgBrsz, cv::Size(resize.width, resize.height));

    return make_pair(imgArsz, imgBrsz);
                }

cv::Size resolutions[2] =
{
    {
        640,
        480
    },
    {
        1280,
        720
    }
};

pair<string, string> images[2] =
{
    make_pair
    (
                    string("../../../../../../../resources/office_a_1280x720_420p.yuv"),
                    string("../../../../../../../resources/office_b_1280x720_420p.yuv")
    ),
    make_pair
    (
                    string("../../../../../../../resources/office_a_1280x720_420p.yuv"),
                    string("../../../../../../../resources/office_a_1280x720_420p.yuv")
    )
};

class OpticalFlow : public TestWithParam< std::tr1::tuple<cv::Size, pair<string, string>> >
{
protected:
    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker check;
    ArrayChecker outputChecker;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    // Function parameters
    static cv::Mat imgA;
    static cv::Mat imgB;
    static std::vector<cv::Mat> pyrA;
    static std::vector<cv::Mat> pyrB;
    static u32 count;
    static std::vector<cv::Point2f> prevPts;
    static std::vector<cv::Point2f> nextPts;
    static std::vector<cv::Point2f> nextPtsShavePc;
    static std::vector<cv::Point2f> nextPtsShaveM;
    static std::vector<u8> status;
    static std::vector<u8> statusShavePc;
    static std::vector<u8> statusShaveM;
    static std::vector<fp32> error;
    static std::vector<fp32> errorShavePc;
    static std::vector<fp32> errorShaveM;
    static cv::Size winSize;
    static s32 levels;
    static u32 maxIterations;
    static fp32 epsilon;
    static cv::TermCriteria termCriteria;
    static s32 flags;
    static std::string leftImg;
    static std::string rightImg;

public:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }

    static void SetUpTestCase()
    {
        winSize = cv::Size(SEARCH_WIN_SZ, SEARCH_WIN_SZ);
        maxIterations = STOP_CRIT_MAX_ITER;
        epsilon = STOP_CRIT_EPS;
        termCriteria = cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, maxIterations, epsilon);
        flags = CV_LKFLOW_PYR_A_READY | CV_LKFLOW_PYR_B_READY;
    }

    virtual void TearDown()
    {
        prevPts.erase(prevPts.begin(), prevPts.end());
        nextPts.erase(nextPts.begin(), nextPts.end());

        nextPtsShavePc.erase(nextPtsShavePc.begin(), nextPtsShavePc.end());
        errorShavePc.erase(errorShavePc.begin(), errorShavePc.end());
        statusShavePc.erase(statusShavePc.begin(), statusShavePc.end());

        nextPtsShaveM.erase(nextPtsShaveM.begin(), nextPtsShaveM.end());
        errorShaveM.erase(errorShaveM.begin(), errorShaveM.end());
        statusShaveM.erase(statusShaveM.begin(), statusShaveM.end());
    }

    static void printPoints(std::vector<cv::Point2f>& corners1, std::vector<cv::Point2f>& corners2,
                            std::vector<u8>& status1, std::vector<u8>& status2)
    {
        for (u32 i = 0; i < corners1.size(); i++)
        {
            printf("%03d (%6.1f, %6.1f) (%6.1f, %6.1f) %d %d\n", i, corners1[i].x, corners1[i].y,
                   corners2[i].x, corners2[i].y, status1[i], status2[i]);
        }
    }

    static bool isNear(cv::Point2f a, cv::Point2f b, fp32 delta)
    {
        EXPECT_LT(abs(a.x - b.x), delta);
        EXPECT_LT(abs(a.y - b.y), delta);

        return ((abs(a.x - b.x) <= delta) && (abs(a.y - b.y) <= delta));
    }

    static void showDebug(cv::Mat imgA, cv::Mat imgB, cv::Mat imgC,
                          std::vector<cv::Point2f>& cornersA, std::vector<cv::Point2f>& cornersB, std::vector<cv::Point2f>& cornersC,
                          std::vector<u8>& statusA, std::vector<u8>& statusB)
    {
        drawCircles(imgA, cornersA, 3, cv::Scalar(255, 0, 255));
        drawCircles(imgB, cornersB, 3, cv::Scalar(255, 0, 255));
        drawCircles(imgC, cornersC, 3, cv::Scalar(0, 255, 0));

        drawTrails(imgB, cornersA, cornersB, statusA);
        drawTrails(imgC, cornersA, cornersC, statusB);
#ifdef VISUAL_OF_TEST
        showPyramid(pyrA, "pyrA");
        showPyramid(pyrB, "pyrB");
#endif
        u32 foundPcCnt = 0;
        u32 foundMCnt = 0;
        u32 matchesCnt = 0;

        for (u32 i = 0; i < cornersB.size(); i++)
        {
            if (statusA[i] == 1 || statusB[i] == 1)
            {
                if (isNear(cornersB[i], cornersC[i], 0.1))
                {

                    matchesCnt++;
                }
                else
                {
                    printf("Pos: %d, CornerB:(%.3f %.3f) cornerC (%.3f %.3f)\n",
                           i, cornersB[i].x, cornersB[i].y, cornersC[i].x, cornersC[i].y);
                }
                if (statusA[i] == 1)
                    foundPcCnt++;

                if (statusB[i] == 1)
                    foundMCnt++;
            }
        }

        char s[32];

        sprintf(s, "Total: %lu", cornersB.size());
        putText(imgB, std::string(s), cv::Point(10, imgB.rows - 100),
                CV_FONT_HERSHEY_SIMPLEX, 0.5f, CV_RGB(0, 0, 0), 2.5f);

        sprintf(s, "Matching: %d", matchesCnt);
        putText(imgB, std::string(s), cv::Point(10, imgB.rows - 80),
                CV_FONT_HERSHEY_SIMPLEX, 0.5f, CV_RGB(0, 0, 0), 2.5f);

        sprintf(s, "Found PC: %d", foundPcCnt);
        putText(imgB, std::string(s), cv::Point(10, imgB.rows - 60),
                CV_FONT_HERSHEY_SIMPLEX, 0.5f, CV_RGB(0, 0, 0), 2.5f);

        sprintf(s, "Found Myriad: %d", foundMCnt);
        putText(imgB, std::string(s), cv::Point(10, imgB.rows - 40),
                CV_FONT_HERSHEY_SIMPLEX, 0.5f, CV_RGB(0, 0, 0), 2.5f);
#ifdef VISUAL_OF_TEST
        cv::imshow("imgA", imgA);
        cv::imshow("imgB", imgB);
        cv::imshow("imgC", imgC);
#endif
        cv::imwrite("imgA.jpg", imgA);
        cv::imwrite("imgB.jpg", imgB);
        cv::imwrite("imgC.jpg", imgC);

        testPause();
    }

    void mvcvCalcOpticalFlowPyrLK_pc_test(std::vector<cv::Mat>& pyrA, std::vector<cv::Mat>& pyrB)
    {
        Mat pcPyrA[pyrA.size()];
        Mat pcPyrB[pyrB.size()];
        u32 width = pyrA[0].cols;
        u32 height = pyrA[0].rows;
        //#define HEAP_SIZE (20 * 1024)
        //u8 heap[HEAP_SIZE];
        mvCvSize searchWinSz;
        mvCvTermCriteria stopCrit;

        // Initialize heap
        //init_heap(heap, HEAP_SIZE);
        //clear_heap();

        memcpy(&searchWinSz, &winSize, sizeof(winSize));
        //memcpy(&stopCrit, &termCriteria, sizeof(termCriteria));

        stopCrit.max_iter = termCriteria.maxCount;
        stopCrit.epsilon = termCriteria.epsilon;

        // Initialize input data structures
        for (u32 i = 0; i < (u32)pyrA.size(); i++)
        {
            Mat ma(height / pow(2, i), width / pow(2, i), CV_8U, (u8*)pyrA[i].data, false,
                   width / pow(2, i) + searchWinSz.width * 2);
            Mat mb(height / pow(2, i), width / pow(2, i), CV_8U, (u8*)pyrB[i].data, false,
                   width / pow(2, i) + searchWinSz.width * 2);

            pcPyrA[i] = ma;
            pcPyrB[i] = mb;

            printf("%dx%d %lu\n", pcPyrA[i].cols, pcPyrA[i].rows, pcPyrA[i].step1());

        }
        // Run algorithm
        mvcvCalcOpticalFlowPyrLK_tuned(pcPyrA, pcPyrB, (mvCvPoint2D32f*)prevPts.data(),
                                       (mvCvPoint2D32f*)nextPtsShavePc.data(), (u8*)statusShavePc.data(),
                                       (fp32*)errorShavePc.data(), searchWinSz, levels, stopCrit, flags, prevPts.size());

    }

    void RunOf(cv::Mat imgA, cv::Mat imgB, u32 maxCornerCount,
               std::vector<cv::Point2f>& prevPts)
    {
        // Adjust number of levels - don't want the top most to become too small
        if (imgA.cols < 1280)
            levels = MAX_PYRAMID_LEVELS - 1;
        else
            levels = MAX_PYRAMID_LEVELS;

        // Generate points
        if (prevPts.empty())
        {
            cv::goodFeaturesToTrack(imgA, prevPts, maxCornerCount, 0.01, 10);
        }
        // Build pyramids
        cv::buildOpticalFlowPyramid(imgA, pyrA, winSize, levels, false);
        cv::buildOpticalFlowPyramid(imgB, pyrB, winSize, levels, false);

        //prevPts[0] = prevPts[259];

        // Reserve space in the buffers that are passed to Myriad to get the output
        prevPts.resize(maxCornerCount);
        count = prevPts.size();
        nextPts = prevPts;

        nextPtsShavePc.resize(count);
        errorShavePc.resize(count);
        statusShavePc.resize(count);

        nextPtsShaveM.resize(count);
        errorShaveM.resize(count);
        statusShaveM.resize(count);

        // Track points
        //cv::calcOpticalFlowPyrLK(pyrA, pyrB, prevPts, nextPts, status,
        //error, winSize, levels, termCriteria, flags);

        mvcvCalcOpticalFlowPyrLK_pc_test(pyrA, pyrB);

        OpticalFlowPyrLK_asm_test(pyrA, pyrB, prevPts, nextPtsShaveM,
                                  statusShaveM, errorShaveM, winSize, levels, termCriteria, flags);

        //printPoints(nextPtsShavePc, nextPtsShaveM, statusShavePc, statusShaveM);

        //showDebug(imgA, imgB, imgB.clone(), prevPts, nextPtsShavePc, nextPtsShaveM,
        //          statusShavePc, statusShaveM);
    }

    void RunOfIter(cv::Mat a, cv::Mat b, u32 iter, u32 pixelsToShift, u32 maxCorners = 300)
    {
        b = shift(b, pixelsToShift);

        for (u32 i = 0; i < iter; i++)
        {

            RunOf(a.clone(), b.clone(), maxCorners, prevPts);

            // Swap corners
            prevPts = nextPtsShavePc;

            // Swap images
            cv::Mat tmp;
            tmp = a;
            a = b;
            b = tmp;
        }
    }
};

cv::Mat OpticalFlow::imgA;
cv::Mat OpticalFlow::imgB;
std::vector<cv::Mat> OpticalFlow::pyrA;
std::vector<cv::Mat> OpticalFlow::pyrB;
u32 OpticalFlow::count;
std::vector<cv::Point2f> OpticalFlow::prevPts;
std::vector<cv::Point2f> OpticalFlow::nextPts;
std::vector<cv::Point2f> OpticalFlow::nextPtsShavePc;
std::vector<cv::Point2f> OpticalFlow::nextPtsShaveM;
std::vector<u8> OpticalFlow::status;
std::vector<u8> OpticalFlow::statusShavePc;
std::vector<u8> OpticalFlow::statusShaveM;
std::vector<fp32> OpticalFlow::error;
std::vector<fp32> OpticalFlow::errorShavePc;
std::vector<fp32> OpticalFlow::errorShaveM;
cv::Size OpticalFlow::winSize;
s32 OpticalFlow::levels;
u32 OpticalFlow::maxIterations;
fp32 OpticalFlow::epsilon;
cv::TermCriteria OpticalFlow::termCriteria;
s32 OpticalFlow::flags;
std::string OpticalFlow::leftImg;
std::string OpticalFlow::rightImg;

TEST_F(OpticalFlow , RealShift720)
{
    string left = "../../../../../../../resources/office_a_1280x720_420p.yuv";
    string right = "../../../../../../../resources/office_b_1280x720_420p.yuv";

    pair<cv::Mat, cv::Mat> imgs = loadImages(left, right, cv::Size(1280, 720), cv::Size(1280, 720));

    RunOfIter(imgs.first, imgs.second, 1, 10);
}

INSTANTIATE_TEST_CASE_P(RealImage, OpticalFlow, Combine(ValuesIn(resolutions), ValuesIn(images)));
TEST_P(OpticalFlow, ResImagesIterShift)
{
    cv::Size resize = tr1::get<0>(GetParam());
    string left = tr1::get<1>(GetParam()).first;
    string right = tr1::get<1>(GetParam()).second;

    printf("Resolution: %dx%d\n", resize.width, resize.height);
    printf("Left image: %s\n", left.c_str());
    printf("Right image: %s\n", right.c_str());

    pair<cv::Mat, cv::Mat> imgs = loadImages(left, right, cv::Size(1280, 720), resize);

    RunOfIter(imgs.first, imgs.second, 1, 10);
}
