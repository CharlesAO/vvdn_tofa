#include "dummy/shared/RANSAC.h"
#include "RANSAC_asm_test.h"
#include "InputGenerator.h"
#include "UniformGenerator.h"
#include "RandomGenerator.h"
#include <ctime>
#include <memory>
#include <stdlib.h>
#include "ArrayChecker.h"
#include <cmath>
#include "dummy/shared/FundamentalMatrix.h"
#include <iostream>
#include <tuple>
#include <functional>
using ::testing::TestWithParam;
using ::testing::Values;

float p1[2*MAX_NUMPOINTS];
float p2[2*MAX_NUMPOINTS];

float fm[9];
int Inliers[MAX_NUMPOINTS];
struct fundamental_debug Debug;
struct ransac_params Params;

double eye[9] = {1,0,0,0,1,0,0,0,1};
double T[9] = {1,0,10,0,1,-10,0,0,1};

#define rand PRNG2

unsigned int PRNG2()
{
    // our initial starting seed is 5323
    static unsigned int nSeed = 5323;

    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);

    // Take the seed and return a value between 0 and 32767
    return nSeed  % RAND_MAX;
}


void CreateOutliers(float *p1, float * p2,int numoutliers,int numpoints)
{
    int n;
    unsigned int dist = (numpoints-1)/numoutliers;
    assert(numoutliers<=numpoints);
    for (n=0; n<numoutliers; n++)
    {

        unsigned int point = dist * n ;
        p1[2* point] = NAN;//p1[2* point] + 1000 + (float)(rand()%500);
        p1[2* point+1] = NAN;//p1[2* point+1] + 1000 + (float)(rand()%500);
        p2[2* point] = NAN;//p2[2* point] + 1000 + (float)(rand()%500);
        p2[2* point+1] = NAN;//p2[2* point+1] + 1000 + (float)(rand()%500);

    }
}



void AddNoise(float *p1, float * p2,float noise,int numpoints)
{
    int n;
    for (n=0; n<numpoints; n++)
    {

        p1[2*n] += noise*(2*((float)rand() / (float)RAND_MAX)-1);
        p1[2*n+1] += noise*(2*((float)rand() / (float)RAND_MAX)-1);
        p2[2*n] += noise*(2*((float)rand() / (float)RAND_MAX)-1);
        p2[2*n+1] += noise*(2*((float)rand() / (float)RAND_MAX)-1);

    }
}




int RANSACApp_genSparsePoints(float *p, double *T, int w, int h, int numpoints)
{
    float x,y,Tx,Ty;
    int n=0;

    float distance = sqrt(w*h/numpoints);

    for(y=distance/2; y+distance<h; y+=distance)
    {
        for(x=distance/2; x+distance<w; x+=distance)
        {
            n++;
            assert(n<=numpoints);


            Tx = T[0] * x + T[1] * y + T[2];
            Ty = T[3] * x + T[4] * y + T[5];



            *p++ = Tx;
            *p++ = Ty;


        }
    }
    return(n);
}




float CalcEps(double *T,
              float *p,
              int numpoints,
              float *fm,
              int *inliers)
{

    float eps;
    float aveps = 0;
    float maxeps = 0;
    float *Pi_dash;
    float *PiF;
    int n_ins = 0;
    Pi_dash = new float[2];
    PiF = new float[3];

   /* printf("fm=");
    for (int n=0; n<9; n+=3)
    {
        printf("%f        %f        %f\n",fm[n],fm[n+1],fm[n+2]);

    }*/


    for (int n = 0; n<numpoints; n++)
    {


        if(inliers[n])
        {
            // Transfer the point by the known true transformation
            Pi_dash[0] = T[0] * p[2*n] + T[1] * p[2*n+1] + T[2];
            Pi_dash[1] = T[3] * p[2*n] + T[4] * p[2*n+1] + T[5];

            // Calculate Pi*F
            PiF[0] = p[2*n]*fm[0] + p[2*n+1]*fm[1] + fm[2];
            PiF[1] = p[2*n]*fm[3] + p[2*n+1]*fm[4] + fm[5];
            PiF[2] = p[2*n]*fm[6] + p[2*n+1]*fm[7] + fm[8];
            //Calculate Pi*F*pi_dash
            eps = fabs(PiF[0]*Pi_dash[0] + PiF[1]*Pi_dash[1]+PiF[2]);
            if(eps > maxeps)
            {
                maxeps = eps;
            }
            aveps += eps;
            n_ins++;
        }
    }

    aveps /= n_ins;
    delete [] Pi_dash;

    return(aveps);


}



