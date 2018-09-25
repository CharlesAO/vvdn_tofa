#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/padBayer5/padBayer5.h>

//##########################################################################################
void padBayer5Reference(uint8_t *inLine[13],
                        uint8_t *outLine,
                        uint32_t exeNo,
                        uint32_t sliceWidth, //slice width
                        uint32_t fullW,      //full width
                        uint32_t outH,       //full height
                        uint32_t svuF,       //first shave
                        uint32_t svuL)       //last shave
{
    uint8_t *curLine;
    uint32_t curPix;
    uint32_t curSvu = scGetShaveNumber();
    int      pix = 0;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Figure out which line we need to output (consider BAYER Vertical replication)
    static uint8_t topPadMagic[5] = {12, 10, 10, 8, 8};
    static uint8_t botPadMagic[5] = { 4,  4,  2, 2, 0};

    if      (exeNo <       5) { curLine = inLine[topPadMagic[exeNo]           ];} //top-padding case:
    else if (exeNo >= outH-5) { curLine = inLine[botPadMagic[exeNo - (outH-5)]];} //bottom-padding
    else                      { curLine = inLine[6];                            } //central case

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Horizontal adjustments:
    if(svuF == svuL) //single shave case
    {
        //I leave this as homework :)
    }
    else if(curSvu == svuF) //LEFT REPLICATION
    {
        //Bayer replication:
        outLine[pix++] = curLine[1];
        outLine[pix++] = curLine[0];
        outLine[pix++] = curLine[1];
        outLine[pix++] = curLine[0];
        outLine[pix++] = curLine[1];

        //Remaining pixels: via copy:
        while(pix<sliceWidth)
        {
            outLine[pix]= curLine[pix-5];
            pix++;
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Inner shave:
    else if((curSvu > svuF) & (curSvu < svuL))
    {
        for(pix=0; pix<sliceWidth; pix++)
            outLine[pix] = curLine[pix-5];
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Last Shave
    else
    {
        int lastSlcW = fullW - (svuL - svuF)*sliceWidth;
        int lastPix;

        for(pix=0; pix<lastSlcW-5; pix++)
            outLine[pix] = curLine[pix-5];
        lastPix = pix-1;

        outLine[pix++] = outLine[lastPix - 1];
        outLine[pix++] = outLine[lastPix    ];
        outLine[pix++] = outLine[lastPix - 1];
        outLine[pix++] = outLine[lastPix    ];
        outLine[pix++] = outLine[lastPix - 1];
    }
}

//##########################################################################################
//assumes single input plane, extend to more if needed...
void svuPadBayer5(SippFilter *fptr)
{
    uint8_t *output;
    uint8_t *inLine[13];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 lpi   = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo = fptr->exeNo * lpi;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        //Faster way of getting IO pointers
        output = (uint8_t*)getPlaneIoPtrs(fptr, 0/*parent*/, iterNum, 0/*plane*/, inLine);

        //Implementation:
        padBayer5Reference(inLine, output,   runNo,
                           fptr->sliceWidth, fptr->outputW,
                           fptr->outputH,    fptr->gi->sliceFirst, fptr->gi->sliceLast);
    }
}
