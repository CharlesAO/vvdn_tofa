#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (10000)

TestRunner testRunnerhuffman_encode(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int huffman_encodeCycleCount;

void huffman_encode_asm_test(unsigned char* haclen, unsigned short* hacbit,  unsigned char* hdclen,  
                    unsigned short* hdcbit,  unsigned short* qtable,  short dc,  short* data,
                    unsigned char* jpgbuff,  unsigned int* jpgn,  unsigned int* buf,  unsigned int* n, unsigned int shave_buff_size)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();

    testRunnerhuffman_encode.Init();
    testRunnerhuffman_encode.SetInput("haclen", (unsigned char*)haclen, 16 * 12 * sizeof(char), SHAVE0);
    testRunnerhuffman_encode.SetInput("hacbit", (unsigned char*)hacbit, 16 * 12 * sizeof(short), SHAVE0);
    testRunnerhuffman_encode.SetInput("hdclen", (unsigned char*)hdclen, 12 * sizeof(char), SHAVE0);
    testRunnerhuffman_encode.SetInput("hdcbit", (unsigned char*)hdcbit, 12 * sizeof(short), SHAVE0);
    testRunnerhuffman_encode.SetInput("qtable", (unsigned short *)qtable, 8 * 8, SHAVE0);
    testRunnerhuffman_encode.SetInput("data",   (unsigned char*)data, 8 * 8 * sizeof(short), SHAVE0);
    testRunnerhuffman_encode.SetInput("jpgbuff",   (unsigned char*)jpgbuff, shave_buff_size, SHAVE0);
    
    testRunnerhuffman_encode.SetInput("jpgn", *jpgn, SHAVE0);
    testRunnerhuffman_encode.SetInput("buf", *buf, SHAVE0);
    testRunnerhuffman_encode.SetInput("n", *n, SHAVE0);
    
    testRunnerhuffman_encode.SetInput("dc", dc, SHAVE0);
    testRunnerhuffman_encode.SetInput("shave_buff_size", (unsigned int)shave_buff_size, SHAVE0);
    
    testRunnerhuffman_encode.Run(SHAVE0);
    
    {
        huffman_encodeCycleCount = testRunnerhuffman_encode.GetVariableValue("cycleCount");
        functionInfo.AddCyclePerPixelInfo((float)(huffman_encodeCycleCount - 2));
        functionInfo.setExpectedCycles((float)EXPECTED_CC);
    }

    testRunnerhuffman_encode.GetOutput("jpgn", SHAVE0, jpgn);
    testRunnerhuffman_encode.GetOutput("buf", SHAVE0, buf);
    testRunnerhuffman_encode.GetOutput("n", SHAVE0, n);
    testRunnerhuffman_encode.GetOutput("jpgbuff", SHAVE0, shave_buff_size, (unsigned char*)jpgbuff);
}

