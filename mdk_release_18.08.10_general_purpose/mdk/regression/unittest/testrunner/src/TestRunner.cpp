#include <fstream>
#include "TestRunner.h"
#include "IOProcessing.h"
#include "FunctionInfo.h"
#include "half.h"
#include <cstdio>
#include "internalDebug.h"
//#include <helpers.h>

void saveFile(std::string fileName, char* buffer, unsigned int bytes)
{
    std::ofstream f;

    f.open(fileName.c_str(), std::ios::out | std::ios::binary);
    f.write(buffer, bytes);
    f.close();
}

bool TestRunner::debugMode = false;

#ifdef CVMA2480 // only for ma2480 we disable register checking at the moment
bool TestRunner::registersChecking = false;
#else
bool TestRunner::registersChecking = true;
#endif

bool TestRunner::defaultPort = true;
char TestRunner::srvIP[20] = "127.0.0.1";
char TestRunner::srvPort[10] = "30001";

TestRunner::TestRunner(std::string projectPath, std::string elfPath): ioProcessor(projectPath)
{
    elfFilePath = elfPath;
    outputAvailable = false;
    testInitialized = false;
}

TestRunner::TestRunner(std::string projectPath, std::string elfPath, unsigned int debugInterface): ioProcessor(projectPath, debugInterface)
{
    elfFilePath = elfPath;
    outputAvailable = false;
    testInitialized = false;
}

TestRunner::TestRunner(std::string projectPath, std::string elfPath, std::string kernelAsmLabel, unsigned int debugInterface): ioProcessor(projectPath, debugInterface)
{
    elfFilePath = elfPath;
    asmLabel = kernelAsmLabel;
    outputAvailable = false;
    testInitialized = false;
}

TestRunner::~TestRunner()
{
    // TODO Auto-generated destructor stub
}

void TestRunner::Init()
{
    #ifdef MV_MVDBG_GEN1
    if(!testInitialized)
    {
        testInitialized = true;
        ioProcessor.SetProjectPath(projectPath);
        ioProcessor.SetSrvIP(srvIP);
        ioProcessor.SetSrvPort(srvPort, defaultPort);
        ioProcessor.SetAsmLabel(asmLabel);
        ioProcessor.LoadElf(elfFilePath);
    }
    #endif
    #ifdef MV_MVDBG_GEN2
    if(!testInitialized)
    {
        testInitialized = true;
        ioProcessor.SetProjectPath(projectPath);
        ioProcessor.SetSrvIP(srvIP);
        ioProcessor.SetSrvPort(srvPort, defaultPort);
        ioProcessor.SetAsmLabel(asmLabel);
    }
        ioProcessor.LoadElf(elfFilePath);
    #endif


}


std::string TestRunner::getSymbolName(std::string name, std::string symbolPrefix, TargetCore target)
{
    char targetStr[3];

    if (target <= SHAVE11)
    {
        if(symbolPrefix.length() > 0)
        {
            sprintf(targetStr, "%d", target);
            std::string str = symbolPrefix + std::string(targetStr) + "_" + name;
            return symbolPrefix + std::string(targetStr) + "_" + name;
        }
    }

    return name;
}

void TestRunner::SetInput(std::string name, unsigned int value, TargetCore target)
{
    //while(1);
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, value);
}

void TestRunner::SetInput(std::string name, int value, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, value);
}

void TestRunner::SetInput(std::string name, float value, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, *((int*)&value));
}

void TestRunner::SetInput(std::string name, unsigned char value, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, value);
}

void TestRunner::SetInput(std::string name, unsigned char* data,
        unsigned int size, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data, size);
}

void TestRunner::SetInputAndSave(std::string name, unsigned char* data,
        unsigned int size, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    saveFile(name + ".pc", (char*)data, size);
    //printf("%s %d\n", shaveName.c_str(), size);
    ioProcessor.setDataAndLoad(shaveName, data, size);
}

void TestRunner::SetInput(std::string name, unsigned char** data,
        unsigned int size, unsigned int width, unsigned int lineNo, TargetCore target)
{

    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);

    //while(1);

    ioProcessor.setData(shaveName, data, size, width, lineNo);
}

