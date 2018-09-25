#include "monoImbalance.h"

void mvcvMonoImbalance(u16** in, half** out,int thrDark, int thrBright, int inputBits, u32 inWidth)

{
    int avg, luma, diff,thr;
	int i;
	int rshift = inputBits + 1;
    u16* lines[3];
	float  result;
    float  normVal= 1.0f/((1<<inputBits)-1);
	

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

	
	//Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
		
		 /* Average 4-connected neighbours */
        avg = (lines[0][i] + lines[2][i] + lines[1][i-1] + lines[1][i+1]) >> 2;

        /* Compute Luma*2 */
        luma = avg + lines[1][i];

        /* Compute luma-adapted threshold and re-normalize to input pixel range */
        thr = thrDark + (((thrBright - thrDark) * luma) >> rshift);

        diff = avg - lines[1][i];

        /* Filter the difference between current pixel and neighbours */
        if (diff > thr) {
            diff = 2*thr - diff;
            if (diff < 0) {
                diff = 0;
            }
        } else if (diff < -thr) {
            diff = -2*thr - diff;
            if (diff > 0) {
                diff = 0;
            }
        }

        /* Add half of modified difference to input */
        result = (float)lines[1][i] + .5 + diff/2;

        /* Normalize to range [0, 1.0] */
        out[0][i] = result * normVal;
    }

    return;
}
