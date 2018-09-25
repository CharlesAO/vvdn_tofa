#include "bilateral5x5.h"
#include <math.h>

u16 bilateralVal(u16 *pDepthData, int depthStride, u16 sigma);

void mvcvBilateral5x5(u16* input, u16* sigma, u32 width, u16* output)
{
	
	unsigned int i;
	
	output[0] = input[0];
	output[1] = input[1];
	output[width - 1] = input[width - 1];
	output[width - 2] = input[width - 2];

	for(i = 2; i < width - 2; i++)
	{
		output[i] = bilateralVal(input + i, width, sigma[i]);
	}
	
	return;
}


u16 max(u16 a, u16 b)
{
	if(a > b)
		return a;
	return b;
}

u16 bilateralVal(u16 *pDepthData, int depthStride, u16 sigma)
{
	int dCenter = (int) pDepthData[0];
    /*if (0 == dCenter)
    {
        return 0;
    }*/

    float sumDepth = 0;
    float sumWeights = 0;
	int wy;
	int wx;
	
    sigma = max(sigma, (uint16_t)1);
    float sigmaFactor = -0.5f / (float) sigma;

    const u16* pData = pDepthData  -2 * depthStride - 2;
    for (wy = -2; wy <= 2; wy++)
    {

        for (wx = 0; wx < 5; wx++)
        {
            int dCurrent = (int) pData[wx];

            float dist = (float) fabs(dCurrent - dCenter);
			float weight = exp(sigmaFactor * dist);
           
            sumDepth += ((weight * (float) dCurrent));
            sumWeights += weight;
        }
        pData += depthStride;
    }

    return (u16) (0.5f + sumDepth / sumWeights);
}
