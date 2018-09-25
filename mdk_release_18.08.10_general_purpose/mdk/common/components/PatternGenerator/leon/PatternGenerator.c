///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Pattern Generator.
///

// 1: Includes
// ----------------------------------------------------------------------------

#include "PatternGeneratorApi.h"
#include "stdio.h"
#include "assert.h"


// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// must be even because YUV422. we write 2 pixel in the same time in memory
#define BORDER 4

// threshold for pattern checker
#define THRESHOLD_Y     45
#define THRESHOLD_U     22
#define THRESHOLD_V     18
#ifdef MA2X8X
#define RGB     1
#define BARS    16
#define COLORS  16
#define STRIPES 63
#else
#define COLORS  8
#define BARS    7
#define RGB     0
#define STRIPES 64
#endif

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
unsigned int patternWIDTH = 0;
unsigned int patternHEIGHT = 0;
unsigned int k, type=0;
char *patternYuvPlane;
uint32_t *patternRgbPlane;
PatternRGBColor defaultColor[] = { {255, 255, 255},
                                   {255, 255,   0},
                                   {  0, 200, 255},
                                   {  0, 128,   0},
                                   {255,   0, 255},
                                   {255,   0,   0},
                                   {  0,   0, 255},
                                   {  0,   0,   0}} ;
uint32_t grayPatternRGB[] = {
    0x00000000, 0x00111111, 0x00222222, 0x00333333,
    0x00444444, 0x00555555, 0x00666666, 0x00777777,
    0x00888888, 0x00999999, 0x00AAAAAA, 0x00BBBBBB,
    0x00CCCCCC, 0x00DDDDDD, 0x00EEEEEE, 0x00FFFFFF
};
uint32_t colorPatternRGB[] = {
    0x00E50500, 0x00D50F0C, 0x00C61A19, 0x00B72526,
    0x00a73032, 0x00983B3F, 0x0089454C, 0x007A5059,
    0x006A5B65, 0x005B6672, 0x004C717F, 0x003D7B8C,
    0x002D8698, 0x001E91A5, 0x000F9CB2, 0x0000A7BF
};
uint32_t colorStripesRGB[] = {0x00E51C00, 0x00E42700, 0x00E33200, 0x00E33D00, 0x00E24700, 0x00E15200, 0x00E15C00, 0x00E06700,
                              0x00E07100, 0x00DF7C00, 0x00DE8600, 0x00DE9000, 0x00DD9A00, 0x00DDA400, 0x00DCAE00, 0x00DBB800,
                              0x00DBC200, 0x00DACC00, 0x00DAD600, 0x00D2D900, 0x00C7D800, 0x00BDD800, 0x00B2D700, 0x00A7D700,
                              0x009CD600, 0x0092D500, 0x0087D500, 0x007DD400, 0x0072D400, 0x0068D300, 0x005DD200, 0x0053D200,
                              0x0049D100, 0x003FD100, 0x0035D000, 0x002BCF00, 0x0021CF00, 0x0017CE00, 0x000DCE00, 0x0003CD00,
                              0x0000CC06, 0x0000CC10, 0x0000CB19, 0x0000CB23, 0x0000CA2C, 0x0000C936, 0x0000C93F, 0x0000C849,
                              0x0000C852, 0x0000C75B, 0x0000C664, 0x0000C66D, 0x0000C576, 0x0000C57F, 0x0000C488, 0x0000C391,
                              0x0000C39A, 0x0000C2A3, 0x0000C2AC, 0x0000C1B4, 0x0000C0BD, 0x0000BAC0, 0x0000B0BF, 0x0000A6BF
};

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

// RGB -> YUV convertor
void converterRGBtoYUV(PatternRGBColor colorRGB, PatternYUVColor *colorYUV)
{
    (*colorYUV).Y = (( 66 * colorRGB.R + 129 * colorRGB.G +  25 * colorRGB.B + 128) >> 8) +  16;
    (*colorYUV).U = ((-38 * colorRGB.R -  74 * colorRGB.G + 112 * colorRGB.B + 128) >> 8) + 128;
    (*colorYUV).V = ((112 * colorRGB.R -  94 * colorRGB.G -  18 * colorRGB.B + 128) >> 8) + 128;
}


// Validate coordinates to avoid memory illegal accesses
void validateCoords(PatternPoint *point)
{
    if (point->x > patternWIDTH)
    {
        point->x = patternWIDTH - 1;
    }

    if (point->y > patternHEIGHT)
    {
        point->y = patternHEIGHT - 1;
    }

}


