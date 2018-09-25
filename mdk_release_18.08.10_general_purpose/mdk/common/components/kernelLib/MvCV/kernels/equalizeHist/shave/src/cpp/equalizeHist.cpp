#include "equalizeHist.h"

/// equalizehistogram kernel - makes an equalization trough an image with a given histogram
/// @param[in] in         - array of pointers to input lines
/// @param[out]out        - array of pointers to output lines
/// @param[in] hist       - pointer to an input array that indicates the cumulative histogram of the image
/// @param[in] width      - width of input line
void mvcvEqualizeHist(u8** in, u8** out, u32 *hist, u32 width)
{
	u8 *in_line;
	u8 *out_line;
	u32 i;

	in_line   = *in;
	out_line  = *out;


	for(i=0;i<width;i++)
	{
		out_line[i]= (u8)((fp32)hist[in_line[i]] * ((fp32)255/(fp32)hist[255]));
	}

    return;
}