#include "warpMeshExpand.h"
#include <stdlib.h>
#include <math.h>

#define TILE_SIZE 16
void mvcvWarpMeshExpand(half *xcoord, half *ycoord, half *mx, half *my)
{
	int i, j;
    int mesh_gran_x = 16, mesh_gran_y = 16;
	half x_sample,y_sample;
	

	for (i = 0; i< mesh_gran_y; i++)
        {
		for (j = 0; j< mesh_gran_x; j++)
		{
			x_sample = ((mesh_gran_y - i) * ((xcoord[0] * (mesh_gran_x - j) + xcoord[1] * j)/mesh_gran_x) +
				                       i  * ((xcoord[2] * (mesh_gran_x - j) + xcoord[3] * j)/mesh_gran_x))/mesh_gran_y ;
			
			y_sample = ((mesh_gran_y - i) * ((ycoord[0] * (mesh_gran_x - j) + ycoord[1] * j)/mesh_gran_x) +
				                       i  * ((ycoord[2] * (mesh_gran_x - j) + ycoord[3] * j)/mesh_gran_x))/mesh_gran_y ;

			mx[i*TILE_SIZE + j] = x_sample;
			my[i*TILE_SIZE + j] = y_sample;
		}
        }
		return;
}
