#include "dilate.h"

// Dilate: The function dilates the source image using the specified structuring element.
// This is determined by the shape of a pixel neighborhood over which the maximum is taken.

/// Dilate kernel
/// @param[in]  src      - array of pointers to input lines of the input image
/// @param[out] dst      - array of pointers to output lines
/// @param[in]  kernel   - array of pointers to input kernel
/// @param[in]  width    - width  of the input line
/// @param[in]  height   - height of the input line
/// @param[in]  k        - kernel size
void mvcvDilate(u8** src, u8** dst, u8** kernel, u32 width, u32 height, u32 k)
{
	int j, i1, j1;
	u8 max = 0;
	u8* row[15];
    height+=1;
	for(j = 0;j < (int)k; j++)
			row[j] = src[j];

	// Determine the anchor positions which must be replaces by a maximum value
	for ( j = 0; j < (int)width; j++)
	{
		max = 0;
		// Determine the maximum number into a k*k square
		for (i1 = 0; i1 < (int)k; i1++ )
		{
			for (j1 = 0; j1 < (int)k; j1++)
			{
				if (kernel[i1][j1] && (max < row[i1][j1 + j]))
				{
						max = row[i1][j1 + j];

				}
			}
		}

		dst[0][j] = max;
	}

}