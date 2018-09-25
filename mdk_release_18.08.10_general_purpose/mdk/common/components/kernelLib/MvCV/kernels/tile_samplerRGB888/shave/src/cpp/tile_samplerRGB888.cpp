#include "tile_samplerRGB888.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void mvcvtile_samplerRGB888(unsigned char *tile, unsigned char * out_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height)
{
	int i, j;
	unsigned int mesh_gran_x = 16, mesh_gran_y = 16;
	float x_sample,y_sample,x,y ;
	float a11R, a12R, a21R, a22R;
	float a11G, a12G, a21G, a22G;
	float a11B, a12B, a21B, a22B;
	float tx, ty, b1R, b2R;
	float         b1G, b2G; 
	float         b1B, b2B; 
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

						
			    a11R = tile[(unsigned int)( (y + 0)  * tile_width * 3 + (x * 3 + 0))]; 
			    a12R = tile[(unsigned int)( (y + 0)  * tile_width * 3 + (x * 3 + 3))]; 						   
			    a21R = tile[(unsigned int)( (y + 1)  * tile_width * 3 + (x * 3 + 0))]; 
			    a22R = tile[(unsigned int)( (y + 1)  * tile_width * 3 + (x * 3 + 3))]; 
				                                                           
				a11G = tile[(unsigned int)( (y + 0)  * tile_width * 3 + (x * 3 + 1))]; 
			    a12G = tile[(unsigned int)( (y + 0)  * tile_width * 3 + (x * 3 + 4))]; 						   
			    a21G = tile[(unsigned int)( (y + 1)  * tile_width * 3 + (x * 3 + 1))]; 
			    a22G = tile[(unsigned int)( (y + 1)  * tile_width * 3 + (x * 3 + 4))]; 
										   				    		  		 				  		  				
			    a11B = tile[(unsigned int)( (y + 0)  * tile_width * 3 + (x * 3 + 2))]; 
			    a12B = tile[(unsigned int)( (y + 0)  * tile_width * 3 + (x * 3 + 5))]; 						   
			    a21B = tile[(unsigned int)( (y + 1)  * tile_width * 3 + (x * 3 + 2))]; 
			    a22B = tile[(unsigned int)( (y + 1)  * tile_width * 3 + (x * 3 + 5))]; 																														
			

			
			tx = x_sample;
			tx = tx - floor(tx);
			ty = y_sample;
			ty = ty - floor(ty);



				b1R = (1-tx) * a11R + tx * a12R;
				b2R = (1-tx) * a21R + tx * a22R;
				
				b1G = (1-tx) * a11G + tx * a12G;
				b2G = (1-tx) * a21G + tx * a22G;

				b1B = (1-tx) * a11B + tx * a12B;
				b2B = (1-tx) * a21B + tx * a22B;


				out_tile[i*16*3 + 3*j + 0] = (1-ty) * b1R + ty * b2R;
				out_tile[i*16*3 + 3*j + 1] = (1-ty) * b1G + ty * b2G;
				out_tile[i*16*3 + 3*j + 2] = (1-ty) * b1B + ty * b2B;
				

		}
        } 
		return;
}
