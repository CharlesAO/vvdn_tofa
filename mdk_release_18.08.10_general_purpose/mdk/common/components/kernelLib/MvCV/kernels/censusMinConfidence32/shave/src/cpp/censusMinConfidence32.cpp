#include "censusMinConfidence32.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
// input - input line of the image
// minimumPosition - minimum position of the minimum value
// minimumValue - ratio between first and second minimum values
// width - image width
void mvcvCensusMinConfidence32(u8 *input, u8 *minimumPosition, u8* minimumValue, u32 width)
{
    //u32 j;
    u32 disparities = 32;
    u32 scaleFactor = 255;

    u8 minCost = 255;
    u8 secondMinCost = 255;
    u8 minPos = 0;
    
    for (u32 i = 0; i < width; i++)
    {
        minCost = input[i * disparities];
        minPos = 0;
        secondMinCost = 255;

        // compute minimum value and minimum position
        for (u32 disp = 0; disp < disparities; disp++)
        {
            if (input[i * disparities + disp] < minCost)
            {
                minCost = input[i * disparities + disp];
                minPos = disp;
            }
        }
        minimumPosition[i] = minPos;
        
        // check if first pixel is second min
        if (input[i * disparities + 0] < input[i * disparities + 1] && // is smaller than the right neighbour
            input[i * disparities + 0] != minCost
            &&  input[i * disparities + 0] < secondMinCost) // is not minimum value already found
        {
            secondMinCost = input[i * disparities + 0];
        }

        // compute second minimum value which is also local minimum (minimum of its two neighbours)
        for (u32 disp = 1; disp < (disparities-1); disp++)
        {
            if (input[i * disparities + disp] < input[i * disparities + disp - 1] && // is smaller than the left neighbour
                input[i * disparities + disp] < input[i * disparities + disp + 1] && // is smaller than the right neighbour
                input[i * disparities + disp] != minCost
                &&  input[i * disparities + disp] < secondMinCost) // is not minimum value already found
            {
                secondMinCost = input[i * disparities + disp];
            }
        }

        // check if last pixel is second min
        if (input[i * disparities + (disparities - 1)] < input[i * disparities + (disparities - 1) - 1] && // is smaller than the left neighbour
            input[i * disparities + (disparities - 1)] != minCost
            &&  input[i * disparities + (disparities - 1)] < secondMinCost) // is not minimum value already found
        {
            secondMinCost = input[i * disparities + (disparities - 1)];
        }
/*         printf("mincost = %5d minpos = %5d secondMinCost = %5d\n", minCost, minPos, secondMinCost); 
		//if(i == 10 ) exit(0); */
        minimumValue[i] = scaleFactor * minCost / secondMinCost;
    }
}
