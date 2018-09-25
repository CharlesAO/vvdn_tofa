#include <channelExtract.h>

/// integral image kernel - this kernel makes the sum of all pixels before it and on the left of it's column  ( this particular case makes square sum of pixels in f32 format)
/// @param[in] in         - array of pointers to input lines      
/// @param[out]out        - array of pointers for output lines    
/// @param[in] width      - width of input line
/// @param[in] plane      - number 0 to extract plane R, 1 for extracting G, 2 for extracting B
void mvcvChannelExtract(u8** in, u8** out, u32 width, u32 plane)
{
	u8 *in_line;
	u8 *out_line;
	u32 i, c=0;
	in_line  = *in;
	out_line = *out;

	for (i=plane;i<width;i=i+3)
	{
		out_line[c]=in_line[i];
		c++;
	}
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvChannelExtract_opt(uint8_t ** in, uint8_t ** out, uint32_t width, uint32_t plane)
{
uchar8 *out_line = *(uchar8**)out;

	// re-allign input data to plane pointer (uses unaligned vector loads)
	uchar8 *in_line = (uchar8*)(&in[0][plane]);
	width -= plane;

	// Do nothing when plane is invalid
	if (plane > 2) return;

	uchar8 a1, b1, c1;
	uchar8 a2, b2, c2;
	UNUSED(a2);
	UNUSED(b2);
	UNUSED(c2);
	a1 = *in_line++;
	b1 = *in_line++;
	c1 = *in_line++;
	width -= 24;

	// Same idea as above but with a cleanup loop in case we want to handle odd-size inputs
	// Vectorized part requires remaining width >= 24
	while (width >= 24) {
		//    for (i = 0; i < (width-23); i+=3*8) {
		

		// in:  < a0, a1, a2, a3, a4, a5, a6, a7, b0, b1, b2, b3, b4, b5, b6, b7, c0, c1, c2, c3, c4, c5, c6, c7 >
		// out: < o0, --, --, o1, --, --, o2, --, --, o3, --, --, o4, --, --, o5, --, --, o6, --, --, o7, --, -- >
		uchar8 dest1 = (uchar8){ a1[0], a1[3], a1[6], b1[1], b1[4], b1[7], c1[2], c1[5] };
		*out_line++ = dest1;
	
		a1 = *in_line++;
		b1 = *in_line++;
		c1 = *in_line++;
		width -= 24;

	}

	uchar8 dest1 = (uchar8){ a1[0], a1[3], a1[6], b1[1], b1[4], b1[7], c1[2], c1[5] };
	*out_line++ = dest1;
	
	if (width) {
		// Finish leftover bit if needed
		u8 *in_line_remainder = (u8*)in_line;
		u8 *out_line_remainder = (u8*)out_line;
		u32 i;

		for (i = 0; i < width; i += 3) {
			*out_line_remainder++ = *in_line_remainder;
			in_line_remainder += 3;
		}
	}
	return;
}
#endif //MOVICOMPILE_OPTIMIZED
