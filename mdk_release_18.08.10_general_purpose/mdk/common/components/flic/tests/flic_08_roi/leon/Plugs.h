///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin
///

#ifndef __PLUGS_H__
#define __PLUGS_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>

//################################################
//Types
typedef struct {
  uint32_t x; //top-left-corner .x
  uint32_t y; //top-left-corner .y
  uint32_t w; //width
  uint32_t h; //height
}Roi;

class RoiFrame : public ImgFrame{
  public:
    Roi roi;
};

//################################################
//Plugs
class PlgIsp : public ThreadedPlugin{
  public:
   MReceiver<PoPtr<RoiFrame>> inO; //out frames from pool
   MSender  <PoPtr<RoiFrame>> out; //to FaceDet
   void *threadFunc(void *);
   void Create();
};

class PlgFaceDet : public ThreadedPlugin{
  public:
   SReceiver<PoPtr<RoiFrame>> in; //in frames
   MSender  <PoPtr<RoiFrame>> out;//out ROIs
   void *threadFunc(void *);
   void Create();
};

class PlgOut : public ThreadedPlugin{
  public:
   SReceiver<PoPtr<RoiFrame>> in;//in ROIs
   void *threadFunc(void *);
   void Create();
};

#endif