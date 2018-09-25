#include <cv.h>
#include <cvaux.h>
#include <assert.h>
#include <stdio.h>
#include <highgui.h>

#include <yuv.h>

/* 
 * Given a yuv 420 file stream, extract the luma and chroma components,
 * the 2 chroma are then upsampled by a 2 factor, and return an image 
 * composed by 3 layers Y, U and V (format YUV 444, i.e. 3 byte for each pixel)
 */
IplImage* loadImageYUV(const char* name_file, int w, int h)
{
   IplImage *py, *pu, *pv, *pu_big, *pv_big, *image;
   int i, temp;

    FILE* pf = fopen(name_file, "rb");
    if (pf == NULL)
    {
        fprintf(stderr, "Error open file %s", name_file);
        exit(-1);
    }

    py = cvCreateImage(cvSize(w,h), IPL_DEPTH_8U, 1);
    pu = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
    pv = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);

    pu_big = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    pv_big = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);

    image = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3);

    assert(py);
    assert(pu);
    assert(pv);
    assert(pu_big);
    assert(pv_big);
    assert(image);

    // Read Y
    for (i = 0; i < w * h; i++)
    {
        temp = fgetc(pf);
        py->imageData[i] = (unsigned char)temp;
    }

    // Read U
    for (i = 0; i < (w * h) / 4; i++)
    {
        temp = fgetc(pf);
        pu->imageData[i] = (unsigned char)temp;
    }

    // Read V
    for (i = 0; i < (w * h) / 4; i++)
    {
        temp = fgetc(pf);
        pv->imageData[i] = (unsigned char)temp;
    }

    fclose(pf);

    cvResize(pu, pu_big, CV_INTER_LINEAR);
    cvResize(pv, pv_big, CV_INTER_LINEAR);

    cvReleaseImage(&pu);
    cvReleaseImage(&pv);

    cvMerge(py, pu_big, pv_big, NULL, image);

    cvReleaseImage(&py);
    cvReleaseImage(&pu_big);
    cvReleaseImage(&pv_big);

    return image;
}

/*
 * Save image YUV 444 in YUV 420 format
 *
 */
void saveImageYUV(const char* name_file, IplImage* img)
{
    IplImage* pu_small, *pv_small, *py, *pu, *pv;
    int w = img->width;
    int h = img->height;
    int i, temp;
    FILE  * pf;

    py = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    pu = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    pv = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
    pu_small = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);
    pv_small = cvCreateImage(cvSize(w / 2, h / 2), IPL_DEPTH_8U, 1);

    assert(py);
    assert(pu);
    assert(pv);
    assert(pu_small);
    assert(pv_small);

    cvSplit(img, py, pu, pv, NULL);

    cvResize(pu, pu_small, CV_INTER_LINEAR);
    cvResize(pv, pv_small, CV_INTER_LINEAR);

    cvReleaseImage(&pu);
    cvReleaseImage(&pv);

    pf = fopen(name_file, "wb");
    if (pf == NULL)
    {
        fprintf(stderr, "Error open file %s", name_file);
        exit(-1);
    }

    for (i = 0; i < w * h; i++)
       temp = fputc(py->imageData[i], pf);

    for (i = 0; i < (w * h) / 4; i++)
       temp = fputc(pu_small->imageData[i], pf);

    for (i = 0; i < (w * h) / 4; i++)
       temp = fputc(pv_small->imageData[i], pf);

    fclose(pf);
    cvReleaseImage(&py);
    cvReleaseImage(&pu_small);
    cvReleaseImage(&pv_small);
}

/* 
 * Show YUV 444 images after conversion to RGB color
 *
 */
void showImageYUV(const char* window, IplImage* img, int waitTime)
{
    int w = img->width;
    int h = img->height;
    IplImage* dst = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3); 

    cvCvtColor(img,dst,CV_YCrCb2RGB);
    cvShowImage(window, dst);
    cvWaitKey(waitTime);

    cvReleaseImage(&dst);
}