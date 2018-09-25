#include "meshExpand.h"

void mvcvMeshExpand(unsigned char *out, unsigned char **in, unsigned int width, half **mesh, float my)
{
	int i, k, ix, iy, posx;
	half dx, dy, nx, ny, ox, oy;
	half *meshBot = mesh[0];
	half *meshTop = mesh[1];
	half contribBot = (half)my;
	half contribTop = (half)(1.0f-my);
	half contribRight[8] = { 0.0f, 0.125f, 0.25f, 0.375f, 0.5f, 0.625f, 0.75f, 0.875f };
	half contribLeft[8] = { 1.0f, 0.875f, 0.75f, 0.625f, 0.5f, 0.375f, 0.25f, 0.125f };
	half meshX0 = meshBot[0]*contribBot+meshTop[0]*contribTop;
	half meshY0 = meshBot[1]*contribBot+meshTop[1]*contribTop;
	half meshX1, meshY1;

	// mesh expansion
	for (i=0; i<(int)width; i+=8) {
		meshX1 = meshBot[0]*contribBot+meshTop[0]*contribTop;
		meshY1 = meshBot[1]*contribBot+meshTop[1]*contribTop;
		for (k=0; k<8; k++) {
			posx = i+k;
			dx = meshX0*contribLeft[k]+meshX1*contribRight[k];
			dy = meshY0*contribLeft[k]+meshY1*contribRight[k];
			ix = (int)dx;
			if (ix<0) ix--;
			iy = (int)dy;
			if (iy<0) iy--;
			nx = dx-(half)ix;
			ny = dy-(half)iy;
			ox = (half)1.0f-nx;
			oy = (half)1.0f-ny;
			out[posx] = (half)in[iy][posx+ix]*ox*oy + \
					    (half)in[iy][posx+ix+1]*nx*oy + \
					    (half)in[iy-1][posx+ix]*ox*ny + \
					    (half)in[iy-1][posx+ix+1]*nx*ny;
		}
		meshX0 = meshX1;
		meshY0 = meshY1;
		meshBot += 2;
		meshTop += 2;
	}

}
