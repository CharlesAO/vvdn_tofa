// ========================================================================================
// =================== DUMP ELF AREEA =====================================================
// ========================================================================================

#ifdef VCS_TESTING_DUMP_ELF  
//
#include "moviDebugDll.h"
#include <cstdio>
#include <cstdlib>
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif
#include "internalDebug.h"
#include "cstring"
//ELF File Header
typedef struct
{
    unsigned char     id[16];           // ID of the ELF file
    unsigned short    type;             // Type of the object file
    unsigned short    target;           // Target processor
    unsigned int      version;          // File version
    unsigned int      entry;            // Entry point virtual address
    unsigned int      phOffset;         // Program header table file offset
    unsigned int      shOffset;         // Section header table file offset
    unsigned int      flags;            // Processor-specific flags
    unsigned short    ehSize;           // ELF header size in bytes
    unsigned short    phEntrySize;      // Program header table entry size
    unsigned short    phCount;          // Program header table entry count
    unsigned short    shEntrySize;      // Section header table entry size
    unsigned short    shCount;          // Section header table entry count
    unsigned short    shStringIndex;    // Section header string table index
}ElfFileHeader;

//ELF Section Header
typedef struct
{
    unsigned int name;                  // Section name(string tbl index)
    unsigned int type;                  // Section type
    unsigned int flags;                 // Section flags
    unsigned int addr;                  // Section virtual addr at execution
    unsigned int offset;                // Section file offset
    unsigned int size;                  // Section size in bytes
    unsigned int link;                  // Link to another section
    unsigned int info;                  // Additional section information
    unsigned int addralign;             // Section alignment
    unsigned int entsize;               // Entry size if section holds table
}ElfSectionHeader;

typedef struct {
    unsigned int size;
    unsigned int address;
    unsigned int fileOfset;
}sectionsInfo;

typedef struct {
    unsigned int refAdr;
    unsigned int runAdr;
    unsigned int size;
} AutoCheckStruct;

TargetCore  VCSTEST_TARGET = SHAVE8;
std::string VCSTEST_PREFIX = "vcstest";

// apparently a part of the test was wrote for demonstrate that PC, reference is good. 
// Nothing to do with  asm verification tests. I need this variable for skip this tests.
static unsigned int EnableDumpingElf = 0;



//=======================================================================================
//=======================================================================================
//=======================================================================================
static std::string getSymbolName(std::string name, std::string symbolPrefix,
        TargetCore target)
{
    char targetStr[3];

    if(symbolPrefix.length() > 0) {
        sprintf(targetStr, "%d", target);
        std::string str = symbolPrefix + std::string(targetStr) + "_" + name;
        return symbolPrefix + std::string(targetStr) + "_" + name;
    }
    return name;
}


void getDatas(unsigned char * outputBuff, unsigned int startAddr,
        unsigned int size)
{
    unsigned int addr = startAddr;
    unsigned int sz = size>>2;
    unsigned int *crtBufAdr = (unsigned int *)outputBuff;

    for(unsigned int i = 0; i < sz; i++) {
        DllMoviDebugGet(addr, 4, crtBufAdr);
        crtBufAdr++;
        addr += 4;
    }
}

void setDatas(unsigned int startAddr, unsigned char * inBuff, unsigned int size)
{
    unsigned int addr = startAddr;
    unsigned int sz = size>>2;
    unsigned int *crtBufAdr = (unsigned int *)inBuff;

    for(unsigned int i = 0; i < sz; i++) {
        DllMoviDebugSet(addr, crtBufAdr[0], 4);
        crtBufAdr++;
        addr += 4;
    }
}

static unsigned int getIntVal(std::string name, TargetCore target, std::string prefix)
{
    std::string shaveName = getSymbolName(name, prefix, target);
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(shaveName.c_str()));
    unsigned int value;
    DllMoviDebugGet(symbAddr, 4, &value);
    return value;
}

unsigned int getSimbolAdr(std::string name, TargetCore target, std::string prefix)
{
    std::string shaveName = getSymbolName(name, prefix, target);
    unsigned int symbAddr = DllMoviDebugGetAddress(const_cast<char*>(shaveName.c_str()));
    return symbAddr;
}

static void setIntVal(std::string name, unsigned int value, TargetCore target, std::string prefix)
{
    std::string shaveName = getSymbolName(name, prefix, target);
    unsigned int symAddr = DllMoviDebugGetAddress(const_cast<char*>(shaveName.c_str()));
    DllMoviDebugSet(symAddr, value, sizeof(unsigned int));
}


