#include <sipp.h>
#include <sippBicubic.h>

typedef struct
{
    //UInt64 *xyAddr;
    UInt32 width;
    UInt32 height;
	UInt32 firstShave;
	UInt8 *input;
	UInt8 *output;
	UInt32 bpp;
	tBicubicReg cmd[12];
	UInt32 lastShave;
	UInt32 shaveCount;
}
StartBicubicParam;

//Shave symbols that need to be understood by leon need to be declared through "SVU_SYM" MACRO,
//as moviCompile adds a leading _ to symbol exported
void SVU_SYM(svuStartBicubic)(SippFilter *fptr);
SHAVE_SYM_EXPORT(svuStartBicubic);
