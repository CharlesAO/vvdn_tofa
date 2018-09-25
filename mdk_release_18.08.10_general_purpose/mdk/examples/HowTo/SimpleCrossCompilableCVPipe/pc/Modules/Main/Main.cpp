///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  This is the main file of the app disparity map computation
///


#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


//---------------------------------------------------------------------------//
//------------------------------- INCLUDES ----------------------------------//
//---------------------------------------------------------------------------//

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string>

// Local code
#include <SimpleCrossCompilableCVPipe.h>
#include <Buffers.h>

using namespace std;

//---------------------------------------------------------------------------//
//------------------------------- Functions ---------------------------------//
//---------------------------------------------------------------------------//


// Main function
int main(int argc, char* argv[])
{
    //--------------------------- Local Vars --------------------------------//
	FILE* inputImage  = fopen("../../input/DunLoghaire_320x240.yuv", "rb");
	FILE* outputImage = fopen("output_320x240_P400.yuv", "wb");
    
	const int width = MAX_WIDTH;
	const int height = MAX_HEIGHT;

	unsigned char* inputBuffer  = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
	unsigned char* outputBuffer = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    
    unsigned int bytesLoaded = fread(inputBuffer, 1, width * height, inputImage);  	
    if(bytesLoaded!=width * height) {
        if(feof(inputImage)){
	    printf("End of file!\n");
	}
	if(ferror(inputImage)){
	    printf("Error reading from file.\n");
	}
	printf("Could not read from stream enough bytes.\n");
        printf("Expected %u, loaded %u bytes\n", width*height, bytesLoaded);
        exit(1);
    }
    fclose(inputImage);

    ///////////////////////////////////////////////////////////////////////////
    //---------------------- Compute DOG Algorythm --------------------------//
    ///////////////////////////////////////////////////////////////////////////
   
    // Init buffers
    SimpleCrossCompilableCVPipeInit(width);

    // Run algo
    SimpleCrossCompilableCVPipeRun(inputBuffer, outputBuffer, width, height);

    // Save results
    fwrite(outputBuffer, 1, sizeof(unsigned char) * width * height, outputImage);
	fclose(outputImage);

	// Free alocated buffers
    free(inputBuffer);
	free(outputBuffer);

    return 0;
}