class RANSACAppTest :  public ::testing::TestWithParam<
    std::tr1::tuple<float, float, int, bool> >
{
protected:

    virtual void SetUp()
    {
        randGen.reset(new RandomGenerator);
        uniGen.reset(new UniformGenerator);
        inputGen.AddGenerator(std::string("random"), randGen.get());
        inputGen.AddGenerator(std::string("uniform"), uniGen.get());
    }


    int numpoints;
    int num_inliers;
    int InjectedOutliers;
    float pc_error;
    float myriad_error;
    RandomGenerator dataGenerator;
    InputGenerator inputGen;
    ArrayChecker outputCheck;
    std::auto_ptr<RandomGenerator>  randGen;
    std::auto_ptr<UniformGenerator>  uniGen;

    //virtual void TearDown() {printf("MY_________TEARDOWN\n");}
};







INSTANTIATE_TEST_CASE_P(
    TestWithParameters, RANSACAppTest, Values(
        //           Noise %Outliers expect_equal_outier

        std::tr1::make_tuple( 5.0, 0.3 ,1000,false), // Test that +/- 5 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 2.0, 0.3 ,1000,false), // Test that +/- 2 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 1.0, 0.3 ,1000,false), // Test that +/- 1 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.7, 0.3 ,1000,false), // Test that +/- .7 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.6, 0.3 ,1000,false), // Test that +/- .6 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.5, 0.3 ,1000,false), // Test that +/- .5 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.4, 0.3 ,1000,false), // Test that +/- .4 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.3, 0.3 ,1000,false), // Test that +/- .3 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.2, 0.3 ,1000,false), // Test that +/- .2 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.1, 0.3 ,1000,false), // Test that +/- .1 pixel noise is solved with 30% outliers
        std::tr1::make_tuple( 0.0, 0.4 ,1000,true),  // Test for exactly 40% inliers
        std::tr1::make_tuple( 0.0, 0.3 ,1000,true),  // Test for exactly 30% inliers
        std::tr1::make_tuple( 0.0, 0.2 ,1000,true),  // Test for exactly 20% inliers
        std::tr1::make_tuple( 0.0, 0.1 ,1000,true)   // Test for exactly 10% inliers
    ));

TEST_P(RANSACAppTest, ParameterisedTesting)
{
    float noise = std::tr1::get<0>(GetParam());
    float outlier_frac = std::tr1::get<1>(GetParam());
    int numpoint_req = std::tr1::get<2>(GetParam());
    bool expect_equal_outliers = std::tr1::get<3>(GetParam());
    srand(1);

    numpoints = RANSACApp_genSparsePoints(p1,eye,1920,1080,numpoint_req);
    numpoints = RANSACApp_genSparsePoints(p2,eye,1920,1080,numpoint_req);
    InjectedOutliers = (int)(outlier_frac * (float)numpoints);
    AddNoise(p1,p2,noise,numpoints);
    CreateOutliers(p1,p2,InjectedOutliers,numpoints);

    Params.inliers_ratio = ((float)(numpoints-InjectedOutliers)/(float)numpoints)-0.11;
    Params.confidence =  0.999;
    Params.dist_threshold = 1;
    Params.max_iterations = 10000;
    Params.nPoints = numpoints;
   /* printf("InjectedOutliers:%d\n",InjectedOutliers);
    printf("numpoints:%d\n",numpoints);
    printf("Noise:%f\n",noise);

    printf("inliers_ratio:%f\n",Params.inliers_ratio);
    printf("confidence:%f\n",Params.confidence);
    printf("dist_threshold:%f\n",Params.dist_threshold);
    printf("max_iterations:%d\n",Params.max_iterations);
*/
    printf("PC CODE\n");
    findFundamentalMat(p1,p2,numpoints,fm,Inliers,&Debug,Params.inliers_ratio,Params.confidence,Params.dist_threshold,Params.max_iterations);
    pc_error = CalcEps(eye,p1,numpoints,fm,Inliers);


   // printf("RANSAC_asm_test\n");
    RANSAC_asm_test(p1,p2, fm, Inliers,&Params,&Debug);
    myriad_error = CalcEps(eye,p1,numpoints,fm,Inliers);
   // printf("myriad_error=%f\n",myriad_error);
   // printf("pc_error=%f\n",pc_error);
    if(expect_equal_outliers)
    {
        EXPECT_EQ(numpoints - Debug.RANSAC_nInliers,InjectedOutliers);
    }
    else
    {
        EXPECT_GE(numpoints - Debug.RANSAC_nInliers,InjectedOutliers);
    }
    EXPECT_NEAR(myriad_error,pc_error,0.21);


}





