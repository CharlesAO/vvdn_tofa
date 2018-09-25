#include <stdlib.h>
#include <swcFrameTypes.h>
#include <OsDrvSvu.h>
#include <stdio.h>
#include <DrvCpr.h>
#include "rtems_config.h"
#include <rtems/bspIo.h>
#include "RANSAC.h"
#include <math.h>
#include <VcsHooksApi.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

#define SHAVE_NUMBER    0



// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

extern u32 dummy0_start;

// 4: Static Local Data
// ----------------------------------------------------------------------------
RANSAC_input RANSAC_In;
RANSAC_output RANSAC_Out;
struct ransac_params Params;
struct fundamental_debug Debug;


static osDrvSvuHandler_t handler;
float p1[2*MAX_NUMPOINTS];
float p2[2*MAX_NUMPOINTS];
int Inliers[MAX_NUMPOINTS];
float fm[9];
float eye[9] = {1,0,0,0,1,0,0,0,1};
float T[9] = {1,0,10,0,1,-10,0,0,1};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

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




int RANSACApp_genSparsePoints(float *p, float *T, int w, int h, int numpoints)
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


void RANSAC_Init(void)
{
    RANSAC_In.p1 = &p1[0];
    RANSAC_In.p2 = &p2[0];

    Params.nPoints = 0;
    Params.inliers_ratio = 0.5;
    Params.confidence =  0.999;
    Params.dist_threshold = 1;
    Params.max_iterations = 1000;
    Params.nPoints = RANSACApp_genSparsePoints(p1,eye,1920,1080,MAX_NUMPOINTS);
    Params.nPoints = RANSACApp_genSparsePoints(p2,eye,1920,1080,MAX_NUMPOINTS);
    AddNoise(RANSAC_In.p1,RANSAC_In.p2,1,Params.nPoints);
    RANSAC_In.Params = &Params;

    RANSAC_Out.inliers = &Inliers[0];
    RANSAC_Out.Debug = &Debug;
    RANSAC_Out.fm = &fm[0];
}



void *POSIX_Init (void *args)
{
    int i;
    u32 running;
//    printf ("RTEMS POSIX Started **************************\n ************************** \n");  /* initialise variabes */

    initClocksAndMemory();
//    printf ("RTEMS POSIX Started **************************\n ************************** \n");  /* initialise variables */



    OsDrvSvuInit();

    if (OsDrvSvuOpenShave(&handler, SHAVE_NUMBER, OS_MYR_PROTECTION_SEM) == OS_MYR_DRV_SUCCESS)
    {
        printf("L0\n");
        OsDrvSvuResetShave(&handler);
        printf("L1\n");
        OsDrvSvuSetAbsoluteDefaultStack(&handler);
        printf("L2\n");
        RANSAC_Init();

        printf("L4\n");
        OsDrvSvuStartShaveCC(&handler,(u32)&dummy0_start, "ii", ((u32)&RANSAC_In), ((u32)&RANSAC_Out));
        printf("L5\n");
        OsDrvSvuWaitShaves(1, &handler, OS_DRV_SVU_WAIT_FOREVER, &running);
        printf("L6\n");
        OsDrvSvuCloseShave(&handler);
        printf("L7\n");
    }
    else
    {
        printf("[T2] cannot open shave %d\n", SHAVE_NUMBER);
    }
    ;

    exit(0);
}

// User extension to be able to catch abnormal terminations
static void Fatal_extension(
    Internal_errors_Source  the_source,
    bool                    is_internal,
    Internal_errors_t       the_error
)
{
    switch(the_source)
    {
    case RTEMS_FATAL_SOURCE_EXIT:
        if(the_error)
        {
            printk("Exited with error code %d\n", the_error);
        }
        break; // normal exit
    case RTEMS_FATAL_SOURCE_ASSERT:
        printk("%s : %d in %s \n",
               ((rtems_assert_context *)the_error)->file,
               ((rtems_assert_context *)the_error)->line,
               ((rtems_assert_context *)the_error)->function);
        break;
    case RTEMS_FATAL_SOURCE_EXCEPTION:
        rtems_exception_frame_print((const rtems_exception_frame *) the_error);
        break;
    default:
        printk ("\nSource %d Internal %d Error %d  0x%X:\n",
                the_source, is_internal, the_error, the_error);
        break;
    }
}