// This function draws a rectangle specifing topLeft and bottomRight positions in the frame
void drawColorRect(PatternPoint topLeftCorner, PatternPoint bottomRightCorner, PatternYUVColor color)
{
    unsigned int i,j;
    validateCoords(&topLeftCorner);
    validateCoords(&bottomRightCorner);
    if(RGB)
    {
         for (i = topLeftCorner.y; i < bottomRightCorner.y; i++)
        {
            for (j = (topLeftCorner.x ); j < (bottomRightCorner.x ); j++)
            {
               switch(type)
               {
                case 1:
                    patternRgbPlane[i * patternWIDTH + j]=grayPatternRGB[k];
                    break;
                case 2:
                    patternRgbPlane[i * patternWIDTH + j]=colorPatternRGB[k];
                    break;
                case 3:
                    patternRgbPlane[i * patternWIDTH + j]=colorStripesRGB[k];
                    break;
                default:
                    assert(0);
                }
            }
        }
    }
    else
    {
        for (i = topLeftCorner.y; i < bottomRightCorner.y; i++)
        {
            for (j = (topLeftCorner.x / 2); j < (bottomRightCorner.x / 2); j++)
            {

                patternYuvPlane[2 * i * patternWIDTH + 4 * j] = color.Y;
                patternYuvPlane[2 * i * patternWIDTH + 4 * j + 1] = color.U;
                patternYuvPlane[2 * i * patternWIDTH + 4 * j + 2] = color.Y;
                patternYuvPlane[2 * i * patternWIDTH + 4 * j + 3] = color.V;

            }
        }

    }
}




// Generate the horizontal pattern
unsigned int CreateHorizontalColorBars (frameBuffer *frame, unsigned int interlaced)
{
    // declare
    PatternYUVColor myColorYUV;
    PatternPoint myTopLeft, myBottomRight;
    unsigned int w, lineWidth;

    // init
    patternWIDTH = frame->spec.width;
    patternHEIGHT = frame->spec.height;
    myTopLeft.x = 0;
    myTopLeft.y = 0;
    myBottomRight.x = frame->spec.width;
    myBottomRight.y = frame->spec.height;
    if(RGB)
    {
        type=2;
        patternRgbPlane= (uint32_t *)frame->p1;
    }
    else
    {
        patternYuvPlane = (char *)frame->p1;
    }

   // add color line
    lineWidth = (patternHEIGHT - 2 * BORDER) / BARS;
    myTopLeft.x = BORDER;
    myTopLeft.y = BORDER;
    myBottomRight.x = patternWIDTH - BORDER;
    myBottomRight.y = BORDER + lineWidth;

    for (k = 0; k < COLORS; k++)
    {
        converterRGBtoYUV(defaultColor[k], &myColorYUV);
        drawColorRect(myTopLeft, myBottomRight, myColorYUV);
        myTopLeft.y += lineWidth;
        myBottomRight.y += lineWidth;
    }
    converterRGBtoYUV(defaultColor[7], &myColorYUV);
    myBottomRight.y = patternHEIGHT - BORDER;
    drawColorRect(myTopLeft, myBottomRight, myColorYUV);

    // check if the frame is interlaced
    if (interlaced == 1)
    {
        converterRGBtoYUV(defaultColor[7], &myColorYUV);
        for (k = BORDER; k < (patternHEIGHT - BORDER); k++)
            for (w = 0; w < (BORDER / 2); w++)
                if ((k % 2) == 0)
                {
                    // Left Border
                    patternYuvPlane[2* k * patternWIDTH + 4 * w] = myColorYUV.Y;
                    patternYuvPlane[2* k * patternWIDTH + 4 * w + 1] = myColorYUV.U;
                    patternYuvPlane[2* k * patternWIDTH + 4 * w + 2] = myColorYUV.Y;
                    patternYuvPlane[2* k * patternWIDTH + 4 * w + 3] = myColorYUV.V;
                    // Right Border
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 4] = myColorYUV.Y;
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 3] = myColorYUV.U;
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 2] = myColorYUV.Y;
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 1] = myColorYUV.V;
                }
    }

    return 1;
}