void TestRunner::SetInput(std::string name, unsigned short* data,
        unsigned int size, TargetCore target)
{
    SetInput(name, (unsigned char*)data, size * sizeof(unsigned short), target);
}

void TestRunner::SetInput(std::string name, unsigned int* data,
        unsigned int size, TargetCore target)
{
    SetInput(name, (unsigned char*)data, size * sizeof(unsigned int), target);
}

void TestRunner::SetInput(std::string name, int* data,
        unsigned int size, TargetCore target)
{
    SetInput(name, (unsigned char*)data, size * sizeof(int), target);
}

void TestRunner::SetInput(std::string name, unsigned short** data,
        unsigned int size, unsigned int width, unsigned int lineNo, TargetCore target)
{
    SetInput(name, (unsigned char**)data, size * sizeof(unsigned short), width * sizeof(unsigned short), lineNo, target);
}

void TestRunner::SetInput(std::string name, float* data,
        unsigned int size, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data, size);
}

void TestRunner::SetInput(std::string name, float** data, unsigned int size,
        unsigned int width, unsigned int lineNo, TargetCore target)
{
    SetInput(name, (unsigned char**)data, size * sizeof(float), width * sizeof(float), lineNo, target);
}

void TestRunner::SetInput(std::string name, half* data, unsigned int size,
        TargetCore target)
{
    SetInput(name, (unsigned char*)data, size * sizeof(half), target);
}

void TestRunner::SetInput(std::string name, half** data, unsigned int size,
        unsigned int width, unsigned int lineNo, TargetCore target)
{
    SetInput(name, (unsigned char**)data, size * sizeof(half), width * sizeof(half), lineNo, target);
}

void TestRunner::SetInput(std::string name, float4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, int4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, uint4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, short8 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, ushort8 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, char16 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, uchar16 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, half8 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, float4x4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, int4x4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, uint4x4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, long4x4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, ulong4x4 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, short8x8 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, ushort8x8 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}

void TestRunner::SetInput(std::string name, half8x8 data, TargetCore target)
{
    std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
    ioProcessor.setData(shaveName, data);
}


// one u32 value get
unsigned int TestRunner::GetVariableValue(std::string varName)
{
    //while(1);
    return ioProcessor.GetValue(varName);
}
// multiple line get
void TestRunner::GetOutput(std::string name, TargetCore target, unsigned int lineDataSize,
    unsigned int lineWidth, unsigned int lineNo, unsigned char** output)
{
    //while(1);
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, lineDataSize, lineWidth, lineNo, output);
        WRITE_AUTOCHECK_OUT((char*)name.c_str(), target, (unsigned int)lineDataSize,
                (unsigned int)lineWidth, (unsigned int)lineNo, output);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, unsigned int lineDataSize,
        unsigned int lineWidth, unsigned int lineNo, unsigned short** output)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, lineDataSize * sizeof(unsigned short), lineWidth * sizeof(unsigned short), lineNo, (unsigned char**)output);
        WRITE_AUTOCHECK_OUT((char*)name.c_str(), target, (unsigned int)lineDataSize,
                            (unsigned int)lineWidth, (unsigned int)lineNo, output);

    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, unsigned int lineDataSize,
    unsigned int lineWidth, unsigned int lineNo, half** output)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, lineDataSize * sizeof(half), lineWidth * sizeof(half), lineNo, (unsigned char**)output);
        WRITE_AUTOCHECK_OUT((char*)name.c_str(), target, (unsigned int)lineDataSize,
                (unsigned int)lineWidth, (unsigned int)lineNo, output);
    }

}

void TestRunner::GetOutput(std::string name, TargetCore target, unsigned int lineDataSize,
    unsigned int lineWidth, unsigned int lineNo, float** output)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, lineDataSize * sizeof(float), lineWidth * sizeof(float), lineNo, (unsigned char**)output);
        WRITE_AUTOCHECK_OUT((char*)name.c_str(), target, (unsigned int)lineDataSize,
                (unsigned int)lineWidth, (unsigned int)lineNo, output);
    }
}

