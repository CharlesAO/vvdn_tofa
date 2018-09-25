// Calc Empipolar Distance kernel test

//Asm function prototype:
// void mvcvCalcEpipolarDistance_asm(float *p_1, float *p_2, int nPoints, 
//                               float RANSAC_dist_threshold, float *fm, 
//                               int *inliers, int *nInliers);

//Asm test function prototype:
// void mvcvCalcEpipolarDistance_asm_test(float *p_1, float *p_2, int nPoints, 
//                               float RANSAC_dist_threshold, float *fm, 
//                               int *inliers, int *nInliers);

//C function prototype:
// void mvcvCalcEpipolarDistance(float *p_1, float *p_2, int nPoints, 
//                               float RANSAC_dist_threshold, float *fm, 
//                               int *inliers, int *nInliers);

//p_1                      - pointer to image 1 input points
//p_2                      - pointer to image 2 input points
//nPoints                  - number of points
//RANSAC_dist_threshold    - inlier threshold
//fm                       - fundamental matrix
//inliers                  - inliers vector
//nInliers                 - number of inliers

#include "gtest/gtest.h"
#include "calcEpipolarDistance.h"
#include "calcEpipolarDistance_asm_test.h"
#include "RandomGenerator.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "TestEventListener.h"
#include "ArrayChecker.h"
#include <ctime>

#define DELTA 0.0001

class calcEpipolarDistanceTest : public ::testing::Test {
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }   

    float *p1;
    float *p2;
    int nPoints;
    float RANSAC_dist_threshold;
    float *fm;
    int *outLineC, *outLineAsm;
    int nInliersC, nInliersAsm;
    float distC, distAsm;

    InputGenerator inputGen;
    RandomGenerator randomGen;
    ArrayChecker outputCheck;
    
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    virtual void TearDown() {}
};

TEST_F(calcEpipolarDistanceTest, TestNPoints240)
{
    nPoints = 240;
    RANSAC_dist_threshold = 1.2;

    inputGen.SelectGenerator("random");
    p1 = inputGen.GetLineFloat(2 * nPoints, -2.0, 2.0);
    p2 = inputGen.GetLineFloat(2 * nPoints, -2.0, 2.0);
    fm = inputGen.GetLineFloat(9, -10.0, 20.0);

    outLineC = (int*)inputGen.GetEmptyLine(nPoints * sizeof(int));
    outLineAsm = (int*)inputGen.GetEmptyLine(nPoints * sizeof(int));
    
    distC = mvcvCalcEpipolarDistance(p1, p2, nPoints, RANSAC_dist_threshold, fm, 
                                     outLineC, &nInliersC);

    distAsm = calcEpipolarDistance_asm_test(p1, p2, nPoints, RANSAC_dist_threshold, 
                                            fm, outLineAsm, &nInliersAsm);
    RecordProperty("CyclePerPixel", calcEpipolarDistanceCycleCount / nPoints);
    
    EXPECT_EQ(nInliersC, nInliersAsm);
    EXPECT_NEAR(distC, distAsm, DELTA);
    outputCheck.CompareArrays(outLineC, outLineAsm, nPoints); 
}

TEST_F(calcEpipolarDistanceTest, TestNPoints1920)
{
    nPoints = 1920;
    RANSAC_dist_threshold = 2.31;

    inputGen.SelectGenerator("random");
    p1 = inputGen.GetLineFloat(2 * nPoints, -1.2, 0.57);
    p2 = inputGen.GetLineFloat(2 * nPoints, -2.0, 2.0);
    fm = inputGen.GetLineFloat(9, -1.5, 2.5);

    outLineC = (int*)inputGen.GetEmptyLine(nPoints * sizeof(int));
    outLineAsm = (int*)inputGen.GetEmptyLine(nPoints * sizeof(int));
    
    distC = mvcvCalcEpipolarDistance(p1, p2, nPoints, RANSAC_dist_threshold, fm, 
                                     outLineC, &nInliersC);

    distAsm = calcEpipolarDistance_asm_test(p1, p2, nPoints, RANSAC_dist_threshold, 
                                            fm, outLineAsm, &nInliersAsm);
    RecordProperty("CyclePerPixel", calcEpipolarDistanceCycleCount / nPoints);
    
    EXPECT_EQ(nInliersC, nInliersAsm);
    EXPECT_NEAR(distC, distAsm, DELTA);
    outputCheck.CompareArrays(outLineC, outLineAsm, nPoints); 
}

TEST_F(calcEpipolarDistanceTest, TestNPoints7)
{
    nPoints = 8;
    RANSAC_dist_threshold = 0.34;

    inputGen.SelectGenerator("random");
    p1 = (float*)inputGen.GetEmptyLine(2 * nPoints * sizeof(float));
    p2 = (float*)inputGen.GetEmptyLine(2 * nPoints * sizeof(float));
    fm = (float*)inputGen.GetEmptyLine(9  * sizeof(float));

    p1[0] = -0.273531;    p2[0] = 1.182971;
    p1[1] = -0.512795;    p2[1] = 0.474989;
    p1[2] = 1.686613;     p2[2] = 1.643552;
    p1[3] = 0.661141;     p2[3] = -0.888699;
    p1[4] = -1.885330;    p2[4] = -0.756643;
    p1[5] = 1.568609;     p2[5] = 1.073735;
    p1[6] = 1.085785;     p2[6] = 0.888303;
    p1[7] = -0.866918;    p2[7] = -0.063556;
    p1[8] = 1.270042;     p2[8] = 0.637958;
    p1[9] = -0.809517;    p2[9] = -1.634654;
    p1[10] = 0.995212;    p2[10] = 0.370215;
    p1[11] = -1.240571;   p2[11] = -0.766253;
    p1[12] = 0.169067;    p2[12] = -0.653794;
    p1[13] = -1.225295;   p2[13] = 0.851665;
    p1[14] = -1.225295;   p2[14] = 0.851665;

    fm[0] = -0.044554;
    fm[1] = 1.579207;
    fm[2] = -1.795464;
    fm[3] = 1.681915;
    fm[4] = -0.933588;
    fm[5] = 1.891149;
    fm[6] = 0.343055;
    fm[7] = -0.818918;
    fm[8] = 1.459759;

    outLineC = (int*)inputGen.GetEmptyLine(nPoints * sizeof(int));
    outLineAsm = (int*)inputGen.GetEmptyLine(nPoints * sizeof(int));
    
    distC = mvcvCalcEpipolarDistance(p1, p2, nPoints, RANSAC_dist_threshold, fm, 
                                     outLineC, &nInliersC);


    distAsm = calcEpipolarDistance_asm_test(p1, p2, nPoints, RANSAC_dist_threshold, 
                                            fm, outLineAsm, &nInliersAsm);
    RecordProperty("CyclePerPixel", calcEpipolarDistanceCycleCount / nPoints); 

    EXPECT_EQ(nInliersC, nInliersAsm);
    EXPECT_NEAR(distC, distAsm, DELTA);
    outputCheck.CompareArrays(outLineC, outLineAsm, nPoints);
}