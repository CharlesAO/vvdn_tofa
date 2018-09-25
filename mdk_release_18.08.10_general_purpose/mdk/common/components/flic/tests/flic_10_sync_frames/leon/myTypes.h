///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test defs
///

#ifndef __MY_TYPES_H__
#define __MY_TYPES_H__

#include <Flic.h>
#include <PoolObj.h>

//test macros
#define TEST_CMD 1234
#define VAL_COCO 0x77
#define VAL_BOBO 0x88

//=============================
class CocoFrame : public PoBuf {
  public: uint32_t uCoco[2];
};

class BoboFrame : public PoBuf {
  public: uint32_t uBobo[4];
};

//=============================
class MegaMsg
{
  public:
    uint32_t         command;
    PoPtr<CocoFrame> fCoco;
    PoPtr<BoboFrame> fBobo;
};


#endif