#include "warpMeshSample8bit.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void mvcvWarpMeshSample8bit( u8 **tile, u8 *out_tile, half *mx, half *my, u32 tile_width, u32 tile_height)
{

	int i, j;
	unsigned int mesh_gran_x = 16, mesh_gran_y = 16;
	half x_sample,y_sample,x,y ;
	half a11, a12, a21, a22;
	half tx, ty, b1, b2;

    tile_width+=1;
    tile_height+=1;
    UNUSED(tile_width); // avoid 'unused variable' error from scan build
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



				out_tile[i*16 + j] = (1-ty) * b1 + ty * b2;


		}
        }  
		return;
}


#ifdef MOVICOMPILE_OPTIMIZED
// ////////////////////////////////////////////////////////////////////////////////////////////////////
// moviCompileTeam optimized implementation of the kernel
// ////////////////////////////////////////////////////////////////////////////////////////////////////

#define populate_AII(n) usTemp1 = *(unsigned short *) (&tile[s8MY.s##n][s8MX.s##n]); \
                        A11.s##n = usTemp1&0XFF;						\
                        A12.s##n = usTemp1 >>8;				\
                        usTemp2 = *(unsigned short *) (&tile[s8MY.s##n+1][s8MX.s##n]); \
                        A21.s##n = usTemp2 &0XFF;			\
                        A22.s##n = usTemp2 >>8;

// This pattern should take us closer to 1X once the ld.u8.f16 starts working
#if 0
#define populate_AII(n) uc8Tmp = (  mvuConvert_half8(*(uchar8 *)(&tile[s8MY.s##n  ][s8MX.s##n  ]))); \
                        A11.s##n = uc8Tmp.s0; \
                        A12.s##n = uc8Tmp.s1; \
                        A21.s##n = tile[s8MY.s##n+1][s8MX.s##n  ];   \
                        A22.s##n = tile[s8MY.s##n+1][s8MX.s##n+1];
#endif

void mvcvWarpMeshSample8bit_opt( u8 ** __restrict__ tile, u8 * __restrict__ out_tile,
					     half * __restrict__ mx, half * __restrict__ my,
					     u32 tile_width, u32 tile_height) {
UNUSED(tile_width);
UNUSED(tile_height);
	int i,j;
        half8 * __restrict__ ph8MX, * __restrict__ ph8MY;
	half8 h8MX, h8MY;
	half8 h8FracX, h8FracY;

	short8 s8MX, s8MY;

	ph8MX = (half8 * __restrict__)mx;
	ph8MY = (half8 * __restrict__)my;

	uchar8 uc8OutputValue;
	half8 A11, A12, A21, A22;
	uchar8 *puc8OutputPtr = (uchar8 *)out_tile;
	unsigned short usTemp1, usTemp2;
	half8 uc8Tmp;
	UNUSED(uc8Tmp);
	for (i = 0; i< 16; i++)
#pragma clang loop unroll_count(2)
		for (j = 0; j< 2; j++)
		{
		  h8MX = *ph8MX;
		  s8MX = mvuConvert_short8 (h8MX);

		  h8MY = *ph8MY;
		  s8MY = mvuConvert_short8 (h8MY);

		  ph8MX++; ph8MY++;

		  h8FracX = h8MX - mvuConvert_half8(s8MX);
		  h8FracY = h8MY - mvuConvert_half8(s8MY);

		  half8 h8FracXComplement = 1-h8FracX;
		  half8 h8FracYComplement = 1-h8FracY;

		  populate_AII(0);
		  populate_AII(1);
		  populate_AII(2);
		  populate_AII(3);
		  populate_AII(4);
		  populate_AII(5);
		  populate_AII(6);
		  populate_AII(7);

		  uc8OutputValue = mvuConvert_uchar8(h8FracYComplement * (h8FracXComplement * A11 + h8FracX * A12) + h8FracY * ((h8FracXComplement * A21 + h8FracX * A22)));
		  *puc8OutputPtr++ = uc8OutputValue;
		}
		return;
}
#endif
