#include "interpolatePixelBilinearS16.h"
#include <mv_types.h>
#include <math.h>

float mvcvInterpolatePixelBilinearS16(s16* line1, s16* line2, fp32 x, fp32 y)
{
    int xp, yp;
    int xp_plus_1, yp_plus_1;
    float w0, w1, w2, w3;
    s16* p0;
    s16* p1;
    float result;

    xp = (int)x;
    yp = (int)y;
    xp_plus_1 = xp+1;
    yp_plus_1 = yp+1;

    p0 = line1;
    p1 = line2;

    w0 = (xp_plus_1 - x)  * (yp_plus_1 - y);
    w1 = (x         - xp) * (yp_plus_1 - y);
    w2 = (xp_plus_1 - x)  * (y         - yp);
    w3 = (x         - xp) * (y         - yp);

    result = w0*p0[xp] + w1*p0[xp_plus_1] + w2*p1[xp] + w3*p1[xp_plus_1];

    return result;
}
