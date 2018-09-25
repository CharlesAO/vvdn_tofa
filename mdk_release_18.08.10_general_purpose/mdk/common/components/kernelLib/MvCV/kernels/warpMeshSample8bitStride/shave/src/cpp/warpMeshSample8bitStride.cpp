#include "warpMeshSample8bitStride.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void mvcvWarpMeshSample8bitStride( u8 **tile, u8 *out_tile, half *mx, half *my, u32 out_stride, u32 tile_height)
{
    int i, j;
    unsigned int mesh_gran_x = 16, mesh_gran_y = 16;
    half x_sample,y_sample,x,y ;
    half a11, a12, a21, a22;
    half tx, ty, b1, b2;

    tile_height+=1;
    UNUSED(tile_height); // avoid 'unused variable' error from scan build
    for (i = 0; i< (int)mesh_gran_y; i++)
    { 
        for (j = 0; j< (int)mesh_gran_x; j++)
        {
            x_sample = mx[i * 16 +j];
            y_sample = my[i * 16 +j];


            x = floor(x_sample);
            y = floor(y_sample);


                a11 = tile[(unsigned int) (y + 0)][(unsigned int) (x + 0)];
                a12 = tile[(unsigned int) (y + 0)][(unsigned int) (x + 1)];
                a21 = tile[(unsigned int) (y + 1)][(unsigned int) (x + 0)];
                a22 = tile[(unsigned int) (y + 1)][(unsigned int) (x + 1)];

            //printf("%x\n", tile[(unsigned int)y]);
            //if(j == 15) exit(0);
            tx = x_sample;
            tx = tx - floor(tx);
            ty = y_sample;
            ty = ty - floor(ty);



                b1 = (1-tx) * a11 + tx * a12;
                b2 = (1-tx) * a21 + tx * a22;



                out_tile[i*out_stride + j] = (1-ty) * b1 + ty * b2;


        }
     }
        return;
}
