#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/shave_system.h>
#include <CyclesCount.h>
#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Jacobi>
#include <math.h>

#define MAX_MM 5000
#define NB_POINT 27

using namespace std;
using namespace Eigen;

typedef unsigned int u32;

// the code for calculating the mflops is commented

void benchRot()
{
    std::cout<<"\n\n\n----- Bench rotation  -----\n\n";
    int k=2;
    int size = k*k; // matrix size
    u32 tick;
    float c=3.0;
    float s=1.0;
//    double tab_mflops[NB_POINT];
    int tab_sizes[NB_POINT];
    int count=0;

    while(size<=MAX_MM)
    {
        tab_sizes[count]=size;

        // vector initialization
        VectorXf A(size);
        VectorXf B(size);
        for(int i=0;i<size;i++){
            A[i]=i;
            B[i]=size-i;
        }

        //nb_op_base represents the number of operations
 //       double nb_op_base = 6.0*size;

        //getting the execution time
        tick = swcReadPC3();
        for(int i=0;i<NB_REP;i++)
        internal::apply_rotation_in_the_plane(A,B, JacobiRotation<float>(c,s));
        tick = swcReadPC3()-tick;
//        float exec_time = (((float)tick/NB_REP)*(1.0000000/(float)FRQ));
        float cc = (float)tick/(float)NB_REP;
        printf("Size[%d][%d]= %d => CycleCount = %f \n",k,k,size,cc);

        //applying the mflops formula: number of operations/(execution time * 1e6)
//        tab_mflops[count++] = ((float)nb_op_base/exec_time)/1000000.000000;

        if(k<9)  k=k+1;
            else k=k+3;
        size = k*k;
    }

//     std::cout<<"\n\n ----- TESTING ----- \n";
//     for(int i=0;i<count;i++){
//         printf("%d. size= %d   ==>   mflops = %g  \n",i,tab_sizes[i], tab_mflops[i]);
//     }
    std::cout<<"\n --> Finished rotation ! <-- \n\n"<< std::flush;

}
