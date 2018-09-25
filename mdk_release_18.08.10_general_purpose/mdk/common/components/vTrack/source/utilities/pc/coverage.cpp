///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Implements the coverage calculation
///
#include "coverage.h"

float calcDistance(std::pair<float, float> p1 ,std::pair<float, float> p2)
{
    return sqrt((p1.first-p2.first)*(p1.first-p2.first)+(p1.second-p2.second)*(p1.second-p2.second));
}
float calcCoverage(std::vector<std::pair<float, float> > list)
{
    int i , j;
    std::pair<int , int> ref;
    int n;
    float rez;
    float Di;
    float sumOfDistances;
    float sumOfDi = 0;
    n=list.size();
    for(i = 0; i<n; i++)
    {
        sumOfDistances = 0;
        ref = list[i];
        for(j = 0; j<n; j++ )
        {
            if(j==i)
            {
                continue;
            }
            rez=calcDistance(list[i],list[j]);
            if(rez!=0)
            {
                sumOfDistances += 1/rez;
            }
        }
        if(sumOfDistances!=0)
        {
            Di = (n-1)/sumOfDistances;
            sumOfDi += 1/Di;
        }
    }
    if(sumOfDi!=0)
    {
        return n/sumOfDi;
    }
    return 0;
}
