///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     IOProcessing class definition
///

#ifndef __IO_PROCESSING_CLASS__
#define __IO_PROCESSING_CLASS__

#include <string>
#include <assert.h>
#include <VectorTypes.h>
enum MoviDebugBoards
{
    BOARD_ASIC = 0,     //No target
    BOARD_VCS_SIM,      //Sabre ASIC
    BOARD_MOVISIM,      //Movidius simulator
    BOARD_FPGA,         //Sabre FPGA
    BOARD_INVALID
};


enum RegisterType
{
    IRF = 0,
    SRF,
    VRF
};

typedef enum
{
	FINISH = -2,
	SHAVE_ALL = -1,
    SHAVE0 = 0,
    SHAVE1,
    SHAVE2,
    SHAVE3,
    SHAVE4,
    SHAVE5,
    SHAVE6,
    SHAVE7,
    SHAVE8,
    SHAVE9,
    SHAVE10,
    SHAVE11,
    LEON_OS,
    LEON_RT,
    LEON = LEON_OS
} TargetCore;

class IOProcessing {
public:
	IOProcessing(std::string prjPath);
	IOProcessing(std::string prjPath, unsigned int dbgInterface);
	void SetProjectPath(std::string prjPath);
	void SetAsmLabel(std::string asmLbl);
	void SetSrvIP(char *serverIP);
	void SetSrvPort(char *serverPort, bool defaultPort);

	void LoadElf(std::string elfFilePath);
	void LoadFromFile(unsigned int addr, std::string fileName);
	void LoadFromFile(std::string symbol, std::string fileName);
	void SaveToFile(unsigned int addr, unsigned int size, std::string fileName);
	void SaveToFile(std::string symbol, unsigned int size, std::string fileName);
	bool Run(TargetCore target);
	bool Run(TargetCore target, std::string entryPoint);
	
	unsigned int GetIntOutput(std::string varName);
	void GetOutput(std::string varName, unsigned int size, unsigned char* output);
	void GetOutput(unsigned int addr, unsigned int size, unsigned char* output);	
	void GetOutput(std::string varName, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo,
			unsigned char** output);
	unsigned int GetValue(std::string symbName);
	
	void GetOutput(std::string varName, float4* output);
	void GetOutput(unsigned int addr, float4* output);	
			
	void GetOutput(std::string varName, int4* output);
	void GetOutput(unsigned int addr, int4* output);	
	
	void GetOutput(std::string varName, uint4* output);
	void GetOutput(unsigned int addr, uint4* output);	

	void GetOutput(std::string varName, short8* output);
	void GetOutput(unsigned int addr, short8* output);	
	
	void GetOutput(std::string varName, ushort8* output);
	void GetOutput(unsigned int addr, ushort8* output);	

	void GetOutput(std::string varName, char16* output);
	void GetOutput(unsigned int addr, char16* output);

	void GetOutput(std::string varName, uchar16* output);
	void GetOutput(unsigned int addr, uchar16* output);
	
	void GetOutput(std::string varName, half8* output);
	void GetOutput(unsigned int addr, half8* output);	
	
	void GetOutput(std::string varName, float4x4* output);
	void GetOutput(unsigned int addr, float4x4* output);	
	
	void GetOutput(std::string varName, int4x4* output);
	void GetOutput(unsigned int addr, int4x4* output);	
	
	void GetOutput(std::string varName, uint4x4* output);
	void GetOutput(unsigned int addr, uint4x4* output);	
	
	void GetOutput(std::string varName, long4x4* output);
	void GetOutput(unsigned int addr, long4x4* output);	
	
	void GetOutput(std::string varName, ulong4x4* output);
	void GetOutput(unsigned int addr, ulong4x4* output);	
	
	void GetOutput(std::string varName, short8x8* output);
	void GetOutput(unsigned int addr, short8x8* output);	
	
	void GetOutput(std::string varName, ushort8x8* output);
	void GetOutput(unsigned int addr, ushort8x8* output);	
	
	void GetOutput(std::string varName, half8x8* output);
	void GetOutput(unsigned int addr, half8x8* output);