// single line get
void TestRunner::GetOutput(std::string name, TargetCore target,
        unsigned int size, unsigned char* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, size, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)size, (char*)value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target,
        unsigned int size, unsigned short* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, size * sizeof(unsigned short), (unsigned char*)value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)size, value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target,
    unsigned int size, half* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, size * sizeof(half), (unsigned char*)value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)size, value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target,
    unsigned int size, float* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, size * sizeof(float), (unsigned char*)value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)size, value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, float4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(float) * 4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, int4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(int) * 4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, uint4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(unsigned int) * 4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, short8* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(short) * 8), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, ushort8* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(unsigned short) * 8), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, char16* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(char) * 16), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, uchar16* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(unsigned char) * 16), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, half8* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(half) * 8), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, float4x4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(float) * 4 *4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, int4x4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(int) * 4 *4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, uint4x4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(unsigned int) * 4 *4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, long4x4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(long) * 4 * 4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, ulong4x4* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(unsigned long) * 4 * 4), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, short8x8* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(short) * 8 * 8), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, ushort8x8* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(unsigned short) * 8 * 8), value);
    }
}

void TestRunner::GetOutput(std::string name, TargetCore target, half8x8* value)
{
    if(outputAvailable)
    {
        std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
        ioProcessor.GetOutput(shaveName, value);
        WRITE_AUTOCHECK_OUT_FRM2((char*)name.c_str(), target,
                (unsigned int)(sizeof(half) * 8 * 8), value);
    }
}

//The following inserts guard data (canary values) around output data
template <typename VectorType>
void TestRunner::GuardInsert(std::string name, TargetCore target, VectorType* value)
{

    uint32_t canary[4] = {0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
    uint32_t symAdr,aux;
    std::string prefixName = getSymbolName(name, TARGET_PREFIX, target);
    (void)value;
    symAdr=ioProcessor.getSymAddr(prefixName);

    //Move to prefix
    aux=symAdr-4*sizeof(uint32_t);
    ioProcessor.setData(aux,(unsigned char*)&canary[0],sizeof(uint32_t) * 4);

    //Move to postfix
    aux=symAdr+sizeof(VectorType);

    if(aux&0x3)
    {
        uint32_t mask[4] = { 0, 0xff, 0xffff, 0xffffff};
        uint32_t aux_cannary;

        ioProcessor.GetOutput(aux - (aux & 0x3),sizeof(uint32_t),(unsigned char*)&aux_cannary);

        canary[0] = (aux_cannary & mask[aux&0x3]) | (canary[0] & (~mask[aux&0x3]));

        ioProcessor.setData(aux - (aux & 0x3),(unsigned char*)&canary[0],sizeof(uint32_t) * 4);
    }
    else
    {
        ioProcessor.setData(aux,(unsigned char*)&canary[0],sizeof(uint32_t) * 4);
    }
}

template <typename VectorType>
void TestRunner::GuardInsert(std::string name, TargetCore target,
    unsigned int size, VectorType* value)
{

    uint32_t canary[4] = {0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
    uint32_t symAdr,aux;
    std::string prefixName = getSymbolName(name, TARGET_PREFIX, target);
    (void)value;
    symAdr=ioProcessor.getSymAddr(prefixName);

    //Move to prefix
    aux=symAdr-4*sizeof(uint32_t);
    ioProcessor.setData(aux,(unsigned char*)&canary[0],sizeof(uint32_t) * 4);

    //Move to postfix
    aux=symAdr+size*sizeof(VectorType);

    if(aux&0x3)
    {
        uint32_t mask[4] = { 0, 0xff, 0xffff, 0xffffff};
        uint32_t aux_cannary;

        ioProcessor.GetOutput(aux - (aux & 0x3),sizeof(uint32_t),(unsigned char*)&aux_cannary);

        canary[0] = (aux_cannary & mask[aux&0x3]) | (canary[0] & (~mask[aux&0x3]));

        ioProcessor.setData(aux - (aux & 0x3),(unsigned char*)&canary[0],sizeof(uint32_t) * 4);
    }
    else
    {
        ioProcessor.setData(aux,(unsigned char*)&canary[0],sizeof(uint32_t) * 4);
    }
}

template <typename VectorType>
void TestRunner::GuardInsert(std::string name, TargetCore target, unsigned int lineDataSize,
    unsigned int lineWidth, unsigned int lineNo, VectorType** output)
{

    uint32_t canary[4] = {0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
    uint32_t symAdr,aux;

    std::string prefixName = getSymbolName(name, TARGET_PREFIX, target);
    (void)output;
    symAdr=ioProcessor.getSymAddr(prefixName);

    //Move to prefix
    aux=symAdr-4*sizeof(uint32_t);

    ioProcessor.setData(aux,(unsigned char*)&canary[0],sizeof(uint32_t) * 4);

    //Move to postfix
    aux= symAdr + sizeof(VectorType) * (lineNo-1)*lineWidth + sizeof(VectorType) * lineDataSize;

    if(aux&0x3)
    {
        uint32_t mask[4] = { 0, 0xff, 0xffff, 0xffffff};
        uint32_t aux_cannary;

        ioProcessor.GetOutput(aux - (aux & 0x3),sizeof(uint32_t),(unsigned char*)&aux_cannary);

        canary[0] = (aux_cannary & mask[aux&0x3]) | (canary[0] & (~mask[aux&0x3]));

        ioProcessor.setData(aux - (aux & 0x3),(unsigned char*)&canary[0],sizeof(uint32_t) * 4);
    }
    else
    {
        ioProcessor.setData(aux,(unsigned char*)&canary[0],sizeof(uint32_t) * 4);
    }

}


template void TestRunner::GuardInsert<unsigned char>(std::string name, TargetCore target, unsigned char* values);
template void TestRunner::GuardInsert<unsigned char>(std::string name, TargetCore target, unsigned int size, unsigned char* values);
template void TestRunner::GuardInsert<unsigned char>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, unsigned char** output);

template void TestRunner::GuardInsert<unsigned short>(std::string name, TargetCore target, unsigned int size, unsigned short* value);
template void TestRunner::GuardInsert<unsigned short>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, unsigned short** output);

template void TestRunner::GuardInsert<unsigned int>(std::string name, TargetCore target, unsigned int* values);

template void TestRunner::GuardInsert<float>(std::string name, TargetCore target, unsigned int size, float* value);
template void TestRunner::GuardInsert<float>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, float** output);