// Generate the vertical pattern
unsigned int CreateVerticalColorBars (frameBuffer *frame, unsigned int interlaced)
{
    // declare
    PatternYUVColor myColorYUV;
    PatternPoint myTopLeft, myBottomRight;
    unsigned int w, lineWidth;

    // init
    patternWIDTH = frame->spec.width;
    patternHEIGHT = frame->spec.height;
    myTopLeft.x = 0;
    myTopLeft.y = 0;
    myBottomRight.x = frame->spec.width;
    myBottomRight.y = frame->spec.height;
    if(RGB)
    {   type=2;
        patternRgbPlane= (uint32_t *)frame->p1;
    }
    else
    {
        patternYuvPlane = (char *)frame->p1;
    }

    // all image white
    converterRGBtoYUV(defaultColor[0], &myColorYUV);
    drawColorRect(myTopLeft, myBottomRight, myColorYUV);

    // add color line
    lineWidth = (patternWIDTH - 2 * BORDER) / BARS;
    myTopLeft.x = BORDER;
    myTopLeft.y = BORDER;
    myBottomRight.x = BORDER + lineWidth;
    myBottomRight.y = patternHEIGHT - BORDER;

    for (k = 0; k < COLORS; k++)
    {
        converterRGBtoYUV(defaultColor[k], &myColorYUV);
        drawColorRect(myTopLeft, myBottomRight, myColorYUV);
        myTopLeft.x += lineWidth;
        myBottomRight.x += lineWidth;
    }
    converterRGBtoYUV(defaultColor[7], &myColorYUV);
    myBottomRight.x = frame->spec.width - BORDER;
    drawColorRect(myTopLeft, myBottomRight, myColorYUV);

    // check if the frame is interlaced
    if (interlaced == 1)
    {
        converterRGBtoYUV(defaultColor[7], &myColorYUV);
        for (k = BORDER; k < (frame->spec.height - BORDER); k++)
            for (w = 0; w < (BORDER / 2); w++)
                if ((k % 2) == 0)
                {
                    // Left Border
                    patternYuvPlane[2 * k * patternWIDTH + 4 * w] = myColorYUV.Y;
                    patternYuvPlane[2 * k * patternWIDTH + 4 * w + 1] = myColorYUV.U;
                    patternYuvPlane[2 * k * patternWIDTH + 4 * w + 2] = myColorYUV.Y;
                    patternYuvPlane[2 * k * patternWIDTH + 4 * w + 3] = myColorYUV.V;
                    // Right Border
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 4] = myColorYUV.Y;
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 3] = myColorYUV.U;
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 2] = myColorYUV.Y;
                    patternYuvPlane[2 * (k + 1) * patternWIDTH - 4 * w - 1] = myColorYUV.V;
                }
    }

    return 1;
}


// Custom color stripes pattern
unsigned int CreateColorStripesPattern (frameBuffer *frame)
{
    // declare
    PatternRGBColor leftColor, rightColor;
    PatternYUVColor myLeftColorYUV, myRightColorYUV;
    PatternPoint myTopLeft, myBottomRight;
    unsigned int stripeWidth;

    // init
    patternWIDTH = frame->spec.width;
    patternHEIGHT = frame->spec.height;
    myTopLeft.x = 0;
    myTopLeft.y = 0;
    myBottomRight.x = frame->spec.width;
    myBottomRight.y = frame->spec.height;
    if(RGB)
    {
        type=3;
        patternRgbPlane= (uint32_t *)frame->p1;
    }
    else
    {
        patternYuvPlane = (char *)frame->p1;
    }

    // all image white
    stripeWidth = frame->spec.height / STRIPES;
    converterRGBtoYUV(defaultColor[0], &myLeftColorYUV);
    drawColorRect(myTopLeft, myBottomRight, myLeftColorYUV);

    // add color line
    myBottomRight.x = patternWIDTH / 2;
    myBottomRight.y = stripeWidth;

    for (k = 0;  k < 64; k++)
    {
        leftColor.R = ((k >> 0) & 0x1) * 255;
        leftColor.G = ((k >> 1) & 0x1) * 255;
        leftColor.B = ((k >> 2) & 0x1) * 255;
        converterRGBtoYUV(leftColor, &myLeftColorYUV);
        drawColorRect(myTopLeft, myBottomRight, myLeftColorYUV);

        myTopLeft.x = frame->spec.width / 2;
        myBottomRight.x = frame->spec.width;
        rightColor.R = ((k >> 3) & 0x1) * 255;
        rightColor.G = ((k >> 4) & 0x1) * 255;
        rightColor.B = ((k >> 5) & 0x1) * 255;
        converterRGBtoYUV(rightColor, &myRightColorYUV);
        drawColorRect(myTopLeft, myBottomRight, myRightColorYUV);

        myTopLeft.x = 0;
        myTopLeft.y += stripeWidth;
        myBottomRight.y += stripeWidth;
    }

    return 1;
}