//=======================================================================================
//=======================================================================================
//=======================================================================================
template<typename T>
void writeExpectedOutputBackGeneral(char* name, TargetCore target,
        unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, T** data)
{
    EnableDumpingElf = 1;
    std::string varNm;
    std::string simbNm;

    // enable autoCheck
    varNm ="savedElf";
    setIntVal(varNm, (unsigned int)1, VCSTEST_TARGET, VCSTEST_PREFIX);

    // get current numbers of auto-check buffers
    varNm ="nrOfCheckVariable";
    unsigned int autoCheckBufersNr = getIntVal(varNm, VCSTEST_TARGET, VCSTEST_PREFIX);

    // get auto-check buffers base address
    varNm ="StartAddrPcBoardLength";
    unsigned int autoCheckBufersBsAdr = getSimbolAdr(varNm, VCSTEST_TARGET, VCSTEST_PREFIX);

    // extract referenced address, where will be wroten the lines
    varNm = "pcExpectedOutputPointer";
    unsigned int expectedRefAddr = getIntVal(varNm, VCSTEST_TARGET, VCSTEST_PREFIX);

    // extract output address reference
    unsigned int newGeneratedRefAddr = getSimbolAdr(name, target, TARGET_PREFIX);

    for(unsigned int i = 0; i < lineNo; i++) {
        // populate with data
        AutoCheckStruct autoCheckStruct = {0, 0, 0};

        autoCheckStruct.refAdr = expectedRefAddr;
        autoCheckStruct.runAdr = newGeneratedRefAddr + i * lineWidth * sizeof(T);
        autoCheckStruct.size   = lineDataSize;

        setDatas(autoCheckBufersBsAdr + autoCheckBufersNr * 4 * 3,
                (unsigned char*) &autoCheckStruct, 4 * 3);
        setDatas(expectedRefAddr,
                (unsigned char*)data[i], lineDataSize * sizeof(T));
        expectedRefAddr += (lineDataSize * sizeof(T));
        autoCheckBufersNr++;
    }

    varNm ="pcExpectedOutputPointer";
    setIntVal(varNm, (unsigned int)expectedRefAddr, VCSTEST_TARGET, VCSTEST_PREFIX);

    varNm ="nrOfCheckVariable";
    setIntVal(varNm, (unsigned int)autoCheckBufersNr, VCSTEST_TARGET, VCSTEST_PREFIX);
}

template<typename T>
void writeExpectedOutputBackGeneral(char* name, TargetCore target,
        unsigned int size, T* data)
{
    EnableDumpingElf = 1;
    std::string varNm;
    std::string simbNm;

    // enable autoCheck
    varNm ="savedElf";
    setIntVal(varNm, (unsigned int)1, VCSTEST_TARGET, VCSTEST_PREFIX);

    // get current numbers of auto-check buffers
    varNm ="nrOfCheckVariable";
    unsigned int autoCheckBufersNr = getIntVal(varNm, VCSTEST_TARGET, VCSTEST_PREFIX);

    // get auto-check buffers base address
    varNm ="StartAddrPcBoardLength";
    unsigned int autoCheckBufersBsAdr = getSimbolAdr(varNm, VCSTEST_TARGET, VCSTEST_PREFIX);

    // extract referenced address, where will be wroten the lines
    varNm = "pcExpectedOutputPointer";
    unsigned int expectedRefAddr = getIntVal(varNm, VCSTEST_TARGET, VCSTEST_PREFIX);

    // extract output address reference
    unsigned int newGeneratedRefAddr = getSimbolAdr(name, target, TARGET_PREFIX);

    // populate with data
    AutoCheckStruct autoCheckStruct = {0, 0, 0};

    autoCheckStruct.refAdr = expectedRefAddr;
    autoCheckStruct.runAdr = newGeneratedRefAddr;
    autoCheckStruct.size   = size;

    setDatas(autoCheckBufersBsAdr + autoCheckBufersNr * 4 * 3,
            (unsigned char*) &autoCheckStruct, 4 * 3);
    setDatas(expectedRefAddr,
            (unsigned char*)data, size * sizeof(T));
    expectedRefAddr += (size * sizeof(T));
    autoCheckBufersNr++;

    varNm ="pcExpectedOutputPointer";
    setIntVal(varNm, (unsigned int)expectedRefAddr, VCSTEST_TARGET, VCSTEST_PREFIX);

    varNm ="nrOfCheckVariable";
    setIntVal(varNm, (unsigned int)autoCheckBufersNr, VCSTEST_TARGET, VCSTEST_PREFIX);
}


