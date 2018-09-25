#include "harrisResponse.h"

fp32 mvcvHarrisResponse(u8 *data, u32 x, u32 y, u32 step_width, fp32 k)
{
	#define RADIUS  (3) // asm, for speed reason suport jus radius 3
	s32 dx;
	s32 dy;

	float xx = 0;
	float xy = 0;
	float yy = 0;

	// Skip border and move the pointer to the first pixel
	data += step_width + 1;

	for (u32 r = y - RADIUS; r < y + RADIUS; r++)
	{
		for (u32 c = x - RADIUS; c < x + RADIUS; c++)
		{
			s32 index = r * step_width + c;
			s32 firstPatchXIndex = (index > 1) ?index - 1 : 0;
			s32 firstPatchYIndex = (index > (s32)step_width) ? index - step_width : 0;
			UNUSED(firstPatchXIndex);
			UNUSED(firstPatchYIndex);
			dx = data[index - 1] - data[index + 1];
			dy = data[index - (s32)step_width] - data[index + (s32)step_width];
            //printf ("%x - %x\n", dx, dy);
			xx += dx * dx;
			xy += dx * dy;
			yy += dy * dy;
		}
	}

	float det = xx * yy - xy * xy;
	float trace = xx + yy;

	//k changes the response of edges.
	//seems sensitive to window size, line thickness, and image blur =o/
	return (det - k * trace * trace);

}