// Grey gradient pattern
unsigned int CreateLinearGreyPattern (frameBuffer *frame)
{
     // declare
    PatternYUVColor myColorYUV;
    PatternRGBColor myColorRGB;
    PatternPoint myTopLeft, myBottomRight;
    unsigned int lineWidth;

    // init
    patternWIDTH = frame->spec.width;
    patternHEIGHT = frame->spec.height;
    myTopLeft.x = 0;
    myTopLeft.y = 0;
    myBottomRight.x = frame->spec.width;
    myBottomRight.y = frame->spec.height;
    if(RGB)
    {   type=1;
        patternRgbPlane= (uint32_t *)frame->p1;
    }
    else
    {
        patternYuvPlane = (char *)frame->p1;
    }

     // all image white
    converterRGBtoYUV(defaultColor[0], &myColorYUV);
    drawColorRect(myTopLeft, myBottomRight, myColorYUV);

    // add color line
    lineWidth = patternWIDTH / 16;
    myBottomRight.x = lineWidth;
    myBottomRight.y = patternHEIGHT;

    for (k = 0; k < 16; k++)
    {
        myColorRGB.R = k * 15;
        myColorRGB.G = k * 15;
        myColorRGB.B = k * 15;
        converterRGBtoYUV(myColorRGB, &myColorYUV);
        drawColorRect(myTopLeft, myBottomRight, myColorYUV);
        myTopLeft.x += lineWidth;
        if (k == 15)
            myBottomRight.x = frame->spec.width;
        else
            myBottomRight.x += lineWidth;
    }

    return 1;
}



// Check the pattern
unsigned int PatternCheck (frameBuffer *inputFrame, frameBuffer *outputFrame, unsigned int interlaced)
{
    printf("Start check pattern !\n");

    // declare
    unsigned int resultOfCheck = 1;
    unsigned int i, j;
    char *inputPattern, *outputPattern;
    char message[255] = "";
    unsigned int testLineOffset = 0;

    // set offset for interlaced resolution
    if (interlaced == 1)
    {
        testLineOffset = 2;
    }

    // test Vic
    inputPattern  = (char *)inputFrame->p1;
    outputPattern = (char *)outputFrame->p1;


    // test pixel by pixel
    for (j = testLineOffset; j < (outputFrame->spec.height - testLineOffset); j++)
    {
        for (i = 0; i < (2 * outputFrame->spec.width); i += 8) /* one line contain 2*width char */
        {
            //CheckPatternPrintf ("%c - %c\n", inputPattern[j*2*outputFrame->spec.width + i], outputPattern[j*2*outputFrame->spec.width + i]);
            //CheckPatternPrintf ("%c - %c\n", inputPattern[j*2*outputFrame->spec.width + i+1], outputPattern[j*2*outputFrame->spec.width + i+1]);

            if ((inputPattern[j * 2 * outputFrame->spec.width + i] > (outputPattern[j * 2 * outputFrame->spec.width + i] + THRESHOLD_Y)) ||
                    (inputPattern[j * 2 * outputFrame->spec.width + i] < (outputPattern[j * 2 * outputFrame->spec.width + i] - THRESHOLD_Y)))
            {
                sprintf(message, "Diffrent pixel (%d , %d)", i, j);
                printf("%s \n", message);
                resultOfCheck = 0;
                break;
            }
            if ((inputPattern[j * 2 * outputFrame->spec.width + i + 1] > (outputPattern[j * 2 * outputFrame->spec.width + i + 1] + THRESHOLD_U)) ||
                    (inputPattern[j * 2 * outputFrame->spec.width + i + 1] < (outputPattern[j * 2 * outputFrame->spec.width + i + 1] - THRESHOLD_U)))
            {
                sprintf(message, "Diffrent pixel (%d , %d)", i, j);
                printf("%s \n", message);
                resultOfCheck = 0;
                break;
            }
            if ((inputPattern[j * 2 * outputFrame->spec.width + i + 3] > (outputPattern[j * 2 * outputFrame->spec.width + i + 3] + THRESHOLD_V)) ||
                    (inputPattern[j * 2 * outputFrame->spec.width + i + 3] < (outputPattern[j * 2 * outputFrame->spec.width + i + 3] - THRESHOLD_V)))
            {
                sprintf(message, "Diffrent pixel (%d , %d)", i, j);
                printf("%s \n", message);
                resultOfCheck = 0;
                break;
            }
        }

    }

    if (resultOfCheck == 0)
    {
        printf("Pattern Check Mismatch !\n");
    }
    else
    {
        printf("Pattern Checked !\n");
    }

    printf("Finish check pattern !\n\n");
    return resultOfCheck;
}

