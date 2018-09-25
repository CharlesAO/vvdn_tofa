#include "fastAtan2Positive.h"
#include <mv_types.h>
#include <math.h>
#define MATH_2PI                6.283185307179586
#define PI_OVER_4               0.7853981633974483f
#define D3_PI_OVER_4            2.356194490192345f
#define FASTATAN2_C1            0.9675f
#define FASTATAN2_C3            0.1821f

/// Calculates the principal value of the arctangent of y/x, using the signs of the two arguments to determine the quadrant of the result.
/// @param[in]  y - the y coordinate of the pixel.
/// @param[in]  x - the x coordinate of the pixel.
/// @return     The value of the angle in radians between [0, 2 * PI]
half mvcvFastAtan2Positive(half y, half x)
{
    half angle, r;
    half abs_y    = fabsf(y) + 1e-7;

    if((x == 0.0f) && (y == 0.0f))
    {
        return (half)0.0;
    }
    else if(x > (half)0.0)
    {
        r = (x - abs_y) / (x + abs_y);
        angle = PI_OVER_4;
    }
    else
    {
        r = (x + abs_y) / (abs_y - x);
        angle = D3_PI_OVER_4;
    }
    angle += (FASTATAN2_C3 * r * r - FASTATAN2_C1) * r;
    if (y < (half)0.0)
    {
        return ((half)MATH_2PI - angle);
    }
    else
    {
        return (half)angle;
    }
}