template void TestRunner::GuardInsert<half>(std::string name, TargetCore target, unsigned int size, half* value);
template void TestRunner::GuardInsert<half>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, half** output);

template void TestRunner::GuardInsert<float4>(std::string name, TargetCore target, float4* value);
template void TestRunner::GuardInsert<int4>(std::string name, TargetCore target, int4* value);
template void TestRunner::GuardInsert<uint4>(std::string name, TargetCore target, uint4* value);
template void TestRunner::GuardInsert<short8>(std::string name, TargetCore target, short8* value);
template void TestRunner::GuardInsert<ushort8>(std::string name, TargetCore target, ushort8* value);
template void TestRunner::GuardInsert<char16>(std::string name, TargetCore target, char16* value);
template void TestRunner::GuardInsert<uchar16>(std::string name, TargetCore target, uchar16* value);
template void TestRunner::GuardInsert<half8>(std::string name, TargetCore target, half8* value);

template void TestRunner::GuardInsert<float4x4>(std::string name, TargetCore target, float4x4* output);
template void TestRunner::GuardInsert<int4x4>(std::string name, TargetCore target, int4x4* output);
template void TestRunner::GuardInsert<uint4x4>(std::string name, TargetCore target, uint4x4* output);
template void TestRunner::GuardInsert<long4x4>(std::string name, TargetCore target, long4x4* output);
template void TestRunner::GuardInsert<ulong4x4>(std::string name, TargetCore target, ulong4x4* output);
template void TestRunner::GuardInsert<short8x8>(std::string name, TargetCore target, short8x8* output);
template void TestRunner::GuardInsert<ushort8x8>(std::string name, TargetCore target, ushort8x8* output);
template void TestRunner::GuardInsert<half8x8>(std::string name, TargetCore target, half8x8* output);