	void setData(unsigned int addr, unsigned char* data, unsigned int size);
	void setData(std::string symbol, unsigned char* data, unsigned int size);
	void setDataAndLoad(std::string symbol, unsigned char* data, unsigned int size);
	void setData(std::string symbol, unsigned char** data, unsigned int lineDataSize,
				 unsigned int lineWidth, unsigned int noOfLines);
	void setData(unsigned int addr, float* data, unsigned int size);
	void setData(std::string symbol, float* data, unsigned int size);
	void setData(std::string symbol, float* data, unsigned int lineDataSize,
					 unsigned int lineWidth, unsigned int noOfLines);
					 
	void setData(unsigned int addr, float4 data);
	void setData(std::string symbol, float4 data);
					 
	void setData(unsigned int addr, int4 data);
	void setData(std::string symbol, int4 data);
	
	void setData(unsigned int addr, uint4 data);
	void setData(std::string symbol, uint4 data);
			
	void setData(unsigned int addr, short8 data);
	void setData(std::string symbol, short8 data);
	
	void setData(unsigned int addr, ushort8 data);
	void setData(std::string symbol, ushort8 data);
					 
	void setData(unsigned int addr, char16 data);
	void setData(std::string symbol, char16 data);

	void setData(unsigned int addr, uchar16 data);
	void setData(std::string symbol, uchar16 data);
	
	void setData(unsigned int addr, half8 data);
	void setData(std::string symbol, half8 data);
		
	void setData(unsigned int addr, float4x4 data);
	void setData(std::string symbol, float4x4 data);
	
	void setData(unsigned int addr, int4x4 data);
	void setData(std::string symbol, int4x4 data);	
	
	void setData(unsigned int addr, uint4x4 data);
	void setData(std::string symbol, uint4x4 data);	

	void setData(unsigned int addr, long4x4 data);
	void setData(std::string symbol, long4x4 data);	
	
	void setData(unsigned int addr, ulong4x4 data);
	void setData(std::string symbol, ulong4x4 data);	

	void setData(unsigned int addr, short8x8 data);
	void setData(std::string symbol, short8x8 data);	
	
	void setData(unsigned int addr, ushort8x8 data);
	void setData(std::string symbol, ushort8x8 data);	
	
	void setData(unsigned int addr, half8x8 data);
	void setData(std::string symbol, half8x8 data);
			 
	void setData(std::string symbol, unsigned int value);
	bool InteractiveDebug(void);
	unsigned int getSymbolAddr(std::string symbolName); // returns sym address parsing map file
	unsigned int getSymAddr(std::string symbolName); // returns sym address using movidebugdll functionality
	
	void setRegistersCheckingOn()
	{
		isFirstTest = true;
	}

private:
	std::string projectPath;
	std::string mapFilePath;
	std::string asmLabel;
	std::string functionLabel;
	char srvIP[20];
	char srvPort[10];
	bool elfLoaded;
	unsigned int cycleCount;
	unsigned int debugInterface;
	bool runWithSimulator;
	static bool isFirstTest;
	static bool defaultPort;
	
	int runCmd(std::string cmd);
	bool cleanProject(void);
	unsigned int getValueFromAddr(unsigned int address, TargetCore target);
	std::string getTargetName(TargetCore target);
	int connectionInit();
	int connectionInit(int targets, char *moviDebugServerIP, char *moviDebugServerPort);
	void connectionClose();
	std::string getRegisterName(unsigned int paramPosition, RegisterType type);
	std::string getShaveName(TargetCore target);
	void selectTarget(TargetCore target);
	void storePreservedRegisters();
	void verifyPreservedRegisters();
	void printInfoPreservedRegisters();
	bool runCheckingRegisters(TargetCore target);

	/**
	 * C++ version 0.4 char* style "itoa":
	 * Written by Lukás Chmela
	 * Released under GPLv3.
	 */
	char* itoa(int value, char* result, int base) {
		// check that the base if valid
		if (base < 2 || base > 36) { *result = '\0'; return result; }
	
		char* ptr = result, *ptr1 = result, tmp_char;
		int tmp_value;
	
		do {
			tmp_value = value;
			value /= base;
			*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
		} while ( value );
	
		// Apply negative sign
		if (tmp_value < 0) *ptr++ = '-';
		*ptr-- = '\0';
		while(ptr1 < ptr) {
			tmp_char = *ptr;
			*ptr--= *ptr1;
			*ptr1++ = tmp_char;
		}
		return result;
	}	
};

#endif //__IO_PROCESSING_CLASS__
