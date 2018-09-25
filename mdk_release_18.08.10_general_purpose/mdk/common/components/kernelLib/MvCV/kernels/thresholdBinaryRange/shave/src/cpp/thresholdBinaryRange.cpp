#include "thresholdBinaryRange.h"

/// thresholdBinaryRange kernel -this kernel set output to 0xFF if source(x,y) is in specified range, otherwise  output = 0.
/// @param[in] in                - array of pointers to input lines    
/// @param[out] out              - array of pointers for output lines  
/// @param[in] lowerValue        - lowerValue   
/// @param[in] upperValue        - upperValue   
/// @param[in] width             - width of input line
 void mvcvThresholdBinaryRange(u8** in, u8** out,  u8 lowerValue, u8 upperValue, u32 width)
{

    int i;
    for (i = 0; i < (int)width; i++)
    {
        if ((in[0][i] > lowerValue) && (in[0][i] < upperValue)){
            out[0][i] = 255;
        } 
		else {
			out[0][i] = 0;
		}
     }
    return;
}