//The following inserts guard data (canary values) around output data
template <typename VectorType>
void TestRunner::GuardCheck(std::string name, TargetCore target, VectorType* value)
{

    uint32_t pre_canary_check[4];
    uint32_t post_canary_check[4];
    uint32_t mask[4] = { 0, 0xff, 0xffff, 0xffffff};
    uint32_t symAdr,aux;
    std::string prefixName = getSymbolName(name, TARGET_PREFIX, target);
    (void)value;
    symAdr=ioProcessor.getSymAddr(prefixName);

    //Move to prefix
    aux=symAdr-4*sizeof(uint32_t);
    ioProcessor.GetOutput(aux,sizeof(uint32_t) * 4,(unsigned char*)&pre_canary_check[0]);
    EXPECT_EQ(pre_canary_check[0], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[1], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[2], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[3], 0xDEADBEEF);

    //Move to postfix
    aux=symAdr+sizeof(VectorType);

    ioProcessor.GetOutput(aux - (aux & 0x3),sizeof(uint32_t) * 4,(unsigned char*)&post_canary_check[0]);

    EXPECT_EQ(post_canary_check[0] & (~mask[aux&0x3]), 0xDEADBEEF & (~mask[aux&0x3]));
    EXPECT_EQ(post_canary_check[1], 0xDEADBEEF);
    EXPECT_EQ(post_canary_check[2], 0xDEADBEEF);
    EXPECT_EQ(post_canary_check[3], 0xDEADBEEF);
}

//The following inserts guard data (canary values) around output data
template <typename VectorType>
void TestRunner::GuardCheck(std::string name, TargetCore target,
        unsigned int size, VectorType* value)
{

    uint32_t pre_canary_check[4];
    uint32_t post_canary_check[4];
    uint32_t mask[4] = { 0, 0xff, 0xffff, 0xffffff};
    uint32_t symAdr,aux;

    std::string prefixName = getSymbolName(name, TARGET_PREFIX, target);
    (void)value;
    symAdr=ioProcessor.getSymAddr(prefixName);

    //Move to prefix
    aux=symAdr-4*sizeof(uint32_t);
    ioProcessor.GetOutput(aux,sizeof(uint32_t) * 4,(unsigned char*)&pre_canary_check[0]);
    EXPECT_EQ(pre_canary_check[0], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[1], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[2], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[3], 0xDEADBEEF);

    //Move to postfix
    aux=symAdr+size*sizeof(VectorType);

    ioProcessor.GetOutput(aux - (aux & 0x3),sizeof(uint32_t) * 4,(unsigned char*)&post_canary_check[0]);

    EXPECT_EQ(post_canary_check[0] & (~mask[aux&0x3]), 0xDEADBEEF & (~mask[aux&0x3]));
    EXPECT_EQ(post_canary_check[1], 0xDEADBEEF);
    EXPECT_EQ(post_canary_check[2], 0xDEADBEEF);
    EXPECT_EQ(post_canary_check[3], 0xDEADBEEF);
}


template <typename VectorType>
void TestRunner::GuardCheck(std::string name, TargetCore target, unsigned int lineDataSize,
    unsigned int lineWidth, unsigned int lineNo, VectorType** output)
{
    uint32_t pre_canary_check[4];
    uint32_t post_canary_check[4];
    uint32_t mask[4] = { 0, 0xff, 0xffff, 0xffffff};
    uint32_t symAdr,aux;
    std::string prefixName = getSymbolName(name, TARGET_PREFIX, target);
    (void)output;
    symAdr=ioProcessor.getSymAddr(prefixName);

    //Move to prefix
    aux=symAdr-4*sizeof(uint32_t);

    ioProcessor.GetOutput(aux,sizeof(uint32_t) * 4,(unsigned char*)&pre_canary_check[0]);
    EXPECT_EQ(pre_canary_check[0], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[1], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[2], 0xDEADBEEF);
    EXPECT_EQ(pre_canary_check[3], 0xDEADBEEF);

    //Move to postfix
    aux=symAdr + sizeof(VectorType) * (lineNo-1)*lineWidth + sizeof(VectorType) *lineDataSize;

    ioProcessor.GetOutput(aux - (aux & 0x3),sizeof(uint32_t) * 4,(unsigned char*)&post_canary_check[0]);

    EXPECT_EQ(post_canary_check[0] & (~mask[aux&0x3]), 0xDEADBEEF & (~mask[aux&0x3]));
    EXPECT_EQ(post_canary_check[1], 0xDEADBEEF);
    EXPECT_EQ(post_canary_check[2], 0xDEADBEEF);
    EXPECT_EQ(post_canary_check[3], 0xDEADBEEF);
}

