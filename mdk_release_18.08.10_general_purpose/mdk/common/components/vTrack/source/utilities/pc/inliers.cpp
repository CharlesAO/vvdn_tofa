#include "inliers.h"

Inliers::Inliers(float fx, float fy, float cx, float cy, bool useOpenCV  , bool useUndistort , float param1, float param2 )
{
    this->useOpenCV = useOpenCV;
    this->useUndistort = useUndistort;
    this->fx = fx;
    this->fy = fy;
    this->cx = cx;
    this->cy = cy;
    if(useOpenCV)
    {
        initCameraParams(param1,param2);
    }
    else
    {
        this->w = param1;
    }
}

void Inliers::initCameraParams(float dist0,float  dist1)
{
        camera_matrix = new cv::Mat(3,3,CV_64F);
        dist_coeffs = new cv::Mat(8,1,CV_64F);
        camera_matrix->at<double>(0,0) = this->fx;// fx
        camera_matrix->at<double>(0,1) = 0;
        camera_matrix->at<double>(0,2) = this->cx; //cx
        camera_matrix->at<double>(1,0) = 0;
        camera_matrix->at<double>(1,1) = this->fy; //fy
        camera_matrix->at<double>(1,2) = this->cy; //cy
        camera_matrix->at<double>(2,0) = 0;
        camera_matrix->at<double>(2,1) = 0;
        camera_matrix->at<double>(2,2) = 1;
        dist_coeffs->at<double>(0,0) = dist0; //dist0
        dist_coeffs->at<double>(0,1) = dist1; //dist1
}

void Inliers::addMatchesToList(bool * isInlier,float currX, float currY, float prevX, float prevY)
{
    matches_currImage.push_back(Point2f(currX, currY));
    matches_prevImage.push_back(Point2f(prevX, prevY));
    isInlierList.push_back(isInlier);
}

void Inliers::unwarpPoints(std::vector<Point2f> * unwarpedPoints,std::vector<Point2f> matches_image)
{
    float mul2tanwby2;
    float normX, normY;
    float retX , retY;
    float normVec;
    float r_u;
    int i;
    mul2tanwby2 = tan(w/2) * 2;
    for(i = 0; i< matches_image.size() ; i++)
    {
        normX = (matches_image[i].x - cx) / fx;
        normY = (matches_image[i].y - cy) / fy;
        normVec = sqrt(normX*normX + normY*normY);
        if ((abs(normVec * w) <= 89.0 * M_PI / 180.0) && (normVec * mul2tanwby2 != 0) )
        {
            r_u = tan(normVec * w) / (normVec * mul2tanwby2);
        }
        else
        {
            r_u = std::numeric_limits<float>::max();
        }
        retX = (r_u * normX * fx) + cx;
        retY = (r_u * normY * fy) + cy;
        unwarpedPoints->push_back(Point2f(retX, retY));
    }

}

int Inliers::calcPercentageInliers()
{

    cv::Mat  points1(matches_prevImage);
    cv::Mat  points2(matches_currImage);
    int inliersCount = 0;
    int i;

    if(matches_currImage.size() == 0 || matches_prevImage.size() == 0)
    {
        clearPoints();
        return 0;
    }

    if(useUndistort)
    {
        if(useOpenCV)
        {
            undistortPoints(points1, points1, *(camera_matrix), *(dist_coeffs),  cv::Mat(), *(camera_matrix));
            undistortPoints(points2, points2, *(camera_matrix), *(dist_coeffs),  cv::Mat(), *(camera_matrix));
        }
        else
        {
            std::vector<Point2f> unwarpedCurrPoints;
            std::vector<Point2f> unwarpedPrevPoints;
            unwarpPoints(&unwarpedPrevPoints, matches_prevImage);
            unwarpPoints(&unwarpedCurrPoints, matches_currImage);
            points1 = cv::Mat(unwarpedPrevPoints);
            points2 = cv::Mat(unwarpedCurrPoints);
        }
    }
    cv::Mat status(points1.rows,1,CV_8U);
    cv::Mat fundamentalMatrix = cv::findFundamentalMat(points1, points2 , CV_FM_RANSAC, MAX_EPIPOLAR_DISTANCE, 0.99, status);
    if(fundamentalMatrix.empty())
    {
        printf("matrix empty \n");
        clearPoints();
        return 0;
    }
    for(i = 0; i < points1.rows; i++)
    {
        if((bool)status.at<uchar>(i,1))
        {
            inliersCount++;
            *(isInlierList.at(i))=true;
        }
        else
        {
            *(isInlierList.at(i))=false;
        }
    }
    clearPoints();

    return (int)((float)(inliersCount)/points1.rows * 100);
}

void Inliers::clearPoints()
{
    matches_currImage.clear();
    matches_prevImage.clear();
    isInlierList.clear();
}