// INTERFACE
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int lineDataSize, unsigned int lineWidth,
        unsigned int lineNo, unsigned char** data)
{
    writeExpectedOutputBackGeneral(name, target,
            lineDataSize, lineWidth, lineNo, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int lineDataSize, unsigned int lineWidth,
        unsigned int lineNo, unsigned short** data)
{
    writeExpectedOutputBackGeneral(name, target,
            lineDataSize, lineWidth, lineNo, data);
}

void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int lineDataSize, unsigned int lineWidth,
        unsigned int lineNo, unsigned int** data)
{
    writeExpectedOutputBackGeneral(name, target,
            lineDataSize, lineWidth, lineNo, data);
}

void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int lineDataSize, unsigned int lineWidth,
        unsigned int lineNo, half** data)
{
    writeExpectedOutputBackGeneral(name, target,
            lineDataSize, lineWidth, lineNo, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int lineDataSize, unsigned int lineWidth,
        unsigned int lineNo, float** data)
{
    writeExpectedOutputBackGeneral(name, target,
            lineDataSize, lineWidth, lineNo, data);
}
// 1 line
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, char* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, unsigned short* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, unsigned int* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, half* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, float* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, float4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, int4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, uint4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, short8* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, ushort8* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, char16* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, uchar16* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, half8* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, float4x4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, int4x4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, uint4x4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, long4x4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, ulong4x4* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, short8x8* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, ushort8x8* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}
void writeExpectedOutputBack(char* name, TargetCore target,
        unsigned int size, half8x8* data)
{
    writeExpectedOutputBackGeneral(name, target,
            size, data);
}

//=======================================================================================
//=======================================================================================
//=======================================================================================
static int writeInElfNewSection(char *newElfFileName, sectionsInfo infoSections,
        unsigned char *dataBuffer, char *elfFileInputName);
static int extractSectionsInformation(sectionsInfo *infoSections,
        char *elfFileName, char* sectionName);



static int ExtractTheNewElf(char* newElfFileName)
{
#define SECTION_NAME "S.shv8.cmx.data"
#define INFILE_NAME "dummy/output/dummy.elf"
    unsigned int returnVal = false;
    sectionsInfo section = {0,0,0};
    unsigned char *dataReadedFromBoard;
    if(extractSectionsInformation(&section, (char*)INFILE_NAME, (char*)SECTION_NAME)) {
        dataReadedFromBoard = (unsigned char *)malloc(section.size);
        getDatas(dataReadedFromBoard, section.address, section.size);
        if(writeInElfNewSection(newElfFileName, section, dataReadedFromBoard, (char*)INFILE_NAME)){
            returnVal = true;
        }
        free(dataReadedFromBoard);
    }
    return returnVal;
}

static int writeInElfNewSection(char *newElfFileName, sectionsInfo infoSections,
        unsigned char *dataBuffer, char *elfFileInputName)
{
    // read the input buffer
    FILE *fileInputStream;
    fileInputStream = fopen(elfFileInputName, "rb");
    if (fileInputStream == NULL) {
        printf("\n Error reading input elf file. Incorrect name, probably. \n");
        fclose(fileInputStream);
        return false;
    }
    fseek(fileInputStream, 0L, SEEK_END);
    int sz = ftell(fileInputStream);
    fseek(fileInputStream, 0L, SEEK_SET);
    unsigned char *bigBuffer = (unsigned char *)malloc(sz);
    fread(bigBuffer,1, sz, fileInputStream);
    fclose(fileInputStream);
    // write output elf file
    FILE *fileOutputStream;
    fileOutputStream = fopen(newElfFileName, "wb");
    if (fileOutputStream == NULL) {
        printf("\n Error open output elf file. Probably is in use by other program. \n");
        fclose(fileOutputStream);
        free(bigBuffer);
        return false;
    }
    fwrite(bigBuffer, 1, sz, fileOutputStream);
    fseek(fileOutputStream, infoSections.fileOfset, SEEK_SET);
    fwrite(dataBuffer, 1, infoSections.size, fileOutputStream);
    fclose(fileOutputStream);
    free(bigBuffer);
    return true;
}

