#include "tile_sampleru10.h"
#include <stdlib.h>
#include <math.h>

void mvcvtile_sampleru10(unsigned short *tile, unsigned short * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height)
{
	int i, j;
	unsigned int mesh_gran_x = 16, mesh_gran_y = 16;
	float x_sample,y_sample,x,y ;
	float a11, a12, a21, a22;
	float tx, ty, b1, b2;
	
	tile_height+=1;
	for (i = 0; i< (int)mesh_gran_y; i++)
        {
		for (j = 0; j< (int)mesh_gran_x; j++)
		{
			x_sample = ((mesh_gran_y - i) * ((xcoord[0] * (mesh_gran_x - j) + xcoord[1] * j)/mesh_gran_x) +
				                       i  * ((xcoord[2] * (mesh_gran_x - j) + xcoord[3] * j)/mesh_gran_x))/mesh_gran_y ;
			
			y_sample = ((mesh_gran_y - i) * ((ycoord[0] * (mesh_gran_x - j) + ycoord[1] * j)/mesh_gran_x) +
				                       i  * ((ycoord[2] * (mesh_gran_x - j) + ycoord[3] * j)/mesh_gran_x))/mesh_gran_y ;


			x = floor(x_sample);
			y = floor(y_sample);

						
			    a11 = tile[(unsigned int)( (y + 0)  * tile_width + (x + 0))]; 
			    a12 = tile[(unsigned int)( (y + 0)  * tile_width + (x + 1))]; 
						   
			    a21 = tile[(unsigned int)( (y + 1)  * tile_width + (x + 0))]; 
			    a22 = tile[(unsigned int)( (y + 1)  * tile_width + (x + 1))]; 
										   				    		  							  		  				
			
			tx = x_sample;
			tx = tx - floor(tx);
			ty = y_sample;
			ty = ty - floor(ty);



				b1 = (1-tx) * a11 + tx * a12;
				b2 = (1-tx) * a21 + tx * a22;



				out_tile[i*16 + j] = (1-ty) * b1 + ty * b2;
		

		}
        }
		return;
}
