#ifndef INTERNAL_DEBUG_H_
#define INTERNAL_DEBUG_H_

#include <string>
#include "IOProcessing.h"
#include "half.h"

#ifdef VCS_TESTING_DUMP_ELF
  int saveElf(void);

  //======================================================================================
  //======Because I chose to use template, I need to define function in header============
  //======================================================================================
  template<typename T>
  void writeExpectedOutputBackGeneral(char* name, TargetCore target,
          unsigned int lineDataSize, unsigned int lineWidth, unsigned int lineNo, T** data);

  template<typename T>
  void writeExpectedOutputBackGeneral(char* name, TargetCore target,
          unsigned int size, T* data);

  // INTERFACE
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int lineDataSize, unsigned int lineWidth,
          unsigned int lineNo, unsigned char** data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int lineDataSize, unsigned int lineWidth,
          unsigned int lineNo, unsigned short** data);

  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int lineDataSize, unsigned int lineWidth,
          unsigned int lineNo, unsigned int** data);

  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int lineDataSize, unsigned int lineWidth,
          unsigned int lineNo, half** data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int lineDataSize, unsigned int lineWidth,
          unsigned int lineNo, float** data);
  // 1 line
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, char* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, unsigned short* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, unsigned int* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, half* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, float* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, float4* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, int4* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, uint4* data);
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, short8* data);		  
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, ushort8* data);				  
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, char16* data);		  
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, uchar16* data);	
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, half8* data);	
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, float4x4* data);		  
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, int4x4* data);		  
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, uint4x4* data);		 
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, long4x4* data);	
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, ulong4x4* data);	
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, short8x8* data);	
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, ushort8x8* data);	
  void writeExpectedOutputBack(char* name, TargetCore target,
          unsigned int size, half8x8* data);	
  #define SAVE_ELF_FILE saveElf()
  #define WRITE_AUTOCHECK_OUT(a,b,c,d,e,f) writeExpectedOutputBack(a,b,c,d,e,f)
  #define WRITE_AUTOCHECK_OUT_FRM2(a,b,c,d) writeExpectedOutputBack(a,b,c,d)

  unsigned int getSimbolAdr(std::string name, TargetCore target, std::string prefix);
  void setDatas(unsigned int startAddr, unsigned char * inBuff, unsigned int size);
  void getDatas(unsigned char * outputBuff, unsigned int startAddr,
          unsigned int size);
#else
  unsigned int getSimbolAdr(std::string name, TargetCore target, std::string prefix);
  void setDatas(unsigned int startAddr, unsigned char * inBuff, unsigned int size);
  void getDatas(unsigned char * outputBuff, unsigned int startAddr,
          unsigned int size);
  #define SAVE_ELF_FILE
  #define WRITE_AUTOCHECK_OUT(a,b,c,d,e,f) 
  #define WRITE_AUTOCHECK_OUT_FRM2(a,b,c,d)
  
#endif


#endif /* INTERNAL_DEBUG_H_ */
