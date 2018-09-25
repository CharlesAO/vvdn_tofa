///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test defines
///

#ifndef _CNN_FRAME_H_
#define _CNN_FRAME_H_

#include <ImgFrame.h>

#ifndef DESC_SIZE
#define DESC_SIZE 128
#endif

#define FathomStart() rtems_task_wake_after(100)
#define FathomWait()

class CnnFrame: public ImgFrame
{
    public:  //descriptor info
      uint32_t descPos;
      uint8_t  descBuf[DESC_SIZE];

      CnnFrame(){descPos = 0;}

      int DescAdd(uint8_t *data, uint32_t size)
      {
        //TBD: +mutex if if multiple CNNs could add a description
        //      in parallel for same input Frame/roi
          if((descPos + size) < DESC_SIZE){
            memcpy(&descBuf[descPos], data, size);
            descPos += size;
            return 0;//OK
          }
          else return 1;//won't fit
      }
};

typedef PoPtr<CnnFrame> CnnFramePtr;
#endif