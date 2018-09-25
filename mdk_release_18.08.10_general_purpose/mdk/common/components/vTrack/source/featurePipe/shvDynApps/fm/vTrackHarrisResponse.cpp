#include "mv_types.h"
#include "vTrackHarrisResponse.h"
#include "stdio.h"

fp32 harrisResponse(u8 *data, u32 x, u32 y, u32 step_width, fp32 k){
    #define RADIUS  (1) // asm, for speed reason suport jus radius 3
    s32 dx;
    s32 dy;
    float xx = 0;
    float xy = 0;
    float yy = 0;
//    printf("(%u, %u)\n", x, y);
    // Skip border and move the pointer to the first pixel
//  data += step_width + 1;

    float weights[9] = {1,2,1,
                        2,4,2,
                        1,2,1};

    for (u32 r = y - RADIUS; r <= y + RADIUS; r++)
    {
        for (u32 c = x - RADIUS; c <= x + RADIUS; c++)
        {
            s32 curr = r * step_width + c;
            s32 currM1 = (r - 1) * step_width + c;
            s32 currP1 = (r + 1) * step_width + c;


            dx = 2*(data[curr - 1] - data[curr + 1]) + (data[currM1 - 1] - data[currM1 + 1]) + (data[currP1 - 1] - data[currP1 + 1]);
            dy = 2*(data[currM1] - data[currP1]) + (data[currM1-1] - data[currP1-1]) + (data[currM1+1] - data[currP1+1]);

            float weight = weights[(r - (y - RADIUS)) * 3 + (c - (x - RADIUS))]/16.0f;
            xx += weight * dx * dx;
            xy += weight * dx * dy;
            yy += weight * dy * dy;
        }
    }

    float det = xx * yy - xy * xy;
    float trace = xx + yy;

    //k changes the response of edges.
    //seems sensitive to window size, line thickness, and image blur =o/
    return (det - k * trace * trace);

}