template void TestRunner::GuardCheck<unsigned char>(std::string name, TargetCore target, unsigned char* values);

template void TestRunner::GuardCheck<unsigned char>(std::string name, TargetCore target, unsigned int size, unsigned char* values);
template void TestRunner::GuardCheck<unsigned char>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, unsigned char** output);

template void TestRunner::GuardCheck<unsigned short>(std::string name, TargetCore target, unsigned int size, unsigned short* value);
template void TestRunner::GuardCheck<unsigned short>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, unsigned short** output);

template void TestRunner::GuardCheck<unsigned int>(std::string name, TargetCore target, unsigned int* values);

template void TestRunner::GuardCheck<float>(std::string name, TargetCore target, unsigned int size, float* value);
template void TestRunner::GuardCheck<float>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, float** output);

template void TestRunner::GuardCheck<half>(std::string name, TargetCore target, unsigned int size, half* value);
template void TestRunner::GuardCheck<half>(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, half** output);

template void TestRunner::GuardCheck<float4>(std::string name, TargetCore target, float4* value);
template void TestRunner::GuardCheck<int4>(std::string name, TargetCore target, int4* value);
template void TestRunner::GuardCheck<uint4>(std::string name, TargetCore target, uint4* value);
template void TestRunner::GuardCheck<short8>(std::string name, TargetCore target, short8* value);
template void TestRunner::GuardCheck<ushort8>(std::string name, TargetCore target, ushort8* value);
template void TestRunner::GuardCheck<char16>(std::string name, TargetCore target, char16* value);
template void TestRunner::GuardCheck<uchar16>(std::string name, TargetCore target, uchar16* value);
template void TestRunner::GuardCheck<half8>(std::string name, TargetCore target, half8* value);

template void TestRunner::GuardCheck<float4x4>(std::string name, TargetCore target, float4x4* output);
template void TestRunner::GuardCheck<int4x4>(std::string name, TargetCore target, int4x4* output);
template void TestRunner::GuardCheck<uint4x4>(std::string name, TargetCore target, uint4x4* output);
template void TestRunner::GuardCheck<long4x4>(std::string name, TargetCore target, long4x4* output);
template void TestRunner::GuardCheck<ulong4x4>(std::string name, TargetCore target, ulong4x4* output);
template void TestRunner::GuardCheck<short8x8>(std::string name, TargetCore target, short8x8* output);
template void TestRunner::GuardCheck<ushort8x8>(std::string name, TargetCore target, ushort8x8* output);
template void TestRunner::GuardCheck<half8x8>(std::string name, TargetCore target, half8x8* output);

// run
void TestRunner::Run(TargetCore target)
{

    if(debugMode)
    {
        debugMode = ioProcessor.InteractiveDebug();
    }
    else
    {
        if(registersChecking)
            ioProcessor.setRegistersCheckingOn();
        registersChecking = ioProcessor.Run(target);

    }

    outputAvailable = true;
}

void TestRunner::Run(TargetCore target, std::string entryPoint)
{
    if(debugMode)
    {
        debugMode = ioProcessor.InteractiveDebug();
    }
    else
    {
        if(registersChecking)
            ioProcessor.setRegistersCheckingOn();
        registersChecking = ioProcessor.Run(target, entryPoint);
    }

    outputAvailable = true;
}

