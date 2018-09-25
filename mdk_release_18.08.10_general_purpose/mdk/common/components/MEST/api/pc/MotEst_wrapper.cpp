///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Wrapper over HW or C Model used for MEST API.
///
/// This is the implementation to the wrapper
///

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <unistd.h>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>

#include <MotEst_wrapper.h>
#include <MotEstCModel.h>
#include <Parser/denseofConfigParser.h>

#ifdef WIN32
#define STRICMP _stricmp
#else
#define STRICMP strcasecmp
#endif

using namespace std;
using namespace cv;

//-----------------------------------------------------------------

void imgToRaw(ARGS &args, bool write_downsampled = true)
{
    Mat img1, img2, img3, img4, img5, img6;

    img1 = imread(args.ImgFname[0], CV_LOAD_IMAGE_GRAYSCALE);
    img2 = imread(args.ImgFname[1], CV_LOAD_IMAGE_GRAYSCALE);

    char *p;

    p = strrchr(args.ImgFname[0], '.');
    *p = 0;
    p = strrchr(args.ImgFname[1], '.');
    *p = 0;

    p = strrchr(args.ImgFname[0], '/') + 1;
    std::string bin0 = p;
    p = strrchr(args.ImgFname[1], '/') + 1;
    std::string bin1 = p;

    //    filenames are not without extension
    std::string tmpdir = "/tmp/ds/";
    mkdir(tmpdir.c_str(), 0777);
    bin0 = tmpdir + bin0;
    bin1 = tmpdir + bin1;

    if (!args.DisHpel)
    {
        resize(img1, img3, Size(0, 0), 0.5, 0.5, INTER_AREA);
        resize(img2, img4, Size(0, 0), 0.5, 0.5, INTER_AREA);
    }
    if (!args.DisQpel)
    {
        resize(img3, img5, Size(0, 0), 0.5, 0.5, INTER_AREA);
        resize(img4, img6, Size(0, 0), 0.5, 0.5, INTER_AREA);
    }

    args.Width = img1.cols;
    args.Height = img1.rows;

    if (write_downsampled)
    {
        std::ofstream fout;
        fout.open(bin0 + ".raw");
        fout.write((char*) &img1.data[0], img1.cols * img1.rows);
        fout.close();
        fout.open(bin1 + ".raw");
        fout.write((char*) &img2.data[0], img2.cols * img2.rows);
        fout.close();

        if (!args.DisHpel)
        {
            fout.open(bin0 + "_half.raw");
            fout.write((char*) &img3.data[0], img3.cols * img3.rows);
            fout.close();
            fout.open(bin1 + "_half.raw");
            fout.write((char*) &img4.data[0], img4.cols * img4.rows);
            fout.close();

            if (!args.DisQpel)
            {
                fout.open(bin0 + "_quarter.raw");
                fout.write((char*) &img5.data[0], img5.cols * img5.rows);
                fout.close();
                fout.open(bin1 + "_quarter.raw");
                fout.write((char*) &img6.data[0], img6.cols * img6.rows);
                fout.close();
            }
        }
    }

}

// sets the filenames for the input files
// needed if other image formats are read, different than raw pixels
void setFilenames(ARGS &args)
{
    char *p;
    p = strrchr(args.ImgFname[0], '/') + 1;
    std::string bin0 = p;
    p = strrchr(args.ImgFname[1], '/') + 1;
    std::string bin1 = p;

    std::string tmpdir = "/tmp/ds/";
    bin0 = tmpdir + bin0;
    bin1 = tmpdir + bin1;

//    std::cout << bin0 << "\n";
    strcpy(args.ImgFname[0], std::string(bin0 + ".raw").c_str());
    strcpy(args.ImgFname[1], std::string(bin1 + ".raw").c_str());
}

#ifdef __cplusplus
extern "C"
{
#endif

int runMotestWrapper(ARGS args, uint32_t *res, uint32_t resSz)

{
    int status;

    // if PNG file is detected at input
    // it is converted to raw at the above given path
//    cout << args.ImgFname[0] << "\n";
//    cout << args.RasterStep << "\n";
    char *ext = strrchr(args.ImgFname[0], '.');
    if (!STRICMP(ext, ".png") || !STRICMP(ext, ".pgm"))
    {
        imgToRaw(args, true);
        setFilenames(args);
    }

    callPreprocImg = NULL; //reseting the handler
    callPostprocImg = NULL; //reseting the handler

    // passing the arguments to MotEst and running it
    if ((status = MotEstConfig(args)))
        return status;

    memcpy(res, getResPtr(), resSz * sizeof(uint32_t));

    return 0;
}

void *getSrcFramePtr()
{
    return img[1];
}

void *getRefFramePtr()
{
    return img[0];
}

uint32_t *getResPtr()
{
    return results;
}

#ifdef __cplusplus
}
#endif
