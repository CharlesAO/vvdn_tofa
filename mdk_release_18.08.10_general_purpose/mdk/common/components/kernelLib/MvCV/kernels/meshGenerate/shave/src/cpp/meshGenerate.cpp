#include <meshGenerate.h>

void mvcvMeshGenerate(half *mesh, float *mat, u32 width, u32 posx, u32 posy)
{
	unsigned int i;
	float xo, yo;

	yo = (float)posy;
	for (i=0; i<width; i++)
	{
		xo = (float)(posx+i);
		// x increases naturally
		mesh[0] = (half)((xo*mat[0]+yo*mat[1]+mat[2])-xo);
		// y increases inverted, due to sipp pointer mechanism
		mesh[1] = (half)(yo-(xo*mat[3]+yo*mat[4]+mat[5]));
		mesh += 2;
	}
}
