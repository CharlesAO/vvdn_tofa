//---------------------------------------------------------------------------//

///
/// @file  Main.cpp
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  Computing and testing the homography between two images
///
//---------------------------------------------------------------------------//

// Local Modules
#include "CloseRangePattern.h"
#include "Helpers.h"

#include <exception>
#include <iostream>
// OpenCV
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//--------------------------------------------------//
// Print help menu                                  //
//--------------------------------------------------//
void printHelp() {
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            "~~~~~~"
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
         << endl;
    cout << " Usage:" << endl;
    cout << ">> stereoCalibHomography.exe -l LeftImage -r RightImage -ho "
            "Homography -td Offset \n"
         << endl
         << endl;
    cout << " Options:" << endl << endl;
    cout << "[REQUIRED] -l or --left: to specify the path for the left image"
         << endl;
    cout << "[REQUIRED] -r or --right: to specify the path for the right "
            "image"
         << endl;
    cout << "[REQUIRED] -ho or --homography: to specify the path for the "
            "homography between left and right image"
         << endl;
    cout << "[REQUIRED] -td or --true_disparity: offset to compensate "
            "'infinity' "
            "objects"
         << endl;
    cout << "-h or --help: help menu" << endl << endl;
    cout << " Example:" << endl;
    cout << " >> stereoCalibHomography -l cam0.png -r cam1.png -ho "
            "homography.bin -td value"
         << endl;
    cout << " ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
            "~~~~~~"
            "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
         << endl;
}

//--------------------------------------------------//
// Input Parser                                     //
//--------------------------------------------------//
void inputParser(unsigned int nrInput, char *data[], RunConfiguration &runCfg) {
    runCfg.input_imgs = 0;
    runCfg.is_h_set = false;

    for (int i = 1; i < nrInput; i++) {
        if ((strcmp(data[i], "-l") == 0) || (strcmp(data[i], "--left") == 0)) {
            runCfg.leftImage = data[i + 1];
            runCfg.input_imgs++;
            i++;
        } else if ((strcmp(data[i], "-r") == 0) ||
                   (strcmp(data[i], "--right") == 0)) {
            runCfg.rightImage = data[i + 1];
            runCfg.input_imgs++;
            i++;
        } else if ((strcmp(data[i], "-ho") == 0) ||
                   (strcmp(data[i], "--homography") == 0)) {
            runCfg.homographyPath = data[i + 1];
            runCfg.is_h_set = true;
            i++;
        } else if ((strcmp(data[i], "-td") == 0) ||
                   (strcmp(data[i], "--true_disparity") == 0)) {
            runCfg.trueDisparity = atof(data[i + 1]);
            i++;
        } else if ((strcmp(data[i], "-h") == 0) ||
                   (strcmp(data[i], "--help") == 0)) {
            printHelp();
            throw exception();
        } else {
            cout << "[Error] Unknown option " << data[i]
                 << ". Please check help(-h)" << endl;
            throw exception();
        }
    }

    // check for input errors
    if (runCfg.input_imgs != 2) {
        cout << "[Error] Image Paths not set correctly, Please check "
                "help (-h)"
             << endl;
        throw exception();
    } else if (!runCfg.is_h_set) {
        cout << "[Error] Homography filepath not set correctly, Please "
                "check help "
                "(-h)"
             << endl;
        throw exception();
    }
}

Mat createDebugImage(const Mat &srcL, const Mat &srcR,
                     const vector<Point2f> &cornersL,
                     const vector<Point2f> cornersR) {
    vector<vector<DMatch>> matches;
    for (int i = 0; i < (6 * 9); ++i) {
        vector<DMatch> match;
        match.emplace_back(i, i, 0);
        matches.push_back(match);
    }
    vector<KeyPoint> left_kps, right_kps;
    for (int i = 0; i < cornersL.size(); i++) {
        left_kps.emplace_back(cornersL[i], 1.0);
        right_kps.emplace_back(cornersR[i], 1.0);
    }
    Mat matches_img;
    drawMatches(srcL, left_kps, srcR, right_kps, matches, matches_img);
    return matches_img;
}

//--------------------------------------------------//
// Main function                                    //
//--------------------------------------------------//
int main(int argc, char **argv) {
    // Parse input arguments
    RunConfiguration runCfg;
    inputParser(argc, argv, runCfg);

    // Load images
    Mat srcL, srcR;
    loadImages(runCfg, srcL, srcR);

    // Detect chessboard corners
    vector<Point2f> cornersL, cornersR;
    CloseRangePatternCalib(srcL, srcR, cornersL, cornersR, 1, 1);

    // Compute homography
    const int cfgRANSACInliersTreshold = 1;
    Mat homography =
        findHomography(cornersL, cornersR, CV_RANSAC, cfgRANSACInliersTreshold);

    // Test homography
    computeQualityMetrics(homography, cornersL, cornersR,
                          cfgRANSACInliersTreshold);

    // Compensate for non-zero (non-infinity) disparity
    Mat rectification_mat = homography.clone();
    rectification_mat.at<double>(0, 2) += runCfg.trueDisparity;

    // Save to binary file
    saveToFile(rectification_mat, runCfg);

    // print result to command line
    cout << "Output:\n" << rectification_mat << endl;

    // DEBUG
    // save rectified images
    Mat srcR_rect;
    warpPerspective(srcR, srcR_rect, homography.inv(), srcR.size());
    imwrite("rect_l.png", srcL);
    imwrite("rect_r.png", srcR_rect);

    // create debug images
    Mat debug_img = createDebugImage(srcL, srcR, cornersL, cornersR);
    imshow("Matches: L <-> R", debug_img);

    // pause to view image
    cout << "\n\nPress SPACE to exit" << endl;
    while (cvWaitKey(1) != 32)
        ;

    return 0;
}