static int extractSectionsInformation(sectionsInfo *infoSections,
        char *elfFileName, char* sectionName)
{
    FILE *fileStream;
    ElfFileHeader fileHeader;
    ElfSectionHeader sectionHeader;
    int sectionId = -1;
    fileStream = fopen(elfFileName, "rb");
    if(fread((void*)&fileHeader, sizeof(ElfFileHeader), 1, fileStream)< 1){
        printf("\n Error reading input file header. \n");
        fclose(fileStream);
        return false;
    }
    unsigned int sectionHeaderOffset = fileHeader.shOffset;

// ==============================EXTRACT section name==========================================================
    //Find the string table offset
    ElfSectionHeader strtabSectionHeader;
    fseek(fileStream, sectionHeaderOffset + fileHeader.shStringIndex * sizeof(strtabSectionHeader), SEEK_SET);
    if(fread(&strtabSectionHeader, sizeof(strtabSectionHeader), 1, fileStream) != 1) {
        printf("\n MSG_MTC_ERR_CANNOT_READ_ELF_SECTION_HEADER \n");
        fclose(fileStream);
        return false;
    }
    //Read the .strtab section from the input file
    fseek(fileStream, strtabSectionHeader.offset, SEEK_SET);
    int c, index = 0;
    char retValue[1024];
    unsigned int pos = ftell(fileStream);
    unsigned int stop = pos + strtabSectionHeader.size;
    int compareRezult = 0;
    int crtOffset = -1;
    //Read all the strings
    do {
        sectionId = crtOffset+1;
        index = 0;
        c = fgetc(fileStream);
        while((c != '\0')&&(c != EOF)) {
            crtOffset++;
            retValue[index++] =(char)c;
            c = fgetc(fileStream);
        }
        crtOffset++;
        retValue[index++] = '\0';
        compareRezult = strcmp(retValue, sectionName);
        pos = ftell(fileStream);
    }while((pos < stop) && (compareRezult != 0));
    if (compareRezult != 0){
        printf("\n Section not found. \n");
        printf("\n Invalid Section Name. \n");
        fclose(fileStream);
        return false;
    }

    //Read the section header searching for string index
    unsigned int nameId = -1;
    do {
        nameId++;
        fseek(fileStream, sectionHeaderOffset + nameId * sizeof(ElfSectionHeader), SEEK_SET);
        if(fread(&sectionHeader, sizeof(sectionHeader), 1, fileStream) != 1) {
            printf("ERROR: Not able to read from file sections headers %d \n", nameId);
            fclose(fileStream);
            return false;
        }
    }while(((int)sectionHeader.name != sectionId) && (nameId < fileHeader.shCount));

    if ((int)sectionHeader.name != sectionId){
        printf("\n Section not found. \n");
        printf("\n Invalid Section Id. \n");
        fclose(fileStream);
        return false;
    }

    infoSections->address = sectionHeader.addr;
    infoSections->size = sectionHeader.size;
    infoSections->fileOfset = sectionHeader.offset;
    fclose(fileStream);
    return true;
}


static int resetElfToDefault(void)
{
    std::string varNm;
    // hardcoded for shave 0
    TargetCore target = SHAVE0;

    varNm ="pcExpectedOutput";
    unsigned int autoCheckBufersBsAdr = getSimbolAdr(varNm, VCSTEST_TARGET, VCSTEST_PREFIX);
    varNm ="pcExpectedOutputPointer";
    setIntVal(varNm, (unsigned int)autoCheckBufersBsAdr, VCSTEST_TARGET, VCSTEST_PREFIX);

    varNm ="nrOfCheckVariable";
    setIntVal(varNm, (unsigned int)0, VCSTEST_TARGET, VCSTEST_PREFIX);
    // disable  autoCheck
    varNm ="savedElf";
    setIntVal(varNm, (unsigned int)0, VCSTEST_TARGET, VCSTEST_PREFIX);
    return true;
}



int saveElf(void)
{
    if (1 == EnableDumpingElf) 
    {
        static int elfNr = 0;
        char cCurrentPath[FILENAME_MAX];
        char fileElfName[FILENAME_MAX + 100];
    
        if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
        {
            EnableDumpingElf = 0;
            return false;
        }
        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

        int i = strlen(cCurrentPath) - 1;
		int c = 0;
		//(cCurrentPath[i] != '/') & (i>0)
        while(c < 2 ) {
            i--;
			if (cCurrentPath[i] == '/') c++;
        }

        if (0 == i) {
            printf("\n Error reading new elf path directory! \n");
            EnableDumpingElf = 0;
            return false;
        }
		
        strcpy(fileElfName, cCurrentPath);
        strcat(fileElfName, "/");
		
		unsigned int j;
		
		for ( j= i+1 ; j<= strlen(cCurrentPath) - 1; j++)
		{
			if (cCurrentPath[j] == '/') 
			{
				cCurrentPath[j] = '\0';
				break;
			}
		}
		
			
        strcat(fileElfName, &cCurrentPath[i+1]);

        sprintf(fileElfName,"%s%d.elf", fileElfName, elfNr);
        printf ("The New Elf Name is: \n %s \n", fileElfName);

        elfNr++;

        if(ExtractTheNewElf(fileElfName)) {
            if(resetElfToDefault()) {
                EnableDumpingElf = 0;
                return true;
            }
            else {
                printf("\n Error reseting elf to default! \n");
                EnableDumpingElf = 0;
                return false;
            }
        }
        else {
            printf("Error Extracting elf! \n\n");
            EnableDumpingElf = 0;
            return false;
        }
    }
    EnableDumpingElf = 0;
    return false;
}
#endif
