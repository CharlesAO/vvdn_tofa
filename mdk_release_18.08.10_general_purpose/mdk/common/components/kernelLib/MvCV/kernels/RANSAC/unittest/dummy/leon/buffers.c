#include "RANSAC.h"

#include <stdio.h>
#include <string.h>

#ifndef PC
#include <swcLeonUtils.h>
#endif


#include <Sections.h>


#define printf(...)

#ifndef PC

extern RANSAC_output dummy0_RANSAC_Outputs;
extern RANSAC_input dummy0_RANSAC_Inputs;


//Inputs
DDR_DATA float p1[2*MAX_NUMPOINTS];
DDR_DATA float p2[2*MAX_NUMPOINTS];
DDR_DATA struct ransac_params Params;

//Outputs
DDR_DATA int Inliers[MAX_NUMPOINTS];
DDR_DATA float fm[9];
DDR_DATA struct fundamental_debug Debug;

UNIT_TEST void RANSAC_Init()
{

    dummy0_RANSAC_Inputs.p1 = &p1[0];
    dummy0_RANSAC_Inputs.p2 = &p2[0];
    dummy0_RANSAC_Inputs.Params = &Params;

    dummy0_RANSAC_Outputs.fm = &fm[0];
    dummy0_RANSAC_Outputs.inliers = &Inliers[0];
    dummy0_RANSAC_Outputs.Debug = &Debug;
}

#endif
