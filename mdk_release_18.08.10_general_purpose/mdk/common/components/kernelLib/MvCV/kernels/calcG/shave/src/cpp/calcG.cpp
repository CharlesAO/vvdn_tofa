#include "calcG.h"
#include <mvcv_types.h>
/// calcG - calcOpticalFlow component 
///@param[in] Ix		- pointer tot the X line 
///@param[in] Iy		- pointer tot the Y line 
///@param[in] isz		- ClSize
///@param[in] jsz		- ClSize
///@param[in] minI		- mvCvPoint
///@param[in] minJ		- mvCvPoint
///@param[out] G		- output scalar

mvScalar myScalar(float s0, float s1, float s2, float s3)
{
	mvScalar s;

	s[0] = s0;
	s[1] = s1;
	s[2] = s2;
	s[3] = s3;

	return s;
}

void mvcvCalcG(const float* Ix, ClSizeW isz, mvCvPointW minI, const float* Iy, ClSizeW jsz, mvCvPointW minJ, mvScalar& G)
{
    float Gxx = 0;
	float Gyy = 0;
	float Gxy = 0;
	float D = 0;

	//svu_assert(isz.x + minI.x <= 13 && isz.y + minI.y <= 13 && "isz wrong\n");
	//svu_assert(jsz.x + minJ.x <= 11 && jsz.y + minJ.y <= 11 && "jsz wrong\n");

	for (int y = 0; y < jsz.y; y++)
	{
		const float* ix = Ix +
			(y + minJ.y - minI.y) * (isz.x - 2) + minJ.x - minI.x;
		const float* iy = Iy + (ix - Ix);

		// Vectorize
		for(int x = 0; x < jsz.x; x++)
		{
			// Compute the gradients matrix (eq. 24)
			Gxx += ix[x] * ix[x];
			Gxy += ix[x] * iy[x];
			Gyy += iy[x] * iy[x];
		}
	}

	// Determinant of gradients matrix must not be 0 so the
	// matrix in (eq. 28) is invertible
	D = Gxx * Gyy - Gxy * Gxy;

	//return cvScalar(Gxx, Gyy, Gxy, D);
	G = myScalar(Gxx, Gyy, Gxy, D);
	
	return;
}
