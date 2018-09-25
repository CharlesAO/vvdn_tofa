#ifndef TESTRUNNER_H_
#define TESTRUNNER_H_

#include <string.h>
#include "IOProcessing.h"
#include "half.h"
#include <cstdio>
#include <VectorTypes.h>

#ifndef TARGET_PREFIX
#define TARGET_PREFIX ""
#endif

class TestRunner {
public:
	TestRunner(std::string projectPath, std::string elfFilePath);
	TestRunner(std::string projectPath, std::string elfPath, unsigned int debugInterface);
	TestRunner(std::string projectPath, std::string elfPath, std::string kernelAsmLabel, unsigned int debugInterface);
	virtual ~TestRunner();

	void Init();
	void SetInput(std::string name, unsigned int value, TargetCore target);
	void SetInput(std::string name, int value, TargetCore target);
	void SetInput(std::string name, float value, TargetCore target);
	void SetInput(std::string name, unsigned char value, TargetCore target);
    
	void SetInput(std::string name, unsigned char* data, unsigned int size, TargetCore target);
	void SetInputAndSave(std::string name, unsigned char* data, unsigned int size, TargetCore target);
	void SetInput(std::string name, unsigned char** data, unsigned int size, unsigned int width, unsigned int lineNo, TargetCore target);
    
	void SetInput(std::string name, unsigned short* data, unsigned int size, TargetCore target);
	void SetInput(std::string name, unsigned int* data, unsigned int size, TargetCore target);
	void SetInput(std::string name, int* data, unsigned int size, TargetCore target); 
	void SetInput(std::string name, unsigned short** data, unsigned int size, unsigned int width, unsigned int lineNo, TargetCore target);

	void SetInput(std::string name, float* data, unsigned int size, TargetCore target);
	void SetInput(std::string name, float** data, unsigned int size, unsigned int width, unsigned int lineNo, TargetCore target);
    
	void SetInput(std::string name, half* data, unsigned int size, TargetCore target);
	void SetInput(std::string name, half** data, unsigned int size, unsigned int width, unsigned int lineNo, TargetCore target);

	void SetInput(std::string name, float4 data, TargetCore target);
	void SetInput(std::string name, int4 data, TargetCore target);
	void SetInput(std::string name, uint4 data, TargetCore target);
	void SetInput(std::string name, short8 data, TargetCore target);
	void SetInput(std::string name, ushort8 data, TargetCore target);
	void SetInput(std::string name, char16 data, TargetCore target);
	void SetInput(std::string name, uchar16 data, TargetCore target);
	void SetInput(std::string name, half8 data, TargetCore target);

	void SetInput(std::string name, float4x4 data, TargetCore target);
	void SetInput(std::string name, int4x4 data, TargetCore target);
	void SetInput(std::string name, uint4x4 data, TargetCore target);
	void SetInput(std::string name, long4x4 data, TargetCore target);
	void SetInput(std::string name, ulong4x4 data, TargetCore target);
	void SetInput(std::string name, short8x8 data, TargetCore target);
	void SetInput(std::string name, ushort8x8 data, TargetCore target);
	void SetInput(std::string name, half8x8 data, TargetCore target);
	
	template<class T>
	void GetOutput(std::string name, TargetCore target, T* value);
    
    void GetOutput(std::string name, TargetCore target, unsigned int size, unsigned char* values);
	void GetOutput(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, unsigned char** output);

	void GetOutput(std::string name, TargetCore target, unsigned int size, unsigned short* value);
	void GetOutput(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, unsigned short** output);

	void GetOutput(std::string name, TargetCore target, unsigned int size, float* value);
	void GetOutput(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, float** output);

	void GetOutput(std::string name, TargetCore target, unsigned int size, half* value);
	void GetOutput(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, half** output);

	void GetOutput(std::string name, TargetCore target, float4* value);
	void GetOutput(std::string name, TargetCore target, int4* value);
	void GetOutput(std::string name, TargetCore target, uint4* value);
	void GetOutput(std::string name, TargetCore target, short8* value);
	void GetOutput(std::string name, TargetCore target, ushort8* value);
	void GetOutput(std::string name, TargetCore target, char16* value);
	void GetOutput(std::string name, TargetCore target, uchar16* value);
	void GetOutput(std::string name, TargetCore target, half8* value);
		
	void GetOutput(std::string name, TargetCore target, float4x4* output);
	void GetOutput(std::string name, TargetCore target, int4x4* output);
	void GetOutput(std::string name, TargetCore target, uint4x4* output);
	void GetOutput(std::string name, TargetCore target, long4x4* output);
	void GetOutput(std::string name, TargetCore target, ulong4x4* output);
	void GetOutput(std::string name, TargetCore target, short8x8* output);
	void GetOutput(std::string name, TargetCore target, ushort8x8* output);
	void GetOutput(std::string name, TargetCore target, half8x8* output);

	template <typename VectorType>
	void GuardInsert(std::string name, TargetCore target, VectorType* value);
	template <typename VectorType>
	void GuardInsert(std::string name, TargetCore target, unsigned int size, VectorType* value);
	template <typename VectorType>
	void GuardInsert(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, VectorType** output);

    template <typename VectorType>
    void GuardCheck(std::string name, TargetCore target, VectorType* value);
    template <typename VectorType>
    void GuardCheck(std::string name, TargetCore target, unsigned int size, VectorType* value);
    template <typename VectorType>
    void GuardCheck(std::string name, TargetCore target, unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, VectorType** output);

	unsigned int GetVariableValue(std::string name);
	static void setDebugOn()
	{
		debugMode = true;
	}
	static void setSrvIP(char *serverIP)
	{
		strcpy(srvIP, serverIP);
	}

	static void setSrvPort(char *serverPort)
	{
		strcpy(srvPort, serverPort);
		defaultPort = false;
	}	
 
	static void setRegistersCheckingOn()
	{
		registersChecking = true;
	}
	
	void Run(TargetCore target);
	void Run(TargetCore target, std::string entryPoint);
private:
	IOProcessing ioProcessor;

	std::string projectPath;
	std::string elfFilePath;
	std::string asmLabel;

	bool outputAvailable;
	bool testInitialized;
	static bool debugMode;
	static bool registersChecking;
	static bool defaultPort;
	static char srvIP[20];
	static char srvPort[10];
		
	std::string getSymbolName(std::string name, std::string symbolPrefix, TargetCore target);
};

template<class T>
void TestRunner::GetOutput(std::string name, TargetCore target, T* value)
{
	std::string shaveName = getSymbolName(name, TARGET_PREFIX, target);
	if(outputAvailable)
	{
		*value = ioProcessor.GetIntOutput(shaveName);
	}
}

#endif /* TESTRUNNER_H_ */
